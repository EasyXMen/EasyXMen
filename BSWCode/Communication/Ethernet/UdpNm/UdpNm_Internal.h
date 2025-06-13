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
**  FILENAME    : UdpNm_Internal.h                                            **
**                                                                            **
**  Created on  : 25/07/19                                                    **
**  Author      : lili.wang                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : UdpNm internal use                                          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef UDPNM_INTERNAL_H_
#define UDPNM_INTERNAL_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "UdpNm_Cfg.h"
#include "NmStack_Types.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/* CBV(Control bit vector)  consist*/
/* repeat message request bit */
#define UDPNM_CBV_BIT_RMP_MASK ((uint8)(0x01u))
/* NM Coordinator sleep ready bit */
#define UDPNM_CBV_BIT_CSR_MASK ((uint8)(0x08u))
/* active wakeup bit */
#define UDPNM_CBV_BIT_AW_MASK ((uint8)(0x10u))
/* partial Network information bit */
#define UDPNM_CBV_BIT_PNI_MASK ((uint8)(0x40u))

/* Define in which channel handle EIRA timer */
/*
 * Note: EIRA is related to all channels, but only needs to handle the timer
 * in one of the channels.
 */
#define UDPNM_EIRA_HANDLE_CHANNEL (0u)

#if (STD_ON == UDPNM_DEV_ERROR_DETECT)
#define UDPNM_DET_REPORT(ApiId, ErrorId) ((void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId), (ErrorId)))
#else
#define UDPNM_DET_REPORT(ApiId, ErrorId)
#endif /* STD_ON == UDPNM_DEV_ERROR_DETECT */

#ifdef UDPNM_VER_4_2_2
#define UdpNm_GetCarWakeUpRxEnabled(ch)     UDPNM_CAR_WAKEUP_RX_ENABLED

#define UdpNm_GetCarWakeUpFilterEnabled(ch) UDPNM_CAR_WAKEUP_FILTER_ENABLED

#define UdpNm_GetCarWakeUpFilterNodeId(ch)  UDPNM_CAR_WAKEUP_FILTER_NODE_ID
#endif

#define UdpNm_SetTxPduCbvBit(chIndex, mask) /* PRQA S 3472 */ /* MISRA Rule Dir 4.9 */                         \
    do                                                                                                         \
    {                                                                                                          \
        if (UDPNM_PDU_OFF != UdpNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition)                              \
        {                                                                                                      \
            SchM_Enter_UdpNm_Context();                                                                        \
            UdpNm_ChRunTime[chIndex].txPduData[UdpNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition] |= (mask); \
            SchM_Exit_UdpNm_Context();                                                                         \
        }                                                                                                      \
    } while (0)

#define UdpNm_ClrTxPduCbvBit(chIndex, mask) /* PRQA S 3472 */ /* MISRA Rule Dir 4.9 */                          \
    do                                                                                                          \
    {                                                                                                           \
        if (UDPNM_PDU_OFF != UdpNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition)                               \
        {                                                                                                       \
            SchM_Enter_UdpNm_Context();                                                                         \
            UdpNm_ChRunTime[chIndex].txPduData[UdpNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition] &= ~(mask); \
            SchM_Exit_UdpNm_Context();                                                                          \
        }                                                                                                       \
    } while (0)
/*******************************************************************************
**                      Private Type Definitions                               *
*******************************************************************************/
typedef enum
{
    UDPNM_UNINIT,
    UDPNM_INIT,
} UdpNm_InitStatusType;

typedef enum
{
    UDPNM_NETWORK_REQUESTED,
    UDPNM_NETWORK_RELEASED
} UdpNm_NetRequestStatusType;

typedef enum
{
    UDPNM_CSR_IND_INIT,
    UDPNM_CSR_IND_CANCELLED,
    UDPNM_CSR_IND_INDICATED
} UdpNm_CSRIndStatusType;

typedef struct
{
    /* current udpNm state */
    Nm_StateType udpNmState;
    /* network mode */
    Nm_ModeType udpnmMode;
    /* Network request status */
    UdpNm_NetRequestStatusType netRequestStatus;
    /* Indicate if UdpNm_NetworkRequest was called within two MainFunction */
    boolean netRequestFlg;
    /* Passive startup flag */
    boolean passiveStartUp;

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
    /* tx ablity */
    boolean udpNmTxEnable;
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
    UdpNm_CSRIndStatusType coordReadyToSleepInd;
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

/* detected Remote Sleep */
#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
    boolean remoteSleepInd;
    uint16 remoteTimer;
    /*record the remote sleep timer passed before the timer suspended*/
    uint16 remoteTimerpassed;
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED)
    /* Immediate transmit nm pdu count */
    uint8 immedMsgTxCnt;
#endif /* STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED */

    /* state timer */
    uint16 stateTimer;
    boolean stateToutFlg;
    /* nm timeout timer */
    uint16 nmTimeoutTimer;
    boolean nmToutFlg;

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    /* tx message cycle timer */
    uint16 msgTxCylceTimer;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    /* First NM Pdu send,if need retry */
    boolean retrySendMsgFlag;
#endif
    uint16 msgTimeoutTimer;
    boolean msgToutFlg;

    boolean sendNmMsgFlg;
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

    /* last rx pdu data */
    uint8 rxPduData[UDPNM_PDU_LENGTH];
    boolean rxPduFlg;
    boolean rxPduExtFlg;
    /* last tx pdu data */
    uint8 txPduData[UDPNM_PDU_LENGTH];

/* pn function this is ERA*/
#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    uint8 pnInfoEra[UDPNM_PN_INFO_LENGTH];
    uint16 pnEraResetTimer[UDPNM_PN_NUM];
/* store pn info */
#endif /* STD_ON == UDPNM_PN_ERA_CALC_ENABLED */

#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
    boolean repeatMessageRequest;
#endif /* STD_ON == UDPNM_NODE_DETECTION_ENABLED */

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    boolean pnRxIndFlg;
#endif /* STD_ON == UDPNM_GLOBAL_PN_ENABLED */
} UdpNm_InnerChannelType;
#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
typedef struct
{
    uint8 pnEiraInfo[UDPNM_PN_INFO_LENGTH];
    uint16 pnEiraResetTimer[UDPNM_PN_NUM];
} UdpNm_InnerGlobalType;
#endif
#endif /* UDPNM_INTERNAL_H_ */
