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
 **  @file               : LinIf_Types.h
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of LinIf
 **
 ***********************************************************************************************************************/

#ifndef LINIF_TYPES_H_
#define LINIF_TYPES_H_
/* PRQA S 1536 EOF */ /* VL_LinIf_1536 */

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "EcuM.h"
#include "Lin_GeneralTypes.h"
#include "Com_Types.h"
#include "LinIf_Cfg.h"

/* ===================================================== macros ===================================================== */
#define LINIF_NC_RESPONSE_LEN            0x08u
#define LINIF_FIX_FRAME_BYTE_NUM         8u
#define LINIF_SDU_LEN                    8u
#define LINIF_SUPPLIER_ID_MSD            8u
#define LINIF_FUNCTION_ID_MSD            8u
#define LINIF_DO_NOT_CARE_FRAME_PID      0xffu
#define LINIF_ASSIGN_PID_MSG_PAYLOAD_LEN 4u
#define LINIF_NC_FRAME_PCI_VALUE         6u

/* ================================================ type definitions ================================================ */
typedef uint8 LinIf_SchHandleType;

typedef enum
{
    LINIF_CLASSIC,
    LINIF_ENHANCED
} LinIf_ChecksumType;

typedef enum
{
    LINIF_ASSIGN,
    LINIF_ASSIGN_FRAME_ID_RANGE,
#ifdef LINIF_NC_OPTIONAL_REQUEST_SUPPORTED
    LINIF_ASSIGN_NAD,
    LINIF_CONDITIONAL,
#endif
    LINIF_EVENT_TRIGGERED,
    LINIF_FREE,
    LINIF_MRF,
    LINIF_SAVE_CONFIGURATION,
    LINIF_SPORADIC,
    LINIF_SRF,
    LINIF_UNASSIGN,
    LINIF_UNCONDITIONAL
} LinIf_FrameTypeType;

typedef enum
{
    LINIF_CONTINUE_AT_IT_POINT,
    LINIF_START_FROM_BEGINNING
} LinIf_ResumePositionType;

typedef enum
{
    LINIF_RUN_CONTINUOUS,
    LINIF_RUN_ONCE
} LinIf_RunModeType;

typedef enum
{
    LINIF_UL_CDD,
    LINIF_UL_PDUR,
    LINIF_UL_LINSM
} LinIf_ULModuleType;

typedef enum
{
    LINIF_STARTUP_NORMAL,
    LINIF_STARTUP_SLEEP
} LinIf_StartupStateType;

typedef enum
{
    LINIF_RX_PDU             = 0,
    LINIF_TX_PDU             = 1,
    LINIF_INTERNAL_PDU       = 2,
    LINIF_SLAVE_TO_SLAVE_PDU = 3
} LinIf_PduDirectionIdType;

typedef enum
{
    LINIF_VER_ISO17987,
    LINIF_VER_LIN13,
    LINIF_VER_LIN20,
    LINIF_VER_LIN21,
    LINIF_VER_LIN22
} LinIf_LinProtocolVersionType;

typedef enum
{
    LINIF_MASTER,
    LINIF_SLAVE
} LinIf_NodeTypeType;

typedef enum
{
    LINIF_UNINIT,
    LINIF_INIT
} LinIf_StatusType;

typedef enum
{
    LINIF_CHANNEL_UNINIT,
    LINIF_CHANNEL_OPERATIONAL,
    LINIF_CHANNEL_GOTO_SLEEP,
    LINIF_CHANNEL_SLEEP
} LinIf_ChannelStateType;

typedef enum
{
    LINIF_SLAVE_FRAME_IDLE,
    LINIF_SLAVE_FRAME_RESPONSE /*Response transmitting or receiving*/
} LinIf_SlaveFrameStatusType;

/**
 * @brief Container the configuration param of the rxPdu referenced by linif frame
 */
typedef struct LinIf_RxPduTag
{
    void (*RxIndicationUL)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
    LinIf_ULModuleType UserRxIndicationUL;
    PduIdType          RxPduRef;
} LinIf_RxPduType;

/**
 * @brief Container the configuration param of the txPdu referenced by linif frame
 */
typedef struct LinIf_TxPduTag
{
    void (*TxConfirmationUL)(PduIdType TxPduId, Std_ReturnType result);
    LinIf_ULModuleType UserTxUL;
    PduIdType          TxPduId;
    PduIdType          TxPduRef;
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
    ApplicationType PduApplicationId;
#endif
    Std_ReturnType (*TxTriggerTransmitUL)(PduIdType TxPduId, PduInfoType* PduInfoPtr);
    boolean ContainResponseErrorSignal;
} LinIf_TxPduType;

/**
 * @brief container the pdu direction config, referenced by linif frame
 */
typedef struct LinIf_PduDirectionTag
{
    const LinIf_RxPduType*   RxPdu;
    const LinIf_TxPduType*   TxPdu;
    LinIf_PduDirectionIdType PduDirectionId;
} LinIf_PduDirectionType;

/**
 * @brief Define linif frame's substitution frame
 */
typedef struct LinIf_SubstitutionFramesTag
{
    uint16 SubstitutionFrameRef;
    uint8  FramePriority;
} LinIf_SubstitutionFramesType;

/**
 * @brief Define linif frame's fixed frame sdu byte
 */
typedef struct LinIf_FixedFrameSduByteTag
{
    uint8 FixedFrameSduBytePos;
    uint8 FixedFrameSduByteVal;
} LinIf_FixedFrameSduByteType;

/**
 * @brief Define linif frame's fixed frame sdu
 */
typedef struct LinIf_FixedFrameSduTag
{
    LinIf_FixedFrameSduByteType FixedFrameSduByte[LINIF_FIX_FRAME_BYTE_NUM];
} LinIf_FixedFrameSduType;

/**
 * @brief Container linif frame configeuration param
 */
typedef struct LinIf_FrameTag
{
    const LinIf_FixedFrameSduType*      FixedFrameSdu;
    const LinIf_PduDirectionType*       PduDirection;
    const LinIf_SubstitutionFramesType* SubstitutionFrames;
    boolean*                            IsTransmitPending;
    LinIf_ChecksumType                  ChecksumType;
    LinIf_FrameTypeType                 FrameType;
    uint8                               FrameId;
    uint8                               ProtectedId;
    uint8                               FrameIndex;
    uint8                               Length;
    uint8                               FrameIdAssociatedWithEvent;
    uint8                               NumOfSubstitutionFrame;
} LinIf_FrameType;

/**
 * @brief Container linif master node configuration param
 */
typedef struct LinIf_MasterTag
{
    uint8 Jitter;
} LinIf_MasterType;

/**
 * @brief Define slave node configuration referenced by linif slave node
 */
typedef struct LinIf_NodeConfigurationIdentificationTag
{
    uint16 FunctionId;
    uint16 NasTimeoutCnt;
    uint16 SupplierId;
    uint8  ConfiguredNAD;
    uint8  InitialNAD;
    uint8  VariantId;
} LinIf_NodeConfigurationIdentificationType;

/**
 * @brief Container slave node configuration
 */
typedef struct LinIf_SlaveTag
{
    const LinIf_NodeConfigurationIdentificationType* NodeConfigurationIdentification;
    LinIf_LinProtocolVersionType                     LinProtocolVersion;
    Com_SignalIdType                                 ResponseErrorSignal;
    boolean                                          ResponseErrorSignalConfigured;
} LinIf_SlaveType;

/**
 * @brief Container linif entry container referenced by linif schedule table
 */
typedef struct LinIf_EntryTag
{
    uint16 FrameRef;
    uint8  Delay;
    uint8  EntryIndex;
#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
    LinIf_SchHandleType CollisionResolvingRef;
#endif
} LinIf_EntryType;

/**
 * @brief Define LinIf master node and slave node param
 */
typedef struct LinIf_NodeTag
{
    const LinIf_MasterType* Master;
    const LinIf_SlaveType*  Slave;
    LinIf_NodeTypeType      NodeType;
} LinIf_NodeType;

/**
 * @brief Define LinIf schedule table param
 */
typedef struct LinIf_ScheduleTableTag
{
    const LinIf_EntryType*   Entry;
    LinIf_ResumePositionType ResumePosition;
    LinIf_RunModeType        RunMode;
    uint16                   NumOfEntry;
    LinIf_SchHandleType      ScheduleTableIndex;
} LinIf_ScheduleTableType;

/**
 * @brief Define lin channel param referenced by LinIf
 */
typedef struct LinIf_LinDriverChannelRefTag
{
    EcuM_WakeupSourceType WakeUpSource;
    uint8                 LinChannelIdRef;
    uint8                 LinDriverId;
} LinIf_LinDriverChannelRef;

/**
 * @brief LinIf transceiver driver config
 */
typedef struct LinIf_TransceiverDrvConfigTag
{
    EcuM_WakeupSourceType WakeUpSource;
    uint8                 TrcvIdRef;
} LinIf_TransceiverDrvConfigType;

/**
 * @brief Define linif channel type
 */
typedef struct LinIf_ChannelTag
{
    LinIf_ULModuleType GotoSleepConfirmationUL;
    void (*GotoSleepConfirmation)(NetworkHandleType channel, boolean success);
    LinIf_ULModuleType GotoSleepIndicationUL;
    void (*GotoSleepIndication)(NetworkHandleType channel);
    LinIf_ULModuleType ScheduleRequestConfirmationUL;
    void (*ScheduleRequestConfirmation)(NetworkHandleType channel, LinIf_SchHandleType schedule);
    LinIf_ULModuleType WakeupConfirmationUL;
    void (*WakeupConfirmation)(NetworkHandleType channel, boolean success);
    LinIf_StartupStateType                StartupState;
    const LinIf_LinDriverChannelRef*      ChannelRef;
    const LinIf_FrameType*                Frame;
    const LinIf_NodeType*                 NodeType;
    const LinIf_ScheduleTableType*        ScheduleTable;
    const LinIf_TransceiverDrvConfigType* TransceiverDrvConfig;
    uint16                                MainFunctionPeriod;
    uint16                                BusIdleTimeoutCnt;
    uint16                                SleepModeFrameDelayCnt;
    uint16                                MaxFrameCnt;
    uint16                                FrameIndexOffset;
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
    ApplicationType ChannelApplicationId;
#endif
    uint8               NumOfFrame;
    NetworkHandleType   ComMNetworkHandleRef;
    LinIf_SchHandleType NumOfSchedule;
    LinIf_SchHandleType ScheduleIndexOffset;
    boolean             ScheduleChangeNextTimeBase;
    boolean             IsRefByTpChannel;
} LinIf_ChannelType;

/**
 * @brief Define lin driver api
 */
typedef struct Lin_DriverApiTag
{
    Lin_StatusType (*LinGetStatus)(uint8 Channel, uint8** Lin_SduPtr);
    Std_ReturnType (*LinGoToSleep)(uint8 Channel);
    Std_ReturnType (*LinGoToSleepInternal)(uint8 Channel);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_431)
    Std_ReturnType (*LinSendFrame)(uint8 Channel, const Lin_PduType* PduInfoPtr);
#else
    Std_ReturnType (*LinSendFrame)(uint8 Channel, Lin_PduType* PduInfoPtr);
#endif
    Std_ReturnType (*LinWakeup)(uint8 Channel);
    Std_ReturnType (*LinWakeupInternal)(uint8 Channel);
    Std_ReturnType (*LinCheckWakeup)(uint8 Channel);
} Lin_DriverApiType;

/**
 * @brief Define LinIf slave node run time data type
 */
typedef struct LinIf_SlaveRuntimeTag
{
    uint8                      NcResponse[LINIF_NC_RESPONSE_LEN];
    const LinIf_FrameType*     CurFrame;
    LinIf_ChannelStateType     ChannelState;
    LinIf_SlaveFrameStatusType FrameStatus;
    uint16                     NasTimer;
    uint16                     BusIdleTimer;
    uint16                     EventTrgTxFrmIdx;
    boolean                    NcResponsePendingFlag;
    boolean                    ResponseError;
    boolean                    WakeupFlag;
    boolean                    WaitWakeupConfirmFlag;
    boolean                    GotoSleepConfirmationFlag;
} LinIf_SlaveRuntimeType;

typedef struct LinIf_MasterRuntimeTag
{
    Lin_PduType                    TxPdu;
    const LinIf_ScheduleTableType* CurSchedule;
    const LinIf_ScheduleTableType* ResSchedule;
    const LinIf_ScheduleTableType* RdySchedule;
    const LinIf_EntryType*         CurEntry;
    LinIf_ChannelStateType         ChannelState;
    PduIdType                      TxPduId;
    uint16                         CurEntryIndex;
    uint16                         ResEntryIndex;
    uint16                         RootEvent;
    uint16_least                   Timer;
#if (LINIF_MASTER_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
    LinIf_SchHandleType CollisionResolvingRef;
#endif
    boolean WakeupFlag;
} LinIf_MasterRuntimeType;

typedef struct LinIf_PbsMapTypeTag
{
    const uint16* ScheduleTableMapList;
    const uint16* RxPduMapList;
    const uint16* TxPduMapList;
    const uint16  ScheduleTableMapLen;
    const uint16  RxPduMapLen;
    const uint16  TxPduMapLen;
} LinIf_PbsMapType;

/**
 * @brief LinIf global config
 */
typedef struct LinIf_ConfigTag
{
    const LinIf_TxPduType*              TxPdu;
    const LinIf_FrameType*              Frame;
    const LinIf_ChannelType*            Channel;
    const LinIf_SubstitutionFramesType* Substitution;
    const LinIf_PbsMapType*             PbsMap;
    uint16                              NumOfSubstitution;
    uint16                              NumOfTxPdu;
    uint8                               NumOfMasterFrame;
    uint8                               NumOfSlaveFrame;
} LinIf_ConfigType;

#endif
