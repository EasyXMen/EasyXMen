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
**  FILENAME    : CanNm_Internal.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       2020-07-29  Wanglili       R19_11 initial version.
 */
#ifndef CANNM_INTERNAL_H_
#define CANNM_INTERNAL_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanNm_Cfg.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/* CBV(Control bit vector)  consist*/
/* repeat message request bit */
#define CANNM_CBV_BIT_RMP_MASK ((uint8)(0x01u))
/* NM Coordinator sleep ready bit */
#define CANNM_CBV_BIT_CSR_MASK ((uint8)(0x08u))
/* active wakeup bit */
#define CANNM_CBV_BIT_AW_MASK ((uint8)(0x10u))
/* partial Network information bit */
#define CANNM_CBV_BIT_PNI_MASK  ((uint8)(0x40u))

#define CANNM_DEFAULT_NMPDU_LEN (8u)

/* Define in which channel handle EIRA timer */
/*
 * Note: EIRA is related to all channels, but only needs to handle the timer
 * in one of the channels.
 */
#define CANNM_EIRA_HANDLE_CHANNEL           (0u)

#define CanNm_SetTxPduCbvBit(chIndex, mask) /* PRQA S 3472 */ /* MISRA Rule Dir 4.9 */                                \
    do                                                                                                                \
    {                                                                                                                 \
        if (CANNM_PDU_OFF != CanNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition)                                     \
        {                                                                                                             \
            SchM_Enter_CanNm_Context();                                                                               \
            CanNm_ChRunTime[chIndex].txPduData[(uint8)CanNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition] |= (mask); \
            SchM_Exit_CanNm_Context();                                                                                \
        }                                                                                                             \
    } while (0)

#define CanNm_ClrTxPduCbvBit(chIndex, mask) /* PRQA S 3472 */ /* MISRA Rule Dir 4.9 */                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (CANNM_PDU_OFF != CanNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition)                                      \
        {                                                                                                              \
            SchM_Enter_CanNm_Context();                                                                                \
            CanNm_ChRunTime[chIndex].txPduData[(uint8)CanNm_CfgPtr->ChannelConfig[chIndex].PduCbvPosition] &= ~(mask); \
            SchM_Exit_CanNm_Context();                                                                                 \
        }                                                                                                              \
    } while (0)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef enum
{
    CANNM_UNINIT,
    CANNM_INIT,
} CanNm_InitStatusType;

typedef enum
{
    CANNM_NETWORK_REQUESTED,
    CANNM_NETWORK_RELEASED
} CanNm_NetRequestStatusType;

typedef enum
{
    CANNM_CSR_IND_INIT,
    CANNM_CSR_IND_CANCELLED,
    CANNM_CSR_IND_INDICATED
} CanNm_CSRIndStatusType;

typedef struct
{
    /* current canNm state */
    Nm_StateType canNmState;
    /* network mode */
    Nm_ModeType cannmMode;
    /* Network request status */
    CanNm_NetRequestStatusType netRequestStatus;
    /* Passive startup flag */
    boolean passiveStartUp;

#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
    /* tx ablity */
    boolean canNmTxEnable;
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */
    boolean rxPduFlg;
    boolean rxPduExtFlg;

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
    CanNm_CSRIndStatusType coordReadyToSleepInd;
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

/* detected Remote Sleep */
#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
    boolean remoteSleepInd;
    uint16 remoteTimer;
    /*record the remote sleep timer passed before the timer suspended*/
    uint16 remoteTimerpassed;
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED)
    /* Immediate transmit nm pdu count */
    uint8 immedMsgTxCnt;
#endif /* STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED */

    /* state timer */
    uint16 stateTimer;
    /* nm timeout timer */
    uint16 nmTimeoutTimer;
    boolean stateToutFlg;
    boolean nmToutFlg;

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    /* tx message cycle timer */
    uint16 msgTxCylceTimer;

    uint16 msgTimeoutTimer;

#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    /* First NM Pdu send,if need retry */
    boolean retrySendMsgFlag;
#endif

    boolean msgToutFlg;
    boolean sendNmMsgFlg;
    /* Indicate if CanNm_NetworkRequest was called within two MainFunction */
    boolean netRequestFlg;
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
    boolean repeatMessageRequest;
#endif /* STD_ON == CANNM_NODE_DETECTION_ENABLED */

    /* last rx pdu data */
    uint8 rxPduData[CANNM_DEFAULT_PDU_LENGTH];

    /* last tx pdu data */
    uint8 txPduData[CANNM_DEFAULT_PDU_LENGTH];

/* pn function this is ERA*/
#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
    uint8 pnInfoEra[CANNM_PN_INFO_LENGTH];
    uint16 pnEraResetTimer[CANNM_PN_NUM];
/* store pn info */
#endif /* STD_ON == CANNM_PN_ERA_CALC_ENABLED */

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
    boolean pnRxIndFlg;
    boolean pnFilterEnable;
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT */
} CanNm_InnerChannelType;

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT) && (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
typedef struct
{
    boolean pnEiraUpdate;
    uint8 pnEiraInfo[CANNM_PN_INFO_LENGTH];
    uint16 pnEiraResetTimer[CANNM_PN_NUM];
} CanNm_InnerGlobalType;
#endif
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#endif /* CANNM_INTERNAL_H_ */
