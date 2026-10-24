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
 **  @file               : ComM_Pnc.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Pnc Implementation header
 **
 **********************************************************************************************************************/

#ifndef COMM_PNC_H_
#define COMM_PNC_H_
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* =================================================== inclusions =================================================== */
#include "ComM_Internal.h"

#if COMM_PNC_SUPPORT == STD_ON
#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define COMM_NUM_OF_STATEMACHINES_PNC 4u
/* ================================================ type definitions ================================================ */
typedef struct ComM_PncConfigType_Tag /* PRQA S 1536 */ /* VL_ComM_1536 */
{
    uint16            BitIndex;
    uint16            PrepareSleepTimer;
    NetworkHandleType MainChannel;
    boolean           WakeupSleepRequestEnabled;
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    uint8* const      ChannelBitVectorERA;
    uint8             ChannelBitVectorERALength;
    NetworkHandleType PassiveChannel;
#endif
    const uint8 ChannelMappingBitVector[(COMM_CHANNEL_NUMBER / COMM_CHAR_BIT) + 1u];
    const uint8 TxOnlyChannelMappingBitVector[(COMM_CHANNEL_NUMBER / COMM_CHAR_BIT) + 1u];
#if COMM_USER_NUMBER > 0u
    const uint8 UserMappingBitVector[(COMM_USER_NUMBER / COMM_CHAR_BIT) + 1u]; /* PRQA S 1039 */ /* VL_ComM_1039 */
#endif
} ComM_PncConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Initializes Pncs and restarts the internal state machines.
 * @param[in]     pncConfigs  Pointer to post-build configuration data for all pncs
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-64979
 */
void ComM_PncInit(const ComM_PncConfigType* pncConfigs);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * The function returns the current mode of the corresponding pnc.
 * @param[in]    pnc  See PNCHandleType
 * @retval       See ComM_PncModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64986
 */
ComM_PncModeType ComM_PncGetMode(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Function to query the maximum allowed Communication Mode of the corresponding pnc.
 * @param[in]    pnc  See PNCHandleType
 * @retval       See ComM_ModeType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-64993
 */
ComM_ModeType ComM_PncGetMaxComMode(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Function to query the bit index in pnc bit vector of the corresponding pnc.
 * @param[in]    pnc  See PNCHandleType
 * @retval       The bit index in pnc bit vector of the corresponding pnc
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65002
 */
uint16 ComM_PncGetBitIndex(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Store the current aggregated external / internal PNC request.
 * @param[in]    pncBitVectorPtr  Pointer to the PNC bit vector which contain the current aggregated internal and
 *                                external PNC requests (EIRA)
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-64999
 */
void ComM_PncUpdateEIRA(const uint8* pncBitVectorPtr);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Indicate the current external PNC request of the corresponding channel for the corresponding pnc.
 * @param[in]    pnc              See PNCHandleType
 * @param[in]    channel          See NetworkHandleType
 * @param[in]    pncBitVectorPtr  PNC bit vector which contain the current ERA received on the given channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65001
 */
void ComM_PncUpdateChannelBitVectorERA(PNCHandleType pnc, NetworkHandleType channel, const uint8* pncBitVectorPtr);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_PncPassiveChannelRequested
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Function to query whether there is a pnc request on the passive channel(pnc gateway is enabled on the pnc).
 * @param[in]    pnc  See PNCHandleType
 * @retval       TRUE Passive channel request the pnc
 * @retval       FALSE Passive channel not request the pnc or passive channel is assigned to the pnc.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65004
 */
boolean ComM_PncPassiveChannelRequested(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Notification of a wake up on the corresponding partial network cluster.
 * @param[in]    pnc  Identifier of the partial network cluster
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65010
 */
void ComM_PncWakeUpIndication(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Function to query whether there is a pnc request by user or on the active channel(pnc gateway is enabled on the pnc).
 * @param[in]    pnc  See PNCHandleType
 * @retval       TRUE: user or active channel request the pnc
 * @retval       FALSE: user or active channel not request the pnc
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65012
 */
boolean ComM_PncUserOrActiveChannelRequested(PNCHandleType pnc);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * This function shall perform the processing of the ComM Pnc state activities.
 * @param[in]    pnc      See PNCHandleType
 * @param[in]    channel  Indicates which channel is calling ComM_PncMainFunction.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-65013
 */
void ComM_PncMainFunction(PNCHandleType pnc, NetworkHandleType channel);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if COMM_USER_NUMBER > 0u
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Determine if the User is assigned to the corresponding PNC.
 * @param[in]    pnc      See PNCHandleType
 * @param[in]    user     See ComM_UserHandleType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @return       TRUE user is assigned to the pnc else FALSE
 * @trace        CPD-64998
 */
boolean ComM_PncIsUserAssigned(PNCHandleType pnc, ComM_UserHandleType user);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * Determine if the Channel is assigned to the corresponding PNC.
 * @param[in]    pnc      See PNCHandleType
 * @param[in]    channel  See NetworkHandleType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @return       TRUE channel is assigned to the pnc else FALSE
 * @trace        CPD-64939
 */
boolean ComM_PncIsChannelAssigned(PNCHandleType pnc, NetworkHandleType channel);
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif
#endif /* COMM_PNC_H_ */
