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
/*********************************************************************************
**                                                                            **
**  FILENAME    : LinIf_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of LinIf               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef LINIF_TYPES_H
#define LINIF_TYPES_H

#include "ComStack_Types.h"
#include "LinIf_Cfg.h"
#include "EcuM.h"
#include "Lin_GeneralTypes.h"
#include "Com_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LINIF_TYPES_H_AR_RELEASE_MAJOR_VERSION    4
#define LINIF_TYPES_H_AR_RELEASE_MINOR_VERSION    5
#define LINIF_TYPES_H_AR_RELEASE_REVISION_VERSION 0

#define LINIF_TYPES_H_SW_MAJOR_VERSION            2
#define LINIF_TYPES_H_SW_MINOR_VERSION            0
#define LINIF_TYPES_H_SW_PATCH_VERSION            0

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
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
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
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
    LINIF_RX_PDU = 0u,
    LINIF_TX_PDU = 1u,
    LINIF_INTERNAL_PDU = 2u,
    LINIF_SLAVE_TO_SLAVE_PDU = 3u
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

typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(void, LINIF_APPL_CODE, LinIfRxIndicationUL)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    LinIf_ULModuleType LinIfUserRxIndicationUL;
    PduIdType LinIfRxPduRef;
} LinIf_RxPduType;

typedef struct
{
    P2FUNC(void, LINIF_APPL_CODE, LinIfTxConfirmationUL)(PduIdType TxPduId); /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduIdType LinIfTxPduId;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinIfTxTriggerTransmitUL)(PduIdType TxPduId, PduInfoType* PduInfoPtr);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    LinIf_ULModuleType LinIfUserTxUL;
    PduIdType LinIfTxPduRef;
    boolean LinIfContainResponseErrorSignal;
} LinIf_TxPduType;

typedef struct
{
    LinIf_PduDirectionIdType LinIfPduDirectionId;
    P2CONST(LinIf_RxPduType, AUTOMATIC, LINIF_APPL_CONST) LinIfRxPdu;
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_APPL_CONST) LinIfTxPdu;
} LinIf_PduDirectionType;

typedef struct
{
    uint8 LinIfFramePriority;
    uint16 LinIfSubstitutionFrameRef;
} LinIf_SubstitutionFramesType;

typedef struct
{
    uint8 LinIfFixedFrameSduBytePos;
    uint8 LinIfFixedFrameSduByteVal;
} LinIf_FixedFrameSduByteType;

typedef struct
{
    LinIf_FixedFrameSduByteType LinIfFixedFrameSduByte[8];
} LinIf_FixedFrameSduType;

typedef struct
{
    LinIf_ChecksumType LinIfChecksumType;
    uint8 LinIfFrameId;
    uint8 LinIfProtectedId;
    uint8 LinIfFrameIndex;
    uint8 LinIfLength;
    uint8 LinIfFrameIdAssociatedWithEvent;
    LinIf_FrameTypeType LinIfFrameType;
    P2CONST(LinIf_FixedFrameSduType, AUTOMATIC, LINIF_APPL_DATA) LinIfFixedFrameSdu;
    P2CONST(LinIf_PduDirectionType, AUTOMATIC, LINIF_APPL_DATA) LinIfPduDirection;
    uint8 LinIfNumOfSubstitutionFrame;
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_APPL_CONST) LinIfSubstitutionFrames;
    P2VAR(boolean, AUTOMATIC, LINIF_APPL_DATA) LinIfIsTransmitPending; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} LinIf_FrameType;

typedef struct
{
    uint8 LinIfJitter;
} LinIf_MasterType;

typedef struct
{
    uint8 LinIfConfiguredNAD;
    uint16 LinIfFunctionId;
    uint8 LinIfInitialNAD;
    uint16 LinIfNasTimeoutCnt;
    uint16 LinIfSupplierId;
    uint8 LinIfVariantId;
} LinIf_NodeConfigurationIdentificationType;

typedef struct
{
    LinIf_LinProtocolVersionType LinIfLinProtocolVersion;
    Com_SignalIdType LinIfResponseErrorSignal;
    boolean LinIfResponseErrorSignalConfigured;
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST)
    LinIfNodeConfigurationIdentification;
} LinIf_SlaveType;

typedef struct
{
    uint8 LinIfDelay;
    uint8 LinIfEntryIndex;
    LinIf_SchHandleType LinIfCollisionResolvingRef;
    uint16 LinIfFrameRef;
} LinIf_EntryType;

typedef struct
{
    LinIf_NodeTypeType LinIfNodeType;
    P2CONST(LinIf_MasterType, AUTOMATIC, LINIF_CONST) LinIfMaster;
    P2CONST(LinIf_SlaveType, AUTOMATIC, LINIF_CONST) LinIfSlave;
} LinIf_NodeType;

typedef struct
{
    LinIf_ResumePositionType LinIfResumePosition;
    LinIf_RunModeType LinIfRunMode;
    LinIf_SchHandleType LinIfScheduleTableIndex;
    P2CONST(LinIf_EntryType, AUTOMATIC, LINIF_APPL_CONST) LinIfEntry;
    uint16 LinIfNumOfEntry;
} LinIf_ScheduleTableType;

typedef struct
{
    uint8 LinChannelIdRef;
    uint8 LinDriverId;
    EcuM_WakeupSourceType WakeUpSource;
} LinIf_LinDriverChannelRef;

typedef struct
{
    uint8 LinIfTrcvIdRef;
    EcuM_WakeupSourceType WakeUpSource;
} LinIf_TransceiverDrvConfigType;

typedef struct
{
    uint16 LinIfMainFunctionPeriod;
    uint16 LinIfBusIdleTimeoutCnt;
    uint16 LinIfSleepModeFrameDelayCnt;
    LinIf_ULModuleType LinIfGotoSleepConfirmationUL;
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2FUNC(void, LINIF_APPL_CODE, GotoSleepConfirmation)(NetworkHandleType channel, boolean success);
    LinIf_ULModuleType LinIfGotoSleepIndicationUL;
    P2FUNC(void, LINIF_APPL_CODE, GotoSleepIndication)
    (NetworkHandleType channel);
    uint16 LinIfMaxFrameCnt;
    boolean LinIfScheduleChangeNextTimeBase;
    LinIf_ULModuleType LinIfScheduleRequestConfirmationUL;
    P2FUNC(void, LINIF_APPL_CODE, ScheduleRequestConfirmation)(NetworkHandleType channel, LinIf_SchHandleType schedule);
    LinIf_StartupStateType LinIfStartupState;
    LinIf_ULModuleType LinIfWakeupConfirmationUL;
    P2FUNC(void, LINIF_APPL_CODE, WakeupConfirmation)(NetworkHandleType channel, boolean success);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(LinIf_LinDriverChannelRef, AUTOMATIC, LINIF_APPL_CONST) LinIfChannelRef;
    NetworkHandleType LinIfComMNetworkHandleRef;
    uint16 LinIfNumOfFrame;
    uint16 LinIfFrameIndexOffset;
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) LinIfFrame;
    P2CONST(LinIf_NodeType, AUTOMATIC, LINIF_APPL_CONST) LinIfNodeType;
    LinIf_SchHandleType LinIfNumOfSchedule;
    LinIf_SchHandleType LinIfScheduleIndexOffset;
    P2CONST(LinIf_ScheduleTableType, AUTOMATIC, LINIF_APPL_CONST) LinIfScheduleTable;
    P2CONST(LinIf_TransceiverDrvConfigType, AUTOMATIC, LINIF_APPL_CONST) LinIfTransceiverDrvConfig;
} LinIf_ChannelType;

typedef struct
{
    uint16 LinIfNumOfSubstitution;
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_APPL_CONST) LinIfSubstitution;
    uint16 LinIfNumOfTxPdu;
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_APPL_CONST) LinIfTxPdu;
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) LinIfFrame;
    P2CONST(LinIf_ChannelType, AUTOMATIC, LINIF_APPL_CONST) LinIfChannel;
} LinIf_ConfigType;

typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(Lin_StatusType, LINIF_APPL_CODE, LinGetStatus)(uint8 Channel, uint8** Lin_SduPtr);
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinGoToSleep)(uint8 Channel);
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinGoToSleepInternal)(uint8 Channel);
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_431)
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinSendFrame)(uint8 Channel, const Lin_PduType* PduInfoPtr);
#else
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinSendFrame)(uint8 Channel, Lin_PduType* PduInfoPtr);
#endif
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinWakeup)(uint8 Channel);
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinWakeupInternal)(uint8 Channel);
    P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinCheckWakeup)(uint8 Channel);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} Lin_DriverApiType;

#endif
