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
 **  @file               : ComM_Pnc.c **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/08 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

/*=================================================[inclusions]=======================================================*/
#include "ComM_Internal.h"
#include "ComM_PBCfg.h"
#include "ComM_Pnc.h"
#include "ComM_Ch.h"
#if (COMM_BSWM_ENABLE == STD_ON)
#include "BswM_ComM.h"
#endif /* COMM_BSWM_ENABLE == STD_ON */
#if (COMM_USED_MODULE_NM == STD_ON)
#include "Nm.h"
#endif /* COMM_USED_MODULE_NM == STD_ON */
#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_USED_COM == STD_ON)
#include "Com.h"
#endif /* COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_USED_COM == STD_ON */
#include "SchM_ComM.h"

/*====================================================[macros]========================================================*/
#if (COMM_PNC_NUMBER > 0)
/*=================================================[internal data]====================================================*/
/**  \brief pnc post build configure data point */
#define COMM_START_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"
COMM_LOCAL P2CONST(ComM_PncConfigType, AUTOMATIC, COMM_APPL_CONST) ComM_PncCfgPtr;
#define COMM_STOP_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
/**  \brief channel map pnc double dimensional array */
#define COMM_START_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
COMM_LOCAL VAR(uint8, AUTOMATIC) ComM_ChToPncMap[COMM_CHANNEL_NUMBER][COMM_PN_INFO_LENGTH];
#define COMM_STOP_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"

/** \brief used pnc learning */
#define COMM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
COMM_LOCAL VAR(ComM_PncLearnVarType, AUTOMATIC) ComM_PncLearnVar;
#define COMM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"

#else
/**  \brief channel map pnc data point */
#define COMM_START_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"
COMM_LOCAL const uint8 (*ComM_ChToPncMap)[COMM_PN_INFO_LENGTH];
#define COMM_STOP_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"

#endif /* COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */

/**  \brief pnc manage data */
#define COMM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
COMM_LOCAL VAR(ComM_PncVarType, AUTOMATIC) ComM_PncVar[COMM_PNC_NUMBER];
#define COMM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"

/**  \brief pnc transmit IRA data */
#define COMM_START_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"

#if (COMM_PNC_USED_COM == STD_ON)
/**  \brief indication EIRA signal received form com*/
COMM_LOCAL VAR(boolean, AUTOMATIC) ComM_PncRxComSigEiraInd;

COMM_LOCAL uint8 ComM_PncActiveIra[COMM_PN_INFO_LENGTH] = {0};
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
COMM_LOCAL uint8 ComM_PncPassiveIra[COMM_PN_INFO_LENGTH] = {0};
/**  \brief indication ERA signal received form com*/
COMM_LOCAL VAR(boolean, AUTOMATIC) ComM_PncRxComSigEraInd;
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
#endif /* COMM_PNC_USED_COM == STD_ON */

#define COMM_STOP_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"

/*=========================================[internal function declarations]===========================================*/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncBeh_EntryPrepareSleep(uintx pncIdx);

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
COMM_LOCAL void ComM_PncUpdateEra(uintx chIdx, const uint8* pncBitEra);
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */

COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncUpdateEira(P2CONST(uint8, AUTOMATIC, COMM_APPL_CONST) pncBitEira);
/*==========================================[internal function definitions]===========================================*/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
/**
 * @ingroup     ComM pnc
 * @brief       pnc link user state change notify rte by mode switch
 * @param[in]   pncIdx  pnc index
 * @return      NA
 * @note        called by pnc state change
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncStateChangeHandleNotify(uintx pncIdx)
{
    ComM_ModeType lowestUserMode = COMM_FULL_COMMUNICATION;
    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
    for (uintx index = 0; index < pncCfgPtr->belongUserNum; index++)
    {
        const ComM_UserConfigType* usrCfgPtr = pncCfgPtr->belongUserListPtr[index];
        /* Calculate the lowest mode of all PNCs assigned to the current user. If at least one PNC is in
         * COMM_PNC_NO_COMMUNICATION mode, the lowest mode is COMM_NO_COMMUNICATION */
        lowestUserMode = COMM_FULL_COMMUNICATION;
        for (uintx i = 0u; i < usrCfgPtr->pncListNum; i++)
        {
            uintx pncIdx = usrCfgPtr->pncList[i];
            if (ComM_PncVar[pncIdx].pncMode == COMM_PNC_NO_COMMUNICATION)
            {
                lowestUserMode = COMM_NO_COMMUNICATION;
                break;
            }
        }
        /* call user notify */
        ComM_UserStateChangeNotify(usrCfgPtr, lowestUserMode);
    }
}
#endif /* COMM_USER_MODESWITCH_NOTIFY == STD_ON && COMM_USER_NUMBER > 0 */

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc learning send iar
 * @param[in]   chIdx  channel mapping pnc index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncLearnSendIraByCh(uintx chIdx)
{

#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
    /** @ref [SWS_ComM_01030] {OBSOLETE replaced by [SWS_ComM_01090]} and @ref SWS_ComM_01090
     * gateway enable forward receiver era rx to all other channel where ComMPncDynamicMappingEnabled is true
     */
    uint8 txVlu[COMM_PN_INFO_LENGTH] = {0};
    if (chIdx < ComM_PncCfgPtr->pncMapTotalChannelNum)
    {
        for (uintx i = 0u; i < COMM_PN_INFO_LENGTH; i++)
        {
            txVlu[i] = (ComM_ChToPncMap[chIdx][i] & ComM_PncLearnVar.membership[i]);
        }
    }
#else  /* COMM_PNC_GATEWAY_ENABLED != STD_ON */
    const uint8* txVlu = ComM_PncLearnVar.membership;
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
    for (uintx index; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
    {
        if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[chIdx], COOMM_PNC_LEARN_STU))
        {
            Nm_UpdateIRA(ComM_PncCfgPtr->ChCfgPtr[index]->inerChIdx, txVlu);
        }
    }
}
#endif /* COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */
/* pnc state machine */
/**
 * @ingroup     ComM pnc
 * @brief       pnc state entry COMM_PNC_READY_SLEEP
 * @param[in]   chIdx  channel mapping pnc index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncBeh_EntryReadySleep(uintx pncIdx)
{
    ComM_PncModeType oldPncMode = ComM_PncGetStatus(pncIdx);
    ComM_PncVar[pncIdx].pncMode = COMM_PNC_READY_SLEEP;

    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
#if (COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON)
    for (uintx portIdx = 0u; portIdx < pncCfgPtr->ethIfSwitchPortNum; portIdx++)
    {
        (void)EthIf_SwitchPortGroupRequestMode(pncCfgPtr->ethIfSwitchPortPtr[portIdx], ETH_MODE_DOWN);
    }
#endif /* COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON */

    for (uintx chIdx = 0; chIdx < ComM_PncCfgPtr->pncMapTotalChannelNum; chIdx++)
    {
        const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[chIdx];
        if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncCfgPtr->pncOffsetId))
        {
            if (oldPncMode == COMM_PNC_REQUESTED)
            {
                /** @ref [SWS_ComM_00961] {OBSOLETE replaced by [SWS_ComM_01086]}
                 * entering state COMM_PNC_READY_SLEEP for COMM_PNC_REQUESTED comm shall release COMM_FULL_COMMUNICATION
                 * all config this pnc map channel
                 */
                (void)ComM_ChRequstCommMode(chCfgPtr->inerChIdx, chCfgPtr->pncInChIdex[pncIdx], COMM_NO_COMMUNICATION);
            }
        }
    }

#if (COMM_BSWM_ENABLE == STD_ON)
    /** @ref SWS_ComM_00908
     * notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state
     */
    BswM_ComM_CurrentPNCMode(pncCfgPtr->notifyBswMPncId, COMM_PNC_READY_SLEEP);
#endif /* COMM_BSWM_ENABLE == STD_ON */
#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
    ComM_PncStateChangeHandleNotify(pncIdx);
#endif
    /** @ref SWS_ComM_00940
     * If the PNC is released  the sub state COMM_PNC_READY_SLEEP shall be left and the sub state COMM_PNC_PREPARE_SLEEP
     * shall be entered
     */

    if (!ComM_CommonGetBit(ComM_PncVar[pncIdx].setMask, COMM_PNC_RX_EIRA_SET))
    {
        ComM_PncBeh_EntryPrepareSleep(pncIdx);
    }
}

COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncBeh_EntryNoCom(uintx pncIdx)
{
    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
    ComM_PncVar[pncIdx].pncMode = COMM_PNC_NO_COMMUNICATION;

#if (COMM_BSWM_ENABLE == STD_ON)
    /** @ref SWS_ComM_00908
     * notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state
     */
    BswM_ComM_CurrentPNCMode(pncCfgPtr->notifyBswMPncId, COMM_PNC_NO_COMMUNICATION);
#endif /* COMM_BSWM_ENABLE == STD_ON */
#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
    ComM_PncStateChangeHandleNotify(pncIdx);
#endif
}

/**
 * @ingroup     ComM pnc
 * @brief       pnc state entry COMM_PNC_PREPARE_SLEEP
 * @param[in]   chIdx  channel mapping pnc index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncBeh_EntryPrepareSleep(uintx pncIdx)
{
    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
    ComM_PncVar[pncIdx].pncMode = COMM_PNC_PREPARE_SLEEP;
    /** @ref SWS_ComM_00952
     * ComMPncPrepareSleepTimer shall be start with configured initial value
     */
    ComM_PncVar[pncIdx].preSleeptimer = COMM_PNC_PREPARE_SLEEP_TIMER;
#if (COMM_BSWM_ENABLE == STD_ON)
    /** @ref SWS_ComM_00908
     * notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state
     */
    BswM_ComM_CurrentPNCMode(pncCfgPtr->notifyBswMPncId, COMM_PNC_PREPARE_SLEEP);
#endif /* COMM_BSWM_ENABLE == STD_ON */
#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
    ComM_PncStateChangeHandleNotify(pncIdx);
#endif
    if (0u == ComM_PncVar[pncIdx].preSleeptimer)
    {
        ComM_PncBeh_EntryNoCom(pncIdx);
    }
}
/**
 * @ingroup     ComM pnc
 * @brief       pnc state entry COMM_PNC_REQUESTED
 * @param[in]   chIdx  channel mapping pnc index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncBeh_EntryRequst(uintx pncIdx)
{
    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
    ComM_PncVarType* pncVarPtr = &ComM_PncVar[pncIdx];
    ComM_PncModeType oldPncMode = ComM_PncGetStatus(pncIdx);
    pncVarPtr->pncMode = COMM_PNC_REQUESTED;
#if (COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON)
    Eth_ModeType portMode = ETH_MODE_ACTIVE;
    /* eth switch port ref approximate SWS_ComM_01053 */
    if ((TRUE == pncCfgPtr->pncWakeupSleepRequestEnabled)
        && ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_MAP_LEAST_CHANNEL)
        && ((oldPncMode == COMM_PNC_NO_COMMUNICATION) || (oldPncMode == COMM_PNC_PREPARE_SLEEP)))
    {
        portMode = ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST;
    }
    for (uintx portIdx = 0u; portIdx < pncCfgPtr->ethIfSwitchPortNum; portIdx++)
    {
        (void)EthIf_SwitchPortGroupRequestMode(pncCfgPtr->ethIfSwitchPortPtr[portIdx], portMode);
    }
#endif /* COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON */
    /** @ref SWS_ComM_01053 --> @ref SWS_ComM_01067
     * entering the PNC sub state COMM_PNC_REQUESTED from COMM_PNC_NO_COMMUNICATION or COMM_PNC_PREPARE_SLEEP,this PNC
     * reference at least one ComMChannel and ComMPncWakeupSleepRequestEnabled of this PNC is set to
     * TRUE,BswM_ComM_CurrentPNCMode shall be called with COMM_PNC_REQUESTED_WITH_WAKEUP_REQUEST instead of
     * COMM_PNC_REQUESTED
     */
    if (
#if (COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON)
        (portMode == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST)
#else
        (TRUE == pncCfgPtr->pncWakeupSleepRequestEnabled)
        && ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_MAP_LEAST_CHANNEL)
        && ((oldPncMode == COMM_PNC_NO_COMMUNICATION) || (oldPncMode == COMM_PNC_PREPARE_SLEEP))
#endif /* COMM_ETHIF_SWITCHPORTGOURP_ENABLE == STD_ON */
    )
    {
#if (COMM_BSWM_ENABLE == STD_ON)
        BswM_ComM_CurrentPNCMode(pncCfgPtr->notifyBswMPncId, COMM_PNC_REQUESTED_WITH_WAKEUP_REQUEST);
#endif /* COMM_BSWM_ENABLE == STD_ON */
    }
    else
    {
        /** @ref SWS_ComM_00908
         * notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state
         */
#if (COMM_BSWM_ENABLE == STD_ON)
        BswM_ComM_CurrentPNCMode(pncCfgPtr->notifyBswMPncId, COMM_PNC_REQUESTED);
#endif /* COMM_BSWM_ENABLE == STD_ON */
    }
#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
    ComM_PncStateChangeHandleNotify(pncIdx);
#endif
    uint32 pncOffsetId = ComM_PncCfgPtr->pncListCfgPtr[pncIdx].pncOffsetId;
    for (uintx index = 0u; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
    {
        const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[index];
        if (ComM_CommonGetBit(ComM_ChToPncMap[index], pncOffsetId))
        {
            /** @ref SWS_ComM_00993 --> @ref SWS_ComM_01069
             * channel not enable WakeupSleepRequest,shall request channel fullcom
             */
            if ((oldPncMode != COMM_PNC_REQUESTED)
#if defined(COMM_WAKEUPSLEEP_RUST)
                && (!ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_WAKEUPSLEEP_RUST))
#endif /* defined(COMM_WAKEUPSLEEP_RUST */
            )
            {
                (void)
                    ComM_ChRequstCommMode(chCfgPtr->inerChIdx, chCfgPtr->pncInChIdex[pncIdx], COMM_FULL_COMMUNICATION);
#if defined(COMM_PNC_NM_REQUEST)
                /** @ref SWS_ComM_00980 COMM_PNC_REQUESTED
                 * entry COMM_PNC_REQUESTED and ComMPncNmRequest is true,shall call Nm_NetworkRequest
                 */
                if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_PNC_NM_REQUEST))
                {
                    (void)Nm_NetworkRequest(chCfgPtr->inerChIdx);
                }
#endif /* defined(COMM_PNC_NM_REQUEST */
            }
#if defined(COMM_WAKEUPSLEEP_RUST)
            /** @ref SWS_ComM_01070
             * channel enable WakeupSleepRequest and pnc old state COMM_PNC_NO_COMMUNICATION or
             * COMM_PNC_PREPARE_SLEEP,shall request channel COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST
             */
            else if (
                ((oldPncMode == COMM_PNC_NO_COMMUNICATION) || (oldPncMode == COMM_PNC_PREPARE_SLEEP))
                && (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_WAKEUPSLEEP_RUST)))
            {
                ComM_ChRequstCommMode(
                    chCfgPtr->inerChIdx,
                    chCfgPtr->pncInChIdex[pncIdx],
                    COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST);
            }
            /** @ref diff @ref SWS_ComM_01055
             * channel enable WakeupSleepRequest and pnc old state COMM_PNC_READY_SLEEP,shall request
             * channel COMM_FULL_COMMUNICATION
             * @ref SWS_ComM_01071
             * shall request channel COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST
             */
            else if (
                (oldPncMode == COMM_PNC_READY_SLEEP) && ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_WAKEUPSLEEP_RUST))
            {
                ComM_ChRequstCommMode(
                    chCfgPtr->inerChIdx,
                    chCfgPtr->pncInChIdex[pncIdx],
                    COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST);
            }
#endif /* defined(COMM_WAKEUPSLEEP_RUST */
            else
            {
#if (COMM_0PNC_VECTOR_AVOIDANCE == STD_ON)
                /** @ref SWS_ComM_00966 OBSOLETE replaced by @ref SWS_ComM_01084
                 * least one bit in the IRA changes back to '1' again, the ComM module shall request this ComMChannel
                 * again
                 */
                ComM_ChRequstCommMode(chCfgPtr->inerChIdx, chCfgPtr->pncInChIdex[pncIdx], COMM_FULL_COMMUNICATION);
#endif /* COMM_0PNC_VECTOR_AVOIDANCE == STD_ON */
            }
        }
    }
}

#if (COMM_PNC_USED_COM == STD_ON)
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc rx era com signal handle in mainfunction
 * @param[in]   NA
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ComRxEraMainHandle(void)
{
    if (TRUE == ComM_PncRxComSigEraInd)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_PncRxComSigEraInd = FALSE;
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
        uint8 rxEraBuf[COMM_PN_INFO_LENGTH * 2u] = {0};
        for (uintx index = 0; index < ComM_PncCfgPtr->comRxEraSigNum; index++)
        {
            const ComM_ComEiaRxSignalConfigType* eraSigCfgPtr = &ComM_PncCfgPtr->comEraRxSigPtr[index];
            Std_ReturnType ret = Com_ReceiveSignal(eraSigCfgPtr->sigId, rxEraBuf);
            if (E_OK == ret)
            {
                for (uintx i = eraSigCfgPtr->offset; i < eraSigCfgPtr->len; i++)
                {
                    rxEraBuf[i] &= eraSigCfgPtr->pncMask[i];
                }
                ComM_PncUpdateEra(eraSigCfgPtr->chIdx, &rxEraBuf[eraSigCfgPtr->offset]);
            }
#if COMM_FUNC_CHECK
            else
            {
                COMM_CHECK_OUTPUT("Com_ReceiveSignal signalId:%d return error\n", eraSigCfgPtr->sigId);
            }
#endif /* COMM_FUNC_CHECK */
        }
    }
}
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
/**
 * @ingroup     ComM pnc
 * @brief       pnc rx eira com signal handle in mainfunction
 * @param[in]   NA
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ComRxEiraMainHandle(void)
{
    if (TRUE == ComM_PncRxComSigEiraInd)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_PncRxComSigEiraInd = FALSE;
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
        uint8 rxEiraMerge[COMM_PN_INFO_LENGTH] = {0};
        boolean eiraEnCo = FALSE;
        for (uintx index = 0; index < ComM_PncCfgPtr->comRxEiraSigNum; index++)
        {
            /* get all eira signal value */
            uint8 rxsigBuf[COMM_PN_INFO_LENGTH * 2u];
            const ComM_ComEiraRxSignalConfigType* eiraSigCfgPtr = &ComM_PncCfgPtr->comEiraRxSigPtr[index];
            Std_ReturnType ret = Com_ReceiveSignal(eiraSigCfgPtr->sigId, rxsigBuf);
            if (E_OK == ret)
            {
                eiraEnCo = TRUE;
                for (uintx i = eiraSigCfgPtr->offset; i < eiraSigCfgPtr->len; i++)
                {
                    rxEiraMerge[i - eiraSigCfgPtr->offset] |= ((uint8)(rxsigBuf[i] & eiraSigCfgPtr->pncMask[i]));
                }
            }
#if COMM_FUNC_CHECK
            else
            {
                COMM_CHECK_OUTPUT("Com_ReceiveSignal signalId:%d return error\n", eiraSigCfgPtr->sigId);
            }
#endif /* COMM_FUNC_CHECK */
        }
        if (TRUE == eiraEnCo)
        {
            ComM_PncUpdateEira(rxEiraMerge);
        }
    }
}
#endif /* COMM_PNC_USED_COM == STD_ON */

/*==========================================[external function definitions]===========================================*/
/**
 * @ingroup     ComM pnc
 * @brief       get pnc state
 * @param[in]   pncIdx pnc index
 * @return      pncMode
 */
ComM_PncModeType ComM_PncGetStatus(uintx pncIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    return ComM_PncVar[pncIdx].pncMode;
}

#if (COMM_SYNC_WAKEUP == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc wakeup channel
 * @param[in]   chIdx  channel mapping pnc index
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_PncWakupByCh(uintx chIdx)
{
    /** @ref SWS_ComM_00931 update to @ref SWS_ComM_01063
     * ecum wakup channel shall be wakeup up all pncs mapped on this channel
     */
    for (uintx index = 0u; index < ComM_PncCfgPtr->pncListNum; index++)
    {
        uint32 pncOffsetId = ComM_PncCfgPtr->pncListCfgPtr[index].pncOffsetId;
        if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncOffsetId))
        {
            if (ComM_PncVar[index].pncMode == COMM_PNC_NO_COMMUNICATION)
            {
                ComM_PncBeh_EntryPrepareSleep(index);
            }
        }
    }
}
#endif /* COMM_SYNC_WAKEUP == STD_ON */

/**
 * @ingroup     ComM pnc
 * @brief       pnc wakeup
 * @param[in]   pncIdx  pnc index
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_PncWakupIndPnc(uintx pncIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /** @ref SWS_ComM_00964 update to @ref SWS_ComM_01064
     * pnc wakeup main state PNC_NO_COMMUNICATION shall be left and PNC_PREPARE_SLEEP shall be entered
     */
    if (pncIdx < ComM_PncCfgPtr->pncListNum)
    {
        /** setp0: wakeup this pnc channel
         * @ref SWS_ComM_01014 wakeup this pnc channel
         */
        const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];

        for (uintx chIdx = 0; chIdx < ComM_PncCfgPtr->pncMapTotalChannelNum; chIdx++)
        {
            const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[chIdx];

            if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncCfgPtr->pncOffsetId))
            {
                ComM_ChEcuMWakeup(chCfgPtr->inerChIdx);
#if (COMM_MANAGED_CHANNEL_SUPPORT == STD_ON)
                if (NULL_PTR != chCfgPtr->mangaingChIdx)
                {
                    ComM_ChEcuMWakeup(*chCfgPtr->mangaingChIdx);
                }
#endif /* COMM_MANAGED_CHANNEL_SUPPORT == STD_ON */
            }
        }
        /* setp1: a least one chnanel shall be map this pnc*/
        if (ComM_CommonGetBit(ComM_PncVar[pncIdx].setMask, COMM_PNC_MAP_LEAST_CHANNEL)
            && (ComM_PncVar[pncIdx].pncMode == COMM_PNC_NO_COMMUNICATION))
        {
            ComM_PncBeh_EntryPrepareSleep(pncIdx);
        }
    }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    else
    {
    }
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
}

/**
 * @ingroup     ComM pnc
 * @brief       pnc requst commodule
 * @param[in]   chIdx  channel inner index
 * @param[in]   reqIdex  request index data point in this pnc
 * @param[in]   comMode  request mode
 * @return      NA
 * @note        called by ComM_RequestComMode
 */
FUNC(void, COMM_CODE)
ComM_PncRequestComMode(uintx pncIdx, uintx usrInPncIdx, ComM_ModeType comMode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_PncVarType* pncVarPtr = &ComM_PncVar[pncIdx];

#if COMM_PNC_GATEWAY_ENABLED == STD_ON || COMM_TX_ONLY_PNC_ENABLE == STD_ON
    const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];

#endif
    ComM_ModeType modeReq = 0xffu;
    if ((comMode == COMM_FULL_COMMUNICATION) && !ComM_CommonGetBit(pncVarPtr->userReqTable, usrInPncIdx))
    {
        ComM_CommonSetBit(pncVarPtr->userReqTable, usrInPncIdx);
        if (pncVarPtr->userReqCnt == 0u)
        {
            modeReq = COMM_FULL_COMMUNICATION;
        }
        pncVarPtr->userReqCnt++;
    }
    else if ((comMode == COMM_NO_COMMUNICATION) && ComM_CommonGetBit(pncVarPtr->userReqTable, usrInPncIdx))
    {
        ComM_CommonClrBit(pncVarPtr->userReqTable, usrInPncIdx);
        pncVarPtr->userReqCnt--;
        if (pncVarPtr->userReqCnt == 0u)
        {
            modeReq = COMM_NO_COMMUNICATION;
        }
    }
    else
    {
        /* nothing */
    }

    if (modeReq == COMM_FULL_COMMUNICATION)
    {
        /** @ref SWS_ComM_00932
         * a user request full com ,pnc state COMM_PNC_NO_COMMUNICATION shall be entry COMM_PNC_REQUESTED
         * @ref SWS_ComM_01087
         * The sub state COMM_PNC_READY_SLEEP shall be left and the sub state COMM_PNC_REQUESTED shall be
         * entered if at least one ComMUser assigned to this PNC requests "FullCommunication"
         * @ref SWS_ComM_00948
         * COMM_PNC_REQUESTED shall be entered
         */
        if ((COMM_PNC_NO_COMMUNICATION == pncVarPtr->pncMode) || (COMM_PNC_READY_SLEEP == pncVarPtr->pncMode)
            || (COMM_PNC_PREPARE_SLEEP == pncVarPtr->pncMode))
        {
            ComM_PncBeh_EntryRequst(pncIdx);
            /** @ref SWS_ComM_00948
             * stop ComMPncPrepareSleepTimer
             */
            pncVarPtr->preSleeptimer = 0u;
        }

#if (COMM_TX_ONLY_PNC_ENABLE == STD_ON)
        /** @ref SWS_ComM_01078 @ref SWS_ComM_01073
         * no gateway,at least one user request pnc full commuication, shall request ComMChannelPerTxOnlyPnc
         * channel COMM_FULL_COMMUNICATION
         */
        for (uintx chIdx = 0u; chIdx < ComM_PncCfgPtr->txOnlPncChNum; chIdx++)
        {
            if (ComM_CommonGetBit(ComM_PncCfgPtr->txOnlyChToPncMap[chIdx], pncCfgPtr->pncOffsetId))
            {
                ComM_ChRequstCommMode(
                    ComM_PncCfgPtr->ChCfgPtr[chIdx]->inerChIdx,
                    ComM_PncCfgPtr->ChCfgPtr[chIdx]->pncInChIdex[pncIdx],
                    COMM_FULL_COMMUNICATION);
            }
        }
#endif /* COMM_TX_ONLY_PNC_ENABLE == STD_ON */
    }
    else if (modeReq == COMM_NO_COMMUNICATION)
    {
        /* shall be release this pnc */
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
        if (pncCfgPtr->pncGatewayEnable == TRUE)
        {
            /** @ref SWS_ComM_00991
             * pnc state is COMM_PNC_REQUESTED, gateway,all user request pnc no commuication
             * least one channel in this pnc
             * ern in ComMPncGatewayEnabled TRUE(active channel) is 0
             */
            uint8 eraActiveCnt = ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ)
                                     ? (pncVarPtr->eraCnt - 1u)
                                     : pncVarPtr->eraCnt;
            if ((ComM_PncGetStatus(pncIdx) == COMM_PNC_REQUESTED) && (0u == eraActiveCnt)
                && ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_MAP_LEAST_CHANNEL))
            {
                /** @ref SWS_ComM_01079 update @ref SWS_ComM_00959
                 * pnc state is COMM_PNC_REQUESTED, gateway,all user request pnc no commuication
                 * all active channel pnc era is 0 in this pnc
                 * passive channel pnc shall be set pnc 0 (or ComMPncGatewayType is not set @ref SWS_ComM_01080)
                 * include ComMChannelPerPnc or ComMChannelPerTxOnlyPnc
                 */
                for (uintx chIdx = 0u; chIdx < ComM_PncCfgPtr->pncMapTotalChannelNum; chIdx++)
                {
                    if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncCfgPtr->pncOffsetId)
#if (COMM_TX_ONLY_PNC_ENABLE == STD_ON)
                        || ((ComM_PncCfgPtr->txOnlPncChNum > chIdx)
                            && ComM_CommonGetBit(ComM_PncCfgPtr->txOnlyChToPncMap[chIdx], pncCfgPtr->pncOffsetId))
#endif /* COMM_TX_ONLY_PNC_ENABLE == STD_ON */
                    )
                    {
#if (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON)
                        /** @ref SWS_ComM_01050 update @ref SWS_ComM_01082
                         *  following actions
                         *  @ref SWS_ComM_01052 update @ref SWS_ComM_01083
                         * pnc all channel is COMM_PNC_GATEWAY_TPYE_ACTIVE,config ComMSynchronizedPncShutdownEnabled
                         * is on,shall initiative call pnc sync shutdown
                         */
                        if (ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_FRD_SYNC_SHUTDOWN)
                            || ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_TOP_GATEWAY))
                        {
#if defined(COMM_PNC_GATEWAY_TPYE_ACTIVE)
                            if (ComM_PncCfgPtr->ChCfgPtr[chIdx]->pncGateWayType == COMM_PNC_GATEWAY_TPYE_ACTIVE)
                            {
                                Nm_RequestSynchronizedPncShutdown(chIdx, pncCfgPtr->pncId);
                            }
#endif /* defined(COMM_PNC_GATEWAY_TPYE_ACTIVE */
                        }
#endif /* COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON */
                    }
                }
                ComM_PncBeh_EntryReadySleep(pncIdx);
            }
        }
        else
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
        {
            /** @ref SWS_ComM_00938
             * no gateway ,all user assigned this pnc request noCommunication ,COMM_PNC_REQUESTED shall be left and
             * the sub state COMM_PNC_READY_SLEEP shall be entered
             * @ref SWS_ComM_01075
             * all channel in this pnc ComMPncGatewayEnabled is not set
             */
            if (ComM_PncGetStatus(pncIdx) == COMM_PNC_REQUESTED)
            {
                ComM_PncBeh_EntryReadySleep(pncIdx);
                /** @ref SWS_ComM_01080
                 * no gateway or gateway,all user assigned this pnc request noCommunication ,COMM_PNC_REQUESTED
                 * shall be left and the sub state COMM_PNC_READY_SLEEP shall be entered
                 */
            }
        }
#if (COMM_TX_ONLY_PNC_ENABLE == STD_ON)
        /** @ref SWS_ComM_01074 @ref SWS_ComM_01081
         * no gateway or gateway,all user request pnc no commuication, shall request ComMChannelPerTxOnlyPnc
         * channel COMM_NO_COMMUNICATION
         */
        for (uintx chIdx = 0u; chIdx < ComM_PncCfgPtr->txOnlPncChNum; chIdx++)
        {
            if (ComM_CommonGetBit(ComM_PncCfgPtr->txOnlyChToPncMap[chIdx], pncCfgPtr->pncOffsetId))
            {
                ComM_ChRequstCommMode(
                    ComM_PncCfgPtr->ChCfgPtr[chIdx]->inerChIdx,
                    ComM_PncCfgPtr->ChCfgPtr[chIdx]->pncInChIdex[pncIdx],
                    COMM_FULL_COMMUNICATION);
            }
        }
#endif /* COMM_TX_ONLY_PNC_ENABLE == STD_ON */
    }
    else
    {
        /* no thing */
    }
}
/**
 * @ingroup     ComM pnc
 * @brief       pnc init
 * @param[in]   pncCfgPtr  pnc configure data point
 * @return      NA
 * @note        called by ComM_Init
 */
FUNC(void, COMM_CODE)
ComM_PncInit(P2CONST(ComM_PncConfigType, AUTOMATIC, COMM_APPL_CONST) pncCfgPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_PncCfgPtr = pncCfgPtr;

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
    if (NULL_PTR != ComM_PncCfgPtr->chanToPncMap)
    {
        uint32 copySize = (uint32)(COMM_PN_INFO_LENGTH * ComM_PncCfgPtr->pncMapTotalChannelNum);
        for (uintx index = 0; index < copySize; index++)
        {
            ((uint8*)ComM_ChToPncMap)[index] = ((const uint8*)(ComM_PncCfgPtr->chanToPncMap))[index];
        }
    }
#else
    ComM_ChToPncMap = ComM_PncCfgPtr->chanToPncMap;
#endif /* COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */
    /* per pnc init */
    for (uintx index = 0; index < ComM_PncCfgPtr->pncListNum; index++)
    {
        ComM_PncVarType* pncVarPtr = &ComM_PncVar[index];
        const ComM_PncItemConfigType* pncItemCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[index];
        pncVarPtr->pncMode = COMM_PNC_NO_COMMUNICATION;
        pncVarPtr->preSleeptimer = 0u;
        pncVarPtr->userReqTable = pncItemCfgPtr->requestMask;
        pncVarPtr->setMask[0] = 0u;
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
        pncVarPtr->eraActiveTable = pncItemCfgPtr->eraBuf;
        pncVarPtr->eraCnt = 0u;
        for (uintx i = 0u; i < pncItemCfgPtr->eraBufLen; i++)
        {
            pncVarPtr->eraActiveTable[i] = 0x0u;
        }
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
        if (0u < pncItemCfgPtr->pncMapChNum)
        {
            ComM_CommonSetBit(pncVarPtr->setMask, COMM_PNC_MAP_LEAST_CHANNEL);
        }
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)

        if (TRUE == pncItemCfgPtr->pncTopCoor)
        {
            ComM_CommonSetBit(pncVarPtr->setMask, COMM_PNC_TOP_GATEWAY);
        }
        else
        {
            ComM_CommonClrBit(pncVarPtr->setMask, COMM_PNC_TOP_GATEWAY);
        }
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
        pncVarPtr->userReqCnt = 0u;
        for (uintx i = 0u; i < pncItemCfgPtr->requestMaskLen; i++)
        {
            pncVarPtr->userReqTable[i] = 0x0u;
        }
    }
    for (uintx index = 0; index < COMM_PN_INFO_LENGTH; index++)
    {
        ComM_PncActiveIra[index] = 0;
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
        ComM_PncPassiveIra[index] = 0;
#endif
    }
#if (COMM_PNC_USED_COM == STD_ON)
    ComM_PncRxComSigEiraInd = FALSE;

#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
    ComM_PncRxComSigEraInd = FALSE;
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
#endif /* COMM_PNC_USED_COM == STD_ON */
}
/**
 * @ingroup     ComM pnc
 * @brief       pnc update Eira
 * @param[in]   pncBitEira  eira pnc bit
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncUpdateEira(P2CONST(uint8, AUTOMATIC, COMM_APPL_CONST) pncBitEira)
{
    for (uintx pncIdx = 0; pncIdx < ComM_PncCfgPtr->pncListNum; pncIdx++)
    {
        uintx pncId = ComM_PncCfgPtr->pncListCfgPtr[pncIdx].pncOffsetId;
        ComM_PncVarType* pncVarPtr = &ComM_PncVar[pncIdx];

        if (ComM_CommonGetBit(pncBitEira, pncId))
        {
            ComM_CommonSetBit(pncVarPtr->setMask, COMM_PNC_RX_EIRA_SET);

            /** @ref SWS_ComM_00933 @ref SWS_ComM_01065
             * eira=1 main state PNC_NO_COMMUNICATION shall be left and COMM_PNC_READY_SLEEP shall be entered
             */
            if (pncVarPtr->pncMode == COMM_PNC_NO_COMMUNICATION)
            {
                ComM_PncBeh_EntryReadySleep(pncIdx);
            }
            /** @ref SWS_ComM_00950
             * eira=1 ComMPncPrepareSleepTimer shall be stopped and the sub state COMM_PNC_READY_SLEEP shall be entered
             */
            else if (pncVarPtr->pncMode == COMM_PNC_PREPARE_SLEEP)
            {
                ComM_PncBeh_EntryReadySleep(pncIdx);
                pncVarPtr->preSleeptimer = 0u;
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            ComM_CommonClrBit(pncVarPtr->setMask, COMM_PNC_RX_EIRA_SET);
            /** @ref SWS_ComM_00940
             * If the PNC is released  the sub state COMM_PNC_READY_SLEEP shall be left and the sub state
             * COMM_PNC_PREPARE_SLEEP shall be entered
             */
            if (pncVarPtr->pncMode == COMM_PNC_READY_SLEEP)
            {
                ComM_PncBeh_EntryPrepareSleep(pncIdx);
            }
        }
    }
}

#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc update era
 * @param[in]   pncBitEra  era pnc bit
 * @return      NA
 */
COMM_LOCAL void ComM_PncUpdateEra(uintx chIdx, const uint8* pncBitEra)
{
    const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[chIdx];
#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
    if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[chIdx], COOMM_PNC_LEARN_STU))
    {
        /** @ref [SWS_ComM_01031] {OBSOLETE replaced by [SWS_ComM_01091]}
         * update pnc to channel mapping
         */
        for (uintx i = 0u; i < COMM_PN_INFO_LENGTH; i++)
        {
            ComM_ChToPncMap[chIdx][i] = pncBitEra[i];
        }
        /** @ref [SWS_ComM_01030] {OBSOLETE replaced by [SWS_ComM_01090]}
         * forward receiver era rx to all other channel where ComMPncDynamicMappingEnabled is true
         */
        ComM_PncLearnSendIraByCh(chIdx);
    }
#endif /* COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */
    for (uintx pncIdx = 0; pncIdx < ComM_PncCfgPtr->pncListNum; pncIdx++)
    {
        uintx pncId = ComM_PncCfgPtr->pncListCfgPtr[pncIdx].pncOffsetId;
        ComM_PncVarType* pncVarPtr = &ComM_PncVar[pncIdx];

        if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncId) && (NULL_PTR != pncVarPtr->eraActiveTable))
        {
            uintx chInPncIdx = ComM_PncCfgPtr->pncListCfgPtr[pncIdx].chMapEraIndex[chIdx];
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            if (ComM_CommonGetBit(pncBitEra, pncId))
            {
                if (!ComM_CommonGetBit(pncVarPtr->eraActiveTable, chInPncIdx))
                {
                    pncVarPtr->eraCnt++;
                    ComM_CommonSetBit(pncVarPtr->eraActiveTable, chInPncIdx);
                }
                if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_PASSIVE)
                {
                    if (!ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ))
                    {
                        ComM_CommonSetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ);
                    }
                }
                /** @ref [SWS_ComM_00165]
                 * pnc state is COMM_PNC_READY_SLEEP, channel gateway is set;era =1 and least one channel this pnc
                 * COMM_PNC_REQUESTED shall be entered
                 * @ref [SWS_ComM_00951]
                 * The timer ComMPncPrepareSleepTimer shall be stopped and the sub state COMM_PNC_REQUESTED shall be
                 * entered
                 */
                if (((COMM_PNC_READY_SLEEP == pncVarPtr->pncMode) || (COMM_PNC_PREPARE_SLEEP == pncVarPtr->pncMode))
#if defined(COMM_PNC_GATEWAY_TPYE_NONE)
                    && (chCfgPtr->pncGateWayType != COMM_PNC_GATEWAY_TPYE_NONE)
#endif /* defined(COMM_PNC_GATEWAY_TPYE_NONE */
                    && ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_MAP_LEAST_CHANNEL))
                {
                    ComM_PncBeh_EntryRequst(pncIdx);
                    /** @ref SWS_ComM_00951
                     * stop ComMPncPrepareSleepTimer
                     */
                    pncVarPtr->preSleeptimer = 0u;
                }
                /** @ref SWS_ComM_00934 @ref SWS_ComM_01066
                 * in main state COMM_PNC_NO_COMMUNICATION at least one PNC bit representing this PNC in ERAn changes to
                 * ‘1’, then the main state COMM_PNC_NO_COMMUNICATION shall be left and the sub state COMM_PNC_REQUESTED
                 *  shall be entered
                 */
                if ((pncVarPtr->pncMode == COMM_PNC_NO_COMMUNICATION) && (pncVarPtr->eraCnt > 0u)
#if defined(COMM_PNC_GATEWAY_TPYE_NONE)
                    && (chCfgPtr->pncGateWayType != COMM_PNC_GATEWAY_TPYE_NONE)
#endif /* defined(COMM_PNC_GATEWAY_TPYE_NONE */
                )
                {
                    ComM_PncBeh_EntryRequst(pncIdx);
                }
            }
            else
            {
                if (ComM_CommonGetBit(pncVarPtr->eraActiveTable, chInPncIdx))
                {
                    pncVarPtr->eraCnt--;
                    ComM_CommonClrBit(pncVarPtr->eraActiveTable, chInPncIdx);
                }
                if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_PASSIVE)
                {
                    if (ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ))
                    {
                        ComM_CommonClrBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ);
                    }
                }

                uint8 eraActiveCnt = ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ)
                                         ? (pncVarPtr->eraCnt - 1u)
                                         : pncVarPtr->eraCnt;
                if ((ComM_PncGetStatus(pncIdx) == COMM_PNC_REQUESTED) && (pncVarPtr->userReqCnt == 0u)
                    && (eraActiveCnt == 0u))
                {
                    /** @ref SWS_ComM_01079
                     * pnc state is COMM_PNC_REQUESTED, gateway,all user request pnc no commuication
                     * all active channel pnc era is 0 in this pnc
                     * passive channel pnc shall be set pnc 0
                     * include ComMChannelPerPnc or ComMChannelPerTxOnlyPnc
                     */
                    for (uintx index = 0u; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
                    {
                        if (ComM_CommonGetBit(ComM_ChToPncMap[index], pncId)
#if (COMM_TX_ONLY_PNC_ENABLE == STD_ON)
                            || ((index < ComM_PncCfgPtr->txOnlPncChNum)
                                && ComM_CommonGetBit(ComM_PncCfgPtr->txOnlyChToPncMap[index], pncId))
#endif /* COMM_TX_ONLY_PNC_ENABLE == STD_ON */
                        )
                        {
#if (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON)
                            /** @ref SWS_ComM_01050 update @ref SWS_ComM_01082
                             *  following actions
                             *  @ref SWS_ComM_01052 update @ref SWS_ComM_01083
                             * pnc all channel is COMM_PNC_GATEWAY_TPYE_ACTIVE,config
                             * ComMSynchronizedPncShutdownEnabled is on,shall initiative call pnc sync shutdown
                             */
                            if (ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_FRD_SYNC_SHUTDOWN)
                                || ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_TOP_GATEWAY))
                            {
                                if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_ACTIVE)
                                {
                                    Nm_RequestSynchronizedPncShutdown(
                                        chIdx,
                                        ComM_PncCfgPtr->pncListCfgPtr[pncIdx].pncId);
                                }
                            }
#endif /* COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON */
                        }
                    }
                    /** @ref SWS_ComM_00991
                     * pnc state is COMM_PNC_REQUESTED, gateway,all user request pnc no commuication
                     * least one channel in this pnc
                     * ern in ComMPncGatewayEnabled TRUE(active channel) is 0
                     */
                    if (pncVarPtr->eraCnt == 0u)
                    {
                        ComM_PncBeh_EntryReadySleep(pncIdx);
                    }
                }
            }
        }
    }
}
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */

#if (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON) && (COMM_PNC_GATEWAY_ENABLED == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc forward shutdown
 * @param[in]   chIdx  era pnc bit
 * @param[in]   pncBitVectorPtr  pnc vector bit
 * @return      NA
 */
FUNC(void, COMM_CODE)
ComM_PncForwardSyncPncShutdown(uintx chIdx, P2CONST(uint8, AUTOMATIC, COMM_APPL_CONST) pncBitVectorPtr)
{
    /** @ref SWS_ComM_01050 update @ref SWS_ComM_01082
     * following conditions apply
     */
    const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[chIdx];
    if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_PASSIVE)
    {
        for (uintx pncIdx = 0; pncIdx < ComM_PncCfgPtr->pncListNum; pncIdx++)
        {
            /** not in COMM_PNC_REQUESTED shall be ject
             */
            if (ComM_PncGetStatus(pncIdx) != COMM_PNC_REQUESTED)
            {
                continue;
            }
            uintx pncId = ComM_PncCfgPtr->pncListCfgPtr[pncIdx].pncOffsetId;
            if (ComM_CommonGetBit(ComM_ChToPncMap[chIdx], pncId))
            {
                if (ComM_CommonGetBit(pncBitVectorPtr, pncId))
                {
                    ComM_CommonSetBit(ComM_PncVar[pncIdx].setMask, COMM_PNC_FRD_SYNC_SHUTDOWN);
                }
                else
                {
                    ComM_CommonClrBit(ComM_PncVar[pncIdx].setMask, COMM_PNC_FRD_SYNC_SHUTDOWN);
                }
            }
        }
    }
}
#endif /* COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON) && (COMM_PNC_GATEWAY_ENABLED == STD_ON */

#if (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc learn indication
 * @param[in]   chIdx  era pnc bit
 * @param[in]   reqMode  FALSE:no learn TRUE: learn
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_PncLearnInd(uintx chIdx, boolean reqMode)
{
    if (TRUE == reqMode)
    {
        /** @ref SWS_ComM_01026
         * ComM_Nm_PncLearningBitIndication as been called on a channel PNC Learning Phase to active for the according
         * channel
         */
        ComM_CommonSetBit(ComM_PncLearnVar.setMask[chIdx], COOMM_PNC_LEARN_STU);
        /** @ref SWS_ComM_01092
         * gateway is false,comm shall set current membership to all channel where ComMDynamicPncToChannelMappingEnabled
         * is set true.
         * [SWS_ComM_01033] {OBSOLETE replaced by [SWS_ComM_01093]}
         * gateway shall be merge era ind
         */
        ComM_PncLearnSendIraByCh(ComM_PncCfgPtr->pncMapTotalChannelNum);
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
        const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[chIdx];
        uint8 gatewayMask = 0u;
        /** @ref SWS_ComM_01028
         * ComM_Nm_PncLearningBitIndication as been called on a channel and gateway enable,forward coordinated comm
         * channel
         */
#if defined(COMM_PNC_GATEWAY_TPYE_ACTIVE)
        if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_ACTIVE)
        {
#if defined(COMM_PNC_GATEWAY_TPYE_ACTIVE_PASSIVE)
            gatewayMask = COMM_PNC_GATEWAY_TPYE_ACTIVE_PASSIVE;
#endif /* defined(COMM_PNC_GATEWAY_TPYE_ACTIVE_PASSIVE */
        }
#endif /* defined(COMM_PNC_GATEWAY_TPYE_ACTIVE */
#if defined(COMM_PNC_GATEWAY_TPYE_PASSIVE)
        if (chCfgPtr->pncGateWayType == COMM_PNC_GATEWAY_TPYE_PASSIVE)
        {
#if defined(COMM_PNC_GATEWAY_TPYE_ACTIVE)
            gatewayMask = COMM_PNC_GATEWAY_TPYE_ACTIVE;
#endif /* defined(COMM_PNC_GATEWAY_TPYE_ACTIVE */
        }
#endif /* defined(COMM_PNC_GATEWAY_TPYE_PASSIVE */
        if (0u != gatewayMask)
        {
            for (uintx index = 0; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
            {
                chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[index];
                if (0u != (chCfgPtr->pncGateWayType & gatewayMask))
                {
                    ComM_CommonSetBit(ComM_PncLearnVar.setMask[chIdx], COOMM_PNC_LEARN_STU);
                    Nm_PnLearningRequest(chCfgPtr->inerChIdx);
                }
            }
        }
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
    }
    else
    {
        /** @ref SWS_ComM_01029
         * ComM_Nm_PncLearningBitIndication as been called on a channel PNC Learning Phase to active for the according
         * channel
         */
        ComM_CommonClrBit(ComM_PncLearnVar.setMask[chIdx], COOMM_PNC_LEARN_STU);
    }
}

/**
 * @ingroup     ComM pnc
 * @brief       pnc learn request
 * @param[in]   NA
 * @return      learn request execution success status
 * @retval      E_OK - success request learning
 * @retval      E_NOT_OK - request has been denied
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_PncLearningRequest(void)
{
    Std_ReturnType ret = E_OK;
#if defined(COMM_DYN_PNC_MAP_EN)

    for (uintx index = 0; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
    {
        const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[index];
        if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_DYN_PNC_MAP_EN))
        {
            if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[index], COOMM_PNC_LEARN_STU))
            {
                /** @ref SWS_ComM_01045
                 * least one pnc learning phase is active,shall return COMM_E_LEARNING_ACTIVE
                 */
                ret = COMM_E_LEARNING_ACTIVE;
            }
            else
            {
                ComM_ModeType mode;
                ComM_ChGetState(chCfgPtr->inerChIdx, &mode);
                if (mode != COMM_FULL_COMMUNICATION)
                {
                    /** @ref SWS_ComM_01058
                     * least one pnc learning phase is inactive,least one channel resides another state
                     * COMM_FULL_COMMUNICATION
                     */
                    ret = E_NOT_OK;
                }
            }
            if (ret != E_OK)
            {
                break;
            }
        }
    }
    if (E_OK == ret)
    {
        for (uintx index = 0; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
        {
            const ComM_ChannelConfigType* chCfgPtr = ComM_PncCfgPtr->ChCfgPtr[index];
            if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_DYN_PNC_MAP_EN))
            {
                /** @ref SWS_ComM_01046
                 * least one channel pnc learning phase is not active,least one channel resides another state
                 * COMM_FULL_COMMUNICATION
                 */
                ComM_CommonSetBit(ComM_PncLearnVar.setMask[index], COOMM_PNC_LEARN_STU);
                Nm_PnLearningRequest(chCfgPtr->inerChIdx);
            }
        }
    }
#endif /* defined(COMM_DYN_PNC_MAP_EN */
    return ret;
}

/**
 * @ingroup     ComM pnc
 * @brief       pnc update membership
 * @param[in]   Control Boolean Parameter: 0 = Unset the corresponding Bits in PncBit Mask
 * 1 = Set the corresponding Bits in PncBitMask
 * @param[in]   PncMembership Array of uint8 with <PNC Vector Length> Elements that holds the current PNC Membership of
 * the node
 * @return      update execution success status
 * @retval      E_OK: ComM_PncMembership successfully updated
 * @retval      E_NOT_OK: Error occurred while updating the PNC membership.
 * @retval      COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 * @note        called by ComM_UpdatePncMembership
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_PncUpdateMembership(boolean Control, P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncMembership)
{
    Std_ReturnType ret = E_NOT_OK;
    /** @ref SWS_ComM_01049
     * update local membership
     */
    for (uintx index = 0u; index < COMM_PN_INFO_LENGTH; index++)
    {
        if (Control == 0u)
        {
            ComM_PncLearnVar.membership[index] &= PncMembership[index];
        }
        else
        {
            ComM_PncLearnVar.membership[index] |= PncMembership[index];
        }
    }
    ret = E_OK;
    return ret;
}

#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc getting channel mapping
 * @param[out]  mappingTable Pointer to two-dimensional array with the current Pnc-to-channelmapping of the PNC Gateway
 * where the first dimension covers all relevant channels and the second all relevant PNCs.
 * @param[out]  channelCnt Pointer to number of ComM channels that are passed in the Mapping Table parameter
 * @param[out]  pncCnt Pointer to number of PNCs, that are passed in the MappingTable parameter
 * @return      get pnc mapping execution success status
 * @retval      E_OK: Successfully get PNC-to-channel-mapping entry
 * @retval      E_NOT_OK: Getting of PNC-to-channel-mapping entry failed
 * @retval      COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 * @note        called by ComM_GetPncToChannelMapping
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_PncGetPncMap(
    P2VAR(uint8*, AUTOMATIC, COMM_APPL_DATA) mappingTable,
    /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) channelCnt,
    /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) pncCnt /* PRQA S 3432 */ /* MISRA Rule 20.7 */)
{
    Std_ReturnType ret = E_OK;
    *channelCnt = ComM_PncCfgPtr->pncMapTotalChannelNum;
    *pncCnt = ComM_PncCfgPtr->pncListNum;
    uintx pncByteNum = COMM_VLU_MAPU8_BYTE(*pncCnt);
    for (uintx chIdex = 0u; chIdex < ComM_PncCfgPtr->pncMapTotalChannelNum; chIdex++)
    {
        /** @ref SWS_ComM_01036
         */
        for (uintx pncByteIdex = 0u; pncByteIdex < pncByteNum; pncByteIdex++)
        {
            mappingTable[chIdex][pncByteIdex] = ComM_ChToPncMap[chIdex][pncByteIdex];
        }
        /** @ref [SWS_ComM_01035]
         * least one channel and when PNC learning phase is active,shall return COMM_E_LEARNING_ACTIVE
         */
        if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[chIdex], COOMM_PNC_LEARN_STU))
        {
            ret = COMM_E_LEARNING_ACTIVE;
            break;
        }
    }
    return ret;
}
/**
 * @ingroup     ComM pnc
 * @brief       pnc set mapping channel
 * @param[in]   mappingTable Pointer to two-dimensional array with the current Pnc-to-channelmapping of the PNC Gateway
 * where the first dimension covers all relevant channels and the second all relevant PNCs.
 * @param[in]   channelCnt Pointer to number of ComM channels that are passed in the Mapping Table parameter
 * @param[in]   pncCnt Pointer to number of PNCs, that are passed in the MappingTable parameter
 * @return      set pnc mapping execution success status
 * @retval      E_OK: Successfully get PNC-to-channel-mapping entry
 * @retval      E_NOT_OK: Getting of PNC-to-channel-mapping entry failed
 * @retval      COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 * @note        called by ComM_UpdatePncToChannelMapping
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_PncSetPncMap(
    P2VAR(uint8*, AUTOMATIC, COMM_APPL_DATA) mappingTable /* PRQA S 3432 */ /* MISRA Rule 20.7 */,
    VAR(uint8, COMM_APPL_DATA) channelCnt,
    VAR(uint8, COMM_APPL_DATA) pncCnt)
{
    Std_ReturnType ret = E_OK;
    for (uintx index = 0; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
    {
        /** @ref [SWS_ComM_01035]
         * least one channel and when PNC learning phase is active,shall return COMM_E_LEARNING_ACTIVE
         */
        if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[index], COOMM_PNC_LEARN_STU))
        {
            ret = COMM_E_LEARNING_ACTIVE;
            break;
        }
    }
    if (E_OK == ret)
    {
        uintx byteNum = COMM_VLU_MAPU8_NUM(pncCnt);
        for (uintx index = 0; index < channelCnt; index++)
        {
            for (uintx i = 0u; i < byteNum; i++)
            {
                ComM_ChToPncMap[index][i] = mappingTable[index][i];
            }
        }
    }
    return ret;
}
/**
 * @ingroup     ComM pnc
 * @brief       This function resets dynamic entries within the ECUs PNC-to-channel-mapping to default values
 * @param[in]   NA
 * @return      set pnc mapping execution success status
 * @retval      E_OK: Successfully reset PNC-to-channel-mapping to default
 * @retval      E_NOT_OK: Reset of PNC-to-channel-mapping to default failed
 * @retval      COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 * @note        called by ComM_ResetPncToChannelMapping
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_PncResetPncMap(void)
{
    Std_ReturnType ret = E_OK;
    for (uintx index = 0; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
    {
        /** @ref [SWS_ComM_01035]
         * least one channel and when PNC learning phase is active,shall return COMM_E_LEARNING_ACTIVE
         */
        if (ComM_CommonGetBit(ComM_PncLearnVar.setMask[index], COOMM_PNC_LEARN_STU))
        {
            ret = COMM_E_LEARNING_ACTIVE;
            break;
        }
    }
    if (E_OK == ret)
    {
        uintx byteNum = COMM_VLU_MAPU8_NUM(ComM_PncCfgPtr->pncListNum);
        for (uintx index = 0u; index < ComM_PncCfgPtr->pncMapTotalChannelNum; index++)
        {
            for (uintx i = 0u; i < byteNum; i++)
            {
                ComM_ChToPncMap[index][i] = ComM_PncCfgPtr->chanToPncMap[index][i];
            }
        }
    }
    return ret;
}

#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
#endif /* COMM_DYN_PNC_TO_CH_MAPP_SUPPORT ==STD_ON */
COMM_LOCAL void ComM_PncUpdateIRA(uint8 chIdx, const uint8* ComM_PncIra)
{
    uint8 txVlu[COMM_PN_INFO_LENGTH] = {0};
    for (uint8 i = 0; i < COMM_PN_INFO_LENGTH; i++)
    {
        txVlu[i] = (ComM_PncIra[i] & ComM_ChToPncMap[chIdx][i]);
    }
    Nm_UpdateIRA(ComM_PncCfgPtr->ChCfgPtr[chIdx]->inerChIdx, txVlu);
}

COMM_LOCAL void ComM_TxIraMainHandle(void)
{
    uint8 pncActiveIraChanged[COMM_PN_INFO_LENGTH] = {0};
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    uint8 pncPassiveIraChanged[COMM_PN_INFO_LENGTH] = {0};
#endif
    for (uint8 pncIdx = 0u; pncIdx < ComM_PncCfgPtr->pncListNum; pncIdx++)
    {
        const ComM_PncItemConfigType* pncCfgPtr = &ComM_PncCfgPtr->pncListCfgPtr[pncIdx];
        uint8 pncOffsetId = pncCfgPtr->pncOffsetId;
        const ComM_PncVarType* pncVarPtr = &ComM_PncVar[pncIdx];
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
        uint8 passiveRequests = ComM_CommonGetBit(pncVarPtr->setMask, COMM_PNC_PASSIVE_CHREQ) ? 1u : 0u;
        uint8 activeRequests = pncVarPtr->eraCnt - passiveRequests + pncVarPtr->userReqCnt;
#else
        uint8 activeRequests = pncVarPtr->userReqCnt;
#endif
        if (!ComM_CommonGetBit(ComM_PncActiveIra, pncOffsetId)
            && ((activeRequests > 0u)
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
                || (passiveRequests > 0u)
#endif
                    ))
        {
            ComM_CommonSetBit(ComM_PncActiveIra, pncOffsetId);
            ComM_CommonSetBit(pncActiveIraChanged, pncOffsetId);
        }
        else if (
            ComM_CommonGetBit(ComM_PncActiveIra, pncOffsetId)
            && ((activeRequests == 0u)
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
                && (passiveRequests == 0u)
#endif
                    ))
        {
            ComM_CommonClrBit(ComM_PncActiveIra, pncOffsetId);
            ComM_CommonSetBit(pncActiveIraChanged, pncOffsetId);
        }
        else
        {
            /* do nothing */
        }

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
        if (!ComM_CommonGetBit(ComM_PncPassiveIra, pncOffsetId) && (activeRequests > 0u))
        {
            ComM_CommonSetBit(ComM_PncPassiveIra, pncOffsetId);
            ComM_CommonSetBit(pncPassiveIraChanged, pncOffsetId);
        }
        else if (ComM_CommonGetBit(ComM_PncPassiveIra, pncOffsetId) && (activeRequests == 0u))
        {
            ComM_CommonClrBit(ComM_PncPassiveIra, pncOffsetId);
            ComM_CommonSetBit(pncPassiveIraChanged, pncOffsetId);
        }
        else
        {
            /* do nothing */
        }
#endif
    }

    for (uint8 chIdx = 0u; chIdx < ComM_PncCfgPtr->pncMapTotalChannelNum; chIdx++)
    {
        const uint8* pncIraChanged = pncActiveIraChanged;
        const uint8* pncIra = ComM_PncActiveIra;
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
        if (ComM_PncCfgPtr->ChCfgPtr[chIdx]->pncGateWayType == COMM_PNC_GATEWAY_TPYE_PASSIVE)
        {
            pncIraChanged = pncPassiveIraChanged;
            pncIra = ComM_PncPassiveIra;
        }
#endif
        for (uint8 i = 0u; i < COMM_PN_INFO_LENGTH; i++)
        {
            if ((pncIraChanged[i] & ComM_ChToPncMap[chIdx][i]) != 0u)
            {
                ComM_PncUpdateIRA(chIdx, pncIra);
                break;
            }
        }
    }
}

#if (COMM_PNC_USED_COM == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       pnc mainfunction
 * @param[in]   NA
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_PncMainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
    ComM_ComRxEraMainHandle();
#endif
    ComM_ComRxEiraMainHandle();

    ComM_TxIraMainHandle();
}
#endif /* COMM_PNC_USED_COM == STD_ON */

/**
 * @ingroup     ComM pnc
 * @brief       pnc timer handle
 * @param[in]   NA
 * @return      NA
 * @note        place least mainfunction
 */
FUNC(void, COMM_CODE) ComM_PncProcessTimer(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    for (uintx index = 0; index < ComM_PncCfgPtr->pncListNum; index++)
    {
        ComM_PncVarType* pncVarPtr = &ComM_PncVar[index];
        if (pncVarPtr->preSleeptimer > 0u)
        {
            pncVarPtr->preSleeptimer--;
            if (pncVarPtr->preSleeptimer == 0u)
            {
                /* pnc shall switch COMM_PNC_NO_COMMUNICATION */
                ComM_PncBeh_EntryNoCom(index);
            }
        }
    }
}
#if (COMM_PNC_USED_COM == STD_ON)
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON)
/**
 * @ingroup     ComM pnc
 * @brief       com signal era indication
 * @param[in]   sigIdx inner era signal index
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_ComEraRxIndication(uint8 sigIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    COMM_PARA_UNUSED(sigIdx);
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    ComM_PncRxComSigEraInd = TRUE;
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}
#endif /* COMM_PNC_GATEWAY_ENABLED == STD_ON */
/**
 * @ingroup     ComM pnc
 * @brief       com signal eira indication
 * @param[in]   sigIdx inner eira signal index
 * @return      NA
 */
FUNC(void, COMM_CODE) ComM_ComEiraRxIndication(uint8 sigIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    COMM_PARA_UNUSED(sigIdx);
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    ComM_PncRxComSigEiraInd = TRUE;
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}
#endif /* COMM_PNC_USED_COM == STD_ON */

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif /* COMM_PNC_NUMBER > 0 */
