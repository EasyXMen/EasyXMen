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
**  FILENAME    : EthIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Yangbo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for EthIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
 * V2.0.0 [2020/7/20] [YangBo] Initial Vertion.
 * V2.0.1 [2020/11/10] [YangBo] QAC fix.
 * V2.0.2 [2020/01/08] [YangBo] Unit test fix.
 * V2.0.3 [2020/01/28] [YangBo] add surport for Eth and EthTrcv multi api.
 * V2.0.4 [2020/03/31] [YangBo] Det method change.
 * V2.0.5 [2022/05/17] [zhiqiang.huang] Add null pointer check for eth trcv API.
 * V2.0.6 [2022/08/02] [zhiqiang.huang] In EthIf_ProvideTxBuffer, check that controlmode is ACTIVE and then provide
 * buffer.
 * V2.0.7 [2023/03/02] [fupeng.yu] Add "Result" parameter to the EthIf_TxConfirmation for Autosar 4.4,R19
 * V2.0.8 [2023/04/07] [fupeng.yu] In EthIf_SetControllerMode, Modify logic when CtrlMode is ETH_MODE_DOWN
 * V2.0.9 [2023/07/31] [rongbo.hu] Modify precompile condition in EthIf_GetTransceiverMode,Resolve compile error.
 * V2.0.10 [2023/08/02] [fupeng.yu] Add ETHIF_ETH_MAIJORVERSION==19u precompile condition for FifoIdx in
 * EthIf_MainFunctionRx. Modify precompile condition in EthIf_TxConfirmation.
 * V2.0.11 [2023/08/28] [fupeng.yu] 1. In EthIf_SetControllerMode, Resolve Incorrect function return value
 *                                  2. In EthIf_MainFunctionState, modify the logic notified to SM when the link state
 *                                     of the same transceiver changes
 *                                  3. Delete EthIf_SearchEthifCrlIndexFmTrcv()
 * V2.0.12 [2023/10/17] [fupeng.yu] 1. Add link state change handling
 *                                  2. In EthIf_RxIndication, delete ethernet time sync frame handling
 *                                  3. Delete EthIf_UpdateTrcvLinkStateToEthSM()
 * V2.0.13 [2023/10/07] [tong.zhao] Adaptation of different AutoSar versions of eth driver, currently supporting
 * versions 4.2.2, 4.3.1, 4.4.0, 4.5.0
 * V2.0.14 [2023/11/14] [fupeng.yu] 1.In EthIf_CtrlModeIndication, add Invalid value handling for transceiver index
 * V2.0.15 [2023/12/12] [fupeng.yu] add switch feature
 * V2.0.16 [2024/03/13] [fupeng.yu] Compatible with Upper Layer defined TxConfirmation callback
 * V2.0.17 [2024/04/25] [fupeng.yu] add the judgment of module initialization fo EthIf_MainFunction<XX>
 * V2.0.18 [2024/08/06] [fupeng.yu] In EthIf_SetCtrlModeDown, update the correct inner state
 * V2.0.19 [2024/08/13] [fupeng.yu] Modify EthIfULTxConfirmationType
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthIf<br>
  RuleSorce:puhua-rule2024-2.rcf 2.3.5

    \li PRQA S 0311 MISRA Rule 11.8 .<br>
    Reason:Necessary format conversion, which needs to be converted to a void pointer for passing.
 */
/*******************************************************************************
**                       Version Information Definitions                       **
*******************************************************************************/
#define ETHIF_C_AR_RELEASE_MAJOR_VERSION    4U
#define ETHIF_C_AR_RELEASE_MINOR_VERSION    5U
#define ETHIF_C_AR_RELEASE_REVISION_VERSION 0U

#define ETHIF_C_SW_MAJOR_VERSION            2U
#define ETHIF_C_SW_MINOR_VERSION            0U
#define ETHIF_C_SW_PATCH_VERSION            19U

/*******************************************************************************
**                               Includes                                     **
*******************************************************************************/
#include "EthIf.h"
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "EthIf_Cbk.h"
#if (STD_ON == ETHIF_USE_ETHTRCV)
#include "EthTrcv.h"
#endif

/*******************************************************************************
**                             Version Check                                  **
*******************************************************************************/
#if (ETHIF_C_AR_RELEASE_MAJOR_VERSION != ETHIF_H_AR_RELEASE_MAJOR_VERSION)
#error "EthIf.c : Mismatch in Specification Major Version"
#endif
#if (ETHIF_C_AR_RELEASE_MINOR_VERSION != ETHIF_H_AR_RELEASE_MINOR_VERSION)
#error "EthIf.c : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_C_AR_RELEASE_REVISION_VERSION != ETHIF_H_AR_RELEASE_REVISION_VERSION)
#error "EthIf.c : Mismatch in Specification Revision Version"
#endif
#if (ETHIF_C_SW_MAJOR_VERSION != ETHIF_H_SW_MAJOR_VERSION)
#error "EthIf.c : Mismatch in Software Major Version"
#endif
#if (ETHIF_C_SW_MINOR_VERSION != ETHIF_H_SW_MINOR_VERSION)
#error "EthIf.c : Mismatch in Software Minor Version"
#endif

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
static inline void ETHIF_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, ApiId, ErrorId);
}
#else
#define ETHIF_DET_REPORT(ApiId, ErrorId)
#endif
#define VLAN_TAG_SIZE                            (uint16)4u
#define PRIORITY_MAX                             (uint8)8u
#define ETH_FRAME_TYPE_VLAN                      (uint16)0x8100
#define INVALID_BUFFER_INDEX                     0xFFFFFFFFu
#define INVALID_UINT8                            0xFFu
#define ETHIF_INVALID_IDX                        0xFFu
#define ETHIF_VID_MASK                           0x0FFFu
#define ETHIF_INVALID_VID                        0x0FFFu

#define ETH_FRAME_TYPE_TIMESYN                   0x88F7u

#define ETHIF_CONTROLLER_REF_HW(CtrlIdx)         EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfRefHardWare
#define ETHIF_CONTROLLER_TRCVID(CtrlIdx)         EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfEthTrcvId
#define ETHIF_TRCV_ETHTRCV(TrcvIdx)              EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->EthIfEthTrcvIdx
#define ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx)      EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->EthTrcvDriverId
#define ETHIF_TRCV_WAKEUPSOURCE(TrcvIdx)         EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->WakeUpSource

#define ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx) EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfPhysControllerIdRef
#define ETHIF_PHYSCONTROLLER_ETHCONTRLLER(PhysIdx) \
    EthIf_ConfigPtr->EthIf_PhysControllerCfg[PhysIdx].EthIfEthCtrlRef->EthIfEthCtrlId
#define ETHIF_PHYSCONTROLLER_ETHDRIVERID(PhysIdx) \
    EthIf_ConfigPtr->EthIf_PhysControllerCfg[PhysIdx].EthIfEthCtrlRef->EthDriverId

#define ETHIF_CONTROLLER_PORTGROUPID(CtrlIdx)    EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfSwitchPortGroupRef
#define ETHIF_CONTROLLER_SWITCHID(CtrlIdx)       EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfSwitchRef
#define ETHIF_ETHSWT_DRIVER_API(driverId)        EthSwt_DriverApi[driverId]
#define ETHIF_GET_ETHIFCTRLRUNMODE(EthIfCtrlIdx) EthIf_Internal.EthIfCtrlRunState[EthIfCtrlIdx].EthIfCtrlMode
/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef struct
{
    uint8 ethifCrlIdx;
    uint32 frameType;
    uint32 bufferIdx;
} EthIfRunTimeType;
typedef struct
{
    Eth_ModeType EthIfTrcvMode;
    EthTrcv_LinkStateType TrcvLinkState;
} EthIf_TrcvStateType;

typedef struct
{
    Eth_ModeType EthIfEthMode;
    uint8 EthIfEthModeCnt;
} EthIf_EthStateType;

typedef struct
{
    Eth_ModeType EthSwtPortMode;
    uint8 EthSwtPortModeCnt;
    uint32 SwitchOffPortTimer;
    EthTrcv_LinkStateType SwtPortLinkState;
} EthIf_EthSwtPortStateType;

typedef struct
{
    Eth_ModeType EthIfCtrlMode;
    EthTrcv_LinkStateType TrcvLinkState;
    Eth_ModeType EthMode;
#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
    boolean RequstTxProvider;
    Eth_BufIdxType RequestBuffId;
    EthSwt_MgmtInfoType SwtMgmtInfo;
#endif /* STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT */
} EthIf_CtrlStateType;

typedef struct
{
    EthIf_StateType initStatus; /* var to hold EthIf module status */
    /* ETHIF_MAX_FRAME_OWNER_CFG Mapping from buffer index to Frame type*/
    EthIfRunTimeType ethIfRunTime[ETHIF_MAX_FRAME_OWNER_CFG];
    EthIf_CtrlStateType EthIfCtrlRunState[ETHIF_CTRL_NUM];
#if (STD_ON == ETHIF_USE_ETHTRCV)
    EthIf_TrcvStateType EthIfTrcvRunState[ETHIF_ETHTRCV_NUM];
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    EthIf_EthStateType EthIfEthRunState[ETHIF_ETH_NUM];
#if (STD_ON == ETHIF_USE_ETHSWT)
    EthIf_EthSwtPortStateType EthIfEthPortRunState[ETHIF_ETHSWTPORT_NUM];
#endif /* TD_ON == ETHIF_USE_ETHSWT */
} EthIf_InternalType;

#if (0u < ETHIF_ETHSWTPORT_NUM)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static EthIf_SignalQualityResultType EthIf_PortSignalQuality[ETHIF_ETHSWTPORT_NUM];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif /* 0u < ETHIF_ETHSWTPORT_NUM */

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static EthIf_InternalType EthIf_Internal = {
    .initStatus = ETHIF_STATE_UNINIT,

};
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_VAR) EthIf_ConfigPtr = NULL_PTR;
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static Std_ReturnType EthIf_SearchEthifCrlIndexFmVlan(uint8* EthifCtrlId, uint8 EthCtrlId, uint16 VlanId);
static Std_ReturnType EthIf_SearchEthifCrlIndex(uint8* EthifCtrlId, uint8 EthCtrlId);
static void EthIf_InnerSaveCtrlMode(uint8 EthIfCtrlId, Eth_ModeType CtrlMode);
static void EthIf_LinkStateChangeHandle(void);
static Std_ReturnType EthIf_SetCtrlModeActive(uint8 EthIfCtrlIdx, Eth_ModeType CtrlMode, uint8 EthCtrlIdx);
static Std_ReturnType EthIf_SetCtrlModeDown(uint8 EthIfCtrlIdx, Eth_ModeType CtrlMode, uint8 EthCtrlIdx);

#if (STD_ON == ETHIF_USE_ETHSWT)
static Std_ReturnType EthIf_SetEthSwtPortMode(
    const EthIf_PortGroupType* PortGroupCfgPtr,
    EthIf_PortGroupSemanticType PortGroupSemantic,
    Eth_ModeType CtrlMode,
    boolean PendingDown);

static Std_ReturnType EthIf_FindPortGroupId(EthIf_SwitchPortGroupIdxType PortGroupId, uint8* EthIfCtrlId);
static void EthIf_SetSwitchOffPortTimer(const EthIf_PortGroupType* PortGroupPtr, uint32 TimeValue);
static void EthIf_SwtichPortStateHandle(void);
static Std_ReturnType EthIf_SetHwSwtPortMode(uint8 PortIdx, Eth_ModeType CtrlMode);
static void EthIf_GetLinkStateChangeWithoutPortGroups(uint8 EthIfCtrlIdx, EthTrcv_LinkStateType* SwtPortLinkState);

#if (0u < ETHIF_ETHSWTPORTGROUP_NUM)
static void EthIf_GetLinkStateChangeWithPortGroups(uint8 EthIfCtrlIdx, EthTrcv_LinkStateType* SwtPortLinkState);
#endif /* 0u < ETHIF_ETHSWTPORTGROUP_NUM */
#endif /* STD_ON == ETHIF_USE_ETHSWT */

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/**
 * Initializes the Ethernet Interface.
 * Service ID: 0x01
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CfgPtr:Points to the implementation specific structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_Init(P2CONST(EthIf_ConfigType, ETHIF_APPL_DATA, ETHIF_VAR) CfgPtr)
{
    uint8 index;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_INIT, ETHIF_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        EthIf_ConfigPtr = CfgPtr;
        for (index = 0; index < EthIf_ConfigPtr->EthIfOwnersCount; index++)
        {
            EthIf_Internal.ethIfRunTime[index].frameType = EthIf_ConfigPtr->EthIfOwnerCfg[index].EthIfFrameType;
            EthIf_Internal.ethIfRunTime[index].bufferIdx = INVALID_BUFFER_INDEX;
            EthIf_Internal.ethIfRunTime[index].ethifCrlIdx = INVALID_UINT8;
        }
        for (index = 0; index < EthIf_ConfigPtr->EthIfCtrlCount; index++)
        {
            EthIf_Internal.EthIfCtrlRunState[index].EthIfCtrlMode = ETH_MODE_DOWN;
            EthIf_Internal.EthIfCtrlRunState[index].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
        }
#if (STD_ON == ETHIF_USE_ETHTRCV)
        for (index = 0; index < EthIf_ConfigPtr->EthIfTrcvCount; index++)
        {
            EthIf_Internal.EthIfTrcvRunState[index].EthIfTrcvMode = ETH_MODE_DOWN;
            EthIf_Internal.EthIfTrcvRunState[index].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */

        for (index = 0; index < EthIf_ConfigPtr->EthIfPhysControllerCount; index++)
        {
            EthIf_Internal.EthIfEthRunState[index].EthIfEthMode = ETH_MODE_DOWN;
            EthIf_Internal.EthIfEthRunState[index].EthIfEthModeCnt = 0u;
        }

#if (STD_ON == ETHIF_USE_ETHSWT)
        for (index = 0; index < EthIf_ConfigPtr->EthIfEthSwitchPortCount; index++)
        {
            EthIf_Internal.EthIfEthPortRunState[index].EthSwtPortMode = ETH_MODE_DOWN;
            EthIf_Internal.EthIfEthPortRunState[index].EthSwtPortModeCnt = 0u;
            EthIf_Internal.EthIfEthPortRunState[index].SwitchOffPortTimer = 0u;
            EthIf_Internal.EthIfEthPortRunState[index].SwtPortLinkState = ETHTRCV_LINK_STATE_DOWN;
        }
#endif /* STD_ON == ETHIF_USE_ETHSWT */
        EthIf_Internal.initStatus = ETHIF_STATE_INIT;
    }
}
/**
 * Initializes the Ethernet Interface.
 * Service ID: 0x0b
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): VersionInfoPtr:Version information of this module
 * Return value: NA
 *
 */
#if (ETHIF_VERSION_INFO_API == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        /*report development error*/
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VERSION_INFO, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        VersionInfoPtr->moduleID = ETHIF_MODULE_ID;
        VersionInfoPtr->sw_major_version = ETHIF_C_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = ETHIF_C_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = ETHIF_C_SW_PATCH_VERSION;
        VersionInfoPtr->vendorID = ETHIF_VENDOR_ID;
    }
}
#endif
/**
 * Enables / disables the indexed controller.
 * Service ID: 0x03
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 CtrlMode:ETH_MODE_DOWN: disable the controller ETH_MODE_ACTIVE: enable the controller
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: controller mode could not be changed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetControllerMode(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 ethContrllerId;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        if ((ETH_MODE_ACTIVE == CtrlMode) && (ETH_MODE_DOWN == ETHIF_GET_ETHIFCTRLRUNMODE(CtrlIdx)))
        {
            ret = EthIf_SetCtrlModeActive(CtrlIdx, CtrlMode, ethContrllerId);
        }
        else if ((ETH_MODE_DOWN == CtrlMode) && (ETH_MODE_ACTIVE == ETHIF_GET_ETHIFCTRLRUNMODE(CtrlIdx)))
        {
            ret = EthIf_SetCtrlModeDown(CtrlIdx, CtrlMode, ethContrllerId);
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}
/**
 * Obtains the state of the indexed controller.
 * Service ID: 0x04
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): CtrlModePtr:ETH_MODE_DOWN: the controller is disabled ETH_MODE_ACTIVE: the controller is enabled
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: controller could not be initialized
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetControllerMode(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) CtrlModePtr)
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE, ETHIF_E_INV_CTRL_IDX);
    }
    else if (CtrlModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = Eth_DriverApi[driverId].Eth_GetControllerMode(ethContrllerId, CtrlModePtr);
    }
    return ret;
}
/**
 * Enables / disables the wake up mode or clear the wake-up reason of the
 * indexed transceiver
 * Service ID: 0x2e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): TrcvIdx:Index of the transceiver within the context of the
 * Ethernet Interface
 *                 TrcvWakeupMode:ETHTRCV_WUM_DISABLE: disable transceiver wake
 * up ETHTRCV_WUM_ENABLE: enable transceiver wake up ETHTRCV_WUM_CLEAR: clears
 * transceiver wake up reason
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: success E_NOT_OK: transceiver wake up could not be
 * changed or wake-up reason could not be cleared
 *
 */
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
#if (STD_ON == ETHIF_USE_ETHTRCV)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetTransceiverWakeupMode(VAR(uint8, ETHIF_VAR) TrcvIdx, VAR(EthTrcv_WakeupModeType, ETHIF_VAR) TrcvWakeupMode)
{
    Std_ReturnType ret;
    EthTrcv_WakeupModeType currentTrcvWakeupMode;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_TRCV_WAKEUP_MODE, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_TRCV_WAKEUP_MODE, ETHIF_E_INV_TRCV_IDX);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
        ret = EthIf_GetTransceiverWakeupMode(ethTrcvIdx, &currentTrcvWakeupMode);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if (E_OK == ret)
        {
            if (((ETHTRCV_WUM_ENABLE == TrcvWakeupMode) && (ETHTRCV_WUM_DISABLE == currentTrcvWakeupMode))
                || ((ETHTRCV_WUM_DISABLE == TrcvWakeupMode) && (ETHTRCV_WUM_ENABLE == currentTrcvWakeupMode))
                || ((ETHTRCV_WUM_CLEAR == TrcvWakeupMode) && (ETHTRCV_WUM_DISABLE == currentTrcvWakeupMode)))
            {
                if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_SetTransceiverWakeupMode)
                {
                    ret = EthTrcv_DriverApi[trcvDriverId].EthTrcv_SetTransceiverWakeupMode(ethTrcvIdx, TrcvWakeupMode);
                }
            }
        }
#endif /* TD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
/**
 * Returns the wake up mode of the indexed transceiver
 * Service ID: 0x2f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): TrcvIdx:Index of the transceiver within the context of the
 * Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvWakeupModePtr:ETHTRCV_WUM_DISABLE: transceiver wake up
 * is disabled ETHTRCV_WUM_ENABLE: transceiver wake up is enabled
 * Return value: E_OK: transceiver wake up mode could not be obtained,E_NOT_OK: transceiver wake up mode could not be
 * obtained
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTransceiverWakeupMode(
    VAR(uint8, ETHIF_VAR) TrcvIdx,
    P2VAR(EthTrcv_WakeupModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvWakeupModePtr)
{
    Std_ReturnType ret;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE, ETHIF_E_INV_TRCV_IDX);
    }
    else if (TrcvWakeupModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetTransceiverWakeupMode)
        {
            ret = EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetTransceiverWakeupMode(ethTrcvIdx, TrcvWakeupModePtr);
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif
/**
 * Service is called by integration code to check a wakeup source.
 * Service ID: 0x30
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WakeupSource:source (transceiver) which initiated the wake
 * up event
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: when function has been successfully executed,E_NOT_OK:
 * when function could not be successfully executed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_CheckWakeup(VAR(EcuM_WakeupSourceType, ETHIF_VAR) WakeupSource)
{
    Std_ReturnType ret;
#if (STD_ON == ETHIF_USE_ETHTRCV)
    EcuM_WakeupSourceType trcvWakeupSource;
    uint8 trcvIdx;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CHECK_WAKE_UP, ETHIF_E_UNINIT);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
#if (STD_ON == ETHIF_USE_ETHTRCV)
        for (trcvIdx = 0; trcvIdx < EthIf_ConfigPtr->EthIfTrcvCount; trcvIdx++)
        {
            trcvWakeupSource = ETHIF_TRCV_WAKEUPSOURCE(trcvIdx);
            if (0u != (WakeupSource & trcvWakeupSource))
            {
                ethTrcvIdx = ETHIF_TRCV_ETHTRCV(trcvIdx);
                trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvIdx);
                if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_CheckWakeup)
                {
                    ret = EthTrcv_DriverApi[trcvDriverId].EthTrcv_CheckWakeup(ethTrcvIdx);
                }
                break;
            }
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif

#if (STD_ON == ETHIF_USE_ETHTRCV)
/**
 * Obtain state of the indexed transceiver
 * Service ID: 0x4A
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): TrcvIdx:Index of the transceiver within the context of the Ethernet Transceiver Driver
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvModePtr:ETH_MODE_DOWN: the transceiver is disabled
 *                   ETH_MODE_ACTIVE: the transceiver is enabled
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: transceiver mode could not be obtained
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTransceiverMode(VAR(uint8, ETHIF_VAR) TrcvIdx, P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvModePtr)
{
    Std_ReturnType ret;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE, ETHIF_E_INV_TRCV_IDX);
    }
    else if (TrcvModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetTransceiverMode)
        {
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450)
            ret = EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetTransceiverMode(ethTrcvIdx, TrcvModePtr);
#else
            EthTrcv_ModeType trcvMode;
            ret = EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetTransceiverMode(ethTrcvIdx, &trcvMode);
            if (E_OK == ret)
            {
                *TrcvModePtr = (trcvMode == ETHTRCV_MODE_DOWN) ? (ETH_MODE_DOWN) : (ETH_MODE_ACTIVE);
            }
#endif /* ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450 */
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif
/**
 * Provides access to a transmit buffer of the specified Ethernet controller.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType:Ethernet Frame Type (EtherType)
 *                 Priority:Priority value which shall be used for the 3-bit PCP field of the VLAN tag
 * Parameters(INOUT): LenBytePtr:(in: desired length in bytes, out: granted length in bytes)
 * Parameters(OUT): BufIdxPtr:Index to the granted buffer resource. To be used for subsequent requests
 *                  BufPtr:Pointer to the granted buffer
 * Return value: BufReq_ReturnType:(BUFREQ_OK: success BUFREQ_E_NOT_OK: development error detected
 *                                  BUFREQ_E_BUSY: all buffers in use)
 *
 */
FUNC(BufReq_ReturnType, ETHIF_CODE)
EthIf_ProvideTxBuffer(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(uint8, ETHIF_VAR) Priority,
    P2VAR(Eth_BufIdxType, ETHIF_APPL_DATA, ETHIF_VAR) BufIdxPtr,
    P2VAR(uint8*, ETHIF_APPL_DATA, ETHIF_VAR) BufPtr,
    P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) LenBytePtr)
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    uint8 driverId;
    uint8 ethContrllerId;
    Eth_ModeType ethIfCtrlMode;
#if (ETHIF_VLAN_USED == STD_ON)
    uint8* IntenalBuf;
#endif /* ETHIF_VLAN_USED == STD_ON */

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER, ETHIF_E_INV_CTRL_IDX);
    }
    else if ((BufIdxPtr == NULL_PTR) || (BufPtr == NULL_PTR) || (LenBytePtr == NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethIfCtrlMode = ETHIF_GET_ETHIFCTRLRUNMODE(CtrlIdx);
        if (ETH_MODE_ACTIVE == ethIfCtrlMode)
        {
            SchM_Enter_EthIf_ExclusiveArea();
#if (ETHIF_VLAN_USED != STD_ON)
            if ((EthIf_ConfigPtr->EthIfCtrlCfg->EthIfCtrlMtu) >= *LenBytePtr)
            {
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                ret = Eth_DriverApi[driverId]
                          .Eth_ProvideTxBuffer(ethContrllerId, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#else  /*The default version of eth driver is 4.2.2*/
                ret = Eth_DriverApi[driverId].Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
            }
            else
            {
                ret = BUFREQ_E_NOT_OK;
            }
#else /* ETHIF_VLAN_USED == STD_ON */
            if (((EthIf_ConfigPtr->EthIfCtrlCfg->EthIfCtrlMtu) >= *LenBytePtr) && (Priority < PRIORITY_MAX))
            {
                *LenBytePtr = (*LenBytePtr + VLAN_TAG_SIZE);
                /*@req SWS_EthIf_00068 */
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                ret = Eth_DriverApi[driverId]
                          .Eth_ProvideTxBuffer(ethContrllerId, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#else  /*The default version of eth driver is 4.2.2*/
                ret = Eth_DriverApi[driverId].Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
                IntenalBuf = *BufPtr;
                if (BUFREQ_OK == ret)
                {
                    IntenalBuf[0] = (uint8)(Priority << 5u);
                    IntenalBuf[0] &= (uint8)((~((uint8)1) << 4u));
                    IntenalBuf[0] |= (uint8)((EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId & 0x0F00u) >> 8u);
                    IntenalBuf[1] = (uint8)(EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId & 0x00FFu);
                    IntenalBuf[2] = (uint8)((FrameType >> 8u) & 0xFFu);
                    IntenalBuf[3] = (uint8)(FrameType & 0xFFu);
                    *BufPtr = &IntenalBuf[VLAN_TAG_SIZE];
                    *LenBytePtr = (*LenBytePtr - VLAN_TAG_SIZE);
                }
                else
                {
                    ret = BUFREQ_E_NOT_OK;
                }
            }
#endif /* ETHIF_VLAN_USED != STD_ON */
            SchM_Exit_EthIf_ExclusiveArea();
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }

#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
        if (BUFREQ_OK == ret)
        {
            EthIf_Internal.EthIfCtrlRunState[CtrlIdx].RequstTxProvider = TRUE;
            EthIf_Internal.EthIfCtrlRunState[CtrlIdx].RequestBuffId = *BufIdxPtr;
        }
#endif /* STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT */
    }
    (void)Priority;
    (void)FrameType;
    return ret;
}

/**
 * Triggers transmission of a previously filled transmit buffer
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType:Ethernet frame type
 *                 TxConfirmation:Activates transmission confirmation
 *                 PhysAddrPtr:Physical target address (MAC address) in network byte order
 * Parameters(INOUT): LenByte:Data length in byte
 * Parameters(OUT): BufIdx,Index of the buffer resource
 * Return value: Std_ReturnType,E_OK: success E_NOT_OK: transmission failed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_Transmit(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(boolean, ETHIF_VAR) TxConfirmation,
    VAR(uint16, ETHIF_VAR) LenByte,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr)
{
    Std_ReturnType ret;
    uint8 i;
    uint8 driverId;
    uint8 ethContrllerId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT, ETHIF_E_INV_CTRL_IDX);
    }
    else if (BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT, ETHIF_E_INV_PARAM);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        SchM_Enter_EthIf_ExclusiveArea();
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        if (EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfCtrlMtu >= LenByte)
        {
#if (ETHIF_VLAN_USED == STD_ON)
            {
                ret = Eth_DriverApi[driverId].Eth_Transmit(
                    ethContrllerId,
                    BufIdx,
                    ETH_FRAME_TYPE_VLAN,
                    TxConfirmation,
                    (LenByte + VLAN_TAG_SIZE),
                    PhysAddrPtr);
            }
#else  /* ETHIF_VLAN_USED != STD_ON */
            {
                ret = Eth_DriverApi[driverId]
                          .Eth_Transmit(ethContrllerId, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr);
            }
#endif /* ETHIF_VLAN_USED == STD_ON */
            if ((E_OK == ret) && (EthIf_ConfigPtr->EthIfULTxConfirmation != NULL_PTR))
            {
                for (i = 0; i < EthIf_ConfigPtr->EthIfOwnersCount; i++)
                {
                    if ((FrameType == EthIf_Internal.ethIfRunTime[i].frameType))
                    {
                        if (TRUE == TxConfirmation)
                        {
                            EthIf_Internal.ethIfRunTime[i].bufferIdx = BufIdx;
                            EthIf_Internal.ethIfRunTime[i].ethifCrlIdx = CtrlIdx;
                        }
                        else
                        {
                            EthIf_Internal.ethIfRunTime[i].bufferIdx = INVALID_BUFFER_INDEX;
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
        SchM_Exit_EthIf_ExclusiveArea();
    }
    return ret;
}
/**
 * Confirms frame transmission by the indexed controller
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 BufIdx:Index of the transmitted buffer
 *                 Result:E_OK: The transmission was successful, E_NOT_OK: The transmission failed
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
FUNC(void, ETHIF_CODE)
EthIf_TxConfirmation(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    VAR(Std_ReturnType, ETHIF_VAR) Result)
#else  /*The default version of eth driver is 4.2.2*/
FUNC(void, ETHIF_CODE)
EthIf_TxConfirmation(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx)
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
{
    uint8 i;
    uint8 index;
    uint8 ethifCrlId;
    uint8 handle;
    Std_ReturnType ret;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, ETHIF_E_INV_CTRL_IDX);
    }
    else if (BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, ETHIF_E_INV_PARAM);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        if (NULL_PTR != EthIf_ConfigPtr->EthIfULTxConfirmation)
        {
            for (i = 0; i < EthIf_ConfigPtr->EthIfOwnersCount; i++)
            {
                if (BufIdx == EthIf_Internal.ethIfRunTime[i].bufferIdx)
                {
                    for (index = 0; index < EthIf_ConfigPtr->EthIfCtrlCount; index++)
                    {
                        if ((ETHIF_CONTROLLER_PHYSCONTROLLER(index) == CtrlIdx)
                            && (index == EthIf_Internal.ethIfRunTime[i].ethifCrlIdx))
                        {
                            ethifCrlId = index;
                            ret = E_OK;
                            break;
                        }
                    }
                    if (ret == E_OK)
                    {
                        handle = EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfOwner;
                        if (NULL_PTR != EthIf_ConfigPtr->EthIfULTxConfirmation[handle])
                        {
#if (ETHIF_ETH_AUTOSAR_VERSION < ETHIF_ETH_AUTOSAR_431)
                            Std_ReturnType Result = E_OK;
#endif /* ETHIF_ETH_AUTOSAR_VERSION < ETHIF_ETH_AUTOSAR_431 */
                            EthIf_ConfigPtr->EthIfULTxConfirmation[handle](ethifCrlId, BufIdx, Result);
                        }
                    }
                    break;
                }
            }
        }
    }
}
/**
 * Handles a received frame received by the indexed controller
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType,Frame type of received Ethernet frame
 *                 IsBroadcast,parameter to indicate a broadcast frame
 *                 PhysAddrPtr,Pointer to Physical source address (MAC address in network byte order) of
 *                 received Ethernet frame DataPtr,Pointer to payload of received Ethernet frame.
 *                 LenByte,Length of the received frame bytes
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE)
EthIf_RxIndication(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(boolean, ETHIF_VAR) IsBroadcast,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr,
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    VAR(uint16, ETHIF_VAR) LenByte)
{
    void (*upperLayerFun)(
        uint8 CtrlIdx,
        Eth_FrameType FrameType,
        boolean IsBroadcast,
        const uint8* PhysAddrPtr,
        const uint8* DataPtr,
        uint16 LenByte);
    Eth_FrameType UpperLayerFrameType;
    uint16 UpperLayerLenByte;
#if (ETHIF_VLAN_USED == STD_ON)
    boolean status = FALSE;
    uint16 tci = 0u;
    uint16 ctrlVlanId;
#endif /* ETHIF_VLAN_USED == STD_ON */
    uint8 i;
    uint8 handle;
    uint8 ethifCtrlId = 0u;
    Std_ReturnType ret = E_NOT_OK;
    uint16 vlanId = ETHIF_INVALID_VID;
    const uint8* UpperLayerDataPtr;
    const uint8* u8DataPtr = (const uint8*)DataPtr;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, ETHIF_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == u8DataPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
#if (ETHIF_VLAN_USED == STD_ON)
        if (FrameType == ETH_FRAME_TYPE_VLAN)
        {
            tci |= (uint16)u8DataPtr[0] << 8u;
            tci |= (uint16)u8DataPtr[1];
            vlanId = tci & ETHIF_VID_MASK;
        }
#endif /* ETHIF_VLAN_USED == STD_ON */
        ret = EthIf_SearchEthifCrlIndexFmVlan(&ethifCtrlId, CtrlIdx, vlanId);
    }
    if (E_NOT_OK != ret)
    {
#if (ETHIF_VLAN_USED != STD_ON)
        {
            UpperLayerFrameType = FrameType;
            UpperLayerDataPtr = u8DataPtr;
            UpperLayerLenByte = LenByte;
        }
#else  /* ETHIF_VLAN_USED == STD_ON */
        {
            ctrlVlanId = EthIf_ConfigPtr->EthIfCtrlCfg[ethifCtrlId].EthIfVlanId;
            if (FrameType == ETH_FRAME_TYPE_VLAN)
            {
                if (vlanId == ctrlVlanId)
                {
                    status = TRUE;
                    UpperLayerFrameType = 0u;
                    UpperLayerDataPtr = &u8DataPtr[VLAN_TAG_SIZE];
                    UpperLayerLenByte = LenByte - VLAN_TAG_SIZE;
                    UpperLayerFrameType |= (Eth_FrameType)u8DataPtr[2] << 8u;
                    UpperLayerFrameType |= (Eth_FrameType)u8DataPtr[3];
                }
            }
            else
            {
                /* untagged VLAN */
                if (ETHIF_VID_UNUSED == ctrlVlanId)
                {
                    status = TRUE;
                    UpperLayerFrameType = FrameType;
                    UpperLayerDataPtr = u8DataPtr;
                    UpperLayerLenByte = LenByte;
                }
            }
        }
        if (status == TRUE)
#endif /* ETHIF_VLAN_USED != STD_ON */
        {
            for (i = 0u; i < EthIf_ConfigPtr->EthIfOwnersCount; i++)
            {
                if (UpperLayerFrameType == EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfFrameType)
                {
                    handle = EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfOwner;
                    if ((NULL_PTR != EthIf_ConfigPtr->EthIfULRxIndication)
                        && (NULL_PTR != EthIf_ConfigPtr->EthIfULRxIndication[handle]))
                    {
                        upperLayerFun = EthIf_ConfigPtr->EthIfULRxIndication[handle];
                        (*upperLayerFun)(
                            ethifCtrlId,
                            UpperLayerFrameType,
                            IsBroadcast,
                            PhysAddrPtr,
                            (const uint8*)UpperLayerDataPtr,
                            UpperLayerLenByte);
                    }
                }
            }
        }
    }
}
/**
 * Sets the physical source address used by the indexed controller.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 PhysAddrPtr,Pointer to Physical source address (MAC address in network byte order) of received
 * Ethernet frame Parameters(INOUT): NA Parameters(OUT): NA Return value: NA
 *
 */
FUNC(void, ETHIF_CODE)
EthIf_SetPhysAddr(VAR(uint8, ETHIF_VAR) CtrlIdx, P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr)
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR, ETHIF_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        (void)Eth_DriverApi[driverId].Eth_SetPhysAddr(ethContrllerId, PhysAddrPtr);
    }
}
/**
 * Obtains the physical source address used by the indexed controller
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): PhysAddrPtr,Physical source address (MAC address) in network byte order.
 * Return value: NA
 */
FUNC(void, ETHIF_CODE)
EthIf_GetPhysAddr(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr)
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR, ETHIF_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        (void)Eth_DriverApi[driverId].Eth_GetPhysAddr(ethContrllerId, PhysAddrPtr);
    }
}
/**
 * Update the physical source address to/from the indexed controller filter. If the Ethernet Controller is not capable
 * to do the filtering, the software has to do this. Service ID: 0x0c Sync/Async: Synchronous Reentrancy: Non Reentrant
 * for the same CtrlIdx, reentrant for different Parameters(IN): CtrlIdx,Index of the Ethernet controller within the
 * context of the Ethernet Interface PhysAddrPtr,Physical source address (MAC address) in network byte order. Action,Add
 * or remove the address from the Ethernet controllers filter. Parameters(INOUT): NA Parameters(OUT): NA Return value:
 * Std_ReturnType,E_OK: filter was successfully changed E_NOT_OK: filter could not be changed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_UpdatePhysAddrFilter(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr,
    VAR(Eth_FilterActionType, ETHIF_VAR) Action)
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;
    ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER, ETHIF_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = Eth_DriverApi[driverId].Eth_UpdatePhysAddrFilter(ethContrllerId, PhysAddrPtr, Action);
    }
    return ret;
}

#if (ETHIF_GLOBAL_TIME_SUPPORT == STD_ON)
/**
 * Returns a time value out of the HW registers according to the capability of the HW. Is the HW resolution is lower
 * than the Eth_TimeStampType resolution resp. range, than an the remaining bits will be filled with 0. Service ID: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): timeQualPtr,quality of HW time stamp, e.g. based on current drift
 *                  timeStampPtr,current time stamp
 * Return value: Std_ReturnType,E_OK: successful E_NOT_OK: failed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetCurrentTime(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr)
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;
    ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME, ETHIF_E_INV_CTRL_IDX);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = Eth_DriverApi[driverId].Eth_GetCurrentTime(ethContrllerId, timeQualPtr, timeStampPtr);
    }
    return ret;
}

/**
 * Activates egress time stamping on a dedicated message object. Some HW does store once the egress time stamp marker
 * and some HW needs it always before transmission. There will be no functionality, due to the fact, that the message
 * type is always "time stamped" by network design. Service ID: 0x23 Sync/Async: Synchronous Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 BufIdx,Index of the message buffer, where Application expects egress time stamping
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE)
EthIf_EnableEgressTimeStamp(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx)
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
        (void)Eth_DriverApi[driverId].Eth_EnableEgressTimeStamp(ethContrllerId, BufIdx);
#else  /*The default version of eth driver is 4.2.2*/
        (void)Eth_DriverApi[driverId].Eth_EnableEgressTimeStamp(ethContrllerId, (uint8)BufIdx);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
    }
}

/**
 * Reads back the egress time stamp on a dedicated message object. It must be called within the TxConfirmation()
 * function. Service ID: 0x24 Sync/Async: Synchronous Reentrancy: Non Reentrant Parameters(IN): CtrlIdx,Index of the
 * Ethernet controller within the context of the Ethernet Interface BufIdx,Index of the message buffer, where
 * Application expects egress time stamping Parameters(INOUT): NA Parameters(OUT): timeQualPtr,quality of HW time stamp,
 * e.g. based on current drift timeStampPtr,current time stamp Return value: NA
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetEgressTimeStamp(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr)
{
    Std_ReturnType ret = E_OK;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP, ETHIF_E_INV_CTRL_IDX);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
        ret = Eth_DriverApi[driverId].Eth_GetEgressTimeStamp(ethContrllerId, BufIdx, timeQualPtr, timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
        Eth_DriverApi[driverId].Eth_GetEgressTimeStamp(ethContrllerId, BufIdx, timeQualPtr, timeStampPtr);
#else  /*The default version of eth driver is 4.2.2*/
        Eth_DriverApi[driverId].Eth_GetEgressTimeStamp(ethContrllerId, (uint8)BufIdx, timeQualPtr, timeStampPtr);
#endif /*ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431*/
    }
    return ret;
}
/**
 * Reads back the ingress time stamp on a dedicated message object. It must be called within the RxIndication()
 * function. Service ID: 0x25 Sync/Async: Synchronous Reentrancy: Non Reentrant Parameters(IN): CtrlIdx,Index of the
 * Ethernet controller within the context of the Ethernet Interface DataPtr,Pointer to the message buffer, where
 * Application expects ingress time stamping Parameters(INOUT): NA Parameters(OUT): timeQualPtr,quality of HW time
 * stamp, e.g. based on current drift timeStampPtr,current time stamp Return value: NA
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetIngressTimeStamp(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr,
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr)
{
    Std_ReturnType ret = E_OK;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP, ETHIF_E_INV_CTRL_IDX);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr) || (NULL_PTR == DataPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
        ret = Eth_DriverApi[driverId].Eth_GetIngressTimeStamp(ethContrllerId, DataPtr, timeQualPtr, timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
        Eth_DriverApi[driverId].Eth_GetIngressTimeStamp(ethContrllerId, DataPtr, timeQualPtr, timeStampPtr);
#else  /*The default version of eth driver is 4.2.2*/
        /* PRQA S 0311 ++ */ /* MISRA Rule 11.8 */
        Eth_DriverApi[driverId]
            .Eth_GetIngressTimeStamp(ethContrllerId, (Eth_DataType*)DataPtr, timeQualPtr, timeStampPtr);
        /* PRQA S 0311 -- */ /* MISRA Rule 11.8 */
#endif /* ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431 */
    }
    return ret;
}
#endif /* ETHIF_GLOBAL_TIME_SUPPORT == STD_ON */

#if (ETHIF_ENABLE_RX_INTERRUPT == STD_OFF)
/**
 * The function checks for new received frames and issues transmission confirmations in
 *  polling mode. It checks also for transceiver state changes.
 * Service ID: 0x20
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx(void)
{
    if (EthIf_Internal.initStatus == ETHIF_STATE_INIT)
    {
        Eth_RxStatusType RxStatusPtr = ETH_NOT_RECEIVED;
        uint8 i;
        uint8 CtrlIdx;
        uint8 driverId;
        uint8 ethContrllerId;
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
        uint8 FifoIdx;
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_RX, ETHIF_E_UNINIT);
        }
        else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
        {
            for (CtrlIdx = 0u; CtrlIdx < ETHIF_CTRL_NUM; CtrlIdx++)
            {
                driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
                ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                (void)Eth_DriverApi[driverId].Eth_Receive(ethContrllerId, FifoIdx, &RxStatusPtr); /*FifoIdx TODO*/
#else  /*The default version of eth driver is 4.2.2*/
                (void)Eth_DriverApi[driverId].Eth_Receive(ethContrllerId, &RxStatusPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */

                for (i = 0; i < ETHIF_RX_INDICATION_ITERATIONS_MAX; i++)
                {
                    if (RxStatusPtr == ETH_RECEIVED_MORE_DATA_AVAILABLE)
                    {
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                        (void)Eth_DriverApi[driverId].Eth_Receive(
                            ethContrllerId,
                            FifoIdx,
                            &RxStatusPtr); /*FifoIdx TODO*/
#else
                        /*The default version of eth driver is 4.2.2*/
                        (void)Eth_DriverApi[driverId].Eth_Receive(ethContrllerId, &RxStatusPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}
#endif /*ETHIF_ENABLE_RX_INTERRUPT == STD_OFF*/

#if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
/**
 * The function issues transmission confirmations in polling mode.
 *  It checks also for transceiver state changes.
 * Service ID: 0x21
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx(void)
{
    if (EthIf_Internal.initStatus == ETHIF_STATE_INIT)
    {
        uint8 CtrlIdx;
        uint8 driverId;
        uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_TX, ETHIF_E_UNINIT);
        }
        else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
        {
            for (CtrlIdx = 0; CtrlIdx < ETHIF_CTRL_NUM; CtrlIdx++)
            {
                driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
                ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
                (void)Eth_DriverApi[driverId].Eth_TxConfirmation(ethContrllerId);
            }
        }
    }
}

#endif

/**
 * The function is polling different communication hardware (Ethernet
 * transceiver, Ethernet switch ports) related information, e.g. link state,
 * signal quality.
 * Service ID: 0x05
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionState(void)
{
    if (EthIf_Internal.initStatus == ETHIF_STATE_INIT)
    {
#if (STD_ON == ETHIF_USE_ETHTRCV)
        uint8 trcvlIdx;
        uint8 trcvDriverId;
        EthTrcv_LinkStateType linkState;
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
#if (ETHIF_VLAN_USED == STD_ON)
        uint8 EthIfCtrlIdx;
        uint8 driverId;
        uint8 physCtrlIdx;
        Eth_ModeType curCtrlMode;
        Std_ReturnType ret;
        uint8 ethContrllerId;
        boolean modeCtrl = TRUE;
#endif /* ETHIF_VLAN_USED == STD_ON */
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_STATE, ETHIF_E_UNINIT);
        }
        else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
        {
#if (STD_ON == ETHIF_USE_ETHTRCV)
            for (trcvlIdx = 0; trcvlIdx < EthIf_ConfigPtr->EthIfTrcvCount; trcvlIdx++)
            {
                if (EthIf_Internal.EthIfTrcvRunState[trcvlIdx].EthIfTrcvMode == ETH_MODE_ACTIVE)
                {
                    trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvlIdx);
                    if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetLinkState)
                    {
                        if (E_OK
                            == EthTrcv_DriverApi[trcvDriverId].EthTrcv_GetLinkState(
                                ETHIF_TRCV_ETHTRCV(trcvlIdx),
                                &linkState))
                        {
                            EthIf_Internal.EthIfTrcvRunState[trcvlIdx].TrcvLinkState = linkState;
                        }
                    }
                }
                else
                {
                    EthIf_Internal.EthIfTrcvRunState[trcvlIdx].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
                }
            }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */

#if (STD_ON == ETHIF_USE_ETHSWT)
            for (uint8 portIdx = 0; portIdx < EthIf_ConfigPtr->EthIfEthSwitchPortCount; portIdx++)
            {
                if (EthIf_Internal.EthIfEthPortRunState[portIdx].EthSwtPortMode == ETH_MODE_ACTIVE)
                {
                    uint8 switchIdx = EthIf_ConfigPtr->EthIfEthSwtPortCfg[portIdx].EthSwtIdx;
                    uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
                    if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetLinkState)
                    {
                        if (E_OK
                            == ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetLinkState(switchIdx, portIdx, &linkState))
                        {
                            EthIf_Internal.EthIfEthPortRunState[portIdx].SwtPortLinkState = linkState;
                        }
                    }
                }
                else
                {
                    EthIf_Internal.EthIfEthPortRunState[portIdx].SwtPortLinkState = ETHTRCV_LINK_STATE_DOWN;
                }
            }

            EthIf_SwtichPortStateHandle();
#endif /* STD_ON == ETHIF_USE_ETHSWT */

            EthIf_LinkStateChangeHandle();
#if (ETHIF_VLAN_USED == STD_ON)
            /*@req SWS263*/
            for (physCtrlIdx = 0; physCtrlIdx < EthIf_ConfigPtr->EthIfPhysControllerCount; physCtrlIdx++)
            {
                for (EthIfCtrlIdx = 0; EthIfCtrlIdx < EthIf_ConfigPtr->EthIfCtrlCount; EthIfCtrlIdx++)
                {
                    if (EthIf_ConfigPtr->EthIfCtrlCfg[EthIfCtrlIdx].EthIfPhysControllerIdRef == physCtrlIdx)
                    {
                        if (EthIf_Internal.EthIfCtrlRunState[EthIfCtrlIdx].EthIfCtrlMode != ETH_MODE_DOWN)
                        {
                            modeCtrl = FALSE;
                            break;
                        }
                    }
                }

                if (TRUE == modeCtrl)
                {
                    driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(physCtrlIdx);
                    ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(physCtrlIdx);
                    ret = Eth_DriverApi[driverId].Eth_GetControllerMode(ethContrllerId, &curCtrlMode);
                    if ((E_OK == ret) && (curCtrlMode != ETH_MODE_DOWN))
                    {
                        (void)Eth_DriverApi[driverId].Eth_SetControllerMode(ethContrllerId, ETH_MODE_DOWN);
                    }
                }
            }
#endif /* ETHIF_VLAN_USED == STD_ON */
        }
    }
}
/**
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetControllerMode call.
 * Can directly be called within the trigger functions.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the physical Ethernet controller within the context of the Ethernet Interface
 *                 CtrlMode,Notified Ethernet controller mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE)
EthIf_CtrlModeIndication(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode)
{
    Std_ReturnType ret;
    uint8 ethIfCtrlId;
#if (STD_ON == ETHIF_USE_ETHTRCV)
    uint8 trcvId;
    uint8 trcvDriverId;
    uint8 refTrcvId;
#endif

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ret = EthIf_SearchEthifCrlIndex(&ethIfCtrlId, CtrlIdx);
        if (E_OK == ret)
        {
#if (STD_ON == ETHIF_USE_ETHTRCV)
            refTrcvId = ETHIF_CONTROLLER_TRCVID(ethIfCtrlId);
            if (ETHIF_INVALID_IDX != refTrcvId)
            {
                trcvId = ETHIF_TRCV_ETHTRCV(refTrcvId);
                trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvId);
                if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_SetTransceiverMode)
                {
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450)
                    (void)EthTrcv_DriverApi[trcvDriverId].EthTrcv_SetTransceiverMode(trcvId, CtrlMode);
#else
                    EthTrcv_ModeType ctrlMode =
                        (CtrlMode == ETH_MODE_DOWN) ? (ETHTRCV_MODE_DOWN) : (ETHTRCV_MODE_ACTIVE);
                    (void)EthTrcv_DriverApi[trcvDriverId].EthTrcv_SetTransceiverMode(trcvId, ctrlMode);
#endif /* ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450 */
                }
                if (NULL_PTR != EthTrcv_DriverApi[trcvDriverId].EthTrcv_TransceiverLinkStateRequest)
                {
                    EthTrcv_LinkStateType trcvLinkState;

                    trcvLinkState = (ETH_MODE_ACTIVE == CtrlMode) ? ETHTRCV_LINK_STATE_ACTIVE : ETHTRCV_LINK_STATE_DOWN;

                    (void)EthTrcv_DriverApi[trcvDriverId].EthTrcv_TransceiverLinkStateRequest(trcvId, trcvLinkState);
                }
            }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */

#if (STD_ON == ETHIF_USE_ETHSWT)
            uint8 portGroupIdx = ETHIF_CONTROLLER_PORTGROUPID(ethIfCtrlId);
            if (ETHIF_INVALID_IDX != portGroupIdx)
            {
                const EthIf_PortGroupType* portGroupCfgPtr = &EthIf_ConfigPtr->EthIfSwtPortGroupCfg[portGroupIdx];
                if (ETHIF_SWITCH_PORT_GROUP_CONTROL == portGroupCfgPtr->EthIfPortGroupRefSemantics)
                {
                    EthIf_SetEthSwtPortMode(portGroupCfgPtr, ETHIF_SWITCH_PORT_GROUP_CONTROL, CtrlMode, FALSE);
                }
            }
#endif /* STD_ON == ETHIF_USE_ETHSWT */

            EthIf_Internal.EthIfEthRunState[ethIfCtrlId].EthIfEthMode = CtrlMode;
            (void)EthSM_CtrlModeIndication(ethIfCtrlId, CtrlMode);
        }
    }
}
#if (STD_ON == ETHIF_USE_ETHTRCV)
/**
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetTransceiverMode call.
 *  Can directly be called within the trigger functions.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 TrcvMode,Notified Ethernet transceiver mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE)
EthIf_TrcvModeIndication(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        EthIf_Internal.EthIfTrcvRunState[CtrlIdx].EthIfTrcvMode = CtrlMode;
    }
}
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
/**
 * Returns the number and index of all active Ethernet controllers.
 * Service ID: 0x44
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NumberOfCtrlIdx,in: maximum number of controllers in CtrlIdxListPtr, 0 to return the number of
 *                      controllers but without filling CtrlIdxListPtr. out: number of active controllers.
 * Parameters(OUT): CtrlIdxListPtr,List of active controller indexes
 * Return value: NA
 *
 */
#if (STD_ON == ETH_GET_CTRLIDX_LIST)
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetCtrlIdxList(
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) NumberOfCtrlIdx,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) CtrlIdxListPtr)
{
    Eth_ModeType ctrlMode;
    uint8 index;
    uint8 maxCtrlNbr;
    Std_ReturnType ret;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST, ETHIF_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if (NumberOfCtrlIdx == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST, ETHIF_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if (((*NumberOfCtrlIdx) != 0x00u) && (CtrlIdxListPtr == NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION, ETHIF_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif /*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        maxCtrlNbr = *NumberOfCtrlIdx;
        *NumberOfCtrlIdx = 0u;
        for (index = 0; index < EthIf_ConfigPtr->EthIfCtrlCount; index++)
        {
            if (E_OK == EthIf_GetControllerMode(index, &ctrlMode))
            {
                if (ETH_MODE_ACTIVE == ctrlMode)
                {
                    (*NumberOfCtrlIdx)++;
                    if ((maxCtrlNbr != 0u) && ((*NumberOfCtrlIdx) < maxCtrlNbr))
                    {
                        CtrlIdxListPtr[*NumberOfCtrlIdx] = index;
                    }
                }
            }
        }
        ret = E_OK;
    }
    return ret;
}
#endif

/*
 * Brief              Returns the VLAN identifier of the requested Ethernet controller.
 * Service ID:        0x43
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): N/A
 * Parameters(OUT):   VlanIdPtr,Pointer to store the VLAN identifier (VID) of the Ethernet controller. 0 if the
 *                              the Ethernet controller represents no virtual network (VLAN)
 * Return value:      Std_ReturnType,E_OK: failure
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetVlanId(VAR(uint8, ETHIF_VAR) CtrlIdx, P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) VlanIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_UNINIT);
    }
    else if (NULL_PTR == VlanIdPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
#if (STD_ON == ETHIF_VLAN_USED)
        *VlanIdPtr = EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId;
#else
        *VlanIdPtr = 0u;
#endif /* STD_ON == ETHIF_VLAN_USED */
        ret = E_OK;
    }

    return ret;
}

#if (STD_ON == ETHIF_USE_ETHSWT)
#if (STD_ON == ETHIF_GET_PORT_MAC_ADDR_API)
/*
 * Brief              Obtains the port over which this MAC-address can be reached.
 * Service ID:        0x28
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    MacAddrPtr, MAC-address for which a switch port is searched over which the node with this
 *                    MAC-address can be reached
 * Parameters(INOUT): N/A
 * Parameters(OUT):   SwtichIdxPtr,Pointer to the switch index
 *                    PortIdxPtr,Pointer to the port index
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK: an error occurred, e.g. multiple ports were found
 */
FUNC(BufReq_ReturnType, ETHIF_CODE)
EthIf_GetPortMacAddr(
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) MacAddrPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) SwtichIdxPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PortIdxPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR, ETHIF_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if ((MacAddrPtr == NULL_PTR) || (NULL_PTR == SwtichIdxPtr) || (NULL_PTR == PortIdxPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR, ETHIF_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethIfSwtIdx;
        uint8 switchIdx;
        uint8 ethSwtDriverId;

        for (ethIfSwtIdx = 0; ethIfSwtIdx < EthIf_ConfigPtr->EthIfSwitchCount; ethIfSwtIdx++)
        {
            switchIdx = EthIf_ConfigPtr->EthIfSwitchCfg[ethIfSwtIdx].EthIfEthSwitchRef->EthSwtIdx;
            ethSwtDriverId = EthIf_ConfigPtr->EthIfSwitchCfg[ethIfSwtIdx].EthIfEthSwitchRef->EthSwtDriverId;
            if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_GetPortMacAddr)
            {
                ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_GetPortMacAddr(switchIdx, MacAddrPtr, PortIdxPtr);
                if (E_OK == ret)
                {
                    *SwtichIdxPtr = switchIdx;
                    break;
                }
            }
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_GET_PORT_MAC_ADDR_API */

#if (STD_ON == ETHIF_GET_ARL_TABLE)
/*
 * Brief              Obtains the address resolution table of a switch and copies the list into a user provided buffer.
 * Service ID:        0x29
 * Sync/Async:        Synchronous/Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    switchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): numberOfElements,In: Maximum number of elements which can be written into the arlTable
 *                                     Out: Number of elements which are currently available in the EthSwitch module.
 * Parameters(OUT):   arlTableListPointer,Returns a pointer to the memory where the ARL table of the switch consisting
 *                    of a list of structs with MAC-address, VLAN-ID and port shall be stored.
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK: requested switchIdx is not valid or inactive
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetArlTable(
    VAR(uint8, ETHIF_VAR) switchIdx,
    P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR) numberOfElements,
    P2VAR(Eth_MacVlanType, ETHIF_APPL_DATA, ETHIF_VAR) arlTableListPointer)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_ARL_TABEL, ETHIF_E_UNINIT);
    }
    else if (switchIdx >= EthIf_ConfigPtr->EthIfEthSwitchCount)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_ARL_TABEL, ETHIF_E_PARAM_POINTER);
    }
    else if ((*numberOfElements > 0) && (NULL_PTR == arlTableListPointer))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_ARL_TABEL, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetArlTable)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                      .EthSwt_GetArlTable(switchIdx, numberOfElements, arlTableListPointer);
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_GET_ARL_TABLE */

#if (STD_ON == ETHIF_STORE_CONFIGURATION_API)
/*
 * Brief              Stores the configuration of the learned MAC/Port tables of a switch in a persistent manner
 *                    and will be used by e.g. CDD.
 * Service ID:        0x2c
 * Sync/Async:        Synchronous /Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK: Configuration could not be persistently stored
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_StoreConfiguration(VAR(uint8, ETHIF_VAR) SwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_STORE_CONFIG, ETHIF_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_StoreConfiguration)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_StoreConfiguration(SwitchIdx);
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_STORE_CONFIGURATION_API */

#if (STD_ON == ETHIF_RESET_CONFIGURATION_API)
/*
 * Brief              Resets the configuration of the learned MAC/Port tables of a switch in a persistent manner and
 *                    will be used by e.g. CDD. The statically configured entries shall still remain.
 * Service ID:        0x2d
 * Sync/Async:        Synchronous /Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK: configuration could not be persistently resetted
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ResetConfiguration(VAR(uint8, ETHIF_VAR) SwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RESET_CONFIG, ETHIF_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_ResetConfiguration)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_ResetConfiguration(SwitchIdx);
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_RESET_CONFIGURATION_API */

/*
 * Brief              Request a mode for the EthIfSwtPortGroup. The call shall be forwarded to EthSwt by calling
 *                    EthSwt_SetSwitchPortMode for all EthSwtPorts referenced by the port group.
 * Service ID:        0x06
 * Sync/Async:        Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    PortGroupIdx,Index of the port group within the context of the Ethernet Interface
 *                    PortMode,ETH_MODE_DOWN: Disable the Ethernet switch port group ETH_MODE_ACTIVE: Enable the
 *                    Ethernet switch port group
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK: port group mode could not be changed
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGroupRequestMode(
    VAR(EthIf_SwitchPortGroupIdxType, ETHIF_VAR) PortGroupIdx,
    VAR(Eth_ModeType, ETHIF_VAR) PortMode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_UNINIT);
    }
    else if (E_OK == EthIf_FindPortGroupId(PortGroupIdx, NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_INV_PORT_GROUP_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        const EthIf_PortGroupType* portGroupCfgPtr = &EthIf_ConfigPtr->EthIfSwtPortGroupCfg[PortGroupIdx];

        if (ETHIF_SWITCH_PORT_GROUP_LINK_INFO == portGroupCfgPtr->EthIfPortGroupRefSemantics)
        {
            ret = EthIf_SetEthSwtPortMode(portGroupCfgPtr, ETHIF_SWITCH_PORT_GROUP_LINK_INFO, PortMode, FALSE);
        }
    }

    return ret;
}

/*
 * Brief              Request to set all configured and affected EthSwtPorts to ETH_MODE_ACTIVE
 * Service ID:        0x07
 * Sync/Async:        Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    N/A
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: Request was accepted E_NOT_OK: Request was rejected
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_StartAllPorts(void)
{
    Std_ReturnType ret;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_START_ALL_PORTS, ETHIF_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portGroupIdx;
        for (portGroupIdx = 0u; portGroupIdx < EthIf_ConfigPtr->EthIfPortGroupCount; portGroupIdx++)
        {
            ret = EthIf_FindPortGroupId(portGroupIdx, NULL_PTR);
            if (ret == E_NOT_OK)
            {
                const EthIf_PortGroupType* portGroupCfgPtr = &EthIf_ConfigPtr->EthIfSwtPortGroupCfg[portGroupIdx];

                if (ETHIF_SWITCH_PORT_GROUP_LINK_INFO == portGroupCfgPtr->EthIfPortGroupRefSemantics)
                {
                    ret = EthIf_SetEthSwtPortMode(
                        portGroupCfgPtr,
                        ETHIF_SWITCH_PORT_GROUP_LINK_INFO,
                        ETH_MODE_ACTIVE,
                        FALSE);
                }
            }
        }
    }

    return ret;
}

#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
/*
 * Brief              Provides additional management information along to an Ethernet frame that requires special
 *                    treatment within the Switch. It has to be called between EthIf_ProvideTxBuffer() and
 *                    EthIf_Transmit() of the related frame.
 * Service ID:        0x38
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    CtrlIdx,Index of an Ethernet Interface controller
 *                    BufIdx,Ethernet Tx Buffer index
 *                    MgmtInfoPtr,Pointer to the management information
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: Management infos successfully set
 *                                   E_NOT_OK: Setting of management infos failed
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetSwitchMgmtInfo(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(EthSwt_MgmtInfoType, ETHIF_APPL_DATA, ETHIF_VAR) MgmtInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_INV_CTRL_IDX);
    }
    else if (BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_INV_PARAM);
    }
    else if (NULL_PTR == MgmtInfoPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        if (EthIf_Internal.EthIfCtrlRunState[CtrlIdx].RequstTxProvider)
        {
            for (uint8 portIdx = 0u; portIdx < EthIf_ConfigPtr->EthIfEthSwitchPortCount; portIdx++)
            {
                if (MgmtInfoPtr->SwitchPortIdx == portIdx)
                {
                    uint8 switchIdx = EthIf_ConfigPtr->EthIfEthSwtPortCfg[portIdx].EthSwtIdx;
                    uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
                    if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetMgmtInfo)
                    {
                        ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetMgmtInfo(CtrlIdx, BufIdx, MgmtInfoPtr);
                    }
                    EthIf_Internal.EthIfCtrlRunState[CtrlIdx].SwtMgmtInfo = *MgmtInfoPtr;
                    break;
                }
            }
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT */

/*
 * Brief              Request the MgmtObject of the (in this context) unique DataPtr
 * Service ID:        0x47
 * Sync/Async:        Synchronous
 * Reentrancy:        Reentrant
 * Parameters(IN):    CtrlIdx,Index of an Ethernet Interface controller
 *                    DataPtr,Ethernet data pointer
 * Parameters(INOUT): N/A
 * Parameters(OUT):   MgmtObjectPtr,MgmtObjectPtr Pointer to the management object
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK:management object could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetRxMgmtObject(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    P2VAR(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    EthSwt_MgmtObjectType** MgmtObjectPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_RX_MGMT_OBJECT, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfCtrlCount <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_RX_MGMT_OBJECT, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 switchIdx = 0u;
#if (1u < ETHIF_ETHSWT_NUM)
        for (switchIdx = 0u; switchIdx < EthIf_ConfigPtr->EthIfEthSwitchCount; switchIdx++)
#endif /* 1u < ETHIF_ETHSWT_NUM */
        {
            uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
            if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetRxMgmtObject)
            {
                ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetRxMgmtObject(CtrlIdx, DataPtr, MgmtObjectPtr);
#if (1u < ETHIF_ETHSWT_NUM)
                if (E_OK == ret)
                {
                    break;
                }
#endif /* 1u < ETHIF_ETHSWT_NUM */
            }
        }
    }

    return ret;
}

/*
 * Brief              Request the MgmtObject of the (in this context) unique BufIdx
 * Service ID:        0x48
 * Sync/Async:        Synchronous
 * Reentrancy:        Reentrant
 * Parameters(IN):    CtrlIdx,Index of an Ethernet Interface controller
 *                    BufIdx,Ethernet Rx Buffer index
 * Parameters(INOUT): N/A
 * Parameters(OUT):   MgmtObjectPtr,Pointer to the management object
 * Return value:      Std_ReturnType,E_OK: success E_NOT_OK:management object could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetTxMgmtObject(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    EthSwt_MgmtObjectType** MgmtObjectPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        if (BufIdx == EthIf_Internal.EthIfCtrlRunState[CtrlIdx].RequestBuffId)
        {
            uint8 switchIdx = EthIf_Internal.EthIfCtrlRunState[CtrlIdx].SwtMgmtInfo.SwitchIdx;
            uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
            if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetTxMgmtObject)
            {
                ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetTxMgmtObject(CtrlIdx, BufIdx, MgmtObjectPtr);
            }
        }
    }

    return ret;
}

#if (STD_ON == ETHIF_GLOBAL_TIME_SUPPORT)
/*
 * Brief              Provides additional management information along to an Ethernet frame that requires special
 *                    treatment within the Switch. It has to be called between EthIf_ProvideTxBuffer() and
 *                    EthIf_Transmit() of the related frame.
 * Service ID:        0x39
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                    BufIdx,Index of the message buffer, where Application expects egress time stamping
 * Parameters(INOUT): N/A
 * Parameters(OUT):   MgmtInfoPtr,Management information
 * Return value:      Std_ReturnType,E_OK: Time stamping on egress successfully enabled
 *                                   E_NOT_OK: Enabling of time stamping on egress has been failed
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchEnableTimeStamping(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    P2VAR(EthSwt_MgmtInfoType, ETHIF_APPL_DATA, ETHIF_VAR) MgmtInfo)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_UNINIT);
    }
    else if ((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_INV_CTRL_IDX);
    }
    else if (BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_INV_PARAM);
    }
    else if (NULL_PTR == MgmtInfo)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        if (BufIdx == EthIf_Internal.EthIfCtrlRunState[CtrlIdx].RequestBuffId)
        {
            uint8 switchIdx = EthIf_Internal.EthIfCtrlRunState[CtrlIdx].SwtMgmtInfo.SwitchIdx;
            uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[switchIdx].EthSwtDriverId;
            if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_PortEnableTimeStamp)
            {
                ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_PortEnableTimeStamp(switchIdx, BufIdx, MgmtInfo);
            }
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_GLOBAL_TIME_SUPPORT */

#if (STD_ON == ETHIF_VERIFY_CONFIG_API)
/*
 * Brief              Forwarded to EthSwt_VerifyConfig. EthSwt_VerifyConfig verifies the Switch Configuration
 *                    depending on the HW-Architecture, HW-capability and the intended accuracy of this verification.
 * Service ID:        0x40
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   Result,Result of verification, TRUE: configureation verified ok, FALSE:configuraton
 *                    values found corrupted
 * Return value:      Std_ReturnType,E_OK: Configuration verificaton succeeded
 *                                   E_NOT_OK:  Configuration verification not succeeded
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_VerifyConfig(VAR(uint8, ETHIF_VAR) SwitchIdx, P2VAR(boolean, ETHIF_APPL_DATA, ETHIF_VAR) Result)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_VERIFY_CONFIG, ETHIF_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_VerifyConfig)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_VerifyConfig(SwitchIdx, Result);
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_VERIFY_CONFIG_API */

#if (STD_ON == ETHIF_SET_FORWARDINGD_MODE_API)
/*
 * Brief              Verifies the Switch Configuration. If Configuration is not valid, Switch is reconfigured
 * Service ID:        0x41
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    mode,True Forwarding enabled, False Forwarding disabled
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: stopping of frame forwarding succeeded
 *                                   E_NOT_OK:stopping of frame forwarding not succeeded
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetForwardingMode(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(boolean, ETHIF_VAR) mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_FORWARDING_MODE, ETHIF_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetForwardingMode)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetForwardingMode(SwitchIdx, mode);
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_SET_FORWARDINGD_MODE_API */

/*
 * Brief              Retrieves the signal quality of the link of the given Ethernet switch port
 * Service ID:        0x1A
 * Sync/Async:        Synchronous
 * Reentrancy:        Reentrant for different Ethernet switch indexes and Ethernet Switch port indexes.
 *                    Non reentrant for the same SwitchPortIdx.
 * Parameters(IN):    SwitchIdx,Index of the Ethernet switch within the context of the Ethernet Interface
 *                    SwitchPortIdx,Index of the Ethernet switch port within the context of the Ethernet Interface
 * Parameters(INOUT): N/A
 * Parameters(OUT):   ResultPtr,Pointer to the memory where the signal quality in percent shall be stored
 * Return value:      Std_ReturnType,E_OK: The signal quality retrieved successfully
 *                                   E_NOT_OK:The signal quality not retrieved successfully
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortSignalQuality(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthIf_SignalQualityResultType, ETHIF_APPL_DATA, ETHIF_VAR) ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_INV_SWT_IDX);
    }
    else if (NULL_PTR == ResultPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetPortSignalQuality)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetPortSignalQuality(SwitchIdx, SwitchPortIdx, &ResultPtr->ActualSignalQuality);
                    ResultPtr->HighestSignalQuality = EthIf_PortSignalQuality[SwitchPortIdx].HighestSignalQuality;
                    ResultPtr->LowestSignalQuality = EthIf_PortSignalQuality[SwitchPortIdx].LowestSignalQuality;
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Clear the stored signal quality of the link of the given Ethernet switch port
 * Service ID:        0x1b
 * Sync/Async:        Synchronous
 * Reentrancy:        Reentrant for different Ethernet switch indexes and Ethernet Switch port indexes.
 *                    Non reentrant for the same SwitchPortIdx.
 * Parameters(IN):    SwitchIdx,Index of the Ethernet switch within the context of the Ethernet Interface
 *                    SwitchPortIdx,Index of the Ethernet switch port within the context of the Ethernet Interface
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK: The signal quality cleared successfully
 *                                   E_NOT_OK:The signal quality cleared not successfully
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ClearSwitchPortSignalQuality(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(uint8, ETHIF_VAR) SwitchPortIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CLEAR_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CLEAR_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                EthIf_PortSignalQuality[SwitchPortIdx].HighestSignalQuality = 0u;
                EthIf_PortSignalQuality[SwitchPortIdx].LowestSignalQuality = 0xFFFFFFFF;
                EthIf_PortSignalQuality[SwitchPortIdx].LowestSignalQuality = 0x0u;
                ret = E_OK;
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Obtains the mode of the indexed switch port
 * Service ID:        0x49
 * Sync/Async:        Synchronous /Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   PortModePtr,ETH_MODE_DOWN: The Ethernet switch port of the given Ethernet switch is disabled
 *                                ETH_MODE_ACTIVE: The Ethernet switch port of the given Ethernet switch is enabled
 * Return value:      Std_ReturnType,E_OK:success
 *                                   E_NOT_OK:The mode of the indexed switch port could not be obtained, or the
 *                                   function is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) PortModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetSwitchPortMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetSwitchPortMode(SwitchIdx, SwitchPortIdx, PortModePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Obtains the link state of the indexed switch port
 * Service ID:        0x4B
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   LinkStatePtr,ETHTRCV_LINK_STATE_DOWN: Switch port is disconnected
 *                                 ETHTRCV_LINK_STATE_ACTIVE: Switch port is connected
 * Return value:      Std_ReturnType,E_OK:success
 *                                   E_NOT_OK:Link state of the indexed switch port could not be obtained, or the
 *                                   function is called in state ETHSWT_STATE_INIT
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetLinkState(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_LinkStateType, ETHIF_APPL_DATA, ETHIF_VAR) LinkStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_LINK_STATE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_LINK_STATE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetLinkState)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetLinkState(SwitchIdx, SwitchPortIdx, LinkStatePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Obtains the baud rate of the indexed switch port
 * Service ID:        0x4D
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   BaudRatePtr,ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                                ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                                ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                                ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection
 * Return value:      Std_ReturnType,E_OK:success
 *                                   E_NOT_OK:Baud rate of the indexed switch port could not be obtained, or the
 *                                   function is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetBaudRate(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_BaudRateType, ETHIF_APPL_DATA, ETHIF_VAR) BaudRatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_BAUD_RATE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_BAUD_RATE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetBaudRate)
                {
                    ret =
                        ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetBaudRate(SwitchIdx, SwitchPortIdx, BaudRatePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Obtains the duplex mode of the indexed switch port
 * Service ID:        0x4F
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   DuplexModePtr,ETHTRCV_DUPLEX_MODE_HALF: half duplex connections
 *                                  ETHTRCV_DUPLEX_MODE_FULL: full duplex connection
 * Return value:      Std_ReturnType,E_OK:success
 *                                   E_NOT_OK:duplex mode of the indexed switch port could not be obtained, or the
 *                                   function is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetDuplexMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthTrcv_DuplexModeType, ETHIF_APPL_DATA, ETHIF_VAR) DuplexModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_DUPLEX_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_DUPLEX_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetDuplexMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetDuplexMode(SwitchIdx, SwitchPortIdx, DuplexModePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Reads a list with drop counter values of the corresponding port of the switch. The meaning of
 *                    these values is described at Eth_CounterType.
 * Service ID:        0x51
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   CounterPtr,counter values
 * Return value:      Std_ReturnType,E_OK:success
 *                                   E_NOT_OK:counter values read failure
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetCounterValue(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_CounterType, ETHIF_APPL_DATA, ETHIF_VAR) CounterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_COUNTER_VALUE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_COUNTER_VALUE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetCounterValues)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetCounterValues(SwitchIdx, SwitchPortIdx, CounterPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Returns a list of statistic counters defined with Eth_RxTatsType. The majority of these Counters
 *                    are derived from the IETF RFC2819.
 * Service ID:        0x52
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   RxStatsPtr,List of values according to IETF RFC 2819 (Remote Network Monitoring Management
 * Information Base) Return value:      Std_ReturnType,E_OK:success E_NOT_OK:drop counter could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetRxStats(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_RxStatsType, ETHIF_APPL_DATA, ETHIF_VAR) RxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_RX_STATS, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_RX_STATS, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetRxStats)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetRxStats(SwitchIdx, SwitchPortIdx, RxStatsPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              List of values to read statistic values for transmission
 * Service ID:        0x53
 * Sync/Async:        Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   TxStatsPtr,List of values to read statistic values for transmission
 * Return value:      Std_ReturnType,E_OK:success E_NOT_OK:Tx-statistics could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetTxStats(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_TxStatsType, ETHIF_APPL_DATA, ETHIF_VAR) TxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_STATS, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_STATS, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetTxStats)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetTxStats(SwitchIdx, SwitchPortIdx, TxStatsPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              List of values to read statistic error counter values for transmission from
 * Service ID:        0x54
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   TxStatsPtr,List of values to read statistic error values for transmission
 * Return value:      Std_ReturnType,E_OK:success E_NOT_OK:Tx-statistics could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetTxErrorCounterValues(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(Eth_TxErrorCounterValuesType, ETHIF_APPL_DATA, ETHIF_VAR) TxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_ERROR_COUNTER_VALUE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_ERROR_COUNTER_VALUE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetTxErrorCounterValues)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetTxErrorCounterValues(SwitchIdx, SwitchPortIdx, TxStatsPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Returns the MAC learning mode
 * Service ID:        0x55
 * Sync/Async:        Synchronous/Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   MacLearningModePtr,Defines whether MAC addresses shall be learned and if they shall be
 *                    learned in software or hardware
 * Return value:      Std_ReturnType,E_OK:success E_NOT_OK:configuration could be persistently reset
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetMacLearningMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(EthSwt_MacLearningType, ETHIF_APPL_DATA, ETHIF_VAR) MacLearningModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAC_LEARNING_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAC_LEARNING_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetMacLearningMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetMacLearningMode(SwitchIdx, SwitchPortIdx, MacLearningModePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              This function retrieves the OUI (24 bit) of the indexed Ethernet switch port
 * Service ID:        0x56
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    SwitchPortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   OrgUniqueIdPtr,Pointer to the memory where the Organizationally Unique
 *                                   Identifier (OUI) shall be stored
 *                    ModelNrPtr,Pointer to the memory where the Manufacturer’s Model Number shall be stored
 *                    RevisionNrPtr,Pointer to the memory where the Revision Number shall be stored
 * Return value:      Std_ReturnType,E_OK:organizationally unique identifier of the Ethernet transceiver could be read.
 *                                   E_NOT_OK:organizationally unique identifier of the Ethernet
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchPortIdentifier(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) OrgUniqueIdPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) ModelNrPtr,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) RevisionNrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_IDENTIFIER, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_IDENTIFIER, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == SwitchPortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetPortIdentifier)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetPortIdentifier(
                                  SwitchIdx,
                                  SwitchPortIdx,
                                  OrgUniqueIdPtr,
                                  ModelNrPtr,
                                  RevisionNrPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Obtain the Organizationally Unique Identifier that is given by the IEEE of the indexed Ethernet
 *                    switch
 * Service ID:        0x57
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   OrgUniqueIdPtr,Pointer to the memory where the Organizationally Unique Identifier shall be stored
 * Return value:      Std_ReturnType,E_OK:organizationally unique identifier of the Ethernet switch could be read
 *                                   E_NOT_OK:organizationally unique identifier of the Ethernet switch could not be
 *                                   read (i.e. no OUI is available for this Ethernet switch)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetSwitchIdentifier(VAR(uint8, ETHIF_VAR) SwitchIdx, P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) OrgUniqueIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_IDENTIFIER, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_IDENTIFIER, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetSwitchIdentifier)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetSwitchIdentifier(SwitchIdx, OrgUniqueIdPtr);
        }
    }

    return ret;
}

/*
 * Brief              Store the given port mirror configuration in a shadow buffer in the Ethernet switch driver
 *                    for the given MirroredSwitchIdx
 * Service ID:        0x58
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    MirroredSwitchIdx,Index of the switch within the context of the Ethernet Switch Driver, where the
 *                    Ethernet switch port is located, that has to be mirrored
 *                    PortMirrorConfigurationPtr
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:organizationally unique identifier of the Ethernet switch could be read
 *                                   E_NOT_OK:organizationally unique identifier of the Ethernet switch could not be
 *                                   read (i.e. no OUI is available for this Ethernet switch)
 *                                   ETHSWT_PORT_MIRRORING_CONFIGURATION_NOT_SUPPORTED: port mirroring configuration
 *                                   is not supported by Ethernet switch driver or by the Ethernet switch hardware
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_WritePortMirrorConfiguration(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    P2CONST(EthSwt_PortMirrorCfgType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorConfigurationPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_WRITE_PORT_MIRROR_CONFIGURATION, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= MirroredSwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_WRITE_PORT_MIRROR_CONFIGURATION, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[MirroredSwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_WritePortMirrorConfiguration)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                      .EthSwt_WritePortMirrorConfiguration(MirroredSwitchIdx, PortMirrorConfigurationPtr);
        }
    }

    return ret;
}

/*
 * Brief              Obtain the port mirror configuration of the given Ethernet switch
 * Service ID:        0x59
 * Sync/Async:        Synchronous/Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    MirroredSwitchIdx,Index of the switch within the context of the Ethernet Switch Driver, where the
 *                    Ethernet switch ports are located, that have to be mirrored
 * Parameters(INOUT): N/A
 * Parameters(OUT):   PortMirrorConfigurationPtr,Pointer to the memory where the port configuration shall be stored
 * Return value:      Std_ReturnType,E_OK: the port mirror configuration for the indexed Ethernet switch port was red
 *                    successfully E_NOT_OK:the port mirror configuration for the indexed Ethernet switch was not red
 *                    successfully. (i.e. indexed Ethernet switch is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_ReadPortMirrorConfiguration(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    P2VAR(EthSwt_PortMirrorCfgType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorConfigurationPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_READ_PORT_MIRROR_CONFIGURATION, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= MirroredSwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_READ_PORT_MIRROR_CONFIGURATION, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[MirroredSwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_ReadPortMirrorConfiguration)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                      .EthSwt_ReadPortMirrorConfiguration(MirroredSwitchIdx, PortMirrorConfigurationPtr);
        }
    }

    return ret;
}

/*
 * Brief              Delete the stored port mirror configuration of the given MirroredSwitchIdx
 * Service ID:        0x5A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    MirroredSwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   PortMirrorConfigurationPtr,Pointer to the memory where the port configuration shall be stored
 * Return value:      Std_ReturnType,E_OK: Port mirror configuration was deleted successfully
 *                    E_NOT_OK:Port mirror configuration was not deleted successfully. (e.g. the port
 *                    mirroring is enabled)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_DeletePortMirrorConfiguration(VAR(uint8, ETHIF_VAR) MirroredSwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_DELETE_PORT_MIRROR_CONFIGURATION, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= MirroredSwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_DELETE_PORT_MIRROR_CONFIGURATION, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[MirroredSwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_DeletePortMirrorConfiguration)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_DeletePortMirrorConfiguration(MirroredSwitchIdx);
        }
    }

    return ret;
}

/*
 * Brief              Obtain the current status of the port mirroring for the indexed Ethernet switch port
 * Service ID:        0x5B
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   PortMirrorStatePtr,Pointer to the memory where the port mirroring state (either
 *                    PORT_MIRRORING_ENABLED or PORT_MIRRORING_DISABLED)of the given Ethernet switch
 *                    port shall be stored
 * Return value:      Std_ReturnType,E_OK:the port mirroring state for the indexed Ethernet switch port returned
 *                    successfully  E_NOT_OK:the port mirror configuration for the indexed Ethernet switch returned
 *                    not successfully. (i.e. indexed ethernet switch port is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetPortMirrorState(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    P2VAR(EthSwt_PortMirrorStateType, ETHIF_APPL_DATA, ETHIF_VAR) PortMirrorStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MIRROR_STATE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MIRROR_STATE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetPortMirrorState)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetPortMirrorState(SwitchIdx, PortIdx, PortMirrorStatePtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Request to set the given port mirroring state of the port mirror configuration for the given
 *                    Ethernet switch
 * Service ID:        0x5C
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver,where the port
 *                    mirroring configuration is located that has to be enabled and disabled, repectively
 *                    PortIdx,Index of the port at the addressed switch
 *                    PortMirrorState,Contain the requested port mirroring state either PORT_MIRRORING_ENABLED
 *                    or PORT_MIRRORING_DISABLED
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:the requested port mirroring state for the indexed Ethernet switch port was
 *                    set successfully  E_NOT_OK:the requested port mirroring state for the indexed Ethernet switch was
 *                    not set successfully. (i.e. indexed Ethernet switch is not available, no port mirrior
 *                    configuration is available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortMirrorState(
    VAR(uint8, ETHIF_VAR) MirroredSwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthSwt_PortMirrorStateType, ETHIF_VAR) PortMirrorState)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_MIRROR_STATE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= MirroredSwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_MIRROR_STATE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[MirroredSwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortMirrorState)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_SetPortMirrorState(MirroredSwitchIdx, PortMirrorState);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Activates a given test mode of the indexed Ethernet switch port
 * Service ID:        0x5D
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 *                    Mode,Test mode to be activated
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:the port test mode for the indexed Ethernet switch port was set successfully
 *                    E_NOT_OK:the port test mode for the indexed Ethernet switch was not set successfully. (i.e.
 *                    indexed Ethernet switch port is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortTestMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyTestModeType, ETHIF_VAR) Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TEST_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TEST_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortTestMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortTestMode(SwitchIdx, PortIdx, Mode);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Activates a given test loop-back mode of the indexed Ethernet switch port
 * Service ID:        0x5E
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 *                    Mode,Loop-back mode to be activated
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:the port mirroring loop-back back mode for the indexed Ethernet switch
 *                    port was activated successfully E_NOT_OK:the port mirroring loop-back back mode for the indexed
 *                    Ethernet switch port was not activated successfully. (i.e.indexed Ethernet switch port is
 *                    not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortLoopbackMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyLoopbackModeType, ETHIF_VAR) Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_LOOPBACK_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_LOOPBACK_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortLoopbackMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortLoopbackMode(SwitchIdx, PortIdx, Mode);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Activates a given transmission mode of the indexed Ethernet switch port
 * Service ID:        0x5F
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 *                    Mode,Transmission mode to be activated
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:the port Tx mode for the indexed Ethernet switch port was activated
 *                    successfully E_NOT_OK: the port Tx mode for the indexed Ethernet switch port was not
 *                    activated successfully. (i.e. indexed Ethernet switch port is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SetPortTxMode(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(EthTrcv_PhyTxModeType, ETHIF_VAR) Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TX_MODE, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TX_MODE, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortTxMode)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_SetPortTxMode(SwitchIdx, PortIdx, Mode);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Retrieves the cable diagnostics result of the indexed Ethernet switch port respectively the
 *                    referenced Ethernet Transceiver Driver
 * Service ID:        0x60
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   ResultPtr,Pointer to the location where the cable diagnostics result shall be stored
 * Return value:      Std_ReturnType,E_OK:the port cable diagnostic result for the indexed Ethernet switch port
 *                    was obtained successfully E_NOT_OK: the port cable diagnostic result for the indexed Ethernet
 *                    switch port was not obtained successfully. (i.e. indexed Ethernet switch port is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_GetPortCableDiagnosticsResult(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    P2VAR(EthTrcv_CableDiagResultType, ETHIF_APPL_DATA, ETHIF_VAR) ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetPortCableDiagnosticsResult)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetPortCableDiagnosticsResult(SwitchIdx, PortIdx, ResultPtr);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Trigger the cable diagnostics of the given Ethernet Switch port (PortIdx) by calling EthTrcv_Run
 *                    CableDiagnostic of the referenced Ethernet transceiver
 * Service ID:        0x61
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the port at the addressed switch
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:The trigger to run the cable diagnostic has been accepted
 *                    E_NOT_OK: The trigger to run the cable diagnostic has not been accepted
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_RunPortCableDiagnostic(VAR(uint8, ETHIF_VAR) SwitchIdx, VAR(uint8, ETHIF_VAR) PortIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RUN_CABLE_DIAGNOSTIC, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RUN_CABLE_DIAGNOSTIC, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_RunPortCableDiagnostic)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_RunPortCableDiagnostic(SwitchIdx, PortIdx);
                }
                break;
            }
        }
    }

    return ret;
}

/*
 * Brief              Retrieves the data in memory of the indexed Ethernet switch in variable length
 * Service ID:        0x63
 * Sync/Async:        Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the switch within the context of the Ethernet Switch Driver
 *                    Offset,Offset of the Ethernet switch memory from where the reading starts
 *                    Length,Length of data in bytes that shall be copied
 * Parameters(INOUT): N/A
 * Parameters(OUT):   BufferPtr,Pointer to the location where the data shall be copied
 * Return value:      Std_ReturnType,E_OK:the data read was triggered successfully  E_NOT_OK: the data read was
 *                    not triggered successfully (i.e. indexed Ethernet switch is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchGetCfgDataRaw(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    VAR(uint32, ETHIF_VAR) Offset,
    VAR(uint16, ETHIF_VAR) Length,
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) BufferPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetCfgDataRaw)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetCfgDataRaw(SwitchIdx, Offset, Length, BufferPtr);
        }
    }

    return ret;
}

/*
 * Brief              Retrieves the total size of data and the memory start address of the indexed Ethernet Switch
 * Service ID:        0x64
 * Sync/Async:        Asynchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchIdx,Index of the Ethernet switch within the context of the Ethernet Switch Driver
 * Parameters(INOUT): N/A
 * Parameters(OUT):   DataSizePtr,Pointer to the location where the total size of the configuration data shall be copied
 *                    DataAdressPtr,Pointer to the location where the start address of the configuration registers
 *                    shall be copied
 * Return value:      Std_ReturnType,E_OK:the data was obtained successfully  E_NOT_OK: the data was not obtained
 *                    successfully. (i.e. indexed Ethernet switch is not available)
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchGetCfgDataInfo(
    VAR(uint8, ETHIF_VAR) SwitchIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) DataSizePtr,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) DataAdressPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchIdx].EthSwtDriverId;
        if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetCfgDataInfo)
        {
            ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetCfgDataInfo(SwitchIdx, DataSizePtr, DataAdressPtr);
        }
    }

    return ret;
}

/*
 * Brief              The function retrieves the maximum amount of allocated FIFO buffer of the indexed Ethernet switch
 *                    egress port. If the Ethernet switch hardware does not support Ethernet switch port based maximal
 *                    FIFO buffer level, the content of SwitchPortEgressFifoBufferLevelPtr shall be set to 0xFFFFFFFF.
 *                    This API may be called by e.g. a CDD
 * Service ID:        0x65
 * Sync/Async:        Reentrant Reentrant for different SwitchIdx and PortIdx. Non reentrant for the same
 *                    SwitchIdx and PortIdx
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    SwitchPortIdx,Index of the Ethernet switch within the context of the Ethernet Switch Driver
 *                    PortIdx,Index of the Ethernet switch egress port at the addressed Ethernet switch
 *                    SwitchPortEgressFifoIdx,Index of the egress FIFO of the addressed Ethernet switch port
 * Parameters(INOUT): N/A
 * Parameters(OUT):   SwitchPortEgressFifoBufferLevelPtr,Pointer to a memory location, where the maximum amount of
 *                    allocated FIFO buffer (in bytes) since the last read out shall be stored
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: The maximal FIFO buffer level could not be obtained
 */
FUNC(Std_ReturnType, ETHIF_CODE)
EthIf_SwitchPortGetMaxFIFOBufferFillLevel(
    VAR(uint8, ETHIF_VAR) SwitchPortIdx,
    VAR(uint8, ETHIF_VAR) PortIdx,
    VAR(uint8, ETHIF_VAR) SwitchPortEgressFifoIdx,
    P2VAR(uint32, ETHIF_APPL_DATA, ETHIF_VAR) SwitchPortEgressFifoBufferLevelPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL, ETHIF_E_UNINIT);
    }
    else if (EthIf_ConfigPtr->EthIfEthSwitchCount <= SwitchPortIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL, ETHIF_E_INV_SWT_IDX);
    }
    else
#endif /* STD_ON == ETHIF_DEV_ERROR_DETECT */
    {
        uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
        for (uint8 portId = 0u; portId < portCount; portId++)
        {
            if (portId == PortIdx)
            {
                uint8 ethSwtDrvId = EthIf_ConfigPtr->EthIfEthSwtCfg[SwitchPortIdx].EthSwtDriverId;
                if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId).EthSwt_GetMaxFIFOBufferFillLevel)
                {
                    ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDrvId)
                              .EthSwt_GetMaxFIFOBufferFillLevel(
                                  SwitchPortIdx,
                                  PortIdx,
                                  SwitchPortEgressFifoIdx,
                                  SwitchPortEgressFifoBufferLevelPtr);
                }
                break;
            }
        }
    }

    return ret;
}

#endif /* STD_ON == ETHIF_USE_ETHSWT */

/*
 * Brief              Find ethifCtrlId from EthCtrlId and vlanId
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    EthCrlId
 *                    VlanId
 * Parameters(INOUT): N/A
 * Parameters(OUT):   EthifCtrlId
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: not find
 */
static Std_ReturnType EthIf_SearchEthifCrlIndexFmVlan(uint8* EthifCtrlId, uint8 EthCtrlId, uint16 VlanId)
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;

    for (index = 0u; index < EthIf_ConfigPtr->EthIfCtrlCount; index++)
    {
        if (ETHIF_CONTROLLER_PHYSCONTROLLER(index) == EthCtrlId)
        {
#if (ETHIF_VLAN_USED == STD_ON)
            uint16 ctrlVlanId = EthIf_ConfigPtr->EthIfCtrlCfg[index].EthIfVlanId;
            if ((VlanId == ctrlVlanId) || ((VlanId == ETHIF_INVALID_VID) && (ETHIF_VID_UNUSED == ctrlVlanId)))
            {
                *EthifCtrlId = index;
                ret = E_OK;
                break;
            }

#else  /* ETHIF_VLAN_USED != STD_ON */
            (void)VlanId;
            *EthifCtrlId = index;
            ret = E_OK;
            break;
#endif /* ETHIF_VLAN_USED == STD_ON */
        }
    }
    return ret;
}

/*
 * Brief              Find ethifCtrlId from EthCtrlId
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    ethCrlId
 * Parameters(INOUT): N/A
 * Parameters(OUT):   ethifCtrlId
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: not find
 */
static Std_ReturnType EthIf_SearchEthifCrlIndex(uint8* EthifCtrlId, uint8 EthCtrlId)
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;

    for (index = 0u; index < EthIf_ConfigPtr->EthIfCtrlCount; index++)
    {
        if (ETHIF_CONTROLLER_PHYSCONTROLLER(index) == EthCtrlId)
        {
            *EthifCtrlId = index;
            ret = E_OK;
            break;
        }
    }
    return ret;
}

/*
 * Brief              save controller mode to inner buffer
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    ethIfCtrlId
 *                    CtrlMode
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      N/A
 */
static void EthIf_InnerSaveCtrlMode(uint8 EthIfCtrlId, Eth_ModeType CtrlMode)
{
    EthIf_Internal.EthIfCtrlRunState[EthIfCtrlId].EthIfCtrlMode = CtrlMode;
    EthIf_Internal.EthIfCtrlRunState[EthIfCtrlId].EthMode = CtrlMode;
}

/*
 * Brief              handle link state change
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    N/A
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      N/A
 */
static void EthIf_LinkStateChangeHandle(void)
{
    uint8 ethIfCtrlIdx;
    EthTrcv_LinkStateType LinkState;
#if (STD_ON == ETHIF_USE_ETHTRCV)
    uint8 TrcvIdx;
#endif /* STD_ON == ETHIF_USE_ETHTRCV */

    for (ethIfCtrlIdx = 0u; ethIfCtrlIdx < EthIf_ConfigPtr->EthIfCtrlCount; ethIfCtrlIdx++)
    {
        LinkState = ETHTRCV_LINK_STATE_DOWN;

        if (ETHIF_GET_ETHIFCTRLRUNMODE(ethIfCtrlIdx) == ETH_MODE_ACTIVE)
        {
            uint8 ethIfRefW = ETHIF_CONTROLLER_REF_HW(ethIfCtrlIdx);
#if (STD_ON == ETHIF_USE_ETHTRCV)
            if (ETHIF_CTRL_REF_HW_TRCV == ethIfRefW)
            {
                TrcvIdx = ETHIF_CONTROLLER_TRCVID(ethIfCtrlIdx);
                if (ETHIF_INVALID_IDX == TrcvIdx)
                {
                    LinkState = ETHTRCV_LINK_STATE_ACTIVE;
                }
                else
                {
                    LinkState = EthIf_Internal.EthIfTrcvRunState[TrcvIdx].TrcvLinkState;
                }
            }
            else
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
#if (STD_ON == ETHIF_USE_ETHSWT)
                if (ETHIF_CTRL_REF_HW_SWT == ethIfRefW)
            {
                EthIf_GetLinkStateChangeWithoutPortGroups(ethIfCtrlIdx, &LinkState);
            }
            else if (ETHIF_CTRL_REF_HW_SWT_PORT_GROUP == ethIfRefW)
            {
#if (0u < ETHIF_ETHSWTPORTGROUP_NUM)
                EthIf_GetLinkStateChangeWithPortGroups(ethIfCtrlIdx, &LinkState);
#endif /* 0u < ETHIF_ETHSWTPORTGROUP_NUM */
            }
            else
#endif /* STD_ON == ETHIF_USE_ETHSWT */
            {
                /* ETHIF_CTRL_REF_HW_NONE */
                LinkState = ETHTRCV_LINK_STATE_ACTIVE;
            }
        }

        if (EthIf_Internal.EthIfCtrlRunState[ethIfCtrlIdx].TrcvLinkState != LinkState)
        {
            uint8 idx;
            for (idx = 0u; idx < EthIf_ConfigPtr->EthIfTrcvLinkStateChgCount; idx++)
            {
                if (NULL_PTR != EthIf_ConfigPtr->EthIfTrcvLink[idx])
                {
                    /*To notify the current trcv link state, eg:EthSM*/
                    EthIf_ConfigPtr->EthIfTrcvLink[idx](ethIfCtrlIdx, LinkState);
                }
            }
            EthIf_Internal.EthIfCtrlRunState[ethIfCtrlIdx].TrcvLinkState = LinkState;
        }
    }
}

/*
 * Brief              set the controller mode to active
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    EthIfCtrlIdx
 *                    CtrlMode
 *                    EthCtrlIdx
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: failure
 */
static Std_ReturnType EthIf_SetCtrlModeActive(uint8 EthIfCtrlIdx, Eth_ModeType CtrlMode, uint8 EthCtrlIdx)
{
    Std_ReturnType ret = E_NOT_OK;

    if (ETH_MODE_ACTIVE == CtrlMode)
    {
        EthIf_EthStateType* ethRunStatePtr = &EthIf_Internal.EthIfEthRunState[EthCtrlIdx];

        if (ethRunStatePtr->EthIfEthMode == ETH_MODE_DOWN)
        {
            uint8 ethDriverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(EthIfCtrlIdx));
            if (E_OK == Eth_DriverApi[ethDriverId].Eth_SetControllerMode(EthCtrlIdx, CtrlMode))
            {
                ethRunStatePtr->EthIfEthModeCnt++;
                EthIf_InnerSaveCtrlMode(EthIfCtrlIdx, CtrlMode);
                ret = E_OK;
            }
        }
        else
        {
            (void)EthSM_CtrlModeIndication(EthIfCtrlIdx, CtrlMode);
            ethRunStatePtr->EthIfEthModeCnt++;
            EthIf_InnerSaveCtrlMode(EthIfCtrlIdx, CtrlMode);
            ret = E_OK;
        }
    }
    return ret;
}

/*
 * Brief              set the controller mode to down
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    EthIfCtrlIdx
 *                    CtrlMode
 *                    EthCtrlIdx
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: failure
 */
static Std_ReturnType EthIf_SetCtrlModeDown(uint8 EthIfCtrlIdx, Eth_ModeType CtrlMode, uint8 EthCtrlIdx)
{
    Std_ReturnType ret = E_NOT_OK;

    if (ETH_MODE_DOWN == CtrlMode)
    {
#if (STD_ON == ETHIF_USE_ETHSWT)
        uint8 portGroupIdx = ETHIF_CONTROLLER_PORTGROUPID(EthIfCtrlIdx);
        if (ETHIF_INVALID_IDX != portGroupIdx)
        {
            const EthIf_PortGroupType* portGroupCfgPtr = &EthIf_ConfigPtr->EthIfSwtPortGroupCfg[portGroupIdx];
            if (ETHIF_SWITCH_PORT_GROUP_CONTROL == portGroupCfgPtr->EthIfPortGroupRefSemantics)
            {
                EthIf_SetEthSwtPortMode(portGroupCfgPtr, ETHIF_SWITCH_PORT_GROUP_CONTROL, CtrlMode, TRUE);
            }
        }
#endif /* STD_ON == ETHIF_USE_ETHSWT */

        EthIf_EthStateType* ethRunStatePtr = &EthIf_Internal.EthIfEthRunState[EthCtrlIdx];
        uint8 ethModeCntTemp = ethRunStatePtr->EthIfEthModeCnt;

        ethModeCntTemp--;

        if (0u == ethModeCntTemp)
        {
            uint8 ethDriverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(EthIfCtrlIdx));
            Eth_ModeType oldEthIfEthMode = ethRunStatePtr->EthIfEthMode;
            Std_ReturnType result = Eth_DriverApi[ethDriverId].Eth_SetControllerMode(EthCtrlIdx, CtrlMode);
            Eth_ModeType newEthIfEthMode = ethRunStatePtr->EthIfEthMode;
            if ((ETH_MODE_ACTIVE == oldEthIfEthMode) && (ETH_MODE_DOWN == newEthIfEthMode) && (E_OK == result))
            {
                ethRunStatePtr->EthIfEthModeCnt = ethModeCntTemp;
                EthIf_InnerSaveCtrlMode(EthIfCtrlIdx, CtrlMode);
                ret = E_OK;
            }
        }
        else
        {
            (void)EthSM_CtrlModeIndication(EthIfCtrlIdx, CtrlMode);
            ethRunStatePtr->EthIfEthModeCnt = ethModeCntTemp;
            EthIf_InnerSaveCtrlMode(EthIfCtrlIdx, CtrlMode);
            ret = E_OK;
        }
    }
    return ret;
}

#if (STD_ON == ETHIF_USE_ETHSWT)
/*
 * Brief              obtain the link state without port group
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    EthIfCtrlIdx
 * Parameters(INOUT): N/A
 * Parameters(OUT):   SwtPortLinkState
 * Return value:      N/A
 */
static void EthIf_GetLinkStateChangeWithoutPortGroups(uint8 EthIfCtrlIdx, EthTrcv_LinkStateType* SwtPortLinkState)
{
    uint8 switchIdx;

    if (ETHIF_GET_ETHIFCTRLRUNMODE(EthIfCtrlIdx) == ETH_MODE_ACTIVE)
    {
        switchIdx = ETHIF_CONTROLLER_SWITCHID(EthIfCtrlIdx);
        if (ETHIF_INVALID_IDX == switchIdx)
        {
            *SwtPortLinkState = ETHTRCV_LINK_STATE_ACTIVE;
        }
        else
        {
            for (uint8 portId = 0u; portId < EthIf_ConfigPtr->EthIfEthSwitchCount; portId++)
            {
                if (switchIdx == EthIf_ConfigPtr->EthIfEthSwtPortCfg[portId].EthSwtIdx)
                {
                    *SwtPortLinkState = EthIf_Internal.EthIfEthPortRunState[portId].SwtPortLinkState;
                    break;
                }
            }
        }
    }
}
#if (0u < ETHIF_ETHSWTPORTGROUP_NUM)
/*
 * Brief              obtain the link state with port group
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    EthIfCtrlIdx
 * Parameters(INOUT): N/A
 * Parameters(OUT):   SwtPortLinkState
 * Return value:      N/A
 */
static FUNC(void, ETHIF_CODE)
    EthIf_GetLinkStateChangeWithPortGroups(uint8 EthIfCtrlIdx, EthTrcv_LinkStateType* SwtPortLinkState)
{
    uint8 portGroupIdx;

    if (ETHIF_GET_ETHIFCTRLRUNMODE(EthIfCtrlIdx) == ETH_MODE_ACTIVE)
    {
        portGroupIdx = ETHIF_CONTROLLER_PORTGROUPID(EthIfCtrlIdx);
        if (ETHIF_INVALID_IDX == portGroupIdx)
        {
            *SwtPortLinkState = ETHTRCV_LINK_STATE_ACTIVE;
        }
        else
        {
            for (uint8 portId = 0u; portId < EthIf_ConfigPtr->EthIfSwtPortGroupCfg[portGroupIdx].EthSwtPortNum;
                 portId++)
            {
                if (portId == EthIf_ConfigPtr->EthIfEthSwtPortCfg[portId].EthPortIdx)
                {
                    *SwtPortLinkState = EthIf_Internal.EthIfEthPortRunState[portId].SwtPortLinkState;
                    break;
                }
            }
        }
    }
}
#endif /* 0u < ETHIF_ETHSWTPORTGROUP_NUM */

/*
 * Brief              set switch port mode
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    PortGroupCfgPtr
 *                    PortGroupSemantic
 *                    CtrlMode
 *                    PendingDown
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: failure
 */
static Std_ReturnType EthIf_SetEthSwtPortMode(
    const EthIf_PortGroupType* PortGroupCfgPtr,
    EthIf_PortGroupSemanticType PortGroupSemantic,
    Eth_ModeType CtrlMode,
    boolean PendingDown)
{
    Std_ReturnType ret = E_NOT_OK;
    boolean callHwApi;
    uint8 ethSwtPortModeCnt;
    uint8 refPortId;
    EthIf_EthSwtPortStateType* portRunStatePtr;
    uint8 ethSwtPortNum = PortGroupCfgPtr->EthSwtPortNum;

    for (uint8 portIdx = 0u; portIdx < ethSwtPortNum; portIdx++)
    {
        refPortId = PortGroupCfgPtr->RefEthSwtPortIdList[portIdx];
        portRunStatePtr = &EthIf_Internal.EthIfEthPortRunState[refPortId];
        callHwApi = FALSE;
        ethSwtPortModeCnt = portRunStatePtr->EthSwtPortModeCnt;

        /* @SWS_EthIf_00264 */
        if (ETH_MODE_ACTIVE == CtrlMode)
        {
            if (portRunStatePtr->EthSwtPortMode == ETH_MODE_DOWN)
            {
                callHwApi = TRUE;
                ethSwtPortModeCnt++;
            }
            else
            {
                portRunStatePtr->EthSwtPortModeCnt++;
                ret = E_OK;
            }
        }
        /* @SWS_EthIf_00265 */
        else if (ETH_MODE_DOWN == CtrlMode)
        {
            if (portRunStatePtr->EthSwtPortMode == ETH_MODE_ACTIVE)
            {
                if (1u == ethSwtPortModeCnt)
                {
                    ethSwtPortModeCnt--;
                    if (ETHIF_SWITCH_PORT_GROUP_CONTROL == PortGroupSemantic)
                    {
                        if (!PendingDown)
                        {
                            callHwApi = TRUE;
                        }
                    }
                    else if (ETHIF_SWITCH_PORT_GROUP_LINK_INFO == PortGroupSemantic)
                    {
                        EthIf_SetSwitchOffPortTimer(PortGroupCfgPtr, ETHIF_SWITCHOFF_PORT_TIME_DELAY);
                        portRunStatePtr->EthSwtPortModeCnt = ethSwtPortModeCnt;
                        portRunStatePtr->EthSwtPortMode = CtrlMode;
                        ret = E_OK;
                    }
                    else
                    {
                        ret = E_NOT_OK;
                    }
                }
                else
                {
                    portRunStatePtr->EthSwtPortModeCnt--;
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
            break;
        }

        if (callHwApi)
        {
            ret = EthIf_SetHwSwtPortMode(refPortId, CtrlMode);
            if (E_OK == ret)
            {
                portRunStatePtr->EthSwtPortModeCnt = ethSwtPortModeCnt;
                portRunStatePtr->EthSwtPortMode = CtrlMode;

                if ((ETH_MODE_ACTIVE == CtrlMode) && (ETHIF_SWITCH_PORT_GROUP_LINK_INFO == PortGroupSemantic))
                {
                    EthIf_SetSwitchOffPortTimer(PortGroupCfgPtr, ETHIF_PORT_STARTUP_ACTIVE_TIME);
                    portRunStatePtr->EthSwtPortModeCnt = 0u;
                    portRunStatePtr->EthSwtPortMode = ETH_MODE_DOWN;
                }
            }
        }
    }

    return ret;
}

/*
 * Brief              Find ethifCtrlId from PortGroupId
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    PortGroupId
 * Parameters(INOUT): N/A
 * Parameters(OUT):   EthIfCtrlId
 * Return value:      Std_ReturnType,E_OK:success  E_NOT_OK: not find
 */
static Std_ReturnType EthIf_FindPortGroupId(EthIf_SwitchPortGroupIdxType PortGroupId, uint8* EthIfCtrlId)
{
    Std_ReturnType ret = E_NOT_OK;

    for (uint8 idx = 0; idx < EthIf_ConfigPtr->EthIfCtrlCount; idx++)
    {
        if (ETHIF_CONTROLLER_PORTGROUPID(idx) == PortGroupId)
        {
            ret = E_OK;
            if (NULL_PTR != EthIfCtrlId)
            {
                *EthIfCtrlId = idx;
            }
            break;
        }
    }

    return ret;
}

/*
 * Brief              set switch-off times
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    PortGroupPtr
 *                    TimeValue
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      N/A
 */
static void EthIf_SetSwitchOffPortTimer(const EthIf_PortGroupType* PortGroupPtr, uint32 TimeValue)
{
    uint8 refPortId;
    for (uint8 portIdx = 0u; portIdx < PortGroupPtr->EthSwtPortNum; portIdx++)
    {
        refPortId = PortGroupPtr->RefEthSwtPortIdList[portIdx];
        EthIf_Internal.EthIfEthPortRunState[refPortId].SwitchOffPortTimer = TimeValue;
    }
}

/*
 * Brief              handle switch port state
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    N/A
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      N/A
 */
static void EthIf_SwtichPortStateHandle(void)
{
    uint8 portCount = EthIf_ConfigPtr->EthIfEthSwitchPortCount;
    EthIf_EthSwtPortStateType* swtPortStatePtr;

    for (uint8 portIdx = 0u; portIdx < portCount; portIdx++)
    {
        swtPortStatePtr = &EthIf_Internal.EthIfEthPortRunState[portIdx];
        if (swtPortStatePtr->SwitchOffPortTimer > 0)
        {
            swtPortStatePtr->SwitchOffPortTimer--;

            if ((swtPortStatePtr->SwitchOffPortTimer == 0u) && (swtPortStatePtr->EthSwtPortMode == ETH_MODE_DOWN))
            {
                EthIf_SetHwSwtPortMode(portIdx, swtPortStatePtr->EthSwtPortMode);
            }
        }
    }
}

/*
 * Brief              Set the mode of switch hardware
 * Service ID:        N/A
 * Sync/Async:        Synchronous
 * Reentrancy:        Non Reentrant
 * Parameters(IN):    PortIdx
 *                    CtrlMode
 * Parameters(INOUT): N/A
 * Parameters(OUT):   N/A
 * Return value:      N/A
 */
static Std_ReturnType EthIf_SetHwSwtPortMode(uint8 PortIdx, Eth_ModeType CtrlMode)
{
    Std_ReturnType ret = E_NOT_OK;

    uint8 ethSwtIdx = EthIf_ConfigPtr->EthIfEthSwtPortCfg[PortIdx].EthSwtIdx;
    uint8 ethSwtDriverId = EthIf_ConfigPtr->EthIfEthSwtCfg[ethSwtIdx].EthSwtDriverId;

    if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_SetSwitchPortMode)
    {
        ret = ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_SetSwitchPortMode(ethSwtIdx, PortIdx, CtrlMode);
        if (E_OK == ret)
        {
            if (NULL_PTR != ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_PortLinkStateRequest)
            {
                EthTrcv_LinkStateType linkState =
                    (ETH_MODE_ACTIVE == CtrlMode) ? ETHTRCV_LINK_STATE_ACTIVE : ETHTRCV_LINK_STATE_DOWN;
                /* @SWS_EthIf_00412 or @SWS_EthIf_00412 */
                ETHIF_ETHSWT_DRIVER_API(ethSwtDriverId).EthSwt_PortLinkStateRequest(ethSwtIdx, PortIdx, linkState);
            }
        }
    }

    return ret;
}
#endif /* STD_ON == ETHIF_USE_ETHSWT */

#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
