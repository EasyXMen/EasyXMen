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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : EthIf_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef ETHIF_TYPES_H
#define ETHIF_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthIf_Cfg.h"
#include "Eth_GeneralTypes.h"
#include "EcuM.h"
#include "Platform_Types.h"
#include "ComStack_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* @req SWS_EthIf_00150 */
typedef enum
{
    ETHIF_STATE_UNINIT, /* Ethernet Interface is not yet configured*/
    ETHIF_STATE_INIT,   /* Ethernet Interface is configured*/
} EthIf_StateType;

/* Definition of EthIf_MeasurementIdxType */
typedef uint8 EthIf_MeasurementIdxType;
#define ETHIF_MEAS_DROP_CRTLIDX 0x01u
#define ETHIF_MEAS_ALL          0xFFu

/* Definition referenced ethernet hardware for ethernet interface controller*/
#define ETHIF_CTRL_REF_HW_NONE           0x00u
#define ETHIF_CTRL_REF_HW_TRCV           0x01u
#define ETHIF_CTRL_REF_HW_SWT            0x02u
#define ETHIF_CTRL_REF_HW_SWT_PORT_GROUP 0x04u

/*definition no Vlan ID is set for ethernet interface controller */
#define ETHIF_VID_UNUSED 0x1000u

typedef uint8 EthIf_SwitchPortGroupIdxType;

typedef void (*EthIfULRxIndicationType)(
    uint8 CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8* PhysAddrPtr,
    const uint8* DataPtr,
    uint16 LenByte);

typedef void (*EthIfULTxConfirmationType)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Std_ReturnType Result);

/* Function pointer for User defined TrcvLinkState callback */
typedef void (*EthIfTrcvLinkStateChgType)(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState);

typedef struct
{
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_SetControllerMode)
    (uint8 CtrlIdx, Eth_ModeType CtrlMode);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetControllerMode)
    (uint8 CtrlIdx, Eth_ModeType* CtrlModePtr);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    P2FUNC(BufReq_ReturnType, ETHIF_APPL_CODE, Eth_ProvideTxBuffer)
    (uint8 CtrlIdx, uint8 Priority, Eth_BufIdxType* BufIdxPtr, uint8** BufPtr, uint16* LenBytePtr);
#else  /*The default version of eth driver is 4.2.2*/
    P2FUNC(BufReq_ReturnType, ETHIF_APPL_CODE, Eth_ProvideTxBuffer)
    (uint8 CtrlIdx, Eth_BufIdxType* BufIdxPtr, uint8** BufPtr, uint16* LenBytePtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_Transmit)
    (uint8 CtrlIdx,
     Eth_BufIdxType BufIdx,
     Eth_FrameType FrameType,
     boolean TxConfirmation,
     uint16 LenByte,
     const uint8* PhysAddrPtr);
    P2FUNC(void, ETHIF_APPL_CODE, Eth_SetPhysAddr)(uint8 CtrlIdx, const uint8* PhysAddrPtr);
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetPhysAddr)(uint8 CtrlIdx, uint8* PhysAddrPtr);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_UpdatePhysAddrFilter)
    (uint8 CtrlIdx, const uint8* PhysAddrPtr, Eth_FilterActionType Action);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetCurrentTime)
    (uint8 CtrlIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_EnableEgressTimeStamp)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);
#else  /*The default version of eth driver is 4.2.2*/
    P2FUNC(void, ETHIF_APPL_CODE, Eth_EnableEgressTimeStamp)(uint8 CtrlIdx, uint8 BufIdx);
#endif /*ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431*/
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetEgressTimeStamp)
    (uint8 CtrlIdx, Eth_BufIdxType BufIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetEgressTimeStamp)
    (uint8 CtrlIdx, Eth_BufIdxType BufIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#else  /*The default version of eth driver is 4.2.2*/
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetEgressTimeStamp)
    (uint8 CtrlIdx, uint8 BufIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetIngressTimeStamp)
    (uint8 CtrlIdx, const Eth_DataType* DataPtr, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#elif (ETHIF_ETH_AUTOSAR_VERSION == ETHIF_ETH_AUTOSAR_431)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetIngressTimeStamp)
    (uint8 CtrlIdx, const Eth_DataType* DataPtr, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#else  /*The default version of eth driver is 4.2.2*/
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetIngressTimeStamp)
    (uint8 CtrlIdx, Eth_DataType* DataPtr, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_440 */
#if (ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_Receive)
    (uint8 CtrlIdx, uint8 FifoIdx, Eth_RxStatusType* RxStatusPtr);
#else  /*The default version of eth driver is 4.2.2*/
    P2FUNC(void, ETHIF_APPL_CODE, Eth_Receive)(uint8 CtrlIdx, Eth_RxStatusType* RxStatusPtr);
#endif /* ETHIF_ETH_AUTOSAR_VERSION >= ETHIF_ETH_AUTOSAR_431 */
    P2FUNC(void, ETHIF_APPL_CODE, Eth_TxConfirmation)(uint8 CtrlIdx);
} EthIf_EthDriverApiType;

typedef struct
{
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverWakeupMode)
    (uint8 TrcvIdx, EthTrcv_WakeupModeType TrcvWakeupMode);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverWakeupMode)
    (uint8 TrcvIdx, EthTrcv_WakeupModeType* TrcvWakeupModePtr);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_TransceiverLinkStateRequest)
    (uint8 TrcvIdx, EthTrcv_LinkStateType LinkState);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverMode)
    (uint8 TrcvIdx, Eth_ModeType* TrcvModePtr);
#else
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverMode)
    (uint8 TrcvIdx, EthTrcv_ModeType* TrcvModePtr);
#endif /* ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450 */
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetLinkState)
    (uint8 TrcvIdx, EthTrcv_LinkStateType* LinkStatePtr);
#if (ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverMode)
    (uint8 TrcvIdx, Eth_ModeType TrcvMode);
#else
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverMode)
    (uint8 TrcvIdx, EthTrcv_ModeType TrcvMode);
#endif /* ETHIF_ETHTRCV_AUTOSAR_VERSION >= ETHIF_ETHTRCV_AUTOSAR_450 */
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_CheckWakeup)(uint8 TrcvIdx);
} EthIf_EthTrcvDriverApiType;

typedef struct
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
        *EthSwt_SetMacLearningMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType MacLearningMode);
    Std_ReturnType (
        *EthSwt_GetMacLearningMode)(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode);
    Std_ReturnType (*EthSwt_SetMgmtInfo)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, const EthSwt_MgmtInfoType* MgmtInfoPtr);
    Std_ReturnType (
        *EthSwt_PortEnableTimeStamp)(uint8 SwitchIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtInfoType* MgmtInfoPtr);
    Std_ReturnType (*EthSwt_VerifyConfig)(uint8 SwitchIdx, boolean* Result);
    Std_ReturnType (*EthSwt_SetForwardingMode)(uint8 SwitchIdx, boolean mode);
    Std_ReturnType (*EthSwt_GetPortSignalQuality)(uint8 SwitchIdx, uint8 PortIdx, uint32* SignalQualityPtr);
    Std_ReturnType (*EthSwt_GetPortIdentifier)(
        uint8 SwitchIdx,
        uint8 PortIdx,
        uint32* OrgUniqueIdPtr,
        uint8* ModelNrPtr,
        uint8* RevisionNrPtr);
    Std_ReturnType (*EthSwt_GetSwitchIdentifier)(uint8 SwitchIdx, uint32* OrgUniqueIdPtr);
    Std_ReturnType (*EthSwt_WritePortMirrorConfiguration)(
        uint8 MirroredSwitchIdx,
        const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr);
    Std_ReturnType (*EthSwt_ReadPortMirrorConfiguration)(
        uint8 MirroredSwitchIdx,
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
        uint8 SwitchIdx,
        uint8 SwitchPortIdx,
        uint8 SwitchPortEgressFifoIdx,
        uint32* SwitchPortEgressFifoBufferLevelPtr);
    Std_ReturnType (
        *EthSwt_GetRxMgmtObject)(uint8 CtrlIdx, Eth_DataType* DataPtr, EthSwt_MgmtObjectType** MgmtObjectPtr);
    Std_ReturnType (
        *EthSwt_GetTxMgmtObject)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, EthSwt_MgmtObjectType** MgmtObjectPtr);
} EthIf_EthSwtDriverApiType;

typedef struct
{
    uint8 EthIfCtrlIdx;
    uint16 EthIfCtrlMtu;
    uint8 EthIfMaxTxBufsTotal;
    uint16 EthIfVlanId;
    uint8 EthIfPhysControllerIdRef;
    uint8 EthIfEthTrcvId;
    uint8 EthIfSwitchRef;
    uint8 EthIfSwitchPortGroupRef;
    uint8 EthIfRefHardWare;
} EthIf_ControllerCfgType;

typedef struct
{
    uint16 EthIfFrameType;
    uint8 EthIfOwner;
} EthIf_FrameOwnerCfgType;

typedef struct
{
    uint8 EthSwtIdx;
    uint8 EthSwtDriverId;
} EthIf_EthSwitchType;

typedef struct
{
    uint8 EthIfSwitchIdx;
    const EthIf_EthSwitchType* EthIfEthSwitchRef;
} EthIf_SwitchType;

typedef struct
{
    uint8 EthIfEthTrcvIdx; /*to identify the different Ethtrcv channel*/
    uint8 EthTrcvDriverId; /*to identify the different Trcv API*/
    EcuM_WakeupSourceType WakeUpSource;
} EthIfEthTrcvRefType;

typedef struct
{
    uint8 EthIfTrcvIdx;
    const EthIfEthTrcvRefType* EthIfEthTrcvRef;
} EthIf_TrcvType;

typedef struct
{
    uint8 EthIfEthCtrlId; /*to identify the different Eth channel*/
    uint8 EthDriverId;    /*to identify the different driver API*/
} EthIfEthCtrlRefType;

typedef struct
{
    uint8 EthIfPhysControllerIdx;
    const EthIfEthCtrlRefType* EthIfEthCtrlRef;
} EthIf_PhysControllerType;

typedef enum
{
    ETHIF_SWITCH_PORT_GROUP_CONTROL,
    ETHIF_SWITCH_PORT_GROUP_LINK_INFO,
    ETHIF_SWITCH_PORT_GROUP_NONE
} EthIf_PortGroupSemanticType;

typedef struct
{
    uint8 EthPortIdx;
    uint8 EthSwtIdx;
} EthIf_EthSwtPortType;

typedef struct
{
    uint8 EthIfSwitchPortGroupIdx;
    EthIf_PortGroupSemanticType EthIfPortGroupRefSemantics;
    uint8 EthSwtPortNum;
    const uint8* RefEthSwtPortIdList;
} EthIf_PortGroupType;

/* @req SWS_EthIf_00149 */
typedef struct
{
    P2CONST(EthIf_ControllerCfgType, AUTOMATIC, ETHIF_APPL_CONST)
    EthIfCtrlCfg; /*pointer to hold controller config data*/
    P2CONST(EthIf_FrameOwnerCfgType, AUTOMATIC, ETHIF_APPL_CONST) EthIfOwnerCfg; /* pointer to hold Owner config data */
    P2CONST(EthIf_TrcvType, AUTOMATIC, ETHIF_APPL_CONST) EthIfTrcvCfg;           /*pointer to holad trcv config data*/
    P2CONST(EthIf_PhysControllerType, AUTOMATIC, ETHIF_APPL_CONST)
    EthIf_PhysControllerCfg; /*pointer to hold PhysController config data*/
    P2CONST(EthIf_SwitchType, AUTOMATIC, ETHIF_APPL_CONST) EthIfSwitchCfg; /* pointer to hold switches config data */
    P2CONST(EthIf_EthSwitchType, AUTOMATIC, ETHIF_APPL_CONST) EthIfEthSwtCfg;
    P2CONST(EthIf_EthSwtPortType, AUTOMATIC, ETHIF_APPL_CONST) EthIfEthSwtPortCfg;
    P2CONST(EthIf_PortGroupType, AUTOMATIC, ETHIF_APPL_CONST) EthIfSwtPortGroupCfg;
    P2CONST(EthIfULTxConfirmationType, AUTOMATIC, ETHIF_APPL_CONST)
    EthIfULTxConfirmation; /* pointer to hold Tx confirmation functions list */
    P2CONST(EthIfULRxIndicationType, AUTOMATIC, ETHIF_APPL_CONST)
    EthIfULRxIndication; /* Ptr to Rx indication function list */
    P2CONST(EthIfTrcvLinkStateChgType, AUTOMATIC, ETHIF_APPL_CONST)
    EthIfTrcvLink;                    /*pointer to hold Trcv Link state functions list */
    uint8 EthIfCtrlCount;             /* No of If Controllers configure */
    uint8 EthIfTrcvCount;             /* No of Trcv configured */
    uint8 EthIfSwitchCount;           /* No of Switches configured */
    uint8 EthIfOwnersCount;           /* No of Owners configured */
    uint8 EthIfPhysControllerCount;   /* No of Eth */
    uint8 EthIfTrcvLinkStateChgCount; /* No of EthIfTrcvLinkStateChgConfig configured*/
    uint8 EthIfPortGroupCount;        /* No of switch port group*/
    uint8 EthIfEthSwitchCount;        /* No of Ethernet switch*/
    uint8 EthIfEthSwitchPortCount;    /* No of Ethernet switch port*/
} EthIf_ConfigType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* ETHIF_TYPES_H */
