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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_EcuM.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : Callback of ComM Implementation header for Nm
 **
 **********************************************************************************************************************/

#ifndef COMM_NM_H_
#define COMM_NM_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Cfg.h"

#if COMM_NM_ENABLED == STD_ON
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * Indication that a NM-message has been received in the Bus Sleep Mode, what indicates that some nodes in the network
 * have already entered the Network Mode.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59633
 */
void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel);

/**
 * Notification that the network management has entered Network Mode.
 * @param[in]    Channel  Channel
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-59634
 */
void ComM_Nm_NetworkMode(NetworkHandleType Channel);

/**
 * Notification that the network management has entered Prepare Bus-Sleep Mode.
 * @param[in]    Channel  Channel
 * @reentrant    TRUE (but not for the same NM-Channel)
 * @synchronous  FALSE
 * @trace        CPD-59635
 */
void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel);

/**
 * Notification that the network management has entered Bus-Sleep Mode. This callback function should perform a
 * transition of the hardware and transceiver to bus-sleep mode.
 * @param[in]    Channel  Channel
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-59636
 */
void ComM_Nm_BusSleepMode(NetworkHandleType Channel);

/**
 * If NmIf has started to shut down the coordinated busses, AND not all coordinated busses have indicated bus sleep
 * state, AND on at least on one of the coordinated busses NM is restarted, THEN the NM Interface shall call the
 * callback function ComM_Nm_RestartIndication with the nmNetworkHandle of the channels which have already indicated bus
 * sleep state.
 * @param[in]    Channel  Channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59637
 */
void ComM_Nm_RestartIndication(NetworkHandleType Channel);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * Function to indicate the current aggregated external / internal PNC request called by Nm.
 * @param[in]    PncBitVectorPtr  Pointer to the PNC bit vector which contain the current aggregated internal and
 *                                external PNC requests (EIRA)
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-59638
 */
void ComM_Nm_UpdateEIRA(const uint8* PncBitVectorPtr);

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
/**
 * Function to indicate the current external PNC request per channel called by Nm.
 * @param[in]    Channel          Channel
 * @param[in]    PncBitVectorPtr  PNC bit vector which contain the current external PNC requests (ERA) received on the
 *                                given channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59639
 */
void ComM_Nm_UpdateERA(NetworkHandleType Channel, const uint8* PncBitVectorPtr);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* COMM_NM_ENABLED == STD_ON */
#endif /* COMM_NM_H_ */
