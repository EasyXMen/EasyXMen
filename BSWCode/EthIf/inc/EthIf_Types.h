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
/* PRQA S 1536 EOF */ /* VL_EthIf_1536 */

#ifndef ETHIF_TYPES_H
#define ETHIF_TYPES_H

/* PRQA S 1756,1758 EOF */ /* VL_EthIf_1756,VL_EthIf_1758 */

/* =================================================== inclusions =================================================== */
#include "EthIf_Cfg.h"
#include "Eth_GeneralTypes.h"
#include "EcuM.h"
#include "Platform_Types.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* messurement index for EthIf_MeasurementIdxType */
#define ETHIF_MEAS_DROP_CRTLIDX 0x01u
#define ETHIF_MEAS_ALL          0xFFu

/* represent no Vlan ID is set for ethernet interface controller */
#define ETHIF_VID_UNUSED 0x1000u

/* ================================================ type definitions ================================================ */

/**
 * @brief  define referenced ethernet hardware for ethernet interface controller.
 */
typedef enum EthIf_CtrlRefTrcvTag
{
    CONTROLLER_REF_TRANSCEIVER,
    CONTROLLER_REF_SWITCH,
    CONTROLLER_REF_SWITCH_PORT_GROUP,
    CONTROLLER_REF_NO_HARDWARE,
} EthIf_CtrlRefTrcvType;

/* Definition of EthIf_MeasurementIdxType */
typedef uint8 EthIf_MeasurementIdxType;

typedef void (*EthIfULTxConfirmationType)(uint8 ctrlIdx, Eth_BufIdxType bufIdx, Std_ReturnType result);

/* Function pointer for User defined TrcvLinkState callback */
typedef void (*EthIfTrcvLinkStateChgType)(uint8 CtrlIdx, EthTrcv_LinkStateType TransceiverLinkState);

typedef uint8 EthIf_SwitchPortGroupIdxType;

#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
typedef void (*EthIfULRxIndicationFuncType)(
    uint8               CtrlIdx,
    Eth_FrameType       FrameType,
    boolean             IsBroadcast,
    const uint8*        PhysAddrPtr,
    const Eth_DataType* DataPtr,
    uint16              DataLen);
#else
typedef void (*EthIfULRxIndicationFuncType)(
    uint8         CtrlIdx,
    Eth_FrameType FrameType,
    boolean       IsBroadcast,
    const uint8*  PhysAddrPtr,
    const uint8*  DataPtr,
    uint16        LenByte);
#endif

typedef void (*EthIfULTxConfirmationFuncType)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Std_ReturnType Result);

/* Function pointer for User defined TrcvLinkState callback */
typedef void (*EthIfTrcvLinkStateChgFuncType)(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState);

/**
 * @brief configuration of Eth Driver Apis type.
 */
typedef struct EthIf_EthDriverApiTag
{
    Std_ReturnType (*Eth_SetControllerMode)(uint8 CtrlIdx, Eth_ModeType CtrlMode);
    Std_ReturnType (*Eth_GetControllerMode)(uint8 CtrlIdx, Eth_ModeType* CtrlModePtr);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    BufReq_ReturnType (*Eth_ProvideTxBuffer)(
        uint8           CtrlIdx,
        uint8           Priority,
        Eth_BufIdxType* BufIdxPtr,
        uint8**         BufPtr,
        uint16*         LenBytePtr);
#else
    BufReq_ReturnType (
        *Eth_ProvideTxBuffer)(uint8 CtrlIdx, Eth_BufIdxType* BufIdxPtr, uint8** BufPtr, uint16* LenBytePtr);
#endif
    Std_ReturnType (*Eth_Transmit)(
        uint8          CtrlIdx,
        Eth_BufIdxType BufIdx,
        Eth_FrameType  FrameType,
        boolean        TxConfirmation,
        uint16         LenByte,
        const uint8*   PhysAddrPtr);
    void (*Eth_SetPhysAddr)(uint8 CtrlIdx, const uint8* PhysAddrPtr);
    void (*Eth_GetPhysAddr)(uint8 CtrlIdx, uint8* PhysAddrPtr);
    Std_ReturnType (*Eth_UpdatePhysAddrFilter)(uint8 CtrlIdx, const uint8* PhysAddrPtr, Eth_FilterActionType Action);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_R23_11)
    Std_ReturnType (*Eth_GetCurrentTimeTuple)(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeTupleType* currentTimeTuplePtr);
    Std_ReturnType (*Eth_SetPhcTime)(uint8 CtrlIdx, uint8 ClkUnitIdx, const TimeStampType* timeStampPtr);
    Std_ReturnType (*Eth_SetPhcCorrection)(uint8 CtrlIdx, uint8 ClkUnitIdx, sint32 rateDeviation, sint32 offset);
    Std_ReturnType (
        *Eth_GetPhcTime)(uint8 CtrlIdx, uint8 ClkUnitIdx, TimeStampQualType* timeQualPtr, TimeStampType* timeStampPtr);
    Std_ReturnType (*Eth_SetPpsSignalMode)(uint8 CtrlIdx, uint8 ClkUnitIdx, boolean signalMode);
    Std_ReturnType (*Eth_GetSpiStatus)(uint8 CtrlIdx, Eth_SpiStatusType* SpiStatusType);
#else
    Std_ReturnType (
        *Eth_GetCurrentTime)(uint8 CtrlIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#endif
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    void (*Eth_EnableEgressTimeStamp)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);
#else
    void (*Eth_EnableEgressTimeStamp)(uint8 CtrlIdx, uint8 BufIdx);
#endif
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
    Std_ReturnType (*Eth_GetEgressTimeStamp)(
        uint8                  CtrlIdx,
        Eth_BufIdxType         BufIdx,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
    void (*Eth_GetEgressTimeStamp)(
        uint8                  CtrlIdx,
        Eth_BufIdxType         BufIdx,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#else
    void (*Eth_GetEgressTimeStamp)(
        uint8                  CtrlIdx,
        uint8                  BufIdx,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#endif
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
    Std_ReturnType (*Eth_GetIngressTimeStamp)(
        uint8                  CtrlIdx,
        const Eth_DataType*    DataPtr,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
    void (*Eth_GetIngressTimeStamp)(
        uint8                  CtrlIdx,
        const Eth_DataType*    DataPtr,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#else
    void (*Eth_GetIngressTimeStamp)(
        uint8                  CtrlIdx,
        Eth_DataType*          DataPtr,
        Eth_TimeStampQualType* timeQualPtr,
        Eth_TimeStampType*     timeStampPtr);
#endif
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    void (*Eth_Receive)(uint8 CtrlIdx, uint8 FifoIdx, Eth_RxStatusType* RxStatusPtr);
#else
    void (*Eth_Receive)(uint8 CtrlIdx, Eth_RxStatusType* RxStatusPtr);
#endif
    void (*Eth_TxConfirmation)(uint8 CtrlIdx);

} EthIf_EthDriverApiType;

/**
 * @brief configuration of EthTransceiver Driver Apis type.
 */
typedef struct EthIf_EthTrcvDriverApiTag
{
    Std_ReturnType (*EthTrcv_TransceiverLinkStateRequest)(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R19_11)
    Std_ReturnType (*EthTrcv_GetTransceiverMode)(uint8 TrcvIdx, Eth_ModeType* TrcvModePtr);
#else
    Std_ReturnType (*EthTrcv_GetTransceiverMode)(uint8 TrcvIdx, EthTrcv_ModeType* TrcvModePtr);
#endif
    Std_ReturnType (*EthTrcv_GetLinkState)(uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R19_11)
    Std_ReturnType (*EthTrcv_SetTransceiverMode)(uint8 TrcvIdx, Eth_ModeType TrcvMode);
#else
    Std_ReturnType (*EthTrcv_SetTransceiverMode)(uint8 TrcvIdx, EthTrcv_ModeType TrcvMode);
#endif
    Std_ReturnType (*EthTrcv_CheckWakeup)(uint8 TrcvIdx);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R22_11)
    Std_ReturnType (*EthTrcv_GetBusWuReason)(uint8 TrcvIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr);
#endif
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_431)
    Std_ReturnType (*EthTrcv_GetPhySignalQuality)(uint8 TrcvIdx, uint32* SignalQualityPtr);
    Std_ReturnType (*EthTrcv_SetPhyTestMode)(uint8 TrcvIdx, EthTrcv_PhyTestModeType Mode);
    Std_ReturnType (*EthTrcv_SetPhyLoopbackMode)(uint8 TrcvIdx, EthTrcv_PhyLoopbackModeType Mode);
    Std_ReturnType (*EthTrcv_SetPhyTxMode)(uint8 TrcvIdx, EthTrcv_PhyTxModeType Mode);
    Std_ReturnType (*EthTrcv_GetCableDiagnosticsResult)(uint8 TrcvIdx, EthTrcv_CableDiagResultType* ResultPtr);
    Std_ReturnType (
        *EthTrcv_GetPhyIdentifier)(uint8 TrcvIdx, uint32* OrgUniqueIdPtr, uint8* ModelNrPtr, uint8* RevisionNrPtr);
#endif
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R20_11)
    Std_ReturnType (*EthTrcv_GetMacMethod)(uint8 TrcvIdx, EthTrcv_MacMethodType* MacModePtr);
#endif
    Std_ReturnType (*EthTrcv_GetBaudRate)(uint8 TrcvIdx, EthTrcv_BaudRateType* BaudRatePtr);
    Std_ReturnType (*EthTrcv_GetDuplexMode)(uint8 TrcvIdx, EthTrcv_DuplexModeType* DuplexModePtr);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_R19_11)
    Std_ReturnType (*EthTrcv_RunCableDiagnostic)(uint8 TrcvIdx);
#endif
} EthIf_EthTrcvDriverApiType;

/**
 * @brief configuration of EthSwitch Driver Apis type.
 */
typedef struct EthIf_EthSwtDriverApiTag
{
    Std_ReturnType (*EthSwt_SetSwitchPortMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode);
    Std_ReturnType (*EthSwt_GetSwitchPortMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* SwitchPortModePrt);
    Std_ReturnType (*EthSwt_GetLinkState)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr);
    Std_ReturnType (*EthSwt_GetBaudRate)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_BaudRateType* BaudRatePtr);
    Std_ReturnType (*EthSwt_GetDuplexMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_DuplexModeType* DuplexModePtr);
    Std_ReturnType (*EthSwt_GetPortMacAddr)(uint8 SwitchIdx, const uint8* MacAddrPtr, uint8* PortIdxPtr);
    Std_ReturnType (
        *EthSwt_GetArlTable)(uint8 SwitchIdx, uint16* numberOfElements, Eth_MacVlanType* arlTableListPointer);
    Std_ReturnType (*EthSwt_GetCounterValues)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_CounterType* CounterPtr);
    Std_ReturnType (*EthSwt_GetRxStats)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_RxStatsType* RxStats);
    Std_ReturnType (*EthSwt_GetTxStats)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_TxStatsType* TxStats);
    Std_ReturnType (
        *EthSwt_GetTxErrorCounterValues)(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_TxErrorCounterValuesType* TxStats);
    Std_ReturnType (*EthSwt_StoreConfiguration)(uint8 SwitchIdx);
    Std_ReturnType (*EthSwt_ResetConfiguration)(uint8 SwitchIdx);
    Std_ReturnType (
        *EthSwt_GetMacLearningMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode);
    Std_ReturnType (*EthSwt_SetMgmtInfo)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, const EthSwt_MgmtInfoType* MgmtInfoPtr);
    Std_ReturnType (
        *EthSwt_PortEnableTimeStamp)(uint8 SwitchIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtInfoType* MgmtInfoPtr);
    Std_ReturnType (*EthSwt_VerifyConfig)(uint8 SwitchIdx, boolean* Result);
    Std_ReturnType (*EthSwt_SetForwardingMode)(uint8 SwitchIdx, boolean mode);
    Std_ReturnType (*EthSwt_GetPortSignalQuality)(uint8 SwitchIdx, uint8 PortIdx, uint32* SignalQualityPtr);
    Std_ReturnType (*EthSwt_GetPortIdentifier)(
        uint8   SwitchIdx,
        uint8   PortIdx,
        uint32* OrgUniqueIdPtr,
        uint8*  ModelNrPtr,
        uint8*  RevisionNrPtr);
    Std_ReturnType (*EthSwt_GetSwitchIdentifier)(uint8 SwitchIdx, uint32* OrgUniqueIdPtr);
    Std_ReturnType (*EthSwt_WritePortMirrorConfiguration)(
        uint8                           MirroredSwitchIdx,
        const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);
    Std_ReturnType (*EthSwt_ReadPortMirrorConfiguration)(
        uint8                     MirroredSwitchIdx,
        EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);
    Std_ReturnType (*EthSwt_DeletePortMirrorConfiguration)(uint8 MirroredSwitchIdx);
    Std_ReturnType (
        *EthSwt_GetPortMirrorState)(uint8 SwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType* PortMirrorStatePtr);
    Std_ReturnType (*EthSwt_SetPortMirrorState)(uint8 MirroredSwitchIdx, EthSwt_PortMirrorStateType PortMirrorState);
    Std_ReturnType (*EthSwt_SetPortTestMode)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTestModeType Mode);
    Std_ReturnType (*EthSwt_SetPortLoopbackMode)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyLoopbackModeType Mode);
    Std_ReturnType (*EthSwt_SetPortTxMode)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTxModeType Mode);
    Std_ReturnType (*EthSwt_RunPortCableDiagnostic)(uint8 SwitchIdx, uint8 PortIdx);
    Std_ReturnType (
        *EthSwt_GetPortCableDiagnosticsResult)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_CableDiagResultType* ResultPtr);
    Std_ReturnType (*EthSwt_GetCfgDataRaw)(uint8 SwitchIdx, uint32 Offset, uint16 Length, uint8* BufferPtr);
    Std_ReturnType (*EthSwt_GetCfgDataInfo)(uint8 SwitchIdx, uint32* DataSizePtr, uint32* DataAdressPtr);
    Std_ReturnType (*EthSwt_PortLinkStateRequest)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_LinkStateType PortLinkState);
    Std_ReturnType (*EthSwt_GetMaxFIFOBufferFillLevel)(
        uint8   SwitchIdx,
        uint8   SwitchPortIdx,
        uint8   SwitchPortEgressFifoIdx,
        uint32* SwitchPortEgressFifoBufferLevelPtr);
    Std_ReturnType (
        *EthSwt_GetRxMgmtObject)(uint8 CtrlIdx, Eth_DataType* DataPtr, EthSwt_MgmtObjectType** MgmtObjectPtr);
    Std_ReturnType (
        *EthSwt_GetTxMgmtObject)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtObjectType** MgmtObjectPtr);
    Std_ReturnType (*EthSwt_CheckWakeup)(uint8 SwitchIdx);
    Std_ReturnType (
        *EthSwt_GetSwitchPortWakeupReason)(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_WakeupReasonType* WakeupReasonPtr);
} EthIf_EthSwtDriverApiType;

/**
 * @brief configuration type of EthIfController.
 */
typedef struct EthIf_ControllerCfgTag
{
    uint32 EthIfMaxTxBufsTotal;
    uint16 EthIfCtrlMtu;
    uint16 EthIfVlanId;
    uint8  EthIfPhysControllerIdx;
    /* transceiver ref type */
    EthIf_CtrlRefTrcvType TrcvRefType;
    /* ref transceiver this is index of transceiver else if ref switch this is index of switch */
    uint8 TrcvRefIdx;
    /* controller index in Eth context */
    uint8 EthIfEthCtrlIdx;
    /* eth driver index in EthIf context */
    uint8 EthIfEthIfDriverIdx;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 EthIfPartition;
#endif
} EthIf_ControllerCfgType;

/**
 * @brief configuration type of EthIfFrameOwer.
 */
typedef struct EthIf_FrameOwnerCfgTag
{
    uint16 EthIfFrameType;
    uint8  EthIfOwner;
} EthIf_FrameOwnerCfgType;

/**
 * @brief configuration type of EthIfTransceiver.
 */
typedef struct EthIf_TrcvCfgTag
{
    EcuM_WakeupSourceType WakeUpSource;
    uint8                 EthIfEthTrcvIdx; /*to identify the different Ethtrcv channel*/
    uint8                 EthTrcvDriverId; /*to identify the different Trcv API*/
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 EthIfPartition;
#endif
} EthIf_TrcvCfgType;

/**
 * @brief configuration type of EthIfPhysController.
 */
typedef struct EthIf_PhysControllerTag
{
    /* EthIfController ID whitch refered this physical controller */
    uint8 EthIfEthIfCtrlId; /*to identify the different driver API*/
} EthIf_PhysControllerType;

/**
 * @brief switchPortGroup RefSemantic type.
 */
typedef enum
{
    ETHIF_SWITCH_PORT_GROUP_CONTROL,
    ETHIF_SWITCH_PORT_GROUP_LINK_INFO,
    ETHIF_SWITCH_PORT_GROUP_NONE
} EthIf_PortGroupSemanticType;

/**
 * @brief configuration type of EthIfSwitchPort.
 */
typedef struct EthIf_EthSwtPortCfgTag
{
    uint8 EthPortIdx;
    /* EthSwt index in context of EthIf */
    uint8 EthIfEthIfSwtIdx;
    /* Id of Ethtransceiver the switchport referenced */
    uint8 ReferredEthTrcv;
} EthIf_EthSwtPortCfgType;

/**
 * @brief configuration type of EthIfSwitchPortGroup.
 */
typedef struct EthIf_PortGroupTag
{
    /* ethif controller whitch referenced this switch */
    uint8                       ReferencedByCtrl;
    EthIf_PortGroupSemanticType EthIfPortGroupRefSemantics;
    uint8                       EthSwtPortNum;
    uint8                       HostPort;
    uint8                       UpLinkPort;
    const uint8*                RefEthSwtPortList;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 EthIfPartition;
#endif
} EthIf_PortGroupType;

/**
 * @brief configuration type of EthIfSwitch.
 */
typedef struct EthIf_SwitchCfgTag
{
    EcuM_WakeupSourceType WakeUpSource;
    uint8                 EthIfEthSwtIdx;
    /* ethif controller whitch referenced this switch */
    uint8 ReferencedByCtrl;
    uint8 EthIfEthSwtDriverId;
    uint8 HostPortIdxInEthIf;
    /* start index of SwitchPorts of a switch on a partition */
    uint8 EthIfSwtPortOnPartitonStarIdx;
    /* number of ports on this switch */
    uint8 NumberOfPorts;
    /* index array where store ports information of this switch in EthIfEthSwtPortCfg*/
    const uint8* RefEthSwtPortList;
#if (STD_ON == ETHIF_ECUC_MULTIPLE_PARTITION_USED)
    uint8 EthIfPartition;
#endif
} EthIf_SwitchCfgType;

/**
 * @brief configuration type of EthIf module.
 */
typedef struct EthIf_ConfigTag
{
    /* pointer to hold controller config data */
    const EthIf_ControllerCfgType* EthIfCtrlCfg;
    /* pointer to hold Owner config data */
    const EthIf_FrameOwnerCfgType* EthIfOwnerCfg;
    /* pointer to holad trcv config data */
    const EthIf_TrcvCfgType* EthIfTrcvCfg;
    /* pointer to hold PhysController config data */
    const EthIf_PhysControllerType* EthIf_PhysControllerCfg;
    const EthIf_SwitchCfgType*      EthIfSwitchCfg; /* pointer to hold switches config data */
    const EthIf_EthSwtPortCfgType*  EthIfEthSwtPortCfg;
    const EthIf_PortGroupType*      EthIfSwtPortGroupCfg;

    /* pointer to hold Tx confirmation functions list */
    const EthIfULTxConfirmationFuncType* UserTxConfirmation;
    /* Ptr to Rx indication function list */
    const EthIfULRxIndicationFuncType* UserRxIndication;
    /* pointer to hold Trcv Link state functions list */
    const EthIfTrcvLinkStateChgFuncType* UserTrcvLinkStateChg;

    const EthIf_EthDriverApiType*     EthIfEthDrvApiCfg;
    const EthIf_EthTrcvDriverApiType* EthIfEthTrcvApiCfg;
    const EthIf_EthSwtDriverApiType*  EthIfEthSwitchApiCfg;
#if (ETHIF_USED_PARTITION_NUM > 1)
    /* index of EthIfController sorted by partition index  */
    const uint8* CtrlIndexMap;
    /* index range of EthIfController on each partion */
    const uint8* CtrlIndexRangeMap;
#endif
#if ((ETHIF_USED_PARTITION_NUM > 1) && (ETHIF_SWITCHPORT_PORTS_NUM > 0))
    /* index of EthIfController sorted by partition index  */
    const uint8* SwitchPortIndexMap;
    /* index range of EthIfController on each partion */
    const uint8* SwitchPortIndexRangeMap;
#endif
#if (ETHIF_SWITCHPORT_PORTS_NUM > 0)
    /*  EthIfPortStartupActiveTime */
    uint16 PortStartUpActiveTimeCnt;
#endif
#if (ETHIF_SWITCH_PORT_GROUP_GROUP_NUM > 0)
    /* EthIfSwitchOffPortTimeDelay */
    uint16 SwitchOffPortTimeDelayCnt;
#endif
} EthIf_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
