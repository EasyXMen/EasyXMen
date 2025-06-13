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
/*********************************************************************************
**                                                                            **
**  FILENAME    : EthTSyn.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for EthTSyn                                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>  <DATE>    <AUTHOR>        <REVISION LOG>
 *  V2.0.0  20200716 yuzhe.zhang    Initial version
 *  V2.0.1  20221108 Xinrun.Wang    Modify Ingress Interface,
 *                                  Add correctionField handle,
 *                                  Handle index out of bounds,
 *                                  Optimize time Calculation.
 *  V2.0.2  20221109 Xinrun.Wang    Add SchM for time optimization
 *  V2.0.3  20221114 Xinrun.Wang    Adjust get correction field algorithm,
 *                                  add validation of sequenceId and timeDomain.
 *                                  Adjust timeInterval, flags field.
 *  V2.0.4  20221115 Xinrun.Wang    Adjust PortId Field.
 *  V2.0.5  20221213 Xinrun.Wang    Adjust fields accoring to PRS,
 *                                  add configuration according to unknown fields,
 *                                  adjust handle for crc, ofs, and sgw.
 *  V2.0.6  20230105 Xinrun.Wang    Get BufID before calling EthIf_EnableEgressTimeStamp
 *  V2.0.7  20230324 Xinrun.Wang    Implement R20-11 PRS_TS_00198,
 *                                  Correct CorrectionField Calculation
 *                                  Validate pdelay calculation result
 *                                  Change Target Mac Add to configured one for all messages
 *  V2.0.8  20230508 Xinrun.Wang    Set CorrectionField to 0 for timeSlave
 *  V2.0.9  20230605 Xinrun.Wang    Match Master correctionField with slave
 *  V2.0.10 20230615 Xinrun.Wang    Add EthTSynGlobalTimeSequenceCounterHysteresis impl.
 *  V2.0.11 20230811 xiaojian.liang CPT-6371, unable to receive data from multicast MAC address
 *                                  CPT-6412, logMessageInterval is computed by the configuration generator
 *  V2.0.12 20230905 xiaojian.liang CPT-6808, compilation error if HardwareTimestampSupport is activated
 *                                  CPT-6796, generate SourcePortIdentity by phys address and port index
 *  V2.0.13 20230912 xiaojian.liang CPD-32994, verify the ETHTSYN_E_CTRL_IDX for the parameter CtrlIdx
 *                                  CPT-6807, verify the SourcePortIdentity in the received follow up message
 *  V2.0.14 20230930 xiaojian.liang detecting if rcvdPdelayResp[FollowUp]->requestingPortIdentity != thisPortIdentity
 *  V2.0.15 20231124 xiaojian.liang Performance optimization
 *          20231201 xiaojian.liang CPT-7665, Implement the time slave as the Peer delay responder
 *                                  CPT-7833, The cycle of the sync is delayed by EthTSynGlobalTimeDebounceTime
 *  V2.0.16 20240123 xiaojian.liang CPT-7851 EthTSyn_timereduce fails if time1.secondsHi >= time2.secondsHi &&
 *                                  time1.seconds < time2.seconds.
 *  V2.0.17 20240321 han.li         CPT-8327, Length is wrong when EthTSynTLVFollowUpOFSSubTLV is configured in
 *                                  EthTSyn_TranssmitMessage
 *          20240403 xiaojian.liang CPT-8634 Unable to sync if EthTSynGlobalTimeSequenceCounterJumpWidth is 0.
 *          20240422 xiaojian.liang CPT-7851 EthTSyn_TimeStampSub return 1 if t1.second + 1 == t2.second &&
 *                                  t1.nanosecond < t2.nanosecond.
 *          20240429 shuangyang.fu  CPT-8448 can not synchronize when HW stamp on master enabling.
 *          20240509 shuangyang.fu  CPT-8976 can not synchronize when crc checking enabling.
 *          20240511 han.li         CPT-8986 add the ETHTRCV_LINK_STATE_ACTIVE condition judgment for sending Sync and
 * Pdelay_Req msg.
 *          20240513 shuangyang.fu  CPT-9012 The sending order of TLV is inconsistent with the document.
 *          20240516 shuangyang.fu  CPT-8980 Invalid first actual argument of StbM_GetOffset.
 *          20240523 shuangyang.fu  CPT-9071 The configuration of the CRC checksum field(eg. CRCMessageLength
 * EthTSynCrcDomainNumber etc.) at the receiver end is not being used.
 *          20240627 shuangyang.fu  CPD-55289 package the DET Interface
 *  V2.0.18 20241017 xiongfei.shi   CPT-10256 Implementing a time synchronization policy in case of invalid local
 * timestamp
 *          20231109 xiongfei.shi   CPT-10601 Support for multiple EthTSynPortConfig configured under one
 * EthTSyncGlobalTimeDomain
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthTSyn<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 1531, 1532, 1533 MISRA Rule 8.7, 8.9.<br>
    Reason:The exception is global configuration data(1531, 1533) and API(1532).

    \li PRQA S 3415 MISRA Rule 13.5.<br>
    Reason:#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_IsPortIdentityEqual.
*/
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthTSyn.h"
#include "SchM_EthTSyn.h"
#include "EthTSyn_Types.h"
#include "istd_lib.h"
#include "StbM.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_CRC_SUPPORT == STD_ON)
#include "Crc.h"
#endif
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON) || (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
#include "Det.h"
#endif
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define ETHTSYN_C_AR_MAJOR_VERSION 4U
#define ETHTSYN_C_AR_MINOR_VERSION 5U
#define ETHTSYN_C_AR_PATCH_VERSION 0U
#define ETHTSYN_C_SW_MAJOR_VERSION 2U
#define ETHTSYN_C_SW_MINOR_VERSION 0U
#define ETHTSYN_C_SW_PATCH_VERSION 18U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                             \
    (ETHTSYN_C_AR_MAJOR_VERSION != ETHTSYN_H_AR_MAJOR_VERSION)    \
    || (ETHTSYN_C_AR_MINOR_VERSION != ETHTSYN_H_AR_MINOR_VERSION) \
    || (ETHTSYN_C_AR_PATCH_VERSION != ETHTSYN_H_AR_PATCH_VERSION) \
    || (ETHTSYN_C_SW_MAJOR_VERSION != ETHTSYN_H_SW_MAJOR_VERSION) \
    || (ETHTSYN_C_SW_MINOR_VERSION != ETHTSYN_H_SW_MINOR_VERSION) \
    || (ETHTSYN_C_SW_PATCH_VERSION != ETHTSYN_H_SW_PATCH_VERSION))

#error "EthTSyn.c version mismatching with EthTSyn.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#ifndef ETHTSYN_LOCAL
#define ETHTSYN_LOCAL static
#endif

#ifndef ETHTSYN_LOCAL_INLINE
#define ETHTSYN_LOCAL_INLINE static inline
#endif

#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#define ETHTSYN_DET_REPORT(ApiId, ErrorId) \
    ((void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId), (ErrorId)))
#endif

#define TIMEDOMIN_CFG(timedomainid) EthTSyn_ConfigData->EthTSynGlobalTimeDomain[timedomainid]
#define PORT_CFG(timedomainid, PortId) \
    EthTSyn_ConfigData->EthTSynGlobalTimeDomain[timedomainid].EthTSynPortConfig[PortId]
#define PORT_TOCFGDOMIN(PortId) EthTSyn_ConfigData->EthTSynGlobalTimeDomain[EthTSyn_Port[PortId].timedomainid]
#define PORT_TOCFGPORT(PortId)                                                     \
    EthTSyn_ConfigData->EthTSynGlobalTimeDomain[EthTSyn_Port[PortId].timedomainid] \
        .EthTSynPortConfig[EthTSyn_Port[PortId].portIdinTimedomain]
#define ETHTSYN_UNUSED(var)                                           (void)(var)

#define ETHTSYN_PHYS_ADDR_LENGTH                                      6u

#define ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET                    20u
#define ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET           44u
#define ETHTSYN_PDELAY_RESP_FOLLOW_UP_REQUESTING_PORT_IDENTITY_OFFSET 44u

#define ETHTSYN_HIBYTE(x)                                             (uint8)((uint16)(x) >> 8)
#define ETHTSYN_LOBYTE(x)                                             (uint8)(x)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
static Std_ReturnType EthTSyn_GetIngressTV(
    uint8 CtrlIdx,
    uint8 PortId,
    const uint8* DataPtr,
    EthTSyn_TimeStampType* TV);
static Std_ReturnType EthTSyn_GetEgressTV(
    uint8 CtrlIdx,
    uint8 PortId,
    EthTSyn_TimeStampType* TV,
    Eth_BufIdxType BufIdx);
static EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* Data);
static EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType VirtualLocalTime);
static void EthTSyn_TimeToData(EthTSyn_TimeStampType Time, uint8* Data);
static void EthTSyn_TranssmitMessage(EthTSynMessageType EthTSynMessage, uint16 PortId);
static boolean EthTSyn_StatusGet(uint16 PortId, uint8 status);
static void EthTSyn_CreatCorrectionField(uint64 NS, uint8* Data);
static EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* Data);
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_CRC_SUPPORT == STD_ON)
static Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint8 portId,
    CrcValidatedType ValidatedType,
    const uint8* Data,
    uint8* SGWByte,
    uint8* OFSByte,
    uint16 frameLength,
    EthTSynCrcFlagsRxValidatedCfgType* CrcFlagsRxValidated);
#endif
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbmUseEthHW(uint16 PortId);
#endif

static void EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp);
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2);
static boolean EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2);
ETHTSYN_LOCAL sint8 EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2);
ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td);
static boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentityA, const uint8* portIdentityB);
static EthTSynPortType* EthTSyn_FindPort(uint8 ctrlIdx);
static EthTSynMessageType EthTSyn_MainFunctionMaster(uint8 portIndex);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
static boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer);
static boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx);
static boolean EthTSyn_ValidateInitStatus(uint8 apiId);
static boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr);
static boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer);
static boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType Mode);
#endif
static void EthTSyn_InitPortStatus(EthTSynPortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"
static boolean EthTSyn_Initialized = FALSE;
#define ETHTSYN_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"
static EthTSynPortType EthTSyn_Port[ETHTSYN_ETH_PORT_NUM];
static const EthTSyn_ConfigType* EthTSyn_ConfigData;
static uint8 EthTSyn_ReceivedPdelayReqSourcePortId[ETHTSYN_ETH_PORT_NUM][ETHTSYN_PORT_IDENTITY_LENGTH];
#define ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * This function initializes the Time Synchronization over Ethernet.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @ConfigPtr, Pointer to the selected configuration set.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
void EthTSyn_Init(const EthTSyn_ConfigType* configPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateInit(configPtr))
#endif
    {
        EthTSyn_ConfigData = configPtr;
        uint8 EthTSyn_PortNum = 0u;
        uint8 i;
        for (i = 0u; i < ETHTSYN_GLOBALTIME_DOMAIN_NUM; i++)
        {
            uint8 j;
            for (j = 0u; j < TIMEDOMIN_CFG(i).EthTSynPortConfigNum; j++)
            {
                EthTSynPortType* port = &EthTSyn_Port[EthTSyn_PortNum];
                port->index = EthTSyn_PortNum;
                port->timedomainid = TIMEDOMIN_CFG(i).EthTSynGlobalTimeDomainId;
                port->CtrlId = PORT_CFG(i, j).EthTSynGlobalTimeEthIfRef;
                port->EthTSynSCHysteresisCount = 255u;
                port->PortRoleType = (NULL_PTR != PORT_CFG(i, j).EthTSynPortRole.EthTSynGlobalTimeMaster)
                                         ? ETHTSYN_MASTER
                                         : ETHTSYN_SLAVE;
                EthTSyn_InitPortStatus(port);
                port->EthTSynFramePrio = PORT_CFG(i, j).EthTSynFramePrio;
                port->portIdinTimedomain = j;
                /* SWS_EthTSyn_00003 */
                uint64 delay = PORT_CFG(i, j).EthTSynPdelayConfig.EthTSynGlobalTimePropagationDelay;
                port->pdelay.nanoseconds = (uint32)(delay % ETHTSYN_NANOSECOND_TO_SECOND);
                delay /= ETHTSYN_NANOSECOND_TO_SECOND;
                port->pdelay.seconds = (uint32)delay;
                port->pdelay.secondsHi = (uint16)(delay >> 32);
                port->EthTSyn_TransmissionMode = ETHTSYN_TX_OFF;

                /* Initialize data */
                (void)ILib_memset(
                    EthTSyn_ReceivedPdelayReqSourcePortId[EthTSyn_PortNum],
                    0,
                    ETHTSYN_PORT_IDENTITY_LENGTH);

                uint8 srcPhyAddr[ETHTSYN_PHYS_ADDR_LENGTH];
                EthIf_GetPhysAddr(port->CtrlId, srcPhyAddr);
                /* thisPortIdentity.clockIdentity */
                port->thisPortIdentity[0] = srcPhyAddr[0];
                port->thisPortIdentity[1] = srcPhyAddr[1];
                port->thisPortIdentity[2] = srcPhyAddr[2];
                port->thisPortIdentity[3] = 0xFFu;
                port->thisPortIdentity[4] = 0xFEu;
                port->thisPortIdentity[5] = srcPhyAddr[3];
                port->thisPortIdentity[6] = srcPhyAddr[4];
                port->thisPortIdentity[7] = srcPhyAddr[5];
                /* thisPortIdentity.portNumber */
                uint16 portNumber = (uint16)port->index + 1u;
                port->thisPortIdentity[8] = ETHTSYN_HIBYTE(portNumber);
                port->thisPortIdentity[9] = ETHTSYN_LOBYTE(portNumber);

                (void)ILib_memset(&port->portIdentity, 0, ETHTSYN_PORT_IDENTITY_LENGTH);
                port->userData.userByte0 = 0u;
                port->userData.userByte1 = 0u;
                port->userData.userByte2 = 0u;
                port->userData.userDataLength = 0u;
                port->TimeBaseUpdateCounter = 0u;
                port->preciseOriginTimestamp.nanoseconds = 0u;
                port->preciseOriginTimestamp.seconds = 0u;
                port->preciseOriginTimestamp.secondsHi = 0u;
                port->T1vlt = port->preciseOriginTimestamp;
                port->t1 = port->T1vlt;
                port->t2 = port->t1;
                port->t3 = port->t1;
                port->t4 = port->t1;
                port->pdelay = port->t1;
                EthTSyn_PortNum++;
            }
        }
        EthTSyn_Initialized = TRUE;
    }
}
#if (ETHTSYN_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): @versioninfo, Pointer to the memory location holding the version information of the module.
 * Return value: NA
 */
void EthTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTSYN_VAR) versioninfo)
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidatePointer(ETHTSYN_SID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID = ETHTSYN_VENDOR_ID;
        versioninfo->moduleID = ETHTSYN_MODULE_ID;
        versioninfo->sw_major_version = ETHTSYN_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ETHTSYN_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ETHTSYN_H_SW_PATCH_VERSION;
    }
}
#endif

/**
 * This API is used to turn on and off the TX capabilities of the EthTSyn.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @CtrlIdx Index of the Ethernet controller
                   @Mode ETHTSYN_TX_OFF ETHTSYN_TX_ON
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    NA
 */
void EthTSyn_SetTransmissionMode(
    uint8 CtrlIdx,
    EthTSyn_TransmissionModeType Mode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateTransmissionMode(CtrlIdx, Mode))
#endif
    {
        EthTSynPortType* port = EthTSyn_FindPort(CtrlIdx);
        port->EthTSyn_TransmissionMode = Mode;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#define ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#include "EthTSyn_MemMap.h"
/**
 * By this API service the EthTSyn gets an indication and the data of a received frame.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @CtrlIdx, Index of the Ethernet controller
                   @FrameType, frame type of received Ethernet frame
                   @IsBroadcast, parameter to indicate a broadcast frame
                   @PhysAddrPtr, pointer to Physical source address (MAC address in network byte
                    order) of received Ethernet frame
                   @DataPtr Pointer to payload of the received Ethernet frame (i.e. Ethernet
                    header is not provided).
                   @LenByte Length of received data
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    NA
 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void EthTSyn_RxIndication(
    uint8 CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8* PhysAddrPtr,
    const uint8* DataPtr,
    uint16 LenByte)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    StbM_TimeBaseStatusType timeBaseStatus;
    StbM_TimeBaseStatusType offsetTimeBaseStatus;

    SchM_Enter_EthTSyn_Context();
    boolean schMExited = FALSE;

    ETHTSYN_UNUSED(IsBroadcast);
    ETHTSYN_UNUSED(PhysAddrPtr);
#if ((ETHTSYN_CRC_SUPPORT == STD_OFF) || (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON))
    ETHTSYN_UNUSED(LenByte);
#endif
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateRxindication(CtrlIdx, DataPtr))
#endif
    {
        EthTSynPortType* port = EthTSyn_FindPort(CtrlIdx);
        if ((port != NULL_PTR) && (FrameType == 0x88F7u) && (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
            && (port->timedomainid == DataPtr[4]))
        {
            uint8 i = port->index;
            EthTSyn_TimeStampType TVnow;
            boolean isValidIngressTime = FALSE;
            if (EthTSyn_GetIngressTV(CtrlIdx, i, DataPtr, &TVnow) == E_OK)
            {
                isValidIngressTime = TRUE;
            }
            SchM_Exit_EthTSyn_Context();
            schMExited = TRUE;
            EthTSynMessageType EthTSynMessage = (DataPtr[0] & 0x0Fu);
            const uint8* masterPortIdentity = &DataPtr[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET];
            uint16 SequenceId = (((uint16)DataPtr[30]) << 8u) + DataPtr[31];
            const EthTSynPortConfigCfgType* portCfg = &PORT_TOCFGPORT(i);
            switch (EthTSynMessage)
            {
            case SYNC: {
                if (port->PortRoleType == ETHTSYN_SLAVE)
                {
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
                    if ((port->SyncStatusType == WAITFOR_FOLLOWUP)
                        /* PRQA S 3415 ++ */ /* MISRA Rule 13.5 */
                        && !EthTSyn_IsPortIdentityEqual(port->portIdentity, masterPortIdentity))
                    /* PRQA S 3415 -- */ /* MISRA Rule 13.5 */
                    {
                        (void)Det_ReportRuntimeError(
                            ETHTSYN_MODULE_ID,
                            ETHTSYN_INSTANCE_ID,
                            ETHTSYN_SID_RXINDICATION,
                            ETHTSYN_E_TSCONFLICT);
                    }
                    else
#endif
                    {
                        (void)StbM_GetTimeBaseStatus(
                            PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                            &timeBaseStatus,
                            &offsetTimeBaseStatus);

                        const uint16 sequenceCounterJumpWidth =
                            portCfg->EthTSynPortRole.EthTSynGlobalTimeSlave->EthTSynGlobalTimeSequenceCounterJumpWidth;
                        uint16 sequenceCounterJump = SequenceId - port->ReceivedSyncSequenceId;
                        if ((sequenceCounterJump > sequenceCounterJumpWidth) && (sequenceCounterJumpWidth != 0u)
                            && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u))
                        {
                            port->EthTSynSCHysteresisCount = 0u;
                        }

                        if (isValidIngressTime
                            && (((sequenceCounterJump > 0u) && (sequenceCounterJump <= sequenceCounterJumpWidth))
                                || (sequenceCounterJumpWidth == 0u) || !port->hasReceivedSyncReq
                                || ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)))
                        {
                            port->hasReceivedSyncReq = TRUE;
                            port->T1vlt = TVnow;
                            port->SyncStatusType = WAITFOR_FOLLOWUP;
                            (void)ILib_memcpy(port->portIdentity, masterPortIdentity, ETHTSYN_PORT_IDENTITY_LENGTH);
                            port->ReceivedSyncSequenceId = SequenceId;
                        }
                    }
                }
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
                else
                {
                    (void)Det_ReportRuntimeError(
                        ETHTSYN_MODULE_ID,
                        ETHTSYN_INSTANCE_ID,
                        ETHTSYN_SID_RXINDICATION,
                        ETHTSYN_E_TMCONFLICT);
                }
#endif
            }
            break;
            case FOLLOW_UP: {
                const EthTSynGlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr =
                    portCfg->EthTSynPortRole.EthTSynGlobalTimeSlave;
                if ((port->SyncStatusType == WAITFOR_FOLLOWUP)
                    /* PRQA S 3415 ++ */ /* MISRA Rule 13.5 */
                    && EthTSyn_IsPortIdentityEqual(port->portIdentity, masterPortIdentity)
                    /* PRQA S 3415 -- */ /* MISRA Rule 13.5 */
                    && (SequenceId == port->ReceivedSyncSequenceId) && (globalTimeSlaveCfgPtr != NULL_PTR))
                {
                    uint64 Timeout = globalTimeSlaveCfgPtr->EthTSynGlobalTimeFollowUpTimeout;
                    EthTSyn_TimeStampType td;
                    if (!isValidIngressTime)
                    {
                        port->SyncStatusType = WAITFOR_SYNC;
                        break;
                    }

                    EthTSyn_TimeStampSub(TVnow, port->T1vlt, &td);
                    if ((Timeout != 0u) && !EthTSyn_timeless(td, Timeout))
                    {
                        port->SyncStatusType = WAITFOR_SYNC;
                        break;
                    }

                    Std_ReturnType ret;
                    uint8 SGWByte = 0u;
                    uint8 OFSByte = 0u;
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
                    CrcValidatedType ValidatedType = globalTimeSlaveCfgPtr->EthTSynRxCrcValidated;
                    ret = EthTSyn_TlvScanAndCRCValidation(
                        i,
                        ValidatedType,
                        &DataPtr[0u],
                        &SGWByte,
                        &OFSByte,
                        LenByte,
                        globalTimeSlaveCfgPtr->EthTSynCrcFlagsRxValidated);
#else
                    ret = E_OK;
#endif
                    if ((ret == E_OK)
                        && (port->EthTSynSCHysteresisCount
                            <= globalTimeSlaveCfgPtr->EthTSynGlobalTimeSequenceCounterHysteresis))
                    {
                        port->EthTSynSCHysteresisCount++;
                    }
                    if ((ret == E_OK)
                        && (port->EthTSynSCHysteresisCount
                            > globalTimeSlaveCfgPtr->EthTSynGlobalTimeSequenceCounterHysteresis))
#else
                    if (port->EthTSynSCHysteresisCount
                        <= globalTimeSlaveCfgPtr->EthTSynGlobalTimeSequenceCounterHysteresis)
                    {

                        port->EthTSynSCHysteresisCount++;
                    }
                    if (port->EthTSynSCHysteresisCount
                        > globalTimeSlaveCfgPtr->EthTSynGlobalTimeSequenceCounterHysteresis)
#endif
                    {
                        port->SyncStatusType = WAITFOR_SYNC;
                        EthTSyn_TimeStampType T2vlt;
                        EthTSyn_TimeStampType T3vlt;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
                        /* SWS_EthTSyn_00179 */
                        StbM_VirtualLocalTimeType VirtualLocalTime;
                        ret = StbM_GetCurrentVirtualLocalTime(
                            PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                            &VirtualLocalTime);
                        if (E_OK == ret)
                        {
                            T2vlt = EthTSyn_VTToGT(VirtualLocalTime);
                            T3vlt.nanoseconds = T2vlt.nanoseconds;
                            T3vlt.seconds = T2vlt.seconds;
                            T3vlt.secondsHi = T2vlt.secondsHi;
                        }
                        else
                        {
                            break;
                        }
#else
                        Eth_TimeStampQualType timeQualPtr = ETH_INVALID;
                        Eth_TimeStampType timeStampPtr;
                        ret = EthIf_GetCurrentTime(CtrlIdx, &timeQualPtr, &timeStampPtr);
                        if ((E_OK == ret) && (ETH_VALID == timeQualPtr))
                        {
                            T3vlt.nanoseconds = timeStampPtr.nanoseconds;
                            T3vlt.seconds = timeStampPtr.seconds;
                            T3vlt.secondsHi = timeStampPtr.secondsHi;
                        }
                        else
                        {
                            break;
                        }

                        if (EthTSyn_StbmUseEthHW(i))
                        {
                            T2vlt.nanoseconds = T3vlt.nanoseconds;
                            T2vlt.seconds = T3vlt.seconds;
                            T2vlt.secondsHi = T3vlt.secondsHi;
                        }
                        else
                        {
                            StbM_VirtualLocalTimeType VirtualLocalTime;
                            ret = StbM_GetCurrentVirtualLocalTime(
                                PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                                &VirtualLocalTime);
                            if (ret == E_OK)
                            {
                                T2vlt = EthTSyn_VTToGT(VirtualLocalTime);
                            }
                            else
                            {
                                break;
                            }
                        }
#endif
                        timeBaseStatus = 0x0u;
                        StbM_TimeStampType globalTime;
                        EthTSyn_TimeStampType Syncreceptiondelay;
                        StbM_UserDataType userDataPtr;
                        if ((SGWByte != 0u) && (DataPtr[SGWByte] == 0x01u))
                        {
                            timeBaseStatus = STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
                        }
                        globalTime.timeBaseStatus = timeBaseStatus;
                        userDataPtr = port->userData;
                        EthTSyn_TimeStampSub(T3vlt, port->T1vlt, &Syncreceptiondelay);
                        EthTSyn_TimeStampType TG;
                        if (PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef < 16u)
                        {
                            TG = EthTSyn_DataToTime(&(DataPtr[34]));
                            EthTSyn_TimeStampType correction = EthTSyn_GetCorrectionField(&(DataPtr[8]));
                            TG = EthTSyn_timeadd(
                                EthTSyn_timeadd(EthTSyn_timeadd(TG, port->pdelay), Syncreceptiondelay),
                                correction);
                            EthTSyn_timeoptimal(&TG);
                            globalTime.nanoseconds = TG.nanoseconds;
                            globalTime.seconds = TG.seconds;
                            globalTime.secondsHi = TG.secondsHi;
                            StbM_MeasurementType measureDataPtr;
                            measureDataPtr.pathDelay = port->pdelay.nanoseconds;
                            StbM_VirtualLocalTimeType localTimePtr;
                            uint64 TV = T2vlt.nanoseconds
                                        + (((uint64)T2vlt.seconds + (((uint64)T2vlt.secondsHi) << 32))
                                           * ETHTSYN_NANOSECOND_TO_SECOND);
                            localTimePtr.nanosecondsHi = (uint32)(TV >> 32u);
                            localTimePtr.nanosecondsLo = (uint32)TV;
                            (void)StbM_BusSetGlobalTime(
                                PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                                &globalTime,
                                &userDataPtr,
                                &measureDataPtr,
                                &localTimePtr);
                        }
                        else if (OFSByte != 0u)
                        {
                            TG = EthTSyn_DataToTime(&(DataPtr[OFSByte]));
                            globalTime.nanoseconds = TG.nanoseconds;
                            globalTime.seconds = TG.seconds;
                            globalTime.secondsHi = TG.secondsHi;
                            (void)StbM_SetOffset(
                                PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                                &globalTime,
                                &userDataPtr);
                        }
                        else
                        {
                            /* Intentionally Empty */
                        }
                    }
                }
                else
                {
                    /* Intentionally Empty */
                }
            }
            break;
            case PDELAY_REQ:
                if (isValidIngressTime)
                {
                    port->pdelayResponderTimestamp.pdelayReqEventIngressTimestamp = TVnow;
                    port->PdelayRXStatusType = SEND_PDELAY_RESP;
                    port->ReceivedPdelayReqSequenceId = SequenceId;
                    (void)ILib_memcpy(&EthTSyn_ReceivedPdelayReqSourcePortId[i][0], &DataPtr[20], 10u);
                }
                break;
            case PDELAY_RESP: {
                if (port->PdelayTXStatusType != WAITFOR_PDELAY_RESP)
                {
                    break;
                }

                /* CODE_EthTSyn_00001 */
                const uint8* requestingPortIdentity = &DataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
                if (!isValidIngressTime
                    || (SequenceId != port->PdelayReqSequenceId)
                    /* PRQA S 3415 ++ */ /* MISRA Rule 13.5 */
                    || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, port->thisPortIdentity))
                /* PRQA S 3415 -- */ /* MISRA Rule 13.5 */
                {
                    port->PdelayTXStatusType = SEND_PDELAY_REQ;
                    break;
                }

                uint64 Timeout = portCfg->EthTSynPdelayConfig.EthTSynPdelayRespAndRespFollowUpTimeout;
                EthTSyn_TimeStampType td;
                EthTSyn_TimeStampSub(TVnow, port->t1, &td);
                if ((Timeout != 0u) && !EthTSyn_timeless(td, Timeout))
                {
                    port->PdelayTXStatusType = SEND_PDELAY_REQ;
                    break;
                }

                port->t2 = EthTSyn_DataToTime(&(DataPtr[34]));
                port->t4 = TVnow;
                port->PdelayTXStatusType = WAITFOR_PDELAY_RESP_FOLLOWUP;
            }
            break;
            case PDELAY_RESP_FOLLOW_UP: {
                if (port->PdelayTXStatusType != WAITFOR_PDELAY_RESP_FOLLOWUP)
                {
                    break;
                }

                port->PdelayTXStatusType = SEND_PDELAY_REQ;

                /* CODE_EthTSyn_00002 */
                const uint8* requestingPortIdentity = &DataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
                if (!isValidIngressTime
                    || (SequenceId != port->PdelayReqSequenceId)
                    /* PRQA S 3415 ++ */ /* MISRA Rule 13.5 */
                    || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, port->thisPortIdentity))
                /* PRQA S 3415 -- */ /* MISRA Rule 13.5 */
                {
                    break;
                }

                uint64 Timeout = portCfg->EthTSynPdelayConfig.EthTSynPdelayRespAndRespFollowUpTimeout;
                EthTSyn_TimeStampType td;
                EthTSyn_TimeStampSub(TVnow, port->t4, &td);
                if ((Timeout != 0u) && !EthTSyn_timeless(td, Timeout))
                {
                    break;
                }

                port->t3 = EthTSyn_DataToTime(&(DataPtr[34]));
                /* pdelay = ((t4-t1)-(t3-t2))/2 */
                EthTSyn_TimeStampType td41;
                EthTSyn_TimeStampSub(port->t4, port->t1, &td41);

                EthTSyn_TimeStampType td32;
                EthTSyn_TimeStampSub(port->t3, port->t2, &td32);

                if (EthTSyn_TimeStampCmp(td41, td32) >= 0)
                {
                    EthTSyn_TimeStampType pdelay;
                    EthTSyn_TimeStampSub(td41, td32, &pdelay);
                    pdelay.nanoseconds /= 2u;
                    uint32 s = pdelay.seconds;
                    pdelay.seconds /= 2u;
                    if ((s % 2u) > 0u)
                    {
                        pdelay.nanoseconds += ETHTSYN_NANOSECOND_TO_SECOND / 2u;
                        if (pdelay.nanoseconds >= ETHTSYN_NANOSECOND_TO_SECOND)
                        {
                            pdelay.nanoseconds -= ETHTSYN_NANOSECOND_TO_SECOND;
                            pdelay.seconds++;
                        }
                    }

                    uint64 PdelayThreshold = portCfg->EthTSynPdelayConfig.EthTSynPdelayLatencyThreshold;
                    if ((PdelayThreshold == 0u) || EthTSyn_timeless(pdelay, PdelayThreshold))
                    {
                        port->pdelay = pdelay;
                    }
                }
            }
            break;
            default:
                /* do nothing */
                break;
            }
        }
    }
    if (!schMExited)
    {
        SchM_Exit_EthTSyn_Context();
    }
}
#define ETHTSYN_STOP_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#include "EthTSyn_MemMap.h"
#define ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         Confirms the transmission of an Ethernet frame
 * @id            0x07
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant Dont care
 * @param[in]     CtrlIdx  Index of the Ethernet controller within the context of the Ethernet Interface
 * @param[in]     BufIdx   Index of the buffer resource
 * @retval        E_OK     The transmission was successful
 * @retval        E_NOT_OK The transmission failed
 */
/* clang-format off */
void EthTSyn_TxConfirmation(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Std_ReturnType Result) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
/* clang-format on */
{
    ETHTSYN_UNUSED(Result);

    StbM_VirtualLocalTimeType VirtualLocalTime;

    SchM_Enter_EthTSyn_Context();
    boolean schMExited = FALSE;
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_TXCONFIRMATION, CtrlIdx))
#endif
    {
        EthTSynPortType* port = EthTSyn_FindPort(CtrlIdx);
        if (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
        {
            uint8 i = port->index;
            EthTSyn_TimeStampType t2vlt = {0u, 0u, 0u};
            boolean isValidEgressTime = FALSE;
            if (EthTSyn_GetEgressTV(CtrlIdx, i, &t2vlt, BufIdx) == E_OK)
            {
                isValidEgressTime = TRUE;
            }
            SchM_Exit_EthTSyn_Context();
            schMExited = TRUE;
            boolean isValidVT = FALSE;
            if (StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef, &VirtualLocalTime)
                == E_OK)
            {
                isValidVT = TRUE;
            }
            uint64 timenow = (((uint64)VirtualLocalTime.nanosecondsHi) << 32u) + VirtualLocalTime.nanosecondsLo;
            if (!isValidVT || ((timenow - port->Txconfirmation) >= ETHTSYN_TX_CONFIRMATION))
            {
                switch (port->TranssmitMessageType)
                {
                case SYNC:
                case FOLLOW_UP:
                    port->SyncStatusType = SEND_SYNC;
                    break;
                case PDELAY_REQ:
                case PDELAY_RESP:
                case PDELAY_RESP_FOLLOW_UP:
                    port->PdelayTXStatusType = SEND_PDELAY_REQ;
                    break;
                default:
                    /* do nothing */
                    break;
                }
            }
            else
            {
                switch (port->TranssmitMessageType)
                {
                case SYNC: {
                    EthTSyn_TimeStampType T0 = port->SyncEgressTuple.t0;
                    EthTSyn_TimeStampType T0vlt = port->SyncEgressTuple.t0vlt;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
                    /* SWS_EthTSyn_00189 */
                    EthTSyn_TimeStampType T4vlt;
                    if (StbM_GetCurrentVirtualLocalTime(
                            PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                            &VirtualLocalTime)
                        == E_OK)
                    {
                        T4vlt = EthTSyn_VTToGT(VirtualLocalTime);
                        EthTSyn_TimeStampType td;
                        EthTSyn_TimeStampSub(T4vlt, T0vlt, &td); /* preciseOriginTimestamp = T0 + t4vlt - t0vlt */
                        port->preciseOriginTimestamp = EthTSyn_timeadd(td, T0);
                    }
                    else
                    {
                        port->SyncStatusType = SEND_SYNC;
                        break;
                    }
#else
                    if (!isValidEgressTime)
                    {
                        port->SyncStatusType = SEND_SYNC;
                        break;
                    }

                    if (EthTSyn_StbmUseEthHW(i))
                    {
                        /* SWS_EthTSyn_00188 */
                        EthTSyn_TimeStampType td;
                        EthTSyn_TimeStampSub(t2vlt, T0vlt, &td);
                        port->preciseOriginTimestamp =
                            EthTSyn_timeadd(td, T0); /* preciseOriginTimestamp = T0 + t2vlt - t0vlt */
                    }
                    else
                    {
                        Std_ReturnType ret;

                        /* SWS_EthTSyn_00017 */
                        EthTSyn_TimeStampType T3vlt = {0u, 0u, 0u};
                        EthTSyn_TimeStampType T4vlt;
                        Eth_TimeStampQualType timeQual = ETH_INVALID;
                        Eth_TimeStampType timeStamp;
                        ret = EthIf_GetCurrentTime(CtrlIdx, &timeQual, &timeStamp);
                        if (!((ret == E_OK) && (timeQual == ETH_VALID)))
                        {
                            port->SyncStatusType = SEND_SYNC;
                            break;
                        }

                        T3vlt.nanoseconds = timeStamp.nanoseconds;
                        T3vlt.seconds = timeStamp.seconds;
                        T3vlt.secondsHi = timeStamp.secondsHi;

                        if (StbM_GetCurrentVirtualLocalTime(
                                PORT_TOCFGDOMIN(i).EthTSynSynchronizedTimeBaseRef,
                                &VirtualLocalTime)
                            != E_OK)
                        {
                            port->SyncStatusType = SEND_SYNC;
                            break;
                        }

                        T4vlt = EthTSyn_VTToGT(VirtualLocalTime);
                        EthTSyn_TimeStampType td32vlt;
                        EthTSyn_TimeStampSub(T3vlt, t2vlt, &td32vlt); /* td32vlt = t3vlt - t2vlt */
                        EthTSyn_TimeStampType td40vlt;
                        EthTSyn_TimeStampSub(T4vlt, T0vlt, &td40vlt); /* td40vlt = t4vlt - t0vlt */
                        if (EthTSyn_TimeStampCmp(td40vlt, td32vlt) < 0)
                        {
                            port->SyncStatusType = SEND_SYNC;
                            break;
                        }

                        EthTSyn_TimeStampType tdTemp;
                        EthTSyn_TimeStampSub(td40vlt, td32vlt, &tdTemp);
                        /** [SWS_EthTSyn_00017]
                         *  preciseOriginTimestamp = T0 - (T3VLT - T2VLT) + (T4VLT - T0VLT)
                         */
                        port->preciseOriginTimestamp = EthTSyn_timeadd(tdTemp, T0);
                    }
#endif
                    port->SyncStatusType = SEND_FOLLOWUP;
                }
                break;
                case PDELAY_REQ: {
                    if (isValidEgressTime)
                    {
                        port->PdelayTXStatusType = WAITFOR_PDELAY_RESP;
                        port->t1 = t2vlt;
                    }
                    else
                    {
                        port->PdelayTXStatusType = SEND_PDELAY_REQ;
                    }
                }
                break;
                case PDELAY_RESP: {
                    if (isValidEgressTime)
                    {
                        port->pdelayResponderTimestamp.pdelayRespEventEgressTimestamp = t2vlt;
                        port->PdelayRXStatusType = SEND_PDELAY_RESP_FOLLOWUP;
                    }
                    else
                    {
                        port->PdelayTXStatusType = SEND_PDELAY_REQ;
                    }
                }
                break;
                case PDELAY_RESP_FOLLOW_UP: {
                    port->PdelayRXStatusType = WAITFOR_PDELAY_REQ;
                }
                break;
                default:
                    /* do nothing */
                    break;
                }
            }
        }
    }

    if (!schMExited)
    {
        SchM_Exit_EthTSyn_Context();
    }
}
#define ETHTSYN_STOP_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthTSyn_MemMap.h"
/**
 * Allows resetting state machine in case of unexpected Link loss to avoid
   inconsistent Sync and Follow_Up sequences
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @CtrlIdx Index of the Ethernet controller
                    @TrcvLinkState ETHTRCV_LINK_STATE_DOWN ETHTRCV_LINK_STATE_ACTIVE
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType EthTSyn_TrcvLinkStateChg(
    uint8 CtrlIdx,
    EthTrcv_LinkStateType TrcvLinkState) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateInitStatus(ETHTSYN_SID_TRCVLINKSTATECHG))
#endif
    {
        EthTSynPortType* port = EthTSyn_FindPort(CtrlIdx);
        if (port != NULL_PTR)
        {
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) && (TrcvLinkState == ETHTRCV_LINK_STATE_DOWN))
            {
                /*[SWS_EthTSyn_00019]*/
                EthTSyn_InitPortStatus(port);
            }
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_DOWN) && (TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE))
            {
                /*[SWS_EthTSyn_00020]*/
                port->EthtrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
            }
            ret = E_OK;
#if (ETHTSYN_DEST_PHY_ADDR_MULTICAST == STD_ON)
            Eth_FilterActionType filterAction =
                (TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER;
            (void)EthIf_UpdatePhysAddrFilter(CtrlIdx, EthTSynDestPhyAddr, filterAction);
#endif
        }
        return ret;
    }
}
#define ETHTSYN_STOP_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * Main function for cyclic call / resp. Sync, Follow_Up and Pdelay_Req transmissions
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
void EthTSyn_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if (EthTSyn_Initialized)
    {
        uint8 i;
        for (i = 0u; i < ETHTSYN_ETH_PORT_NUM; i++)
        {
            EthTSynMessageType TranssmitMsgType = MSG_NONE;
            const EthTSynPortConfigCfgType* portCfg = &PORT_TOCFGPORT(i);
            EthTSynPortType* port = &EthTSyn_Port[i];
            if (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
            {
                if (ETHTSYN_MASTER == port->PortRoleType)
                {
                    TranssmitMsgType = EthTSyn_MainFunctionMaster(i);
                }
                else
                {
                    uint64 TxPdelayReqPeriod = portCfg->EthTSynPdelayConfig.EthTSynGlobalTimeTxPdelayReqPeriod;
                    if (TxPdelayReqPeriod != 0u)
                    {
                        if (port->PdelayReqPeriod <= ETHTSYN_MAIN_FUNCTION_PERIOD)
                        {
                            TranssmitMsgType = PDELAY_REQ;
                            port->PdelayReqPeriod = TxPdelayReqPeriod;
                        }
                        else
                        {
                            port->PdelayReqPeriod -= ETHTSYN_MAIN_FUNCTION_PERIOD;
                        }
                    }
                }

                uint64 debounceCounter = portCfg->EthTSynGlobalTimeDebounceTime * ETHTSYN_MAIN_FUNCTION_PERIOD;
                if (port->PdelayRXStatusType == SEND_PDELAY_RESP)
                {
                    if (portCfg->EthTSynPdelayConfig.EthTSynGlobalTimePdelayRespEnable)
                    {
                        if (TranssmitMsgType == MSG_NONE)
                        {
                            TranssmitMsgType = PDELAY_RESP;
                            port->PdelaydebounceCounter = debounceCounter;
                        }
                    }
                }
                else if (port->PdelayRXStatusType == SEND_PDELAY_RESP_FOLLOWUP)
                {
                    if (port->PdelaydebounceCounter <= ETHTSYN_MAIN_FUNCTION_PERIOD)
                    {
                        if (TranssmitMsgType == MSG_NONE)
                        {
                            TranssmitMsgType = PDELAY_RESP_FOLLOW_UP;
                            port->PdelaydebounceCounter = debounceCounter;
                        }
                    }
                    else
                    {
                        port->PdelaydebounceCounter -= ETHTSYN_MAIN_FUNCTION_PERIOD;
                    }
                }
                else
                {
                    ; /* do nothing */
                }

                if (TranssmitMsgType != MSG_NONE)
                {
                    EthTSyn_TranssmitMessage(TranssmitMsgType, i);
                }
            }
        }
    }
}

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"

static Std_ReturnType EthTSyn_GetIngressTV(uint8 CtrlIdx, uint8 PortId, const uint8* DataPtr, EthTSyn_TimeStampType* TV)
{
    Std_ReturnType ret;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    ETHTSYN_UNUSED(CtrlIdx);
    ETHTSYN_UNUSED(DataPtr);
    StbM_VirtualLocalTimeType VirtualLocalTime;
    ret = StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef, &VirtualLocalTime);
    if (ret == E_OK)
    {
        *TV = EthTSyn_VTToGT(VirtualLocalTime);
    }
#else
    ETHTSYN_UNUSED(PortId);
    Eth_TimeStampQualType timeQualPtr = ETH_INVALID;
    Eth_TimeStampType timeStampPtr;
    ret = EthIf_GetIngressTimeStamp(CtrlIdx, (const Eth_DataType*)DataPtr, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        TV->nanoseconds = timeStampPtr.nanoseconds;
        TV->seconds = timeStampPtr.seconds;
        TV->secondsHi = timeStampPtr.secondsHi;
    }
    else
    {
        ret = E_NOT_OK;
    }
#endif
    return ret;
}

static Std_ReturnType EthTSyn_GetEgressTV(uint8 CtrlIdx, uint8 PortId, EthTSyn_TimeStampType* TV, Eth_BufIdxType BufIdx)
{
    Std_ReturnType ret;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    StbM_VirtualLocalTimeType VirtualLocalTime;
    ETHTSYN_UNUSED(BufIdx);
    ETHTSYN_UNUSED(CtrlIdx);

    ret = StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef, &VirtualLocalTime);
    if (ret == E_OK)
    {
        *TV = EthTSyn_VTToGT(VirtualLocalTime);
    }
#else
    ETHTSYN_UNUSED(PortId);
    Eth_TimeStampQualType timeQualPtr = ETH_INVALID;
    Eth_TimeStampType timeStampPtr;
    ret = EthIf_GetEgressTimeStamp(CtrlIdx, BufIdx, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        TV->nanoseconds = timeStampPtr.nanoseconds;
        TV->seconds = timeStampPtr.seconds;
        TV->secondsHi = timeStampPtr.secondsHi;
    }
    else
    {
        ret = E_NOT_OK;
    }
#endif
    return ret;
}

static EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* Data)
{
    EthTSyn_TimeStampType t;
    t.nanoseconds = ((uint32)Data[6] << 24u) + ((uint32)Data[7] << 16u) + ((uint32)Data[8] << 8u) + (uint32)Data[9];
    t.seconds = ((uint32)Data[2] << 24u) + ((uint32)Data[3] << 16u) + ((uint32)Data[4] << 8u) + (uint32)Data[5];
    t.secondsHi = ((uint16)Data[0] << 8u) + (uint16)Data[1];
    return t;
}
static EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType VirtualLocalTime)
{
    EthTSyn_TimeStampType t;
    uint64 Vt = (((uint64)VirtualLocalTime.nanosecondsHi) << 32u) + VirtualLocalTime.nanosecondsLo;
    t.nanoseconds = (uint32)(Vt % ETHTSYN_NANOSECOND_TO_SECOND);
    Vt /= ETHTSYN_NANOSECOND_TO_SECOND;
    t.seconds = (uint32)Vt;
    t.secondsHi = (uint16)(Vt >> 32u);
    return t;
}

static void EthTSyn_TimeToData(EthTSyn_TimeStampType Time, uint8* Data)
{
    uint64 second = (((uint64)Time.secondsHi) << 32u) + Time.seconds;
    uint8 i;
    for (i = 0u; i < 6u; i++)
    {
        Data[i] = (uint8)(((second << (i * 8u))) >> 40u);
    }
    for (i = 6u; i < 10u; i++)
    {
        Data[i] = (uint8)(((Time.nanoseconds << ((i - 6u) * 8u))) >> 24u);
    }
}
static void EthTSyn_TranssmitMessage(EthTSynMessageType EthTSynMessage, uint16 PortId)
{
    Eth_BufIdxType BufId = 0u;
    uint8 i;
    uint8 Priority;
    uint8* DataBuf = NULL_PTR;
    uint8 Data[200];
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
    uint8 crcdata[17];
#endif
    uint16 Length;
    uint16 DataLen = 0u;
    Eth_FrameType FrameType = 0x88F7u;
    EthTSynPortType* port = &EthTSyn_Port[PortId];
    boolean hasCapturedEgress = FALSE;

    if (port->EthTSyn_TransmissionMode == ETHTSYN_TX_ON)
    {
        uint8 CtrlIdx = port->CtrlId;
        /* SWS_EthTSyn_00162 */
        Priority = port->EthTSynFramePrio;
        EthTSynGlobalTimeMasterCfgType MasterCfg = {0};
        if (PORT_TOCFGPORT(PortId).EthTSynPortRole.EthTSynGlobalTimeMaster != NULL_PTR)
        {
            MasterCfg = *(PORT_TOCFGPORT(PortId).EthTSynPortRole.EthTSynGlobalTimeMaster);
        }
        else
        {
            MasterCfg.EthTSynCorrectionField = 0u;
        }

        /*transportSpecific and messageType */
        Data[0] = (uint8)((uint8)(0x01u << 4u) | ((uint8)EthTSynMessage));
        /*versionPTP */
        Data[1] = 0x02u;
        /*domainNumber */
        Data[4] = port->timedomainid;
        /*reserved */
        Data[5] = 0x00u;
        /*CorrectionField*/
        EthTSyn_CreatCorrectionField(MasterCfg.EthTSynCorrectionField, &Data[8u]);
        /*reserved */
        Data[16] = 0x00u;
        Data[17] = 0x00u;
        Data[18] = 0x00u;
        Data[19] = 0x00u;

        /* sourcePortIdentity */
        (void)ILib_memcpy(
            &Data[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET],
            port->thisPortIdentity,
            ETHTSYN_PORT_IDENTITY_LENGTH);

        /*reserved */
        for (i = 0u; i < 8u; i++)
        {
            Data[46u + i] = 0x00u;
        }
        switch (EthTSynMessage)
        {
        case SYNC: {
            /* SWS_EthTSyn_00202 */
            StbM_TimeStampType globalTime;
            StbM_VirtualLocalTimeType localTime;
            StbM_UserDataType userData;
            EthTSyn_TimeStampType T0 = {0u, 0u, 0u};
            Std_ReturnType ret = StbM_BusGetCurrentTime(
                PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef,
                &globalTime,
                &localTime,
                &userData);
            if (ret == E_OK)
            {
                T0.nanoseconds = globalTime.nanoseconds;
                T0.seconds = globalTime.seconds;
                T0.secondsHi = globalTime.secondsHi;
                EthTSyn_TimeStampType T0vlt = {0u, 0u, 0u};
                T0vlt = EthTSyn_VTToGT(localTime);
                port->SyncEgressTuple.t0vlt = T0vlt;
                port->SyncEgressTuple.t0 = T0;
                hasCapturedEgress = TRUE;
                /*messageLength */
                Data[2] = 0u;
                Data[3] = 44U;
                /*flags Octet 0*/
                Data[6] = 0x02u;
                Data[7] = 0x08u;
                /*sequenceId*/
                if (port->SyncSequenceId < 0xFFFFu)
                {
                    port->SyncSequenceId++;
                }
                else
                {
                    port->SyncSequenceId = 0u;
                }
                Data[30] = (uint8)(port->SyncSequenceId >> 8u);
                Data[31] = (uint8)port->SyncSequenceId;
                /*control*/
                Data[32] = 0x0u;
                /*logMessageInterval in seconds*/
                Data[33] = (uint8)MasterCfg.logMessageInterval;
                /*reserved */
                for (i = 0; i < 10u; i++)
                {
                    Data[34u + i] = 0x00u;
                }
                DataLen = 44U;
            }
            break;
        }
        case FOLLOW_UP:
            /*messageLength */
            Data[2] = 0u;
            Data[3] = 76U;
            /*flags Octet 0*/
            Data[6] = 0x00u;
            Data[7] = 0x08u;
            /*sequenceId*/
            Data[30] = (uint8)(port->SyncSequenceId >> 8u);
            Data[31] = (uint8)port->SyncSequenceId;
            /*control*/
            Data[32] = 0x02u;
            /*logMessageInterval in seconds*/
            Data[33] = (uint8)MasterCfg.logMessageInterval;
            /*preciseOriginTimestamp */
            EthTSyn_TimeToData(port->preciseOriginTimestamp, &Data[34]);
            /*IEEE Follow_Up information TLV */
            /*tlvType */
            Data[44] = 0x00u;
            Data[45] = 0x03u;
            /*lengthField*/
            Data[46] = 0x00u;
            Data[47] = 28u;
            /*organizationId*/
            Data[48] = 0x00u;
            Data[49] = 0x80u;
            Data[50] = 0xC2u;
            /*organizationSubType*/
            Data[51] = 0x00u;
            Data[52] = 0x00u;
            Data[53] = 0x01u;
            /* cumulativeScaledRateOffset*/
            Data[54] = (uint8)(MasterCfg.EthTSynCumulativeScaledRateOffset >> 24u);
            Data[55] = (uint8)(MasterCfg.EthTSynCumulativeScaledRateOffset >> 16u);
            Data[56] = (uint8)(MasterCfg.EthTSynCumulativeScaledRateOffset >> 8u);
            Data[57] = (uint8)(MasterCfg.EthTSynCumulativeScaledRateOffset);
            /* gmTimeBaseIndicator*/
            /* lastGmPhaseChange */
            /* scaledLastGmFreqChange*/
            for (i = 0u; i < 18u; i++)
            {
                Data[58u + i] = 0x00u;
            }
            DataLen = 76U;
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
            /* Autosar Follow_Up information TLV */
            /*tlvType */
            Data[76] = 0x00u;
            Data[77] = 0x03u;
            /*lengthField*/
            Data[78] = 0x00u;
            Data[79] = 6u;
            /*organizationId*/
            Data[80] = 0x1Au;
            Data[81] = 0x75u;
            Data[82] = 0xFBu;
            /*organizationSubType*/
            Data[83] = 0x60u;
            Data[84] = 0x56u;
            Data[85] = 0x76u;
            Data[3] += 10u;
            DataLen += 10U;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
            uint8 DataID = PORT_TOCFGDOMIN(PortId)
                               .EthTSynGlobalTimeFollowUpDataIDList
                               ->EthTSynGlobalTimeFollowUpDataIDListElement[(port->SyncSequenceId) % 16u]
                               .EthTSynGlobalTimeFollowUpDataIDListValue;
#endif
            /* Sub-TLV: Time Secured */
            if (MasterCfg.EthTSynTLVFollowUpTimeSubTLV)
            {
                Data[79] += 5U;
                /* Type */
                Data[DataLen] = 0x28u;
                /* Length */
                Data[DataLen + 1u] = 0x03u;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
                if ((MasterCfg.EthTSynCrcTimeFlagsTxSecured != NULL_PTR)
                    && (MasterCfg.EthTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED))
                {
                    /* CRC_Time_Flags */
                    Data[DataLen + 2u] = 0u;
                    EthTSynCrcTimeFlagsTxSecuredCfgType CrcTimeFlags = *(MasterCfg.EthTSynCrcTimeFlagsTxSecured);
                    uint8 CrcData0[24u] = {0};
                    uint8 CrcData1[16u] = {0};
                    uint8 CrcData0length = 1u;
                    uint8 CrcData1length = 1u;
                    if (CrcTimeFlags.EthTSynCrcMessageLength)
                    {
                        Data[DataLen + 2u] += 0x01u;
                        CrcData1[CrcData1length] = Data[3];
                        CrcData1length++;
                    }
                    if (CrcTimeFlags.EthTSynCrcDomainNumber)
                    {
                        Data[DataLen + 2u] += 0x02u;
                        CrcData0[CrcData0length] = Data[4];
                        CrcData0length++;
                    }
                    if (CrcTimeFlags.EthTSynCrcCorrectionField)
                    {
                        Data[DataLen + 2u] += 0x04u;
                        (void)ILib_memcpy(&CrcData1[CrcData0length], &Data[8], 8u);
                        CrcData1length += 8u;
                    }
                    if (CrcTimeFlags.EthTSynCrcSourcePortIdentity)
                    {
                        Data[DataLen + 2u] += 0x08u;
                        (void)ILib_memcpy(&CrcData0[CrcData0length], &Data[20], 10u);
                        CrcData0length += 10u;
                    }
                    if (CrcTimeFlags.EthTSynCrcSequenceId)
                    {
                        Data[DataLen + 2u] += 0x10u;
                        CrcData1[CrcData1length] = Data[30];
                        CrcData1[CrcData1length + 1u] = Data[31];
                        CrcData1length += 2u;
                    }
                    if (CrcTimeFlags.EthTSynCrcPreciseOriginTimestamp)
                    {
                        Data[DataLen + 2u] += 0x20u;
                        (void)ILib_memcpy(&CrcData0[CrcData0length], &Data[34], 10u);
                        CrcData0length += 10u;
                    }
                    CrcData0[0] = Data[DataLen + 2u];
                    CrcData1[0] = Data[DataLen + 2u];
                    CrcData0[CrcData0length] = DataID;
                    CrcData1[CrcData0length] = DataID;
                    /* CRC_Time_0 */
                    Data[DataLen + 3u] = Crc_CalculateCRC8H2F(&CrcData0[0], (uint32)CrcData0length + 1u, 0xFF, TRUE);
                    /* CRC_Time_1 */
                    Data[DataLen + 4u] = Crc_CalculateCRC8H2F(&CrcData1[0], (uint32)CrcData1length + 1u, 0xFF, TRUE);
                }
                else
#endif
                {
                    Data[DataLen + 2u] = 0x00u;
                    Data[DataLen + 3u] = 0x00u;
                    Data[DataLen + 4u] = 0x00u;
                }
                Data[3] += 5u;
                DataLen += 5U;
            }
            /* Sub-TLV: Status */
            if (MasterCfg.EthTSynTLVFollowUpStatusSubTLV)
            {
                Data[79] += 4u;
                /* Length */
                Data[DataLen + 1u] = 0x02u;
                /* Status */
                if (EthTSyn_StatusGet(PortId, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
                {
                    Data[DataLen + 2u] = 0x01u;
                }
                else
                {
                    Data[DataLen + 2u] = 0x00u;
                }
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
                if ((MasterCfg.EthTSynCrcTimeFlagsTxSecured != NULL_PTR)
                    && (MasterCfg.EthTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED))
                {
                    /* Type */
                    Data[DataLen] = 0x50u;
                    crcdata[0] = Data[DataLen + 2u];
                    crcdata[1] = DataID;
                    Data[DataLen + 3u] = Crc_CalculateCRC8H2F(&crcdata[0], 2, 0xFF, TRUE);
                }
                else
#endif
                {
                    /* Type */
                    Data[DataLen] = 0x51u;
                    Data[DataLen + 3u] = 0x00u;
                }
                Data[3] += 4u;
                DataLen += 4U;
            }
            /* Sub-TLV: UserData Secured */
            if (MasterCfg.EthTSynTLVFollowUpUserDataSubTLV)
            {
                /* UserData */
                StbM_UserDataType userData = port->userData;

                if (userData.userDataLength > 0u)
                {
                    Data[79] += 7u;
                    /* Length */
                    Data[DataLen + 1u] = 0x05u;
                    Data[DataLen + 2u] = userData.userDataLength;
                    Data[DataLen + 3u] = userData.userByte0;
                    Data[DataLen + 4u] = userData.userByte1;
                    Data[DataLen + 5u] = userData.userByte2;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
                    if ((MasterCfg.EthTSynCrcTimeFlagsTxSecured != NULL_PTR)
                        && (MasterCfg.EthTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED))
                    {
                        /* Type */
                        Data[DataLen] = 0x60u;
                        crcdata[4] = DataID;
                        (void)ILib_memcpy(&crcdata[0], &Data[DataLen + 2u], 4u);
                        Data[DataLen + 6u] = Crc_CalculateCRC8H2F(&crcdata[0], 5, 0xFF, TRUE);
                    }
                    else
#endif
                    {
                        /* Type */
                        Data[DataLen] = 0x61u;
                        Data[DataLen + 6u] = 0x00u;
                    }
                    Data[3] += 7u;
                    DataLen += 7U;
                }
            }
            /* Sub-TLV: OFS */
            if (MasterCfg.EthTSynTLVFollowUpOFSSubTLV)
            {
                Data[79] += 19u;
                /* Length */
                Data[DataLen + 1u] = 0x11u;
                /* OfsTimeDomain */
                Data[DataLen + 2u] = PORT_TOCFGDOMIN(PortId).EthTSynGlobalTimeDomainId;
                /* OFS */
                StbM_TimeStampType timeStamp;
                StbM_UserDataType userData;
                (void)StbM_GetOffset(PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef, &timeStamp, &userData);
                port->userData = userData;
                EthTSyn_TimeStampType ttime;
                ttime.nanoseconds = timeStamp.nanoseconds;
                ttime.seconds = timeStamp.seconds;
                ttime.secondsHi = timeStamp.secondsHi;
                EthTSyn_TimeToData(ttime, &Data[DataLen + 3u]);
                if ((timeStamp.timeBaseStatus & STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY) > 0u)
                {
                    Data[DataLen + 13u] = 0x01u;
                }
                else
                {
                    Data[DataLen + 13u] = 0x00u;
                }
                Data[DataLen + 14u] = userData.userDataLength;
                Data[DataLen + 15u] = userData.userByte0;
                Data[DataLen + 16u] = userData.userByte1;
                Data[DataLen + 17u] = userData.userByte2;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
                if ((MasterCfg.EthTSynCrcTimeFlagsTxSecured != NULL_PTR)
                    && (MasterCfg.EthTSynGlobalTimeTxCrcSecured == CRC_SUPPORTED))
                {
                    /* Type */
                    Data[DataLen] = 0x44u;
                    crcdata[16] = DataID;
                    (void)ILib_memcpy(&crcdata[0], &Data[DataLen + 2u], 16u);
                    Data[DataLen + 18u] = Crc_CalculateCRC8H2F(&crcdata[0], 17, 0xFF, TRUE);
                }
                else
#endif
                {
                    /* Type */
                    Data[DataLen] = 0x34u;
                    Data[DataLen + 18u] = 0x00u;
                }
                Data[3] += 19u;
                DataLen += 19U;
            }

            if (Data[79] == 6u)
            {
                for (i = 0u; i < 10u; i++)
                {
                    Data[76u + i] = 0x00u;
                }
                Data[3] -= 10u;
                DataLen -= 10U;
            }
#endif
            break;
        case PDELAY_REQ:
            /*messageLength */
            Data[2] = 0u;
            Data[3] = 54u;
            /*flags Octet 0*/
            Data[6] = 0x00u;
            Data[7] = 0x08u;
            /*sequenceId*/
            if (port->PdelayReqSequenceId < 0xFFFFu)
            {
                port->PdelayReqSequenceId++;
            }
            else
            {
                port->PdelayReqSequenceId = 0u;
            }
            Data[30] = (uint8)(port->PdelayReqSequenceId >> 8u);
            Data[31] = (uint8)port->PdelayReqSequenceId;
            /*control*/
            Data[32] = 0x5u;
            /*logMessageInterval in seconds*/
            Data[33] = (uint8)PORT_TOCFGPORT(PortId).EthTSynPdelayConfig.logMessageInterval;
            /*reserved */
            for (i = 0u; i < 20u; i++)
            {
                Data[34u + i] = 0x00u;
            }
            DataLen = 54u;
            break;
        case PDELAY_RESP:
            /*messageLength */
            Data[2] = 0u;
            Data[3] = 54u;
            /*flags Octet 0*/
            Data[6] = 0x02u;
            Data[7] = 0x08u;
            /*sequenceId*/
            Data[30] = (uint8)(port->ReceivedPdelayReqSequenceId >> 8u);
            Data[31] = (uint8)port->ReceivedPdelayReqSequenceId;
            /* control */
            Data[32] = 0x5u;
            /*logMessageInterval*/
            Data[33] = 0x7F;
            /*requestReceiptTimestamp*/
            EthTSyn_TimeToData(port->pdelayResponderTimestamp.pdelayReqEventIngressTimestamp, &Data[34]);
            /*requestingPortIdentity*/
            (void)ILib_memcpy(&Data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[PortId][0], 10u);
            DataLen = 54u;
            break;
        case PDELAY_RESP_FOLLOW_UP:
            /*messageLength */
            Data[2] = 0u;
            Data[3] = 54u;
            /*flags Octet 0*/
            Data[6] = 0x00u;
            Data[7] = 0x08u;
            /*sequenceId*/
            Data[30] = (uint8)(port->ReceivedPdelayReqSequenceId >> 8u);
            Data[31] = (uint8)port->ReceivedPdelayReqSequenceId;
            /*control*/
            Data[32] = 0x5u;
            /*logMessageInterval*/
            Data[33] = 0x7F;
            /*responseOriginTimestamp*/
            EthTSyn_TimeToData(port->pdelayResponderTimestamp.pdelayRespEventEgressTimestamp, &Data[34]);
            /*requestingPortIdentity*/
            (void)ILib_memcpy(&Data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[PortId][0], 10u);
            DataLen = 54u;
            break;
        default:
            /* do nothing */
            break;
        }
        if ((FrameType == SYNC) && (!hasCapturedEgress))
        {
            return;
        }

        StbM_VirtualLocalTimeType VirtualLocalTime;
        if (StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef, &VirtualLocalTime)
            != E_OK)
        {
            return;
        }

        Length = DataLen;
        BufReq_ReturnType ret = EthIf_ProvideTxBuffer(CtrlIdx, FrameType, Priority, &BufId, &DataBuf, &Length);
        if ((ret == BUFREQ_OK) && (DataBuf != NULL_PTR) && (Length >= DataLen))
        {
            /*[SWS_EthTSyn_00201] [SWS_EthTSyn_00202] [SWS_EthTSyn_00203]*/
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
            if ((EthTSynMessage == PDELAY_REQ) || (EthTSynMessage == PDELAY_RESP) || (EthTSynMessage == SYNC))
            {
                EthIf_EnableEgressTimeStamp(CtrlIdx, BufId);
            }
#endif
            (void)ILib_memcpy(DataBuf, Data, DataLen);
            port->Txconfirmation = (((uint64)VirtualLocalTime.nanosecondsHi) << 32u) + VirtualLocalTime.nanosecondsLo;
            port->TranssmitMessageType = EthTSynMessage;
            (void)EthIf_Transmit(CtrlIdx, BufId, FrameType, TRUE, DataLen, &EthTSynDestPhyAddr[0]);
        }
        else
        {
            port->SyncSequenceId--;
        }
    }
}
static boolean EthTSyn_StatusGet(uint16 PortId, uint8 status)
{
    boolean ret;
    StbM_TimeBaseStatusType SyncBase, OffsetBase;
    uint8 timebaseid = PORT_TOCFGDOMIN(PortId).EthTSynSynchronizedTimeBaseRef;
    Std_ReturnType stdRet = StbM_GetTimeBaseStatus(timebaseid, &SyncBase, &OffsetBase);
    ret = (stdRet == E_OK) && ((SyncBase & status) > 0u);
    return ret;
}

static void EthTSyn_CreatCorrectionField(uint64 NS, uint8* Data)
{
    uint8 iloop;

    for (iloop = 0U; iloop < 6U; iloop++)
    {
        Data[iloop] = (uint8)(NS >> (40u - (uint8)(iloop << 3u)));
    }
    Data[6] = 0u;
    Data[7] = 0u;
}

static EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* Data)
{
    uint8 iloop;
    EthTSyn_TimeStampType timeStamp;
    uint64 ttime = 0u;

    for (iloop = 0u; iloop < 6u; iloop++)
    {
        ttime += ((uint64)Data[iloop]) << (40u - (uint8)(iloop << 3u));
    }
    timeStamp.secondsHi = 0;
    timeStamp.seconds = (uint32)(ttime / ETHTSYN_NANOSECOND_TO_SECOND);
    timeStamp.nanoseconds = (uint32)(ttime % ETHTSYN_NANOSECOND_TO_SECOND);

    return timeStamp;
}

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_CRC_SUPPORT == STD_ON)
static Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint8 portId,
    CrcValidatedType ValidatedType,
    const uint8* Data,
    uint8* SGWByte,
    uint8* OFSByte,
    uint16 frameLength,
    EthTSynCrcFlagsRxValidatedCfgType* CrcFlagsRxValidated)
{
    Std_ReturnType ret = E_OK;
    uint8 CrcData[17];

    uint16 msgLength = (Data[2] << 8) | Data[3];
    if (msgLength != frameLength)
    {
        ret = E_NOT_OK;
    }
    else if ((ValidatedType != CRC_IGNORED) && (msgLength > 86u))
    {
        uint8 datap = 85u;

        while (datap < (msgLength - 1))
        {
            if ((Data[datap + 1u] == 0x34u) || (Data[datap + 1u] == 0x44u))
            {
                *OFSByte = datap + 4u;
                *SGWByte = datap + 14u;
            }
            else if ((Data[datap + 1u] != 0x61u) && (Data[datap + 1u] != 0x60u))
            {
                *SGWByte = datap + 3u;
            }
            else
            {
                /* do nothing */
            }
            if (CRC_NOT_VALIDATED == ValidatedType)
            {
                if ((Data[datap + 1u] == 0x28u) || (Data[datap + 1u] == 0x44u) || (Data[datap + 1u] == 0x50u)
                    || (Data[datap + 1u] == 0x60u))
                {
                    ret = E_NOT_OK;
                    break;
                }
                else
                {
                    datap += Data[datap + 2u] + 2u;
                }
            }
            else
            {
                uint8 DataID =
                    PORT_TOCFGDOMIN(portId)
                        .EthTSynGlobalTimeFollowUpDataIDList->EthTSynGlobalTimeFollowUpDataIDListElement[Data[31] % 16u]
                        .EthTSynGlobalTimeFollowUpDataIDListValue;
                switch (Data[datap + 1u])
                {
                case 0x28: {
                    /* CRC_Time_Flags */
                    uint8 CrcData0[24] = {0};
                    uint8 CrcData1[16] = {0};
                    uint8 CrcData0length = 1u;
                    uint8 CrcData1length = 1u;
                    CrcData0[0] = Data[datap + 3u];
                    CrcData1[0] = Data[datap + 3u];
                    /* local and promote CRC element configuration matching */
                    if ((CrcFlagsRxValidated->EthTSynCrcMessageLength) ^ ((Data[datap + 3u] & 0x01u) > 0u)
                        || (CrcFlagsRxValidated->EthTSynCrcDomainNumber) ^ ((Data[datap + 3u] & 0x02u) > 0u)
                        || (CrcFlagsRxValidated->EthTSynCrcCorrectionField) ^ ((Data[datap + 3u] & 0x04u) > 0u)
                        || (CrcFlagsRxValidated->EthTSynCrcSequenceId) ^ ((Data[datap + 3u] & 0x10u) > 0u)
                        || (CrcFlagsRxValidated->EthTSynCrcPreciseOriginTimestamp) ^ ((Data[datap + 3u] & 0x20u) > 0u)
                        || (CrcFlagsRxValidated->EthTSynCrcSourcePortIdentity) ^ ((Data[datap + 3u] & 0x08u) > 0u))
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                    /* CRCMessageLength */
                    if ((CrcFlagsRxValidated->EthTSynCrcMessageLength) && ((Data[datap + 3u] & 0x01u) > 0u))
                    {
                        CrcData1[CrcData1length] = Data[3];
                        CrcData1length++;
                    }
                    /* CRCDomainNumber */
                    if ((CrcFlagsRxValidated->EthTSynCrcDomainNumber) && ((Data[datap + 3u] & 0x02u) > 0u))
                    {
                        CrcData0[CrcData0length] = Data[4];
                        CrcData0length++;
                    }
                    /* CrcCorrectionField */
                    if ((CrcFlagsRxValidated->EthTSynCrcCorrectionField) && ((Data[datap + 3u] & 0x04u) > 0u))
                    {
                        (void)ILib_memcpy(&CrcData1[CrcData0length], &Data[8], 8u);
                        CrcData1length += 8u;
                    }
                    /* CRCSequenceIdentity */
                    if ((CrcFlagsRxValidated->EthTSynCrcSequenceId) && ((Data[datap + 3u] & 0x10u) > 0u))
                    {
                        CrcData1[CrcData1length] = Data[30];
                        CrcData1[CrcData1length + 1u] = Data[31];
                        CrcData1length += 2u;
                    }
                    /* preciseOriginTimestamp */
                    if ((CrcFlagsRxValidated->EthTSynCrcPreciseOriginTimestamp) && ((Data[datap + 3u] & 0x20u) > 0u))
                    {
                        (void)ILib_memcpy(&CrcData0[CrcData0length], &Data[34], 10u);
                        CrcData0length += 10u;
                    }
                    /* sourcePortIdentity */
                    if ((CrcFlagsRxValidated->EthTSynCrcSourcePortIdentity) && ((Data[datap + 3u] & 0x08u) > 0u))
                    {
                        (void)ILib_memcpy(&CrcData0[CrcData0length], &Data[20], 10u);
                        CrcData0length += 10u;
                    }
                    CrcData0[CrcData0length] = DataID;
                    CrcData1[CrcData0length] = DataID;
                    /* CRC_Time_0 */
                    uint8 CRC_Time_0 = Crc_CalculateCRC8H2F(&CrcData0[0], (uint32)CrcData0length + 1u, 0xFF, TRUE);
                    /* CRC_Time_1 */
                    uint8 CRC_Time_1 = Crc_CalculateCRC8H2F(&CrcData1[0], (uint32)CrcData1length + 1u, 0xFF, TRUE);
                    if ((CRC_Time_0 != Data[datap + 4u]) || (CRC_Time_1 != Data[datap + 5u]))
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                    break;
                }
                case 0x44: {
                    (void)ILib_memcpy(&CrcData[0], &Data[datap + 3u], 16u);
                    CrcData[16] = DataID;
                    uint8 CRC_data = Crc_CalculateCRC8H2F(&CrcData[0], 17, 0xFF, TRUE);
                    if (CRC_data != Data[datap + 19u])
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                    break;
                }

                case 0x50: {
                    CrcData[0] = Data[datap + 3u];
                    CrcData[1] = DataID;
                    uint8 CRC_data = Crc_CalculateCRC8H2F(&CrcData[0], 2, 0xFF, TRUE);
                    if (CRC_data != Data[datap + 4u])
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                    break;
                }
                case 0x60: {
                    (void)ILib_memcpy(&CrcData[0], &Data[datap + 3u], 4u);
                    CrcData[4] = DataID;
                    uint8 CRC_data = Crc_CalculateCRC8H2F(&CrcData[0], 5, 0xFF, TRUE);
                    if (CRC_data != Data[datap + 7u])
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                    break;
                }
                default:
                    /* Intentionally Empty */
                    break;
                }
                if (ValidatedType == CRC_VALIDATED)
                {
                    if ((Data[datap + 1u] == 0x34u) || (Data[datap + 1u] == 0x51u) || (Data[datap + 1u] == 0x61u))
                    {

                        ret = E_NOT_OK;
                        break;
                    }
                }
                datap += Data[datap + 2u] + 2u;
            }
        }
    }
    return ret;
}
#endif

#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbmUseEthHW(uint16 PortId)
{
    return PORT_TOCFGPORT(PortId).SynchronizedTimeBaseUseEthFreerunning;
}
#endif

ETHTSYN_LOCAL void EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp)
{
    while (timestamp->nanoseconds >= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        timestamp->seconds++;
        if (timestamp->seconds == 0u)
        {
            timestamp->secondsHi++;
        }
        else
        {
            /* do nothing */
        }
        timestamp->nanoseconds -= ETHTSYN_NANOSECOND_TO_SECOND;
    }
}

ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2)
{
    uint64 second1 = (((uint64)time1.secondsHi) << 32u) + time1.seconds;
    uint64 second2 = (((uint64)time2.secondsHi) << 32u) + time2.seconds;
    uint64 second = second1 + second2;

    EthTSyn_TimeStampType t;
    t.nanoseconds = time1.nanoseconds + time2.nanoseconds;
    t.seconds = (uint32)second;
    t.secondsHi = (uint16)(second >> 32u);
    EthTSyn_timeoptimal(&t);
    return t;
}

static boolean EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2)
{
    boolean ret = TRUE;
    if (time2 <= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        if ((time1.nanoseconds > time2) || (time1.seconds > 0u) || (time1.secondsHi > 0u))
        {
            ret = FALSE;
        }
    }
    else
    {
        uint64 second = (((uint64)time1.secondsHi) << 32u) + time1.seconds;
        if (second > (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            ret = FALSE;
        }
        if (second == (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            if (time1.nanoseconds > (time2 % ETHTSYN_NANOSECOND_TO_SECOND))
            {
                ret = FALSE;
            }
        }
    }
    return ret;
}

ETHTSYN_LOCAL sint8 EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2)
{
    uint64 t1Seconds = ((uint64)t1.secondsHi << 32) | (uint64)t1.seconds;
    uint64 t2Seconds = ((uint64)t2.secondsHi << 32) | (uint64)t2.seconds;
    sint8 sign;
    if (t1Seconds < t2Seconds)
    {
        sign = -1;
    }
    else if (t1Seconds == t2Seconds)
    {
        sint32 tdNanoseconds = (sint32)t1.nanoseconds - (sint32)t2.nanoseconds;
        if (tdNanoseconds < 0)
        {
            sign = -1;
        }
        else if (tdNanoseconds == 0)
        {
            sign = 0;
        }
        else
        {
            sign = 1;
        }
    }
    else
    {
        sign = 1;
    }
    return sign;
}

ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td)
{
    /* PRQA S 1339 ++ */ /* VL_EthTSyn_1339 */
    EthTSyn_timeoptimal(&t1);
    EthTSyn_timeoptimal(&t2);
    /* PRQA S 1339 -- */
    uint64 tdSeconds =
        (((uint64)t1.secondsHi << 32) | (uint64)t1.seconds) - (((uint64)t2.secondsHi << 32) | (uint64)t2.seconds);
    sint32 tdNanoseconds = (sint32)t1.nanoseconds - (sint32)t2.nanoseconds;

    if (tdNanoseconds < 0)
    {
        tdNanoseconds += (sint32)ETHTSYN_NANOSECOND_TO_SECOND;
        tdSeconds--;
    }

    td->secondsHi = (uint16)((uint64)tdSeconds >> 32u);
    td->seconds = (uint32)tdSeconds;
    td->nanoseconds = (uint32)tdNanoseconds;
}

ETHTSYN_LOCAL boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentityA, const uint8* portIdentityB)
{
    return ILib_memcmp(portIdentityA, portIdentityB, ETHTSYN_PORT_IDENTITY_LENGTH) == 0;
}

ETHTSYN_LOCAL EthTSynPortType* EthTSyn_FindPort(uint8 ctrlIdx)
{
    EthTSynPortType* port = NULL_PTR;

    for (uint8 portIdx = 0u; portIdx < ETHTSYN_ETH_PORT_NUM; ++portIdx)
    {
        if (ctrlIdx == EthTSyn_Port[portIdx].CtrlId)
        {
            port = &EthTSyn_Port[portIdx];
        }
    }
    return port;
}

ETHTSYN_LOCAL EthTSynMessageType EthTSyn_MainFunctionMaster(uint8 portIndex)
{
    EthTSynMessageType syncMessageType = MSG_NONE;
    const EthTSynPortConfigCfgType* portCfgPtr = &PORT_TOCFGPORT(portIndex);
    EthTSynPortType* portPtr = &EthTSyn_Port[portIndex];

    uint64 globalTimeDebounceTime = portCfgPtr->EthTSynGlobalTimeDebounceTime;
    if ((globalTimeDebounceTime > 0u) && (portPtr->SyncdebounceCounter > 0u))
    {
        --portPtr->SyncdebounceCounter;
    }

    uint64 globalTimeTxPeriod = portCfgPtr->EthTSynPortRole.EthTSynGlobalTimeMaster->EthTSynGlobalTimeTxPeriod;
    if ((globalTimeTxPeriod > 0u) && (portPtr->SyncReqPeriod > 0u))
    {
        --portPtr->SyncReqPeriod;
    }

    if (portPtr->SyncdebounceCounter != 0u)
    {
        return syncMessageType;
    }

    if (portPtr->SyncStatusType == SEND_SYNC)
    {
        boolean isImmediate = FALSE;
        if (portCfgPtr->EthTSynPortRole.EthTSynGlobalTimeMaster->EthTSynImmediateTimeSync)
        {
            uint8 timeBaseUpdateCounter =
                StbM_GetTimeBaseUpdateCounter(PORT_TOCFGDOMIN(portIndex).EthTSynSynchronizedTimeBaseRef);
            if (EthTSyn_StatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE)
                && (timeBaseUpdateCounter != portPtr->TimeBaseUpdateCounter))
            {
                syncMessageType = SYNC;
                isImmediate = TRUE;
                portPtr->TimeBaseUpdateCounter = timeBaseUpdateCounter;
            }
        }

        if (EthTSyn_StatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) && (globalTimeTxPeriod > 0u)
            && (portPtr->SyncReqPeriod == 0u))
        {
            syncMessageType = SYNC;
        }

        if (syncMessageType == SYNC)
        {
            portPtr->SyncReqPeriod =
                isImmediate ? portCfgPtr->EthTSynPortRole.EthTSynGlobalTimeMaster->EthTSynCyclicMsgResumeTime
                            : globalTimeTxPeriod;
            portPtr->SyncdebounceCounter = globalTimeDebounceTime;
        }
    }
    else
    {
        syncMessageType = FOLLOW_UP;
        portPtr->SyncStatusType = SEND_SYNC;
        portPtr->SyncdebounceCounter = globalTimeDebounceTime;
    }

    return syncMessageType;
}

static void EthTSyn_InitPortStatus(EthTSynPortType* port)
{
    port->EthtrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
    port->PdelayReqPeriod = 0u;
    port->SyncReqPeriod = 0u;
    port->PdelayTXStatusType = SEND_PDELAY_REQ;
    port->PdelayRXStatusType = WAITFOR_PDELAY_REQ;
    port->SyncStatusType = (ETHTSYN_MASTER == port->PortRoleType) ? SEND_SYNC : WAITFOR_SYNC;
    port->hasReceivedSyncReq = FALSE;
    port->SyncSequenceId = 0xFFFFu;
    port->PdelayReqSequenceId = 0xFFFFu;
    port->ReceivedPdelayReqSequenceId = 0u;
    port->SyncdebounceCounter = 0u;
    port->PdelaydebounceCounter = 0u;
}

#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)

static boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = pointer != NULL_PTR;
    if (!valid)
    {
        ETHTSYN_DET_REPORT(apiId, ETHTSYN_E_PARAM_POINTER);
    }
    return valid;
}

static boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr)
{
    boolean valid = !EthTSyn_Initialized;
    if (!valid)
    {
        ETHTSYN_DET_REPORT(ETHTSYN_SID_INIT, ETHTSYN_E_ALREADY_INITIALIZED);
    }
    else
    {
        valid = EthTSyn_ValidatePointer(ETHTSYN_SID_INIT, configPtr);
    }
    return valid;
}

static boolean EthTSyn_ValidateInitStatus(uint8 apiId)
{
    boolean valid = EthTSyn_Initialized;
    if (!valid)
    {
        ETHTSYN_DET_REPORT(apiId, ETHTSYN_E_UNINIT);
    }
    return valid;
}

static boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx)
{
    boolean valid = EthTSyn_ValidateInitStatus(apiId);
    if (valid)
    {
        EthTSynPortType* portPtr = EthTSyn_FindPort(ctrlIdx);
        if (portPtr == NULL_PTR)
        {
            valid = FALSE;
            ETHTSYN_DET_REPORT(apiId, ETHTSYN_E_CTRL_IDX);
        }
    }
    return valid;
}

static boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer)
{
    return EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_RXINDICATION, ctrlIdx)
           && EthTSyn_ValidatePointer(ETHTSYN_SID_RXINDICATION, pointer);
}

static boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType Mode)
{
    boolean valid = EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_SETTRANSMISSIONMODE, ctrlIdx);
    if (valid && (Mode > ETHTSYN_TX_ON))
    {
        ETHTSYN_DET_REPORT(ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_PARAM);
        valid = FALSE;
    }
    return valid;
}

#endif
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
