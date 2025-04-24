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
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Ch.c **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

/*================================================[inclusions]========================================================*/
#include "ComM_Internal.h"
#include "ComM_PBCfg.h"
#include "ComM_Ch.h"
#include "SchM_ComM.h"
#if (COMM_USED_MODULE_NM == STD_ON)
#include "Nm.h"
#endif /* COMM_USED_MODULE_NM == STD_ON */

#if (COMM_DCM_INDICATION == STD_ON)
#include "Dcm_Cbk.h"
#include "ComM_Dcm.h"
#endif /* COMM_DCM_INDICATION == STD_ON */

#if (COMM_BSWM_ENABLE == STD_ON)
#include "BswM_ComM.h"
#endif /* COMM_BSWM_ENABLE == STD_ON */

#if defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
#include "NvM.h"
#endif /* defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR */

#if !defined(COMM_USED_NM_EXD_FUNC)
#define COMM_USED_NM_EXD_FUNC 0
#endif /* !defined(COMM_USED_NM_EXD_FUNC */

#if COMM_USED_NM_EXD_FUNC
#include "NmExt.h"
#endif /* COMM_USED_NM_EXD_FUNC */

/*====================================================[macros]========================================================*/

/*==========================================[external data]===========================================================*/

/*==========================================[internal data]===========================================================*/
/** \brief comm channel post build configure data point */
#define COMM_START_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"
COMM_LOCAL P2CONST(ComM_ChannelConfigType, AUTOMATIC, COMM_APPL_CONST) ComM_ChannelConfigPtr;
#define COMM_STOP_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"

/** \brief channel manage val */
#define COMM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
COMM_LOCAL VAR(ComM_ChVarType, AUTOMATIC) ComM_ChVarTable[COMM_CHANNEL_NUMBER];
#define COMM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "ComM_MemMap.h"

#if COMM_FUNC_CHECK
/** \brief channel number var,max support 256u channel */
#define COMM_START_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
COMM_LOCAL VAR(uint8, AUTOMATIC) ComM_ChNum;
#define COMM_STOP_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
#endif /* COMM_FUNC_CHECK */

#if ((COMM_USER_NUMBER > 0u) && (COMM_USER_MODESWITCH_NOTIFY == STD_ON))
/** \brief user notify rte cache mode */
#define COMM_START_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
COMM_LOCAL VAR(ComM_ModeType, COMM_VAR) ComM_UserNotifyMode[COMM_USER_NUMBER] = {0};
#define COMM_STOP_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/*=========================================[internal function declarations]===========================================*/
COMM_LOCAL void ComM_ChStateTransitionToNoCom(uintx chIdx, uint32 currentReqCnt);
COMM_LOCAL void ComM_ChBeh_EntryFullCom(uintx chIdx, uint32 currentReqCnt);
COMM_LOCAL void ComM_ChBeh_EntryComReadySleep(uintx chIdx);
#if (COMM_USED_MODULE_NM == STD_ON)
COMM_LOCAL void ComM_ChBeh_EntrySlientMode(uintx chIdx);
COMM_LOCAL void ComM_ChannelNmModeEventHandle(NetworkHandleType Channel, uint32 currentReqCnt);
#endif /* COMM_USED_MODULE_NM == STD_ON */
/*==========================================[internal function definitions]===========================================*/
/**
 * @ingroup     ComM Channel
 * @brief       behaviour entry COMM_FULL_COM_NETWORK_REQUESTED this channel
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntryFullComReq(uintx chIdx, uint32 currentReqCnt)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    uint8 oldSepcMode = chVarPtr->specMode;
    chVarPtr->specMode = COMM_SPEC_FULL_COM_REQ;
#if (COMM_USED_MODULE_NM == STD_ON)
    uint8 nmVariantMask = 0u;
    nmVariantMask |= COMM_NM_VARIANT_FULL;
    nmVariantMask |= COMM_NM_VARIANT_PASSIVE;
    /** @ref SWS_ComM_00869
     * entry for anthoher state or substate ComMNmVariant=FULL
     */
    boolean passiveWakeup = ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
    /* clear passive wakeup */
    ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
    if ((oldSepcMode != COMM_SPEC_FULL_COM_REQ) && ((chCfgPtr->nmVariant & nmVariantMask) != 0u))
    {
        /* passive wakeup */
        if (passiveWakeup)
        {
            /** @ref SWS_ComM_00665
             * call
             */
            Std_ReturnType nmRet = Nm_PassiveStartUp(chCfgPtr->inerChIdx);
            if (nmRet == E_OK)
            {
                ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ);
            }
#if COMM_FUNC_CHECK
            else
            {
                FuncCheckOutput("Nm_PassiveStartUp:%d return:%d\n", chCfgPtr->inerChIdx, nmRet);
            }
#endif /* COMM_FUNC_CHECK */
        }

        /* by user request */
        if (currentReqCnt > 0u)
        {
            Std_ReturnType nmRet = Nm_NetworkRequest(chCfgPtr->inerChIdx);
            if (nmRet == E_OK)
            {
                ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ);
            }
#if COMM_FUNC_CHECK
            else
            {
                FuncCheckOutput("Nm_NetworkRequest:%d return:%d\n", chCfgPtr->inerChIdx, nmRet);
            }
#endif /* COMM_FUNC_CHECK */
        }
    }
#endif /* COMM_USED_MODULE_NM == STD_ON */
#if (COMM_MANAGED_CHANNEL_SUPPORT == STD_ON)
#if (defined(COMM_NM_VARIANT_LIGHT) || defined(COMM_NM_VARIANT_NONE))
    if (
#if (defined(COMM_NM_VARIANT_LIGHT) && defined(COMM_NM_VARIANT_NONE))
        (chCfgPtr->nmVariant == COMM_NM_VARIANT_LIGHT) || (chCfgPtr->nmVariant == COMM_NM_VARIANT_NONE)
#elif defined(COMM_NM_VARIANT_LIGHT)
        (chCfgPtr->nmVariant == COMM_NM_VARIANT_LIGHT)
#else
        (chCfgPtr->nmVariant == COMM_NM_VARIANT_NONE)
#endif
    )
    {
        /** @ref SWS_ComM_00889
         * ComMNmVariant=LIGHT|NONE no user requset (include dcm) ComMTMinFullComModeDuration expired shall be switch
         * COMM_FULL_COM_READY_SLEEP
         */
        if (NULL_PTR != chCfgPtr->mangaingChIdx)
        {
            boolean passiveWakeup = ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
            /* clear passive wakeup */
            ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
#if (COMM_USED_MODULE_NM == STD_ON)
            if (passiveWakeup != 0u)
            {
                /** @ref SWS_ComM_01016
                 * managed shall notify manging channel
                 */
                Std_ReturnType nmRet = Nm_PassiveStartUp(ComM_ChannelConfigPtr[*chCfgPtr->mangaingChIdx].inerChIdx);
                if (nmRet == E_OK)
                {
                    ComM_CommonSetBit(ComM_ChVarTable[*chCfgPtr->mangaingChIdx].funcMask, COMM_FUN_NMIF_REQ);
                }
#if COMM_FUNC_CHECK
                else
                {
                    FuncCheckOutput(
                        "Nm_PassiveStartUp:%d return:%d\n",
                        ComM_ChannelConfigPtr[*chCfgPtr->mangaingChIdx].inerChIdx,
                        nmRet);
                }
#endif /* COMM_FUNC_CHECK */
            }
#endif /* COMM_USED_MODULE_NM == STD_ON */
        }
#endif /* COMM_MANAGED_CHANNEL_SUPPORT == STD_ON */
    }
#endif
}
/**
 * @ingroup     ComM Channel
 * @brief       channel behaviour :entry COMM_NO_COM_REQUEST_PENDING
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntryNoComRequestPending(uintx chIdx, uint32 currentReqCnt)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    chVarPtr->specMode = COMM_SPEC_NOCOM_REQ_PENDING;
    /** @ref SWS_ComM_00895
     * in COMM_SPEC_NOCOM_REQ_PENDING and CommunicationAllowed is true,channel state shall immediatelt switch to state
     *  COMM_FULL_COMMUNICATION
     */
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_ALLOW_STU))
    {
        ComM_ChBeh_EntryFullCom(chIdx, currentReqCnt);
    }
}
/**
 * @ingroup     ComM Channel
 * @brief       channel behaviour :entry COMM_NO_COMMUNICATION
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntryNoCom(uintx chIdx)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    chVarPtr->specMode = COMM_SPEC_NOCOM_NOPENDING_REQ;
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    /** @ref SWS_ComM_00073
     * on entering state comm no commuication the ComM channel state machine shall switch off the transmit and reception
     */
#if (COMM_USED_MODULE_BUSSM == STD_ON)
    if (NULL_PTR != chCfgPtr->busSmCfgType)
    {

        chVarPtr->busSmReqMode = COMM_NO_COMMUNICATION;

        (void)chCfgPtr->busSmCfgType->busSm_RequestComMode(chCfgPtr->busIndex, COMM_NO_COMMUNICATION);
    }
#endif /* COMM_USED_MODULE_BUSSM == STD_ON */
#if (COMM_USED_MODULE_NM == STD_ON)
    /** @ref SWS_ComM_00288
     * entry state COMM_NO_COMMUNICATION and ComMNmVariant=FULL comm moudle shall call Nm_NetworkRelease
     */
    if ((chCfgPtr->nmVariant == COMM_NM_VARIANT_FULL) && (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ)))
    {
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ);
        (void)Nm_NetworkRelease(chCfgPtr->inerChIdx);
    }
#endif /* COMM_USED_MODULE_NM == STD_ON */
#if (COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON)
    /** @ref SWS_ComM_00355
     * ComMResetAfterForcingNoComm is open,when ComM enters COMM_NO_COMMUNICATION trans form
     * COMM_FULL_COM_NETWORK_REQUESTED,then ComM shall call bswm set ecu reset
     */
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_FORCE))
    {
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_FORCE);
#if (COMM_BSWM_ENABLE == STD_ON)
        BswM_ComM_InitiateReset();
#endif /* COMM_BSWM_ENABLE == STD_ON */
    }
#endif
}
/**
 * @ingroup     ComM Channel
 * @brief       channel behaviour :entry COMM_FULL_COM_READY_SLEEP
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntryComReadySleep(uintx chIdx)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    chVarPtr->specMode = COMM_SPEC_FULL_READY_SLEEP;
#if (COMM_USED_MODULE_NM == STD_ON)
    /** @ref SWS_ComM_00133
     * ComMNmVariant=FULL shall release network
     */
    if ((chCfgPtr->nmVariant == COMM_NM_VARIANT_FULL) && (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ)))
    {
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_NMIF_REQ);
        (void)Nm_NetworkRelease(chCfgPtr->inerChIdx);
    }
#endif /* COMM_USED_MODULE_NM == STD_ON */
#if (COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON)
    /** @ref SWS_ComM_00891
     * ComMNmVariant=LIGHT time shall be start ComMNmLightTimeout
     */
    if (chCfgPtr->nmVariant == COMM_NM_VARIANT_LIGHT)
    {
        chVarPtr->durationTmr = chCfgPtr->nmLightTimeout;
    }
#endif /* COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON */
#if defined(COMM_NM_VARIANT_SLAVE_ACTIVE)
    /* lin slave custom handle:switch ready sleep shall be request linsm nocom */
    if (chCfgPtr->nmVariant == COMM_NM_VARIANT_SLAVE_ACTIVE)
    {
#if (COMM_USED_MODULE_BUSSM == STD_ON)
        if (NULL_PTR != chCfgPtr->busSmCfgType)
        {
            chVarPtr->busSmReqMode = COMM_NO_COMMUNICATION;

            (void)chCfgPtr->busSmCfgType->busSm_RequestComMode(chCfgPtr->busIndex, COMM_NO_COMMUNICATION);
        }
#endif /* COMM_USED_MODULE_BUSSM == STD_ON */
    }
#endif /* defined(COMM_NM_VARIANT_SLAVE_ACTIVE */
#if defined(COMM_BUS_TYPE_INTERNAL)
    /** @ref SWS_ComM_00671
     * ComMBusType=COMM_BUS_TYPE_INTERNAL,state machine shall immediately switch to state COMM_NO_COMMUNICATION
     */
    if (chCfgPtr->busType == COMM_BUS_TYPE_INTERNAL)
    {
#if (COMM_USER_MODESWITCH_NOTIFY == STD_ON)
        /* virturl bussm ind */
        chVarPtr->busSmIndMode = COMM_NO_COMMUNICATION;
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_BUSSM_IND);
#endif /* COMM_USER_MODESWITCH_NOTIFY == STD_ON */
        ComM_ChBeh_EntryNoCom(chIdx);
    }
#endif /* defined(COMM_BUS_TYPE_INTERNAL */
}
/**
 * @ingroup     ComM Channel
 * @brief       channel behaviour :entry COMM_FULL_COMMUNICATION
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntryFullCom(uintx chIdx, uint32 currentReqCnt)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    uint8 oldState = chVarPtr->specMode;

#if (COMM_USED_MODULE_BUSSM == STD_ON)
    /** @ref SWS_ComM_00069
     * call bus nm switch on transmission and reception capability
     */
    if ((NULL_PTR != chCfgPtr->busSmCfgType)
        && ((oldState != COMM_SPEC_FULL_READY_SLEEP) && (oldState != COMM_SPEC_FULL_COM_REQ)))
    {
        ComM_ModeType busSmReqMode = COMM_FULL_COMMUNICATION;
#if defined(COMM_WAKEUPSLEEP_RUST)
        if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_WAKEUPSLEEP_RUST)
            && (chVarPtr->reqMode == COMM_REQMODE_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST))
        {
            busSmReqMode = COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST;
        }
#endif /* defined(COMM_WAKEUPSLEEP_RUST */
        /** @ref SWS_ComM_01057
         * COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST shall call bussm even if the ComM channel is already in
         * state,other channel shall ignore the request
         */
        if ((chVarPtr->busSmIndMode != COMM_FULL_COMMUNICATION) || (chVarPtr->busSmReqMode != busSmReqMode)
#if defined(COMM_WAKEUPSLEEP_RUST)
            || (COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST == busSmReqMode)
#endif /* defined(COMM_WAKEUPSLEEP_RUST */

        )
        {
            chVarPtr->busSmReqMode = busSmReqMode;

            (void)chCfgPtr->busSmCfgType->busSm_RequestComMode(chCfgPtr->busIndex, busSmReqMode);
        }
    }
#endif /* COMM_USED_MODULE_BUSSM == STD_ON */
#if (COMM_USER_MODESWITCH_NOTIFY == STD_ON)
#if defined(COMM_BUS_TYPE_INTERNAL)
    else
    {
        /* virtual bussm ind */
        if (chCfgPtr->busType == COMM_BUS_TYPE_INTERNAL)
        {
            chVarPtr->busSmIndMode = COMM_FULL_COMMUNICATION;
            ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_BUSSM_IND);
        }
    }
#endif /* defined(COMM_BUS_TYPE_INTERNAL */
#endif /* COMM_USER_MODESWITCH_NOTIFY == STD_ON */

    /** @ref SWS_ComM_00899
     * entering state COMM_FULL_COMMUNICATION comm channel state shall switch COMM_FULL_COM_NETWORK_REQUESTED,when
     * switch from COMM_SILENT_COMMUNICATION ,comm channel state can switch COMM_FULL_COM_READY_SLEEP
     * Rationale @ref SWS_ComM_00296
     */
    ComM_ChBeh_EntryFullComReq(chIdx, currentReqCnt);
}
#if (COMM_USED_MODULE_NM == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       channel behaviour :entry COMM_SILENT_COMMUNICATION
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChBeh_EntrySlientMode(uintx chIdx)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    chVarPtr->specMode = COMM_SPEC_SILENT_COM;
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#if (COMM_USED_MODULE_BUSSM == STD_ON)
    /** @ref SWS_ComM_00071
     * on entering state silent the ComM channel state machine shall switch off the transmit keep reception capability
     * on
     */
    if (NULL_PTR != chCfgPtr->busSmCfgType)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

        chVarPtr->busSmReqMode = COMM_SILENT_COMMUNICATION;
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

        (void)chCfgPtr->busSmCfgType->busSm_RequestComMode(chCfgPtr->busIndex, COMM_SILENT_COMMUNICATION);
    }
#endif /* COMM_USED_MODULE_BUSSM == STD_ON */
}
#endif /* COMM_USED_MODULE_NM == STD_ON */
/**
 * @ingroup     ComM Channel
 * @brief       in Mainfunction handle channel request mode
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL void ComM_ChReqModeMainHandle(uint8 chIdx)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    uint32 currentReqCnt = chVarPtr->reqCnt;
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
#if (COMM_USED_MODULE_NM == STD_ON)
    if ((chCfgPtr->nmVariant == COMM_NM_VARIANT_FULL) || (chCfgPtr->nmVariant == COMM_NM_VARIANT_PASSIVE))
    {
        ComM_ChannelNmModeEventHandle(chIdx, currentReqCnt);
    }
#endif
#if (                                                                                        \
    ((COMM_WAKEUP_INHIBITION_ENABLED == STD_ON) || (COMM_MODE_LIMITATION_ENABLED == STD_ON)) \
    && (COMM_DCM_INDICATION == STD_ON))                                                      \
    || (COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON)

#endif /* ((COMM_WAKEUP_INHIBITION_ENABLED == STD_ON) || (COMM_MODE_LIMITATION_ENABLED == STD_ON)) && \
          (COMM_DCM_INDICATION == STD_ON)) || (COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON */
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    ComM_ModeType modeReq = chVarPtr->reqMode;
    uint8 specMode = chVarPtr->specMode;
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    if ((modeReq == COMM_FULL_COMMUNICATION) || (modeReq == COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST)
        || ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP))
    {
        /** @ref SWS_ComM_00215
         * nocom limit is request,shall ignore all user requests with COMM_FULL_COMMUNICATION
         */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        if (!ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ)
#if (COMM_DCM_INDICATION == STD_ON)
            || ComM_CommonGetBit(chVarPtr->reqMask, chCfgPtr->dcmInReqIdx)
#endif
        )
#endif
        {
            switch (specMode)
            {
            case COMM_SPEC_NOCOM_REQ_PENDING:
                ComM_ChBeh_EntryFullCom(chIdx, currentReqCnt);
                break;
            /** @ref SWS_ComM_00875
             * in COMM_SPEC_NOCOM_NOPENDING_REQ and user request COMM_FULL_COMMUNICATION ,and com limit is disable
             * ch shall entry COMM_SPEC_NOCOM_REQ_PENDING
             * @ref SWS_ComM_00218
             * A communication request (COMM_FULL_COMMUNICATION) by a user shall be inhibited if the ComM Inhibition
             * status is equal to ComMNoWakeup is set to TRUE for the corresponding channel and the current state of the
             * channel is COMM_NO_COMMUNICATION or COMM_SILENT_COMMUNICATION
             */
            case COMM_SPEC_NOCOM_NOPENDING_REQ:
#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
                if (!ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP)
#if COMM_DCM_INDICATION == STD_ON
                    || ComM_CommonGetBit(chVarPtr->reqMask, chCfgPtr->dcmInReqIdx)
#endif
                )
#endif
                {
                    ComM_ChBeh_EntryNoComRequestPending(chIdx, currentReqCnt);
                }
                break;
            /** @ref SWS_ComM_00877 @ref SWS_ComM_00882
             * user reuqest full com and limit state shall be switch ful com state
             * @ref SWS_ComM_00878 @ref SWS_ComM_00883
             * dcm indicate ComM_DCM_ActiveDiagnostic ,and ComMNmVariant=FULL|LIGHT|NONE, comm state shall be switch
             * COMM_FULL_COMMUNICATION
             * @ref SWS_ComM_00218
             * open ComMNoWakeup in COMM_SILENT_COMMUNICATION state shall limit switch COMM_FULL_COMMUNICATION
             */
            case COMM_SPEC_FULL_READY_SLEEP:
#if COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON
                /** @ref SWS_ComM_00892
                 * ComMNmVariant=LIGHT In sub-state COMM_FULL_COM_READY_SLEEP
                 * switch COMM_FULL_COM_NETWORK_REQUESTED the timer for ComMNmLightTimeout shall be canceled
                 */
                if (chCfgPtr->nmVariant == COMM_NM_VARIANT_LIGHT)
                {
                    chVarPtr->durationTmr = 0u;
                }
#endif /* COMM_NM_VARIANT_LIGHT_SUPPORT == STD_ON */
                ComM_ChBeh_EntryFullComReq(chIdx, currentReqCnt);
                break;
            case COMM_SPEC_SILENT_COM:
#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
                if (!ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP)
#if COMM_DCM_INDICATION == STD_ON
#if defined(COMM_NM_VARIANT_NORMAL)
                    || (ComM_CommonGetBit(chVarPtr->reqMask, chCfgPtr->dcmInReqIdx)
                        && (0u != (chCfgPtr->nmVariant & COMM_NM_VARIANT_NORMAL)))
#endif /* defined(COMM_NM_VARIANT_NORMAL */
#endif /* COMM_DCM_INDICATION == STD_ON */
                )
#endif /* COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */
                {
                    ComM_ChBeh_EntryFullCom(chIdx, currentReqCnt);
                }
                break;
            default:
                /* do nothing */
                break;
            }
        }
    }

    ComM_ChStateTransitionToNoCom(chIdx, currentReqCnt);
}

COMM_LOCAL void ComM_ChStateTransitionToNoCom(uintx chIdx, uint32 currentReqCnt)
{
    const ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    uint8 oldState;
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    do
    {
        oldState = chVarPtr->specMode;
        switch (oldState)
        {
        case COMM_SPEC_FULL_COM_REQ:
            /** @ref SWS_ComM_00888
             * no user request and dcm diag is inactive ComMNmVariant=FULL in state
             * COMM_FULL_COM_NETWORK_REQUESTED,shall switch COMM_FULL_COM_READY_SLEEP
             */
            if (currentReqCnt == 0u)
            {
                /* CODE_ComM_00001 durationTmr(ComMTMinFullComModeDuration) is always 0 if nmVariant is FULL or
                 * SLAVE_ACTIVE (SWS_ComM_00889 */
#if defined COMM_NM_VARIANT_LIGHT || defined COMM_NM_VARIANT_NONE
                if (chVarPtr->durationTmr == 0u)
#endif /* defined COMM_NM_VARIANT_LIGHT || defined COMM_NM_VARIANT_NONE */
                {
                    ComM_ChBeh_EntryComReadySleep(chIdx);
                }
            }
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ))
            {
#if (COMM_DCM_INDICATION == STD_ON)
                if (!ComM_CommonGetBit(chVarPtr->reqMask, chCfgPtr->dcmInReqIdx))
#endif
                {
                    ComM_ChBeh_EntryComReadySleep(chIdx);
                }
            }
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
            break;

        case COMM_SPEC_FULL_READY_SLEEP:
#if defined COMM_NM_VARIANT_LIGHT
            /** @ref SWS_ComM_00610
             * ComMNmVariant=LIGHT,ComMNmLightTimeout has expired ComM channel state machine shall switch to state
             * COMM_NO_COMMUNICATION, durationTmr is always 0 if nmVariant != LIGHT
             */
            if ((chCfgPtr->nmVariant == COMM_NM_VARIANT_LIGHT) && (chVarPtr->durationTmr == 0u))
            {
                ComM_ChBeh_EntryNoCom(chIdx);
            }
#endif /* defined COMM_NM_VARIANT_LIGHT */
            break;
        case COMM_SPEC_NOCOM_REQ_PENDING:
            /** @ref SWS_ComM_00896 SWS_ComM_00897
             * no valid pending request COMM_FULL_COMMUNICATION, comm shall switch back to default
             * COMM_SPEC_NOCOM_NOPENDING_REQ
             */
            if ((chVarPtr->reqCnt == 0u) && !ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP))
            {
                ComM_ChBeh_EntryNoCom(chIdx);
            }
            break;
        default:
            /* do nothong */
            break;
        }
    } while (chVarPtr->specMode != oldState);
}

#if (COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       channel full comm request notification RTE
 * @param[in]   chIdx  channel inner index
 * @param[in]   reqIdex  request index data point in this channel
 * @param[in]   comMode  request mode
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChRequestUpdate(uintx chIdx, uintx reqIdex, ComM_ModeType comMode)
{
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    if ((NULL_PTR != chCfgPtr->fullComReqNotifyFunPtr) && (reqIdex < chCfgPtr->belongUserNum))
    {
        const ComM_UserConfigType* usrCfgPtr = chCfgPtr->belongUserListPtr[reqIdex].belongChUserCfgPtr;
#if COMM_FUNC_CHECK
        if ((NULL_PTR != usrCfgPtr) && (NULL_PTR != chCfgPtr->userFullComNotifyNumPtr)
            && (NULL_PTR != chCfgPtr->useruserFullComNotifyArryPtr))
#endif /* COMM_FUNC_CHECK */
        {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();
            uintx usrNum = *(chCfgPtr->userFullComNotifyNumPtr);
            if (comMode == COMM_FULL_COMMUNICATION)
            {
                boolean findFlg = FALSE;
                for (uintx index = 0u; index < usrNum; index++)
                {
                    if (chCfgPtr->useruserFullComNotifyArryPtr[index] == usrCfgPtr->userId)
                    {
                        findFlg = TRUE;
                        break;
                    }
                }
                if (FALSE == findFlg)
                {
                    /* shall be add this user request in channel mangage var */
                    chCfgPtr->useruserFullComNotifyArryPtr[usrNum] = usrCfgPtr->userId;
                    *(chCfgPtr->userFullComNotifyNumPtr) = *chCfgPtr->userFullComNotifyNumPtr + 1u;
                }
            }
            else
            {
                /* shall be delete this user request in channel mangage var */
                boolean findFlg = FALSE;
                for (uintx index = 0u; index < usrNum; index++)
                {
                    if (chCfgPtr->useruserFullComNotifyArryPtr[index] == usrCfgPtr->userId)
                    {
                        findFlg = TRUE;
                    }
                    if ((TRUE == findFlg) && ((index + 1) < usrNum))
                    {
                        chCfgPtr->useruserFullComNotifyArryPtr[index] =
                            chCfgPtr->useruserFullComNotifyArryPtr[index + 1u];
                    }
                }
                if (TRUE == findFlg)
                {
                    *(chCfgPtr->userFullComNotifyNumPtr) = *chCfgPtr->userFullComNotifyNumPtr - 1u;
                }
            }
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
            chCfgPtr->fullComReqNotifyFunPtr();
        }
#if COMM_FUNC_CHECK
        else
        {
            FuncCheckOutput("configure data error\n");
        }
#endif /* COMM_FUNC_CHECK */
    }
}
#endif /* COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON */

#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
/**
 * @ingroup     ComM Channel
 * @brief       channel state change handle
 * @param[in]   chIdx  channel inner index
 * @return      NA
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ChStateChangeHandle(uintx chIdx)
{
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    for (uintx index = 0u; index < chCfgPtr->belongUserNum; index++)
    {
        const ComM_UserConfigType* usrCfgPtr = chCfgPtr->belongUserListPtr[index].belongChUserCfgPtr;
        ComM_ModeType baseMode = COMM_FULL_COMMUNICATION;
        for (uintx i = 0u; i < usrCfgPtr->directChListNum; i++)
        {
            uintx pollChIdx = usrCfgPtr->directChList[i];
            ComM_ModeType busSmIndMode = ComM_ChVarTable[pollChIdx].busSmIndMode;
            if (busSmIndMode == COMM_BUS_SLEEP)
            {
                busSmIndMode = COMM_NO_COMMUNICATION;
            }
            if (busSmIndMode < baseMode)
            {
                baseMode = busSmIndMode;
            }
            if (baseMode == COMM_NO_COMMUNICATION)
            {
                break;
            }
        }
        ComM_UserStateChangeNotify(usrCfgPtr, baseMode);
    }
}
#endif

/*======================================[external function definitions]===============================================*/
/**
 * @ingroup     ComM Channel
 * @brief       channel internal initialization
 * @param[in]   channelConfigPtr  channel configure data point
 * @param[in]   chNum  channel number
 * @return      NA
 * @note        called by ComM_Init
 */
void ComM_ChInit(const ComM_ChannelConfigType* channelConfigPtr, uint32 chNum) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_ChannelConfigPtr = channelConfigPtr;
#if COMM_FUNC_CHECK
    ComM_ChNum = (uint8)chNum;
#endif /* COMM_FUNC_CHECK */
    for (uintx chIdx = 0u; chIdx < chNum; chIdx++)
    {
        /** @ref SWS_ComM_00313
         * default after init is COMM_NO_COMMUNICATION
         */
        const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
        ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];

        chVarPtr->specMode = COMM_SPEC_NOCOM_NOPENDING_REQ;
        /* reset bussm mode indication */
        chVarPtr->busSmIndMode = COMM_NO_COMMUNICATION;
        chVarPtr->reqCnt = 0u;
        chVarPtr->reqMask = chCfgPtr->requestMask;

        chVarPtr->funcMask[0] = 0u;
#if (COMM_USED_MODULE_NM == STD_ON)
        chVarPtr->NmModeEvent = COMM_NM_MODE_EVENT_NONE;
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_NO_COM_INHIBITION))
        {
            ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ);
        }
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON */

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        if (ComM_CommonGetBit(chCfgPtr->cfgMask, COMM_NO_WAKEUP_INHIBITION))
        {
            ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP);
        }
#endif /* COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

        for (uintx i = 0u; i < chCfgPtr->requestMaskLen; i++)
        {
            chVarPtr->reqMask[i] = 0u;
        }
#if (defined(COMM_NM_VARIANT_LIGHT) || defined(COMM_NM_VARIANT_NONE))
        chVarPtr->durationTmr = 0u;
#endif
#if (COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON)
        if (NULL_PTR != chCfgPtr->fullComReqNotifyFunPtr)
        {
            for (uintx i = 0u; i < chCfgPtr->belongUserNum; i++)
            {
                chCfgPtr->useruserFullComNotifyArryPtr[i] = 0xffu;
            }
            *(chCfgPtr->userFullComNotifyNumPtr) = 0u;
        }
#endif /* COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON */
    }

#if ((COMM_USER_NUMBER > 0u) && (COMM_USER_MODESWITCH_NOTIFY == STD_ON))
    for (uintx index = 0u; index < COMM_USER_NUMBER; index++)
    {
        ComM_UserNotifyMode[index] = COMM_NO_COMMUNICATION;
    }
#endif
}

/**
 * @ingroup     ComM Channel
 * @brief       get channel state
 * @param[in]   chIdx  channel inner index
 * @param[out]  chStu  channel state
 * @return      NA
 */
FUNC(void, COMM_CODE)
ComM_ChGetState(uintx chIdx, ComM_StateType* chStu) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    *chStu = ComM_ChVarTable[chIdx].specMode;
}

/**
 * @ingroup     ComM Channel
 * @brief       get channel max allowed mode
 * @param[in]   chIdx  channel inner index
 * @param[out]  NA
 * @return      ComM_ModeType:max allow mode of channel
 * @retval      COMM_NO_COMMUNICATION
 * @retval      COMM_FULL_COMMUNICATION
 * @note        called by ComM_GetMaxComMode
 */
ComM_ModeType ComM_ChGetMaxAllowMode(uintx chIdx)
{
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON || COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    const ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
#else
    COMM_PARA_UNUSED(chIdx);
#endif
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ))
    {
        return COMM_NO_COMMUNICATION;
    }
#endif
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    uint8 chSpecMode = chVarPtr->specMode;
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP) && (chSpecMode != COMM_FULL_COM_READY_SLEEP)
        && (chSpecMode != COMM_FULL_COM_NETWORK_REQUESTED))
    {
        return COMM_NO_COMMUNICATION;
    }
#endif
    return COMM_FULL_COMMUNICATION;
}

/**
 * @ingroup     ComM Channel
 * @brief       get bus nm comm mode
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[out]  ComMode - see ComM_ModeType
 * @return      Function execution success status
 * @retval      E_OK - success get mode
 * @retval      E_NOT_OK - request has been denied
 * @note        called by ComM_GetCurrentComMode
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_ChGetCurrentComMode(uintx chIdx, ComM_ModeType* ComMode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret;
#if COMM_FUNC_CHECK
    if (chIdx >= ComM_ChNum)
    {
        COMM_CHECK_OUTPUT("chidx out of reange\n chIdx:%d,chNum:%d", chIdx, ComM_ChNum);
        ret = E_NOT_OK;
    }
    else
#endif /* COMM_FUNC_CHECK */
    {
        const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
#if defined(COMM_BUS_TYPE_INTERNAL)
        if (chCfgPtr->busType != COMM_BUS_TYPE_INTERNAL)
#endif /* defined(COMM_BUS_TYPE_INTERNAL */
        {
#if (COMM_USED_MODULE_BUSSM == STD_ON)
            const ComM_BusSmConfigType* busSmCfgPtr = chCfgPtr->busSmCfgType;
#if COMM_FUNC_CHECK
            if ((NULL_PTR != busSmCfgPtr) && (NULL_PTR != busSmCfgPtr->busSm_GetCurrentComMode))
#endif /* COMM_FUNC_CHECK */
            {
                ret = busSmCfgPtr->busSm_GetCurrentComMode(chCfgPtr->busIndex, ComMode);
            }
#if COMM_FUNC_CHECK
            else
            {
                ret = E_NOT_OK;
            }
#endif /* COMM_FUNC_CHECK */

#else
            ret = E_NOT_OK;
#endif /* COMM_USED_MODULE_BUSSM == STD_ON */
        }
#if defined(COMM_BUS_TYPE_INTERNAL)
        else
        {
            ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
            *ComMode = chVarPtr->busSmIndMode;
        }
#endif /* defined(COMM_BUS_TYPE_INTERNAL */
    }
    return ret;
}
/**
 * @ingroup     ComM Channel
 * @brief       indicate to ComM when communication is allowed by channel
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   allowed - TRUE: Communication is allowed FALSE: Communication is not allowed
 * @return      NA
 * @note        called by ComM_CommunicationAllowed
 */
FUNC(void, COMM_CODE) ComM_ChComAllow(uintx chIdx, boolean allowed) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    if (TRUE == allowed)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_ALLOW_STU);
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    }
    else
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_ALLOW_STU);
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    }
}
/**
 * @ingroup     ComM Channel
 * @brief       notification that this channel bussm state
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   ComMode - busSm indication channel state
 * @return      NA
 * @note        called by ComM_BusSM_ModeIndication and ComM_BusSM_BusSleepMode
 */
FUNC(void, COMM_CODE) ComM_ChBusSmModeInd(uintx chIdx, ComM_ModeType comMode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    if (comMode != ComM_ChVarTable[chIdx].busSmIndMode)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        chVarPtr->busSmIndMode = comMode;
#if (COMM_USER_MODESWITCH_NOTIFY == STD_ON)
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_BUSSM_IND);
#endif /* COMM_USER_MODESWITCH_NOTIFY == STD_ON */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
        if (comMode != COMM_BUS_SLEEP)
        {
#if (COMM_BSWM_ENABLE == STD_ON)
            BswM_ComM_CurrentMode((NetworkHandleType)(chCfgPtr->inerChIdx), comMode);
#endif /* COMM_BSWM_ENABLE == STD_ON */
        }
        else
        {
#if defined(COMM_NM_VARIANT_SLAVE_ING)
            /** @ref SWS_ComM_01018 in state COMM_FULL_COMMUNICATION
             *  ComMNmVariant=SLAVE_ACTIVE | SLAVE_PASSIVE ComM_BusSm_BusSleepMode indicate shall be
             * switch COMM_NO_COMMUNICATION
             */
            if ((0u != (chCfgPtr->nmVariant & COMM_NM_VARIANT_SLAVE_ING))
                && ((chVarPtr->specMode == COMM_FULL_COM_READY_SLEEP)
                    || (chVarPtr->specMode == COMM_FULL_COM_NETWORK_REQUESTED)))
            {
                SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
                ComM_ChBeh_EntryNoCom(chIdx);
                SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
            }
#endif /* defined(COMM_NM_VARIANT_SLAVE_ING */
        }
#if (COMM_DCM_INDICATION == STD_ON)
        if (NULL_PTR != chCfgPtr->dcmNotifyIdPtr)
        {
            switch (comMode)
            {
            case COMM_FULL_COMMUNICATION:
                Dcm_ComM_FullComModeEntered(*(chCfgPtr->dcmNotifyIdPtr));
                break;

            case COMM_SILENT_COMMUNICATION:
                Dcm_ComM_SilentComModeEntered(*(chCfgPtr->dcmNotifyIdPtr));
                break;

            case COMM_NO_COMMUNICATION:
                Dcm_ComM_NoComModeEntered(*(chCfgPtr->dcmNotifyIdPtr));
                break;

            default:
                /* nothing to do */
                break;
            }
        }
#endif /* COMM_DCM_INDICATION == STD_ON */
    }
}

/**
 * @ingroup     ComM Channel
 * @brief       requesting of a communication mode this channel
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   reqIdex -index of channel request
 * @param[in]   indMode - requst mode
 * @return      Function execution success status
 * @retval      E_OK - Successfully changed to the new modee
 * @retval      E_NOT_OK - Changing to the new mode failed
 * @retval      COMM_E_MODE_LIMITATION - Mode can not be granted because of mode inhibition
 * @note        called by ComM_RequestComMode or pnc switch state
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ChRequstCommMode(uintx chIdx, uintx reqIdex, ComM_ModeType comMode)
{
    Std_ReturnType ret = E_OK;
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    ComM_ModeType modeReq = 0xffu;

    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    if (chCfgPtr->nmVariant != COMM_NM_VARIANT_PASSIVE)
    {
        if (!ComM_CommonGetBit(chVarPtr->reqMask, reqIdex)
            && ((comMode == COMM_FULL_COMMUNICATION) || (comMode == COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST)))
        {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            ComM_CommonSetBit(chVarPtr->reqMask, reqIdex);
            if (chVarPtr->reqCnt == 0u)
            {
                modeReq = COMM_FULL_COMMUNICATION;
            }
            chVarPtr->reqCnt++;
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

#if (COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON)
            ComM_ChRequestUpdate(chIdx, reqIdex, COMM_FULL_COMMUNICATION);
#endif /* COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON */
        }
        else if (ComM_CommonGetBit(chVarPtr->reqMask, reqIdex) && (comMode == COMM_NO_COMMUNICATION))
        {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            ComM_CommonClrBit(chVarPtr->reqMask, reqIdex);
            chVarPtr->reqCnt--;
            if (chVarPtr->reqCnt == 0u)
            {
                modeReq = COMM_NO_COMMUNICATION;
            }
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#if (COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON)
            ComM_ChRequestUpdate(chIdx, reqIdex, COMM_NO_COMMUNICATION);
#endif /* COMM_FULL_COMM_REQUEST_NOTIFY == STD_ON */
        }
        else
        {
            /* nothing */
        }
    }

    if (0xffu != modeReq)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        chVarPtr->reqMode = comMode;
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    }
    return ret;
}

/**
 * @ingroup     ComM Channel
 * @brief       channel passive wakeup by ecum mode
 * @param[in]   chIdx - index of channel same as ChannelId
 * @return      NA
 * @note        called by ComM_EcuM_PNCWakeUpIndication ComM_EcuM_WakeUpIndication
 */
FUNC(void, COMM_CODE) ComM_ChEcuMWakeup(uintx chIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
#if (COMM_MANAGED_CHANNEL_SUPPORT == STD_ON)
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
#endif /* COMM_MANAGED_CHANNEL_SUPPORT == STD_ON */
    /** @ref SWS_ComM_00893
     * in COMM_SPEC_NOCOM_NOPENDING_REQ and wakeup indication and ComMSynchronousWakeUp is false
     * comm shall switch COMM_SPEC_NOCOM_REQ_PENDING If the indicated ComM channel is a managed channel, then the ComM
     * channel state machine of the referencing managing channel shall be switch COMM_SPEC_NOCOM_REQ_PENDING
     */
    if (chVarPtr->specMode == COMM_SPEC_NOCOM_NOPENDING_REQ)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#if (COMM_MANAGED_CHANNEL_SUPPORT == STD_ON)
        if (NULL_PTR != chCfgPtr->mangaingChIdx)
        {
            chVarPtr = &ComM_ChVarTable[*chCfgPtr->mangaingChIdx];
            uint32 currentReqCnt = chVarPtr->reqCnt;
            if ((chVarPtr->specMode == COMM_SPEC_NOCOM_NOPENDING_REQ))
            {
                ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
            }
        }
#endif /* COMM_MANAGED_CHANNEL_SUPPORT == STD_ON */
    }
}
/**
 * @ingroup     ComM Channel
 * @brief       channel passive wakeup by nm mode
 * @param[in]   chIdx - index of channel same as ChannelId
 * @return      NA
 * @note        called by ComM_Nm_NetworkStartIndication ComM_Nm_RestartIndication
 */
FUNC(void, COMM_CODE) ComM_ChPassiveWakeup(uintx chIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_PASSIVE_WAKEUP);
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

    /** @ref SWS_ComM_00894
     * ComM_Nm_RestartIndication state immediately switch to sub-state COMM_SPEC_NOCOM_REQ_PENDING in state
     * COMM_SPEC_NOCOM_NOPENDING_REQ
     */
    /* shall be add handle is immediately*/
}
/**
 * @ingroup     ComM Channel
 * @brief       channel process handle
 * @param[in]   chIdx - index of channel same as ChannelId
 * @return      NA
 * @note        called by ComM_Mainfunction_{channel}
 */
FUNC(void, COMM_CODE) ComM_ChProcessTimer(uint8 chIdx) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (COMM_USER_MODESWITCH_NOTIFY == STD_ON) || defined(COMM_NM_VARIANT_LIGHT) || defined(COMM_NM_VARIANT_NONE)
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
#endif

#if (COMM_USER_MODESWITCH_NOTIFY == STD_ON)
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_BUSSM_IND))
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_BUSSM_IND);
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#if (COMM_USER_NUMBER > 0)
        ComM_ChStateChangeHandle(chIdx);
#endif /* COMM_USER_NUMBER > 0 */
    }
#endif /* COMM_USER_MODESWITCH_NOTIFY == STD_ON */

#if (defined(COMM_NM_VARIANT_LIGHT) || defined(COMM_NM_VARIANT_NONE))
    if (0u < chVarPtr->durationTmr)
    {
        chVarPtr->durationTmr--;
    }
#endif /* defined(COMM_NM_VARIANT_LIGHT) && defined(COMM_NM_VARIANT_NONE */

    ComM_ChReqModeMainHandle(chIdx);
}

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       channel switch inhbition status wakeup
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   stu FALSE: Wake up inhibition is switched off TRUE: Wake up inhibition is switched on
 * @note        called by ComM_PreventWakeUp
 */
void ComM_ChLimitWakeUp(uintx chIdx, boolean stu)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    if (TRUE == stu)
    {
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP);
    }
    else
    {
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP);
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}

#endif /* COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       channel switch inhbition status nocom
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   stu FALSE: Limit ECU to COMM_NO_COMMUNICATION disabled TRUE: Limit ECU to COMM_NO_COMMUNICATION
 * @note        called by ComM_LimitChannelToNoComMode and ComM_LimitECUToNoComMode
 */
void ComM_ChLimitNoCom(uintx chIdx, boolean stu) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    if (TRUE == stu)
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ);
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

        /** @ref SWS_ComM_00303
         * nocom limit is open in state COMM_FULL_COM_NETWORK_REQUESTED,comm channel shall be switch
         * COMM_FULL_COM_READY_SLEEP
         */
        if (COMM_SPEC_FULL_COM_REQ == chVarPtr->specMode)
        {
#if (COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON)
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            ComM_CommonSetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_FORCE);
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#endif /* COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON */
        }
    }
    else
    {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ);
#if (COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON)
        ComM_CommonClrBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_FORCE);
#endif /* COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    }
}
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON */

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/**
 * @ingroup     ComM Channel
 * @brief       requesting of a communication mode this channel check is limit
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   reqIdex -index of channel request
 * @param[in]   indMode - requst mode
 * @return      Function execution success status
 * @retval      E_OK - Successfully changed to the new modee
 * @retval      COMM_E_MODE_LIMITATION - Mode can not be granted because of mode inhibition
 * @note        called by ComM_RequestComMode or pnc switch state
 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_ChRequstCommModeLimitCheck(uintx chIdx, uintx reqIdex, ComM_ModeType comMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
    const ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];

    if ((COMM_FULL_COMMUNICATION == comMode)
#if (COMM_DCM_INDICATION == STD_ON)
        && (reqIdex != chCfgPtr->dcmInReqIdx)
#endif /* COMM_DCM_INDICATION == STD_ON */
    )
    {
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ))
        {
            ret = COMM_E_MODE_LIMITATION;
        }
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON */
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        /** @ref SWS_ComM_00218 channel is COMM_NO_COMMUNICATION or COMM_SILENT_COMMUNICATION not allow request
         * COMM_FULL_COMMUNICATION*/
        uint8 localSpecMode = chVarPtr->specMode;
        if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP)
            && ((localSpecMode != COMM_SPEC_FULL_COM_REQ) || (localSpecMode != COMM_SPEC_FULL_READY_SLEEP)))
        {
            ret = COMM_E_MODE_LIMITATION;
        }
#endif /* COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */
    }
    return ret;
}
#endif

#if (COMM_DCM_INDICATION == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       indication dcm diagnostic state switch
 * @param[in]   chIdx - index of channel same as ChannelId
 * @param[in]   stu Status FALSE: inactive diagnostic TRUE: active diagnostic
 * @return      NA
 * @note        called by ComM_DCM_ActiveDiagnostic and ComM_DCM_InactiveDiagnostic
 */
FUNC(void, COMM_CODE) ComM_ChDiagIndication(uintx chIdx, boolean flag) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[chIdx];
    if (TRUE == flag)
    {
#if defined(COMM_NM_VARIANT_NORMAL)
        /** @ref SWS_ComM_00866
         * ComMNmVariant=FULL|LIGHT|NONE DCM_ActiveDiagnostic indication shall be treated as a COMM_FULL_COMMUNICATION
         */
        if (0u != (chCfgPtr->nmVariant & COMM_NM_VARIANT_NORMAL))
        {
            (void)ComM_ChRequstCommMode(chIdx, chCfgPtr->dcmInReqIdx, COMM_FULL_COMMUNICATION);
        }
#endif /* defined(COMM_NM_VARIANT_NORMAL */
    }
    else
    {
        (void)ComM_ChRequstCommMode(chIdx, chCfgPtr->dcmInReqIdx, COMM_NO_COMMUNICATION);
    }
}
#endif /* COMM_DCM_INDICATION==STD_ON */

#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
/**
 * @ingroup     ComM Channel
 * @brief       user state change notify rte by modeswich
 * @param[in]   usrCfgPtr  channel inner index
 * @param[in]   mode  user comm mode
 * @return      NA
 */
FUNC(void, COMM_CODE)
ComM_UserStateChangeNotify(P2CONST(ComM_UserConfigType, AUTOMATIC, COMM_APPL_CONST) usrCfgPtr, ComM_ModeType mode)
{
    if ((usrCfgPtr->userNotifyFuncPtr) && (ComM_UserNotifyMode[usrCfgPtr->userId] != mode))
    {
        Std_ReturnType ret = E_NOT_OK;
        switch (mode)
        {
        case COMM_FULL_COMMUNICATION:
            ret = usrCfgPtr->userNotifyFuncPtr(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION);
            break;
        case COMM_SILENT_COMMUNICATION:
            ret = usrCfgPtr->userNotifyFuncPtr(RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION);
            break;
        case COMM_NO_COMMUNICATION:
            ret = usrCfgPtr->userNotifyFuncPtr(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION);
            break;
        default: /* COV_COMM_MISRA */
            break;
        }
        if (ret == E_OK)
        {
            ComM_UserNotifyMode[usrCfgPtr->userId] = mode;
        }
    }
}
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @ingroup     ComM Channel
 * @brief       get channel inhibition state
 * @param[in]   chIdx  channel inner index
 * @param[out]  chStu  channel inhibition state (bit mask) bit0 nocom bit1 wakeup
 * @return      NA
 * @note        called by ComM_GetInhibitionStatus
 */
void ComM_ChGetInhibitionStu(uintx chIdx, ComM_InhibitionStatusType* chStu) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    const ComM_ChVarType* chVarPtr = &ComM_ChVarTable[chIdx];
    *chStu = 0x0u;
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_NOCOM_REQ))
    {
        *chStu |= COMM_LIMITED_TO_NO_COM_YES;
    }
    if (ComM_CommonGetBit(chVarPtr->funcMask, COMM_FUN_LIMIT_WAKEUP))
    {
        *chStu |= COMM_WAKEUP_INHIBITION_ACTIVE_YES;
    }
}
#endif

#if (COMM_USED_MODULE_NM == STD_ON)
COMM_LOCAL void ComM_ChannelStoreNmModeEvent(uint8* Object, uint8 Desired)
{
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    *Object = Desired;
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}

void ComM_ChannelNmModeIndication(NetworkHandleType Channel, uint8 NmModeEvent)
{
    const ComM_ChannelConfigType* chCfgPtr = &ComM_ChannelConfigPtr[Channel];
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[Channel];
    if ((chCfgPtr->nmVariant == COMM_NM_VARIANT_FULL) || (chCfgPtr->nmVariant == COMM_NM_VARIANT_PASSIVE))
    {
        ComM_ChannelStoreNmModeEvent(&chVarPtr->NmModeEvent, NmModeEvent);
    }
}

COMM_LOCAL boolean ComM_ChannelCompareAndClearNmModeEvent(uint8* Object, uint8 Expected)
{
    boolean equal = FALSE;
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    if (*Object == Expected)
    {
        equal = TRUE;
        *Object = COMM_NM_MODE_EVENT_NONE;
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    return equal;
}

COMM_LOCAL void ComM_ChannelNmModeEventHandle(NetworkHandleType Channel, uint32 currentReqCnt)
{
    ComM_ChVarType* chVarPtr = &ComM_ChVarTable[Channel];

    if (ComM_ChannelCompareAndClearNmModeEvent(&chVarPtr->NmModeEvent, COMM_NM_IND_PREPARE_BUSSLEEP))
    {
        /* if channel state keep in COMM_FULL_COM_NETWORK_REQUESTED, active request has requested via
         * Nm_NetworkRequest() */
        if (chVarPtr->specMode == COMM_FULL_COM_READY_SLEEP)
        {
            ComM_ChBeh_EntrySlientMode(Channel);
        }
    }

    if (ComM_ChannelCompareAndClearNmModeEvent(&chVarPtr->NmModeEvent, COMM_NM_IND_NETWORK_MODE))
    {
        if (chVarPtr->specMode == COMM_SILENT_COM)
        {
            ComM_ChBeh_EntryFullCom(Channel, currentReqCnt);
        }
    }

    if (ComM_ChannelCompareAndClearNmModeEvent(&chVarPtr->NmModeEvent, COMM_NM_IND_BUS_SLEEP))
    {
        if ((chVarPtr->specMode != COMM_NO_COM_NO_PENDING_REQUEST)
            && (chVarPtr->specMode != COMM_NO_COM_REQUEST_PENDING))
        {
            ComM_ChBeh_EntryNoCom(Channel);
        }
    }
}
#endif

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
