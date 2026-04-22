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
 ************************************************************************************************************************
 **
 **  @file               : EthIf.c
 **  @author             : bo.yang
 **  @date               : 2020/07/20
 **  @vendor             : isoft
 **  @description        : Ethernet Interface
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "EthIf.h"
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "EthIf_Cbk.h"
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
#include "Os.h"
#endif
#if (STD_ON == ETHIF_ENABLE_SECRITY_EVENT_REPORTING)
#include "IdsM.h"
#endif
#if (STD_ON == ETHIF_HAST_SWITCH_PORT_GROUP_NO_CONNECT_CONTROLLER)
#include "BswM_EthIf.h"
#endif
/* ===================================================== macros ===================================================== */
#if !defined(ETHIF_LOCAL_INLINE)
#define ETHIF_LOCAL_INLINE static inline
#endif

#if !defined(ETHIF_LOCAL)
#define ETHIF_LOCAL static /* PRQA S 3414 */ /* VL_EthIf_MacroDefineSpecifierKeyword */
#endif

#define VLAN_TAG_SIZE        (uint16)4u
#define PRIORITY_MAX         (uint8)8u
#define ETH_FRAME_TYPE_VLAN  (uint16)0x8100u
#define INVALID_BUFFER_INDEX 0xFFFFFFFFu
#define INVALID_UINT8        0xFFu
#define INVALID_UINT32       0xFFFFFFFFu
#define ETHIF_INVALID_IDX    0xFFu
#define ETHIF_VID_MASK       0x0FFFu

#define ETHIF_VLAN_PRIORITY_BITS_SHIT_5_BITS (5u)
#define ETHIF_VLAN_CFI_BIT_SHIT_4_BITS       (4u)
#define ETHIF_VLAN_VLANID_MASK_HIGH_4_BITS   (0x0F00u)
#define ETHIF_VLAN_VLANID_MASK_LOW_8_BITS    (0x00FFu)
#define ETHIF_SHIFT_USE_8_BITS               (8u)
#define ETHIF_FRAME_TYPE_POS_2_OFFSET        (2u)
#define ETHIF_FRAME_TYPE_POS_3_OFFSET        (3u)
#define ETHIF_BIT_MAST_TO_GET_LOW_8_BITS     (0xFFu)

#define ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx)               EthIf_ConfigPtr->EthIfCtrlCfg[ctrlIdx]
#define ETHIF_PHYCONTROLLER_CFG_PHYCONTROLLER(ctrlIdx)         EthIf_ConfigPtr->EthIf_PhysControllerCfg[ctrlIdx]
#define ETHIF_TRANSCEIVER_CFG_TRANSCEIVER(trcvIdx)             EthIf_ConfigPtr->EthIfTrcvCfg[trcvIdx]
#define ETHIF_SWITCH_CFG_SWITCH(swtIdx)                        EthIf_ConfigPtr->EthIfSwitchCfg[swtIdx]
#define ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(swtGroupIdx) EthIf_ConfigPtr->EthIfSwtPortGroupCfg[swtGroupIdx]
#define ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIdx)               EthIf_ConfigPtr->EthIfEthSwtPortCfg[portIdx]

#define ETHIF_ETH_DRIVER_API_CFG(ethIdx)       EthIf_ConfigPtr->EthIfEthDrvApiCfg[ethIdx]
#define ETHIF_ETHTRCV_DRIVER_API_CFG(trcvIdx)  EthIf_ConfigPtr->EthIfEthTrcvApiCfg[trcvIdx]
#define ETHIF_SWITCH_DRIVER_API_CFG(switchIdx) EthIf_ConfigPtr->EthIfEthSwitchApiCfg[switchIdx]

#define ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx)     EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvIdx
#define ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx) EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthTrcvDriverId

#define ETHIF_SWITCH_CFG_ETHSWITCH_IDX(switchIdx)     EthIf_ConfigPtr->EthIfSwitchCfg[switchIdx].EthIfEthSwtIdx
#define ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(switchIdx) EthIf_ConfigPtr->EthIfSwitchCfg[switchIdx].EthIfEthSwtDriverId

#define ETHIF_CONTROLLER_CFG_ETH_IDX(ctrlIdx)        EthIf_ConfigPtr->EthIfCtrlCfg[ctrlIdx].EthIfEthCtrlIdx
#define ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(ctrlIdx) EthIf_ConfigPtr->EthIfCtrlCfg[ctrlIdx].EthIfEthIfDriverIdx

/* ================================================ type definitions ================================================ */
/* PRQA S 1503,1532 ++*/  /* VL_QAC_NoUsedApi,  VL_QAC_OneFunRef */
/* PRQA S 6040,6030 ++*/  /* VL_MTR_EthIf_QacSTPAR,  VL_MTR_EthIf_QacSTMIF */
/* PRQA S 6520,6510 EOF*/ /* VL_MTR_EthIf_QacSTVAR, VL_MTR_EthIf_QacSTCONF */
/* PRQA S 1536 EOF */     /* VL_EthIf_1536 */
typedef struct
{
    Eth_ModeType          EthIfTrcvMode;
    EthTrcv_LinkStateType TrcvLinkState;
} EthIf_TrcvStateType;

typedef struct
{
    Eth_ModeType EthIfEthMode;
    uint8        EthIfEthModeCnt;
} EthIf_EthStateType;

typedef struct
{
    Eth_ModeType          EthSwtPortMode;
    uint8                 EthSwtPortModeCnt;
    uint32                SwitchOffPortTimer;
    EthTrcv_LinkStateType SwtPortLinkState;
} EthIf_EthSwtPortStateType;

/* PRQA S 3213 ++ */ /* VL_EthIf_3213 */
typedef struct EthIf_ElementIndexRangeTag
{
    uint8 EqualStart;
    uint8 InequalEnd;
} EthIf_ElementRangeType;

typedef struct EthIf_MainFuncRunningCountTag
{
    uint8 TrcvLinkChgRunCount;
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
    uint8 TrcvQualityCheckRunCount;
#endif
} EthIf_MainFuncRunningCountType;
/* PRQA S 3213 -- */

/* ========================================== internal function declarations ======================================== */
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
/* @Brief   report error to Det */
ETHIF_LOCAL_INLINE void ETHIF_DET_REPORT(uint8 appiId, uint8 errorId)
{
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, appiId, errorId);
}
#else
#define ETHIF_DET_REPORT(appiId, errorId)
#endif

/* @Brief   call configured RxIndication functions. */
ETHIF_LOCAL void EthIf_InternalUpperLayerRxIndication(
    uint8               ethifCtrlId,
    Eth_FrameType       upperLayerFrameType,
    boolean             IsBroadcast,
    const uint8*        PhysAddrPtr,
    const Eth_DataType* upperLayerDataPtr,
    uint16              upperLayerLenByte);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R20_11)

/* @Brief   handle controller mode request of TxoffLine. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeTxOffLine(uint8 ctrlIdx);

/* @Brief   handle controller mode request of active with wakeup request. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeActiveWithWakeup(uint8 ctrlIdx);
#endif

/* @Brief   handle controller mode request of ACTIVE. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeActive(uint8 ctrlIdx);

/* @Brief   handle controller mode request of Down. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetEthCtrlModeDown(uint8 ctrlIdx);

/* @Brief   set mode or state of ethernet hardwar(transceiver, switch and switchportGroup) trigger by controller mode
 * request.*/
ETHIF_LOCAL void EthIf_InternalSetCtrlModeSetHardwareMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode);

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
/* @Brief   set mode or state of ethernet hardwar(transceiver) trigger by controller mode request. */
ETHIF_LOCAL void EthIf_InternalSetCtrlModeSetTrcvMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode);
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* @Brief   set mode or state of ethernet hardwar(switch trigger by controller mode request. */
ETHIF_LOCAL void
    EthIf_InternalSetCtrlModeSetSwitchPortMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode);

/* internal used function to excute EthSwt_GetLinkState */
ETHIF_LOCAL_INLINE Std_ReturnType
    EthIf_InternalSwitchPortGetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr);

/* internal used function to excute EthIf_GetSwitchPortSignalQuality */
ETHIF_LOCAL Std_ReturnType EthIf_InternalGetSwitchPortSignalQuality(
    uint8                          SwitchIdx,
    uint8                          SwitchPortIdx,
    EthIf_SignalQualityResultType* ResultPtr);

#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/* @Brief   set mode or state of ethernet hardwar(switchportGroup) trigger by controller mode request. */
ETHIF_LOCAL void
    EthIf_InternalSetCtrlModeSetSwitchPortGroupMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode);
#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/* @Brief   set mode or state of ethernet switchportGroup trigger by BswM. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetSwitchPortGroupMode(uint8 ethIfGroupIdx, Eth_ModeType setMode);

/* @brief Is all switchPortGroup referenced same port are requested down mode. */
ETHIF_LOCAL boolean EthIf_InternalIsALLPortGroupRequestModeDown(uint8 ethIfSwtIdx, uint8 portIdx);
#endif
/* @Brief   get port state request by requested transceiver mode. */
ETHIF_LOCAL_INLINE void
    EthIf_InternalGetTrcvStateCoupledWithMode(Eth_ModeType trcvMode, EthTrcv_LinkStateType* trcvState);

/* @Brief   exceute request ethernet driver to set controller mode. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetEthCtrlMode(uint8 ctrlIdx, Eth_ModeType mode);

/* @brief get EthIfCtrl Id by PhyCtrl and VlanId */
ETHIF_LOCAL Std_ReturnType EthIf_GetMatchedEthIfCtrl(uint8* EthifCtrlId, uint8 EthCtrlId, uint16 frameVlanId);

#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
ETHIF_LOCAL_INLINE uint8 EthIf_GetEthIfPartionByEcuCParttion(uint8 ecuCPartition);
#endif

#if ((ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431) || (ETHIF_SWITCHPORT_PORTS_NUM > 0))
/* internal used function to update signal queality of transceiver or switchPort */
ETHIF_LOCAL_INLINE void EthIf_InternalUpdateSignalQuality(EthIf_SignalQualityResultType* signalQualityPtr);
#endif

#if (ETHIF_USED_PARTITION_NUM > 1)
/* internal used function to get rangeon a decicated array whitch removed 0 */
ETHIF_LOCAL_INLINE void
    EthIf_InternalGetElementRange(const uint8* indexArray, uint8 rangeIndex, EthIf_ElementRangeType* range);
#endif
/* internal used function to excute EthTrcv_GetLinkState */
ETHIF_LOCAL_INLINE Std_ReturnType
    EthIf_InternalTransceiverGetLinkState(uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr);

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431) && (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
/* internal used function to excute EthIf_GetTrcvSignalQuality */
ETHIF_LOCAL Std_ReturnType EthIf_InternalGetTrcvSignalQuality(uint8 TrcvIdx, EthIf_SignalQualityResultType* ResultPtr);
#endif

/* internal used function to report transceiver link state change to controller */
ETHIF_LOCAL void EthIf_InternalTrcvLinkChgNotificationToCtrl(uint8 trcvIdx, EthTrcv_LinkStateType newLinkState);

/* internal used function to report transceiver link state change to user */
ETHIF_LOCAL_INLINE void EthIf_InternalTrcvLinkChgNotificationToUser(uint8 ctrlIdx, EthTrcv_LinkStateType newLinkState);

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* internal used function to polling  switchport link state.*/
ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortLinkState(uint8 ethIfPartition);

/* internal used function to check EthIfPortStartupActiveTime and EthIfSwitchOffPortTimeDelay of switchPorts.*/
ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortTimer(uint8 ethIfPartition);

#if (ETHIF_HAST_SWITCH_PORT_REF_TRANSCEIVER == STD_ON)
/* internal used function to polling  switchport signal quality. */
ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortSignalQuality(uint8 ethIfPartition);
#endif
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* @brief check StartActiveTimer and SwitchOffPortTimer. */
ETHIF_LOCAL void EthIf_InteralCheckSwitchPortTimer(uint8 ethIfSwitchPortIdx);
#endif

/* internal used function to polling  transceiver link state and quality. */
ETHIF_LOCAL void EthIf_InternalPollingTrcvLinkStateAndQuality(
    uint8   ethIfPartition,
    boolean reqGetLinkState
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
    ,
    boolean reqGetSignal
#endif
);

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* to switch link state  */
ETHIF_LOCAL void EthIf_InteralCheckChgOfSwitchLinkState(uint8 ethIfPartition);
#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/* to check switchPortGroup link state  */
ETHIF_LOCAL void EthIf_InteralCheckChgOfSwitchPortGroupLinkState(uint8 ethIfPartition);

/* get linkstate of switchPortGroup */
ETHIF_LOCAL void
    EthIf_InteralGetSwitchPortGroupLinkState(uint8 switchPortGroupIdx, EthTrcv_LinkStateType* groupLinkStatePtr);
#endif

ETHIF_LOCAL_INLINE void EthIf_InternalReportTxConfirmationToUpper(
    uint8          owerIdx,
    uint8          ethifCrlId,
    Eth_BufIdxType bufIdx,
    Std_ReturnType result);

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidTransceiver(uint8 trcvIdx, uint8 ethIfPartition, uint8* errorCode);
#endif

ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidController(uint8 ctrlIdx, uint8 ethIfPartition, uint8* errorCode);
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidSwitch(uint8 swtIdx, uint8 ethIfPartition, uint8* errorCode);
#endif

/* ============================================ internal data definition ============================================ */

#define ETHIF_START_SEC_VAR_CLEARED_PTR
#include "EthIf_MemMap.h"
ETHIF_LOCAL const EthIf_ConfigType* EthIf_ConfigPtr;
#define ETHIF_STOP_SEC_VAR_CLEARED_PTR
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "EthIf_MemMap.h"
/* PRQA S 3218,3229 ++ */ /* VL_EthIf_FileScopOnlyAccessedInOneFunction,VL_EthIf_3229 */
ETHIF_LOCAL EthIf_MainFuncRunningCountType EthIf_TrcvPollingCnt[ETHIF_USED_PARTITION_NUM];
/* PRQA S 3218,3229 -- */
#define ETHIF_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "EthIf_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/**
 * @brief           Initializes the Ethernet Interface.
 * @param[in]       CfgPtr :Points to the implementation specific structure.
 * @return          None
 * @reentrant       FALSE
 * @synchronous     TRUE
 * @trace           CPD-63576
 */
void EthIf_Init(const EthIf_ConfigType* CfgPtr)
{
    uint8 index;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_INIT, ETHIF_E_INIT_FAILED);
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_INIT, ETHIF_E_INVALID_PARTITION);
    }
#endif
    else
#endif
    {
        if (EthIf_ConfigPtr == NULL_PTR)
        {
            /* first time to init, clear all runtime information */
            /* if re-init, only update the pointer of configuration, other runtime information will not be cleared */
            EthIf_ConfigPtr = CfgPtr;
            for (index = 0u; index < ETHIF_FRAME_OWNER_FRAME_OWNER_NUM; index++)
            {
                EthIf_FrameOwnerRuntimeInformation[index].FrameType =
                    EthIf_ConfigPtr->EthIfOwnerCfg[index].EthIfFrameType;
                EthIf_FrameOwnerRuntimeInformation[index].BufferIdx   = INVALID_BUFFER_INDEX;
                EthIf_FrameOwnerRuntimeInformation[index].EthIfCrlIdx = ETHIF_CONTROLLER_CONTROLLER_NUM;
            }
            for (index = 0u; index < ETHIF_CONTROLLER_CONTROLLER_NUM; index++)
            {
                EthIf_ControllerInformation[index].CtrlMode                   = ETH_MODE_DOWN;
                EthIf_ControllerInformation[index].ReqCtrlMode                = ETH_MODE_DOWN;
                EthIf_ControllerInformation[index].HasModeReq                 = FALSE;
                EthIf_ControllerInformation[index].NoHardwareRefReportLinkChg = FALSE;
            }
            for (index = 0u; index < ETHIF_PHYCONTROLLER_PHYCONTROLLER_NUM; index++)
            {
                EthIf_PhyControllerInformation[index].CtrlMode = ETH_MODE_DOWN;
            }
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0)
            for (index = 0u; index < ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM; index++)
            {
                EthIf_TransceiverInformation[index].TrcvMode                               = ETH_MODE_DOWN;
                EthIf_TransceiverInformation[index].TrcvLinkState                          = ETHTRCV_LINK_STATE_DOWN;
                EthIf_TransceiverInformation[index].TrcvSignalQuality.HighestSignalQuality = 0u;
                EthIf_TransceiverInformation[index].TrcvSignalQuality.ActualSignalQuality  = 0u;
                EthIf_TransceiverInformation[index].TrcvSignalQuality.LowestSignalQuality  = INVALID_UINT32;
            }
#endif
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
            for (index = 0u; index < ETHIF_SWITCHPORT_PORTS_NUM; index++)
            {
                EthIf_SwitchPortRuntimeInformation[index].PortLinkState = ETHTRCV_LINK_STATE_DOWN;
                EthIf_SwitchPortRuntimeInformation[index].PortSignalQuality.HighestSignalQuality = 0u;
                EthIf_SwitchPortRuntimeInformation[index].PortSignalQuality.ActualSignalQuality  = 0u;
                EthIf_SwitchPortRuntimeInformation[index].PortSignalQuality.LowestSignalQuality  = INVALID_UINT32;
                EthIf_SwitchPortRuntimeInformation[index].PortMode                               = ETH_MODE_DOWN;
                EthIf_SwitchPortRuntimeInformation[index].SwitchOffPortTimer                     = 0u;
                EthIf_SwitchPortRuntimeInformation[index].StartActiveTimer                       = 0u;
            }
#endif
#if (ETHIF_SWITCH_SWITCHS_NUM > 0)
            for (index = 0u; index < ETHIF_SWITCH_SWITCHS_NUM; index++)
            {
                EthIf_SwitchLinkState[index] = ETHTRCV_LINK_STATE_DOWN;
            }
#endif
#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
            for (index = 0u; index < ETHIF_SWITCH_PORT_GROUP_GROUP_NUM; index++)
            {
                EthIf_SwitchPortGroupRuntimeInformation[index].GroupLinkState       = ETHTRCV_LINK_STATE_DOWN;
                EthIf_SwitchPortGroupRuntimeInformation[index].IsNotRequestModeDown = FALSE;
            }
#endif
#if (STD_ON == ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API)
            EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN = 0u;
#endif
            EthIf_TrcvPollingCnt[ethIfPartition].TrcvLinkChgRunCount = 0u;
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
            EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount = 0u;
#endif
        }
        EthIf_InitStatus[ethIfPartition] = TRUE;
    }
}

#if (ETHIF_VERSION_INFO_API == STD_ON)
/**
 * @brief   Returns the version information of this module.
 */
void EthIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        /*report development error*/
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VERSION_INFO, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfoPtr->moduleID         = ETHIF_MODULE_ID;
        VersionInfoPtr->sw_major_version = ETHIF_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = ETHIF_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = ETHIF_SW_PATCH_VERSION;
        VersionInfoPtr->vendorID         = ETHIF_VENDOR_ID;
    }
}
#endif

/* @brief    Enables / disables the indexed controller. */
Std_ReturnType EthIf_SetControllerMode(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE, errorCode);
    }
    else
#endif
    {
        ret = E_OK;
        if (EthIf_ControllerInformation[CtrlIdx].CtrlMode != CtrlMode)
        {
            EthIf_ControllerInformation[CtrlIdx].ReqCtrlMode = CtrlMode;
            EthIf_ControllerInformation[CtrlIdx].HasModeReq  = TRUE;
            switch (CtrlMode)
            {
            case ETH_MODE_ACTIVE:
                ret = EthIf_InternalSetCtrlModeActive(CtrlIdx);
                break;
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R20_11)
            case ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST:
                ret = EthIf_InternalSetCtrlModeActiveWithWakeup(CtrlIdx);
                break;
#endif
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R21_11)
            case ETH_MODE_ACTIVE_TX_OFFLINE:
                ret = EthIf_InternalSetCtrlModeTxOffLine(CtrlIdx);
                break;
#endif
            case ETH_MODE_DOWN:
                ret = EthIf_InternalSetEthCtrlModeDown(CtrlIdx);
                break;
            default: /* PRQA S 2016 */ /* VL_EthIf_SwitchStatementDefaultClauseEmpty */
                break;
            }
        }
    }
    return ret;
}

/* @brief  Obtains the state of the indexed controller. */
Std_ReturnType EthIf_GetControllerMode(uint8 CtrlIdx, Eth_ModeType* CtrlModePtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_MODE, errorCode);
    }
    else if (CtrlModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 phyCtrlIndx = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfPhysControllerIdx;
        if (EthIf_PhyControllerInformation[phyCtrlIndx].CtrlMode == EthIf_ControllerInformation[CtrlIdx].CtrlMode)
        {
            uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
            uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
            ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetControllerMode(ethContrllerId, CtrlModePtr);
        }
        else
        {
            *CtrlModePtr = EthIf_ControllerInformation[CtrlIdx].CtrlMode;
            ret          = E_OK;
        }
    }
    return ret;
}

#if (ETHIF_WAKEUP_SUPPORT == STD_ON)
/* @brief  This API request the affected Ethernet hardware to check for a signaled wake-up. */
Std_ReturnType EthIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CHECK_WAKE_UP, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CHECK_WAKE_UP, ETHIF_E_UNINIT);
    }
    else if (WakeupSource > ETHIF_WAKEUPSOURCE_MAX)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CHECK_WAKE_UP, ETHIF_E_INV_PARAM);
    }
    else
#endif
    {
        /* @SWS_EthIf_00245:  Ethernet transceivers (either referenced by EthIfTransceiver or by EthIfSwitchPortGroups)
        the function EthIf_CheckWakeup shall forward the call to function <EthTrcv>_CheckWakeup */
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0)
        for (uint8 trcvIdx = 0u; trcvIdx < ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM; trcvIdx++)
        {
            EcuM_WakeupSourceType trcvWakeupSource = ETHIF_TRANSCEIVER_CFG_TRANSCEIVER(trcvIdx).WakeUpSource;
            if ((0u != (WakeupSource & trcvWakeupSource))
#if ((STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED) && (STD_ON == ETHIF_DEV_ERROR_DETECT))
                && (ethIfPartition == ETHIF_TRANSCEIVER_CFG_TRANSCEIVER(trcvIdx).EthIfPartition)
#endif
            )
            {
                uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(trcvIdx);
                uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(trcvIdx);
                if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_CheckWakeup)
                {
                    ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_CheckWakeup(ethTrcvIdx);
                }
            }
        }
#endif
        /* @SWS_EthIf_00500:  For all affected Ethernet switches (referenced by EthIfSwitch)
            the function EthIf_CheckWakeup shall forward the call to function EthSwt_SwitchCheckWakeup. */
#if (ETHIF_SWITCH_SWITCHS_NUM > 0)
        for (uint8 swtIdx = 0u; swtIdx < ETHIF_SWITCH_SWITCHS_NUM; swtIdx++)
        {
            EcuM_WakeupSourceType swtWakeupSource = ETHIF_SWITCH_CFG_SWITCH(swtIdx).WakeUpSource;
            if ((0u != (WakeupSource & swtWakeupSource))
#if ((STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED) && (STD_ON == ETHIF_DEV_ERROR_DETECT))
                && (ethIfPartition == ETHIF_SWITCH_CFG_SWITCH(swtIdx).EthIfPartition)
#endif
            )
            {
                uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(swtIdx);
                uint8 driverId  = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(swtIdx);
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_CheckWakeup)
                {
                    ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_CheckWakeup(ethSwtIdx);
                }
            }
        }
#endif
    }
    return ret;
}
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R22_11)
/* @brief  obtains the wake up reasons of the indexed Ethernet Transceiver (PHY). */
Std_ReturnType EthIf_GetPhyWakeupReason(uint8 TrcvIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_WAKEUP_REASON, errorCode);
    }
    else if (WakeupReasonPtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_WAKEUP_REASON, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_GetBusWuReason)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_GetBusWuReason(ethTrcvIdx, WakeupReasonPtr);
        }
    }
#endif

    return ret;
}
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R22_11)
/*  This function obtains the wake up reasons of the indexed Ethernet switch port. */
Std_ReturnType
    EthIf_GetSwitchPortWakeupReason(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GETSWITCHPORTWAKEUPREASON, errorCode);
    }
    else if (SwitchPortIdx >= ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).NumberOfPorts)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GETSWITCHPORTWAKEUPREASON, ETHIF_E_INV_PORT_IDX);
    }
    else if (WakeupReasonPtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GETSWITCHPORTWAKEUPREASON, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwitchIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverId     = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetSwitchPortWakeupReason)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetSwitchPortWakeupReason(
                ethSwitchIdx,
                SwitchPortIdx,
                WakeupReasonPtr);
        }
    }
    return ret;
}
#endif

/*  Obtains the physical source address used by the indexed controller. */
void EthIf_GetPhysAddr(uint8 CtrlIdx, uint8* PhysAddrPtr)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR, errorCode);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetPhysAddr(ethContrllerId, PhysAddrPtr);
    }
}

/*  Sets the physical source address used by the indexed controller. */
void EthIf_SetPhysAddr(uint8 CtrlIdx, const uint8* PhysAddrPtr)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR, errorCode);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_SetPhysAddr(ethContrllerId, PhysAddrPtr);
    }
}

/*  @brief         Update the physical source address to/from the indexed controller filter. */
Std_ReturnType EthIf_UpdatePhysAddrFilter(uint8 CtrlIdx, const uint8* PhysAddrPtr, Eth_FilterActionType Action)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER, errorCode);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_UpdatePhysAddrFilter(ethContrllerId, PhysAddrPtr, Action);
    }
    return ret;
}

/* @brief         Returns the number and index of all active Ethernet controllers. */
#if (STD_ON == ETHIF_GET_CTRL_IDX_LIST)
Std_ReturnType EthIf_GetCtrlIdxList(uint8* NumberOfCtrlIdx, uint8* CtrlIdxListPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST, ETHIF_E_UNINIT);
    }
    else if ((NumberOfCtrlIdx == NULL_PTR) || (((*NumberOfCtrlIdx) != 0x00u) && (CtrlIdxListPtr == NULL_PTR)))
    {
        /* @SWS_EthIf_00300: shall check the OUT parameter CtrlIdxListPtr for being valid only if the the OUT parameter
                                NumberOfCtrlIdx is greater 0x00 */
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8        maxCtrlNbr = *NumberOfCtrlIdx;
        Eth_ModeType ctrlMode   = ETH_MODE_DOWN;
        *NumberOfCtrlIdx        = 0u;
        /* SWS_EthIf_00298: API shall return only the NumberOfCtrlIdx which are active. */
        for (uint8 index = 0u; index < ETHIF_CONTROLLER_CONTROLLER_NUM; index++)
        {
            if ((ETH_MODE_ACTIVE == EthIf_ControllerInformation[index].CtrlMode)
                && ((maxCtrlNbr != 0u) && ((*NumberOfCtrlIdx) < maxCtrlNbr)))
            {
                CtrlIdxListPtr[(*NumberOfCtrlIdx)] = index;
                (*NumberOfCtrlIdx)++;
            }
        }
        ret = E_OK;
    }
    return ret;
}
#endif

#if (ETHIF_GET_VLAN_ID_SUPPORT == STD_ON)
/* @brief         Returns the VLAN identifier of the requested Ethernet controller. */
Std_ReturnType EthIf_GetVlanId(uint8 CtrlIdx, uint16* VlanIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_UNINIT);
    }
    else if (NULL_PTR == VlanIdPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_PARAM_POINTER);
    }
    else if ((ETHIF_CONTROLLER_CONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VLANID, ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif
    {
#if (STD_ON == ETHIF_CTRL_HAS_CONFIGED_VLAN)
        if (ETHIF_VID_UNUSED != EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId)
        {
            *VlanIdPtr = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfVlanId;
        }
        else
#endif
        {
            *VlanIdPtr = 0u;
        }
        ret = E_OK;
    }

    return ret;
}
#endif

#if (STD_ON == ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API)
/* @brief         Returns the VLAN identifier of the requested Ethernet controller. */
Std_ReturnType EthIf_GetAndResetMeasurementData(
    EthIf_MeasurementIdxType MeasurementIdx,
    boolean                  MeasurementResetNeeded,
    uint32*                  MeasurementDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_AND_RESET_MEASUREMENT_DATA, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_AND_RESET_MEASUREMENT_DATA, ETHIF_E_UNINIT);
    }
    else
#endif
    {
        SchM_Enter_EthIf_ExclusiveArea();
        switch (MeasurementIdx)
        {
        case ETHIF_MEAS_DROP_CRTLIDX:
            if (NULL_PTR != MeasurementDataPtr)
            {
                *MeasurementDataPtr = EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN;
            }
            if (MeasurementResetNeeded)
            {
                EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN = 0u;
            }
            ret = E_OK;
            break;
        case ETHIF_MEAS_ALL:
            if (MeasurementResetNeeded)
            {
                EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN = 0u;
                ret                                                                = E_OK;
            }
            break;
        default: /* PRQA S 2016 */ /* VL_EthIf_SwitchStatementDefaultClauseEmpty */
            break;
        }
        SchM_Exit_EthIf_ExclusiveArea();
    }

    return ret;
}
#endif

#if (STD_ON == ETHIF_STORE_CONFIGURATION_API)
/*  @brief         Trigger the storage/reset of the configuration of the learned MAC/Port tables of a switch in a
 * persistent manner */
Std_ReturnType EthIf_StoreConfiguration(uint8 SwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_STORE_CONFIG, errorCode);
    }
    else
#endif
    {
        uint8 ethSwitchIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverId     = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        ret                = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_StoreConfiguration(ethSwitchIdx);
    }

    return ret;
}
#endif

#if (STD_ON == ETHIF_RESET_CONFIGURATION_API)
/*  @brief        The function shall request to reset the configuration of the learned MAC/Port tables of a Ethernet
 * switch in a persistent manner. */
Std_ReturnType EthIf_ResetConfiguration(uint8 SwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RESET_CONFIG, errorCode);
    }
    else
#endif
    {
        uint8 driverId = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_ResetConfiguration)
        {
            uint8 ethSwitchIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
            ret                = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_ResetConfiguration(ethSwitchIdx);
        }
    }

    return ret;
}
#endif

#if (ETHIF_GLOBAL_TIME_SUPPORT == STD_ON)
/*  @brief           Reads the current time of the timestamp clock and the current time of the PHC in an atomic
 * operation. */
Std_ReturnType EthIf_GetCurrentTimeTuple(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeTupleType* currentTimeTuplePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if ((ETHIF_CONTROLLER_CONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME_TUPLE, ETHIF_E_INV_CTRL_IDX);
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (!EthIf_InitStatus[ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfPartition])
#else
    else if (!EthIf_InitStatus[0u])
#endif
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME_TUPLE, ETHIF_E_UNINIT);
    }
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
    else if ((ETHIF_CLOCK_UNIT_CLOCK_UNIT_NUM) <= ClkUnitIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME_TUPLE, ETHIF_E_INV_CLKUNIT_IDX);
    }
#endif
    else if (NULL_PTR == currentTimeTuplePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME_TUPLE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethCtrId  = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverIdx = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
        ret = ETHIF_ETH_DRIVER_API_CFG(driverIdx).Eth_GetCurrentTimeTuple(ethCtrId, ClkUnitIdx, currentTimeTuplePtr);
#else
        /* EthSyn special requirment from SXF */
        Eth_TimeStampQualType timeQual;
        TimeStampType         timeStamp;
        /* PRQA S 0310 ++ */ /* VL_EthIf_CastingToDifferentObjectPointerType */
        ret =
            ETHIF_ETH_DRIVER_API_CFG(driverIdx).Eth_GetCurrentTime(ethCtrId, &timeQual, (Eth_TimeStampType*)&timeStamp);
        /* PRQA S 0310 -- */
        currentTimeTuplePtr->timestampClockValue = timeStamp;
        /* PRQA S 4322 ++ */ /* VL_EthIf_TypeBeingCastToDifferentEnumType */
        currentTimeTuplePtr->timeQuality = (TimeStampQualType)timeQual;
        /* PRQA S 4322 -- */
        (void)ClkUnitIdx;
#endif
    }
    return ret;
}

/*  @brief           Activates egress time stamping on a dedicated message object. */
void EthIf_EnableEgressTimeStamp(uint8 CtrlIdx, Eth_BufIdxType BufIdx)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP, errorCode);
    }
    else
#endif
    {
        uint8 ethCtrlIdx = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverIdx  = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
        ETHIF_ETH_DRIVER_API_CFG(driverIdx).Eth_EnableEgressTimeStamp(ethCtrlIdx, BufIdx);
#else
        ETHIF_ETH_DRIVER_API_CFG(driverIdx).Eth_EnableEgressTimeStamp(ethCtrlIdx, (uint8)BufIdx);
#endif
    }
}

/*  @brief           Reads back the ingress time stamp on a dedicated message object. */
Std_ReturnType EthIf_GetIngressTimeStamp(
    uint8                  CtrlIdx,
    const Eth_DataType*    DataPtr,
    Eth_TimeStampQualType* timeQualPtr,
    Eth_TimeStampType*     timeStampPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP, errorCode);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr) || (NULL_PTR == DataPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                  .Eth_GetIngressTimeStamp(ethContrllerId, DataPtr, timeQualPtr, timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
        ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetIngressTimeStamp(ethContrllerId, DataPtr, timeQualPtr, timeStampPtr);
        ret = E_OK;
#else
        /* PRQA S 0311 ++ */ /* VL_Com_PointerObjectType */
        ETHIF_ETH_DRIVER_API_CFG(driverId)
            .Eth_GetIngressTimeStamp(ethContrllerId, (Eth_DataType*)DataPtr, timeQualPtr, timeStampPtr);
        /* PRQA S 0311 -- */
        ret = E_OK;
#endif
    }
    return ret;
}

/*  @brief           Reads back the egress time stamp on a dedicated message object. */
Std_ReturnType EthIf_GetEgressTimeStamp(
    uint8                  CtrlIdx,
    Eth_BufIdxType         BufIdx,
    Eth_TimeStampQualType* timeQualPtr,
    Eth_TimeStampType*     timeStampPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP, errorCode);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethCtrlIdx = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId   = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetEgressTimeStamp(ethCtrlIdx, BufIdx, timeQualPtr, timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
        ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetEgressTimeStamp(ethCtrlIdx, BufIdx, timeQualPtr, timeStampPtr);
        ret = E_OK;
#else
        /*The default version of eth driver is 4.2.2*/
        ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetEgressTimeStamp(ethCtrlIdx, (uint8)BufIdx, timeQualPtr, timeStampPtr);
        ret = E_OK;
#endif
    }
    return ret;
}
#endif

#if ((ETHIF_PHC_SUPPORT == STD_ON) && (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11))
/*  @brief           Sets the absolute time of the PHC. */
Std_ReturnType EthIf_SetPhcTime(uint8 CtrlIdx, uint8 ClkUnitIdx, const TimeStampType* timeStampPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHC_TIME, errorCode);
    }
    else if ((ETHIF_CLOCK_UNIT_CLOCK_UNIT_NUM) <= ClkUnitIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHC_TIME, ETHIF_E_INV_CLKUNIT_IDX);
    }
    else if (NULL_PTR == timeStampPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHC_TIME, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_SetPhcTime(ethContrllerId, ClkUnitIdx, timeStampPtr);
    }
    return ret;
}

/*  @brief           Sets the absolute time of the PHC. */
Std_ReturnType EthIf_SetPhcCorrection(uint8 CtrlIdx, uint8 ClkUnitIdx, sint32 rateDeviation, sint32 offset)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHC_CORRECTION, errorCode);
    }
    else if ((ETHIF_CLOCK_UNIT_CLOCK_UNIT_NUM) <= ClkUnitIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHC_CORRECTION, ETHIF_E_INV_CLKUNIT_IDX);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ret =
            ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_SetPhcCorrection(ethContrllerId, ClkUnitIdx, rateDeviation, offset);
    }
    return ret;
}

/*  @brief           Returns the current time value out of the HW registers of the PHC. */
Std_ReturnType
    EthIf_GetPhcTime(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeStampQualType* timeQualPtr, TimeStampType* timeStampPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHC_TIME, errorCode);
    }
    else if ((ETHIF_CLOCK_UNIT_CLOCK_UNIT_NUM) <= ClkUnitIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHC_TIME, ETHIF_E_INV_CLKUNIT_IDX);
    }
    else if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHC_TIME, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetPhcTime(ethContrllerId, ClkUnitIdx, timeQualPtr, timeStampPtr);
    }

    return ret;
}
#endif

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
/*  @brief           Enables/disables the generation of a PPS signal. */
Std_ReturnType EthIf_SetPpsSignalMode(uint8 CtrlIdx, uint8 ClkUnitIdx, boolean signalMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PPS_SIGNAL_MODE, errorCode);
    }
    else if ((ETHIF_CLOCK_UNIT_CLOCK_UNIT_NUM) <= ClkUnitIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PPS_SIGNAL_MODE, ETHIF_E_INV_CLKUNIT_IDX);
    }
    else
#endif
    {
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_SetPpsSignalMode(ethContrllerId, ClkUnitIdx, signalMode);
    }
    return ret;
}
#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/*  @brief         Request a mode for the EthIfSwtPortGroup. */
Std_ReturnType EthIf_SwitchPortGroupRequestMode(EthIf_SwitchPortGroupIdxType PortGroupIdx, Eth_ModeType PortMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_UNINIT);
    }
    else if (PortGroupIdx >= ETHIF_SWITCH_PORT_GROUP_GROUP_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_INV_PORT_GROUP_IDX);
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (ethIfPartition != ETHIF_CONTROLLER_CFG_CONTROLLER(PortGroupIdx).EthIfPartition)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_PORTGROUP_MODE, ETHIF_E_INVALID_PARTITION);
    }
#endif
    else
#endif
    {
        /* ECUC_EthIf_00059:  Used in case all ports in this group are controlled
            by EthIf_SwitchPortGroupRequestMode.*/
        const EthIf_PortGroupType* portGroupCfgPtr = &EthIf_ConfigPtr->EthIfSwtPortGroupCfg[PortGroupIdx];
        if (ETHIF_SWITCH_PORT_GROUP_LINK_INFO == portGroupCfgPtr->EthIfPortGroupRefSemantics)
        {
            ret = EthIf_InternalSetSwitchPortGroupMode(PortGroupIdx, PortMode);
        }
    }

    return ret;
}
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/*  @brief         Request to set all configured and affected EthSwtPorts to ETH_MODE_ACTIVE. */
Std_ReturnType EthIf_StartAllPorts(void)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_START_ALL_PORTS, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_START_ALL_PORTS, ETHIF_E_UNINIT);
    }
    else
#endif
    {
        for (uint8 idx = 0u; idx < ETHIF_SWITCHPORT_PORTS_NUM; idx++)
        {
            uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(idx).EthIfEthIfSwtIdx;
            uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(idx).EthPortIdx;
            uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
            uint8 ethSwitch   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
            /* @SWS_EthIf_00275 : then EthIf shall forward the call to the following functions in the given order to all
                 EthSwtPorts of all configured EthIfSwitches. */
            if ((ETH_MODE_DOWN == EthIf_SwitchPortRuntimeInformation[idx].PortMode)
                && (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode))
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(ethSwitch, port, ETH_MODE_ACTIVE);
                EthIf_SwitchPortRuntimeInformation[idx].StartActiveTimer = EthIf_ConfigPtr->PortStartUpActiveTimeCnt;
            }

            if ((ETHTRCV_LINK_STATE_DOWN == EthIf_SwitchPortRuntimeInformation[idx].PortLinkState)
                && (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest))
            {
                (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                    ethSwitch,
                    port,
                    ETHTRCV_LINK_STATE_ACTIVE);
            }
            if (E_OK != ret)
            {
                break;
            }
        }
    }

    return ret;
}
#endif

#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)

/* @brief         Provides additional management information along to an Ethernet frame that requires special treatment
 * within the Switch.*/
Std_ReturnType EthIf_SetSwitchMgmtInfo(
    uint8                CtrlIdx,
    Eth_BufIdxType       BufIdx,
    EthSwt_MgmtInfoType* MgmtInfoPtr) /* PRQA S 3673 */ /* VL_EthIf_TypeOfPointerCouldbePointToConst */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_UNINIT);
    }
    else if ((ETHIF_CONTROLLER_CONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_INV_CTRL_IDX);
    }
    else if (BufIdx >= ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_INV_PARAM);
    }
    else if (NULL_PTR == MgmtInfoPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_SWITCH_MGMT_INFO, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwt = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).TrcvRefIdx;
        if (ethSwt < ETHIF_SWITCH_SWITCHS_NUM)
        {
            uint8 driverId = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethSwt);
            if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_SetMgmtInfo)
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_SetMgmtInfo(CtrlIdx, BufIdx, MgmtInfoPtr);
            }
        }
    }

    return ret;
}
#endif

#if ((STD_ON == ETHIF_GLOBAL_TIME_SUPPORT) && (ETHIF_SWITCH_SWITCHS_NUM > 0))
/*  @brief         Activates egress time stamping on a dedicated message object, addressed by CtrlIdx and BufIdx. */
Std_ReturnType EthIf_SwitchEnableTimeStamping(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtInfoType* MgmtInfo)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, errorCode);
    }
    else if (BufIdx >= ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_INV_PARAM);
    }
    else if (NULL_PTR == MgmtInfo)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_ENABLE_TIME_STAMPING, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).TrcvRefIdx;
        if (ethSwtIdx < ETHIF_SWITCH_SWITCHS_NUM)
        {
            uint8 driverId    = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethSwtIdx);
            uint8 ethIfSwitch = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethSwtIdx);
            if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_PortEnableTimeStamp)
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_PortEnableTimeStamp(ethIfSwitch, BufIdx, MgmtInfo);
            }
        }
    }
    return ret;
}
#endif

#if (STD_ON == ETHIF_VERIFY_CONFIG_API)
/*  @brief           Forwarded to EthSwt_VerifyConfig. EthSwt_VerifyConfig verifies the Switch Configuration
                    depending on the HW-Architecture, HW-capability and the intended accuracy of this verification. */
Std_ReturnType EthIf_VerifyConfig(uint8 SwitchIdx, boolean* Result)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_VERIFY_CONFIG, errorCode);
    }
    else if (NULL_PTR == Result)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_VERIFY_CONFIG, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtDrvId = ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).EthIfEthSwtDriverId;
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDrvId).EthSwt_VerifyConfig)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDrvId).EthSwt_VerifyConfig(SwitchIdx, Result);
        }
    }

    return ret;
}
#endif

#if (STD_ON == ETHIF_SET_FORWARDING_MODE_API)

/*  @brief           Control of switch frame forwarding. */
Std_ReturnType EthIf_SetForwardingMode(uint8 SwitchIdx, boolean mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_FORWARDING_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtDrvId = ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).EthIfEthSwtDriverId;
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDrvId).EthSwt_SetForwardingMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDrvId).EthSwt_SetForwardingMode(SwitchIdx, mode);
        }
    }

    return ret;
}
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
/*  @brief           Retrieves the signal quality of the link of the given Ethernet transceiver. */
Std_ReturnType EthIf_GetTrcvSignalQuality(uint8 TrcvIdx, EthIf_SignalQualityResultType* ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_SIGNAL_QUALITY, errorCode);
    }
    else if (NULL_PTR == ResultPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_SIGNAL_QUALITY, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        ret = EthIf_InternalGetTrcvSignalQuality(TrcvIdx, ResultPtr);
    }
#endif

    return ret;
}

/* @brief           Clear the stored signal quality of the link of the given Ethernet transceiver.*/
Std_ReturnType EthIf_ClearTrcvSignalQuality(uint8 TrcvIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CLEAR_TRCV_SIGNAL_QUALITY, errorCode);
    }
    else
#endif
    {
        EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality.ActualSignalQuality  = 0u;
        EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality.LowestSignalQuality  = INVALID_UINT32;
        EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality.HighestSignalQuality = 0u;
        ret                                                                          = E_OK;
    }
#endif

    return ret;
}

#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/**
 * @brief           Retrieves the signal quality of the link of the given Ethernet switch port.
 */
Std_ReturnType
    EthIf_GetSwitchPortSignalQuality(uint8 SwitchIdx, uint8 SwitchPortIdx, EthIf_SignalQualityResultType* ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, errorCode);
    }
    else if (SwitchPortIdx >= ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).NumberOfPorts)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_INV_PORT_IDX);
    }
    else if (NULL_PTR == ResultPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        ret = EthIf_InternalGetSwitchPortSignalQuality(SwitchIdx, SwitchPortIdx, ResultPtr);
    }
    return ret;
}
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/*  @brief           Clear the stored signal quality of the link of the given Ethernet switch port. */
Std_ReturnType EthIf_ClearSwitchPortSignalQuality(uint8 SwitchIdx, uint8 SwitchPortIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CLEAR_SWITCH_PORT_SIGNAL_QUALITY, errorCode);
    }
    else if (SwitchPortIdx >= ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).NumberOfPorts)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CLEAR_SWITCH_PORT_SIGNAL_QUALITY, ETHIF_E_INV_PORT_IDX);
    }
    else
#endif
    {
        uint8 portIndex = ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).EthIfSwtPortOnPartitonStarIdx + SwitchPortIdx;
        EthIf_SwitchPortRuntimeInformation[portIndex].PortSignalQuality.ActualSignalQuality  = 0u;
        EthIf_SwitchPortRuntimeInformation[portIndex].PortSignalQuality.LowestSignalQuality  = INVALID_UINT32;
        EthIf_SwitchPortRuntimeInformation[portIndex].PortSignalQuality.HighestSignalQuality = 0u;
        ret                                                                                  = E_OK;
    }

    return ret;
}
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
/*  @brief           Activates a given test mode. */
Std_ReturnType EthIf_SetPhyTestMode(uint8 TrcvIdx, EthTrcv_PhyTestModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_TEST_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyTestMode)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyTestMode(ethTrcvIdx, Mode);
        }
    }
#endif
    return ret;
}

/*  @brief           Activates a given loopback mode. */
Std_ReturnType EthIf_SetPhyLoopbackMode(uint8 TrcvIdx, EthTrcv_PhyLoopbackModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_LOOPBACK_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyLoopbackMode)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyLoopbackMode(ethTrcvIdx, Mode);
        }
    }
#endif

    return ret;
}

/* @brief           Activates a given transmission mode. */
Std_ReturnType EthIf_SetPhyTxMode(uint8 TrcvIdx, EthTrcv_PhyTxModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_TX_MODE, errorCode);
    }
    else if (((ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM) <= TrcvIdx))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_TX_MODE, ETHIF_E_INV_TRCV_IDX);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyTxMode)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_SetPhyTxMode(ethTrcvIdx, Mode);
        }
    }
#endif

    return ret;
}

/*  @brief           Retrieves the cable diagnostics result of a given transceiver. */
Std_ReturnType EthIf_GetCableDiagnosticsResult(uint8 TrcvIdx, EthTrcv_CableDiagResultType* ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
    uint8 errorCode;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CABLE_DIAGNOSTICS_RESULT, errorCode);
    }
    else if (NULL_PTR == ResultPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CABLE_DIAGNOSTICS_RESULT, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_GetCableDiagnosticsResult)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_GetCableDiagnosticsResult(ethTrcvIdx, ResultPtr);
        }
    }
#endif

    return ret;
}

/*  @brief           Obtains the PHY identifier of the Ethernet Interface according to IEEE 802.3-2015. */
Std_ReturnType EthIf_GetPhyIdentifier(uint8 TrcvIdx, uint32* OrgUniqueIdPtr, uint8* ModelNrPtr, uint8* RevisionNrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
    uint8 errorCode;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_IDENTIFIER, errorCode);
    }
    else if ((NULL_PTR == OrgUniqueIdPtr) || (NULL_PTR == ModelNrPtr) || (NULL_PTR == RevisionNrPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_IDENTIFIER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx   = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 trcvDriverId = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId).EthTrcv_GetPhyIdentifier)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(trcvDriverId)
                      .EthTrcv_GetPhyIdentifier(ethTrcvIdx, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);
        }
    }
#endif

    return ret;
}

#endif

/*  @brief           Provides access to a transmit buffer of the specified Ethernet controller. */
BufReq_ReturnType EthIf_ProvideTxBuffer(
    uint8           CtrlIdx,
    Eth_FrameType   FrameType,
    uint8           Priority,
    Eth_BufIdxType* BufIdxPtr,
    uint8**         BufPtr,
    uint16*         LenBytePtr)
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

#if (ETHIF_CTRL_HAS_CONFIGED_VLAN == STD_ON)
    uint8* IntenalBuf;
#endif

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER, errorCode);
    }
    else if ((BufIdxPtr == NULL_PTR) || (BufPtr == NULL_PTR) || (LenBytePtr == NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* @SWS_EthIf_00068: latest accepted controller mode is equal to ETH_MODE_ACTIVEor
         ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST for the given EthIfController,then the function EthIf_ProvideTxBuffer shall
         forward */
        if (ETH_MODE_ACTIVE == EthIf_ControllerInformation[CtrlIdx].CtrlMode)
        {
            const EthIf_ControllerCfgType* ctrlCfgPtr     = &ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx);
            uint8                          driverId       = ctrlCfgPtr->EthIfEthIfDriverIdx;
            uint8                          ethContrllerId = ctrlCfgPtr->EthIfEthCtrlIdx;
            SchM_Enter_EthIf_ExclusiveArea();
#if (ETHIF_CTRL_HAS_CONFIGED_VLAN != STD_ON)
            if ((ctrlCfgPtr->EthIfCtrlMtu) >= *LenBytePtr)
            {
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                          .Eth_ProvideTxBuffer(ethContrllerId, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#else
                ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                          .Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#endif
            }
#else
            if (((ctrlCfgPtr->EthIfCtrlMtu) >= *LenBytePtr) && (Priority < PRIORITY_MAX))
            {
                /* @SWS_EthIf_00147: VLAN is used  EthIf shall increment the input desired length by 4 bytes before
                 * calling */
                *LenBytePtr = (*LenBytePtr + VLAN_TAG_SIZE);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                          .Eth_ProvideTxBuffer(ethContrllerId, 0u, BufIdxPtr, BufPtr, LenBytePtr);
#else
                ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                          .Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#endif
                IntenalBuf = *BufPtr;
                if (BUFREQ_OK == ret)
                {
                    if (ETHIF_VID_UNUSED != EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId)
                    {
                        IntenalBuf[0] = (uint8)(Priority << ETHIF_VLAN_PRIORITY_BITS_SHIT_5_BITS);
                        IntenalBuf[0] &= (uint8)((uint8)(~((uint8)1u)) << ETHIF_VLAN_CFI_BIT_SHIT_4_BITS);
                        IntenalBuf[0] |= (uint8)((EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId
                                                  & ETHIF_VLAN_VLANID_MASK_HIGH_4_BITS)
                                                 >> ETHIF_SHIFT_USE_8_BITS);
                        IntenalBuf[1] = (uint8)(EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId
                                                & ETHIF_VLAN_VLANID_MASK_LOW_8_BITS);
                        IntenalBuf[ETHIF_FRAME_TYPE_POS_2_OFFSET] =
                            (uint8)((FrameType >> ETHIF_SHIFT_USE_8_BITS) & ETHIF_BIT_MAST_TO_GET_LOW_8_BITS);
                        IntenalBuf[ETHIF_FRAME_TYPE_POS_3_OFFSET] =
                            (uint8)(FrameType & ETHIF_BIT_MAST_TO_GET_LOW_8_BITS);
                        *BufPtr     = &IntenalBuf[VLAN_TAG_SIZE];
                        *LenBytePtr = (*LenBytePtr - VLAN_TAG_SIZE);
                    }
                }
                else
                {
                    ret = BUFREQ_E_NOT_OK;
                }
            }
#endif
            SchM_Exit_EthIf_ExclusiveArea();
        }
#if (STD_ON == ETHIF_SWITCH_MANAGEMENT_SUPPORT)
        if (BUFREQ_OK == ret)
        {
            /* update managment later */
        }
#endif
    }
    (void)Priority;
    (void)FrameType;
    return ret;
}

/*  @brief           Triggers transmission of a previously filled transmit buffer. */
Std_ReturnType EthIf_Transmit(
    uint8          CtrlIdx,
    Eth_BufIdxType BufIdx,
    Eth_FrameType  FrameType,
    boolean        TxConfirmation,
    uint16         LenByte,
    const uint8*   PhysAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT, errorCode);
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
#endif
    {
        SchM_Enter_EthIf_ExclusiveArea();
        uint8         driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        uint8         ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        Eth_FrameType ethFrameType   = FrameType;
        uint16        ethLenByte     = LenByte;
        if (ETHIF_VID_UNUSED != EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId)
        {
            ethFrameType = ETH_FRAME_TYPE_VLAN;
            ethLenByte   = (LenByte + VLAN_TAG_SIZE);
        }
        if ((EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfCtrlMtu >= ethLenByte)
            && (ETH_MODE_ACTIVE == EthIf_ControllerInformation[CtrlIdx].CtrlMode))
        {
            ret = ETHIF_ETH_DRIVER_API_CFG(driverId)
                      .Eth_Transmit(ethContrllerId, BufIdx, ethFrameType, TxConfirmation, ethLenByte, PhysAddrPtr);
            if ((E_OK == ret) && (EthIf_ConfigPtr->UserTxConfirmation != NULL_PTR))
            {
                for (uint8 frameOwnerIdx = 0u; frameOwnerIdx < ETHIF_FRAME_OWNER_FRAME_OWNER_NUM; frameOwnerIdx++)
                {
                    if ((FrameType == EthIf_FrameOwnerRuntimeInformation[frameOwnerIdx].FrameType) && (TxConfirmation))
                    {
                        /* make TxConfirmation to know which EthIfCtrl send this FrameType message. */
                        EthIf_FrameOwnerRuntimeInformation[frameOwnerIdx].EthIfCrlIdx = CtrlIdx;
                        EthIf_FrameOwnerRuntimeInformation[frameOwnerIdx].BufferIdx   = BufIdx;
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

#if (ETHIF_ENABLE_RX_INTERRUPT == STD_OFF)
/* @SWS_EthIf_00099: The receive frame check shall be pre compile time configurable On/Off by the configuration
 *                    parameter: EthIfEnableRxInterrupt
 * brif : The function checks for new received frames and issues reception indications in polling mode.
 */
void EthIf_MainFunctionRx(uint8 ethIfPartition)
{
    if (EthIf_InitStatus[ethIfPartition])
    {
#if ((STD_ON == ETHIF_DEV_ERROR_DETECT) && (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED))
        uint8 applicationID = (uint8)GetApplicationID();
        if (ethIfPartition != EthIf_GetEthIfPartionByEcuCParttion(applicationID))
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_RX, ETHIF_E_PARTITION_MISMATCH);
        }
        else
#endif
        {
            Eth_RxStatusType RxStatusPtr    = ETH_NOT_RECEIVED;
            uint8            lastPhyCtrlIdx = INVALID_UINT8;
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
            uint8 FifoIdx;
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */

#if (ETHIF_USED_PARTITION_NUM > 1)
            EthIf_ElementRangeType ctrlIdxRange;
            EthIf_InternalGetElementRange(EthIf_ConfigPtr->CtrlIndexRangeMap, ethIfPartition, &ctrlIdxRange);
            for (uint8 ctrlIdx = ctrlIdxRange.EqualStart; ctrlIdx < ctrlIdxRange.InequalEnd; ctrlIdx++)
#else
            for (uint8 ctrlIdx = 0u; ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM; ctrlIdx++)
#endif
            {
#if (ETHIF_USED_PARTITION_NUM > 1)
                uint8 cfgCtrlIndex = EthIf_ConfigPtr->CtrlIndexMap[ctrlIdx];
#else
                uint8 cfgCtrlIndex = ctrlIdx;
#endif
                uint8 currentPhyCtrlIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(cfgCtrlIndex).EthIfPhysControllerIdx;
                if (currentPhyCtrlIdx == lastPhyCtrlIdx)
                {
                    continue;
                }
                lastPhyCtrlIdx = currentPhyCtrlIdx;
                uint8 driverId = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(cfgCtrlIndex);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                (void)ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_Receive(currentPhyCtrlIdx, FifoIdx, &RxStatusPtr);
#else  /*The default version of eth driver is 4.2.2*/
                (void)ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_Receive(currentPhyCtrlIdx, &RxStatusPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */

                for (uint8 i = 1u; i < ETHIF_RX_INDICATION_ITERATIONS; i++)
                {
                    if (RxStatusPtr == ETH_RECEIVED_MORE_DATA_AVAILABLE)
                    {
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                        (void)ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_Receive(currentPhyCtrlIdx, FifoIdx, &RxStatusPtr);
#else  /*The default version of eth driver is 4.2.2*/
                        (void)ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_Receive(currentPhyCtrlIdx, &RxStatusPtr);
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
#endif

#if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
/* @SWS_EthIf_00100: The transmission confirmation check shall be pre compile time configurable On/Off by the
 *                   configuration parameter: EthIfEnableTxInterrupt.
 * brif : The function issues transmission confirmations in polling mode. It checks also for transceiver state changes.
 */
void EthIf_MainFunctionTx(uint8 ethIfPartition)
{
    if (EthIf_InitStatus[ethIfPartition])
    {
#if ((STD_ON == ETHIF_DEV_ERROR_DETECT) && (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED))
        uint8 applicationID = (uint8)GetApplicationID();
        if (ethIfPartition != EthIf_GetEthIfPartionByEcuCParttion(applicationID))
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_TX, ETHIF_E_PARTITION_MISMATCH);
        }
        else
#endif
        {
            uint8 lastPhyCtrlIdx = INVALID_UINT8;
#if (ETHIF_MAIN_FUNCTION_STATE_PERIOD == 0u)
            boolean reqGetLinkState = FALSE;
            EthIf_TrcvPollingCnt[ethIfPartition].TrcvLinkChgRunCount++;
            if (EthIf_TrcvPollingCnt[ethIfPartition].TrcvLinkChgRunCount >= ETHIF_TRCV_LINK_STATE_CHG_MAIN_RELOAD)
            {
                EthIf_TrcvPollingCnt[ethIfPartition].TrcvLinkChgRunCount = 0u;
                reqGetLinkState                                          = TRUE;
            }
#endif
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
            boolean reqGetSignal = FALSE;
            EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount++;
            if (EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount >= ETHIF_SIGNAL_QUALITY_CHECK_PERIOD)
            {
                EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount = 0u;
                reqGetSignal                                                  = TRUE;
            }
#endif
#if (ETHIF_USED_PARTITION_NUM > 1)
            EthIf_ElementRangeType ctrlIdxRange;
            EthIf_InternalGetElementRange(EthIf_ConfigPtr->CtrlIndexRangeMap, ethIfPartition, &ctrlIdxRange);
            for (uint8 ctrlIdx = ctrlIdxRange.EqualStart; ctrlIdx < ctrlIdxRange.InequalEnd; ctrlIdx++)
#else
            for (uint8 ctrlIdx = 0u; ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM; ctrlIdx++)
#endif
            {
#if (ETHIF_USED_PARTITION_NUM > 1)
                uint8 cfgCtrlIndex = EthIf_ConfigPtr->CtrlIndexMap[ctrlIdx];
#else
                uint8 cfgCtrlIndex = ctrlIdx;
#endif
                uint8 currentPhyCtrlIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(cfgCtrlIndex).EthIfPhysControllerIdx;
                if (currentPhyCtrlIdx == lastPhyCtrlIdx)
                {
                    continue;
                }
                lastPhyCtrlIdx = currentPhyCtrlIdx;
                uint8 driverId = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(cfgCtrlIndex);
                (void)ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_TxConfirmation(currentPhyCtrlIdx);
            }
#if (ETHIF_MAIN_FUNCTION_STATE_PERIOD == 0u)
            EthIf_InternalPollingTrcvLinkStateAndQuality(
                ethIfPartition,
                reqGetLinkState
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
                ,
                reqGetSignal
#endif
            );
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
            EthIf_InteralPollEthSwitchPortTimer(ethIfPartition);
#endif
#endif
        }
    }
}

#endif

#if (ETHIF_MAIN_FUNCTION_STATE_PERIOD > 0u)
/**
 * The function is polling different communication hardware (Ethernet
 * transceiver, Ethernet switch ports) related information, e.g. link state,
 * signal quality.
 */
void EthIf_MainFunctionState(uint8 ethIfPartition)
{
    if (EthIf_InitStatus[ethIfPartition])
    {
#if ((STD_ON == ETHIF_DEV_ERROR_DETECT) && (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED))
        uint8 applicationID = (uint8)GetApplicationID();
        if (ethIfPartition != EthIf_GetEthIfPartionByEcuCParttion(applicationID))
        {
            ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_STATE, ETHIF_E_PARTITION_MISMATCH);
        }
        else
#endif
        {
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
            boolean reqGetSignal = FALSE;
            EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount++;
            if (EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount >= ETHIF_SIGNAL_QUALITY_CHECK_PERIOD)
            {
                EthIf_TrcvPollingCnt[ethIfPartition].TrcvQualityCheckRunCount = 0u;
                reqGetSignal                                                  = TRUE;
            }
#endif
            EthIf_InternalPollingTrcvLinkStateAndQuality(
                ethIfPartition,
                TRUE
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
                ,
                reqGetSignal
#endif
            );
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
            EthIf_InteralPollEthSwitchPortTimer(ethIfPartition);
#endif
        }
    }
}
#endif

/* @brief           Called asynchronously when mode has been read out. */
void EthIf_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if ((ETHIF_PHYCONTROLLER_PHYCONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION, ETHIF_E_INV_CTRL_IDX);
    }
    else if (EthIf_DetCheckIsInvalidController(
                 ETHIF_PHYCONTROLLER_CFG_PHYCONTROLLER(CtrlIdx).EthIfEthIfCtrlId,
                 ethIfPartition,
                 &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION, errorCode);
    }
    else
#endif
    {
        /* @SWS_EthIf_00252: The function shall call EthSM_CtrlModeIndication. */
        for (uint8 idx = 0u; idx < ETHIF_CONTROLLER_CONTROLLER_NUM; idx++)
        {
            Eth_ModeType requestEthMode = EthIf_ControllerInformation[idx].ReqCtrlMode;
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R20_11)
            if (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST == requestEthMode)
            {
                requestEthMode = ETH_MODE_ACTIVE;
            }
#endif
            if ((ETHIF_CONTROLLER_CFG_CONTROLLER(idx).EthIfPhysControllerIdx == CtrlIdx) && (requestEthMode == CtrlMode)
                && (EthIf_ControllerInformation[idx].HasModeReq))
            {
                EthIf_ControllerInformation[idx].CtrlMode = CtrlMode;
                (void)EthSM_CtrlModeIndication(idx, CtrlMode);
                EthIf_ControllerInformation[idx].HasModeReq      = FALSE;
                EthIf_PhyControllerInformation[CtrlIdx].CtrlMode = CtrlMode;
            }
        }
    }
}

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0)
/* @brief   Called asynchronously when a mode change has been read out. */
void EthIf_TrcvModeIndication(uint8 TrcvIdx, Eth_ModeType TrcvMode)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION, errorCode);
    }
    else
#endif
    {
        EthIf_TransceiverInformation[TrcvIdx].TrcvMode = TrcvMode;
#if (STD_ON == ETHIF_HAST_SWITCH_PORT_REF_TRANSCEIVER)
        /* @SWS_EthIf_91055 Incase the EthSwtPort references an EthTrcv the EthIf expects a notification from the
       EthTrcv via API EthIf_TrcvModeIndication.*/
        for (uint8 idx = 0u; idx < ETHIF_SWITCHPORT_PORTS_NUM; idx++)
        {
            if (ETHIF_SWITCHPORT_CFG_SWITCHPORT(idx).ReferredEthTrcv == TrcvIdx)
            {
                EthIf_SwitchPortRuntimeInformation[idx].PortMode = TrcvMode;
            }
        }
#endif
    }
}
#endif

#if (ETHIF_SWITCH_SWITCHS_NUM > 0)
/**
 * @brief           The EthIf shall determine the expected notifications based on the EthSwtPort configuration. In
 *                  case the EthSwtPort references an EthTrcv the EthIf expects a notification from the EthTrcv via
 *                  API EthIf_TrcvModeIndication(). Otherwise the EthIf expects a notification from the EthSwt via
 *                  API EthIf_SwitchPortModeIndication().
 */
void EthIf_SwitchPortModeIndication(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_MODE_INDICATION, errorCode);
    }
    else if (SwitchPortIdx >= ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).NumberOfPorts)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_MODE_INDICATION, ETHIF_E_INV_PORT_IDX);
    }
    else
#endif
    {
        const uint8* indexPtr                                  = ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).RefEthSwtPortList;
        uint8        portIndex                                 = indexPtr[SwitchPortIdx];
        EthIf_SwitchPortRuntimeInformation[portIndex].PortMode = PortMode;
    }
}
#endif
/**
 * @brief           This API is called by the corresponding EthTrcv, if a sleep indication was detected on the network.
 * @param[in]       TrcvIdx : Index of the Ethernet transceiver within the context of the Ethernet Interface.
 * @return          None
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-63704
 */
void EthIf_SleepIndication(uint8 TrcvIdx)
{
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SLEEP_INDICATION, errorCode);
    }
    else
#endif
    {
        /* EthSM do not fulfill EthSM_SleepIndication yet */
    }
#endif
}

/*  @brief           Obtain state of the indexed transceiver. */
Std_ReturnType EthIf_GetTransceiverMode(uint8 TrcvIdx, Eth_ModeType* TrcvModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE, errorCode);
    }
    else if (TrcvModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R19_11)
        ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetTransceiverMode(ethTrcvIdx, TrcvModePtr);
#else
        EthTrcv_ModeType trcvMode;
        ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetTransceiverMode(ethTrcvIdx, &trcvMode);
        if (E_OK == ret)
        {
            *TrcvModePtr = (trcvMode == ETHTRCV_MODE_DOWN) ? (ETH_MODE_DOWN) : (ETH_MODE_ACTIVE);
        }
#endif
    }
#endif

    return ret;
}

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/**
 * @brief  Obtains the link state of the indexed switch port.
 */
Std_ReturnType EthIf_SwitchPortGetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_LINK_STATE, errorCode);
    }
    else if (NULL_PTR == LinkStatePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_LINK_STATE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        ret = EthIf_InternalSwitchPortGetLinkState(SwitchIdx, SwitchPortIdx, LinkStatePtr);
    }
    return ret;
}
#endif

/*  @brief           Obtains the link state of the indexed transceiver. */
Std_ReturnType EthIf_TransceiverGetLinkState(uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_LINK_STATE, errorCode);
    }
    else if (NULL_PTR == LinkStatePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_LINK_STATE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetLinkState)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetLinkState(ethTrcvIdx, LinkStatePtr);
        }
    }
#endif

    return ret;
}

#if (ETHIF_GET_BAUDRATE == STD_ON)
/*  @brief           Obtains the baud rate of the indexed transceiver. */
Std_ReturnType EthIf_TransceiverGetBaudRate(uint8 TrcvIdx, EthTrcv_BaudRateType* BaudRatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_BAUD_RATE, errorCode);
    }
    else if (NULL_PTR == BaudRatePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_BAUD_RATE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetBaudRate)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetBaudRate(ethTrcvIdx, BaudRatePtr);
        }
    }
#endif

    return ret;
}
#endif

/*  @brief           Obtains the duplex mode of the indexed transceiver.*/
Std_ReturnType EthIf_TransceiverGetDuplexMode(uint8 TrcvIdx, EthTrcv_DuplexModeType* DuplexModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_DUPLEX_MODE, errorCode);
    }
    else if (NULL_PTR == DuplexModePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_DUPLEX_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetDuplexMode)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetDuplexMode(ethTrcvIdx, DuplexModePtr);
        }
    }
#endif

    return ret;
}

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/*  @brief         Request the MgmtObject of the (in this context) unique DataPtr. */
Std_ReturnType EthIf_GetRxMgmtObject(uint8 CtrlIdx, Eth_DataType* DataPtr, EthSwt_MgmtObjectType** MgmtObjectPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_RX_MGMT_OBJECT, errorCode);
    }
    else if ((NULL_PTR == DataPtr) || (NULL_PTR == MgmtObjectPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_RX_MGMT_OBJECT, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwt = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).TrcvRefIdx;
        if (ethSwt < ETHIF_SWITCH_SWITCHS_NUM)
        {
            uint8 driverId = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethSwt);
            if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetRxMgmtObject)
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetRxMgmtObject(CtrlIdx, DataPtr, MgmtObjectPtr);
            }
        }
    }

    return ret;
}

/*  @brief         Request the MgmtObject of the (in this context) unique BufIdx. */
Std_ReturnType EthIf_GetTxMgmtObject(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtObjectType** MgmtObjectPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT, errorCode);
    }
    else if (NULL_PTR == MgmtObjectPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT, ETHIF_E_PARAM_POINTER);
    }
    else if (BufIdx >= ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TX_MGMT_OBJECT, ETHIF_E_INV_PARAM);
    }
    else
#endif
    {
        uint8 ethSwt = ETHIF_CONTROLLER_CFG_CONTROLLER(CtrlIdx).TrcvRefIdx;
        if (ethSwt < ETHIF_SWITCH_SWITCHS_NUM)
        {
            uint8 driverId = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethSwt);
            if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetTxMgmtObject)
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetTxMgmtObject(CtrlIdx, BufIdx, MgmtObjectPtr);
            }
        }
    }
    return ret;
}

/* @brief         Obtains the port over which this MAC-address can be reached. */
Std_ReturnType EthIf_GetPortMacAddr(const uint8* MacAddrPtr, uint8* SwtichIdxPtr, uint8* PortIdxPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (ethIfPartition >= ETHIF_USED_PARTITION_NUM)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR, ETHIF_E_INVALID_PARTITION);
    }
    else if (!EthIf_InitStatus[ethIfPartition])
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR, ETHIF_E_UNINIT);
    }
    else if ((MacAddrPtr == NULL_PTR) || (NULL_PTR == SwtichIdxPtr) || (NULL_PTR == PortIdxPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MAC_ADDR, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        for (uint8 ethIfSwtIdx = 0u; ethIfSwtIdx < ETHIF_SWITCH_SWITCHS_NUM; ethIfSwtIdx++)
        {
            uint8 ethSwtDriverId = EthIf_ConfigPtr->EthIfSwitchCfg[ethIfSwtIdx].EthIfEthSwtDriverId;
            if ((NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDriverId).EthSwt_GetPortMacAddr)
#if ((STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED) && (STD_ON == ETHIF_DEV_ERROR_DETECT))
                && (ethIfPartition == ETHIF_SWITCH_CFG_SWITCH(ethIfSwtIdx).EthIfPartition)
#endif
            )
            {
                uint8 switchIdx = EthIf_ConfigPtr->EthIfSwitchCfg[ethIfSwtIdx].EthIfEthSwtIdx;
                ret             = ETHIF_SWITCH_DRIVER_API_CFG(ethSwtDriverId)
                          .EthSwt_GetPortMacAddr(switchIdx, MacAddrPtr, PortIdxPtr);
                /* @SWS_EthSwt_00060 If for the PortIdxPtr the maximal possible value (255) is returned
                the given MAC address cannot be reached via a port of this switch. */
                if ((E_OK == ret) && (INVALID_UINT8 != (*PortIdxPtr)))
                {
                    *SwtichIdxPtr = switchIdx;
                    break;
                }
            }
        }
    }

    return ret;
}

/*  @brief         Obtains the address resolution table of a switch and copies the list into a user provided buffer. */
Std_ReturnType EthIf_GetArlTable(uint8 switchIdx, uint16* numberOfElements, Eth_MacVlanType* arlTableListPointer)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(switchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_ARL_TABEL, errorCode);
    }
    else if ((NULL_PTR == numberOfElements) || ((*numberOfElements != 0u) && (NULL_PTR == arlTableListPointer)))
    {
        /* @SWS_EthIf_00196: If input value of numberOf Elements is 0 the function will not copy any data but only
           return the number of valid entries in the cache arlTableListPointer may be NULL_PTR in this case */
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_ARL_TABEL, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwitchIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(switchIdx);
        uint8 driverId     = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(switchIdx);
        ret                = ETHIF_SWITCH_DRIVER_API_CFG(driverId).EthSwt_GetArlTable(
            ethSwitchIdx,
            numberOfElements,
            arlTableListPointer);
    }
    return ret;
}

/*  @brief           Obtains the duplex mode of the indexed switch port. */
Std_ReturnType
    EthIf_SwitchPortGetDuplexMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_DuplexModeType* DuplexModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_DUPLEX_MODE, errorCode);
    }
    else if (NULL_PTR == DuplexModePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_DUPLEX_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetDuplexMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetDuplexMode(ethSwtIdx, SwitchPortIdx, DuplexModePtr);
        }
    }
    return ret;
}

/*  @brief           Reads a list with drop counter values of the corresponding port of the switch. */
Std_ReturnType EthIf_SwitchPortGetCounterValues(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_CounterType* CounterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_COUNTER_VALUE, errorCode);
    }
    else if (NULL_PTR == CounterPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_COUNTER_VALUE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCounterValues)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCounterValues(ethSwtIdx, SwitchPortIdx, CounterPtr);
        }
    }

    return ret;
}

/*  @brief           Returns a list of statistic counters defined with Eth_RxTatsType. The majority of these Counters
 * are derived from the IETF RFC2819. */
Std_ReturnType EthIf_SwitchPortGetRxStats(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_RxStatsType* RxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_RX_STATS, errorCode);
    }
    else if (NULL_PTR == RxStatsPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_RX_STATS, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetRxStats)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetRxStats(ethSwtIdx, SwitchPortIdx, RxStatsPtr);
        }
    }
    return ret;
}

/*  @brief           List of values to read statistic values for transmission. */
Std_ReturnType EthIf_SwitchPortGetTxStats(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_TxStatsType* TxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_STATS, errorCode);
    }
    else if (NULL_PTR == TxStatsPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_STATS, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetTxStats)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetTxStats(ethSwtIdx, SwitchPortIdx, TxStatsPtr);
        }
    }
    return ret;
}

/*  @brief           List of values to read statistic error counter values for transmission from. */
Std_ReturnType EthIf_SwitchPortGetTxErrorCounterValues(
    uint8                         SwitchIdx,
    uint8                         SwitchPortIdx,
    Eth_TxErrorCounterValuesType* TxStatsPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_ERROR_COUNTER_VALUE, errorCode);
    }
    else if (NULL_PTR == TxStatsPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_TX_ERROR_COUNTER_VALUE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetTxErrorCounterValues)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetTxErrorCounterValues(
                ethSwtIdx,
                SwitchPortIdx,
                TxStatsPtr);
        }
    }

    return ret;
}

/*  @brief           Returns the MAC learning mode. */
Std_ReturnType
    EthIf_SwitchPortGetMacLearningMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAC_LEARNING_MODE, errorCode);
    }
    else if (NULL_PTR == MacLearningModePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAC_LEARNING_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetMacLearningMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetMacLearningMode(
                ethSwtIdx,
                SwitchPortIdx,
                MacLearningModePtr);
        }
    }

    return ret;
}

/* @brief           Returns the MAC learning mode. */
Std_ReturnType EthIf_GetSwitchPortIdentifier(
    uint8   SwitchIdx,
    uint8   SwitchPortIdx,
    uint32* OrgUniqueIdPtr,
    uint8*  ModelNrPtr,
    uint8*  RevisionNrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_IDENTIFIER, errorCode);
    }
    else if ((NULL_PTR == OrgUniqueIdPtr) || (NULL_PTR == ModelNrPtr) || (NULL_PTR == RevisionNrPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_IDENTIFIER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortIdentifier)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx)
                      .EthSwt_GetPortIdentifier(ethSwtIdx, SwitchPortIdx, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);
        }
    }

    return ret;
}

/*  @brief            Obtain the Organizationally Unique Identifier that is given by the IEEE of the indexed Ethernet
 * switch.*/
Std_ReturnType EthIf_GetSwitchIdentifier(uint8 SwitchIdx, uint32* OrgUniqueIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_IDENTIFIER, errorCode);
    }
    else if (NULL_PTR == OrgUniqueIdPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_IDENTIFIER, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetSwitchIdentifier)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetSwitchIdentifier(ethSwtIdx, OrgUniqueIdPtr);
        }
    }

    return ret;
}

/*  @brief           Store the given port mirror configuration in a shadow buffer in the Ethernet switch driver for the
 * given MirroredSwitchIdx. */
Std_ReturnType EthIf_WritePortMirrorConfiguration(
    uint8                           MirroredSwitchIdx,
    const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(MirroredSwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_WRITE_PORT_MIRROR_CONFIGURATION, errorCode);
    }
    else if (NULL_PTR == PortMirrorConfigurationPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_WRITE_PORT_MIRROR_CONFIGURATION, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(MirroredSwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(MirroredSwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_WritePortMirrorConfiguration)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_WritePortMirrorConfiguration(
                ethSwtIdx,
                PortMirrorConfigurationPtr);
        }
    }

    return ret;
}

/* @brief           Obtain the port mirror configuration of the given Ethernet switch. */
Std_ReturnType
    EthIf_ReadPortMirrorConfiguration(uint8 MirroredSwitchIdx, EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(MirroredSwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_READ_PORT_MIRROR_CONFIGURATION, errorCode);
    }
    else if (NULL_PTR == PortMirrorConfigurationPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_READ_PORT_MIRROR_CONFIGURATION, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(MirroredSwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(MirroredSwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_ReadPortMirrorConfiguration)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_ReadPortMirrorConfiguration(
                ethSwtIdx,
                PortMirrorConfigurationPtr);
        }
    }

    return ret;
}

/*  @brief           Delete the stored port mirror configuration of the given MirroredSwitchIdx. */
Std_ReturnType EthIf_DeletePortMirrorConfiguration(uint8 MirroredSwitchIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(MirroredSwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_DELETE_PORT_MIRROR_CONFIGURATION, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(MirroredSwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(MirroredSwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_DeletePortMirrorConfiguration)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_DeletePortMirrorConfiguration(ethSwtIdx);
        }
    }

    return ret;
}

/*  @brief           Obtain the current status of the port mirroring for the indexed Ethernet switch port. */
Std_ReturnType EthIf_GetPortMirrorState(uint8 SwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType* PortMirrorStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MIRROR_STATE, errorCode);
    }
    else if (NULL_PTR == PortMirrorStatePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_MIRROR_STATE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortMirrorState)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortMirrorState(
                ethSwtIdx,
                PortIdx,
                PortMirrorStatePtr);
        }
    }

    return ret;
}

/* @brief  Request to set the given port mirroring state of the port mirror configuration for the given Ethernet switch.
 */
Std_ReturnType
    EthIf_SetPortMirrorState(uint8 MirroredSwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType PortMirrorState)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(MirroredSwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_MIRROR_STATE, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(MirroredSwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(MirroredSwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortMirrorState)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortMirrorState(ethSwtIdx, PortMirrorState);
        }
        (void)PortIdx;
    }

    return ret;
}

/*  @brief           Activates a given test mode of the indexed Ethernet switch port. */
Std_ReturnType EthIf_SetPortTestMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTestModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TEST_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortTestMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortTestMode(ethSwtIdx, PortIdx, Mode);
        }
    }

    return ret;
}

/* @brief           Activates a given test loop-back mode of the indexed Ethernet switch port. */
Std_ReturnType EthIf_SetPortLoopbackMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyLoopbackModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_LOOPBACK_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortLoopbackMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortLoopbackMode(ethSwtIdx, PortIdx, Mode);
        }
    }

    return ret;
}

/*  @brief           Activates a given transmission mode of the indexed Ethernet switch port. */
Std_ReturnType EthIf_SetPortTxMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTxModeType Mode)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PORT_TX_MODE, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortTxMode)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetPortTxMode(ethSwtIdx, PortIdx, Mode);
        }
    }
    return ret;
}

/**
 * @brief            Retrieves the cable diagnostics result of the indexed Ethernet switch port respectively the
 *                   referenced Ethernet Transceiver Driver. */
Std_ReturnType
    EthIf_GetPortCableDiagnosticsResult(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_CableDiagResultType* ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT, errorCode);
    }
    else if (NULL_PTR == ResultPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PORT_CABLE_DIAGNOSTICS_RESULT, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortCableDiagnosticsResult)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortCableDiagnosticsResult(
                ethSwtIdx,
                PortIdx,
                ResultPtr);
        }
    }

    return ret;
}

/**
 * @brief            Trigger the cable diagnostics of the given Ethernet Switch port (PortIdx) by calling EthTrcv_Run
 *                    CableDiagnostic of the referenced Ethernet transceiver. */
Std_ReturnType EthIf_RunPortCableDiagnostic(uint8 SwitchIdx, uint8 PortIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RUN_PORT_CABLE_DIAGNOSTIC, errorCode);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        for (uint8 idx = 0u; idx < ETHIF_SWITCHPORT_PORTS_NUM; idx++)
        {
            /* @SWS_EthIf_00464:  the corresponding EthIfController is in mode ETH_MODE_ACTIVE and forward the call */
            if ((ETHIF_SWITCHPORT_CFG_SWITCHPORT(idx).EthIfEthIfSwtIdx == SwitchIdx)
                && (ETHIF_SWITCHPORT_CFG_SWITCHPORT(idx).EthPortIdx == PortIdx)
                && (EthIf_SwitchPortRuntimeInformation[idx].PortMode == ETH_MODE_ACTIVE)
                && (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_RunPortCableDiagnostic))
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_RunPortCableDiagnostic(ethSwtIdx, PortIdx);
                break;
            }
        }
    }

    return ret;
}

#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
/*  @brief           Trigger the cable diagnostics for the given Ethernet transceiver. */
Std_ReturnType EthIf_RunCableDiagnostic(uint8 TrcvIdx)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RUN_CABLE_DIAGNOSTIC, errorCode);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        /* @SWS_EthIf_00466  ensure that the corresponding EthIfController is in mode ETH_MODE_ACTIVE */
        if ((NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_RunCableDiagnostic)
            && (ETH_MODE_ACTIVE == EthIf_TransceiverInformation[TrcvIdx].TrcvMode))
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_RunCableDiagnostic(ethTrcvIdx);
        }
    }
#endif

    return ret;
}
#endif

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R20_11)
/*  @brief           Obtains the media access mode of the transceiver.  */
Std_ReturnType EthIf_TransceiverGetMacMethod(uint8 TrcvIdx, EthTrcv_MacMethodType* MacModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidTransceiver(TrcvIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_MAC_METHOD, errorCode);
    }
    else if (NULL_PTR == MacModePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSCEIVER_GET_MAC_METHOD, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
        uint8 driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
        if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetMacMethod)
        {
            ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetMacMethod(ethTrcvIdx, MacModePtr);
        }
    }
#endif

    return ret;
}
#endif

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
/*  @brief           When MACPHY controller are used, obtains the SPI interface status. */
Std_ReturnType EthIf_EthGetSpiStatus(uint8 CtrlIdx, Eth_SpiStatusType* SpiStatusPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidController(CtrlIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ETH_GET_SPI_STATUS, errorCode);
    }
    else if (NULL_PTR == SpiStatusPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ETH_GET_SPI_STATUS, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(CtrlIdx);
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(CtrlIdx);
        if (NULL_PTR != ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetSpiStatus)
        {
            ret = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_GetSpiStatus(ethContrllerId, SpiStatusPtr);
        }
    }

    return ret;
}
#endif
/* ========================================== internal function definition ========================================== */

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R20_11)
/* @Brief   handle controller mode request of TxoffLine. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeTxOffLine(uint8 ctrlIdx)
{
    Std_ReturnType ret = E_NOT_OK;
    /* @SWS_EthIf_00504 ETH_MODE_ACTIVE or ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST then ETH_MODE_TX_OFFLINE
        shall be stored as current controller mode.Otherwise the requested controller mode shall return E_NOT_OK. */
    if (ETH_MODE_ACTIVE == EthIf_ControllerInformation[ctrlIdx].CtrlMode)
    {
        /* @SWS_EthIf_00504: ETH_MODE_ACTIVE_TX_OFFLINE shall be stored as current controller mode.  */
        EthIf_ControllerInformation[ctrlIdx].CtrlMode   = ETH_MODE_ACTIVE_TX_OFFLINE;
        EthIf_ControllerInformation[ctrlIdx].HasModeReq = FALSE;
        ret                                             = E_OK;
    }
    return ret;
}

/* @Brief   handle controller mode request of active with wakeup request. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeActiveWithWakeup(uint8 ctrlIdx)
{
    Std_ReturnType ret;

    ret = EthIf_InternalSetEthCtrlMode(ctrlIdx, ETH_MODE_ACTIVE);
    EthIf_InternalSetCtrlModeSetHardwareMode(ctrlIdx, ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST, ETH_MODE_DOWN);

    return ret;
}

#endif
/* @Brief   handle controller mode request of ACTIVE. */
ETHIF_LOCAL_INLINE Std_ReturnType EthIf_InternalSetCtrlModeActive(uint8 ctrlIdx)
{
    Std_ReturnType ret;

    ret = EthIf_InternalSetEthCtrlMode(ctrlIdx, ETH_MODE_ACTIVE);
    EthIf_InternalSetCtrlModeSetHardwareMode(ctrlIdx, ETH_MODE_ACTIVE, ETH_MODE_DOWN);
    return ret;
}

/* @Brief   handle controller mode request of Down. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetEthCtrlModeDown(uint8 ctrlIdx)
{
    /* @SWS_EthIf_00263: if EthIf_SetControllerMode has been called with mode ETH_-MODE_DOWN for all Ethernet
        Interface Controller referencing the Ethernet Controller.  */
    Std_ReturnType ret            = E_OK;
    boolean        reqPhyCtrlDown = TRUE;
    uint8          phyCtrl        = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).EthIfPhysControllerIdx;
    uint8          ctrlNum        = ETHIF_CONTROLLER_CONTROLLER_NUM;

    for (uint8 index = 0u; index < ctrlNum; index++)
    {
        if ((phyCtrl == ETHIF_CONTROLLER_CFG_CONTROLLER(index).EthIfPhysControllerIdx)
            && ((EthIf_ControllerInformation[index].ReqCtrlMode != ETH_MODE_DOWN)
                && (EthIf_ControllerInformation[index].CtrlMode != ETH_MODE_DOWN)))
        {
            reqPhyCtrlDown = FALSE;
            break;
        }
    }

    if (reqPhyCtrlDown)
    {
        ret = EthIf_InternalSetEthCtrlMode(ctrlIdx, ETH_MODE_DOWN);
        EthIf_InternalSetCtrlModeSetHardwareMode(ctrlIdx, ETH_MODE_DOWN, ETH_MODE_ACTIVE);
    }
    else
    {
        EthIf_ControllerInformation[ctrlIdx].CtrlMode   = ETH_MODE_DOWN;
        EthIf_ControllerInformation[ctrlIdx].HasModeReq = FALSE;
        (void)EthSM_CtrlModeIndication(ctrlIdx, ETH_MODE_DOWN);
    }
    return ret;
}

/* @Brief   set mode or state of ethernet hardwar(transceiver, switch and switchportGroup) trigger by controller mode
 * request.*/
ETHIF_LOCAL void EthIf_InternalSetCtrlModeSetHardwareMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode)
{
    if (CONTROLLER_REF_TRANSCEIVER == ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefType)
    {
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
        EthIf_InternalSetCtrlModeSetTrcvMode(ctrlIdx, setMode, checkMode);
#endif
    }
#if (ETHIF_SWITCH_SWITCHS_NUM > 0)
    else if (CONTROLLER_REF_SWITCH == ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefType)
    {
        EthIf_InternalSetCtrlModeSetSwitchPortMode(ctrlIdx, setMode, checkMode);
    }
#endif
#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
    else if (CONTROLLER_REF_SWITCH_PORT_GROUP == ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefType)
    {
        uint8 ethIfGroupIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefIdx;
        if (ETHIF_SWITCH_PORT_GROUP_CONTROL
            == ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(ethIfGroupIdx).EthIfPortGroupRefSemantics)
        {
            EthIf_InternalSetCtrlModeSetSwitchPortGroupMode(ctrlIdx, setMode, checkMode);
        }
    }
#endif
    else
    {
        /* CONTROLLER_REF_NO_HARDWARE */
        EthIf_ControllerInformation[ctrlIdx].NoHardwareRefReportLinkChg = TRUE;
    }
}
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
/* @Brief   set mode or state of ethernet hardwar(transceiver) trigger by controller mode request. */
ETHIF_LOCAL void EthIf_InternalSetCtrlModeSetTrcvMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode)
{
    uint8 ethIfTrcvIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefIdx;
    /* @SWS_EthIf_00266 : reference to an EthIfTransceiver, ETH_MODE_ACTIVE requecst check is ETH_MODE_DOWN */
    /* @SWS_EthIf_00484 : reference to an EthIfTransceiver, ETH_MODE_DOWN requecst check is ETH_MODE_ACTIVE */
    /* @SWS_EthIf_00479 : ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST independent of the current mode */
    if ((checkMode == EthIf_TransceiverInformation[ethIfTrcvIdx].TrcvMode)
        || (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST == setMode))
    {
        EthTrcv_LinkStateType reqTrcvState;
        EthIf_InternalGetTrcvStateCoupledWithMode(setMode, &reqTrcvState);
        uint8 driverId   = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(ethIfTrcvIdx);
        uint8 ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(ethIfTrcvIdx);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R19_11)
        (void)ETHIF_ETHTRCV_DRIVER_API_CFG(driverId).EthTrcv_SetTransceiverMode(ethTrcvIdx, setMode);
#else
        (void)ETHIF_ETHTRCV_DRIVER_API_CFG(driverId).EthTrcv_SetTransceiverMode(ethTrcvIdx, (EthTrcv_ModeType)setMode);
        /* PRQA S 4322 */ /* VL_EthIf_TypeBeingCastToDifferentEnumType */
#endif
        (void)ETHIF_ETHTRCV_DRIVER_API_CFG(driverId).EthTrcv_TransceiverLinkStateRequest(ethTrcvIdx, reqTrcvState);
    }
}
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* @brief           Obtains the mode of the indexed switch port.*/
Std_ReturnType EthIf_GetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* PortModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE, errorCode);
    }
    else if (SwitchPortIdx >= ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).NumberOfPorts)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE, ETHIF_E_INV_PORT_IDX);
    }
    else if (NULL_PTR == PortModePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_SWITCH_PORT_MODE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetSwitchPortMode)
        {
            ret =
                ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetSwitchPortMode(ethSwtIdx, SwitchPortIdx, PortModePtr);
        }
    }
    return ret;
}

/* @Brief   set mode or state of ethernet hardwar(switch trigger by controller mode request. */
ETHIF_LOCAL void EthIf_InternalSetCtrlModeSetSwitchPortMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode)
{
    uint8 ethIfSwtIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefIdx;
    uint8 ethSwtIdx   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
    uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
    uint8 portNum     = ETHIF_SWITCH_CFG_SWITCH(ethIfSwtIdx).NumberOfPorts;
    /* @SWS_EthIf_00478 : reference to an EthIfSwitch, ETH_MODE_ACTIVE requecst check is ETH_MODE_DOWN */
    /* @SWS_EthIf_00485 : reference to an EthIfSwitch, ETH_MODE_DOWN requecst check is ETH_MODE_ACTIVE */
    /* @SWS_EthIf_00480 : ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST independent of the current mode */

    EthTrcv_LinkStateType reqTrcvState;
    EthIf_InternalGetTrcvStateCoupledWithMode(setMode, &reqTrcvState);
    if (ETH_MODE_DOWN != setMode)
    {
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            const uint8* indexPtr  = ETHIF_SWITCH_CFG_SWITCH(ethIfSwtIdx).RefEthSwtPortList;
            uint8        portIndex = indexPtr[iterateIdx];
            EthIf_SwitchPortRuntimeInformation[portIndex].StartActiveTimer = 0u;
            if ((checkMode == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
                || (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST == setMode))
            {
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(
                        ethSwtIdx,
                        ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx,
                        setMode);
                }
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                        ethSwtIdx,
                        ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx,
                        reqTrcvState);
                }
            }
        }
    }
    else
    {
        /* @SWS_EthIf_00485 : ETH_MODE_DOWN request, call state request before calling port mode */
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            const uint8* indexPtr  = ETHIF_SWITCH_CFG_SWITCH(ethIfSwtIdx).RefEthSwtPortList;
            uint8        portIndex = indexPtr[iterateIdx];
            if (checkMode == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
            {
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                        ethSwtIdx,
                        ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx,
                        reqTrcvState);
                }
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(
                        ethSwtIdx,
                        ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx,
                        setMode);
                }
            }
        }
    }
}

#if (ETHIF_GET_BAUDRATE == STD_ON)
/*  @brief           Obtains the baud rate of the indexed switch port. */
Std_ReturnType EthIf_SwitchPortGetBaudRate(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_BaudRateType* BaudRatePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_BAUD_RATE, errorCode);
    }
    else if (NULL_PTR == BaudRatePtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_BAUD_RATE, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetBaudRate)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetBaudRate(ethSwtIdx, SwitchPortIdx, BaudRatePtr);
        }
    }
    return ret;
}
#endif

/* @brief           Retrieves the data in memory of the indexed Ethernet switch in variable length. */
Std_ReturnType EthIf_SwitchGetCfgDataRaw(uint8 SwitchIdx, uint32 Offset, uint16 Length, uint8* BufferPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW, errorCode);
    }
    else if (NULL_PTR == BufferPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_RAW, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCfgDataRaw)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCfgDataRaw(ethSwtIdx, Offset, Length, BufferPtr);
        }
    }

    return ret;
}

/*  @brief           Retrieves the data in memory of the indexed Ethernet switch in variable length. */
Std_ReturnType EthIf_SwitchGetCfgDataInfo(uint8 SwitchIdx, uint32* DataSizePtr, uint32* DataAdressPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO, errorCode);
    }
    else if ((NULL_PTR == DataSizePtr) || (NULL_PTR == DataAdressPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_GET_CFG_DATA_INFO, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCfgDataInfo)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetCfgDataInfo(ethSwtIdx, DataSizePtr, DataAdressPtr);
        }
    }

    return ret;
}

/**
 * @brief           The function retrieves the maximum amount of allocated FIFO buffer of the indexed Ethernet switch
 *                    egress port. If the Ethernet switch hardware does not support Ethernet switch port based maximal
 *                    FIFO buffer level, the content of SwitchPortEgressFifoBufferLevelPtr shall be set to 0xFFFFFFFF.
 *                    This API may be called by e.g. a CDD. */
Std_ReturnType EthIf_SwitchPortGetMaxFIFOBufferFillLevel(
    uint8   SwitchPortIdx,
    uint8   PortIdx,
    uint8   SwitchPortEgressFifoIdx,
    uint32* SwitchPortEgressFifoBufferLevelPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if (EthIf_DetCheckIsInvalidSwitch(SwitchPortIdx, ethIfPartition, &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL, errorCode);
    }
    else if (NULL_PTR == SwitchPortEgressFifoBufferLevelPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SWITCH_PORT_GET_MAX_FIFO_BUFFER_FILL_LEVEL, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchPortIdx);
        uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchPortIdx);
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetMaxFIFOBufferFillLevel)
        {
            ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetMaxFIFOBufferFillLevel(
                ethSwtIdx,
                PortIdx,
                SwitchPortEgressFifoIdx,
                SwitchPortEgressFifoBufferLevelPtr);
        }
    }

    return ret;
}

#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/* @Brief   set mode or state of ethernet hardwar(switchportGroup) trigger by controller mode request. */
ETHIF_LOCAL void
    EthIf_InternalSetCtrlModeSetSwitchPortGroupMode(uint8 ctrlIdx, Eth_ModeType setMode, Eth_ModeType checkMode)
{
    uint8        ethIfGroupIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefIdx;
    const uint8* indexPtr      = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(ethIfGroupIdx).RefEthSwtPortList;
    uint8        portNum       = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(ethIfGroupIdx).EthSwtPortNum;
    /* @SWS_EthIf_00264 : reference to an EthIfSwitchPortGroup, ETH_MODE_ACTIVE requecst check is ETH_MODE_DOWN */
    /* @SWS_EthIf_00265 : reference to an EthIfSwitchPortGroup, ETH_MODE_DOWN requecst check is ETH_MODE_ACTIVE */
    /* @SWS_EthIf_00481 : ref EthIfSwitchPortGroup,ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST independent of the current mode
     */

    EthTrcv_LinkStateType reqTrcvState;
    EthIf_InternalGetTrcvStateCoupledWithMode(setMode, &reqTrcvState);
    if (ETH_MODE_DOWN != setMode)
    {
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            uint8 portIndex                                                = indexPtr[iterateIdx];
            EthIf_SwitchPortRuntimeInformation[portIndex].StartActiveTimer = 0u;
            if ((checkMode == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
                || (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST == setMode))
            {
                uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthIfEthIfSwtIdx;
                uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx;
                uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
                uint8 ethSwitch   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(ethSwitch, port, setMode);
                }

                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                        ethSwitch,
                        port,
                        reqTrcvState);
                }
            }
        }
    }
    else
    {
        /* @SWS_EthIf_00265 : ETH_MODE_DOWN request, call state request before calling port mode */
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            uint8 portIndex = indexPtr[iterateIdx];
            if (checkMode == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
            {
                uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthIfEthIfSwtIdx;
                uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx;
                uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
                uint8 ethSwitch   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                        ethSwitch,
                        port,
                        reqTrcvState);
                }
                if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode)
                {
                    (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(ethSwitch, port, setMode);
                }
            }
        }
    }
}
#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
/* @Brief   set mode or state of ethernet switchportGroup trigger by BswM. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetSwitchPortGroupMode(uint8 ethIfGroupIdx, Eth_ModeType setMode)
{
    Std_ReturnType ret      = E_NOT_OK;
    const uint8*   indexPtr = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(ethIfGroupIdx).RefEthSwtPortList;
    uint8          portNum  = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(ethIfGroupIdx).EthSwtPortNum;

    /* @SWS_EthIf_00265 : reference to an EthIfSwitchPortGroup, ETH_MODE_DOWN requecst check is ETH_MODE_ACTIVE */
    /* @SWS_EthIf_00481 : ref EthIfSwitchPortGroup,ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST independent of the current mode
     */
    EthTrcv_LinkStateType reqTrcvState;
    EthIf_InternalGetTrcvStateCoupledWithMode(setMode, &reqTrcvState);

    if (ETH_MODE_DOWN == setMode)
    {
        EthIf_SwitchPortGroupRuntimeInformation[ethIfGroupIdx].IsNotRequestModeDown = FALSE;
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            uint8 portIndex   = indexPtr[iterateIdx];
            uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthIfEthIfSwtIdx;
            uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx;
            /* @SWS_EthIf_00270 : EthIf_SwitchPortGroupRequestMode is called with ETH_MODE_DOWN EthIf shall start a
            timer with EthIfSwitchOffPortTimedelay for all ports of the respective EthIf_SwitchPortGroup if the mode
            ETH_MODE_DOWN has been requested for all EthIfSwitchPortGroups referencing the port and the current mode is
            ETH_MODE_ACTIVE. */
            if ((ETH_MODE_ACTIVE == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
                && (EthIf_InternalIsALLPortGroupRequestModeDown(ethIfSwtIdx, port)))
            {
                EthIf_SwitchPortRuntimeInformation[portIndex].SwitchOffPortTimer =
                    EthIf_ConfigPtr->SwitchOffPortTimeDelayCnt;
            }
        }
        ret = E_OK;
    }
    else
    {
        EthIf_SwitchPortGroupRuntimeInformation[ethIfGroupIdx].IsNotRequestModeDown = TRUE;
        for (uint8 iterateIdx = 0u; iterateIdx < portNum; iterateIdx++)
        {
            uint8 portIndex   = indexPtr[iterateIdx];
            uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthIfEthIfSwtIdx;
            uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(portIndex).EthPortIdx;
            uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
            uint8 ethSwitch   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
            /* @SWS_EthIf_00272 : been called with ETH_MODE_ACTIVE, Call EthSwt_SetSwitchPortMode with ETH_MODE_ACTIVE,
               if the current mode is ETH_MODE_DOWN. */
            /* @SWS_EthIf_00482 : called with ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST, independent of the current EthSwtPort
             * mode. */
            EthIf_SwitchPortRuntimeInformation[portIndex].StartActiveTimer = 0u;
            if ((((ETH_MODE_DOWN == EthIf_SwitchPortRuntimeInformation[portIndex].PortMode)
                  && (ETH_MODE_ACTIVE == setMode))
                 || (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST == setMode))
                && (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode))
            {
                ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(ethSwitch, port, setMode);
            }
            else
            {
                ret = E_OK;
            }
            /* @SWS_EthIf_00483  */
            EthIf_SwitchPortRuntimeInformation[portIndex].SwitchOffPortTimer = 0u;

            if (E_OK != ret)
            {
                break;
            }

            /* @SWS_EthIf_00272 : been called with ETH_MODE_ACTIVE, Call EthSwt_PortLinkStateRequest with
               ETHTRCV_LINK_STATE_ACTIVE, if the current link state is ETHTRCV_LINK_STATE_DOWN. */
            /* @SWS_EthIf_00482 : called with ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST, with ETHTRCV_LINK_STATE_ACTIVE,
                   only if current link state is ETHTRCV_LINK_STATE_DOWN. */
            if ((ETHTRCV_LINK_STATE_DOWN == EthIf_SwitchPortRuntimeInformation[portIndex].PortLinkState)
                && (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest))
            {
                (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(ethSwitch, port, reqTrcvState);
            }
        }
    }
    return ret;
}

/* @brief Is all switchPortGroup referenced same port are requested down mode. */
ETHIF_LOCAL boolean EthIf_InternalIsALLPortGroupRequestModeDown(uint8 ethIfSwtIdx, uint8 portIdx)
{
    boolean ret = TRUE;
    /* @SWS_EthIf_00270:  shall start a timer with EthIfSwitchOffPortTimedelay for all ports
        of the respective EthIf_SwitchPortGroup if the mode ETH_MODE_DOWN has been requested
        for all EthIfSwitchPortGroups referencing the port. */
    for (uint8 groupIdx = 0u; groupIdx < ETHIF_SWITCH_PORT_GROUP_GROUP_NUM; groupIdx++)
    {
        const uint8* indexPtr = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(groupIdx).RefEthSwtPortList;
        uint8        portNum  = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(groupIdx).EthSwtPortNum;
        for (uint8 idx; idx < portNum; idx++)
        {
            uint8 portCfgIndex = indexPtr[idx];
            if ((ethIfSwtIdx == ETHIF_SWITCHPORT_CFG_SWITCHPORT(portCfgIndex).EthIfEthIfSwtIdx)
                && (portIdx == ETHIF_SWITCHPORT_CFG_SWITCHPORT(portCfgIndex).EthPortIdx)
                && (EthIf_SwitchPortGroupRuntimeInformation[groupIdx].IsNotRequestModeDown))
            {
                ret = FALSE;
                break;
            }
        }
        if (!ret)
        {
            break;
        }
    }
    return ret;
}
#endif

/* @Brief   get port state request by requested transceiver mode. */
ETHIF_LOCAL_INLINE void
    EthIf_InternalGetTrcvStateCoupledWithMode(Eth_ModeType trcvMode, EthTrcv_LinkStateType* trcvState)
{
    if (ETH_MODE_DOWN == trcvMode)
    {
        *trcvState = ETHTRCV_LINK_STATE_DOWN;
    }
    else
    {
        *trcvState = ETHTRCV_LINK_STATE_ACTIVE;
    }
}

/* @Brief   exceute request ethernet driver to set controller mode. */
ETHIF_LOCAL Std_ReturnType EthIf_InternalSetEthCtrlMode(uint8 ctrlIdx, Eth_ModeType mode)
{
    Std_ReturnType ret        = E_NOT_OK;
    uint8          phyCtrlIdx = ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).EthIfPhysControllerIdx;
    if (EthIf_PhyControllerInformation[phyCtrlIdx].CtrlMode == mode)
    {
        EthIf_ControllerInformation[ctrlIdx].CtrlMode   = mode;
        EthIf_ControllerInformation[ctrlIdx].HasModeReq = FALSE;
        (void)EthSM_CtrlModeIndication(ctrlIdx, mode);
        ret = E_OK;
    }
    else
    {
        uint8 driverId       = ETHIF_CONTROLLER_CFG_ETH_DRIVER_IDX(ctrlIdx);
        uint8 ethContrllerId = ETHIF_CONTROLLER_CFG_ETH_IDX(ctrlIdx);
        ret                  = ETHIF_ETH_DRIVER_API_CFG(driverId).Eth_SetControllerMode(ethContrllerId, mode);
    }

    return ret;
}

#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
ETHIF_LOCAL_INLINE uint8 EthIf_GetEthIfPartionByEcuCParttion(uint8 ecuCPartition)
{
    uint8 ethIfPartition = ETHIF_USED_PARTITION_NUM;
    if (ecuCPartition < ECUC_USED_PARTITION_NUM)
    {
        ethIfPartition = EthIf_EthIfPartitionIdMapOnEcucPartition[ecuCPartition];
    }
    return ethIfPartition;
}
#endif

#if ((ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431) || (ETHIF_SWITCHPORT_PORTS_NUM > 0))
ETHIF_LOCAL_INLINE void EthIf_InternalUpdateSignalQuality(EthIf_SignalQualityResultType* signalQualityPtr)
{
    /* Update lowest and hightest in a call if a clean up has been executed before */
    if (signalQualityPtr->ActualSignalQuality > signalQualityPtr->HighestSignalQuality)
    {
        signalQualityPtr->HighestSignalQuality = signalQualityPtr->ActualSignalQuality;
    }
    if (signalQualityPtr->ActualSignalQuality < signalQualityPtr->LowestSignalQuality)
    {
        signalQualityPtr->LowestSignalQuality = signalQualityPtr->ActualSignalQuality;
    }
}

#endif

#if (ETHIF_USED_PARTITION_NUM > 1)
ETHIF_LOCAL_INLINE void
    EthIf_InternalGetElementRange(const uint8* indexArray, uint8 rangeIndex, EthIf_ElementRangeType* range)
{
    if (0u == rangeIndex)
    {
        range->EqualStart = 0u;
    }
    else
    {
        range->EqualStart = indexArray[rangeIndex - 1u];
    }
    range->InequalEnd = indexArray[rangeIndex];
}
#endif

ETHIF_LOCAL void EthIf_InternalPollingTrcvLinkStateAndQuality(
    uint8   ethIfPartition,
    boolean reqGetLinkState
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
    ,
    boolean reqGetSignal
#endif
)
{
    Std_ReturnType        ret = E_NOT_OK;
    EthTrcv_LinkStateType linkState;
#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
    for (uint8 trcvIdx = 0u; trcvIdx < ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM; trcvIdx++)
    {
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
        if (ethIfPartition == ETHIF_TRANSCEIVER_CFG_TRANSCEIVER(trcvIdx).EthIfPartition)
#endif
        {
            /* @SWS_EthIf_00407: EthIf_MainFunctionState shall poll Ethernet communication hardware
                    related information with the period */
            if (reqGetLinkState)
            {
                ret = EthIf_InternalTransceiverGetLinkState(trcvIdx, &linkState);
                if ((E_OK == ret) && (EthIf_TransceiverInformation[trcvIdx].TrcvLinkState != linkState))
                {
                    EthIf_InternalTrcvLinkChgNotificationToCtrl(trcvIdx, linkState);
                    EthIf_TransceiverInformation[trcvIdx].TrcvLinkState = linkState;
                }
            }
            /* @SWS_EthIf_00409: For each Ethernet transceiver where a link state of ETHTRCV_
                -LINK_STATE_ACTIVE is yielded the function shall poll the signal quality  */
#if (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0)
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
            if (reqGetSignal)
            {
                if (ETHTRCV_LINK_STATE_ACTIVE == EthIf_TransceiverInformation[trcvIdx].TrcvLinkState)
                {
                    (void)EthIf_InternalGetTrcvSignalQuality(trcvIdx, NULL_PTR);
                }
            }
#endif
#endif
        }
    }
#endif

#if ((ETHIF_SWITCHPORT_PORTS_NUM > 0) && (ETHIF_HAST_SWITCH_PORT_REF_TRANSCEIVER == STD_ON))
    if (reqGetLinkState)
    {
        EthIf_InteralPollEthSwitchPortLinkState(ethIfPartition);
    }
#endif
#if (                                       \
    (ETHIF_SIGNAL_QUALITY_CHECK_PERIOD > 0) \
    && ((ETHIF_SWITCHPORT_PORTS_NUM > 0) && (ETHIF_HAST_SWITCH_PORT_REF_TRANSCEIVER == STD_ON)))
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
    if (reqGetSignal)
    {
        EthIf_InteralPollEthSwitchPortSignalQuality(ethIfPartition);
    }
#endif
#endif

#if (ETHIF_HAST_CONTROLLER_NO_REF_ETH_HARDWARE == STD_ON)
#if (ETHIF_USED_PARTITION_NUM > 1)
    EthIf_ElementRangeType ctrlIdxRange;
    EthIf_InternalGetElementRange(EthIf_ConfigPtr->CtrlIndexRangeMap, ethIfPartition, &ctrlIdxRange);
    for (uint8 ctrlIdx = ctrlIdxRange.EqualStart; ctrlIdx < ctrlIdxRange.InequalEnd; ctrlIdx++)
#else
    for (uint8 ctrlIdx = 0u; ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM; ctrlIdx++)
#endif
    {
#if (ETHIF_USED_PARTITION_NUM > 1)
        uint8 cfgCtrlIndex = EthIf_ConfigPtr->CtrlIndexMap[ctrlIdx];
#else
        uint8 cfgCtrlIndex = ctrlIdx;
#endif
        if (EthIf_ControllerInformation[cfgCtrlIndex].NoHardwareRefReportLinkChg)
        {
            EthIf_InternalTrcvLinkChgNotificationToUser(cfgCtrlIndex, ETHTRCV_LINK_STATE_ACTIVE);
            EthIf_ControllerInformation[cfgCtrlIndex].NoHardwareRefReportLinkChg = FALSE;
        }
    }
#endif
#if (STD_OFF == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    (void)ethIfPartition;
#endif
}

ETHIF_LOCAL void EthIf_InternalTrcvLinkChgNotificationToCtrl(uint8 trcvIdx, EthTrcv_LinkStateType newLinkState)
{
    for (uint8 ctrlIdx = 0u; ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM; ctrlIdx++)
    {
        /* @SWS_EthIf_00230: Upon change of link state <User>_TrcvLinkStateChg shallbe invoked for every affected
         * EthIfController  */
        if ((ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefIdx == trcvIdx)
            && (ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).TrcvRefType == CONTROLLER_REF_TRANSCEIVER))
        {
            EthIf_InternalTrcvLinkChgNotificationToUser(ctrlIdx, newLinkState);
        }
    }
}

ETHIF_LOCAL_INLINE void EthIf_InternalTrcvLinkChgNotificationToUser(uint8 ctrlIdx, EthTrcv_LinkStateType newLinkState)
{
    for (uint8 idx = 0u; idx < ETHIF_TRCV_LINK_CHG_TRCV_LINK_CHG_NUM; idx++)
    {
        if (NULL_PTR != EthIf_ConfigPtr->UserTrcvLinkStateChg[idx])
        {
            /* terate through all configured <User>_TrcvLinkStateChg and To notify the current trcv link state,
             * eg:EthSM*/
            EthIf_ConfigPtr->UserTrcvLinkStateChg[idx](ctrlIdx, newLinkState);
        }
    }
}

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortLinkState(uint8 ethIfPartition)
{
    EthTrcv_LinkStateType linkState;
#if (ETHIF_USED_PARTITION_NUM > 1)
    EthIf_ElementRangeType ethSwtPortIdxRange;
    EthIf_InternalGetElementRange(
        EthIf_ConfigPtr->EthIfSwitchPortRangOnEachPartiton,
        ethIfPartition,
        &ethSwtPortIdxRange);
    for (uint8 portIdx = ethSwtPortIdxRange.EqualStart; portIdx < ethSwtPortIdxRange.InequalEnd; portIdx++)
#else
    for (uint8 portIdx = 0u; portIdx < ETHIF_SWITCHPORT_PORTS_NUM; portIdx++)
#endif
    {
#if (ETHIF_USED_PARTITION_NUM > 1)
        uint8 cfgPortIndex = EthIf_ConfigPtr->SwitchPortIndexMap[portIdx];
#else
        uint8 cfgPortIndex = portIdx;
#endif
        uint8          ethIfSwt     = EthIf_ConfigPtr->EthIfEthSwtPortCfg[cfgPortIndex].EthIfEthIfSwtIdx;
        uint8          ethIfSwtPort = EthIf_ConfigPtr->EthIfEthSwtPortCfg[cfgPortIndex].EthPortIdx;
        Std_ReturnType ret          = EthIf_InternalSwitchPortGetLinkState(ethIfSwt, ethIfSwtPort, &linkState);
        if ((E_OK == ret) && (linkState != EthIf_SwitchPortRuntimeInformation[cfgPortIndex].PortLinkState))
        {
            EthIf_SwitchPortRuntimeInformation[cfgPortIndex].PortLinkState = linkState;
        }
    }
#if (ETHIF_SWITCH_SWITCHS_NUM > 0u)
    EthIf_InteralCheckChgOfSwitchLinkState(ethIfPartition);
#endif
#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0u)
    EthIf_InteralCheckChgOfSwitchPortGroupLinkState(ethIfPartition);
#endif
}

ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortTimer(uint8 ethIfPartition)
{
    EthTrcv_LinkStateType linkState;
#if (ETHIF_USED_PARTITION_NUM > 1)
    EthIf_ElementRangeType ethSwtPortIdxRange;
    EthIf_InternalGetElementRange(
        EthIf_ConfigPtr->EthIfSwitchPortRangOnEachPartiton,
        ethIfPartition,
        &ethSwtPortIdxRange);
    for (uint8 portIdx = ethSwtPortIdxRange.EqualStart; portIdx < ethSwtPortIdxRange.InequalEnd; portIdx++)
#else
    for (uint8 portIdx = 0u; portIdx < ETHIF_SWITCHPORT_PORTS_NUM; portIdx++)
#endif
    {
#if (ETHIF_USED_PARTITION_NUM > 1)
        uint8 cfgPortIndex = EthIf_ConfigPtr->SwitchPortIndexMap[portIdx];
#else
        uint8 cfgPortIndex = portIdx;
#endif
        EthIf_InteralCheckSwitchPortTimer(cfgPortIndex);
    }
}

#if (ETHIF_HAST_SWITCH_PORT_REF_TRANSCEIVER == STD_ON)
ETHIF_LOCAL void EthIf_InteralPollEthSwitchPortSignalQuality(uint8 ethIfPartition)
{
#if (ETHIF_USED_PARTITION_NUM > 1)
    EthIf_ElementRangeType ethSwtPortIdxRange;
    EthIf_InternalGetElementRange(
        EthIf_ConfigPtr->EthIfSwitchPortRangOnEachPartiton,
        ethIfPartition,
        &ethSwtPortIdxRange);
    for (uint8 portIdx = ethSwtPortIdxRange.EqualStart; portIdx < ethSwtPortIdxRange.InequalEnd; portIdx++)
#else
    for (uint8 portIdx = 0u; portIdx < ETHIF_SWITCHPORT_PORTS_NUM; portIdx++)
#endif
    {
#if (ETHIF_USED_PARTITION_NUM > 1)
        uint8 cfgPortIndex = EthIf_ConfigPtr->SwitchPortIndexMap[portIdx];
#else
        uint8 cfgPortIndex = portIdx;
#endif
        uint8 ethIfSwt     = EthIf_ConfigPtr->EthIfEthSwtPortCfg[cfgPortIndex].EthIfEthIfSwtIdx;
        uint8 ethIfSwtPort = EthIf_ConfigPtr->EthIfEthSwtPortCfg[cfgPortIndex].EthPortIdx;
        /* @SWS_EthIf_00408: For each Ethernet switch port where a link state ETHTRCV_-
            LINK_STATE_ACTIVE is yielded and references an Ethernet Transceiver  */
        if ((ETHTRCV_LINK_STATE_ACTIVE == EthIf_SwitchPortRuntimeInformation[cfgPortIndex].PortLinkState)
            && (EthIf_ConfigPtr->EthIfEthSwtPortCfg[cfgPortIndex].ReferredEthTrcv != ETHIF_INVALID_TRCV_ID))
        {
            (void)EthIf_InternalGetSwitchPortSignalQuality(ethIfSwt, ethIfSwtPort, NULL_PTR);
        }
    }
}
#endif
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
/* @brief check StartActiveTimer and SwitchOffPortTimer. */
ETHIF_LOCAL void EthIf_InteralCheckSwitchPortTimer(uint8 ethIfSwitchPortIdx)
{
    boolean reqSetDown = FALSE;
    SchM_Enter_EthIf_ExclusiveArea();
    if (0u < EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].StartActiveTimer)
    {
        EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].StartActiveTimer--;
        if (0u == EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].StartActiveTimer)
        {
            /* @SWS_EthIf_00276: EthIf shall deactivate all those ports activated due to EthIf_StartAllPorts. */
            reqSetDown = TRUE;
        }
    }
    if (0u < EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].SwitchOffPortTimer)
    {
        EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].SwitchOffPortTimer--;
        if (0u == EthIf_SwitchPortRuntimeInformation[ethIfSwitchPortIdx].SwitchOffPortTimer)
        {
            /* @SWS_EthIf_00271: timer elapses for a port, EthIf shall call the following functions in the given order
             */
            reqSetDown = TRUE;
        }
    }
    SchM_Exit_EthIf_ExclusiveArea();
    if (reqSetDown)
    {
        uint8 ethIfSwtIdx = ETHIF_SWITCHPORT_CFG_SWITCHPORT(ethIfSwitchPortIdx).EthIfEthIfSwtIdx;
        uint8 port        = ETHIF_SWITCHPORT_CFG_SWITCHPORT(ethIfSwitchPortIdx).EthPortIdx;
        uint8 driverIdx   = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(ethIfSwtIdx);
        uint8 ethSwtIdx   = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(ethIfSwtIdx);
        if ((NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest))
        {
            (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_PortLinkStateRequest(
                ethSwtIdx,
                port,
                ETHTRCV_LINK_STATE_DOWN);
        }
        if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode)
        {
            (void)ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_SetSwitchPortMode(ethSwtIdx, port, ETH_MODE_DOWN);
        }
    }
}

ETHIF_LOCAL void EthIf_InteralCheckChgOfSwitchLinkState(uint8 ethIfPartition)
{
    EthTrcv_LinkStateType linkState;
    for (uint8 swtIdx = 0u; swtIdx < ETHIF_SWITCH_SWITCHS_NUM; swtIdx++)
    {
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
        if (ethIfPartition == ETHIF_SWITCH_CFG_SWITCH(swtIdx).EthIfPartition)
#endif
        {
            linkState =
                EthIf_SwitchPortRuntimeInformation[ETHIF_SWITCH_CFG_SWITCH(swtIdx).HostPortIdxInEthIf].PortLinkState;
            if (EthIf_SwitchLinkState[swtIdx] != linkState)
            {
                /* @SWS_EthIf_00260: If the EthIfCtrl references a EthIfSwitch but no port group is configured
                    the EthIf shall indicate the link state of the host port to the EthSM  */
#if (0u == ETHIF_SWITCH_PORT_GROUP_GROUP_NUM)
                uint8 ctrlIdx = ETHIF_SWITCH_CFG_SWITCH(swtIdx).ReferencedByCtrl;
                if (ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM)
                {
                    EthSM_TrcvLinkStateChg(ctrlIdx, linkState);
                }
#endif
                EthIf_SwitchLinkState[swtIdx] = linkState;
            }
        }
    }
}
#endif

#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
ETHIF_LOCAL void EthIf_InteralCheckChgOfSwitchPortGroupLinkState(uint8 ethIfPartition)
{
    EthTrcv_LinkStateType linkState;
    for (uint8 groupIdx = 0u; groupIdx < ETHIF_SWITCH_PORT_GROUP_GROUP_NUM; groupIdx++)
    {
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
        if (ethIfPartition == ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(groupIdx).EthIfPartition)
#endif
        {
            EthIf_InteralGetSwitchPortGroupLinkState(groupIdx, &linkState);
            if (linkState != EthIf_SwitchPortGroupRuntimeInformation[groupIdx].GroupLinkState)
            {
                uint8 ctrlIdx = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(groupIdx).ReferencedByCtrl;
                if (ctrlIdx < ETHIF_CONTROLLER_CONTROLLER_NUM)
                {
                    /* @SWS_EthIf_00261: switchPortGroup is connected to a controller indicate to EthSM */
                    EthSM_TrcvLinkStateChg(ctrlIdx, linkState);
                }
#if (STD_ON == ETHIF_HAST_SWITCH_PORT_GROUP_NO_CONNECT_CONTROLLER)
                else
                {
                    /* @SWS_EthIf_00261: switchPortGroup is not connected to a controller indicate to BswM */
                    BswM_EthIf_PortGroupLinkStateChg(groupIdx, linkState);
                }
#endif
                EthIf_SwitchPortGroupRuntimeInformation[groupIdx].GroupLinkState = linkState;
            }
        }
    }
}

ETHIF_LOCAL void
    EthIf_InteralGetSwitchPortGroupLinkState(uint8 switchPortGroupIdx, EthTrcv_LinkStateType* groupLinkStatePtr)
{
    EthTrcv_LinkStateType linkState     = ETHTRCV_LINK_STATE_ACTIVE;
    uint8                 portCount     = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(switchPortGroupIdx).EthSwtPortNum;
    uint8                 downPortCount = 0u;
    /* @SWS_EthIf_00259: host port or up link port linkdown or all port linkdown  */
    for (uint8 iterateIdx = 0u; iterateIdx < portCount; iterateIdx++)
    {
        uint8 portIdx = ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(switchPortGroupIdx).RefEthSwtPortList[iterateIdx];
        if (EthIf_SwitchPortRuntimeInformation[portIdx].PortLinkState == ETHTRCV_LINK_STATE_DOWN)
        {
            if ((portIdx == ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(switchPortGroupIdx).HostPort)
                || (portIdx == ETHIF_SWITCHPORTGROUP_CFG_SWITCHPORTGROUP(switchPortGroupIdx).UpLinkPort))
            {
                linkState = ETHTRCV_LINK_STATE_DOWN;
                break;
            }
            downPortCount++;
        }
    }
    if (downPortCount == portCount)
    {
        linkState = ETHTRCV_LINK_STATE_DOWN;
    }
    *groupLinkStatePtr = linkState;
}
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
ETHIF_LOCAL_INLINE Std_ReturnType
    EthIf_InternalSwitchPortGetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr)
{
    Std_ReturnType ret       = E_NOT_OK;
    uint8          ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
    uint8          driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
    if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetLinkState)
    {
        ret = ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetLinkState(SwitchIdx, SwitchPortIdx, LinkStatePtr);
    }
    return ret;
}
#endif

ETHIF_LOCAL_INLINE
Std_ReturnType EthIf_InternalTransceiverGetLinkState(uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr)
{
    Std_ReturnType ret        = E_NOT_OK;
    uint8          ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
    uint8          driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
    if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetLinkState)
    {
        ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetLinkState(ethTrcvIdx, LinkStatePtr);
    }
    return ret;
}

#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431) && (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
ETHIF_LOCAL Std_ReturnType EthIf_InternalGetTrcvSignalQuality(uint8 TrcvIdx, EthIf_SignalQualityResultType* ResultPtr)
{
    Std_ReturnType ret        = E_NOT_OK;
    uint8          ethTrcvIdx = ETHIF_TRANSCEIVER_CFG_ETHTRCV_IDX(TrcvIdx);
    uint8          driverIdx  = ETHIF_TRANSCEIVER_CFG_TRCV_DRIVER_IDX(TrcvIdx);
    if (NULL_PTR != ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetPhySignalQuality)
    {
        uint32 signalQuality;
        ret = ETHIF_ETHTRCV_DRIVER_API_CFG(driverIdx).EthTrcv_GetPhySignalQuality(ethTrcvIdx, &signalQuality);
        if (E_OK == ret)
        {
            EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality.ActualSignalQuality = signalQuality;
            EthIf_InternalUpdateSignalQuality(&EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality);
            /* enusre polling function could use this function */
            if (NULL_PTR != ResultPtr)
            {
                *ResultPtr = EthIf_TransceiverInformation[TrcvIdx].TrcvSignalQuality;
            }
        }
    }
    return ret;
}
#endif

#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
ETHIF_LOCAL Std_ReturnType EthIf_InternalGetSwitchPortSignalQuality(
    uint8                          SwitchIdx,
    uint8                          SwitchPortIdx,
    EthIf_SignalQualityResultType* ResultPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    uint8 ethSwtIdx = ETHIF_SWITCH_CFG_ETHSWITCH_IDX(SwitchIdx);
    uint8 driverIdx = ETHIF_SWITCH_CFG_SWITCH_DRIVER_IDX(SwitchIdx);
    if (NULL_PTR != ETHIF_SWITCH_DRIVER_API_CFG(driverIdx).EthSwt_GetPortSignalQuality)
    {
        uint32 signalQuality;
        ret = ETHIF_SWITCH_DRIVER_API_CFG(SwitchIdx).EthSwt_GetPortSignalQuality(
            ethSwtIdx,
            SwitchPortIdx,
            &signalQuality);
        if (E_OK == ret)
        {
            uint8 portIdx = ETHIF_SWITCH_CFG_SWITCH(SwitchIdx).EthIfSwtPortOnPartitonStarIdx + SwitchPortIdx;
            EthIf_SwitchPortRuntimeInformation[portIdx].PortSignalQuality.ActualSignalQuality = signalQuality;
            EthIf_InternalUpdateSignalQuality(&(EthIf_SwitchPortRuntimeInformation[portIdx].PortSignalQuality));
            /* enusre polling function could use this function */
            if (NULL_PTR != ResultPtr)
            {
                *ResultPtr = EthIf_SwitchPortRuntimeInformation[portIdx].PortSignalQuality;
            }
        }
    }
    return ret;
}
#endif

ETHIF_LOCAL_INLINE void EthIf_InternalReportTxConfirmationToUpper(
    uint8          owerIdx,
    uint8          ethifCrlId,
    Eth_BufIdxType bufIdx,
    Std_ReturnType result)
{
    uint8 funcIdx = EthIf_ConfigPtr->EthIfOwnerCfg[owerIdx].EthIfOwner;
    if (NULL_PTR != EthIf_ConfigPtr->UserTxConfirmation[funcIdx])
    {
        EthIf_ConfigPtr->UserTxConfirmation[funcIdx](ethifCrlId, bufIdx, result);
    }
}

#if (ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM > 0u)
ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidTransceiver(uint8 trcvIdx, uint8 ethIfPartition, uint8* errorCode)
{
    boolean ret = TRUE;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    if ((ethIfPartition >= ETHIF_USED_PARTITION_NUM))
    {
        *errorCode = ETHIF_E_INVALID_PARTITION;
    }
    else if (!EthIf_InitStatus[ethIfPartition])
#else
    if (!EthIf_InitStatus[ethIfPartition])
#endif
    {
        *errorCode = ETHIF_E_UNINIT;
    }
    else if (((ETHIF_TRANSCEIVER_TRANSCEIVERS_NUM) <= trcvIdx))
    {
        *errorCode = ETHIF_E_INV_TRCV_IDX;
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (ethIfPartition != ETHIF_TRANSCEIVER_CFG_TRANSCEIVER(trcvIdx).EthIfPartition)
    {
        *errorCode = ETHIF_E_PARTITION_MISMATCH;
    }
#endif
    else
    {
        *errorCode = 0u;
        ret        = FALSE;
    }
    return ret;
}
#endif

ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidController(uint8 ctrlIdx, uint8 ethIfPartition, uint8* errorCode)
{
    boolean ret = TRUE;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    if ((ethIfPartition >= ETHIF_USED_PARTITION_NUM))
    {
        *errorCode = ETHIF_E_INVALID_PARTITION;
    }
    else if (!EthIf_InitStatus[ethIfPartition])
#else
    if (!EthIf_InitStatus[ethIfPartition])
#endif
    {
        *errorCode = ETHIF_E_UNINIT;
    }
    else if ((ETHIF_CONTROLLER_CONTROLLER_NUM) <= ctrlIdx)
    {
        *errorCode = ETHIF_E_INV_CTRL_IDX;
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (ethIfPartition != ETHIF_CONTROLLER_CFG_CONTROLLER(ctrlIdx).EthIfPartition)
    {
        *errorCode = ETHIF_E_PARTITION_MISMATCH;
    }
#endif
    else
    {
        *errorCode = 0u;
        ret        = FALSE;
    }
    return ret;
}
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
ETHIF_LOCAL boolean EthIf_DetCheckIsInvalidSwitch(uint8 swtIdx, uint8 ethIfPartition, uint8* errorCode)
{
    boolean ret = TRUE;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    if ((ethIfPartition >= ETHIF_USED_PARTITION_NUM))
    {
        *errorCode = ETHIF_E_INVALID_PARTITION;
    }
    else if (!EthIf_InitStatus[ethIfPartition])
#else
    if (!EthIf_InitStatus[ethIfPartition])
#endif
    {
        *errorCode = ETHIF_E_UNINIT;
    }
    else if (((ETHIF_SWITCH_SWITCHS_NUM) <= swtIdx))
    {
        *errorCode = ETHIF_E_INV_SWT_IDX;
    }
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    else if (ethIfPartition != ETHIF_SWITCH_CFG_SWITCH(swtIdx).EthIfPartition)
    {
        *errorCode = ETHIF_E_PARTITION_MISMATCH;
    }
#endif
    else
    {
        *errorCode = 0u;
        ret        = FALSE;
    }
    return ret;
}
#endif
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE_FAST
#include "EthIf_MemMap.h"
/*  @brief           Receive indication of an Ethernet frame which was received by the indexed controller. */
void EthIf_RxIndication(
    uint8               CtrlIdx,
    Eth_FrameType       FrameType,
    boolean             IsBroadcast,
    const uint8*        PhysAddrPtr,
    const Eth_DataType* DataPtr,
    uint16              DataLen
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
    ,
    TimeTupleType* IngressTimeTuplePtr,
    /* PRQA S 3673 */ /* VL_EthIf_TypeOfPointerCouldbePointToConst */
    Eth_BufIdxType RxHandleId
#endif
)
{
    const uint8* u8DataPtr      = (const uint8*)DataPtr;
    uint8        ethIfPartition = 0u;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition = (uint8)GetApplicationID();
    ethIfPartition      = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    ethIfPartition = 0u;
#endif
    if ((ETHIF_PHYCONTROLLER_PHYCONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, ETHIF_E_INV_CTRL_IDX);
    }
    else if (EthIf_DetCheckIsInvalidController(
                 ETHIF_PHYCONTROLLER_CFG_PHYCONTROLLER(CtrlIdx).EthIfEthIfCtrlId,
                 ethIfPartition,
                 &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, errorCode);
    }
    else if (
        (NULL_PTR == PhysAddrPtr) || (NULL_PTR == DataPtr)
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
        || (NULL_PTR == IngressTimeTuplePtr)
#endif
    )
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION, ETHIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint16 frameVlanId = ETHIF_VID_UNUSED;
#if (ETHIF_CTRL_HAS_CONFIGED_VLAN == STD_ON)
        if (ETH_FRAME_TYPE_VLAN == FrameType)
        {
            uint16 tci = 0u;
            tci |= (uint16)((uint16)u8DataPtr[0] << ETHIF_SHIFT_USE_8_BITS);
            tci |= (uint16)u8DataPtr[1];
            frameVlanId = tci & ETHIF_VID_MASK;
        }
#endif
        uint8          ethifCtrlId = 0u;
        Std_ReturnType ret         = EthIf_GetMatchedEthIfCtrl(&ethifCtrlId, CtrlIdx, frameVlanId);
        if (E_OK == ret)
        {
            boolean       upIndication = FALSE;
            Eth_FrameType upperLayerFrameType;
            uint16        upperLayerLenByte = 0u;
            const uint8*  upperLayerDataPtr = NULL_PTR;

#if (ETHIF_CTRL_HAS_CONFIGED_VLAN == STD_ON)
            uint16 ctrlVlanId = EthIf_ConfigPtr->EthIfCtrlCfg[ethifCtrlId].EthIfVlanId;
            if (ctrlVlanId < ETHIF_VID_UNUSED)
            {
                if (frameVlanId == ctrlVlanId)
                {
                    upIndication        = TRUE;
                    upperLayerFrameType = 0u;
                    upperLayerDataPtr   = &u8DataPtr[VLAN_TAG_SIZE];
                    upperLayerLenByte   = DataLen - VLAN_TAG_SIZE;
                    upperLayerFrameType |= (Eth_FrameType)((Eth_FrameType)u8DataPtr[ETHIF_FRAME_TYPE_POS_2_OFFSET]
                                                           << ETHIF_SHIFT_USE_8_BITS);
                    upperLayerFrameType |= (Eth_FrameType)u8DataPtr[ETHIF_FRAME_TYPE_POS_3_OFFSET];
                }
#if (ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API == STD_ON)
                else
                {
                    /* @SWS_EthIf_00309: shall return the number of all dropped datagrams, caused by invalid
                     * CrtlIdx/VLAN. */
                    EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN++;
                }
#endif
            }
            else
#endif
            {
                /* controller doesn't configure a VLAN Id */
                if (ETH_FRAME_TYPE_VLAN != FrameType)
                {
                    upIndication        = TRUE;
                    upperLayerFrameType = FrameType;
                    upperLayerDataPtr   = u8DataPtr;
                    upperLayerLenByte   = DataLen;
                }
#if (ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API == STD_ON)
                else
                {
                    /* @SWS_EthIf_00317: If the VLAN is not active the Ethernet Interface shall increment
                        the corresponding measuremen. */
                    EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN++;
                }
#endif
            }

            if (upIndication)
            {
                EthIf_InternalUpperLayerRxIndication(
                    ethifCtrlId,
                    upperLayerFrameType,
                    IsBroadcast,
                    PhysAddrPtr,
                    upperLayerDataPtr,
                    upperLayerLenByte);
            }
        }
#if ((ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API == STD_ON) || (STD_ON == ETHIF_ENABLE_SECRITY_EVENT_REPORTING))
        else
        {
            /* @SWS_EthIf_00309: shall return the number of all dropped datagrams, caused by invalid CrtlIdx/VLAN. */
#if (ETHIF_GET_AND_RESET_MEASUREMENT_DATA_API == STD_ON)
            EthIf_MeasurementData[ethIfPartition].MeasureDropInvControllerVLAN++;
#endif
#if (ETHIF_ENABLE_SECRITY_EVENT_REPORTING == STD_ON)
            IdsM_SetSecurityEvent(SEV_ETH_DROP_MAC_COLLISION);
#endif
        }
#endif
    }
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
    (void)RxHandleId;
    (void)IngressTimeTuplePtr;
#endif
}

/* @brief get EthIfCtrl Id by PhyCtrl and VlanId */
ETHIF_LOCAL Std_ReturnType EthIf_GetMatchedEthIfCtrl(uint8* EthifCtrlId, uint8 EthCtrlId, uint16 frameVlanId)
{
    Std_ReturnType ret = E_NOT_OK;
    for (uint8 index = 0u; index < ETHIF_CONTROLLER_CONTROLLER_NUM; index++)
    {
        if (ETHIF_CONTROLLER_CFG_CONTROLLER(index).EthIfPhysControllerIdx == EthCtrlId)
        {
#if (ETHIF_CTRL_HAS_CONFIGED_VLAN == STD_ON)
            uint16 ctrlVlanId = ETHIF_CONTROLLER_CFG_CONTROLLER(index).EthIfVlanId;
            if (frameVlanId == ctrlVlanId)
            {
                *EthifCtrlId = index;
                ret          = E_OK;
                break;
            }
#else
            (void)frameVlanId;
            *EthifCtrlId = index;
            ret          = E_OK;
            break;
#endif
        }
    }
    return ret;
}

ETHIF_LOCAL void EthIf_InternalUpperLayerRxIndication(
    uint8               ethifCtrlId,
    Eth_FrameType       upperLayerFrameType,
    boolean             IsBroadcast,
    const uint8*        PhysAddrPtr,
    const Eth_DataType* upperLayerDataPtr,
    uint16              upperLayerLenByte)
{
#if (ETHIF_ENABLE_SECRITY_EVENT_REPORTING == STD_ON)
    boolean UplayerNotNotified = TRUE;
#endif
    for (uint8 i = 0u; i < ETHIF_FRAME_OWNER_FRAME_OWNER_NUM; i++)
    {
        uint8 ownerId = EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfOwner;
        if ((upperLayerFrameType == EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfFrameType)
            && (NULL_PTR != EthIf_ConfigPtr->UserRxIndication[ownerId]))
        {
            const EthIfULRxIndicationFuncType upperLayerFun = EthIf_ConfigPtr->UserRxIndication[ownerId];
            upperLayerFun(
                ethifCtrlId,
                upperLayerFrameType,
                IsBroadcast,
                PhysAddrPtr,
                (const uint8*)upperLayerDataPtr,
                upperLayerLenByte);
#if (ETHIF_ENABLE_SECRITY_EVENT_REPORTING == STD_ON)
            UplayerNotNotified = FALSE;
#endif
        }
    }
#if (ETHIF_ENABLE_SECRITY_EVENT_REPORTING == STD_ON)
    /* @SWS_EthIf_00502: respective security events shall be reported to the IdsM  */
    if (UplayerNotNotified)
    {
        IdsM_SetSecurityEvent(SEV_ETH_DROP_UNKNOWN_ETHERTYPE);
    }
#endif
}

/*  @brief           Confirms frame transmission by the indexed controller. */
void EthIf_TxConfirmation(
    uint8          CtrlIdx,
    Eth_BufIdxType BufIdx
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    ,
    Std_ReturnType Result
#endif
)
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    uint8 errorCode;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition  = (uint8)GetApplicationID();
    uint8 ethIfPartition = EthIf_GetEthIfPartionByEcuCParttion(ecuCPartition);
#else
    uint8 ethIfPartition = 0u;
#endif
    if ((ETHIF_PHYCONTROLLER_PHYCONTROLLER_NUM) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, ETHIF_E_INV_CTRL_IDX);
    }
    else if (EthIf_DetCheckIsInvalidController(
                 ETHIF_PHYCONTROLLER_CFG_PHYCONTROLLER(CtrlIdx).EthIfEthIfCtrlId,
                 ethIfPartition,
                 &errorCode))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, errorCode);
    }
    else if (BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION, ETHIF_E_INV_PARAM);
    }
    else
#endif
    {
        if (NULL_PTR != EthIf_ConfigPtr->UserTxConfirmation)
        {
            uint8   ethifCtrlId  = 0u;
            boolean txConfReport = FALSE;
            for (uint8 owerIdx = 0u; owerIdx < ETHIF_FRAME_OWNER_FRAME_OWNER_NUM; owerIdx++)
            {
                if (BufIdx == EthIf_FrameOwnerRuntimeInformation[owerIdx].BufferIdx)
                {
                    for (uint8 index = 0u; index < ETHIF_CONTROLLER_CONTROLLER_NUM; index++)
                    {
                        if ((CtrlIdx == ETHIF_CONTROLLER_CFG_CONTROLLER(index).EthIfPhysControllerIdx)
                            && (index == EthIf_FrameOwnerRuntimeInformation[owerIdx].EthIfCrlIdx))
                        {
                            ethifCtrlId                                             = index;
                            txConfReport                                            = TRUE;
                            EthIf_FrameOwnerRuntimeInformation[owerIdx].EthIfCrlIdx = ETHIF_CONTROLLER_CONTROLLER_NUM;
                            break;
                        }
                    }
                    if (txConfReport)
                    {
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
                        EthIf_InternalReportTxConfirmationToUpper(owerIdx, ethifCtrlId, BufIdx, Result);
#else
                        EthIf_InternalReportTxConfirmationToUpper(owerIdx, ethifCtrlId, BufIdx, E_OK);
#endif
                    }
                    break;
                }
            }
        }
    }
}

#define ETHIF_STOP_SEC_CODE_FAST
#include "EthIf_MemMap.h"
/* PRQA S 1503,1532 -- */
/* PRQA S 6040,6030 -- */
