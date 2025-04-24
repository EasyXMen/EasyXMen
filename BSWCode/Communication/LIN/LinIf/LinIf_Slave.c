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
**  FILENAME    : LinIf_Slave.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "LinIf_Cfg.h"
#if (STD_ON == LINIF_SLAVE_SUPPORT)
#include "LinIf_Slave.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#if (LINTP_SLAVE_SUPPORT == STD_ON)
#include "LinTp_Slave.h"
#endif /*LINTP_SLAVE_SUPPORT == STD_ON*/
#endif /*LINIF_TP_SUPPORTED == STD_ON*/
#include "LinIf_Internal.h"
#include "Com.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/* SID position in Node Configuration Frame */
#define LINIF_NC_NAD_POS                (uint8)0x00
#define LINIF_NC_PCI_POS                (uint8)0x01
#define LINIF_NC_SID_POS                (uint8)0x02
#define LINIF_NC_RSID_POS               (uint8)0x02
#define LINIF_NC_DATA1_POS              (uint8)0x03
#define LINIF_NC_DATA2_POS              (uint8)0x04
#define LINIF_NC_DATA3_POS              (uint8)0x05
#define LINIF_NC_DATA4_POS              (uint8)0x06
#define LINIF_NC_DATA5_POS              (uint8)0x07

#define LINIF_NC_SID_ASSIGNNAD          (uint8)0xB0
#define LINIF_NC_SID_READBYIDENTIFIER   (uint8)0xB2
#define LINIF_NC_SID_SAVECONFIGURATION  (uint8)0xB6
#define LINIF_NC_SID_ASSIGNFRAMEIDRANGE (uint8)0xB7

#define LINIF_NC_RESPONSE_LEN           (uint8)0x08

#define LINIF_NC_NEGTIVE_RESPONSE       (uint8)0x7F
#define LINIF_NC_FUNCTIONAL_REQ_NAD     (uint8)0x7E

#define LINIF_NC_WILDCARD_NAD           (uint8)0x7F
#define LINIF_NC_WILDCARD_SUPPLIERID    (uint16)0x7FFF
#define LINIF_NC_WILDCARD_FUNCTIONID    (uint16)0xFFFF

#define LINIF_GET_CHANNEL_PIDTABLE(ch)  &LinIf_SlavePIDTable[LINIF_GET_FRAME_OFS(ch) - LINIF_MASTER_FRAME_NUM]

#define LINIF_GET_SLAVE_RTDATA_PTR(ch)  &LinIf_SlaveRTData[(ch)-LINIF_MASTER_CHANNEL_NUMBER]

#define LINIF_GET_NC_IDENTIFICATION(ch) \
    LINIF_GET_CHANNEL(ch).LinIfNodeType->LinIfSlave->LinIfNodeConfigurationIdentification

#define LINIF_GET_RESPONSE_ERROR_SIGNAL(ch) LINIF_GET_CHANNEL(ch).LinIfNodeType->LinIfSlave->LinIfResponseErrorSignal

#define LINIF_GET_RESPONSE_ERROR_SIG_CONFIGURED(ch) \
    LINIF_GET_CHANNEL(ch).LinIfNodeType->LinIfSlave->LinIfResponseErrorSignalConfigured

#define USER_GOTO_SLEEP_INDICATION(ch, channel) LINIF_GET_CHANNEL(ch).GotoSleepIndication(channel)
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef enum
{
    LINIF_SLAVE_FRAME_IDLE,
    LINIF_SLAVE_FRAME_RESPONSE /*Response transmitting or receiving*/
} LinIf_SlaveFrameStatusType;

typedef struct
{
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) curFrame;
    uint16 nasTimer;
    uint16 busIdleTimer;
    uint16 eventTrgTxFrmIdx;
    LinIf_ChannelStateType channelState;
    LinIf_SlaveFrameStatusType frameStatus;
    boolean ncResponsePendingFlag;
    uint8 ncResponse[LINIF_NC_RESPONSE_LEN];
    boolean responseError;
    boolean wakeupFlag;
    boolean waitWakeupConfirmFlag;
    boolean gotoSleepConfirmationFlag;
} LinIf_SlaveRuntimeType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveUnconditionalHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveEventTriggeredHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveMRFHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveSRFHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinIf_SlaveUnconditionalRxHandle(
    NetworkHandleType ch,
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinIf_SlaveMRFRxHandle(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcAssignNAD(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);
#endif

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcReadbyIdentifier(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcSaveConfiguration(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcAssignFrameIdRange(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
);

static FUNC(void, LINIF_CODE) LinIf_SlaveSetResponseErrorBit(NetworkHandleType ch, boolean responseError);

static FUNC(void, LINIF_CODE) LinIf_SlaveSetLinPduType(
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr, /* PRQA S 3432 */
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) frame);

static FUNC(void, LINIF_CODE) LinIf_SlaveBuildNCResponse(NetworkHandleType ch, uint8 nad, uint8 sid, uint8 identifier);

static FUNC(void, LINIF_CODE) LinIf_SlaveTimerHandle(NetworkHandleType ch);

static FUNC(void, LINIF_CODE) LinIf_SlaveResetRtData(NetworkHandleType ch);
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if !defined(LINIF_LOCAL)
#define LINIF_LOCAL static
#endif

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"
LINIF_LOCAL VAR(uint8, LINIF_VAR) LinIf_SlavePIDTable[LINIF_SLAVE_FRAME_NUM];

LINIF_LOCAL VAR(uint8, LINIF_VAR) LinIf_SlaveConfiguredNAD[LINIF_SLAVE_CHANNEL_NUMBER];

LINIF_LOCAL VAR(LinIf_SlaveRuntimeType, LINIF_VAR) LinIf_SlaveRTData[LINIF_SLAVE_CHANNEL_NUMBER];

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "LinIf_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the LinIf slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinIf_SlaveInit(void)
{
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST) ncIdentifi;
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) framePtr;
    P2CONST(LinIf_ChannelType, AUTOMATIC, LINIF_VAR) chCfgPtr;
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) slaveRTDataPtr; /* PRQA S 3432 */
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) pidTablePtr;               /* PRQA S 3432 */
    NetworkHandleType ch;
    uint16 frmIdx;

    for (ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
    {
        LinIf_SlaveResetRtData(ch);

        chCfgPtr = &LINIF_GET_CHANNEL(ch);
        slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
        /*@req <SWS_LinIf_00507>*/
        if (LINIF_STARTUP_NORMAL == chCfgPtr->LinIfStartupState)
        {
            slaveRTDataPtr->channelState = LINIF_CHANNEL_OPERATIONAL;

            /*@req <SWS_LinIf_00752>*/
            /* Start the bus idle timeout observation */
            slaveRTDataPtr->busIdleTimer = chCfgPtr->LinIfBusIdleTimeoutCnt;
        }
        else
        {
            slaveRTDataPtr->channelState = LINIF_CHANNEL_SLEEP;
            slaveRTDataPtr->busIdleTimer = 0u;
        }

        /* Initial Pid table */
        pidTablePtr = LINIF_GET_CHANNEL_PIDTABLE(ch);
        for (frmIdx = 0u; frmIdx < LINIF_GET_FRAME_NUM(ch); frmIdx++)
        {
            framePtr = &LINIF_GET_FRAME(ch, frmIdx);
            pidTablePtr[frmIdx] = framePtr->LinIfProtectedId;
        }

        /* Initial configed NAD */
        ncIdentifi = LINIF_GET_NC_IDENTIFICATION(ch);
        if (NULL_PTR != ncIdentifi)
        {
            LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = ncIdentifi->LinIfInitialNAD;
        }

        slaveRTDataPtr->ncResponsePendingFlag = FALSE;
        slaveRTDataPtr->responseError = FALSE;
        (void)ILib_memset(slaveRTDataPtr->ncResponse, 0, LINIF_NC_RESPONSE_LEN);
    }
}

/******************************************************************************/
/*
 * Brief               Transmit request process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      frameIdx: Frame Index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveTransmit(/* PRQA S 1532 */
                    uint16 frameIdx)
{

    /* Locked */
    SchM_Enter_LinIf_ExclusiveArea_Channel();

    /*@req <SWS_LinIf_00341>,<SWS_LinIf_00730>*/
    /* Set transmit request flag */
    LINIF_GET_TRANSMIT_REQ(frameIdx) = TRUE;

    /* Unlocked */
    SchM_Exit_LinIf_ExclusiveArea_Channel();
    return E_OK;
}

/******************************************************************************/
/*
 * Brief               Shutdowns the LINTP slave node
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE) LinTp_SlaveShutdown(void) /* PRQA S 1532 */
{
    LinIf_SlaveInit();
}

/******************************************************************************/
/*
 * Brief               Wakeup process in slave channel.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveWakeUp(/* PRQA S 1532 */
                  NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 linDriver = LINIF_GET_LIN_DRIVER_ID(ch);
    uint8 linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);
    Std_ReturnType ret;

    if (LINIF_CHANNEL_SLEEP == slaveRTDataPtr->channelState)
    {
        if (TRUE == slaveRTDataPtr->wakeupFlag)
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
    else if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->channelState)
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
            slaveRTDataPtr->gotoSleepConfirmationFlag = FALSE;
        }
    }

    /*@req <SWS_LinIf_00720>,<SWS_LinIf_00721>*/
    if (E_OK == ret)
    {
        /*@req <SWS_LinIf_00714>*/
        slaveRTDataPtr->wakeupFlag = FALSE;
        /* Wait LinIf_HeaderIndication */
        slaveRTDataPtr->waitWakeupConfirmFlag = TRUE;
    }
    else
    {
        /*@req <SWS_LinIf_00762>*/
        USER_WAKEUP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), FALSE);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Wakeup confirmation process in slave node
 * Param-Name[in]: ch
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveWakeupConfirmation(/* PRQA S 1532 */
                              EcuM_WakeupSourceType WakeupSource)
{
#if (                                            \
    (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON) \
    || ((LINIF_TRCV_DRIVER_SUPPORTED == STD_ON) && (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON)))
    LinIf_SlaveRuntimeType* slaveRTDataPtr;
    EcuM_WakeupSourceType wakeupSource;
    NetworkHandleType ch;
#endif

#if (LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON)
    /*check all Lin driver channel to find the wake-up source*/
    for (ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
    {
        wakeupSource = LINIF_GET_LIN_DRIVER_WAKEUP_SOURCE(ch);
        if (0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
            slaveRTDataPtr->wakeupFlag = TRUE;
            break;
        }
    }
#endif

#if ((LINIF_TRCV_DRIVER_SUPPORTED == STD_ON) && (LINIF_LIN_TRCV_WAKEUP_SUPPORT == STD_ON))
    /*check all Lin transceiver channel to find the wake-up source*/
    for (ch = LINIF_MASTER_CHANNEL_NUMBER; ch < LINIF_NUMBER_OF_CHANNELS; ch++)
    {
        wakeupSource = LINIF_GET_LIN_TRCV_WAKEUP_SOURCE(ch);
        if (0u != (WakeupSource & wakeupSource))
        {
            /*@req <SWS_LinIf_00716>*/
            slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
            slaveRTDataPtr->wakeupFlag = TRUE;
            break;
        }
    }
#endif
}

/******************************************************************************/
/*
 * Brief               Copy PID table to provide 'PdiBuffer'
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     PidBuffer: Pointer to existing buffer to which the
 *                                current assigned PID values are copied to.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveGetPIDTable(/* PRQA S 1532 */
                       NetworkHandleType ch,
                       P2VAR(Lin_FramePidType, AUTOMATIC, LINIF_APPL_DATA) PidBuffer /* PRQA S 3432 */
)
{
    uint16 pidIdx = LINIF_GET_FRAME_OFS(ch) - LINIF_MASTER_FRAME_NUM;
    uint16 pidTableLen = LINIF_GET_FRAME_NUM(ch);

    (void)ILib_memcpy(PidBuffer, &LinIf_SlavePIDTable[pidIdx], pidTableLen);
}

/******************************************************************************/
/*
 * Brief               Set 'PdiBuffer' pointed PID table to internal PID table.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     PidBuffer: Pointer to buffer which contains the PID
 *                                values to configure.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveSetPIDTable(/* PRQA S 1532 */
                       NetworkHandleType ch,
                       const Lin_FramePidType* PidBuffer)
{
    uint16 pidIdx = LINIF_GET_FRAME_OFS(ch) - LINIF_MASTER_FRAME_NUM;
    uint16 pidTableLen = LINIF_GET_FRAME_NUM(ch);

    (void)ILib_memcpy(&LinIf_SlavePIDTable[pidIdx], PidBuffer, pidTableLen);
}

/******************************************************************************/
/*
 * Brief               Get the current configured NAD.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     Nad: Configured NAD of slave.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveGetConfiguredNAD(/* PRQA S 1532 */
                            NetworkHandleType ch,
                            P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Nad /* PRQA S 3432 */
)
{

    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == LINIF_MASTER_SUPPORT)
    if (ch >= LINIF_MASTER_CHANNEL_NUMBER)
#endif /* STD_ON == LINIF_MASTER_SUPPORT */
    {
        *Nad = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Sets the current configured NAD.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Nad: Configured NAD of slave.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveSetConfiguredNAD(/* PRQA S 1532 */
                            NetworkHandleType ch,
                            uint8 Nad)
{
    LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = Nad;
}

/******************************************************************************/
/*
 * Brief               Header receive process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     Nad: Configured NAD of slave.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE)
LinIf_SlaveHeaderIndication(/* PRQA S 1532 */
                            NetworkHandleType ch,
                            P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    Std_ReturnType ret = E_NOT_OK;
    boolean irrelevantFlag = TRUE;
    const LinIf_ChannelType* chCfgPtr = &LINIF_GET_CHANNEL(ch);

    if (slaveRTDataPtr->waitWakeupConfirmFlag)
    {
        slaveRTDataPtr->waitWakeupConfirmFlag = FALSE;

        /*@req <SWS_LinIf_00761>*/
        chCfgPtr->WakeupConfirmation(chCfgPtr->LinIfComMNetworkHandleRef, TRUE);

        slaveRTDataPtr->channelState = LINIF_CHANNEL_OPERATIONAL;
    }

    /*@req <SWS_LinIf_00189>*/
    /* Receive/transmit LIN frame headers and responses only in the state
       LINIF_CHANNEL_OPERATIONAL*/
    if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->channelState)
    {
        uint16 frmIdx;
        uint16 frmNum = chCfgPtr->LinIfNumOfFrame;
        const LinIf_FrameType* framePtr = chCfgPtr->LinIfFrame;
        const uint8* pidTablePtr = LINIF_GET_CHANNEL_PIDTABLE(ch);
        for (frmIdx = 0u; frmIdx < frmNum; frmIdx++)
        {
            if (pidTablePtr[frmIdx] == PduPtr->Pid)
            {
                irrelevantFlag = FALSE;

                switch (framePtr->LinIfFrameType)
                {
                case LINIF_UNCONDITIONAL:
                    ret = LinIf_SlaveUnconditionalHeaderHandle(ch, framePtr, PduPtr);
                    break;

                case LINIF_EVENT_TRIGGERED:
                    ret = LinIf_SlaveEventTriggeredHeaderHandle(ch, framePtr, PduPtr);
                    break;

                case LINIF_MRF:
                    ret = LinIf_SlaveMRFHeaderHandle(ch, framePtr, PduPtr);
                    break;

                case LINIF_SRF:
                    ret = LinIf_SlaveSRFHeaderHandle(ch, framePtr, PduPtr);
                    break;

                default:
                    /* Other Frame Type */
                    break;
                }

                /* Response transmitting/receiving,save info to runtime data */
                if (E_OK == ret)
                {
                    /* Set frame status to response handle */
                    slaveRTDataPtr->frameStatus = LINIF_SLAVE_FRAME_RESPONSE;
                    /* Save current frame */
                    slaveRTDataPtr->curFrame = framePtr;
                }
                break;
            }
            framePtr++;
        }

        if (irrelevantFlag)
        {
            /*@req <SWS_LinIf_00748>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */

            ret = E_OK;
        }

        /*@req <SWS_LinIf_00752>,<SWS_LinIf_00754>*/
        /* Reload the running bus idle timer*/
        slaveRTDataPtr->busIdleTimer = chCfgPtr->LinIfBusIdleTimeoutCnt;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Receive indication process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveRxIndication(/* PRQA S 1532 */
                        NetworkHandleType ch,
                        P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    LinIf_SlaveRuntimeType* slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    const LinIf_FrameType* framePtr;

    if ((LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
        && (LINIF_RX_PDU == slaveRTDataPtr->curFrame->LinIfPduDirection->LinIfPduDirectionId))
    {
        /*@req <SWS_LinIf_00189>*/
        /* Receive/transmit LIN frame headers and responses only in the state
        LINIF_CHANNEL_OPERATIONAL*/
        if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->channelState)
        {
            framePtr = slaveRTDataPtr->curFrame;
            switch (framePtr->LinIfFrameType)
            {
            case LINIF_UNCONDITIONAL:
                LinIf_SlaveUnconditionalRxHandle(ch, Lin_SduPtr);
                break;

            case LINIF_MRF:
                LinIf_SlaveMRFRxHandle(ch, Lin_SduPtr);
                break;

            default:
                /* Other Frame Type */
                break;
            }

            /*@req <SWS_LinIf_00754>*/
            /* Reload the running bus idle timer*/
            slaveRTDataPtr->busIdleTimer = LINIF_GET_CHANNEL(ch).LinIfBusIdleTimeoutCnt;
        }
    }
    else
    {
        /*@req <SWS_LinIf_00848>*/
        /*No response reception is expected,return without further action.*/
    }
}

/******************************************************************************/
/*
 * Brief               Tx confirmation process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveTxConfirmation(/* PRQA S 1532 */
                          NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr =
        /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
        LINIF_GET_SLAVE_RTDATA_PTR(ch);
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr = slaveRTDataPtr->curFrame;
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) subFramePtr;
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_CONST) txPduPtr;

    if ((LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->channelState)
        && (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
        && (LINIF_TX_PDU == slaveRTDataPtr->curFrame->LinIfPduDirection->LinIfPduDirectionId))
    {
        switch (framePtr->LinIfFrameType)
        {
        case LINIF_UNCONDITIONAL:
        case LINIF_EVENT_TRIGGERED:
            /*@req <SWS_LinIf_00732>*/
            /* Clear pending flag */
            if (LINIF_UNCONDITIONAL == framePtr->LinIfFrameType)
            {
                *framePtr->LinIfIsTransmitPending = FALSE;
                txPduPtr = framePtr->LinIfPduDirection->LinIfTxPdu;
            }
            else
            {
                subFramePtr = &LINIF_GET_GLOBAL_FRAME(slaveRTDataPtr->eventTrgTxFrmIdx);
                *subFramePtr->LinIfIsTransmitPending = FALSE;
                txPduPtr = subFramePtr->LinIfPduDirection->LinIfTxPdu;
            }

            /*@req <SWS_LinIf_00741>*/
            txPduPtr->LinIfTxConfirmationUL(txPduPtr->LinIfTxPduRef);

            if (txPduPtr->LinIfContainResponseErrorSignal)
            {
                /*@req <SWS_LinIf_00747>*/
                LinIf_SlaveSetResponseErrorBit(ch, FALSE);
            }

            /* Transmit finish,Reset channel runtime data */
            LinIf_SlaveResetRtData(ch);
            break;

        case LINIF_SRF:
            if (slaveRTDataPtr->ncResponsePendingFlag)
            {
                /*@req <SWS_LinIf_00734> */
                /* Stop NAS timeout timer */
                slaveRTDataPtr->nasTimer = 0u;

                /* Transmit finish,Reset channel runtime data */
                LinIf_SlaveResetRtData(ch);
                slaveRTDataPtr->ncResponsePendingFlag = FALSE;
                (void)ILib_memset(slaveRTDataPtr->ncResponse, 0, LINIF_NC_RESPONSE_LEN);
            }
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
            else
            {
                boolean isTpTxFinished;
                LinTp_SlaveTxConfirmation(ch, &isTpTxFinished);
                if (isTpTxFinished)
                {
                    /* TP transmit finish,Reset channel runtime data */
                    LinIf_SlaveResetRtData(ch);
                }
            }
#endif
            break;

        default:
            /* Other Frame Type */
            break;
        }

        /*@req <SWS_LinIf_00754>*/
        /* Reload the running bus idle timer*/
        slaveRTDataPtr->busIdleTimer = LINIF_GET_CHANNEL(ch).LinIfBusIdleTimeoutCnt;
    }
    else
    {
        /*@req <SWS_LinIf_00852>*/
        /*No response reception is expected,return without further action.*/
    }
}

/******************************************************************************/
/*
 * Brief               Lin error indication process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel.
 *                     ErrorStatus: Type of detected error.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveLinErrorIndication(/* PRQA S 1532 */
                              NetworkHandleType ch,
                              Lin_SlaveErrorType ErrorStatus)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    LinIf_PduDirectionIdType LinIfPduDirectionId;

    if ((NULL_PTR != slaveRTDataPtr) && (NULL_PTR != slaveRTDataPtr->curFrame))
    {
        LinIfPduDirectionId = slaveRTDataPtr->curFrame->LinIfPduDirection->LinIfPduDirectionId;

        if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
        {
            if (LIN_ERR_HEADER != ErrorStatus)
            {
                /*@req <SWS_LinIf_00743>,<SWS_LinIf_00735>*/
                LinIf_ReportRuntimeError(LINIF_LINERRORINDICATION_ID, LINIF_E_RESPONSE);
            }

            /*@req <SWS_LinIf_00744>,<SWS_LinIf_00736>*/
            if ((LIN_ERR_RESP_STOPBIT == ErrorStatus) || (LIN_ERR_RESP_CHKSUM == ErrorStatus)
                || (LIN_ERR_RESP_DATABIT == ErrorStatus)
                || ((LIN_ERR_INC_RESP == ErrorStatus) && (LINIF_RX_PDU == LinIfPduDirectionId)))
            {
                LinIf_SlaveSetResponseErrorBit(ch, TRUE);
            }
        }

        /*@req <SWS_LinIf_00753>,<SWS_LinIf_00754>*/
        /* Reload the running bus idle timer in OPERATION state */
        if (LINIF_CHANNEL_OPERATIONAL == slaveRTDataPtr->channelState)
        {
            slaveRTDataPtr->busIdleTimer = LINIF_GET_CHANNEL(ch).LinIfBusIdleTimeoutCnt;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Go-to-sleep process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, LINIF_CODE)
LinIf_SlaveGotoSleep(/* PRQA S 1532 */
                     NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 linDriver = LINIF_GET_LIN_DRIVER_ID(ch);
    uint8 linChannel = LINIF_GET_LIN_CHANNEL_ID(ch);

    /*@req <SWS_LinIf_00757>*/
    /* When LinIf_GotoSleep is called, Lin_GotoSleepInternal shall be called
       directly */
    (void)Lin_DriverApi[linDriver].LinGoToSleepInternal(linChannel);

    /*@req <SWS_LinIf_00758>*/
    /* Clear the wakeup flag */
    slaveRTDataPtr->wakeupFlag = FALSE;

    /* Set flag to call GoToSleepConfirmation() in MainFunction */
    slaveRTDataPtr->gotoSleepConfirmationFlag = TRUE;
}

/******************************************************************************/
/*
 * Brief               Main function of slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void LinIf_SlaveMainFunction(NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    LinIf_SlaveTimerHandle(ch);

    if (TRUE == slaveRTDataPtr->gotoSleepConfirmationFlag)
    {
        /* Change channel state to sleep */
        slaveRTDataPtr->channelState = LINIF_CHANNEL_SLEEP;
        slaveRTDataPtr->gotoSleepConfirmationFlag = FALSE;

        /*@req <SWS_LinIf_00753>*/
        /* LINIF_CHANNEL_SLEEP is entered,stop the bus idle timeout observation */
        slaveRTDataPtr->busIdleTimer = 0u;

        /*@req <SWS_LinIf_00759>*/
        /* After calling the function Lin_GoToSleepInternal, the LIN Interface shall
           invoke the function <User>_GotoSleepConfirmation with parameter TRUE.*/
        USER_GOTO_SLEEP_CONFIRMATION(ch, LINIF_GET_COMM_NETWORK(ch), TRUE);
    }
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               Unconditional frame process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     framePtr: Pointed to configed LinIfFrameType
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              E_OK: No error
 *                     E_NOT_OK:
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveUnconditionalHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Std_ReturnType ret = E_NOT_OK;
    const LinIf_SlaveRuntimeType* slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_CONST) txPduPtr;
    PduInfoType pduInfo;

    if (LINIF_RX_PDU == framePtr->LinIfPduDirection->LinIfPduDirectionId)
    {
        if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
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
            LinIf_Det_ReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
        }
        else
        {
            if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
            {
                /*@req <SWS_LinIf_00846>*/
                LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                LinIf_SlaveResetRtData(ch);
            }

            txPduPtr = framePtr->LinIfPduDirection->LinIfTxPdu;
            /*@req <SWS_LinIf_00738>*/
            /* Get the data part of the frame from upper layer */
            pduInfo.SduDataPtr = PduPtr->SduPtr;
            pduInfo.SduLength = framePtr->LinIfLength;
            ret = txPduPtr->LinIfTxTriggerTransmitUL(txPduPtr->LinIfTxPduRef, &pduInfo);

            if (E_NOT_OK == ret)
            {
                /*@req <SWS_LinIf_00740>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
                PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
            }
            else
            {
                /* Set Uncondition frame PID to first byte */
                uint8 uncondFrmIdWithEvent = framePtr->LinIfFrameIdAssociatedWithEvent;
                if (uncondFrmIdWithEvent != 0xffu)
                {
                    PduPtr->SduPtr[0] = LinIf_SlavePIDTable[uncondFrmIdWithEvent - LINIF_MASTER_FRAME_NUM];
                }
                /*@req <SWS_LinIf_00739>*/
                LinIf_SlaveSetLinPduType(PduPtr, framePtr);
            }

            ret = E_OK;
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Event-Triggered frame process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     framePtr: Pointed to configed LinIfFrameType
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveEventTriggeredHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    P2CONST(LinIf_SubstitutionFramesType, AUTOMATIC, LINIF_CONST) subFrmPtr;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) uncondFrmPtr;
    P2CONST(LinIf_TxPduType, AUTOMATIC, LINIF_CONST) txPduPtr;
    PduInfoType pduInfo;
    uint16 frameIdx;
    Std_ReturnType ret = E_NOT_OK;
    uint8 idx;
    uint8 uncondFrmPid;

    subFrmPtr = framePtr->LinIfSubstitutionFrames;
    for (idx = 0; idx < framePtr->LinIfNumOfSubstitutionFrame; idx++)
    {
        frameIdx = subFrmPtr->LinIfSubstitutionFrameRef;
        if (TRUE == LINIF_GET_TRANSMIT_REQ(frameIdx))
        {
            if (NULL_PTR == PduPtr->SduPtr)
            {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                /*@req <SWS_LinIf_00845>*/
                LinIf_Det_ReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
            }
            else
            {
                if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
                {
                    /*@req <SWS_LinIf_00846>*/
                    LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                    LinIf_SlaveResetRtData(ch);
                }

                /*@req <SWS_LinIf_00731>*/
                /* Set Unconditional frame response to the following bytes.*/
                uncondFrmPtr = &LINIF_GET_GLOBAL_FRAME(frameIdx);
                txPduPtr = uncondFrmPtr->LinIfPduDirection->LinIfTxPdu;
                /*@req <SWS_LinIf_00738>*/
                /* Get the data part of the frame from upper layer */
                pduInfo.SduDataPtr = PduPtr->SduPtr;
                pduInfo.SduLength = uncondFrmPtr->LinIfLength;
                ret = txPduPtr->LinIfTxTriggerTransmitUL(txPduPtr->LinIfTxPduRef, &pduInfo);

                if (E_NOT_OK == ret)
                {
                    /*@req <SWS_LinIf_00740>*/
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
                    PduPtr->Drc = LIN_FRAMERESPONSE_IGNORE;
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
                    PduPtr->Drc = LIN_SLAVE_TO_SLAVE;
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
                }
                else
                {
                    /*@req <SWS_LinIf_00739>*/
                    LinIf_SlaveSetLinPduType(PduPtr, framePtr);
                    PduPtr->Dl = uncondFrmPtr->LinIfLength;

                    /* Set Uncondition frame PID to first byte */
                    uncondFrmPid = LinIf_SlavePIDTable[frameIdx - LINIF_MASTER_FRAME_NUM];
                    PduPtr->SduPtr[0u] = uncondFrmPid;

                    /* Backup frame index transmit in event-triggered frame */
                    slaveRTDataPtr->eventTrgTxFrmIdx = frameIdx;
                }

                ret = E_OK;
            }
            break;
        }
        subFrmPtr++;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               MRF frame process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     framePtr: Pointed to configed LinIfFrameType
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveMRFHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /*@req <SWS_LinIf_00789>*/
    if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
    {
        /*@req <SWS_LinIf_00846>*/
        LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

        LinIf_SlaveResetRtData(ch);
    }

    /*@req <SWS_LinIf_00733> */
    LinIf_SlaveSetLinPduType(PduPtr, framePtr);

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               SRF frame process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     framePtr: Pointed to configed LinIfFrameType
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveSRFHeaderHandle(
    NetworkHandleType ch,
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) framePtr,
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr /* PRQA S 3432 */
)
{
    const LinIf_SlaveRuntimeType* slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    Std_ReturnType ret = E_NOT_OK;

    /*@req <SWS_LinIf_00775>*/
    if (TRUE == slaveRTDataPtr->ncResponsePendingFlag)
    {
        if (NULL_PTR == PduPtr->SduPtr)
        {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            /*@req <SWS_LinIf_00845>*/
            LinIf_Det_ReportError(LINIF_HEADERINDICATION_ID, LINIF_E_PARAM_POINTER);
#endif
        }
        else
        {
            if (LINIF_SLAVE_FRAME_RESPONSE == slaveRTDataPtr->frameStatus)
            {
                /*@req <SWS_LinIf_00846>*/
                LinIf_ReportRuntimeError(LINIF_HEADERINDICATION_ID, LINIF_E_RESPONSE);

                LinIf_SlaveResetRtData(ch);
            }

            /*@req <SWS_LinIf_00733> */
            LinIf_SlaveSetLinPduType(PduPtr, framePtr);
            /* Copy response to PduPtr->SduPtr */
            (void)ILib_memcpy(PduPtr->SduPtr, slaveRTDataPtr->ncResponse, LINIF_NC_RESPONSE_LEN);

            ret = E_OK;
        }
    }
    else
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        if (!LinTp_SlaveCheckFunctionAddressFlag(ch))
        {
            /*@req <SWS_LinIf_00776>*/
            LinIf_SlaveSetLinPduType(PduPtr, framePtr);
        }
        /*@req <SWS_LinIf_00776>*/
        LinTp_SlaveSRFIndication(ch, PduPtr);
        if (LinTp_SlaveCheckFunctionAddressFlag(ch))
        {
            /* Functional addressing with SF,no respond */
            LinTp_SlaveSetFunctionAddressFlag(ch, FALSE);
            LinIf_SlaveResetRtData(ch);
        }
        else
        {
#if (LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440)
            if (LIN_FRAMERESPONSE_TX == PduPtr->Drc)
#else  /* The default version of lin driver is 4.2.2 / 4.3.1 */
            if (LIN_MASTER_RESPONSE == PduPtr->Drc)
#endif /* LINIF_LIN_AUTOSAR_VERSION >= LINIF_LIN_AUTOSAR_440 */
            {
                ret = E_OK;
            }
        }
#endif
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Unconditional response receive process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveUnconditionalRxHandle(
    NetworkHandleType ch,
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2CONST(LinIf_RxPduType, AUTOMATIC, LINIF_DATA) rxPduPtr;
    const LinIf_SlaveRuntimeType* slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST)
    framePtr = slaveRTDataPtr->curFrame;
    PduInfoType pduInfo;

    /* Locked */
    SchM_Enter_LinIf_ExclusiveArea_Channel();

    /* Save PDU information */
    pduInfo.SduDataPtr = Lin_SduPtr;
    pduInfo.SduLength = framePtr->LinIfLength;

    /*@req <SWS_LinIf_00734> */
    /* Notify upper */
    rxPduPtr = framePtr->LinIfPduDirection->LinIfRxPdu;
    rxPduPtr->LinIfRxIndicationUL(rxPduPtr->LinIfRxPduRef, &pduInfo);

    /* Receive finish,Reset channel runtime data */
    LinIf_SlaveResetRtData(ch);

    /* Unlocked */
    SchM_Exit_LinIf_ExclusiveArea_Channel();
}

/******************************************************************************/
/*
 * Brief               MRF response receive process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveMRFRxHandle(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST)
    ncIdentifi = LINIF_GET_NC_IDENTIFICATION(ch);
    Std_ReturnType ret;
    boolean tpFlag = FALSE;

    if (LINIF_NC_FUNCTIONAL_REQ_NAD != Lin_SduPtr[LINIF_NC_NAD_POS])
    {
        /* If new MRF received,drop pending NC response */
        slaveRTDataPtr->ncResponsePendingFlag = FALSE;
    }

    /*@req <SWS_LinIf_00750> */
    /* Go-to-sleep command */
    if (0x00u == Lin_SduPtr[LINIF_NC_NAD_POS])
    {
        /*@req <SWS_LinIf_00756> */
        /* Notify upper layer by invoke function <User>_GotoSleepIndication */
        USER_GOTO_SLEEP_INDICATION(ch, LINIF_GET_COMM_NETWORK(ch));

        slaveRTDataPtr->channelState = LINIF_CHANNEL_GOTO_SLEEP;
    }
    else
    {
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        if (((0x06u == Lin_SduPtr[LINIF_NC_PCI_POS])
             && ((LINIF_NC_SID_ASSIGNNAD == Lin_SduPtr[LINIF_NC_SID_POS])
                 || (LINIF_NC_SID_ASSIGNFRAMEIDRANGE == Lin_SduPtr[LINIF_NC_SID_POS])
                 || ((LINIF_NC_SID_READBYIDENTIFIER == Lin_SduPtr[LINIF_NC_SID_POS])
                     && ((0u == Lin_SduPtr[LINIF_NC_DATA1_POS]) || (2u == Lin_SduPtr[LINIF_NC_DATA1_POS])))))
            || ((0x01u == Lin_SduPtr[LINIF_NC_PCI_POS])
                && (LINIF_NC_SID_SAVECONFIGURATION == Lin_SduPtr[LINIF_NC_SID_POS])))
        {
            /* If LinTp response is pending,receive NC request,cancel TP process */
            LinTp_SlavePreInit(ch - LINIF_MASTER_CHANNEL_NUMBER);
#endif /* LINTP_SLAVE_SUPPORT == STD_ON */

            switch (Lin_SduPtr[LINIF_NC_SID_POS])
            {

            case LINIF_NC_SID_ASSIGNNAD:
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
                ret = LinIf_SlaveNcAssignNAD(ch, Lin_SduPtr);
#endif
                break;

            case LINIF_NC_SID_READBYIDENTIFIER:
                ret = LinIf_SlaveNcReadbyIdentifier(ch, Lin_SduPtr);
                break;

            case LINIF_NC_SID_SAVECONFIGURATION:
                ret = LinIf_SlaveNcSaveConfiguration(ch, Lin_SduPtr);
                break;

            case LINIF_NC_SID_ASSIGNFRAMEIDRANGE:
                ret = LinIf_SlaveNcAssignFrameIdRange(ch, Lin_SduPtr);
                break;

            default:
                break;
            }
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
        }
        else
        {
            tpFlag = TRUE;
            LinTp_SlaveMRFIndication(ch, Lin_SduPtr);
            LinIf_SlaveResetRtData(ch);
        }
#endif

        /*@req <SWS_LinIf_00734> */
        if ((!tpFlag) && (E_OK == ret))
        {
            /* Start N_As timer after receive a valid node configuration request*/
            slaveRTDataPtr->nasTimer = ncIdentifi->LinIfNasTimeoutCnt;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Read by Identifier process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Receive a vaild node configuration request and
 *                           the request is successfully processed.
 *                     E_NOT_OK: Receive a invaild node configuration request or
 *                               the request is not successfully processed.
 */
/******************************************************************************/
#if (LINIF_NC_OPTIONAL_REQUEST_SUPPORTED == STD_ON)
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcAssignNAD(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST)
    ncIdentifi = LINIF_GET_NC_IDENTIFICATION(ch);
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    uint8 nad = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8 pci = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8 sid = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8 newNAD = Lin_SduPtr[LINIF_NC_DATA5_POS];
    uint16 supplierID;
    uint16 functionID;
    Std_ReturnType ret = E_NOT_OK;

    supplierID = Lin_SduPtr[LINIF_NC_DATA2_POS];
    supplierID = (supplierID << 8) | Lin_SduPtr[LINIF_NC_DATA1_POS];
    functionID = Lin_SduPtr[LINIF_NC_DATA4_POS];
    functionID = (functionID << 8) | Lin_SduPtr[LINIF_NC_DATA3_POS];
    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((ncIdentifi->LinIfInitialNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (6u == pci)
        && ((ncIdentifi->LinIfSupplierId == supplierID) || (LINIF_NC_WILDCARD_SUPPLIERID == supplierID))
        && ((ncIdentifi->LinIfFunctionId == functionID) || (LINIF_NC_WILDCARD_FUNCTIONID == functionID)))
    {
        /*@req <SWS_LinIf_00779> */
        /* Update configured NAD with the received new NAD */
        LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER] = newNAD;

        LinIf_SlaveResetRtData(ch);
        /* Build positive response for next SRF */
        slaveRTDataPtr->ncResponsePendingFlag = TRUE;
        LinIf_SlaveBuildNCResponse(ch, ncIdentifi->LinIfInitialNAD, sid, 0xFFu);

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
        LinIf_SlaveResetRtData(ch);
    }

    return ret;
}
#endif

/******************************************************************************/
/*
 * Brief               Assign NAD process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Receive a vaild node configuration request and
 *                           the request is successfully processed.
 *                     E_NOT_OK: Receive a invaild node configuration request or
 *                               the request is not successfully processed.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcReadbyIdentifier(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST)
    ncIdentifi = LINIF_GET_NC_IDENTIFICATION(ch);
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    uint8 nad = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8 pci = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8 sid = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8 identifier = Lin_SduPtr[LINIF_NC_DATA1_POS];
    uint8 configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    uint16 supplierID;
    uint16 functionID;
    Std_ReturnType ret = E_NOT_OK;

    supplierID = Lin_SduPtr[LINIF_NC_DATA3_POS];
    supplierID = (supplierID << 8) | Lin_SduPtr[LINIF_NC_DATA2_POS];
    functionID = Lin_SduPtr[LINIF_NC_DATA5_POS];
    functionID = (functionID << 8) | Lin_SduPtr[LINIF_NC_DATA4_POS];
    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (6u == pci)
        && ((ncIdentifi->LinIfSupplierId == supplierID) || (LINIF_NC_WILDCARD_SUPPLIERID == supplierID))
        && ((ncIdentifi->LinIfFunctionId == functionID) || (LINIF_NC_WILDCARD_FUNCTIONID == functionID)))
    {
        if ((0u == identifier) || (2u == identifier))
        {
            LinIf_SlaveResetRtData(ch);
            /*@req <SWS_LinIf_00783>,<SWS_LinIf_00841> */
            /* Build positive/negative response for next SRF */
            slaveRTDataPtr->ncResponsePendingFlag = TRUE;
            LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, identifier);

            ret = E_OK;
        }
        else
        {
/* Identifier not directly support by LinIf Node configuration */
/* Forwarded over Transport Layer to upper layer */
#if ((LINIF_TP_SUPPORTED == STD_ON) && (LINTP_SLAVE_SUPPORT == STD_ON))
            LinTp_SlaveMRFIndication(ch, Lin_SduPtr);
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
        LinIf_SlaveResetRtData(ch);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Assign NAD process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Receive a vaild node configuration request and
 *                           the request is successfully processed.
 *                     E_NOT_OK: Receive a invaild node configuration request or
 *                               the request is not successfully processed.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcSaveConfiguration(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR) /* PRQA S 3432 */
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    uint8 nad = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8 pci = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8 sid = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8 configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    Std_ReturnType ret = E_NOT_OK;

    /*@req <SWS_LinIf_00771> */
    /* Check if valid node configuration request */
    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (1u == pci))
    {
/*@req <SWS_LinIf_00782> */
#if defined(LinIfSaveConfigurationCallout)
        LinIfSaveConfigurationCallout(ch);
#endif

        LinIf_SlaveResetRtData(ch);
        /* Build positive response for next SRF */
        slaveRTDataPtr->ncResponsePendingFlag = TRUE;
        LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, 0xFFu);

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
        LinIf_SlaveResetRtData(ch);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Assign Frame Id Range process in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     Lin_SduPtr: pointer to a buffer where the current SDU is
 *                                 stored.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Receive a vaild node configuration request and
 *                           the request is successfully processed.
 *                     E_NOT_OK: Receive a invaild node configuration request or
 *                               the request is not successfully processed.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, LINIF_CODE) LinIf_SlaveNcAssignFrameIdRange(
    NetworkHandleType ch,
    P2CONST(uint8, AUTOMATIC, LINIF_APPL_DATA) Lin_SduPtr /* PRQA S 3432 */
)
{
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_APPL_CONST) framePtr;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, LINIF_APPL_DATA)
    pidTablePtr = LINIF_GET_CHANNEL_PIDTABLE(ch);
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 frmIdx;
    uint16 frmNumMax;
    uint8 pidTableBak[4u];
    uint8 assignPidIdx;
    uint8 restoreIdx;
    uint8 pid;
    uint8 nad = Lin_SduPtr[LINIF_NC_NAD_POS];
    uint8 pci = Lin_SduPtr[LINIF_NC_PCI_POS];
    uint8 sid = Lin_SduPtr[LINIF_NC_SID_POS];
    uint8 configedNAD = LinIf_SlaveConfiguredNAD[ch - LINIF_MASTER_CHANNEL_NUMBER];
    Std_ReturnType ret = E_NOT_OK;

    if (((configedNAD == nad) || (LINIF_NC_WILDCARD_NAD == nad)) && (6u == pci))
    {
        /*@req <SWS_LinIf_00781> */
        framePtr = &LINIF_GET_FRAME(ch, 0u);
        frmNumMax = LINIF_GET_FRAME_NUM(ch);
        for (frmIdx = 0u; frmIdx < frmNumMax; frmIdx++)
        {
            /* If configed LinIfFrameIndex == received 'start index' */
            if (framePtr->LinIfFrameIndex == Lin_SduPtr[LINIF_NC_DATA1_POS])
            {
                /* Assign frame ID range can set PIDs up to four frames.*/
                for (assignPidIdx = 0u; assignPidIdx < 4u; assignPidIdx++)
                {
                    boolean breakFlag = FALSE;
                    pid = Lin_SduPtr[LINIF_NC_DATA2_POS + assignPidIdx];
                    if (0xFFu != pid)
                    {
                        if ((frmIdx + assignPidIdx) < frmNumMax)
                        {
                            /* Save the original PidTable value and restore it if Settings fail */
                            pidTableBak[assignPidIdx] = pidTablePtr[frmIdx + assignPidIdx];

                            if ((pid != 0x3Cu) && (pid != 0x7Du) && (pid != 0xFEu) && (pid != 0xBFu)
                                && (pidTablePtr[frmIdx + assignPidIdx] != 0x3Cu)
                                && (pidTablePtr[frmIdx + assignPidIdx] != 0x7Du)
                                && (pidTablePtr[frmIdx + assignPidIdx] != 0xFEu)
                                && (pidTablePtr[frmIdx + assignPidIdx] != 0xBFu))
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

                if (assignPidIdx == 4u)
                {
                    LinIf_SlaveResetRtData(ch);
                    /* Build positive response for next SRF */
                    slaveRTDataPtr->ncResponsePendingFlag = TRUE;
                    LinIf_SlaveBuildNCResponse(ch, configedNAD, sid, 0xFFu);

                    ret = E_OK;
                }
                else
                {
                    /* Set fail,reject the request message and shall not sent a response*/
                    /* Restore saved PidTable value */
                    for (restoreIdx = 0u; restoreIdx < assignPidIdx; restoreIdx++)
                    {
                        pid = Lin_SduPtr[LINIF_NC_DATA2_POS + restoreIdx];
                        if (0xFFu != pid)
                        {
                            pidTablePtr[frmIdx + restoreIdx] = pidTableBak[restoreIdx];
                        }
                    }
                }

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
        LinIf_SlaveResetRtData(ch);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Set Response-Error bit to the value same with parameter
 *                     'responseError'.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel.
 *                     ErrorStatus: Type of detected error.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveSetResponseErrorBit(NetworkHandleType ch, boolean responseError)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/*@req <SWS_LinIf_00766>*/
#if defined(LinIfResponseErrorSignalChangedCallout)
    if ((responseError != slaveRTDataPtr->responseError) && (TRUE == LINIF_GET_RESPONSE_ERROR_SIG_CONFIGURED(ch)))
    {
        /*@req <SWS_LinIf_00765>*/
        LinIfResponseErrorSignalChangedCallout(ch, responseError);
    }
#endif
    /* Set the response_error signal */
    slaveRTDataPtr->responseError = responseError;

    /*@req <SWS_LinIf_00764>*/
    if (TRUE == LINIF_GET_RESPONSE_ERROR_SIG_CONFIGURED(ch))
    {
        (void)Com_SendSignal(LINIF_GET_RESPONSE_ERROR_SIGNAL(ch), &slaveRTDataPtr->responseError);
    }
}

/******************************************************************************/
/*
 * Brief               Set Lin_PduType
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      frame: Pointed to configed LinIfFrameType
 * Param-Name[out]     None
 * Param-Name[in/out]  PduPtr: Lin_PduType buffer pointer provided by Lin.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveSetLinPduType(
    P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_DATA) PduPtr, /* PRQA S 3432 */
    P2CONST(LinIf_FrameType, AUTOMATIC, LINIF_CONST) frame)
{
    /* Set DL */
    PduPtr->Dl = frame->LinIfLength;

    /* Set checksum type */
    if (LINIF_ENHANCED == frame->LinIfChecksumType)
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
    switch (frame->LinIfPduDirection->LinIfPduDirectionId)
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

/******************************************************************************/
/*
 * Brief               Build Node Configuration Response
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf Channel
 *                     nad: NAD
 *                     sid: SID
 *                     identifier: Identifier(Only used in Read by identifier
 *                     Service,other service input 0xFF)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveBuildNCResponse(NetworkHandleType ch, uint8 nad, uint8 sid, uint8 identifier)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(LinIf_NodeConfigurationIdentificationType, AUTOMATIC, LINIF_CONST)
    ncIdentifi;
    P2VAR(uint8, AUTOMATIC, LINIF_VAR) ncResponse = slaveRTDataPtr->ncResponse; /* PRQA S 3432 */

    (void)ILib_memset(ncResponse, 0xFF, LINIF_NC_RESPONSE_LEN);
    ncResponse[LINIF_NC_NAD_POS] = nad;
    if (LINIF_NC_SID_READBYIDENTIFIER != sid)
    {
        ncResponse[LINIF_NC_PCI_POS] = 0x01u;        /* length = 1 */
        ncResponse[LINIF_NC_RSID_POS] = sid + 0x40u; /* positive response */
    }
    else
    {
        if (0u == identifier) /* LIN product identification */
        {
            ncIdentifi = LINIF_GET_NC_IDENTIFICATION(ch);

            ncResponse[LINIF_NC_PCI_POS] = 0x06u;                                       /* length = 6 */
            ncResponse[LINIF_NC_RSID_POS] = sid + 0x40u;                                /* positive response*/
            ncResponse[LINIF_NC_DATA1_POS] = (uint8)ncIdentifi->LinIfSupplierId;        /* Supplier ID LSB*/
            ncResponse[LINIF_NC_DATA2_POS] = (uint8)(ncIdentifi->LinIfSupplierId >> 8); /* Supplier ID MSB*/
            ncResponse[LINIF_NC_DATA3_POS] = (uint8)ncIdentifi->LinIfFunctionId;        /* Function ID LSB*/
            ncResponse[LINIF_NC_DATA4_POS] = (uint8)(ncIdentifi->LinIfFunctionId >> 8); /* Function ID LSB*/
            ncResponse[LINIF_NC_DATA5_POS] = ncIdentifi->LinIfVariantId;                /* Variant */
        }
        else if (2u == identifier) /* Bit timing test */
        {
            ncResponse[LINIF_NC_PCI_POS] = 0x03u;                      /* length = 6 */
            ncResponse[LINIF_NC_RSID_POS] = LINIF_NC_NEGTIVE_RESPONSE; /* negtive response */
            ncResponse[LINIF_NC_DATA1_POS] = sid;
            ncResponse[LINIF_NC_DATA2_POS] = 0x12u;
        }
        else
        {
            /* Identifier not directly support by LinIf Node configuration */
        }
    }
}

/******************************************************************************/
/*
 * Brief               Process timer used in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveTimerHandle(NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    if (slaveRTDataPtr->nasTimer > 0u)
    {
        slaveRTDataPtr->nasTimer--;
        if (0u == slaveRTDataPtr->nasTimer)
        {
            /*@req <SWS_LinIf_00786>*/
            /* N_As timeout occurrence,abort pending node config response.*/
            LinIf_SlaveResetRtData(ch);
            slaveRTDataPtr->ncResponsePendingFlag = FALSE;
            (void)ILib_memset(slaveRTDataPtr->ncResponse, 0, LINIF_NC_RESPONSE_LEN);
        }
    }

    if (slaveRTDataPtr->busIdleTimer > 0u)
    {
        slaveRTDataPtr->busIdleTimer--;
        if (0u == slaveRTDataPtr->busIdleTimer)
        {
            /*@req <SWS_LinIf_00755>*/
            /* execute sleep mode transition */
            USER_GOTO_SLEEP_INDICATION(ch, LINIF_GET_COMM_NETWORK(ch));
        }
    }
}

/******************************************************************************/
/*
 * Brief               Process timer used in slave node.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ch: LinIf channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, LINIF_CODE) LinIf_SlaveResetRtData(NetworkHandleType ch)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(LinIf_SlaveRuntimeType, AUTOMATIC, LINIF_VAR)
    slaveRTDataPtr = LINIF_GET_SLAVE_RTDATA_PTR(ch);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    slaveRTDataPtr->curFrame = NULL_PTR;
    slaveRTDataPtr->nasTimer = 0u;
    slaveRTDataPtr->eventTrgTxFrmIdx = 0u;
    slaveRTDataPtr->frameStatus = LINIF_SLAVE_FRAME_IDLE;
    slaveRTDataPtr->wakeupFlag = FALSE;
    slaveRTDataPtr->waitWakeupConfirmFlag = FALSE;
    slaveRTDataPtr->gotoSleepConfirmationFlag = FALSE;
}

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif
