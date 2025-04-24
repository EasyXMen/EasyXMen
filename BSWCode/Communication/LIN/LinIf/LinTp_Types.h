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
**  FILENAME    : LinTp_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of LinTp                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef LINTP_TYPES_H
#define LINTP_TYPES_H

#include "ComStack_Types.h"
#include "LinIf_Cfg.h"
#if (LINIF_TP_SUPPORTED == STD_ON)
#include "LinTp_Cfg.h"
#endif /*LINIF_TP_SUPPORTED == STD_ON*/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#if (STD_ON == LINIF_TP_SUPPORTED)
/*@req <SWS_LinIf_00629> */
typedef enum
{
    LINTP_APPLICATIVE_SCHEDULE,
    LINTP_DIAG_REQUEST,
    LINTP_DIAG_RESPONSE
} LinTp_Mode;

typedef struct
{
    NetworkHandleType LinTpLinIfChannelRef; /* Refer to LinIf channel */
    boolean LinTpDropNotRequestedNad;
#if (STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT)
    boolean LinTpScheduleChangeDiag;
#endif                                 /* STD_ON == LINTP_SCHEDULE_CHANGE_DIAG_SUPPORT */
    NetworkHandleType LinTpChannelRef; /* Refer to ComM */
    uint16 LinTpMaxNumberOfRespPendingFrames;
    uint16 LinTpP2MaxCnt;
    uint16 LinTpP2TimingCnt;
} LinTp_ChannelConfigType;

typedef struct
{
    NetworkHandleType LinTpLinIfChannelRef; /* Refer to LinIf channel */
    uint16 LinTpNcrCnt;
    PduIdType LinTpRxNSduId;
    uint8 LinTpRxNSduNad;
    NetworkHandleType LinTpRxNSduChannelRef; /* Refer to ComM */
    PduIdType LinTpRxNSduPduRef;
} LinTp_RxNSduType;

typedef struct
{
    NetworkHandleType LinTpLinIfChannelRef; /* Refer to LinIf channel */
    uint8 LinTpMaxBufReq;
    uint16 LinTpNasCnt;
    uint16 LinTpNcsCnt;
    PduIdType LinTpTxNSduId;
    uint8 LinTpTxNSduNad;
    NetworkHandleType LinTpTxNSduChannelRef; /* Refer to ComM */
    PduIdType LinTpTxNSduPduRef;
} LinTp_TxNSduType;

typedef struct
{
    uint16 LinTpMaxRxNSduCnt;
    uint16 LinTpNumOfRxNSdu;
    uint16 LinTpMaxTxNSduCnt;
    uint16 LinTpNumOfTxNSdu;
    P2CONST(LinTp_ChannelConfigType, AUTOMATIC, LINIF_APPL_CONST) LinTpChannelConfig;
    P2CONST(LinTp_RxNSduType, AUTOMATIC, LINIF_APPL_CONST) LinTpRxNSdu;
    P2CONST(LinTp_TxNSduType, AUTOMATIC, LINIF_APPL_CONST) LinTpTxNSdu;
} LinTp_ConfigType;

#endif /* STD_ON == LINIF_TP_SUPPORTED */

#endif /* ifndef LINTP_TYPES_H */
