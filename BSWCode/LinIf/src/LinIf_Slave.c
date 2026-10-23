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
 **  @file               : LinIf_Slave.c
 **  @author             : HuRongbo
 **  @date               :
 **  @vendor             : isoft
 **  @description        : LinIf slave implementation source file
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "LinIf_Cfg.h"
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinIf_Slave.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp_Slave.h"
#endif
#endif
#include "LinIf_Internal.h"
#include "LinIf_PartitionVar.h"
#include "Com.h"

/* ===================================================== macros ===================================================== */
/* SID position in Node Configuration Frame */
#define LINIF_NC_NAD_POS   0x00u
#define LINIF_NC_PCI_POS   0x01u
#define LINIF_NC_SID_POS   0x02u
#define LINIF_NC_RSID_POS  0x02u
#define LINIF_NC_DATA1_POS 0x03u
#define LINIF_NC_DATA2_POS 0x04u
#define LINIF_NC_DATA3_POS 0x05u
#define LINIF_NC_DATA4_POS 0x06u
#define LINIF_NC_DATA5_POS 0x07u

#define LINIF_NC_SID_ASSIGNNAD          0xB0u
#define LINIF_NC_ASSIGNFRAMEIDENTIFIER  0xB1u
#define LINIF_NC_SID_READBYIDENTIFIER   0xB2u
#define LINIF_NC_CONDITIONALCHANGENAD   0xB3u
#define LINIF_NC_SID_DATADUMP           0xB4u
#define LINIF_NC_SID_SAVECONFIGURATION  0xB6u
#define LINIF_NC_SID_ASSIGNFRAMEIDRANGE 0xB7u

#define LINIF_NC_PCI_DATA_LEN_6               0x06u
#define LINIF_NC_PCI_DATA_LEN_1               0x01u
#define LINIF_NC_PCI_FRAME_TYPE_OFFSET        0x04u
#define LINIF_NC_PCI_DL_MASK                  0x0Fu
#define LINIF_NC_NEGTIVE_RESPONSE             0x7Fu
#define LINIF_NC_FUNCTIONAL_REQ_NAD           0x7Eu
#define LINIF_NC_READBYIDENTIFIER_TIMING_TEST 0x00u
#define LINIF_NC_READBYIDENTIFIER_PRODUCTID   0x02u
#define LINIF_NC_IDENTIFIER_UNUSED_VALUE      0xFFu

#define LINIF_NC_WILDCARD_NAD        0x7Fu
#define LINIF_NC_WILDCARD_SUPPLIERID (uint16)0x7FFFu
#define LINIF_NC_WILDCARD_FUNCTIONID (uint16)0xFFFFu

#define LINIF_NO_ASSOCIATED_WITH_EVENT 0xffu
#define LINIF_FRAME_TYPE_SF            0x00u
#define LINIF_FRAME_TYPE_FF            0x01u
#define LINIF_FF_SID_FIELD_INDEX       0x03u

/* ============================================ internal data definition ============================================ */
#define LINIF_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "LinIf_MemMap.h"
LINIF_LOCAL uint8 LinIf_SlavePIDTable[LINIF_SLAVE_FRAME_MAX_OF_DIFFER_VARIANTS];
#define LINIF_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "LinIf_MemMap.h"
LINIF_LOCAL uint8 LinIf_SlaveConfiguredNAD[LINIF_SLAVE_CHANNEL_NUMBER];
#define LINIF_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "LinIf_MemMap.h"

/* PRQA S 2814,2824,2844,2889 ++ */ /* VL_QAC_DerefNullPtr */

/* ========================================= internal function declarations ========================================= */

LINIF_LOCAL_INLINE uint8* LinIf_GetChannelPidTable(uint8 ch)
{
    return &LinIf_SlavePIDTable[LinIf_GetFrameOfs(ch) - LinIf_GetMasterFrameNum()];
}

LINIF_LOCAL_INLINE LinIf_SlaveRuntimeType* LinIf_GetSlaveRtDataPtr(uint8 ch)
{
    return LinIf_SlaveRtPartitionData[(ch)-LINIF_MASTER_CHANNEL_NUMBER];
}

LINIF_LOCAL_INLINE const LinIf_NodeConfigurationIdentificationType* LinIf_GetNcIndetification(uint8 ch)
{
    return LinIf_GetChannel(ch)->NodeType->Slave->NodeConfigurationIdentification;
}

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
LINIF_LOCAL_INLINE Com_SignalIdType LinIf_GetResponseErrorSignal(uint8 ch)
{
    return LinIf_GetChannel(ch)->NodeType->Slave->ResponseErrorSignal;
}

LINIF_LOCAL_INLINE boolean LinIf_GetResponseErrorSignalConfigured(uint8 ch)
{
    return LinIf_GetChannel(ch)->NodeType->Slave->ResponseErrorSignalConfigured;
}
#endif

LINIF_LOCAL_INLINE void LinIf_UserGotoSleepIndication(uint8 ch, NetworkHandleType channel)
{
    LinIf_GetChannel(ch)->GotoSleepIndication(channel);
}

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL uint16 LinIf_GetFrameByPid(uint8 ch, uint8 framePid);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveUnconditionalHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveEventTriggeredHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveMRFHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType
    LinIf_SlaveNcFrameHandle(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL LinIf_SlaveNcFrameCheckResult LinIf_SlaveNcFrameCheck(const uint8* Lin_SduPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif
#endif

#if (LINIF_SLAVE_SRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveSRFHeaderHandle(
    NetworkHandleType       ch,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void
    LinIf_SlaveUnconditionalRxHandle(NetworkHandleType ch, LinIf_SlaveRuntimeType* slaveRTDataPtr, uint8* Lin_SduPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void
    LinIf_SlaveMRFRxHandle(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON && LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType
    LinIf_SlaveNcAssignNAD(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcReadbyIdentifier(
    NetworkHandleType       ch,
    const uint8*            Lin_SduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcSaveConfiguration(
    NetworkHandleType       ch,
    const uint8*            Lin_SduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcAssignFrameIdRange(
    NetworkHandleType       ch,
    const uint8*            Lin_SduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcAssignFrameIdRangeHandler(
    NetworkHandleType       ch,
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const uint8*            Lin_SduPtr,
    uint16                  frmIdx);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void
    LinIf_SlaveSetResponseErrorBit(LinIf_SlaveRuntimeType* slaveRTDataPtr, NetworkHandleType ch, boolean responseError);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveSetLinPduType(Lin_PduType* PduPtr, const LinIf_FrameType* frame);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveBuildNCResponse(
    NetworkHandleType       ch,
    uint8                   nad,
    uint8                   sid,
    uint8                   identifier,
    LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveTimerHandle(NetworkHandleType ch, LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveResetRtData(LinIf_SlaveRuntimeType* slaveRTDataPtr);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Initializes the LinIf slave channel.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
void LinIf_SlaveInit(ApplicationType applicationId)
/* PRQA S 6070 -- */
{
    for (NetworkHandleType ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
        if (LinIf_GetChannelApplicationId(ch) != applicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_LinIf_Continue */
        }
#endif
        const LinIf_ChannelType* chCfgPtr       = LinIf_GetChannel(ch);
        LinIf_SlaveRuntimeType*  slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);

        LinIf_SlaveResetRtData(slaveRTDataPtr);
        /*@req <SWS_LinIf_00507>*/
        if (LINIF_STARTUP_NORMAL == chCfgPtr->StartupState)
        {
            slaveRTDataPtr->ChannelState = LINIF_CHANNEL_OPERATIONAL;

            /*@req <SWS_LinIf_00752>*/
            /* Start the bus idle timeout observation */
            slaveRTDataPtr->BusIdleTimer = chCfgPtr->BusIdleTimeoutCnt;
        }
        else
        {
            slaveRTDataPtr->ChannelState = LINIF_CHANNEL_SLEEP;
            slaveRTDataPtr->BusIdleTimer = 0u;
        }

        /* Initial Pid table */
        uint8* pidTablePtr = LinIf_GetChannelPidTable(ch);
        for (uint16 frmIdx = 0u; frmIdx < LinIf_GetFrameNum(ch); frmIdx++)
        {
            const LinIf_FrameType* framePtr = LinIf_GetFrame(ch, frmIdx);
            pidTablePtr[frmIdx]             = framePtr->ProtectedId;
        }

        /* Initial configed NAD */
        const LinIf_NodeConfigurationIdentificationType* ncIdentifi = LinIf_GetNcIndetification(ch);
        if (NULL_PTR != ncIdentifi)
        {
            LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = ncIdentifi->InitialNAD;
        }

        slaveRTDataPtr->NcResponsePendingFlag = FALSE;
        slaveRTDataPtr->ResponseError         = FALSE;
        (void)IStdLib_MemSet(slaveRTDataPtr->NcResponse, 0u, LINIF_NC_RESPONSE_LEN);
#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
        slaveRTDataPtr->BusMirrorEnable = FALSE;
#endif
        (void)IStdLib_MemSet(slaveRTDataPtr->NcResponse, 0u, LINIF_NC_RESPONSE_LEN);
    }

    LINIF_NOUSED(applicationId);
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef */

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Transmit request process in slave node.
 */
Std_ReturnType LinIf_SlaveTransmit(uint16 frameIdx)
{
    /*@req <SWS_LinIf_00341>,<SWS_LinIf_00730>*/
    /* Set transmit request flag */
    LinIf_SetTransmitReq(frameIdx, TRUE);
    return E_OK;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Process slave node wakeup request.
 */
Std_ReturnType LinIf_SlaveWakeUp(NetworkHandleType ch)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
    uint8                   linDriver      = LinIf_GetLinDriverId(ch);
    uint8                   linChannel     = LinIf_GetLinChannelId(ch);
    Std_ReturnType          ret;

    if (LINIF_CHANNEL_SLEEP == slaveRTDataPtr->ChannelState)
    {
        if (slaveRTDataPtr->WakeupFlag)
        {
            /*@req <SWS_LinIf_00713>,<SWS_LinIf_00721>*/
            ret = Lin_DriverApi[linDriver].LinWakeupInternal(linChannel);
        }
        else
        {
            /*@req <SWS_LinIf_00296>,<SWS_LinIf_00720>*/
            ret = Lin_DriverApi[linDriver].LinWakeup(linChannel);
        }
    }
    else if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->ChannelState)
    {
        /*@req <SWS_LinIf_00432>*/
        ret = E_OK;
    }
    else
    {
        /* Wakeup during sleep transition:Initiate a wakeup and stop GoToSleep process */
        /*@req <SWS_LinIf_00760>*/
        ret = Lin_DriverApi[linDriver].LinWakeup(linChannel);
        if (E_OK == ret)
        {
            /* Clear flag to call GoToSleepConfirmation() in MainFunction */
            slaveRTDataPtr->GotoSleepConfirmationFlag = FALSE;
        }
    }

    /*@req <SWS_LinIf_00720>,<SWS_LinIf_00721>*/
    if (E_OK == ret)
    {
        /*@req <SWS_LinIf_00714>*/
        slaveRTDataPtr->WakeupFlag = FALSE;
        /* Wait LinIf_HeaderIndication */
        slaveRTDataPtr->WaitWakeupConfirmFlag = TRUE;
    }
    else
    {
        /*@req <SWS_LinIf_00762>*/
        LinIf_UserWakeupConfirmation(ch, LinIf_GetComMNetwork(ch), FALSE);
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Wakeup confirmation process in slave node
 */
void LinIf_SlaveWakeupConfirmation(EcuM_WakeupSourceType WakeupSource, ApplicationType applicationId)
{
#if (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin driver channel to find the wake-up source*/
    for (NetworkHandleType ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
        EcuM_WakeupSourceType wakeupSource = LinIf_GetLinDriverWakeupSource(ch);
        if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
            LinIf_GetChannelApplicationId(ch) == applicationId &&
#endif
            0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
            slaveRTDataPtr->WakeupFlag             = TRUE;
            break;
        }
    }
#endif

#if ((LINIF_TRCV_DRIVER_SUPPORTED == STD_ON) && (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON))
    /*check all Lin transceiver channel to find the wake-up source*/
    for (NetworkHandleType ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
        EcuM_WakeupSourceType wakeupSource = LinIf_GetLinTrcvWakeupSource(ch);
        if (
#if (LINIF_MULTIPLE_PARTITION_USED == STD_ON)
            LinIf_GetChannelApplicationId(ch) == applicationId &&
#endif
            0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
            slaveRTDataPtr->WakeupFlag             = TRUE;
            break;
        }
    }
#endif

    LINIF_NOUSED(applicationId);
    LINIF_NOUSED(WakeupSource);
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

void LinIf_SlaveEnableBusMirror(NetworkHandleType ch, boolean enbale)
{
    LinIf_SlaveRuntimeType* rtDataPtr = LinIf_GetSlaveRtDataPtr(ch);

    SchM_Enter_LinIf_ExclusiveArea_Channel();

    rtDataPtr->BusMirrorEnable = enbale;

    SchM_Exit_LinIf_ExclusiveArea_Channel();
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Copy PID table to provide 'PidBuffer'
 */
void LinIf_SlaveGetPIDTable(NetworkHandleType ch, Lin_FramePidType* PidBuffer, uint8* PidBufferLength)
{
    if (*PidBufferLength != 0u)
    {
        uint16                 pidIdx         = LinIf_GetFrameOfs(ch) - LinIf_GetMasterFrameNum();
        uint8                  pidTableLen    = LinIf_GetFrameNum(ch);
        const LinIf_FrameType* pFrame         = LinIf_GetFrame(ch, 0u);
        const uint8*           pSlavePidTable = &LinIf_SlavePIDTable[pidIdx];
        uint8                  pidNum         = 0u;

        for (uint16 idx = 0u; idx < pidTableLen; idx++)
        {
            /* @req <SWS_LinIf_00817> */
#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON && LINIF_SLAVE_SRF_FRAME_SUPPORT == STD_ON)
            if ((LINIF_MRF != pFrame->FrameType) && (LINIF_SRF != pFrame->FrameType))
#endif
            {
                PidBuffer[pidNum] = *pSlavePidTable;
                pidNum++;
                pSlavePidTable++;
            }
            pFrame++;
        }
        /* @req <SWS_LinIf_00816> */
        *PidBufferLength = pidNum;
    }
    else
    {
        /* @req <SWS_LinIf_00828> */
        *PidBufferLength = LinIf_GetFrameNum(ch);
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Set 'PdiBuffer' pointed PID table to internal PID table.
 */
void LinIf_SlaveSetPIDTable(NetworkHandleType ch, const Lin_FramePidType* PidBuffer)
{
    uint16 pidIdx      = LinIf_GetFrameOfs(ch) - LinIf_GetMasterFrameNum();
    uint16 pidTableLen = LinIf_GetFrameNum(ch);

    (void)IStdLib_MemCpy(&LinIf_SlavePIDTable[pidIdx], PidBuffer, pidTableLen);
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Get the current configured NAD.
 */
Std_ReturnType LinIf_SlaveGetConfiguredNAD(NetworkHandleType ch, uint8* Nad)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == LINIF_MASTER_SUPPORT)
    if (ch >= LINIF_MASTER_CHANNEL_NUMBER)
#endif
    {
        *Nad = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
        ret  = E_OK;
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Sets the current configured NAD.
 */
void LinIf_SlaveSetConfiguredNAD(NetworkHandleType ch, uint8 Nad)
{
    LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = Nad;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Header receive process in slave node.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_LinIf_STCAL*/
Std_ReturnType LinIf_SlaveHeaderIndication(NetworkHandleType ch, Lin_PduType* PduPtr)
/* PRQA S 6070 -- */
{
    LinIf_SlaveRuntimeType*  slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
    Std_ReturnType           ret            = E_NOT_OK;
    boolean                  irrelevantFlag = TRUE;
    const LinIf_ChannelType* chCfgPtr       = LinIf_GetChannel(ch);

    if (slaveRTDataPtr->WaitWakeupConfirmFlag)
    {
        slaveRTDataPtr->WaitWakeupConfirmFlag = FALSE;

        /*@req <SWS_LinIf_00761>*/
        chCfgPtr->WakeupConfirmation(chCfgPtr->ComMNetworkHandleRef, TRUE);

        slaveRTDataPtr->ChannelState = LINIF_CHANNEL_OPERATIONAL;
    }

    /*@req <SWS_LinIf_00189>*/
    /* Receive/transmit LIN frame headers and responses only in the state
       LINIF_CHANNEL_OPERATIONAL*/
    if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->ChannelState)
    {
        uint16                 frmNum   = LinIf_GetFrameNum(ch);
        const LinIf_FrameType* framePtr = NULL_PTR;
        uint16                 frmIdx   = LinIf_GetFrameByPid(ch, PduPtr->Pid);
        if (frmNum != frmIdx)
        {
            irrelevantFlag = FALSE;
            framePtr       = LinIf_GetFrame(ch, frmIdx);
        }

        if (irrelevantFlag)
        {
            /*@req <SWS_LinIf_00748>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            ret = E_OK;
        }
        else
        {
            switch (framePtr->FrameType)
            {
            case LINIF_UNCONDITIONAL:
                ret = LinIf_SlaveUnconditionalHeaderHandle(slaveRTDataPtr, framePtr, PduPtr);
                break;

#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
            case LINIF_EVENT_TRIGGERED:
                ret = LinIf_SlaveEventTriggeredHeaderHandle(slaveRTDataPtr, framePtr, PduPtr);
                break;
#endif

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
            case LINIF_MRF:
                ret = LinIf_SlaveMRFHeaderHandle(slaveRTDataPtr, framePtr, PduPtr);
                break;
#endif

#if (LINIF_SLAVE_SRF_FRAME_SUPPORT == STD_ON)
            case LINIF_SRF:
                ret = LinIf_SlaveSRFHeaderHandle(ch, framePtr, PduPtr, slaveRTDataPtr);
                break;
#endif

            default:
                /* Other Frame Type */
                break;
            }

            /* Response transmitting/receiving,save info to runtime data */
            if (E_OK == ret)
            {
                /* Set frame status to response handle */
                slaveRTDataPtr->FrameStatus = LINIF_SLAVE_FRAME_RESPONSE;
                /* Save current frame */
                slaveRTDataPtr->CurFrame = framePtr;
            }
        }

        /*@req <SWS_LinIf_00752>,<SWS_LinIf_00754>*/
        /* Reload the running bus idle timer*/
        slaveRTDataPtr->BusIdleTimer = chCfgPtr->BusIdleTimeoutCnt;
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Receive indication process in slave node.
 */
void LinIf_SlaveRxIndication(NetworkHandleType ch, uint8* Lin_SduPtr)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);

    if ((LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
        && (LINIF_RX_PDU == slaveRTDataPtr->CurFrame->PduDirection->PduDirectionId))
    {
        /*@req <SWS_LinIf_00189>*/
        /* Receive/transmit LIN frame headers and responses only in the state
        LINIF_CHANNEL_OPERATIONAL*/
        if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->ChannelState)
        {
            const LinIf_FrameType* framePtr = slaveRTDataPtr->CurFrame;
            switch (framePtr->FrameType)
            {
            case LINIF_UNCONDITIONAL:
                LinIf_SlaveUnconditionalRxHandle(ch, slaveRTDataPtr, Lin_SduPtr);
                break;

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
            case LINIF_MRF:
                LinIf_SlaveMRFRxHandle(ch, Lin_SduPtr, slaveRTDataPtr);
                break;
#endif

            default:
                /* Other Frame Type */
                break;
            }

            /*@req <SWS_LinIf_00754>*/
            /* Reload the running bus idle timer*/
            slaveRTDataPtr->BusIdleTimer = LinIf_GetChannel(ch)->BusIdleTimeoutCnt;
        }
    }
    else
    {
        /*@req <SWS_LinIf_00848>*/
        /*No response reception is expected,return without further action.*/
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Tx confirmation process in slave node.
 */
void LinIf_SlaveTxConfirmation(NetworkHandleType ch)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
    const LinIf_FrameType*  framePtr       = slaveRTDataPtr->CurFrame;
    const LinIf_TxPduType*  txPduPtr;

    if ((LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->ChannelState)
        && (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
        && (LINIF_TX_PDU == slaveRTDataPtr->CurFrame->PduDirection->PduDirectionId))
    {
        switch (framePtr->FrameType)
        {
        case LINIF_UNCONDITIONAL:
#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
        case LINIF_EVENT_TRIGGERED:
#endif
            /*@req <SWS_LinIf_00732>*/
            /* Clear pending flag */
            if (LINIF_UNCONDITIONAL == framePtr->FrameType) /* PRQA S 2991,2995 */ /* VL_LinIf_Logical */
            {
                *framePtr->IsTransmitPending = FALSE;
                txPduPtr                     = framePtr->PduDirection->TxPdu;
            }
#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
            else
            {
                const LinIf_FrameType* subFramePtr = LinIf_GetGlobalFrame(slaveRTDataPtr->EventTrgTxFrmIdx);
                *subFramePtr->IsTransmitPending    = FALSE;
                txPduPtr                           = subFramePtr->PduDirection->TxPdu;
            }
#endif

            /*@req <SWS_LinIf_00741>*/
            txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_OK);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
            /* @req <SWS_LinIf_00839> */
            if (slaveRTDataPtr->BusMirrorEnable)
            {
                LinIf_ReportTxFrameToMirror(ch, &slaveRTDataPtr->TxMirrorBuffer, LIN_TX_OK);
            }
#endif

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
            if (txPduPtr->ContainResponseErrorSignal)
            {
                /*@req <SWS_LinIf_00747>*/
                LinIf_SlaveSetResponseErrorBit(slaveRTDataPtr, ch, FALSE);
            }
#endif

            /* Transmit finish,Reset channel runtime data */
            LinIf_SlaveResetRtData(slaveRTDataPtr);
            break;

#if (LINIF_SLAVE_SRF_FRAME_SUPPORT == STD_ON)
        case LINIF_SRF:
            if (slaveRTDataPtr->NcResponsePendingFlag)
            {
                /*@req <SWS_LinIf_00734> */
                /* Stop NAS timeout timer */
                slaveRTDataPtr->NasTimer = 0u;

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                if (slaveRTDataPtr->BusMirrorEnable)
                {
                    LinIf_ReportTxFrameToMirror(ch, &slaveRTDataPtr->TxMirrorBuffer, LIN_TX_OK);
                }
#endif

                /* Transmit finish,Reset channel runtime data */
                LinIf_SlaveResetRtData(slaveRTDataPtr);
                slaveRTDataPtr->NcResponsePendingFlag = FALSE;
                (void)IStdLib_MemSet(slaveRTDataPtr->NcResponse, 0u, LINIF_NC_RESPONSE_LEN);
            }
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
            else
            {
                boolean isTpTxFinished;
                LinTp_SlaveTxConfirmation(ch, &isTpTxFinished);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                if (slaveRTDataPtr->BusMirrorEnable)
                {
                    LinIf_ReportTxFrameToMirror(ch, &slaveRTDataPtr->TxMirrorBuffer, LIN_TX_OK);
                }
#endif

                if (isTpTxFinished)
                {
                    /* TP transmit finish,Reset channel runtime data */
                    LinIf_SlaveResetRtData(slaveRTDataPtr);
                }
            }
#endif
            break;
#endif

        default:
            /* Other Frame Type */
            break;
        }

        /*@req <SWS_LinIf_00754>*/
        /* Reload the running bus idle timer*/
        slaveRTDataPtr->BusIdleTimer = LinIf_GetChannel(ch)->BusIdleTimeoutCnt;
    }
    else
    {
        /*@req <SWS_LinIf_00852>*/
        /*No response reception is expected,return without further action.*/
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Lin error indication process in slave node.
 */
void LinIf_SlaveLinErrorIndication(NetworkHandleType ch, Lin_SlaveErrorType ErrorStatus)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);

    if (NULL_PTR != slaveRTDataPtr->CurFrame)
    {
        if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
        {
            if (LIN_ERR_HEADER != ErrorStatus)
            {
                /*@req <SWS_LinIf_00743>,<SWS_LinIf_00735>*/
                LinIf_ReportRuntimeError(LINIF_LINERRORINDICATION_ID, LINIF_E_RESPONSE);
            }

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
            LinIf_PduDirectionIdType LinIfPduDirectionId = slaveRTDataPtr->CurFrame->PduDirection->PduDirectionId;
            /*@req <SWS_LinIf_00744>,<SWS_LinIf_00736>*/
            if ((LIN_ERR_RESP_STOPBIT == ErrorStatus) || (LIN_ERR_RESP_CHKSUM == ErrorStatus)
                || (LIN_ERR_RESP_DATABIT == ErrorStatus)
                || ((LIN_ERR_INC_RESP == ErrorStatus) && (LINIF_RX_PDU == LinIfPduDirectionId)))
            {
                LinIf_SlaveSetResponseErrorBit(slaveRTDataPtr, ch, TRUE);
            }
#endif

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
            /* @req <SWS_LinIf_00869> */
            if (slaveRTDataPtr->BusMirrorEnable)
            {
                LinIf_ReportLinErrorToMirror(ch, slaveRTDataPtr->CurFrame, ErrorStatus);
            }
#endif
        }

        /*@req <SWS_LinIf_00753>,<SWS_LinIf_00754>*/
        /* Reload the running bus idle timer in OPERATION state */
        if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->ChannelState)
        {
            slaveRTDataPtr->BusIdleTimer = LinIf_GetChannel(ch)->BusIdleTimeoutCnt;
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * Go-to-sleep process in slave node.
 */
void LinIf_SlaveGotoSleep(NetworkHandleType ch)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);
    uint8                   linDriver      = LinIf_GetLinDriverId(ch);
    uint8                   linChannel     = LinIf_GetLinChannelId(ch);

    /*@req <SWS_LinIf_00757>*/
    /* When LinIf_GotoSleep is called, Lin_GotoSleepInternal shall be called
       directly */
    (void)Lin_DriverApi[linDriver].LinGoToSleepInternal(linChannel);

    /*@req <SWS_LinIf_00758>*/
    /* Clear the wakeup flag */
    slaveRTDataPtr->WakeupFlag = FALSE;

    SchM_Enter_LinIf_ExclusiveArea_Channel();
    slaveRTDataPtr->WaitWakeupConfirmFlag = FALSE;
    SchM_Exit_LinIf_ExclusiveArea_Channel();

    /* Set flag to call GoToSleepConfirmation() in MainFunction */
    slaveRTDataPtr->GotoSleepConfirmationFlag = TRUE;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * LinIf slave main function
 */
void LinIf_SlaveMainHandle(NetworkHandleType ch)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LinIf_GetSlaveRtDataPtr(ch);

    LinIf_SlaveTimerHandle(ch, slaveRTDataPtr);

    if (slaveRTDataPtr->GotoSleepConfirmationFlag)
    {
        /* Change channel state to sleep */
        slaveRTDataPtr->ChannelState              = LINIF_CHANNEL_SLEEP;
        slaveRTDataPtr->GotoSleepConfirmationFlag = FALSE;

        /*@req <SWS_LinIf_00753>*/
        /* LINIF_CHANNEL_SLEEP is entered,stop the bus idle timeout observation */
        slaveRTDataPtr->BusIdleTimer = 0u;

        /*@req <SWS_LinIf_00759>*/
        /* After calling the function Lin_GoToSleepInternal, the LIN Interface shall
           invoke the function <User>_GotoSleepConfirmation with parameter TRUE.*/
        LinIf_UserGotoSleepConfirmation(ch, LinIf_GetComMNetwork(ch), TRUE);
    }

#if (LINTP_SLAVE_SUPPORT == STD_ON)
    if (LinIf_IsSupportTpTransmit(ch))
    {
        LinTp_SlaveMainFunction(ch);
    }
#endif
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/* PRQA S 1532,1503 -- */

/* ========================================== internal function definition ========================================== */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

LINIF_LOCAL uint16 LinIf_GetFrameByPid(uint8 ch, uint8 framePid)
{
    uint16       frmNumMax   = LinIf_GetFrameNum(ch);
    const uint8* pidTablePtr = LinIf_GetChannelPidTable(ch);
    uint16       frmIdx      = 0u;

    for (frmIdx = 0u; frmIdx < frmNumMax; frmIdx++)
    {
        if (pidTablePtr[frmIdx] == framePid)
        {
            break;
        }
    }
    return frmIdx;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * @brief               Unconditional frame process in slave node.
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @param[in]           framePtr: Pointer to frame that receive in current
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter
 * @return              E_OK: Handle success for unconditional frame header
 * @return              E_NOT_OK: Handle faildure for unconditional frame header
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType LinIf_SlaveUnconditionalHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    PduInfoType    pduInfo;

    if (LINIF_RX_PDU == framePtr->PduDirection->PduDirectionId)
    {
        if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
        {
            /*@req <SWS_LinIf_00846>*/
            LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);
        }

        /*@req <SWS_LinIf_00733> */
        LinIf_SlaveSetLinPduType(PduPtr, framePtr);

        ret = E_OK;
    }
    else
    {
        /* Tx Pdu */
        if (NULL_PTR == PduPtr->SduPtr)
        {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            /*@req <SWS_LinIf_00845>*/
            LinIf_DetReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
        }
        else
        {
            if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
            {
                /*@req <SWS_LinIf_00846>*/
                LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                LinIf_SlaveResetRtData(slaveRTDataPtr);
            }

            const LinIf_TxPduType* txPduPtr = framePtr->PduDirection->TxPdu;
            /*@req <SWS_LinIf_00738>*/
            /* Get the data part of the frame from upper layer */
            pduInfo.SduDataPtr = PduPtr->SduPtr;
            pduInfo.SduLength  = framePtr->Length;
            ret                = txPduPtr->TxTriggerTransmitUL(txPduPtr->TxPduRef, &pduInfo);

            if (E_NOT_OK == ret)
            {
                /*@req <SWS_LinIf_00740>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
            }
            else
            {
#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
                /* Set Uncondition frame PID to first byte */
                uint8 uncondFrmIdWithEvent = framePtr->FrameIdAssociatedWithEvent;
                if (uncondFrmIdWithEvent != LINIF_NO_ASSOCIATED_WITH_EVENT)
                {
                    PduPtr->SduPtr[0] = LinIf_SlavePIDTable[uncondFrmIdWithEvent - LinIf_GetMasterFrameNum()];
                }
#endif
                /*@req <SWS_LinIf_00739>*/
                LinIf_SlaveSetLinPduType(PduPtr, framePtr);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                /* @req <SWS_LinIf_00839> */
                if (slaveRTDataPtr->BusMirrorEnable)
                {
                    LinIf_SaveTxDataToMirror(&slaveRTDataPtr->TxMirrorBuffer, framePtr, PduPtr->SduPtr);
                }
#endif
            }

            ret = E_OK;
        }
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_EVENT_TRIGGER_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Event-Triggered frame process in slave node.
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @param[in]           framePtr: Pointer to frame that receive in current
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter
 * @return              E_OK: Handle success for event trigger frame header
 * @return              E_NOT_OK: Handle faildure for event trigger frame header
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType LinIf_SlaveEventTriggeredHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr)
{
    PduInfoType    pduInfo;
    Std_ReturnType ret = E_NOT_OK;

    const LinIf_SubstitutionFramesType* subFrmPtr = framePtr->SubstitutionFrames;
    for (uint8 idx = 0u; idx < framePtr->NumOfSubstitutionFrame; idx++)
    {
        uint16 frameIdx = subFrmPtr->SubstitutionFrameRef;
        if (LinIf_GetTransmitReq(frameIdx))
        {
            if (NULL_PTR == PduPtr->SduPtr)
            {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                /*@req <SWS_LinIf_00845>*/
                LinIf_DetReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
            }
            else
            {
                if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
                {
                    /*@req <SWS_LinIf_00846>*/
                    LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                    LinIf_SlaveResetRtData(slaveRTDataPtr);
                }

                /*@req <SWS_LinIf_00731>*/
                /* Set Unconditional frame response to the following bytes.*/
                const LinIf_FrameType* uncondFrmPtr = LinIf_GetGlobalFrame(frameIdx);
                const LinIf_TxPduType* txPduPtr     = uncondFrmPtr->PduDirection->TxPdu;
                /*@req <SWS_LinIf_00738>*/
                /* Get the data part of the frame from upper layer */
                pduInfo.SduDataPtr = PduPtr->SduPtr;
                pduInfo.SduLength  = uncondFrmPtr->Length;
                ret                = txPduPtr->TxTriggerTransmitUL(txPduPtr->TxPduRef, &pduInfo);

                if (E_NOT_OK == ret)
                {
                    /*@req <SWS_LinIf_00740>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                    PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
                    PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif
                }
                else
                {
                    /*@req <SWS_LinIf_00739>*/
                    LinIf_SlaveSetLinPduType(PduPtr, framePtr);
                    PduPtr->Dl = uncondFrmPtr->Length;

                    /* Set Uncondition frame PID to first byte */
                    PduPtr->SduPtr[0u] = LinIf_SlavePIDTable[frameIdx - LinIf_GetMasterFrameNum()];

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                    /* @req <SWS_LinIf_00839> */
                    if (slaveRTDataPtr->BusMirrorEnable)
                    {
                        LinIf_SaveTxDataToMirror(&slaveRTDataPtr->TxMirrorBuffer, framePtr, PduPtr->SduPtr);
                    }
#endif

                    /* Backup frame index transmit in event-triggered frame */
                    slaveRTDataPtr->EventTrgTxFrmIdx = frameIdx;
                }

                ret = E_OK;
            }
            break;
        }
        subFrmPtr++;
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               MRF frame process in slave node.
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @param[in]           framePtr: Pointer to frame that receive in current
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter
 * @return              E_OK: Handle success for MRF frame header
 * @return              E_NOT_OK: Handle faildure for MRF frame header
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType LinIf_SlaveMRFHeaderHandle(
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr)
{
    /*@req <SWS_LinIf_00789>*/
    if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
    {
        /*@req <SWS_LinIf_00846>*/
        LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

        LinIf_SlaveResetRtData(slaveRTDataPtr);
    }

    /*@req <SWS_LinIf_00733> */
    LinIf_SlaveSetLinPduType(PduPtr, framePtr);

    return E_OK;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SLAVE_SRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               SRF frame process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           framePtr: Pointer to frame that receive in current
 * @param[inout]        PduPtr: Pointer to PDU providing the received PID and pointer to the SDU
                                data buffer as in parameter
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @return              E_OK: Handle success for SRF frame header
 * @return              E_NOT_OK: Handle faildure for SRF frame header
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 6070,6030 ++ */ /* VL_MTR_LinIf_STCAL,VL_MTR_LinIf_STMIF*/
LINIF_LOCAL Std_ReturnType LinIf_SlaveSRFHeaderHandle(
    NetworkHandleType       ch,
    const LinIf_FrameType*  framePtr,
    Lin_PduType*            PduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr)
/* PRQA S 6070,6030 -- */
{
    Std_ReturnType ret = E_NOT_OK;

    /*@req <SWS_LinIf_00775>*/
    if (slaveRTDataPtr->NcResponsePendingFlag)
    {
        if (NULL_PTR == PduPtr->SduPtr)
        {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            /*@req <SWS_LinIf_00845>*/
            LinIf_DetReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
        }
        else
        {
            if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->FrameStatus)
            {
                /*@req <SWS_LinIf_00846>*/
                LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                LinIf_SlaveResetRtData(slaveRTDataPtr);
            }

            /*@req <SWS_LinIf_00733> */
            LinIf_SlaveSetLinPduType(PduPtr, framePtr);
            /* Copy response to PduPtr->SduPtr */
            (void)IStdLib_MemCpy(PduPtr->SduPtr, slaveRTDataPtr->NcResponse, LINIF_NC_RESPONSE_LEN);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
            /* @req <SWS_LinIf_00839> */
            if (slaveRTDataPtr->BusMirrorEnable)
            {
                LinIf_SaveTxDataToMirror(&slaveRTDataPtr->TxMirrorBuffer, framePtr, PduPtr->SduPtr);
            }
#endif

            ret = E_OK;
        }
    }
    else if (LinIf_IsSupportTpTransmit(ch))
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        if (!LinTp_SlaveCheckFunctionAddressFlag(ch))
        {
            /*@req <SWS_LinIf_00776>*/
            LinIf_SlaveSetLinPduType(PduPtr, framePtr);
        }
        /*@req <SWS_LinIf_00776>*/
        if (LinTp_SlaveSRFIndication(ch, PduPtr))
        {
            if (LinTp_SlaveCheckFunctionAddressFlag(ch))
            {
                /* Functional addressing with SF,no respond */
                LinTp_SlaveSetFunctionAddressFlag(ch, FALSE);
                LinIf_SlaveResetRtData(slaveRTDataPtr);
            }
            else
            {
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                if (LIN_FRAMERESPONSE_TX == PduPtr->Drc)
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
                if (LIN_MASTER_RESPONSE == PduPtr->Drc)
#endif
                {
                    ret = E_OK;
                }
#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                /* @req <SWS_LinIf_00839> */
                if (slaveRTDataPtr->BusMirrorEnable)
                {
                    LinIf_SaveTxDataToMirror(&slaveRTDataPtr->TxMirrorBuffer, framePtr, PduPtr->SduPtr);
                }
#endif
            }
        }
#endif
    }
    else
    {
        /* Do nothing */
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Unconditional response receive process in slave node.
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void
    LinIf_SlaveUnconditionalRxHandle(NetworkHandleType ch, LinIf_SlaveRuntimeType* slaveRTDataPtr, uint8* Lin_SduPtr)
{
    const LinIf_FrameType* framePtr = slaveRTDataPtr->CurFrame;
    PduInfoType            pduInfo;

    /* Save PDU information */
    pduInfo.SduDataPtr = Lin_SduPtr;
    pduInfo.SduLength  = framePtr->Length;

    /*@req <SWS_LinIf_00734> */
    /* Notify upper */
    const LinIf_RxPduType* rxPduPtr = framePtr->PduDirection->RxPdu;
    rxPduPtr->RxIndicationUL(rxPduPtr->RxPduRef, &pduInfo);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
    /* @req <SWS_LinIf_00838> */
    if (slaveRTDataPtr->BusMirrorEnable)
    {
        LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
    }
#else
    LINIF_NOUSED(ch);
#endif

    /* Receive finish,Reset channel runtime data */
    LinIf_SlaveResetRtData(slaveRTDataPtr);
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

LINIF_LOCAL Std_ReturnType
    LinIf_SlaveNcFrameHandle(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    switch (Lin_SduPtr[LINIF_NC_SID_POS])
    {
    case LINIF_NC_SID_ASSIGNNAD:
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
        ret = LinIf_SlaveNcAssignNAD(ch, Lin_SduPtr, slaveRTDataPtr);
#endif
        break;

    case LINIF_NC_SID_READBYIDENTIFIER:
        ret = LinIf_SlaveNcReadbyIdentifier(ch, Lin_SduPtr, slaveRTDataPtr);
        break;

    case LINIF_NC_SID_SAVECONFIGURATION:
        ret = LinIf_SlaveNcSaveConfiguration(ch, Lin_SduPtr, slaveRTDataPtr);
        break;

    case LINIF_NC_SID_ASSIGNFRAMEIDRANGE:
        ret = LinIf_SlaveNcAssignFrameIdRange(ch, Lin_SduPtr, slaveRTDataPtr);
        break;

    default:
        /* Do nothing */
        break;
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Check whether the message is a node configuration message.
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 6030 ++ */ /* VL_MTR_LinIf_STMIF*/
LINIF_LOCAL LinIf_SlaveNcFrameCheckResult LinIf_SlaveNcFrameCheck(const uint8* Lin_SduPtr)
/* PRQA S 6030 -- */
{
    uint8 frameType = (uint8)(Lin_SduPtr[1] >> LINIF_NC_PCI_FRAME_TYPE_OFFSET);
    uint8 pci       = 0u;
    uint8 sidIndex  = (frameType == LINIF_FRAME_TYPE_SF) ? LINIF_NC_SID_POS : LINIF_FF_SID_FIELD_INDEX;
    uint8 sid       = 0u;
    LinIf_SlaveNcFrameCheckResult result = LINIF_SLAVE_NC_FRAME_IGNORE;

    if ((LINIF_FRAME_TYPE_FF == frameType) || (LINIF_FRAME_TYPE_SF == frameType))
    {
        sid = Lin_SduPtr[sidIndex];

        if ((sid >= LINIF_NC_SID_ASSIGNNAD) && (sid <= LINIF_NC_SID_ASSIGNFRAMEIDRANGE))
        {
            /* PRQA S  2991,2995 ++ */ /* VL_LinIf_Logical */
            if (LINIF_FRAME_TYPE_FF == frameType)
            {
                result = LINIF_SLAVE_NC_FRAME_REJECT;
            }
            else if (LINIF_FRAME_TYPE_SF == frameType)
            {
                pci = (uint8)(Lin_SduPtr[1] & LINIF_NC_PCI_DL_MASK);
                if ((((LINIF_NC_PCI_DATA_LEN_6 == pci) && (LINIF_NC_SID_ASSIGNNAD == Lin_SduPtr[LINIF_NC_SID_POS]))
                     || (LINIF_NC_SID_ASSIGNFRAMEIDRANGE == Lin_SduPtr[LINIF_NC_SID_POS])
                     || (LINIF_NC_ASSIGNFRAMEIDENTIFIER == Lin_SduPtr[LINIF_NC_SID_POS])
                     || (LINIF_NC_CONDITIONALCHANGENAD == Lin_SduPtr[LINIF_NC_SID_POS])
                     || (LINIF_NC_SID_DATADUMP == Lin_SduPtr[LINIF_NC_SID_POS])
                     || (LINIF_NC_SID_READBYIDENTIFIER == Lin_SduPtr[LINIF_NC_SID_POS]))
                    || ((LINIF_NC_PCI_DATA_LEN_1 == pci)
                        && (LINIF_NC_SID_SAVECONFIGURATION == Lin_SduPtr[LINIF_NC_SID_POS])))
                {
                    result = LINIF_SLAVE_NC_FRAME_ACCEPT;
                }
                else
                {
                    result = LINIF_SLAVE_NC_FRAME_REJECT;
                }
            }
            else
            {
                /* Nothing to do */
            }
            /* PRQA S  2991,2995 -- */
        }
        else
        {
            result = LINIF_SLAVE_NC_FRAME_IGNORE;
        }
    }

    return result;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               MRF response receive process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
/* PRQA S 2002,6070,6010 ++ */ /* VL_QAC_NoDefaultCase,VL_MTR_LinIf_STCAL,VL_MTR_LinIf_STCYC */
LINIF_LOCAL void
    LinIf_SlaveMRFRxHandle(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    const LinIf_NodeConfigurationIdentificationType* ncIdentifi = LinIf_GetNcIndetification(ch);
    Std_ReturnType                                   ret        = E_NOT_OK;
    boolean                                          tpFlag     = FALSE;

    if (LINIF_NC_FUNCTIONAL_REQ_NAD != Lin_SduPtr[LINIF_NC_NAD_POS])
    {
        /* If new MRF received,drop pending NC response */
        slaveRTDataPtr->NcResponsePendingFlag = FALSE;
    }

    /*@req <SWS_LinIf_00750> */
    /* Go-to-sleep command */
    if (0x00u == Lin_SduPtr[LINIF_NC_NAD_POS])
    {
        /*@req <SWS_LinIf_00756> */
        /* Notify upper layer by invoke function <User>_GotoSleepIndication */
        LinIf_UserGotoSleepIndication(ch, LinIf_GetComMNetwork(ch));

        slaveRTDataPtr->ChannelState = LINIF_CHANNEL_GOTO_SLEEP;
    }
    else
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        LinIf_SlaveNcFrameCheckResult result = LinIf_SlaveNcFrameCheck(Lin_SduPtr);
        if (LINIF_SLAVE_NC_FRAME_ACCEPT == result)
        {
            /* If LinTp response is pending,receive NC request,cancel TP process */
            if (LinIf_IsSupportTpTransmit(ch))
            {
                LinTp_SlavePreInit(ch);
            }
#endif /* LINTP_SLAVE_SUPPORT == STD_ON */

            ret = LinIf_SlaveNcFrameHandle(ch, Lin_SduPtr, slaveRTDataPtr);

#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        }
        else if (LinIf_IsSupportTpTransmit(ch) && (LINIF_SLAVE_NC_FRAME_IGNORE == result))
        {
            tpFlag = TRUE;
            LinTp_SlaveMRFIndication(ch, Lin_SduPtr);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
            /* @req <SWS_LinIf_00838> */
            if (slaveRTDataPtr->BusMirrorEnable)
            {
                LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
            }
#endif

            LinIf_SlaveResetRtData(slaveRTDataPtr);
        }
        else
        {
            /* Do nothing */
        }
#endif

        /*@req <SWS_LinIf_00734> */
        if ((!tpFlag) && (E_OK == ret))
        {
            /* Start N_As timer after receive a valid node configuration request*/
            slaveRTDataPtr->NasTimer = ncIdentifi->NasTimeoutCnt;
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/* PRQA S 2002,6070,6010 -- */
#endif

/**
 * @brief               Assign NAD process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @return              E_OK: Node config frame of assign nad handle success
 * @return              E_NOT_OK: Node config frame of assign nad handle faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON && LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType
    LinIf_SlaveNcAssignNAD(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    const LinIf_NodeConfigurationIdentificationType* ncIdentifi = LinIf_GetNcIndetification(ch);
    uint8                                            nad        = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8                                            pci        = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8                                            sid        = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8                                            newNAD     = Lin_SduPtr[LINIF_NC_DATA5_POS];
    Std_ReturnType                                   ret        = E_NOT_OK;

    uint16 supplierID = Lin_SduPtr[LINIF_NC_DATA2_POS];
    supplierID        = (uint16)(supplierID << LINIF_SUPPLIER_ID_MSD) | Lin_SduPtr[LINIF_NC_DATA1_POS];
    uint16 functionID = Lin_SduPtr[LINIF_NC_DATA4_POS];
    functionID        = (uint16)(functionID << LINIF_FUNCTION_ID_MSD) | Lin_SduPtr[LINIF_NC_DATA3_POS];
    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((ncIdentifi->InitialNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (6u == pci)
        && ((ncIdentifi->SupplierId == supplierID) || (LINIF_NC_WILDCARD_SUPPLIERID == supplierID))
        && ((ncIdentifi->FunctionId == functionID) || (LINIF_NC_WILDCARD_FUNCTIONID == functionID)))
    {
        /*@req <SWS_LinIf_00779> */
        /* Update configured NAD with the received new NAD */
        LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = newNAD;

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
        /* @req <SWS_LinIf_00838> */
        if (slaveRTDataPtr->BusMirrorEnable)
        {
            LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
        }
#endif

        LinIf_SlaveResetRtData(slaveRTDataPtr);
        /* Build positive response for next SRF */
        slaveRTDataPtr->NcResponsePendingFlag = TRUE;
        LinIf_SlaveBuildNCResponse(ch, ncIdentifi->InitialNAD, sid, LINIF_NC_IDENTIFIER_UNUSED_VALUE, slaveRTDataPtr);

        ret = E_OK;
    }
    else
    {
        /*@req <SWS_LinIf_00787>,<SWS_LinIf_00788>,<SWS_LinIf_00788>,
         <SWS_LinIf_00871>,<SWS_LinIf_00790>*/
        /*
         * Unknown NAD,functional NAD (0x7E) or invalid or unknown PCI type is
         * received, the LIN Interface shall ignore this LIN frame.
         */
        LinIf_SlaveResetRtData(slaveRTDataPtr);
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Read by Identifier process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @return              E_OK: Node config frame of read bt identifier handle success
 * @return              E_NOT_OK: Node config frame of read bt identifier handle faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType
    LinIf_SlaveNcReadbyIdentifier(NetworkHandleType ch, const uint8* Lin_SduPtr, LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    const LinIf_NodeConfigurationIdentificationType* ncIdentifi = LinIf_GetNcIndetification(ch);
    uint8                                            nad        = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8                                            pci        = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8                                            sid        = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8                                            identifier = Lin_SduPtr[LINIF_NC_DATA1_POS];
    uint8          configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    Std_ReturnType ret         = E_NOT_OK;

    uint16 supplierID = Lin_SduPtr[LINIF_NC_DATA3_POS];
    supplierID        = (uint16)(supplierID << LINIF_SUPPLIER_ID_MSD) | Lin_SduPtr[LINIF_NC_DATA2_POS];
    uint16 functionID = Lin_SduPtr[LINIF_NC_DATA5_POS];
    functionID        = (uint16)(functionID << LINIF_FUNCTION_ID_MSD) | Lin_SduPtr[LINIF_NC_DATA4_POS];
    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (LINIF_NC_FRAME_PCI_VALUE == pci)
        && ((ncIdentifi->SupplierId == supplierID) || (LINIF_NC_WILDCARD_SUPPLIERID == supplierID))
        && ((ncIdentifi->FunctionId == functionID) || (LINIF_NC_WILDCARD_FUNCTIONID == functionID)))
    {
        if ((LINIF_NC_READBYIDENTIFIER_FUNCTION_ID == identifier) || (LINIF_NC_READBYIDENTIFIER_NEGTIVE == identifier))
        {
#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
            /* @req <SWS_LinIf_00838> */
            if (slaveRTDataPtr->BusMirrorEnable)
            {
                LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
            }
#endif

            LinIf_SlaveResetRtData(slaveRTDataPtr);
            /*@req <SWS_LinIf_00783>,<SWS_LinIf_00841> */
            /* Build positive/negative response for next SRF */
            slaveRTDataPtr->NcResponsePendingFlag = TRUE;
            LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, identifier, slaveRTDataPtr);

            ret = E_OK;
        }
        else
        {
            /* Identifier not directly support by LinIf Node configuration */
            /* Forwarded over Transport Layer to upper layer */
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
            if (LinIf_IsSupportTpTransmit(ch))
            {
                LinTp_SlaveMRFIndication(ch, Lin_SduPtr);
            }
            ret = E_NOT_OK; /*Not start Nas timer*/
#endif
        }
    }
    else
    {
        /*@req <SWS_LinIf_00787>,<SWS_LinIf_00788>,<SWS_LinIf_00788>,
         <SWS_LinIf_00871>,<SWS_LinIf_00790>*/
        /*
         * Unknown NAD,functional NAD (0x7E) or invalid or unknown PCI type is
         * received, the LIN Interface shall ignore this LIN frame.
         */
        LinIf_SlaveResetRtData(slaveRTDataPtr);
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Save configuration process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @return              E_OK: Node config frame of save configuration handle success
 * @return              E_NOT_OK: Node config frame of save configuration handle faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcSaveConfiguration(
    NetworkHandleType       ch,
    const uint8*            Lin_SduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    uint8          nad         = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8          pci         = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8          sid         = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8          configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    Std_ReturnType ret         = E_NOT_OK;

    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (1u == pci))
    {
/*@req <SWS_LinIf_00782> */
#if defined(LinIf_SaveConfigurationCallout)
        LinIf_SaveConfigurationCallout(ch);
#endif

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
        /* @req <SWS_LinIf_00838> */
        if (slaveRTDataPtr->BusMirrorEnable)
        {
            LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
        }
#endif

        LinIf_SlaveResetRtData(slaveRTDataPtr);
        /* Build positive response for next SRF */
        slaveRTDataPtr->NcResponsePendingFlag = TRUE;
        LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, LINIF_NC_IDENTIFIER_UNUSED_VALUE, slaveRTDataPtr);

        ret = E_OK;
    }
    else
    {
        /*@req <SWS_LinIf_00787>,<SWS_LinIf_00788>,<SWS_LinIf_00788>,
         <SWS_LinIf_00871>,<SWS_LinIf_00790>*/
        /*
         * Unknown NAD,functional NAD (0x7E) or invalid or unknown PCI type is
         * received, the LIN Interface shall ignore this LIN frame.
         */
        LinIf_SlaveResetRtData(slaveRTDataPtr);
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcAssignFrameIdRangeHandler(
    NetworkHandleType       ch,
    LinIf_SlaveRuntimeType* slaveRTDataPtr,
    const uint8*            Lin_SduPtr,
    uint16                  frmIdx)
{
    uint8* pidTablePtr                                    = LinIf_GetChannelPidTable(ch);
    uint8  assignPidIdx                                   = 0u;
    uint8  pid                                            = 0u;
    uint8  pidTableBak[LINIF_NC_ASSIGN_FRAME_ID_DATA_LEN] = {0};
    uint16 frmNumMax                                      = LinIf_GetFrameNum(ch);
    uint8  configedNAD                                    = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    uint8  sid                                            = Lin_SduPtr[LINIF_NC_SID_POS];

    /* Assign frame ID range can set PIDs up to four frames.*/
    for (assignPidIdx = 0u; assignPidIdx < LINIF_NC_ASSIGN_FRAME_ID_DATA_LEN; assignPidIdx++)
    {
        boolean breakFlag = FALSE;
        pid               = Lin_SduPtr[LINIF_NC_DATA2_POS + assignPidIdx];
        if (LINIF_NC_FRAME_PID_0XFF != pid)
        {
            if ((frmIdx + assignPidIdx) < frmNumMax)
            {
                /* Save the original PidTable value and restore it if Settings fail */
                pidTableBak[assignPidIdx] = pidTablePtr[frmIdx + assignPidIdx];

                if ((pid != LINIF_NC_FRAME_PID_0X3C) && (pid != LINIF_NC_FRAME_PID_0X7D)
                    && (pid != LINIF_NC_FRAME_PID_0XFE) && (pid != LINIF_NC_FRAME_PID_0XBF)
                    && (pidTablePtr[frmIdx + assignPidIdx] != LINIF_NC_FRAME_PID_0X3C)
                    && (pidTablePtr[frmIdx + assignPidIdx] != LINIF_NC_FRAME_PID_0X7D)
                    && (pidTablePtr[frmIdx + assignPidIdx] != LINIF_NC_FRAME_PID_0XFE)
                    && (pidTablePtr[frmIdx + assignPidIdx] != LINIF_NC_FRAME_PID_0XBF))
                {
                    pidTablePtr[frmIdx + assignPidIdx] = pid;
                }
                else
                {
                    /*@req <SWS_LinIf_00809> */
                    /* Frames with frame identifiers 0x3C to 0x3F shall not
                    be changed: 0x3C(PID:0x3C) 0x3D(PID:0x7D) 0x3E(PID:0xFE)
                    0x3F(PID:0xBF) */
                    breakFlag = TRUE;
                }
            }
            else
            {
                /* If pid is 0xFF(means 'do not care'), process the next one */
                /* If out of bound,shall reject the request message */
                breakFlag = TRUE;
            }
        }

        if (breakFlag)
        {
            break;
        }
    }

    Std_ReturnType ret = E_NOT_OK;
    if (assignPidIdx == LINIF_NC_FRAME_PID_0X04)
    {
        LinIf_SlaveResetRtData(slaveRTDataPtr);
        /* Build positive response for next SRF */
        slaveRTDataPtr->NcResponsePendingFlag = TRUE;
        LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, LINIF_NC_FRAME_PID_0XFF, slaveRTDataPtr);

        ret = E_OK;
    }
    else
    {
        /* Set fail,reject the request message and shall not sent a response*/
        /* Restore saved PidTable value */
        for (uint8 restoreIdx = 0u; restoreIdx < assignPidIdx; restoreIdx++)
        {
            pid = Lin_SduPtr[LINIF_NC_DATA2_POS + restoreIdx];
            if (LINIF_NC_FRAME_PID_0XFF != pid)
            {
                pidTablePtr[frmIdx + restoreIdx] = pidTableBak[restoreIdx];
            }
        }
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Assign Frame Id Range process in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           Lin_SduPtr:  pointer to a shadow buffer or memory mapped LIN
                                     Hardware receive buffer where the current SDU is stored
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @return              E_OK: Node config frame of assign Frame Id Range handle success
 * @return              E_NOT_OK: Node config frame of assign Frame Id Range handle faildure
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL Std_ReturnType LinIf_SlaveNcAssignFrameIdRange(
    NetworkHandleType       ch,
    const uint8*            Lin_SduPtr,
    LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    uint8          nad         = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8          pci         = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8          configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    Std_ReturnType ret         = E_NOT_OK;

    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (LINIF_NC_FRAME_PID_0X06 == pci))
    {
        /*@req <SWS_LinIf_00781> */
        const LinIf_FrameType* framePtr  = LinIf_GetFrame(ch, 0u);
        uint16                 frmNumMax = LinIf_GetFrameNum(ch);
        for (uint16 frmIdx = 0u; frmIdx < frmNumMax; frmIdx++)
        {
            /* If configed LinIfFrameIndex == received 'start index' */
            if (framePtr->FrameIndex == Lin_SduPtr[LINIF_NC_DATA1_POS])
            {
                ret = LinIf_SlaveNcAssignFrameIdRangeHandler(ch, slaveRTDataPtr, Lin_SduPtr, frmIdx);

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
                /* @req <SWS_LinIf_00838> */
                if (slaveRTDataPtr->BusMirrorEnable)
                {
                    LinIf_ReportRxFrameToMirror(ch, slaveRTDataPtr->CurFrame, Lin_SduPtr, LIN_RX_OK);
                }
#endif

                break;
            }

            framePtr++;
        }
    }
    else
    {
        /*@req <SWS_LinIf_00787>,<SWS_LinIf_00788>,<SWS_LinIf_00788>,
         <SWS_LinIf_00871>,<SWS_LinIf_00790>*/
        /*
         * Unknown NAD,functional NAD (0x7E) or invalid or unknown PCI type is
         * received, the LIN Interface shall ignore this LIN frame.
         */
        LinIf_SlaveResetRtData(slaveRTDataPtr);
    }

    return ret;
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

#if (LINIF_SUPPORT_RESPONSE_ERROR_SIGNAL == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Set Response-Error bit to the value same with parameter
 * @param[inout]        slaveRTDataPtr: The runtime data of linif channel
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           responseError: Whether an responseError occurred
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void
    LinIf_SlaveSetResponseErrorBit(LinIf_SlaveRuntimeType* slaveRTDataPtr, NetworkHandleType ch, boolean responseError)
{
    /*@req <SWS_LinIf_00766>*/
#if defined(LinIf_ResponseErrorSignalChangedCallout)
    if ((responseError != slaveRTDataPtr->ResponseError) && (TRUE == LinIf_GetResponseErrorSignalConfigured(ch)))
    {
        /*@req <SWS_LinIf_00765>*/
        LinIf_ResponseErrorSignalChangedCallout(ch, responseError);
    }
#endif
    /* Set the response_error signal */
    slaveRTDataPtr->ResponseError = responseError;

    /*@req <SWS_LinIf_00764>*/
    if (LinIf_GetResponseErrorSignalConfigured(ch))
    {
        (void)Com_SendSignal(LinIf_GetResponseErrorSignal(ch), &slaveRTDataPtr->ResponseError);
    }

    /*@req <SWS_LinIf_00742> */
    if (responseError)
    {
        const LinIf_FrameType* framePtr = slaveRTDataPtr->CurFrame;
        /*@req <SWS_LinIf_00796> */
        if (framePtr->FrameType == LINIF_SRF && !slaveRTDataPtr->NcResponsePendingFlag)
        {
#if (LINIF_TP_SUPPORTED == STD_ON)
            LinTp_SlaveTxErrorSignalHandler(ch);
#endif
        }
        else
        {
            const LinIf_TxPduType* txPduPtr = framePtr->PduDirection->TxPdu;
            if (txPduPtr != NULL_PTR)
            {
                txPduPtr->TxConfirmationUL(txPduPtr->TxPduRef, E_NOT_OK);
            }
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

/**
 * @brief               Set Lin_PduType
 * @param[inout]        PduPtr: Lin pdu info
 * @param[in]           frame: The frame currently being processed
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveSetLinPduType(Lin_PduType* PduPtr, const LinIf_FrameType* frame)
{
    /* Set DL */
    PduPtr->Dl = frame->Length;

    /* Set checksum type */
    if (LINIF_ENHANCED == frame->ChecksumType)
    {
        /* Enhanced frame */
        PduPtr->Cs = LIN_ENHANCED_CS;
    }
    else
    {
        /* Classic frame */
        PduPtr->Cs = LIN_CLASSIC_CS;
    }

    /* Set direction */
    switch (frame->PduDirection->PduDirectionId)
    {
    case LINIF_RX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_RX;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_RESPONSE;
#endif
        break;

    case LINIF_TX_PDU:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_TX;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_MASTER_RESPONSE;
#endif
        break;

    default:
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
        PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else /* The default version of lin driver is 4.2.2 / 4.3.1 */
        PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif

        break;
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#if (LINIF_SLAVE_MRF_FRAME_SUPPORT == STD_ON)
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/**
 * @brief               Build Node Configuration Response
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           nad: LinIf channel nad
 * @param[in]           nad: Node config frame sid
 * @param[in]           identifier: LIN product identification
 * @param[in]           slaveRTDataPtr: The runtime data of linif channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
LINIF_LOCAL void LinIf_SlaveBuildNCResponse(
    NetworkHandleType       ch,
    uint8                   nad,
    uint8                   sid,
    uint8                   identifier,
    LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    uint8* ncResponse = slaveRTDataPtr->NcResponse;
    (void)IStdLib_MemSet(ncResponse, LINIF_NC_RESERVED_VALUE, LINIF_NC_RESPONSE_LEN);
    ncResponse[LINIF_NC_NAD_POS] = nad;
    if (LINIF_NC_SID_READBYIDENTIFIER != sid)
    {
        ncResponse[LINIF_NC_PCI_POS]  = 0x01u;                                         /* length = 1 */
        ncResponse[LINIF_NC_RSID_POS] = sid + LINIF_NC_RESPONSE_POSITIVE_RESPONSE_SID; /* positive response */
    }
    else
    {
        if (0u == identifier) /* LIN product identification */
        {
            const LinIf_NodeConfigurationIdentificationType* ncIdentifi = LinIf_GetNcIndetification(ch);

            ncResponse[LINIF_NC_PCI_POS]   = LINIF_NC_FRAME_PCI_VALUE;                      /* length = 6 */
            ncResponse[LINIF_NC_RSID_POS]  = sid + LINIF_NC_RESPONSE_POSITIVE_RESPONSE_SID; /* positive response*/
            ncResponse[LINIF_NC_DATA1_POS] = (uint8)ncIdentifi->SupplierId;                 /* Supplier ID LSB*/
            ncResponse[LINIF_NC_DATA2_POS] =
                (uint8)(ncIdentifi->SupplierId >> LINIF_SUPPLIER_ID_MSD);   /* Supplier ID MSB*/
            ncResponse[LINIF_NC_DATA3_POS] = (uint8)ncIdentifi->FunctionId; /* Function ID LSB*/
            ncResponse[LINIF_NC_DATA4_POS] =
                (uint8)(ncIdentifi->FunctionId >> LINIF_FUNCTION_ID_MSD); /* Function ID LSB*/
            ncResponse[LINIF_NC_DATA5_POS] = ncIdentifi->VariantId;       /* Variant */
        }
        else if (LINIF_NC_READBYIDENTIFIER_NEGTIVE == identifier) /* Bit timing test */
        {
            ncResponse[LINIF_NC_PCI_POS]   = LINIF_NC_NEGTIVE_RESPONSE_PCI_VALUE; /* length = 6 */
            ncResponse[LINIF_NC_RSID_POS]  = LINIF_NC_NEGTIVE_RESPONSE;           /* negtive response */
            ncResponse[LINIF_NC_DATA1_POS] = sid;
            ncResponse[LINIF_NC_DATA2_POS] = LINIF_NC_NEGTIVE_RESPONSE_DATA2_VALUE;
        }
        else
        {
            /* Identifier not directly support by LinIf Node configuration */
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
#endif

/**
 * @brief               Process timer used in slave node.
 * @param[in]           ch: Identification of the LIN channel
 * @param[in]           slaveRTDataPtr: The runtime data of linif channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveTimerHandle(NetworkHandleType ch, LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    if (slaveRTDataPtr->NasTimer > 0u)
    {
        slaveRTDataPtr->NasTimer--;
        if (0u == slaveRTDataPtr->NasTimer)
        {
            /*@req <SWS_LinIf_00786>*/
            /* N_As timeout occurrence,abort pending node config response.*/
            LinIf_SlaveResetRtData(slaveRTDataPtr);
            slaveRTDataPtr->NcResponsePendingFlag = FALSE;
            (void)IStdLib_MemSet(slaveRTDataPtr->NcResponse, 0u, LINIF_NC_RESPONSE_LEN);
        }
    }

    if (slaveRTDataPtr->BusIdleTimer > 0u)
    {
        slaveRTDataPtr->BusIdleTimer--;
        if (0u == slaveRTDataPtr->BusIdleTimer)
        {
            /*@req <SWS_LinIf_00755>*/
            /* execute sleep mode transition */
            LinIf_UserGotoSleepIndication(ch, LinIf_GetComMNetwork(ch));
        }
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

/**
 * @brief               Reset linif slave channel runtime data
 * @param[in]           slaveRTDataPtr: The runtime data of linif channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               -
 */
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
LINIF_LOCAL void LinIf_SlaveResetRtData(LinIf_SlaveRuntimeType* slaveRTDataPtr)
{
    slaveRTDataPtr->CurFrame                  = NULL_PTR;
    slaveRTDataPtr->NasTimer                  = 0u;
    slaveRTDataPtr->EventTrgTxFrmIdx          = 0u;
    slaveRTDataPtr->FrameStatus               = LINIF_SLAVE_FRAME_IDLE;
    slaveRTDataPtr->WakeupFlag                = FALSE;
    slaveRTDataPtr->WaitWakeupConfirmFlag     = FALSE;
    slaveRTDataPtr->GotoSleepConfirmationFlag = FALSE;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/* PRQA S 2814,2824,2844,2889 -- */

#endif
