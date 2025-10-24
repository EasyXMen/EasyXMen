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
 ************************************************************************************************************************
 **
 **  @file               : EthTSyn.c
 **  @author             : yuzhe.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation for EthTSyn
 **
 ***********************************************************************************************************************/
/* PRQA S 6060 ++ */       /* VL_MTR_EthTSyn_STM19 */
/* PRQA S 6520 EOF */      /* VL_MTR_EthTSyn_STVAR */
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "EthTSyn.h"
#include "SchM_EthTSyn.h"
#include "EthTSyn_Types.h"
#include "IStdLib.h"
#include "StbM.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_CRC_SUPPORT == STD_ON)
#include "Crc.h"
#endif
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON) || (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
#include "Det.h"
#endif
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */
#ifndef ETHTSYN_LOCAL
#define ETHTSYN_LOCAL static
#endif

#ifndef ETHTSYN_LOCAL_INLINE
#define ETHTSYN_LOCAL_INLINE ETHTSYN_LOCAL inline
#endif

#define TIMEDOMIN_CFG(timedomainid)    EthTSyn_ConfigData->GlobalTimeDomain[timedomainid]
#define PORT_CFG(timedomainid, PortId) EthTSyn_ConfigData->GlobalTimeDomain[timedomainid].PortConfig[PortId]
#define PORT_TOCFGDOMIN(PortId) \
    EthTSyn_ConfigData->GlobalTimeDomain[EthTSyn_ConfigData->RuntimeVariables[PortId]->DomainIndex]
#define PORT_TOCFGPORT(PortId)                                                                      \
    EthTSyn_ConfigData->GlobalTimeDomain[EthTSyn_ConfigData->RuntimeVariables[PortId]->DomainIndex] \
        .PortConfig[EthTSyn_ConfigData->RuntimeVariables[PortId]->PortIdinTimedomain]
#define ETHTSYN_UNUSED(var) (void)(var)
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_GetEgressTV(uint8 ctrlIdx, uint8 portId, EthTSyn_TimeStampType* tv, Eth_BufIdxType bufIdx);
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* data);
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType virtualLocalTime);
ETHTSYN_LOCAL void                  EthTSyn_TimeToData(EthTSyn_TimeStampType rawTime, uint8* data);
ETHTSYN_LOCAL void                  EthTSyn_TranssmitMessage(EthTSynMessageType ethTSynMessage, uint16 portId);
ETHTSYN_LOCAL boolean               EthTSyn_ExpectedStatusGet(uint16 portId, uint8 expectedStatus);
ETHTSYN_LOCAL void                  EthTSyn_CreatCorrectionField(uint64 ns, uint8* data);
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* data);
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
ETHTSYN_LOCAL void
    EthTSyn_CalculateTimeSecuredCrc(uint16 portId, const uint8* data, uint16 offset, uint8* crc0, uint8* crc1);
#endif
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyTimeSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusNotSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet);
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet);
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataNotSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyTLVHeader(const uint8* data);
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint16       portId,
    const uint8* data,
    uint16       frameLength,
    boolean*     isSyncToGatewayBitSet);
#endif
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbmEthHWUsed(uint16 portId);
#endif
ETHTSYN_LOCAL void                  EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp);
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2);
ETHTSYN_LOCAL boolean               EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2);
ETHTSYN_LOCAL sint32                EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2);
ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td);
ETHTSYN_LOCAL boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentity1, const uint8* portIdentity2);
/* PRQA S 5016 ++ */ /* VL_EthTSyn_5016 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPort(uint8 ctrlIdx, uint8* portIdx);
/* PRQA S 5016 -- */
ETHTSYN_LOCAL EthTSynMessageType EthTSyn_MainFunctionMaster(uint8 portIndex);
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL boolean EthTSyn_GetHardIngressTime(uint8 ctrlIdx, const uint8* dataPtr, EthTSyn_TimeStampType* tVnow);
#else
ETHTSYN_LOCAL boolean EthTSyn_GetSoftIngressTime(const EthTSyn_PortType* port, EthTSyn_TimeStampType* tVnow);
#endif
ETHTSYN_LOCAL boolean
    EthTSyn_GetSyncT2vlt(const EthTSyn_PortType* port, EthTSyn_TimeStampType t1vlt, EthTSyn_TimeStampType* t2vlt);
ETHTSYN_LOCAL void EthTSyn_RxHandleSyncMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime);
ETHTSYN_LOCAL void EthTSyn_RxHandleFollowUpMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime,
    uint16                lenByte);
ETHTSYN_LOCAL void EthTSyn_RxHandlePdelayResPonseMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime);
ETHTSYN_LOCAL void EthTSyn_RxHandlePdelayResPonseFUMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime);
ETHTSYN_LOCAL void EthTSyn_OriginalTimeStampCaculation(
    uint8                 ctrlIdx,
    EthTSyn_PortType*     port,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType ingressTimeStamp);
ETHTSYN_LOCAL uint16 EthTSyn_SyncMessagePack(EthTSyn_PortType* port, uint8* data, boolean* hasCapturedEgress);
ETHTSYN_LOCAL uint16 EthTSyn_FollowUpMessagePack(const EthTSyn_PortType* port, uint8* data);
ETHTSYN_LOCAL uint16 EthTSyn_ReqMessagePack(EthTSyn_PortType* port, uint8* data);
ETHTSYN_LOCAL void   EthTSyn_PublicInformationPack(const EthTSyn_PortType* port, uint8* data, uint8 ethTSynMessage);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
ETHTSYN_LOCAL boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer);
ETHTSYN_LOCAL boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx);
ETHTSYN_LOCAL boolean EthTSyn_ValidateInitStatus(uint8 apiId);
ETHTSYN_LOCAL boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr);
ETHTSYN_LOCAL boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer);
ETHTSYN_LOCAL boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode);
ETHTSYN_LOCAL uint8   EthTSyn_GetPartitionIndex(void);
ETHTSYN_LOCAL boolean EthTSyn_ValidatePartitionContext(uint8 apiId);
#endif
ETHTSYN_LOCAL void EthTSyn_InitPortStatus(EthTSyn_PortType* portPtr);
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/** Module initialize status, TRUE initialized, FALSE not initialized*/
#define ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_Initialized[ETHTSYN_PARTITION_NUMBER_MAX];
#define ETHTSYN_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL const EthTSyn_ConfigType* EthTSyn_ConfigData;
ETHTSYN_LOCAL uint8 EthTSyn_ReceivedPdelayReqSourcePortId[ETHTSYN_ETH_PORT_NUM_MAX][ETHTSYN_PORT_IDENTITY_LENGTH];
#define ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * This function initializes the Time Synchronization over Ethernet.
 */
void EthTSyn_Init(
    const EthTSyn_ConfigType*
        configPtr) /* PRQA S 6030 */ /* VL_MTR_EthTSyn_STMIF */ /* PRQA S 6070 */ /* VL_MTR_EthTSyn_STCAL*/
{
    EthTSyn_ConfigData = configPtr;
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidatePartitionContext(ETHTSYN_SID_INIT) && EthTSyn_ValidateInit(configPtr))
#endif
    {
        uint16 ethTSynPortNum = 0u;
        for (uint8 domainIndex = 0u; domainIndex < ETHTSYN_GLOBALTIME_DOMAIN_NUM; domainIndex++)
        {
            for (uint8 portInnerIndex = 0u; portInnerIndex < TIMEDOMIN_CFG(domainIndex).PortConfigNum; portInnerIndex++)
            {
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
                if (PORT_CFG(domainIndex, portInnerIndex).PortReferrencedPartitionId == GetApplicationID())
#endif
                {
                    EthTSyn_PortType* port  = EthTSyn_ConfigData->RuntimeVariables[ethTSynPortNum];
                    port->Timedomainid      = TIMEDOMIN_CFG(domainIndex).GlobalTimeDomainId;
                    port->DomainIndex       = domainIndex;
                    port->CtrlId            = PORT_CFG(domainIndex, portInnerIndex).GlobalTimeEthIfRef;
                    port->SCHysteresisCount = ETHTSYN_MAX_HYSTERESIS_COUNTER;
                    port->PortRoleType = (PORT_CFG(domainIndex, portInnerIndex).PortRole.GlobalTimeMaster != NULL_PTR)
                                             ? ETHTSYN_MASTER
                                             : ETHTSYN_SLAVE;
                    EthTSyn_InitPortStatus(port);
                    port->FramePrio           = PORT_CFG(domainIndex, portInnerIndex).FramePrio;
                    port->PortIdinTimedomain  = portInnerIndex;
                    port->PortIndexInPortList = ethTSynPortNum;

                    uint64 delay = PORT_CFG(domainIndex, portInnerIndex).PdelayConfig->GlobalTimePropagationDelay;
                    port->Pdelay.Nanoseconds = (uint32)((uint32)delay % ETHTSYN_NANOSECOND_TO_SECOND);
                    delay /= ETHTSYN_NANOSECOND_TO_SECOND;
                    port->Pdelay.Seconds   = (uint32)delay;
                    port->Pdelay.SecondsHi = (uint16)(delay >> ETHTSYN_BIT_SHIFT_32);

                    port->TransmissionMode = ETHTSYN_TX_OFF;

                    /* Initialize data */
                    (void)IStdLib_MemSet(
                        EthTSyn_ReceivedPdelayReqSourcePortId[ethTSynPortNum],
                        0,
                        ETHTSYN_PORT_IDENTITY_LENGTH);

                    uint8 srcPhyAddr[ETHTSYN_PHYS_ADDR_LENGTH];
                    EthIf_GetPhysAddr(port->CtrlId, srcPhyAddr);
                    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                    /* ThisPortIdentity.clockIdentity */
                    port->ThisPortIdentity[0u] = srcPhyAddr[0u];
                    port->ThisPortIdentity[1u] = srcPhyAddr[1u];
                    port->ThisPortIdentity[2u] = srcPhyAddr[2u];
                    port->ThisPortIdentity[3u] = ETHTSYN_PORT_IDENTITY_DEFAULT_VALUE0;
                    port->ThisPortIdentity[4u] = ETHTSYN_PORT_IDENTITY_DEFAULT_VALUE1;
                    port->ThisPortIdentity[5u] = srcPhyAddr[3u];
                    port->ThisPortIdentity[6u] = srcPhyAddr[4u];
                    port->ThisPortIdentity[7u] = srcPhyAddr[5u];
                    /* ThisPortIdentity.portNumber */
                    uint16 portNumber          = (uint16)(ethTSynPortNum + 1u);
                    port->ThisPortIdentity[8u] = (uint8)((uint16)(portNumber) >> ETHTSYN_BIT_SHIFT_8);
                    port->ThisPortIdentity[9u] = (uint8)portNumber;
                    /* PRQA S 3120 -- */

                    (void)IStdLib_MemSet(&port->PortIdentity, 0, ETHTSYN_PORT_IDENTITY_LENGTH);
                    port->UserData.userByte0                 = 0u;
                    port->UserData.userByte1                 = 0u;
                    port->UserData.userByte2                 = 0u;
                    port->UserData.userDataLength            = 0u;
                    port->TimeBaseUpdateCounter              = 0u;
                    port->PreciseOriginTimestamp.Nanoseconds = 0u;
                    port->PreciseOriginTimestamp.Seconds     = 0u;
                    port->PreciseOriginTimestamp.SecondsHi   = 0u;
                    port->T1vlt                              = port->PreciseOriginTimestamp;
                    port->T1                                 = port->T1vlt;
                    port->T2                                 = port->T1;
                    port->T3                                 = port->T1;
                    port->T4                                 = port->T1;
                    ethTSynPortNum++;
                }
            }
        }
        EthTSyn_Initialized[EthTSyn_GetPartitionIndex()] = TRUE;
    }
}
#if (ETHTSYN_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 */
void EthTSyn_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidatePointer(ETHTSYN_SID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID         = ETHTSYN_VENDOR_ID;
        versioninfo->moduleID         = ETHTSYN_MODULE_ID;
        versioninfo->sw_major_version = ETHTSYN_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ETHTSYN_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ETHTSYN_SW_PATCH_VERSION;
    }
}
#endif
/**
 * This API is used to turn on and off the TX capabilities of the EthTSyn.
 */
void EthTSyn_SetTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode)
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidatePartitionContext(ETHTSYN_SID_SETTRANSMISSIONMODE)
        && EthTSyn_ValidateTransmissionMode(ctrlIdx, mode))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindPort(ctrlIdx, NULL_PTR);
        port->TransmissionMode = mode;
    }
}
/**
 * By this API service the EthTSyn gets an indication and the data of a received frame.
 */
/* PRQA S 6040 ++ */ /* VL_MTR_EthTSyn_STPAR */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
void EthTSyn_RxIndication(
    uint8         ctrlIdx,
    Eth_FrameType frameType,
    boolean       isBroadcast,
    const uint8*  physAddrPtr,
    const uint8*  dataPtr,
    uint16        lenByte)
/* PRQA S 6070 -- */
/* PRQA S 6040 -- */
{
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    /* SWS_EthTSyn_00180 */
    SchM_Enter_EthTSyn_Context();
#endif
    ETHTSYN_UNUSED(isBroadcast);
    ETHTSYN_UNUSED(physAddrPtr);
    uint8             portIndex = 0u;
    EthTSyn_PortType* port      = EthTSyn_FindPort(ctrlIdx, &portIndex);
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON)
    ETHTSYN_UNUSED(lenByte);
#endif
    if (
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_RXINDICATION) && EthTSyn_ValidateRxindication(ctrlIdx, dataPtr) &&
#endif
        ((port != NULL_PTR) && (frameType == ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE)
         && (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
         && (port->Timedomainid == dataPtr[ETHTSYN_TIMEDOMAINID_BYTE_OFFSET])))
    {
        EthTSynMessageType    ethTSynMessage = (dataPtr[0u] & ETHTSYN_LOW_4_BITS_MASK);
        EthTSyn_TimeStampType tVnow;
        boolean               isValidIngressTime = FALSE;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
        isValidIngressTime = EthTSyn_GetHardIngressTime(ctrlIdx, dataPtr, &tVnow);
#else
        isValidIngressTime = EthTSyn_GetSoftIngressTime(port, &tVnow);
        SchM_Exit_EthTSyn_Context();
#endif
        switch (ethTSynMessage)
        {
        case SYNC:
        {
            EthTSyn_RxHandleSyncMessage(port, dataPtr, tVnow, isValidIngressTime);
        }
        break;
        case FOLLOW_UP:
        {
            EthTSyn_RxHandleFollowUpMessage(port, dataPtr, tVnow, isValidIngressTime, lenByte);
        }
        break;
        case PDELAY_REQ:
            if (isValidIngressTime)
            {
                port->PdelayResponderTimestamp.PdelayReqEventIngressTimestamp = tVnow;
                port->PdelayRXStatusType                                      = ETHTSYN_SEND_PDELAY_RESP;
                uint16 sequenceId =
                    (uint16)(((uint16)((uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] << ETHTSYN_BIT_SHIFT_8))
                             + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
                port->ReceivedPdelayReqSequenceId = sequenceId;
                (void)IStdLib_MemCpy(
                    &EthTSyn_ReceivedPdelayReqSourcePortId[portIndex][0],
                    &dataPtr[ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_OFFSET],
                    ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_LENGTH);
            }
            break;
        case PDELAY_RESP:
        {
            EthTSyn_RxHandlePdelayResPonseMessage(port, dataPtr, tVnow, isValidIngressTime);
        }
        break;
        case PDELAY_RESP_FOLLOW_UP:
        {
            EthTSyn_RxHandlePdelayResPonseFUMessage(port, dataPtr, tVnow, isValidIngressTime);
        }
        break;
        default:
            /* do nothing */
            break;
        }
    }
    else
    {
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
        SchM_Exit_EthTSyn_Context();
#endif
    }
}
/**
 * Confirms the transmission of an Ethernet frame.
 */
/* clang-format off */
void EthTSyn_TxConfirmation(uint8 ctrlIdx, Eth_BufIdxType bufIdx, Std_ReturnType result)
/* clang-format on */
{
    uint8             portIndex = 0u;
    EthTSyn_PortType* port      = EthTSyn_FindPort(ctrlIdx, &portIndex);
    if (
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_TXCONFIRMATION)
        && EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_TXCONFIRMATION, ctrlIdx) &&
#endif
        (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) && (result == E_OK))
    {
        EthTSyn_TimeStampType ingressTimeStamp  = {0u, 0u, 0u};
        boolean               isValidEgressTime = FALSE;
        if (EthTSyn_GetEgressTV(ctrlIdx, portIndex, &ingressTimeStamp, bufIdx) == E_OK)
        {
            isValidEgressTime = TRUE;
        }
        switch (port->TranssmitMessageType)
        {
        case SYNC:
        {
            EthTSyn_OriginalTimeStampCaculation(ctrlIdx, port, isValidEgressTime, ingressTimeStamp);
        }
        break;
        case PDELAY_REQ:
        {
            if (isValidEgressTime)
            {
                port->PdelayTXStatusType = ETHTSYN_WAITFOR_PDELAY_RESP;
                port->T1                 = ingressTimeStamp;
            }
            else
            {
                port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
            }
        }
        break;
        case PDELAY_RESP:
        {
            if (isValidEgressTime)
            {
                port->PdelayResponderTimestamp.PdelayRespEventEgressTimestamp = ingressTimeStamp;
                port->PdelayRXStatusType                                      = ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP;
            }
            else
            {
                port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
            }
        }
        break;
        case PDELAY_RESP_FOLLOW_UP:
        {
            port->PdelayRXStatusType = ETHTSYN_WAITFOR_PDELAY_REQ;
        }
        break;
        default:
            /* do nothing */
            break;
        }
    }
    else if ((result != E_OK))
    {
        EthTSyn_InitPortStatus(port);
    }
}
/**
 * Allows resetting state machine in case of unexpected Link loss to avoid inconsistent Sync and Follow_Up sequences.
 */
void EthTSyn_TrcvLinkStateChg(uint8 ctrlIdx, EthTrcv_LinkStateType trcvLinkState)
{
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateInitStatus(ETHTSYN_SID_TRCVLINKSTATECHG))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindPort(ctrlIdx, NULL_PTR);
        if (port != NULL_PTR)
        {
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) && (trcvLinkState == ETHTRCV_LINK_STATE_DOWN))
            {
                /*[SWS_EthTSyn_00019]*/
                EthTSyn_InitPortStatus(port);
            }
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_DOWN) && (trcvLinkState == ETHTRCV_LINK_STATE_ACTIVE))
            {
                /*[SWS_EthTSyn_00020]*/
                port->EthtrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
            }
#if (ETHTSYN_DEST_PHY_ADDR_MULTICAST == STD_ON)
            Eth_FilterActionType filterAction =
                (trcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER;
            (void)EthIf_UpdatePhysAddrFilter(ctrlIdx, EthTSynDestPhyAddr, filterAction);
#endif
        }
    }
}
/**
 * Main function for cyclic call/resp. Sync, Follow_Up and Pdelay_Req transmissions.
 */
void EthTSyn_MainFunction(uint8 portIndex) /* PRQA S 6030 */ /* VL_MTR_EthTSyn_STMIF */
{
    EthTSyn_PortType* port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
    if (
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_MAINFUNCTION) &&
#endif
        EthTSyn_Initialized[EthTSyn_GetPartitionIndex()] && (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE))
    {
        EthTSynMessageType               transsmitMsgType  = MSG_NONE;
        const EthTSyn_PortConfigCfgType* portCfg           = &PORT_TOCFGPORT(portIndex);
        uint64                           txPdelayReqPeriod = portCfg->PdelayConfig->GlobalTimeTxPdelayReqPeriod;
        if (ETHTSYN_MASTER == port->PortRoleType)
        {
            transsmitMsgType = EthTSyn_MainFunctionMaster(portIndex);
        }
        else if (txPdelayReqPeriod != 0ULL)
        {
            if (port->PdelayReqPeriod <= ETHTSYN_MAIN_FUNCTION_PERIOD)
            {
                transsmitMsgType      = PDELAY_REQ;
                port->PdelayReqPeriod = txPdelayReqPeriod;
            }
            else
            {
                port->PdelayReqPeriod -= ETHTSYN_MAIN_FUNCTION_PERIOD;
            }
        }

        uint64 debounceCounter = portCfg->GlobalTimeDebounceTime * ETHTSYN_MAIN_FUNCTION_PERIOD;
        if ((port->PdelayRXStatusType == ETHTSYN_SEND_PDELAY_RESP)
            && (portCfg->PdelayConfig->GlobalTimePdelayRespEnable) && (transsmitMsgType == MSG_NONE))
        {
            transsmitMsgType            = PDELAY_RESP;
            port->PdelaydebounceCounter = debounceCounter;
        }
        else if (port->PdelayRXStatusType == ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP)
        {
            if (port->PdelaydebounceCounter <= ETHTSYN_MAIN_FUNCTION_PERIOD)
            {
                if (transsmitMsgType == MSG_NONE)
                {
                    transsmitMsgType            = PDELAY_RESP_FOLLOW_UP;
                    port->PdelaydebounceCounter = debounceCounter;
                }
            }
            else
            {
                port->PdelaydebounceCounter -= ETHTSYN_MAIN_FUNCTION_PERIOD;
            }
        }

        if (transsmitMsgType != MSG_NONE)
        {
            EthTSyn_TranssmitMessage(transsmitMsgType, portIndex);
        }
    }
}
/* ========================================== internal function definitions ========================================= */
/**
 * @brief         the implementation of getting egress time.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     portId: Index of the configured Port.
 * @param[in]     bufIdx: Index of the buffer provided by EthIf_ProvideTxBuffer.
 * @param[out]    tv: specifying the engress timestamp of transmitting data,which is determinant by bufIdx when
 * HARDWARE_TIMESTAMP enabled, or the virtual egress time captured by current virtual local time when HARDWARE_TIMESTAMP
 * disabled.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70672
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_GetEgressTV(uint8 ctrlIdx, uint8 portId, EthTSyn_TimeStampType* tv, Eth_BufIdxType bufIdx)
{
    Std_ReturnType ret;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    StbM_VirtualLocalTimeType VirtualLocalTime;
    ETHTSYN_UNUSED(bufIdx);
    ETHTSYN_UNUSED(ctrlIdx);
    SchM_Enter_EthTSyn_Context();
    ret = StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(portId).SynchronizedTimeBaseRef, &VirtualLocalTime);
    if (ret == E_OK)
    {
        *tv = EthTSyn_VTToGT(VirtualLocalTime);
    }
    SchM_Exit_EthTSyn_Context();
#else
    ETHTSYN_UNUSED(portId);
    Eth_TimeStampQualType timeQualPtr = ETH_INVALID;
    Eth_TimeStampType     timeStampPtr;
    ret = EthIf_GetEgressTimeStamp(ctrlIdx, bufIdx, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        tv->Nanoseconds = timeStampPtr.nanoseconds;
        tv->Seconds     = timeStampPtr.seconds;
        tv->SecondsHi   = timeStampPtr.secondsHi;
    }
    else
    {
        ret = E_NOT_OK;
    }
#endif
    return ret;
}
/**
 * @brief         the implementation of unpacking the time data in the raw data.
 * @param[in]     data: raw received data.
 * @return        parsed timestamp data.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70673
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* data)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    EthTSyn_TimeStampType t;
    t.Nanoseconds = ((uint32)data[6] << ETHTSYN_BIT_SHIFT_24) + ((uint32)data[7] << ETHTSYN_BIT_SHIFT_16)
                    + ((uint32)data[8] << ETHTSYN_BIT_SHIFT_8) + (uint32)data[9];
    t.Seconds = ((uint32)data[2] << ETHTSYN_BIT_SHIFT_24) + ((uint32)data[3] << ETHTSYN_BIT_SHIFT_16)
                + ((uint32)data[4] << ETHTSYN_BIT_SHIFT_8) + (uint32)data[5];
    t.SecondsHi = (uint16)((uint16)(((uint16)data[0]) << ETHTSYN_BIT_SHIFT_8) + (uint16)data[1]);
    /* PRQA S 3120 -- */
    return t;
}
/**
 * @brief         the implementation of translating local time to global time.
 * @param[in]     virtualLocalTime: virtual local time to be translated to global time.
 * @return        The global time after conversion.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70674
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType virtualLocalTime)
{
    EthTSyn_TimeStampType t;
    uint64 Vt     = (((uint64)virtualLocalTime.nanosecondsHi) << ETHTSYN_BIT_SHIFT_32) + virtualLocalTime.nanosecondsLo;
    t.Nanoseconds = (uint32)(Vt % ETHTSYN_NANOSECOND_TO_SECOND);
    Vt /= ETHTSYN_NANOSECOND_TO_SECOND;
    t.Seconds   = (uint32)Vt;
    t.SecondsHi = (uint16)(Vt >> ETHTSYN_BIT_SHIFT_32);
    return t;
}
/**
 * @brief         the implementation of packing the time data to the data buffer.
 * @param[in]     time: time data to be packed in to buffer.
 * @param[out]    data: data buffer after packing time data.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70676
 */
ETHTSYN_LOCAL void EthTSyn_TimeToData(EthTSyn_TimeStampType rawTime, uint8* data)
{
    uint64 second = (((uint64)rawTime.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + rawTime.Seconds;
    uint8  i;
    for (i = 0u; i < ETHTSYN_SECONDS_SIZE; i++)
    {
        data[i] = (uint8)(((second << (i * ETHTSYN_BIT_SHIFT_PER_BYTE))) >> ETHTSYN_BIT_SHIFT_40);
    }
    for (i = ETHTSYN_NANOSECONDS_BYTE_INDEX_START; i < ETHTSYN_NANOSECONDS_BYTE_INDEX_END; i++)
    {
        data[i] = (uint8)(((rawTime.Nanoseconds << ((i - ETHTSYN_SECONDS_SIZE) * ETHTSYN_BIT_SHIFT_PER_BYTE)))
                          >> ETHTSYN_BIT_SHIFT_24);
    }
}
/**
 * @brief         the implementation of packing the data into messages and transmitting them.
 * @param[in]     ethTSynMessage: time data to be packed in to buffer.
 * @param[in]     portId: the index of configured port.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70678
 */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
ETHTSYN_LOCAL void EthTSyn_TranssmitMessage(EthTSynMessageType ethTSynMessage, uint16 portId)
/* PRQA S 6070 -- */ /* VL_MTR_EthTSyn_STCAL */
{
    Eth_BufIdxType    bufId   = 0u;
    uint8*            dataBuf = NULL_PTR;
    uint8             data[ETHTSYN_BUFFER_TO_TRANSSMIT_LENGTH];
    uint16            dataLenth         = 0u;
    EthTSyn_PortType* port              = EthTSyn_ConfigData->RuntimeVariables[portId];
    boolean           hasCapturedEgress = TRUE;
    uint8             priority          = port->FramePrio;
    uint8             ctrlIdx           = port->CtrlId;
    if (port->TransmissionMode == ETHTSYN_TX_ON)
    {
        EthTSyn_PublicInformationPack(port, data, ethTSynMessage);
        switch (ethTSynMessage)
        {
        case SYNC:
        {
            dataLenth = EthTSyn_SyncMessagePack(port, data, &hasCapturedEgress);
            break;
        }
        case FOLLOW_UP:
            dataLenth = EthTSyn_FollowUpMessagePack(port, data);
            break;
        case PDELAY_REQ:
            dataLenth = EthTSyn_ReqMessagePack(port, data);
            break;
        case PDELAY_RESP:
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            /*messageLength */
            data[2] = 0u;
            data[3] = 54u;
            /*flags Octet 0*/
            data[6] = 0x02u;
            data[7] = 0x08u;
            /*sequenceId*/
            data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] =
                (uint8)(port->ReceivedPdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
            data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW] = (uint8)port->ReceivedPdelayReqSequenceId;
            /* control */
            data[32] = 0x5u;
            /*logMessageInterval*/
            data[33] = 0x7Fu;
            /*requestReceiptTimestamp*/
            EthTSyn_TimeToData(port->PdelayResponderTimestamp.PdelayReqEventIngressTimestamp, &data[34]);
            /*requestingPortIdentity*/
            (void)IStdLib_MemCpy(&data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[portId][0], 10u);
            dataLenth = 54u;
            break;
        case PDELAY_RESP_FOLLOW_UP:
            /*messageLength */
            data[2] = 0u;
            data[3] = 54u;
            /*flags Octet 0*/
            data[6] = 0x00u;
            data[7] = 0x08u;
            /*sequenceId*/
            data[30] = (uint8)(port->ReceivedPdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
            data[31] = (uint8)port->ReceivedPdelayReqSequenceId;
            /*control*/
            data[32] = 0x5u;
            /*logMessageInterval*/
            data[33] = 0x7Fu;
            /*responseOriginTimestamp*/
            EthTSyn_TimeToData(port->PdelayResponderTimestamp.PdelayRespEventEgressTimestamp, &data[34]);
            /*requestingPortIdentity*/
            (void)IStdLib_MemCpy(&data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[portId][0], 10u);
            dataLenth = 54u;
            /* PRQA S 3120 -- */
            break;
        default:
            /* do nothing */
            break;
        }
        if (hasCapturedEgress)
        {
            uint16            length    = dataLenth;
            Eth_FrameType     frameType = ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE;
            BufReq_ReturnType ret = EthIf_ProvideTxBuffer(ctrlIdx, frameType, priority, &bufId, &dataBuf, &length);
            if ((ret == BUFREQ_OK) && (dataBuf != NULL_PTR) && (length >= dataLenth))
            {
                /*[SWS_EthTSyn_00201] [SWS_EthTSyn_00202] [SWS_EthTSyn_00203]*/
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
                if ((ethTSynMessage == PDELAY_REQ) || (ethTSynMessage == PDELAY_RESP) || (ethTSynMessage == SYNC))
                {
                    EthIf_EnableEgressTimeStamp(ctrlIdx, bufId);
                }
#endif
                (void)IStdLib_MemCpy(dataBuf, data, dataLenth);
                port->TranssmitMessageType = ethTSynMessage;
                (void)EthIf_Transmit(ctrlIdx, bufId, frameType, TRUE, dataLenth, &EthTSynDestPhyAddr[0]);
            }
            else
            {
                port->SyncSequenceId--;
            }
        }
    }
}
/**
 * @brief         the implementation of getting the status of timebase,which is related by portID,and returning if the
 * expected status has been got.
 * @param[in]     ethTSynMessage: the expected status of timebase.
 * @param[in]     portId: the index of configured port.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70695
 */
ETHTSYN_LOCAL boolean EthTSyn_ExpectedStatusGet(uint16 portId, uint8 expectedStatus)
{
    boolean                 ret;
    StbM_TimeBaseStatusType syncBase;
    uint8                   timebaseid = PORT_TOCFGDOMIN(portId).SynchronizedTimeBaseRef;
    Std_ReturnType          stdRet     = StbM_GetTimeBaseStatus(timebaseid, &syncBase);
    ret                                = (stdRet == E_OK) && ((syncBase & expectedStatus) > 0u);
    return ret;
}
/**
 * @brief         the implementation of packing the correctionfield into data buffer.
 * @param[in]     ns: correctionfield data to be packed.
 * @param[in]     data: data buffer to be sended.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70696
 */
ETHTSYN_LOCAL void EthTSyn_CreatCorrectionField(uint64 ns, uint8* data)
{
    if (data != NULL_PTR)
    {
        for (uint8 iloop = 0u; iloop < ETHTSYN_NS_BYTES_TO_EXTRACT; iloop++)
        {
            data[iloop] =
                (uint8)(ns >> ((ETHTSTN_NS_BITS_TO_EXTRACT - ETHTSYN_BITS_PER_BYTE) - (iloop * ETHTSYN_BITS_PER_BYTE)));
        }
        data[ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX0] = 0u;
        data[ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX1] = 0u;
    }
}
/**
 * @brief         the implementation of unpacking the correctionfield into data buffer.
 * @param[in]     data: data buffer to be parsed.
 * @return        the correctionfield data.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70697
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* data)
{
    EthTSyn_TimeStampType timeStamp;
    uint64                tempTime = 0ULL;

    for (uint8 iloop = 0u; iloop < ETHTSYN_NS_BYTES_TO_EXTRACT; iloop++)
    {
        tempTime += ((uint64)data[iloop])
                    << ((ETHTSTN_NS_BITS_TO_EXTRACT - ETHTSYN_BITS_PER_BYTE) - (uint8)(iloop * ETHTSYN_BITS_PER_BYTE));
    }
    timeStamp.SecondsHi   = 0U;
    timeStamp.Seconds     = (uint32)(tempTime / ETHTSYN_NANOSECOND_TO_SECOND);
    timeStamp.Nanoseconds = (uint32)(tempTime % ETHTSYN_NANOSECOND_TO_SECOND);

    return timeStamp;
}

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**
 * @brief         the implementation of caculating CRC of the sub-tlv in the FU message,which contains the Secured Time.
 * @param[in]     portId: the index of configured port.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[out]    crc0: the CRC_Time_0.
 * @param[out]    crc1: the CRC_Time_1.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-77930
 */
ETHTSYN_LOCAL void
    EthTSyn_CalculateTimeSecuredCrc(uint16 portId, const uint8* data, uint16 offset, uint8* crc0, uint8* crc1)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint8 dataID = PORT_TOCFGDOMIN(portId)
                       .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                       .GlobalTimeFollowUpDataIDListValue;
    const EthTSyn_PortConfigCfgType*     portCfg = &PORT_TOCFGPORT(portId);
    EthTSyn_CrcTimeFlagsTxSecuredCfgType crcTimeFlags;
    if (portCfg->GlobalTimePortRole == ETHTSYN_MASTER)
    {
        crcTimeFlags = *portCfg->PortRole.GlobalTimeMaster->CrcTimeFlagsTxSecured;
    }
    else
    {
        crcTimeFlags = *portCfg->PortRole.GlobalTimeSlave->CrcFlagsRxValidated;
    }
    /* CRC_Time_Flags */
    uint8 crcData0[ETHTSYN_CRC_DATA_0_LENGTH];
    uint8 crcData1[ETHTSYN_CRC_DATA_1_LENGTH];
    (void)IStdLib_MemSet(&crcData0[0u], 0, ETHTSYN_CRC_DATA_0_LENGTH);
    (void)IStdLib_MemSet(&crcData1[0u], 0, ETHTSYN_CRC_DATA_1_LENGTH);
    uint8 crcData0Length = 1u;
    uint8 crcData1Length = 1u;
    crcData0[0u]         = data[offset + 2u];
    crcData1[0u]         = data[offset + 2u];
    /* CRCMessageLength */
    if (crcTimeFlags.CrcMessageLength)
    {
        crcData1[crcData1Length] = data[3];
        crcData1Length++;
    }
    /* CRCDomainNumber */
    if (crcTimeFlags.CrcDomainNumber)
    {
        crcData0[crcData0Length] = data[4];
        crcData0Length++;
    }
    /* CrcCorrectionField */
    if (crcTimeFlags.CrcCorrectionField)
    {
        (void)IStdLib_MemCpy(&crcData1[crcData0Length], &data[8], 8u);
        crcData1Length += 8u;
    }
    /* CRCSequenceIdentity */
    if (crcTimeFlags.CrcSequenceId)
    {
        crcData1[crcData1Length]      = data[30];
        crcData1[crcData1Length + 1u] = data[31];
        crcData1Length += 2u;
    }
    /* preciseOriginTimestamp */
    if (crcTimeFlags.CrcPreciseOriginTimestamp)
    {
        (void)IStdLib_MemCpy(&crcData0[crcData0Length], &data[34], 10u);
        crcData0Length += 10u;
    }
    /* sourcePortIdentity */
    if (crcTimeFlags.CrcSourcePortIdentity)
    {
        (void)IStdLib_MemCpy(&crcData0[crcData0Length], &data[20u], 10u);
        crcData0Length += 10u;
    }
    crcData0[crcData0Length] = dataID;
    crcData1[crcData0Length] = dataID;
    /* CRC_Time_0 */
    *crc0 = Crc_CalculateCRC8H2F(&crcData0[0], (uint32)crcData0Length + 1u, 0xFFu, TRUE);
    /* CRC_Time_1 */
    *crc1 = Crc_CalculateCRC8H2F(&crcData1[0], (uint32)crcData1Length + 1u, 0xFFu, TRUE);
    /* PRQA S 3120 -- */
}
#endif
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured Time.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-77968
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyTimeSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
/* PRQA S 2889 -- */
{
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    Std_ReturnType                        ret                   = E_NOT_OK;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_TIME_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_TIME_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVTime)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcTime0 = 0u;
            uint8 crcTime1 = 0u;
            EthTSyn_CalculateTimeSecuredCrc(portId, data, offset + 1u, &crcTime0, &crcTime1);
            if ((crcTime0 != data[offset + ETHTSYN_CRC_TIME0_OFFSET + 1u])
                || (crcTime1 != data[offset + ETHTSYN_CRC_TIME1_OFFSET + 1u]))
            {
                return E_NOT_OK;
            }
            ret = E_OK;
            break;
        }
#endif
        case ETHTSYN_CRC_IGNORED:
        {
            /* PRS_TS_00108*/
            ret = E_OK;
            break;
        }
        case ETHTSYN_CRC_NOT_VALIDATED:
        {
            /* PRS_TS_00107*/
            ret = E_NOT_OK;
            break;
        }
        default:
            /* do nothing */
            break;
        }
    }
    return ret;
}
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Not Secured status.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-77967
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusNotSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVStatus)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
        /* PRS_TS_00107 PRS_TS_00108 PRS_TS_00109 */
        case ETHTSYN_CRC_IGNORED:
        case ETHTSYN_CRC_NOT_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            /* PRS_TS_00156*/
            if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
            {
                /* PRS_TS_00156*/
                *isSyncToGatewayBitSet = TRUE;
            }
            ret = E_OK;
            break;
        }
        case ETHTSYN_CRC_VALIDATED:
        {
            ret = E_NOT_OK;
            break;
        }
        default:
        {
            ret = E_NOT_OK;
            break;
        }
        }
    }
    /* PRQA S 3120 -- */
    return ret;
}
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured status.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-77966
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVStatus)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        /* PRS_TS_00107 PRS_TS_00109*/
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcData[ETHTSYN_CRC_DATA_LENGTH];
            crcData[0u] = data[offset + 3u];
            crcData[1u] = PORT_TOCFGDOMIN(portId)
                              .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                              .GlobalTimeFollowUpDataIDListValue;
            uint8 crcDataResult = Crc_CalculateCRC8H2F(&crcData[0u], 2u, 0xFFu, TRUE);
            if (crcDataResult != data[offset + 4u])
            {
                ret = E_NOT_OK;
            }
            else
            {
                /*PRS_TS_00156*/
                if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
                {
                    *isSyncToGatewayBitSet = TRUE;
                }
                ret = E_OK;
            }
            break;
        }
#endif
        /* PRS_TS_00108*/
        case ETHTSYN_CRC_IGNORED:
        {
            /*PRS_TS_00156*/
            if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
            {
                *isSyncToGatewayBitSet = TRUE;
            }
            ret = E_OK;
            break;
        }
        case ETHTSYN_CRC_NOT_VALIDATED:
        {
            ret = E_NOT_OK;
            break;
        }
        default: /* PRQA S 3120 */ /* VL_QAC_EmptyClause*/
        {
            ret = E_NOT_OK;
            break;
        }
        }
    }

    return ret;
    /* PRQA S 3120 -- */
}
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Not Secured userdata.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-77965
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataNotSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
{
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVUserData)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
        /* PRS_TS_00107 PRS_TS_00108 PRS_TS_00109*/
        case ETHTSYN_CRC_IGNORED:
        case ETHTSYN_CRC_NOT_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
            port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
            port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
            port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
            port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
            ret                           = E_OK;
            break;
        }
        case ETHTSYN_CRC_VALIDATED:
        {
            ret = E_NOT_OK;
            break;
        }
        default: /* PRQA S 3120 */ /* VL_QAC_EmptyClause*/
        {
            break;
        }
        }
    }

    return ret;
}
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured userdata.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-77964
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVUserData)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        /* PRS_TS_00107 PRS_TS_00109 PRS_TS_00116*/
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcData[ETHTSYN_CRC_DATA_LENGTH];
            (void)IStdLib_MemCpy(&crcData[0u], &data[offset + 3u], 4u);
            crcData[4u] = PORT_TOCFGDOMIN(portId)
                              .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                              .GlobalTimeFollowUpDataIDListValue;
            uint8 crcDataResult = Crc_CalculateCRC8H2F(&crcData[0u], 5u, 0xFFu, TRUE);
            if (crcDataResult != data[offset + 7u])
            {
                ret = E_NOT_OK;
            }
            else
            {
                EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
                port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
                port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
                port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
                port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
                ret                           = E_OK;
            }
            break;
        }
#endif
        case ETHTSYN_CRC_IGNORED:
        {
            EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
            port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
            port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
            port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
            port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
            ret                           = E_OK;
            break;
        }
        case ETHTSYN_CRC_NOT_VALIDATED:
        {
            ret = E_NOT_OK;
            break;
        }
        default:
        {
            ret = E_NOT_OK;
            break;
        }
        }
    }
    return ret;
    /* PRQA S 3120 -- */
}
/**
 * @brief         the implementation of verifying the header of sub-tlv in the FU message.
 * @param[in]     data: raw data buffer to be validated.
 * @retval        E_OK: header validation passed.
 * @retval        E_NOT_OK: header validation failed.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-77963
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyTLVHeader(const uint8* data)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16         Tlv_Length;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Tlv_Length = (((uint16)(((uint16)(((data))[(2u)])) << 8u)) | ((uint16)((uint8)(((data))[3u]))));

    /* Verify header: */
    if (
        /* TLV type */
        (data[ETHTSYN_FUP_TLV_TYPE_1] == 0U) && (data[ETHTSYN_FUP_TLV_TYPE_0] == 3U) &&
        /* length field */
        (Tlv_Length > ETHTSYN_FUP_EXT_TLV_LENGTH_0) &&

        /* organizationId */
        (data[ETHTSYN_FUP_TLV_ORGID_2] == ETHTSYN_FUP_EXT_TLV_ORGID_2)
        && (data[ETHTSYN_FUP_TLV_ORGID_1] == ETHTSYN_FUP_EXT_TLV_ORGID_1)
        && (data[ETHTSYN_FUP_TLV_ORGID_0] == ETHTSYN_FUP_EXT_TLV_ORGID_0) &&
        /* organizationSubType */
        (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_2] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2)
        && (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_1] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1)
        && (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_0] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0))
    {
        ret = E_OK;
    }

    return ret;
    /* PRQA S 3120 -- */
}
/**
 * @brief         the implementation of validating the crc of sub-tlv in the FU message.
 * @param[in]     portId: the index of configured port.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     frameLength: the length of FU message.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-70698
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint16       portId,
    const uint8* data,
    uint16       frameLength,
    boolean*     isSyncToGatewayBitSet)
/* PRQA S 2889 -- */
{
    Std_ReturnType                        ret                   = E_OK;
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(portId);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    uint16 msgLength = (uint16)((uint16)data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_BYTE0] << ETHTSYN_BIT_SHIFT_8)
                       | data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_BYTE1];
    if ((msgLength != frameLength)
        || ((EthTSyn_VerifyTLVHeader(&data[ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR]) != E_OK)
            && (globalTimeSlaveCfgPtr->RxSubTLVStatus || globalTimeSlaveCfgPtr->RxSubTLVTime
                || globalTimeSlaveCfgPtr->RxSubTLVUserData)))
    {
        return E_NOT_OK;
    }
    *isSyncToGatewayBitSet = FALSE;
    uint8 datap            = ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR - 1u;
    while (datap < (msgLength - 1u))
    {
        switch ((uint8)data[datap + 1u])
        {
        case ETHTSYN_SUB_TLV_TYPE_TIME_SECURED:
        {
            ret = EthTSyn_VerifyTimeSecuredSubTlv(&data[0u], msgLength, datap, portId);
            break;
        }
        case ETHTSYN_SUB_TLV_TYPE_STATUS_NOT_SECURED:
        {
            ret = EthTSyn_VerifyStatusNotSecuredSubTlv(&data[0u], msgLength, datap, portId, isSyncToGatewayBitSet);
            break;
        }
        case ETHTSYN_SUB_TLV_TYPE_STATUS_SECURED:
        {
            ret = EthTSyn_VerifyStatusSecuredSubTlv(&data[0u], msgLength, datap, portId, isSyncToGatewayBitSet);
            break;
        }
        case ETHTSYN_SUB_TLV_TYPE_USERDATA_NOT_SECURED:
        {
            ret = EthTSyn_VerifyUserDataNotSecuredSubTlv(&data[0u], msgLength, datap, portId);
            break;
        }
        case ETHTSYN_SUB_TLV_TYPE_USERDATA_SECURED:
        {
            ret = EthTSyn_VerifyUserDataSecuredSubTlv(&data[0u], msgLength, datap, portId);
            break;
        }
        default:
        {
            ret = E_NOT_OK;
            break;
        }
        }
        if (ret != E_OK)
        {
            return ret;
        }
        datap += data[datap + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
    }
    return ret;
}
#endif

#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
/**
 * @brief         specifying dose the timebase associated with this port relate to the Ethernet hardware timestamp.
 * @param[in]     portId: the index of configured port.
 * @retval        TRUE:  the timebase associated with this port relate to the Ethernet hardware timestamp.
 * @retval        FALSE:  the timebase dose not associated with this port relate to the Ethernet hardware timestamp.
 * @synchronous   TRUE
 * @reentrancy    TRUE
 * @trace         CPD-70699
 */
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbmEthHWUsed(uint16 portId)
{
    return PORT_TOCFGPORT(portId).SynchronizedTimeBaseUseEthFreerunning;
}
#endif
/**
 * @brief         the implementation of optimizing the timestamp.
 * @param[inout]  timeStamp: timestamp to be optimize.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70700
 */
ETHTSYN_LOCAL void EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp)
{
    while (timestamp->Nanoseconds >= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        timestamp->Seconds++;
        if (timestamp->Seconds == 0ULL)
        {
            timestamp->SecondsHi++;
        }
        else
        {
            /* do nothing */
        }
        timestamp->Nanoseconds -= ETHTSYN_NANOSECOND_TO_SECOND;
    }
}
/**
 * @brief         The implementation of addition between two timestamps.
 * @param[in]     time1: timestamp to be added.
 * @param[in]     time2: timestamp to be added.
 * @return        the timestamp after addition between two timestamp.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70701
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2)
{
    uint64 second1 = (((uint64)time1.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time1.Seconds;
    uint64 second2 = (((uint64)time2.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time2.Seconds;
    uint64 second  = second1 + second2;

    EthTSyn_TimeStampType t;
    t.Nanoseconds = time1.Nanoseconds + time2.Nanoseconds;
    t.Seconds     = (uint32)second;
    t.SecondsHi   = (uint16)(second >> ETHTSYN_BIT_SHIFT_32);
    EthTSyn_timeoptimal(&t);
    return t;
}
/**
 * @brief         specifying if time1 is less than time2.
 * @param[in]     time1: global timestamp to be compared.
 * @param[in]     time2: locall time to be compared.
 * @retval        TRUE: time1 is less than time2.
 * @retval        FALSE: time1 is not less than time2.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70702
 */
ETHTSYN_LOCAL boolean EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2)
{
    boolean ret = TRUE;
    if (time2 <= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        if ((time1.Nanoseconds > time2) || (time1.Seconds > 0ULL) || (time1.SecondsHi > 0UL))
        {
            ret = FALSE;
        }
    }
    else
    {
        uint64 second = (((uint64)time1.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time1.Seconds;
        if (second > (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            ret = FALSE;
        }
        if (second == (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            if (time1.Nanoseconds > (time2 % ETHTSYN_NANOSECOND_TO_SECOND))
            {
                ret = FALSE;
            }
        }
    }
    return ret;
}
/**
 * @brief         The implementation of compasion between two global timestamps.
 * @param[in]     time1: timestamp to be compared.
 * @param[in]     time2: timestamp to be compared.
 * @return        The positive or negative result of subtracting two timestamps.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70703
 */
ETHTSYN_LOCAL sint32 EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2)
{
    uint64 t1Seconds = ((uint64)t1.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t1.Seconds;
    uint64 t2Seconds = ((uint64)t2.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t2.Seconds;
    sint32 sign;
    if (t1Seconds < t2Seconds)
    {
        sign = -1;
    }
    else if (t1Seconds == t2Seconds)
    {
        sint32 tdNanoseconds = (sint32)t1.Nanoseconds - (sint32)t2.Nanoseconds;
        if (tdNanoseconds < 0L)
        {
            sign = -1;
        }
        else if (tdNanoseconds == 0L)
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
/**
 * @brief         The implementation of subtraction  between two timestamps.
 * @param[in]     time1: timestamp to be subtracted.
 * @param[in]     time2: timestamp to be subtracted.
 * @param[out]    td: the timestamp after subtraction between two timestamp.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70704
 */
ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td)
{
    EthTSyn_TimeStampType t1Copy = t1;
    EthTSyn_TimeStampType t2Copy = t2;
    EthTSyn_timeoptimal(&t1Copy);
    EthTSyn_timeoptimal(&t2Copy);
    uint64 tdSeconds = (((uint64)t1.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t1.Seconds)
                       - (((uint64)t2.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t2.Seconds);
    uint32 tdUnsignedNanoseconds = 0U;
    if ((sint32)((sint32)t1.Nanoseconds - (sint32)t2.Nanoseconds) < 0LL)
    {
        tdUnsignedNanoseconds = ETHTSYN_NANOSECOND_TO_SECOND - (t2.Nanoseconds - t1.Nanoseconds);
        tdSeconds--;
    }
    else
    {
        tdUnsignedNanoseconds = t1.Nanoseconds - t2.Nanoseconds;
    }

    td->SecondsHi   = (uint16)((uint64)tdSeconds >> ETHTSYN_BIT_SHIFT_32);
    td->Seconds     = (uint32)tdSeconds;
    td->Nanoseconds = tdUnsignedNanoseconds;
}
/**
 * @brief         specifying if two portIdentity are the same.
 * @param[in]     portIdentity1: portIdentity to be compared.
 * @param[in]     portIdentity2: portIdentity to be compared.
 * @retval        TRUE: two portIdentity are the same.
 * @retval        FALSE: two portIdentity are not the same.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70708
 */
ETHTSYN_LOCAL boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentity1, const uint8* portIdentity2)
{
    return IStdLib_MemCmp(portIdentity1, portIdentity2, ETHTSYN_PORT_IDENTITY_LENGTH) == 0;
}
/**
 * @brief         The implementation of handling the port of master.
 * @param[in]     portIndex: Index of the configured Port.
 * @return        the type of message to be sended.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70849
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
/* PRQA S 6080 ++ */ /* VL_MTR_EthTSyn_STPTH */
ETHTSYN_LOCAL EthTSynMessageType EthTSyn_MainFunctionMaster(uint8 portIndex)
/* PRQA S 6080 -- */
/* PRQA S 2889 -- */
{
    EthTSynMessageType               syncMessageType = MSG_NONE;
    const EthTSyn_PortConfigCfgType* portCfgPtr      = &PORT_TOCFGPORT(portIndex);
    EthTSyn_PortType*                portPtr         = EthTSyn_ConfigData->RuntimeVariables[portIndex];

    uint64 globalTimeDebounceTime = portCfgPtr->GlobalTimeDebounceTime;
    if ((globalTimeDebounceTime > 0ULL) && (portPtr->SyncdebounceCounter > 0ULL))
    {
        --portPtr->SyncdebounceCounter;
    }

    uint64 globalTimeTxPeriod = portCfgPtr->PortRole.GlobalTimeMaster->GlobalTimeTxPeriod;
    if ((globalTimeTxPeriod > 0ULL) && (portPtr->SyncReqPeriod > 0ULL))
    {
        --portPtr->SyncReqPeriod;
    }

    if (portPtr->SyncdebounceCounter != 0ULL)
    {
        return syncMessageType;
    }

    if (portPtr->SyncStatusType == ETHTSYN_SEND_SYNC)
    {
        boolean isImmediate = FALSE;
        if (portCfgPtr->PortRole.GlobalTimeMaster->ImmediateTimeSync)
        {
            uint8 timeBaseUpdateCounter =
                StbM_GetTimeBaseUpdateCounter(PORT_TOCFGDOMIN(portIndex).SynchronizedTimeBaseRef);
            if (EthTSyn_ExpectedStatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE)
                && (timeBaseUpdateCounter != portPtr->TimeBaseUpdateCounter))
            {
                syncMessageType                = SYNC;
                isImmediate                    = TRUE;
                portPtr->TimeBaseUpdateCounter = timeBaseUpdateCounter;
            }
        }

        if (EthTSyn_ExpectedStatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE) && (globalTimeTxPeriod > 0ULL)
            && (portPtr->SyncReqPeriod == 0ULL))
        {
            syncMessageType = SYNC;
        }

        if (syncMessageType == SYNC)
        {
            portPtr->SyncReqPeriod =
                isImmediate ? portCfgPtr->PortRole.GlobalTimeMaster->CyclicMsgResumeTime : globalTimeTxPeriod;
            portPtr->SyncdebounceCounter = globalTimeDebounceTime;
        }
    }
    else
    {
        syncMessageType              = FOLLOW_UP;
        portPtr->SyncStatusType      = ETHTSYN_SEND_SYNC;
        portPtr->SyncdebounceCounter = globalTimeDebounceTime;
    }

    return syncMessageType;
}
/**
 * @brief         The implementation of initilizing the port status.
 * @param[inout]  port: port pointer.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70850
 */
ETHTSYN_LOCAL void EthTSyn_InitPortStatus(EthTSyn_PortType* portPtr)
{
    portPtr->EthtrcvLinkState   = ETHTRCV_LINK_STATE_DOWN;
    portPtr->PdelayReqPeriod    = 0u;
    portPtr->SyncReqPeriod      = 0u;
    portPtr->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
    portPtr->PdelayRXStatusType = ETHTSYN_WAITFOR_PDELAY_REQ;
    portPtr->SyncStatusType     = (ETHTSYN_MASTER == portPtr->PortRoleType) ? ETHTSYN_SEND_SYNC : ETHTSYN_WAITFOR_SYNC;
    portPtr->FirstlyReceivedSyncReq      = TRUE;
    portPtr->SyncSequenceId              = ETHTSYN_MAX_SEQUENCEID;
    portPtr->PdelayReqSequenceId         = ETHTSYN_MAX_SEQUENCEID;
    portPtr->ReceivedPdelayReqSequenceId = 0u;
    portPtr->SyncdebounceCounter         = 0u;
    portPtr->PdelaydebounceCounter       = 0u;
}

#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief         The implementation of validating pointer,if the pointer is NULL_PTR.
 * @param[in]     apiId: the id of application.
 * @param[in]     pointer: the pointer to be validated.
 * @retval        TRUE: the pointer is not NULL_PTR.
 * @retval        FALSE: the pointer is NULL_PTR.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70851
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = pointer != NULL_PTR;
    if (!valid)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_PARAM_POINTER);
    }
    return valid;
}
/**
 * @brief         The implementation of validating EthTSyn_Init.
 * @param[in]     configPtr: the pointer of configuration.
 * @retval        TRUE: validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70852
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr)
{
    boolean valid = !EthTSyn_Initialized[EthTSyn_GetPartitionIndex()];
    if (!valid)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_INIT, ETHTSYN_E_ALREADY_INITIALIZED);
    }
    else
    {
        valid = configPtr != NULL_PTR;
        if (!valid)
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_INIT, ETHTSYN_E_INIT_FAILED);
        }
    }
    return valid;
}
/**
 * @brief         The implementation of validation whether it has been initialized.
 * @param[in]     apiId: the ID of appication.
 * @retval        TRUE: validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrancy    FALSE
 * @trace         CPD-70988
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateInitStatus(uint8 apiId)
{
    boolean valid = EthTSyn_Initialized[EthTSyn_GetPartitionIndex()];
    if (!valid)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_UNINIT);
    }
    return valid;
}
/**
 * @brief        find the index of configured port related to ctrlIdx.
 * @param[in]    ctrlIdx   Identification of the controllor.
 * @param[out]   portIdx   the index of configured port related to ctrlIdx.
 * @return       the runtime variable(EthTSyn_Port),which is related to the ctrlIdx.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-71029
 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPort(uint8 ctrlIdx, uint8* portIdx)
{
    EthTSyn_PortType* port = NULL_PTR;

    uint8 portIndex = 0u;
    for (portIndex = 0u; portIndex < EthTSyn_ConfigData->PortNum; ++portIndex)
    {
        if (EthTSyn_ConfigData->RuntimeVariables[portIndex]->CtrlId == ctrlIdx)
        {
            port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            break;
        }
    }
    if (portIdx != NULL_PTR)
    {
        *portIdx = portIndex;
    }
    else
    {
        ETHTSYN_UNUSED(portIdx);
    }
    return port;
}
/**
 * @brief        the implementation of checking,if the ctrlIdx is valid.
 * @param[in]    apiId   the index of application.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @retval        TRUE: validation passed.
 * @retval        FALSE: validation failed.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-71030
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx)
{
    boolean valid = FALSE;
    if (EthTSyn_ValidateInitStatus(apiId))
    {
        uint8 portIdx = 0u;
        (void)EthTSyn_FindPort(ctrlIdx, &portIdx);
        if (portIdx >= EthTSyn_ConfigData->PortNum)
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_CTRL_IDX);
        }
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
        else if (PORT_TOCFGPORT(portIdx).PortReferrencedPartitionId != GetApplicationID())
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_INVALID_PARTITION_CONTEXT);
        }
#endif
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}
/**
 * @brief        the implementation of validating EthTSyn_Rxindication.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @param[in]    pointer   the pointer to be validated.
 * @retval       TRUE: validation passed.
 * @retval       FALSE: validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71031
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer)
{
    return EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_RXINDICATION, ctrlIdx)
           && EthTSyn_ValidatePointer(ETHTSYN_SID_RXINDICATION, pointer);
}
/**
 * @brief        the implementation of validating TransmissionMode.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @param[in]    Mode   the Mode to be validated.
 * @retval       TRUE: validation passed.
 * @retval       FALSE: validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71032
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode)
{
    boolean valid = EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_SETTRANSMISSIONMODE, ctrlIdx);
    if (valid && ((uint8)mode > ETHTSYN_TX_ON_MAX_VALUE))
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_PARAM);
        valid = FALSE;
    }
    return valid;
}
/**
 * @brief        the implementation of validating of invalid partition context
 * @param[in]    apiId  ID of API service in which error is detected
 * @retval       TRUE   validation passed.
 * @retval       FALSE  validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71033
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
ETHTSYN_LOCAL boolean EthTSyn_ValidatePartitionContext(uint8 apiId)
/* PRQA S 2889 -- */
{
    if (EthTSyn_GetPartitionIndex() >= EthTSyn_ConfigData->PartitionNum)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    return TRUE;
}
#endif
/**
 * @brief        the implementation of getting the index of partition.
 * @return       the index of partition.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71034
 */
ETHTSYN_LOCAL uint8 EthTSyn_GetPartitionIndex(void)
{
    uint8 partitionIndex = 0u;
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType application = GetApplicationID();
    for (; partitionIndex < EthTSyn_ConfigData->PartitionNum; ++partitionIndex)
    {
        if (EthTSyn_ConfigData->OsApplications[partitionIndex] == application)
        {
            break;
        }
    }
#endif
    return partitionIndex;
}
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
/**
 * @brief        the implementation of getting the hard ingress timestamp if EthTSynHardwareTimestampSupport is TRUE.
 * @param[in]    ctrlIdx index of controllor.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73917
 */
ETHTSYN_LOCAL boolean EthTSyn_GetHardIngressTime(uint8 ctrlIdx, const uint8* dataPtr, EthTSyn_TimeStampType* tVnow)
{
    boolean               isValidIngressTime = TRUE;
    Eth_TimeStampQualType timeQualPtr        = ETH_INVALID;
    Eth_TimeStampType     timeStampPtr;
    Std_ReturnType ret = EthIf_GetIngressTimeStamp(ctrlIdx, (const Eth_DataType*)dataPtr, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        tVnow->Nanoseconds = timeStampPtr.nanoseconds;
        tVnow->Seconds     = timeStampPtr.seconds;
        tVnow->SecondsHi   = timeStampPtr.secondsHi;
    }
    else
    {
        isValidIngressTime = FALSE;
    }
    return isValidIngressTime;
}
#else
/**
 * @brief        the implementation of getting the soft ingress timestamp if EthTSynHardwareTimestampSupport is FALSE.
 * @param[in]    port port pointer.
 * @param[in]    tVnow ingress time.
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-74327
 */
ETHTSYN_LOCAL boolean EthTSyn_GetSoftIngressTime(const EthTSyn_PortType* port, EthTSyn_TimeStampType* tVnow)
{
    boolean                   isValidIngressTime = FALSE;
    StbM_VirtualLocalTimeType VirtualLocalTime;
    if (StbM_GetCurrentVirtualLocalTime(
            PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef,
            &VirtualLocalTime)
        == E_OK)
    {
        *tVnow             = EthTSyn_VTToGT(VirtualLocalTime);
        isValidIngressTime = TRUE;
    }
    return isValidIngressTime;
}
#endif
/**
 * @brief        the implementation of getting the T2vlt.
 * @param[in]    port port pointer.
 * @param[in]    t1vlt Ingress timestamp of SYNC message as captured by HW in the Ethernet controller or by SW in
 * EthTSyn_RxIndication.
 * @param[out]   t2vlt Virtual Local Time component of the Rx Time Tuple (equivalent to TV_Rxin the StbM)
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73917
 */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
ETHTSYN_LOCAL boolean
    EthTSyn_GetSyncT2vlt(const EthTSyn_PortType* port, EthTSyn_TimeStampType t1vlt, EthTSyn_TimeStampType* t2vlt)
/* PRQA S 6070 -- */
{
    boolean isValidT2vlt = TRUE;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
    /* SWS_EthTSyn_00190 */
    if (!EthTSyn_StbmEthHWUsed(port->PortIndexInPortList))
    {
        EthTSyn_TimeStampType     t3vlt;
        EthTSyn_TimeStampType     t4vlt;
        StbM_VirtualLocalTimeType virtualLocalTime;
        TimeTupleType             timeTuple;
        timeTuple.timeQuality = TSQ_INVALID;
        SchM_Enter_EthTSyn_Context();
        Std_ReturnType ret = EthIf_GetCurrentTimeTuple(port->CtrlId, 0u, &timeTuple);
        if ((ret == E_OK) && (timeTuple.timeQuality == TSQ_VALID)
            && (StbM_GetCurrentVirtualLocalTime(
                    PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef,
                    &virtualLocalTime)
                == E_OK))
        {
            t3vlt.Nanoseconds = timeTuple.timestampClockValue.nanoseconds;
            t3vlt.Seconds     = timeTuple.timestampClockValue.seconds;
            t3vlt.SecondsHi   = timeTuple.timestampClockValue.secondsHi;
            t4vlt             = EthTSyn_VTToGT(virtualLocalTime);
        }
        else
        {
            isValidT2vlt = FALSE;
        }
        SchM_Exit_EthTSyn_Context();
        if (isValidT2vlt)
        {
            EthTSyn_TimeStampType tSrd;
            /* SWS_EthTSyn_00190    tsrd = t3vlt-t1vlt*/
            EthTSyn_TimeStampSub(t3vlt, t1vlt, &tSrd);
            if ((sint32)EthTSyn_TimeStampCmp(t4vlt, tSrd) >= 0)
            {
                /* SWS_EthTSyn_00190    t2vlt = t4vlt-tsrd*/
                EthTSyn_TimeStampSub(t4vlt, tSrd, t2vlt);
            }
            /* t4vlt < tSrd,The probability of this scenario occurring is very low, as it can only happen when a
            Sync message is received immediately after power-on or following STBM module initialization. If such
            a situation occurs, the current packet should be discarded, which will not affect subsequent synchronization
            processes */
            else
            {
                isValidT2vlt = FALSE;
            }
        }
    }
    else
#endif
    {
        /* SWS_EthTSyn_00138 SWS_EthTSyn_00180 */
        *t2vlt = t1vlt;
        ETHTSYN_UNUSED(port);
    }

    return isValidT2vlt;
}
/**
 * @brief        the implementation of handling the received Sync message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @param[in]    isValidIngressTime specifying if ingress timestamp has been captured successfully.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73918
 */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
ETHTSYN_LOCAL void EthTSyn_RxHandleSyncMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime)
/* PRQA S 6070 -- */
{
    if (port->PortRoleType == ETHTSYN_SLAVE)
    {
        const uint8* masterPortIdentity = &dataPtr[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET];
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
        if ((port->SyncStatusType == ETHTSYN_WAITFOR_SYNC)
            && !EthTSyn_IsPortIdentityEqual(port->PortIdentity, masterPortIdentity) && !port->FirstlyReceivedSyncReq)
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
            const EthTSyn_PortConfigCfgType* portCfg = &PORT_TOCFGPORT(port->PortIndexInPortList);
            StbM_TimeBaseStatusType          timeBaseStatus;
            (void)StbM_GetTimeBaseStatus(
                PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef,
                &timeBaseStatus);

            const uint16 sequenceCounterJumpWidth =
                portCfg->PortRole.GlobalTimeSlave->GlobalTimeSequenceCounterJumpWidth;
            uint16 sequenceId =
                (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                         + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
            uint16 sequenceCounterJump = sequenceId - port->ReceivedSyncSequenceId;
            if ((sequenceCounterJump > sequenceCounterJumpWidth) && (sequenceCounterJumpWidth != 0u)
                && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u))
            {
                port->SCHysteresisCount = 0u;
            }

            if (isValidIngressTime
                && (((sequenceCounterJump > 0u) && (sequenceCounterJump <= sequenceCounterJumpWidth))
                    || (sequenceCounterJumpWidth == 0u) || port->FirstlyReceivedSyncReq
                    || ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u)))
            {
                EthTSyn_TimeStampType t2vlt;
                if (EthTSyn_GetSyncT2vlt(port, tVnow, &t2vlt))
                {
                    port->SyncStatusType         = ETHTSYN_WAITFOR_FOLLOWUP;
                    port->FirstlyReceivedSyncReq = FALSE;
                    /* tVnow represents the local time after transmission of sync*/
                    port->T1vlt = t2vlt;
                    (void)IStdLib_MemCpy(port->PortIdentity, masterPortIdentity, ETHTSYN_PORT_IDENTITY_LENGTH);
                    port->ReceivedSyncSequenceId = sequenceId;
                }
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
/**
 * @brief        the implementation of handling the received Follow Up message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @param[in]    isValidIngressTime  specifying if ingress timestamp has been captured successfully.
 * @param[in]    lenByte the length of received data.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73919
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
/* PRQA S 6080 ++ */ /* VL_MTR_EthTSyn_STPTH */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
ETHTSYN_LOCAL void EthTSyn_RxHandleFollowUpMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime,
    uint16                lenByte)
/* PRQA S 6070 -- */
/* PRQA S 6080 -- */
/* PRQA S 2889 -- */
{
    const EthTSyn_PortConfigCfgType*      portCfg               = &PORT_TOCFGPORT(port->PortIndexInPortList);
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = portCfg->PortRole.GlobalTimeSlave;
    const uint8*                          masterPortIdentity    = &dataPtr[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET];
    uint16                                sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    EthTSyn_TimeStampType   tg             = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
    StbM_TimeBaseStatusType timeBaseStatus = 0u;
    if ((port->SyncStatusType == ETHTSYN_WAITFOR_FOLLOWUP)
        && EthTSyn_IsPortIdentityEqual(port->PortIdentity, masterPortIdentity)
        && (sequenceId == port->ReceivedSyncSequenceId) && (globalTimeSlaveCfgPtr != NULL_PTR))
    {
        port->SyncStatusType = ETHTSYN_WAITFOR_SYNC;
        if (!isValidIngressTime || (tg.Nanoseconds > ETHTSYN_NANOSECOND_TO_SECOND))
        {
            return;
        }
        uint64                timeout = globalTimeSlaveCfgPtr->GlobalTimeFollowUpTimeout;
        EthTSyn_TimeStampType td;
        EthTSyn_TimeStampSub(tVnow, port->T1vlt, &td);
        if ((timeout != 0ULL) && !EthTSyn_timeless(td, timeout))
        {
            return;
        }

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        boolean        isSyncToGatewayBitSet = FALSE;
        Std_ReturnType ret =
            EthTSyn_TlvScanAndCRCValidation(port->PortIndexInPortList, &dataPtr[0u], lenByte, &isSyncToGatewayBitSet);
        if (ret != E_OK)
        {
            return;
        }

        if (isSyncToGatewayBitSet)
        {
            timeBaseStatus = STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
        }
#else
        ETHTSYN_UNUSED(lenByte);
#endif
        if ((port->SCHysteresisCount <= globalTimeSlaveCfgPtr->GlobalTimeSequenceCounterHysteresis))
        {
            port->SCHysteresisCount++;
        }
        if ((port->SCHysteresisCount > globalTimeSlaveCfgPtr->GlobalTimeSequenceCounterHysteresis))
        {
            StbM_TimeTupleType syncTimetuple;
            syncTimetuple.timeBaseStatus = timeBaseStatus;
            if (PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef
                < ETHTSYN_SYNCHRONIZE_TIMEBASE_MAX_INDEX)
            {
                const StbM_UserDataType* userDataPtr = NULL_PTR;
                EthTSyn_TimeStampType    correction =
                    EthTSyn_GetCorrectionField(&(dataPtr[ETHTSYN_CORRECTIONFIELD_BYTE_INDEX]));
                tg = EthTSyn_timeadd(EthTSyn_timeadd(tg, port->Pdelay), correction);
                EthTSyn_timeoptimal(&tg);
                syncTimetuple.globalTime.nanoseconds = tg.Nanoseconds;
                syncTimetuple.globalTime.seconds     = tg.Seconds;
                syncTimetuple.globalTime.secondsHi   = tg.SecondsHi;
                StbM_MeasurementType measureDataPtr;
                measureDataPtr.pathDelay = port->Pdelay.Nanoseconds;
                uint64 tv                = port->T1vlt.Nanoseconds
                            + (((uint64)port->T1vlt.Seconds + (((uint64)port->T1vlt.SecondsHi) << ETHTSYN_BIT_SHIFT_32))
                               * ETHTSYN_NANOSECOND_TO_SECOND);
                syncTimetuple.virtualLocalTime.nanosecondsHi = (uint32)(tv >> ETHTSYN_BIT_SHIFT_32);
                syncTimetuple.virtualLocalTime.nanosecondsLo = (uint32)tv;
#if ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF
                if (PORT_TOCFGPORT(port->PortIndexInPortList).PortRole.GlobalTimeSlave->RxSubTLVUserData)
                {
                    userDataPtr = &port->UserData;
                }
#endif
                (void)StbM_BusSetGlobalTime(
                    PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef,
                    &syncTimetuple,
                    userDataPtr,
                    &measureDataPtr);
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
/**
 * @brief        the implementation of handling the received Pdelay Response message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @param[in]    isValidIngressTime  specifying if ingress timestamp has been captured successfully.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73920
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
ETHTSYN_LOCAL void EthTSyn_RxHandlePdelayResPonseMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime)
/* PRQA S 2889 -- */
{
    const EthTSyn_PortConfigCfgType* portCfg = &PORT_TOCFGPORT(port->PortIndexInPortList);
    if (port->PdelayTXStatusType != ETHTSYN_WAITFOR_PDELAY_RESP)
    {
        return;
    }

    /* CODE_EthTSyn_00001 */
    uint16 sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    const uint8* requestingPortIdentity = &dataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
    if (!isValidIngressTime || (sequenceId != port->PdelayReqSequenceId)
        || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, port->ThisPortIdentity))
    {
        port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        return;
    }

    uint64                timeout = portCfg->PdelayConfig->PdelayRespAndRespFollowUpTimeout;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(tVnow, port->T1, &td);
    if ((timeout != 0ULL) && !EthTSyn_timeless(td, timeout))
    {
        port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        return;
    }

    port->T2                 = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
    port->T4                 = tVnow;
    port->PdelayTXStatusType = ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP;
}
/**
 * @brief        the implementation of handling the received Pdelay Response Follow Up message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @param[in]    isValidIngressTime  specifying if ingress timestamp has been captured successfully.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73921
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
ETHTSYN_LOCAL void EthTSyn_RxHandlePdelayResPonseFUMessage(
    EthTSyn_PortType*     port,
    const uint8*          dataPtr,
    EthTSyn_TimeStampType tVnow,
    boolean               isValidIngressTime)
/* PRQA S 2889 -- */
{
    const EthTSyn_PortConfigCfgType* portCfg = &PORT_TOCFGPORT(port->PortIndexInPortList);
    if (port->PdelayTXStatusType != ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP)
    {
        return;
    }
    port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
    uint16 sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    /* CODE_EthTSyn_00002 */
    const uint8* requestingPortIdentity = &dataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
    if (!isValidIngressTime || (sequenceId != port->PdelayReqSequenceId)
        || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, port->ThisPortIdentity))
    {
        return;
    }

    uint64                timeout = portCfg->PdelayConfig->PdelayRespAndRespFollowUpTimeout;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(tVnow, port->T4, &td);
    if ((timeout != 0ULL) && !EthTSyn_timeless(td, timeout))
    {
        return;
    }
    port->T3 = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
    /* pdelay = ((t4-t1)-(t3-t2))/2 */
    EthTSyn_TimeStampType td41;
    EthTSyn_TimeStampSub(port->T4, port->T1, &td41);

    EthTSyn_TimeStampType td32;
    EthTSyn_TimeStampSub(port->T3, port->T2, &td32);

    if ((sint32)EthTSyn_TimeStampCmp(td41, td32) >= 0)
    {
        EthTSyn_TimeStampType pdelay;
        EthTSyn_TimeStampSub(td41, td32, &pdelay);
        pdelay.Nanoseconds /= ETHTSYN_DELAY_HALF_PERIOD;
        uint32 s = pdelay.Seconds;
        pdelay.Seconds /= ETHTSYN_DELAY_HALF_PERIOD;
        if ((s % ETHTSYN_DELAY_HALF_PERIOD) > 0UL)
        {
            pdelay.Nanoseconds += ETHTSYN_NANOSECOND_TO_SECOND / ETHTSYN_DELAY_HALF_PERIOD;
            if (pdelay.Nanoseconds >= ETHTSYN_NANOSECOND_TO_SECOND)
            {
                pdelay.Nanoseconds -= ETHTSYN_NANOSECOND_TO_SECOND;
                pdelay.Seconds++;
            }
        }
        uint64 PdelayThreshold = portCfg->PdelayConfig->PdelayLatencyThreshold;
        if ((PdelayThreshold == 0ULL) || EthTSyn_timeless(pdelay, PdelayThreshold))
        {
            port->Pdelay = pdelay;
        }
    }
}
/**
 * @brief        the implementation of caculating the original timestamp to be sended in the Follow Up message.
 * @param[in]    ctrlIdx the index of controllor.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    isValidEgressTime  specifying if ingress timestamp has been captured successfully.
 * @param[in]    ingressTimeStamp the ingress timestamp of SYNC message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73923
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
ETHTSYN_LOCAL void EthTSyn_OriginalTimeStampCaculation(
    uint8                 ctrlIdx,
    EthTSyn_PortType*     port,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType ingressTimeStamp)
/* PRQA S 2889 -- */
/* PRQA S 6070 -- */
{
    if (!isValidEgressTime)
    {
        port->SyncStatusType = ETHTSYN_SEND_SYNC;
        return;
    }
    EthTSyn_TimeStampType t0    = port->SyncEgressTuple.T0;
    EthTSyn_TimeStampType t0vlt = port->SyncEgressTuple.T0vlt;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    /* SWS_EthTSyn_00189 */
    EthTSyn_TimeStampType t4vlt = ingressTimeStamp;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(t4vlt, t0vlt, &td); /* preciseOriginTimestamp = T0 + t4vlt - t0vlt */
    port->PreciseOriginTimestamp = EthTSyn_timeadd(td, t0);
    ETHTSYN_UNUSED(ctrlIdx);

#else
    if (EthTSyn_StbmEthHWUsed(port->PortIndexInPortList))
    {
        /* SWS_EthTSyn_00188 */
        EthTSyn_TimeStampType td;
        EthTSyn_TimeStampSub(ingressTimeStamp, t0vlt, &td);
        port->PreciseOriginTimestamp = EthTSyn_timeadd(td, t0); /* preciseOriginTimestamp = T0 + t2vlt - t0vlt */
    }
    else
    {
        /* SWS_EthTSyn_00017 */
        EthTSyn_TimeStampType t3vlt = {0u, 0u, 0u};
        EthTSyn_TimeStampType t4vlt;
        TimeTupleType         timeTuple;
        timeTuple.timeQuality = TSQ_INVALID;
        SchM_Enter_EthTSyn_Context();
        Std_ReturnType ret = EthIf_GetCurrentTimeTuple(ctrlIdx, 0u, &timeTuple);
        if (!((ret == E_OK) && (timeTuple.timeQuality == TSQ_VALID)))
        {
            SchM_Exit_EthTSyn_Context();
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }

        t3vlt.Nanoseconds = timeTuple.timestampClockValue.nanoseconds;
        t3vlt.Seconds     = timeTuple.timestampClockValue.seconds;
        t3vlt.SecondsHi   = timeTuple.timestampClockValue.secondsHi;
        StbM_VirtualLocalTimeType VirtualLocalTime;
        if (StbM_GetCurrentVirtualLocalTime(
                PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef,
                &VirtualLocalTime)
            != E_OK)
        {
            SchM_Exit_EthTSyn_Context();
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }
        SchM_Exit_EthTSyn_Context();
        t4vlt = EthTSyn_VTToGT(VirtualLocalTime);
        EthTSyn_TimeStampType td32vlt;
        EthTSyn_TimeStampSub(t3vlt, ingressTimeStamp, &td32vlt); /* td32vlt = t3vlt - t2vlt */
        EthTSyn_TimeStampType td40vlt;
        EthTSyn_TimeStampSub(t4vlt, t0vlt, &td40vlt); /* td40vlt = t4vlt - t0vlt */
        if ((sint32)EthTSyn_TimeStampCmp(td40vlt, td32vlt) < 0)
        {
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }

        EthTSyn_TimeStampType tdTemp;
        EthTSyn_TimeStampSub(td40vlt, td32vlt, &tdTemp);
        /** [SWS_EthTSyn_00017]
         *  preciseOriginTimestamp = T0 - (T3VLT - T2VLT) + (T4VLT - T0VLT)
         */
        port->PreciseOriginTimestamp = EthTSyn_timeadd(tdTemp, t0);
    }
#endif
    port->SyncStatusType = ETHTSYN_SEND_FOLLOWUP;
}
/**
 * @brief        the implementation of packing the Sync message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @param[in]    hasCapturedEgress  specifying if T0 and t0vlt have been captured successfully.
 * @return       the length of Sync message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73924
 */
ETHTSYN_LOCAL uint16 EthTSyn_SyncMessagePack(EthTSyn_PortType* port, uint8* data, boolean* hasCapturedEgress)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    /* SWS_EthTSyn_00202 */
    StbM_TimeStampType        globalTime;
    StbM_VirtualLocalTimeType localTime;
    StbM_UserDataType         userData;
    uint16                    dataLenth = 0U;
    EthTSyn_TimeStampType     t0        = {0u, 0u, 0u};
    StbM_TimeTupleType        timeTuple;
    Std_ReturnType            ret =
        StbM_GetCurrentTime(PORT_TOCFGDOMIN(port->PortIndexInPortList).SynchronizedTimeBaseRef, &timeTuple, &userData);
    globalTime = timeTuple.globalTime;
    localTime  = timeTuple.virtualLocalTime;
    if (ret == E_OK)
    {
        t0.Nanoseconds              = globalTime.nanoseconds;
        t0.Seconds                  = globalTime.seconds;
        t0.SecondsHi                = globalTime.secondsHi;
        EthTSyn_TimeStampType t0vlt = {0u, 0u, 0u};
        t0vlt                       = EthTSyn_VTToGT(localTime);
        port->SyncEgressTuple.T0vlt = t0vlt;
        port->SyncEgressTuple.T0    = t0;
        port->UserData              = userData;
        /*messageLength */
        data[2] = 0u;
        data[3] = 44U;
        /*flags Octet 0*/
        data[6] = 0x02u;
        data[7] = 0x08u;
        /*sequenceId*/
        if (port->SyncSequenceId < ETHTSYN_MAX_SEQUENCEID)
        {
            port->SyncSequenceId++;
        }
        else
        {
            port->SyncSequenceId = 0u;
        }
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->SyncSequenceId >> ETHTSYN_BIT_SHIFT_8);
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->SyncSequenceId;
        /*control*/
        data[32] = 0x0u;
        /*logMessageInterval in Seconds*/
        data[33] = (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).LogMessageInterval);
        /*reserved */
        for (uint8 i = 0u; i < 10u; i++)
        {
            data[34u + i] = 0x00u;
        }
        dataLenth = 44U;
    }
    else
    {
        *hasCapturedEgress = FALSE;
    }
    return dataLenth;
    /* PRQA S 3120 -- */
}
/**
 * @brief        the implementation of packing the Follow Up message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @return       the length of Follow Up message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73925
 */
/* PRQA S 6020, 6050 ++ */ /* VL_MTR_EthTSyn_STLIN, VL_MTR_EthTSyn_STST3 */
ETHTSYN_LOCAL uint16 EthTSyn_FollowUpMessagePack(const EthTSyn_PortType* port, uint8* data)
/* PRQA S 6020, 6050 -- */
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
    uint8 crcdata[ETHTSYN_CRC_DATA_TO_BE_CACULATED_LENGTH];
#endif
    uint16 dataLenth = 0U;
    /*messageLength */
    data[2] = 0u;
    data[3] = 76U;
    /*flags Octet 0*/
    data[6] = 0x00u;
    data[7] = 0x08u;
    /*sequenceId*/
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->SyncSequenceId >> ETHTSYN_BIT_SHIFT_8);
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->SyncSequenceId;
    /*control*/
    data[32] = 0x02u;
    /*logMessageInterval in Seconds*/
    data[33] = (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).LogMessageInterval);
    /*preciseOriginTimestamp */
    EthTSyn_TimeToData(port->PreciseOriginTimestamp, &data[34u]);
    /*IEEE Follow_Up information TLV */
    /*tlvType */
    data[44] = 0x00u;
    data[45] = 0x03u;
    /*lengthField*/
    data[46] = 0x00u;
    data[47] = 28u;
    /*organizationId*/
    data[48] = 0x00u;
    data[49] = 0x80u;
    data[50] = 0xC2u;
    /*organizationSubType*/
    data[51] = 0x00u;
    data[52] = 0x00u;
    data[53] = 0x01u;
    /* cumulativeScaledRateOffset*/
    data[54] =
        (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CumulativeScaledRateOffset
                >> ETHTSYN_BIT_SHIFT_24);
    data[55] =
        (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CumulativeScaledRateOffset
                >> ETHTSYN_BIT_SHIFT_16);
    data[56] =
        (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CumulativeScaledRateOffset
                >> ETHTSYN_BIT_SHIFT_8);
    data[57] =
        (uint8)((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CumulativeScaledRateOffset);
    /* gmTimeBaseIndicator*/
    /* lastGmPhaseChange */
    /* scaledLastGmFreqChange*/
    for (uint8 i = 0u; i < 18u; i++)
    {
        data[58u + i] = 0x00u;
    }
    dataLenth = 76U;
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    /* Autosar Follow_Up information TLV */
    /*tlvType */
    data[76] = 0x00u;
    data[77] = 0x03u;
    /*lengthField*/
    data[78] = 0x00u;
    data[79] = ETHTSYN_FUP_EXT_TLV_LENGTH_0;
    /*organizationId*/
    data[80] = ETHTSYN_FUP_EXT_TLV_ORGID_2;
    data[81] = ETHTSYN_FUP_EXT_TLV_ORGID_1;
    data[82] = ETHTSYN_FUP_EXT_TLV_ORGID_0;
    /*organizationSubType*/
    data[83] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2;
    data[84] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1;
    data[85] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0;
    data[3] += 10u;
    dataLenth += 10U;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
    uint8 DataID = PORT_TOCFGDOMIN(port->PortIndexInPortList)
                       .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[(port->SyncSequenceId) % 16u]
                       .GlobalTimeFollowUpDataIDListValue;
#endif
    /* Sub-TLV: Status */
    if ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).TLVFollowUpStatusSubTLV)
    {
        data[79] += 4u;
        /* Length */
        data[dataLenth + 1u] = 0x02u;
        /* Status */
        if (EthTSyn_ExpectedStatusGet(port->PortIndexInPortList, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
        {
            data[dataLenth + 2u] = 0x01u;
        }
        else
        {
            data[dataLenth + 2u] = 0x00u;
        }
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        if ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).GlobalTimeTxCrcSecured
            == ETHTSYN_CRC_SUPPORTED)
        {
            /* Type */
            data[dataLenth]      = 0x50u;
            crcdata[0u]          = data[dataLenth + 2u];
            crcdata[1u]          = DataID;
            data[dataLenth + 3u] = Crc_CalculateCRC8H2F(&crcdata[0u], 2u, 0xFFu, TRUE);
        }
        else
#endif
        {
            /* Type */
            data[dataLenth]      = 0x51u;
            data[dataLenth + 3u] = 0x00u;
        }
        data[3] += 4u;
        dataLenth += 4U;
    }
    /* Sub-TLV: UserData Secured */
    StbM_UserDataType userData = port->UserData;
    if ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).TLVFollowUpUserDataSubTLV
        && (userData.userDataLength > 0u))
    {
        data[79] += 7u;
        /* Length */
        data[dataLenth + 1u] = 0x05u;
        data[dataLenth + 2u] = userData.userDataLength;
        data[dataLenth + 3u] = userData.userByte0;
        data[dataLenth + 4u] = userData.userByte1;
        data[dataLenth + 5u] = userData.userByte2;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        if ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).GlobalTimeTxCrcSecured
            == ETHTSYN_CRC_SUPPORTED)
        {
            /* Type */
            data[dataLenth] = 0x60u;
            crcdata[4]      = DataID;
            (void)IStdLib_MemCpy(&crcdata[0u], &data[dataLenth + 2u], 4u);
            data[dataLenth + 6u] = Crc_CalculateCRC8H2F(&crcdata[0u], 5u, 0xFFu, TRUE);
        }
        else
#endif
        {
            /* Type */
            data[dataLenth]      = 0x61u;
            data[dataLenth + 6u] = 0x00u;
        }
        data[3] += 7u;
        dataLenth += 7U;
    }
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
    /* Sub-TLV: Time Secured */
    if ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).TLVFollowUpTimeSubTLV
        && ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CrcTimeFlagsTxSecured != NULL_PTR)
        && ((*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).GlobalTimeTxCrcSecured
            == ETHTSYN_CRC_SUPPORTED))
    {
        data[79] += 5U;
        /* Type */
        data[dataLenth] = ETHTSYN_SUB_TLV_TYPE_TIME_SECURED;
        /* Length */
        data[dataLenth + 1u] = ETHTSYN_FUP_SUBTLV_TIME_LENGTH;

        data[3] += 5u;

        /* CRC_Time_Flags */
        data[dataLenth + 2u] = (*(PORT_TOCFGPORT(port->PortIndexInPortList)).PortRole.GlobalTimeMaster).CRCTimeFlags;
        uint8 crcTime0       = 0u;
        uint8 crcTime1       = 0u;
        EthTSyn_CalculateTimeSecuredCrc(port->PortIndexInPortList, data, dataLenth, &crcTime0, &crcTime1);
        /* CRC_Time_0 */
        data[dataLenth + ETHTSYN_CRC_TIME0_OFFSET] = crcTime0;
        /* CRC_Time_1 */
        data[dataLenth + ETHTSYN_CRC_TIME1_OFFSET] = crcTime1;

        dataLenth += 5U;
    }
#endif
    if (data[79] == 6u)
    {
        for (uint8 i = 0u; i < 10u; i++)
        {
            data[76u + i] = 0x00u;
        }
        data[3] -= 10u;
        dataLenth -= 10u;
    }
#endif
    return dataLenth;
    /* PRQA S 3120 -- */
}
/**
 * @brief        the implementation of packing the Req message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @return       the length of Req message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73926
 */
ETHTSYN_LOCAL uint16 EthTSyn_ReqMessagePack(EthTSyn_PortType* port, uint8* data)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 dataLenth = 0U;
    /*messageLength */
    data[2] = 0u;
    data[3] = 54u;
    /*flags Octet 0*/
    data[6] = 0x00u;
    data[7] = 0x08u;
    /*sequenceId*/
    if (port->PdelayReqSequenceId < 0xFFFFu)
    {
        port->PdelayReqSequenceId++;
    }
    else
    {
        port->PdelayReqSequenceId = 0u;
    }
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->PdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->PdelayReqSequenceId;
    /*control*/
    data[32] = 0x5u;
    /*logMessageInterval in Seconds*/
    data[33] = (uint8)PORT_TOCFGPORT(port->PortIndexInPortList).PdelayConfig->LogMessageInterval;
    /*reserved */
    for (uint8 i = 0u; i < 20u; i++)
    {
        data[34u + i] = 0x00u;
    }
    dataLenth = 54u;
    /* PRQA S 3120 -- */
    return dataLenth;
}
/**
 * @brief        the implementation of packing the public information in a  gptp message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @param[in]    ethTSynMessage the type of gptp message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73927
 */
ETHTSYN_LOCAL void EthTSyn_PublicInformationPack(const EthTSyn_PortType* port, uint8* data, uint8 ethTSynMessage)
{
    /* SWS_EthTSyn_00162 */
    EthTSyn_GlobalTimeMasterCfgType masterCfg = {0};
    if (PORT_TOCFGPORT(port->PortIndexInPortList).PortRole.GlobalTimeMaster != NULL_PTR)
    {
        masterCfg = *(PORT_TOCFGPORT(port->PortIndexInPortList).PortRole.GlobalTimeMaster);
    }
    else
    {
        masterCfg.CorrectionField = 0u;
    }
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    /*transportSpecific and messageType */
    data[0] = (uint8)((uint8)(0x01u << ETHTSYN_BIT_SHIFT_4) | ((uint8)ethTSynMessage));
    /*versionPTP */
    data[1] = ETHTSYN_PTP_VERSION;
    /*domainNumber */
    data[ETHTSYN_TIMEDOMAINID_BYTE_OFFSET] = port->Timedomainid;
    /*reserved */
    data[5] = 0x00u;
    /*CorrectionField*/
    EthTSyn_CreatCorrectionField(masterCfg.CorrectionField, &data[8u]);
    /*reserved */
    data[16] = 0x00u;
    data[17] = 0x00u;
    data[18] = 0x00u;
    data[19] = 0x00u;

    /* sourcePortIdentity */
    (void)IStdLib_MemCpy(
        &data[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET],
        port->ThisPortIdentity,
        ETHTSYN_PORT_IDENTITY_LENGTH);

    /*reserved */
    for (uint8 i = 0u; i < 8u; i++)
    {
        data[46u + i] = 0x00u;
    }
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6060 -- */
/* PRQA S 1503, 1532 -- */
