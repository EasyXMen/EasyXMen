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
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Pnc.h **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

#ifndef COMM_PNC_H_
#define COMM_PNC_H_

/*=================================================[inclusions]=======================================================*/

#if (COMM_PNC_NUMBER > 0)
/*=============================================[type definitions]=====================================================*/
/** \brief pnc variable */
typedef struct
{
    uint32 preSleeptimer;
    ComM_PncModeType pncMode;
#define COMM_PNC_MAP_LEAST_CHANNEL 0x0u
#define COMM_PNC_PASSIVE_CHREQ     0x1u
#define COMM_PNC_FRD_SYNC_SHUTDOWN 0x2u
#define COMM_PNC_TOP_GATEWAY       0x3u
#define COMM_PNC_RX_EIRA_SET       0x4u
    uint8 setMask[1];
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT)
    uint8* chMapEraIndex;
#endif /* (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT) */
    uint8* eraActiveTable;
    uint8 eraCnt;
#endif
    uint8 userReqCnt;
    uint8* userReqTable;
} ComM_PncVarType;

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)

/** \brief pnc learning variable */
typedef struct
{
    uint8 membership[COMM_PN_INFO_LENGTH];
#define COMM_PNC_LEARN_IND   0x0u
#define COMM_PNC_LEARN_NMREQ 0x1u
#define COOMM_PNC_LEARN_STU  0x2u
    uint8 setMask[COMM_CHANNEL_NUMBER][1];
} ComM_PncLearnVarType;
#endif

/*========================================[external function declarations]============================================*/

BEGIN_C_DECLS
ComM_PncModeType ComM_PncGetStatus(uintx pncIdx);

#if (COMM_SYNC_WAKEUP == STD_ON)
FUNC(void, COMM_CODE) ComM_PncWakupByCh(uintx chIdx);
#endif /* COMM_SYNC_WAKEUP == STD_ON */

FUNC(void, COMM_CODE) ComM_PncWakupIndPnc(uintx pncIdx);

FUNC(void, COMM_CODE)
ComM_PncRequestComMode(uintx pncIdx, uintx usrInPncIdx, ComM_ModeType comMode);

FUNC(void, COMM_CODE)
ComM_PncInit(P2CONST(ComM_PncConfigType, AUTOMATIC, COMM_APPL_CONST) pncCfgPtr);

#if (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON) && (COMM_PNC_GATEWAY_ENABLED == STD_ON)
FUNC(void, COMM_CODE)
ComM_PncForwardSyncPncShutdown(uintx chIdx, P2CONST(uint8, AUTOMATIC, COMM_APPL_CONST) pncBitVectorPtr);
#endif

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
FUNC(void, COMM_CODE) ComM_PncLearnPhaseAcitveHandle(uintx chIdx);

FUNC(void, COMM_CODE) ComM_PncLearnInd(uintx chIdx, boolean reqMode);

FUNC(Std_ReturnType, COMM_CODE) ComM_PncLearningRequest(void);

FUNC(Std_ReturnType, COMM_CODE)
ComM_PncUpdateMembership(boolean Control, P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncMembership);

#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
FUNC(Std_ReturnType, COMM_CODE)
ComM_PncGetPncMap(
    P2VAR(uint8*, AUTOMATIC, COMM_APPL_DATA) mappingTable,
    /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) channelCnt /* PRQA S 3432 */ /* MISRA Rule 20.7 */,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) pncCnt /* PRQA S 3432 */ /* MISRA Rule 20.7 */);

FUNC(Std_ReturnType, COMM_CODE)
ComM_PncSetPncMap(
    P2VAR(uint8*, AUTOMATIC, COMM_APPL_DATA) mappingTable /* PRQA S 3432 */ /* MISRA Rule 20.7 */,
    VAR(uint8, COMM_APPL_DATA) channelCnt,
    VAR(uint8, COMM_APPL_DATA) pncCnt);

FUNC(Std_ReturnType, COMM_CODE) ComM_PncResetPncMap(void);
#endif
#endif

#if (COMM_PNC_USED_COM == STD_ON)
FUNC(void, COMM_CODE) ComM_PncMainFunction(void);
#endif

FUNC(void, COMM_CODE) ComM_PncProcessTimer(void);

#if (COMM_PNC_USED_COM == STD_ON)
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
FUNC(void, COMM_CODE) ComM_ComEraRxIndication(uint8 sigIdx);
#endif
FUNC(void, COMM_CODE) ComM_ComEiraRxIndication(uint8 sigIdx);
#endif

END_C_DECLS

#endif

#endif /* COMM_PNC_H_ */
