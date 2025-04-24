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
/* *******************************************************************************
 **                                                                           **
 **  FILENAME    : UdpNm.c                                                    **
 **                                                                           **
 **  Created on  : 25/07/19                                                   **
 **  Author      : lili.wang                                                  **
 **  Vendor      :                                                            **
 **  DESCRIPTION :      none                                                  **
 **                                                                           **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
 **                                                                           **
 ******************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V1.0.0    20191118  lili.wang   Initial version
 *  V1.0.1    20191204  lili.wang
 *    1.UdpNm_MsgTxCntType change to uint8 from uint16
 *    2.Modify the immediate transmission mechanism,network request or
 *      config transmissons > 0,or UdpNmPnHandleMultipleNetworkRequest=TRUE
 *      transmit immediately
 *    3.immediately transmit retry:if the frist immediately transmit msg
 *      send fail,retry in the next mainfunction
 *    4.Ready sleep to NO state,transmit a Message immediately
 *    5.number followed by "U".
 *    6.because transmit immediately,the first msg should not request the PNC,
 *      so the PN Info data init cannot set to 0xff(1 mean request),should be 0
 *  V1.0.2    20191223  lili.wang
 *    1.recv Pn info data & with PnMask,then save the data to localdatabuf
 *  V1.0.3    20200421  lili.wang
 *    1.modify the Pn info data & with PnMask bug
 *  V2.0.0    20200609  HuRongbo
 *    1. Update code according AUTOSAR R19-11 Specification
 *    2. Modify code with the new coding rule
 *    3. Fix bugs find in V1.0.4
 *  V2.0.1    20200816  Wanglili
 *    1.Synchronize bugs found in CanNm coordination function.
 *    2.The state switching after the network is released is an asynchronous
 *    operation, and it is should judged whether the network is released or in
 *    the repeat msg state at the position where the message is sent.
 *  V2.0.2 20211102 darren.zhang
 *      1.Increased receiving pduinfo length over the configured value
 *          UDPNM_PDU_LENGTH for interception
 *  V2.0.3 20211102 darren.zhang
 *      1.Adjust the header file inclusion order
 *      2.sync pn filter modfify form cannm
 *  V2.0.4    2022-05-05  darren.zhang
 *      1.resolve build warnings
 *  V2.0.5    2022-06-03  darren.zhang
 *      1.resolve build warnings var udpNmTxEnable
 *  V2.0.6    2022-09-21  HuRongbo
 *      1.Delete Det check in UdpNm_MainFunction() (JIRA:PRD0032020-219)
 *  V2.0.7    2023-01-12  shengnan.sun
 *      1.resolve build warning
 *  V2.0.8    2023-04-03  shengnan.sun
 *      1.Solve the occasional frame loss problem reference CanNm
 *  V2.0.9    2023-06-12  xiaojian.liang
 *      Fixed issue CPT-644 - UdpNm_SetUserData null pointer exception
 *  V2.0.10   2023-10-16  xiaojian.liang
 *      Let API UdpNm_GetPduData, UdpNm_RepeatMessageRequest, UdpNm_GetNodeIdentifier and UdpNm_GetLocalNodeIdentifier
 *      always compiled.
 *  V2.0.11   2023-11-20   xiaojian.liang
 *      Performance optimization.
 *      Fixed CPT-8002 UdpNmNodeIdEnabled, UdpNmNodeDetectionEnabled, UdpNmRepeatMsgIndEnabled related features review
 *  V2.0.12   2024-01-24   xiaojian.liang
 *      CPT-6562 Source NID is always zero if UdpNmNodeIdEnabled is true but UdpNmNodeDetectionEnabled is false
 *  V2.0.13   2024-03-05   shengnan.sun
 *      Modify the error code UDPNM_E_NETWORK_TIMEOUT to 0x11u.
 *  V2.0.14   2024-03-09   shengnan.sun
 *      Support multiple partition feature.
 *  V2.0.15   2024-10-30   xiaojian.liang
 *      Fixed CPT-10964 If UdpNmComUserDataSupport is enabled the API UdpNm_SetUserData shall not be available.
 *  V2.0.16   2024-11-27   caihong.liu
 *      1. Fixed CPT-10987 - Fixed CanNm_Transmit precompilation condition and PduId comparison
 *      2. Fixed CPT-11294 - Fixed The PduR_CanNmRxIndication function is called within Critical Section
 ******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:UdpNm<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable.

    \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is global configuration data(1531) and API(1532).

    \li PRQA S 3678 VL_QAC_3678 .<br>
    Reason: Variables cannot be prefixed with const because they are used under other conditions to be compiled
 */

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define UDPNM_C_AR_MAJOR_VERSION 4u
#define UDPNM_C_AR_MINOR_VERSION 5u
#define UDPNM_C_AR_PATCH_VERSION 0u
#define UDPNM_C_SW_MAJOR_VERSION 2u
#define UDPNM_C_SW_MINOR_VERSION 0u
#define UDPNM_C_SW_PATCH_VERSION 16u

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "UdpNm.h"
#include "UdpNm_Internal.h"

#include "istd_lib.h"

#include "Nm.h"
#include "SchM_UdpNm.h"
#include "SoAd.h"
#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT) || (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
#include "PduR_UdpNm.h"
#endif
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
/* Multiple partition check. */
#include "Os.h"
#if (STD_ON == UDPNM_MULTIPLE_PARTITION_USED)
#include "ComM.h"
#endif
#endif /*UDPNM_DEV_ERROR_DETECT == STD_ON*/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (UDPNM_C_AR_MAJOR_VERSION != UDPNM_H_AR_MAJOR_VERSION)
#error "UdpNm.c : Mismatch in Specification Major Version"
#endif
#if (UDPNM_C_AR_MINOR_VERSION != UDPNM_H_AR_MINOR_VERSION)
#error "UdpNm.c : Mismatch in Specification Minor Version"
#endif
#if (UDPNM_C_AR_PATCH_VERSION != UDPNM_H_AR_PATCH_VERSION)
#error "UdpNm.c : Mismatch in Specification Patch Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#ifndef UDPNM_LOCAL
#define UDPNM_LOCAL static
#endif

#ifndef UDPNM_LOCAL_INLINE
#define UDPNM_LOCAL_INLINE static inline
#endif

#define UDPNM_UNUSED_PARAMETER(x) (void)(x)
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef uint8 UdpNm_ChannelIndexType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define UDPNM_START_SEC_CODE
#include "UdpNm_MemMap.h"

#if (STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED)
static FUNC(void, UDPNM_CODE) UdpNm_RxCarWakeUpHadle(uint8 chIndex);
#endif /* STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED */

UDPNM_LOCAL UdpNm_ChannelIndexType UdpNm_FindChannelIndex(NetworkHandleType nmChannelHandle);

static FUNC(void, UDPNM_CODE) UdpNm_StateChange(uint8 chIndex, Nm_StateType nmNewState);

static FUNC(void, UDPNM_CODE) UdpNm_EntryRepeatMessageState(uint8 chIndex, boolean isNetWorkRequest);

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
static FUNC(void, UDPNM_CODE) UdpNm_MsgcycleTimerManage(uint8 chIndex);
#endif
static FUNC(void, UDPNM_CODE) UdpNm_TimerManagement(uint8 chIndex);

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
static FUNC(boolean, UDPNM_CODE) UdpNm_RxPnFilter(
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST) chCfgPtr,
    UdpNm_InnerChannelType* chRTPtr,
    P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr);
#endif

#if (                                   \
    (STD_ON == UDPNM_GLOBAL_PN_ENABLED) \
    && ((STD_ON == UDPNM_PN_ERA_CALC_ENABLED) || (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)))
static FUNC(void, UDPNM_CODE) UdpNm_RxPnEiraEraHandle(uint8 chIndex);
#endif

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
static FUNC(void, UDPNM_CODE) UdpNm_PnEraResetTimerHandle(uint8 chIndex);
#endif /*STD_ON == UDPNM_PN_ERA_CALC_ENABLED*/

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
static FUNC(void, UDPNM_CODE) UdpNm_PnEiraResetTimerHandle(void);
#endif

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
static FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SendNmPdu(uint8 chIndex);
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if UDPNM_PN_EIRA_CALC_ENABLED == STD_ON
static void UdpNm_TxPnEiraHandle(NetworkHandleType networkHandle, uint8* txDta);
#endif

static FUNC(void, UDPNM_CODE) UdpNm_StateManagement(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_BusSleepStateHandle(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_PrepareBusSleepStateHandle(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_RepeatMessageStateHandle(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_NormalOperationStateHandle(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_ReadySleepStateHandle(uint8 chIndex);

static FUNC(void, UDPNM_CODE) UdpNm_RxDataMainHandle(uint8 chIndex);

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
static FUNC(void, UDPNM_CODE) UdpNm_SendMsgMainHandle(uint8 chIndex);
#endif /* UDPNM_PASSIVE_MODE_ENABLED == STD_OFF */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
static FUNC(void, UDPNM_CODE) UdpNm_CoordReadyToSleepIndHandle(uint8 chIndex);
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

static FUNC(void, UDPNM_CODE) UdpNm_EnterBusSleepModeHandle(uint8 chIndex);

#define UDPNM_STOP_SEC_CODE
#include "UdpNm_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define UDPNM_START_SEC_VAR_INIT_UNSPECIFIED
#include "UdpNm_MemMap.h"
static VAR(UdpNm_InitStatusType, UDPNM_VAR_POWER_ON_INIT) UdpNm_InitStatus = UDPNM_UNINIT;
#define UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_VAR_INIT_PTR
#include "UdpNm_MemMap.h"
static P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_APPL_CONST) UdpNm_CfgPtr = NULL_PTR;
#define UDPNM_STOP_SEC_VAR_INIT_PTR
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "UdpNm_MemMap.h"
static VAR(UdpNm_InnerChannelType, UDPNM_VAR) UdpNm_ChRunTime[UDPNM_NUMBER_OF_CHANNEL];

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
static VAR(UdpNm_InnerGlobalType, UDPNM_VAR) UdpNm_InnerGlobal;
#endif

#if UDPNM_DEV_ERROR_DETECT == STD_ON
static boolean UdpNm_ValidateInitStatus(uint8 apiId);
static boolean UdpNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle);
static boolean UdpNm_ValidatePointer(uint8 apiId, const void* pointer);
static boolean UdpNm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer);
static boolean UdpNm_ValidateNetworkHandlePointerAndPointer(
    uint8 apiId,
    NetworkHandleType networkHandle,
    const void* pointer1,
    const void* pointer2);
static boolean UdpNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId);
static boolean UdpNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr);
static boolean UdpNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
static boolean UdpNm_ValidateInit(const UdpNm_ConfigType* configPtr);
#endif

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS UdpNm_ValidatePointer
#endif

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "UdpNm_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define UDPNM_START_SEC_CODE
#include "UdpNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initialize the complete UdpNm module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      UdpNmConfigPtr, Pointer to a selected configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_Init(const UdpNm_ConfigType* UdpNmConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 chIndex;
    uint8 loop;
    UdpNm_InnerChannelType* chRTPtr;

    /* @req SWS_UdpNm_00210 */
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateInit(UdpNmConfigPtr))
#endif
    {
        /* @req SWS_UdpNm_00060 */
        UdpNm_CfgPtr = UdpNmConfigPtr;
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
        const UdpNm_PnInfoType* pnInfoPtr = UdpNm_CfgPtr->PnInfo;
#endif
        for (chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            chRTPtr = &UdpNm_ChRunTime[chIndex];

            /* @req SWS_UdpNm_00141 */
            /* Set state to NM_STATE_BUS_SLEEP */
            chRTPtr->udpNmState = NM_STATE_BUS_SLEEP;
            /* @req SWS_UdpNm_00144 */
            /* Mode enter Bus_Sleep Mode */
            chRTPtr->udpnmMode = NM_MODE_BUS_SLEEP;
            /* @req SWS_UdpNm_00143 */
            /* Set network to released */
            chRTPtr->netRequestStatus = UDPNM_NETWORK_RELEASED;

            /* Passive startup flag */
            chRTPtr->passiveStartUp = FALSE;

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
            /* @req SWS_UdpNm_00169*/
            chRTPtr->udpNmTxEnable = TRUE;
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
            chRTPtr->coordReadyToSleepInd = UDPNM_CSR_IND_CANCELLED;
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
            chRTPtr->remoteSleepInd = FALSE;
            chRTPtr->remoteTimer = 0x0u;
            chRTPtr->remoteTimerpassed = 0x0u;
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED)
            chRTPtr->immedMsgTxCnt = 0u;
#endif /* STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED */

            chRTPtr->stateTimer = 0x0u;
            chRTPtr->stateToutFlg = FALSE;
            /* @req SWS_UdpNm_00206 */
            /* Reset nm-timeout timer */
            chRTPtr->nmTimeoutTimer = 0x0u;
            chRTPtr->nmToutFlg = FALSE;

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
            /* @req SWS_UdpNm_00061 */
            /* Stop the transmission nm pdu message cycle timer */
            chRTPtr->msgTxCylceTimer = 0u;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
            chRTPtr->msgTimeoutTimer = 0u;
            chRTPtr->msgToutFlg = FALSE;
            chRTPtr->sendNmMsgFlg = FALSE;
            chRTPtr->netRequestFlg = FALSE;
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

            /* @req SWS_UdpNm_00025 */
            /* Set each byte of user data to 0xff */
            for (loop = 0u; loop < UDPNM_PDU_LENGTH; loop++)
            {
                chRTPtr->txPduData[loop] = 0xffu;
            }

            const UdpNm_ChannelConfigType* channelConfig = &UdpNm_CfgPtr->ChannelConfig[chIndex];
#if UDPNM_NODE_ID_ENABLED == STD_ON
            /* Set NodeId to config data */
            if (UDPNM_PDU_OFF != channelConfig->PduNidPosition)
            {
                chRTPtr->txPduData[channelConfig->PduNidPosition] = channelConfig->NodeId;
            }
#endif /* UDPNM_NODE_ID_ENABLED == STD_ON */

            /* Set CBV to config data */
            if (UDPNM_PDU_OFF != channelConfig->PduCbvPosition)
            {
                chRTPtr->txPduData[channelConfig->PduCbvPosition] = 0x00;
            }

/* pn fill PNI bit */
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
            /* @req SWS_UdpNm_00332,@req SWS_UdpNm_00333 */
            /* pnenable=true,shall set transmit PNI bit=1 */
            if (UdpNm_CfgPtr->ChannelConfig[chIndex].PnEnabled)
            {
                uint8 pnIdx;

                for (pnIdx = 0; pnIdx < pnInfoPtr->UdpNmPnInfoLength; pnIdx++)
                {
                    /*
                     * Because transmit immediately,the first msg should not
                     * request the PNC,so the PN Info data cannot set to 0xff.
                     */
                    chRTPtr->txPduData[pnInfoPtr->UdpNmPnInfoOffset + pnIdx] = 0x0u;
                }

                UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_PNI_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */

                chRTPtr->pnRxIndFlg = FALSE;
            }

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
            /* Clear pn ERA info buffer */
            for (loop = 0u; loop < pnInfoPtr->UdpNmPnInfoLength; loop++)
            {
                chRTPtr->pnInfoEra[loop] = 0x0u;
            }

            /* Clear pn ERA reset timer */
            for (loop = 0u; loop < UDPNM_PN_NUM; loop++)
            {
                chRTPtr->pnEraResetTimer[loop] = 0x0u;
            }
#endif
#endif
        }

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
        /* Clear pn EIRA info buffer */
        for (loop = 0u; loop < pnInfoPtr->UdpNmPnInfoLength; loop++)
        {
            /*@req SWS_UdpNm_00344*/
            UdpNm_InnerGlobal.pnEiraInfo[loop] = 0x0u;
        }

        /* Clear pn EIRA reset timer */
        for (loop = 0u; loop < UDPNM_PN_NUM; loop++)
        {
            UdpNm_InnerGlobal.pnEiraResetTimer[loop] = 0x0u;
        }
#endif

        UdpNm_InitStatus = UDPNM_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               Passive startup of the AUTOSAR UdpNm.
 * ServiceId           0x0e
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK:Passive startup of network management has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_PassiveStartUp(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_PASSIVESTARUP, nmChannelHandle))
#endif
    {
        SchM_Enter_UdpNm_Context();
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        /* @req SWS_UdpNm_00147 */
        /*
         * In network mode,shall not execute this service,
         * return E_NOT_OK.
         */
        if (NM_MODE_NETWORK != chRTPtr->udpnmMode)
        {
            chRTPtr->passiveStartUp = TRUE;
            ret = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }

    return ret;
}

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Request the network, since ECU needs to communicate on
 *                     the bus.Network state shall be changed to 'requested'.
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of network has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_NetworkRequest(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_NETWORKREQUEST, nmChannelHandle))
#endif
    {
        SchM_Enter_UdpNm_Context();
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        /* @req SWS_UdpNm_00104 */
        /* Change network state to request */
        chRTPtr->netRequestStatus = UDPNM_NETWORK_REQUESTED;
        chRTPtr->netRequestFlg = TRUE;
#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
        chRTPtr->coordReadyToSleepInd = UDPNM_CSR_IND_INIT;
#endif
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Release the network,since ECU doesn't have to communicate
 *                     on the bus.Network state shall be changed to 'released'.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Releasing of network has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_NetworkRelease(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_NETWORKRELEASE, nmChannelHandle))
#endif
    {
        SchM_Enter_UdpNm_Context();
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
        /* @req SWS_UdpNm_00105 */
        /* Change network state to release */
        chRTPtr->netRequestStatus = UDPNM_NETWORK_RELEASED;
        SchM_Exit_UdpNm_Context();
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
        /*
         * If UdpNm_NetworkRelease is called and NM PDU transmission ability
         * has been disabled, ECU will shut down.
         */
        if (FALSE == chRTPtr->udpNmTxEnable)
        {
            UdpNm_EnterBusSleepModeHandle(chIndex);
        }

#endif
        ret = E_OK;
    }
    return ret;
}
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Disable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (0x28) service.
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Disabling of NM PDU transmission ability has
 *                               failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_DisableCommunication(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

/* @req SWS_UdpNm_00307 */
/* In passive mode,the Service shall have no effect and return E_NOT_OK*/
#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_DISABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        SchM_Enter_UdpNm_Context();
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        if (NM_MODE_NETWORK == chRTPtr->udpnmMode)
        {
            /* @req SWS_UdpNm_00170 */
            /* Disable the NM PDU transmission ability */
            chRTPtr->udpNmTxEnable = FALSE;

            /* @req SWS_UdpNm_00173 */
            /*
             * Stop the UdpNm Message Cycle Timer in order to stop the
             * transmission of Network Management PDUs.
             */
            chRTPtr->msgTxCylceTimer = 0u;

            /* @req SWS_UdpNm_00174 */
            /* Transmit ability disable,shall stop nm timeout timer */
            chRTPtr->nmTimeoutTimer = 0u;

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_UdpNm_00175 */
            /*
             * When the NM PDU transmission ability is disabled,the
             * detection of Remote Sleep Indication Timer shall be
             * suspended.
             */
            chRTPtr->remoteTimerpassed = chRTPtr->remoteTimer;
            chRTPtr->remoteTimer = 0u;
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

            ret = E_OK;
        }
        else
        {
            /* @req SWS_UdpNm_00172 */
            /* Current mode is not network mode,shall return E_NOT_OK */
        }

        SchM_Exit_UdpNm_Context();
    }
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

    return ret;
}
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED*/

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Enable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (0x28) service.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Enabling of NM PDU transmission ability has
 *                               failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_EnableCommunication(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

/* @req SWS_UdpNm_00306 */
/* In passive mode,the Service shall have no effect and return E_NOT_OK*/
#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_ENABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        SchM_Enter_UdpNm_Context();
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        if ((FALSE == chRTPtr->udpNmTxEnable) && (NM_MODE_NETWORK == chRTPtr->udpnmMode))
        {
            chRTPtr->udpNmTxEnable = TRUE;

            /* @req SWS_UdpNm_00178 */
            /* Start transmission of NM PDUs. */
            chRTPtr->msgTxCylceTimer = 1u;

            /* @req SWS_UdpNm_00179 */
            /* Restart NM-Timeout Timer */
            chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

/* @req SWS_UdpNm_00180 */
/* Resumed remote sleep indication timer */
#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
            chRTPtr->remoteTimer = chRTPtr->remoteTimerpassed;
            chRTPtr->remoteTimerpassed = 0u;
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */
            ret = E_OK;
        }
        else
        {
            /* @req SWS_UdpNm_00177 */
            /* Transmission ability is enable,return E_NOT_OK */
            /* @req SWS_UdpNm_00305 */
            /* Current mode is not network mode,return E_NOT_OK */
        }
        SchM_Exit_UdpNm_Context();
    }
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

    return ret;
}
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */

#if (UDPNM_USER_DATA_ENABLED == STD_ON) && (UDPNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/******************************************************************************/
/*
 * Brief               Set user data for all NM messages transmitted on the bus
 *                     after this function has returned without error.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmUserDataPtr, Pointer where the user data for the next
 *                     transmitted NM message shall be copied from.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting of user data has failed.
 */
/******************************************************************************/
/* clang-format off */
Std_ReturnType UdpNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
/* clang-format on */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandleAndPointer(UDPNM_SERVICE_ID_SETUSERDATA, nmChannelHandle, nmUserDataPtr))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
        /* @req SWS_UdpNm_00159 */
        /* Set NM user data*/
        uint16 userDataLength = chCfgPtr->UserDataLength;
        uint16 userDataOffset = chCfgPtr->UserDataOffset;
        /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
        SchM_Enter_UdpNm_Context();
        /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
        uint8* userDataPtr = &chRTPtr->txPduData[userDataOffset];
        (void)ILib_memcpy(userDataPtr, nmUserDataPtr, userDataLength);
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif

#if (STD_ON == UDPNM_USER_DATA_ENABLED)
/******************************************************************************/
/*
 * Brief               Get user data from the most recently received NM message.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmUserDataPtr, Pointer where user data out of the most
 *                     recently received NM PDU shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of user data has failed.
 */
/******************************************************************************/
Std_ReturnType UdpNm_GetUserData(
    NetworkHandleType nmChannelHandle,
    uint8* nmUserDataPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandleAndPointer(UDPNM_SERVICE_ID_GETUSERDATA, nmChannelHandle, nmUserDataPtr))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        /* @req SWS_UdpNm_00160 */
        /* Provided most recently received NM PDU user data*/
        uint16 userDataLength = chCfgPtr->UserDataLength;
        uint16 userDataOffset = chCfgPtr->UserDataOffset;
        SchM_Enter_UdpNm_Context();
        const uint8* userDataPtr = &chRTPtr->rxPduData[userDataOffset];
        (void)ILib_memcpy(nmUserDataPtr, userDataPtr, userDataLength);
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif /* STD_ON ==  UDPNM_USER_DATA_ENABLED */

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType UdpNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_ID_ENABLED == STD_ON
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandleAndPointer(UDPNM_SERVICE_ID_GETNODEIDENTIFIER, nmChannelHandle, nmNodeIdPtr))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        /* @req SWS_UdpNm_00132 */
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeIdEnabled)
        {
            SchM_Enter_UdpNm_Context();
            *nmNodeIdPtr = chRTPtr->rxPduData[(uint8)chCfgPtr->PduNidPosition];
            SchM_Exit_UdpNm_Context();
            ret = E_OK;
        }
    }
#else
    UDPNM_UNUSED_PARAMETER(nmChannelHandle);
    UDPNM_UNUSED_PARAMETER(nmNodeIdPtr);
#endif /* UDPNM_NODE_ID_ENABLED == STD_ON */
    return ret;
}

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType UdpNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_ID_ENABLED == STD_ON
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandleAndPointer(UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmChannelHandle, nmNodeIdPtr))
#endif
    {
        /* @req SWS_UdpNm_00133 */
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeIdEnabled)
        {
            *nmNodeIdPtr = chCfgPtr->NodeId;
            ret = E_OK;
        }
    }
#else
    UDPNM_UNUSED_PARAMETER(nmChannelHandle);
    UDPNM_UNUSED_PARAMETER(nmNodeIdPtr);
#endif /* UDPNM_NODE_ID_ENABLED == STD_ON */
    return ret;
}

Std_ReturnType UdpNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST, nmChannelHandle))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled)
        {
            SchM_Enter_UdpNm_Context();
            UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

            /*@req SWS_UdpNm_00137 */
            /*
             * If the service is called in Repeat Message State,
             * Prepare Bus-Sleep Mode or Bus-Sleep Mode,the UdpNm module
             * shall not execute the service and return E_NOT_OK.
             */
            if ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState) || (NM_STATE_READY_SLEEP == chRTPtr->udpNmState))
            {
                chRTPtr->repeatMessageRequest = TRUE;

                ret = E_OK;
            }
            SchM_Exit_UdpNm_Context();
        }
    }
#else
    UDPNM_UNUSED_PARAMETER(nmChannelHandle);
#endif /* UDPNM_NODE_DETECTION_ENABLED == STD_ON */
    return ret;
}

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType UdpNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_USER_DATA_ENABLED == STD_ON || UDPNM_NODE_ID_ENABLED == STD_ON || UDPNM_NODE_DETECTION_ENABLED == STD_ON
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandleAndPointer(UDPNM_SERVICE_ID_GETPDUDATA, nmChannelHandle, nmPduDataPtr))
#endif
    {
        /* @req SWS_UdpNm_00138 @req SWS_UdpNm_00375 */
        /* Provide whole payload of the most recently received NM PDU */
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
#if UDPNM_USER_DATA_ENABLED == STD_OFF
        if (FALSE
#if UDPNM_NODE_ID_ENABLED == STD_ON
            || UdpNm_CfgPtr->ChannelConfig[chIndex].NodeIdEnabled
#endif /* UDPNM_NODE_ID_ENABLED == STD_ON */
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
            || UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled
#endif /* UDPNM_NODE_DETECTION_ENABLED == STD_ON */
        )
#endif /* UDPNM_USER_DATA_ENABLED == STD_OFF */
        {
            const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
            const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
            SchM_Enter_UdpNm_Context();
            (void)ILib_memcpy(nmPduDataPtr, chRTPtr->rxPduData, chCfgPtr->UdpNmPduLength);
            SchM_Exit_UdpNm_Context();
            ret = E_OK;
        }
    }
#else
    UDPNM_UNUSED_PARAMETER(nmChannelHandle);
    UDPNM_UNUSED_PARAMETER(nmPduDataPtr);
#endif /* UDPNM_USER_DATA_ENABLED == STD_ON || UDPNM_NODE_ID_ENABLED == STD_ON || UDPNM_NODE_DETECTION_ENABLED == \
          STD_ON */
    return ret;
}

/******************************************************************************/
/*
 * Brief               Returns the state and the mode of the network management.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmStatePtr, Pointer where state of the network
 *                                 management shall be copied to
 *                     nmModePtr, Pointer where the mode of the network
 *                                management shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of NM state has failed
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType UdpNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandlePointerAndPointer(UDPNM_SERVICE_ID_GETSTATE, nmChannelHandle, nmStatePtr, nmModePtr))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);

        SchM_Enter_UdpNm_Context();
        const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
        *nmStatePtr = chRTPtr->udpNmState;
        *nmModePtr = chRTPtr->udpnmMode;
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}

#if (STD_ON == UDPNM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo, Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETVERSIONINFO, versioninfo))
#endif /* UDPNM_DEV_ERROR_DETECT == STD_ON */
    {
        versioninfo->vendorID = UDPNM_VENDOR_ID;
        versioninfo->moduleID = UDPNM_MODULE_ID;
        versioninfo->sw_major_version = UDPNM_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = UDPNM_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = UDPNM_C_SW_PATCH_VERSION;
    }
}
#endif /* STD_ON == UDPNM_VERSION_INFO_API */

#if ((STD_ON == UDPNM_BUS_SYNCHRONIZATION_ENABLED) && (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED))
/******************************************************************************/
/*
 * Brief               Request bus synchronization.
 * ServiceId           0x14
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of bus synchronization has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION, nmChannelHandle))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        if ((NM_MODE_NETWORK == chRTPtr->udpnmMode)
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
            && (chRTPtr->udpNmTxEnable)
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */
        )
        {
            /* @req SWS_UdpNm_00130 */
            /*
             * This service shall trigger transmission of a single
             * Network Management PDU
             */
            chRTPtr->sendNmMsgFlg = TRUE;
            ret = E_OK;
        }
        else
        {
            /* @req SWS_UdpNm_00187 */
            /*
             * If UdpNm is in Bus-Sleep Mode and Prepare Bus-Sleep Mode
             * the Service shall not execute and Return E_NOT_OK.
             */

            /* @req SWS_UdpNm_00181 */
            /*
             * If the NM PDU transmission ability is disabled,the service
             * shall return E_NOT_OK.
             */
        }
    }

    return ret;
}
#endif

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Check if remote sleep indication takes place or not.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmRemoteSleepIndPtr, Pointer where check result of
 *                     remote sleep indication shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Checking of remote sleep indication bits failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmChannelHandle))
#endif
    {
        SchM_Enter_UdpNm_Context();
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

        if ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState) || (NM_STATE_READY_SLEEP == chRTPtr->udpNmState))
        {
            *nmRemoteSleepIndPtr = chRTPtr->remoteSleepInd;
            ret = E_OK;
        }
        else
        {
            /* @req SWS_UdpNm_00154 */
            /*
             * If UdpNm in Bus-Sleep Mode, Prepare Bus-Sleep Mode or
             * Repeat Message State,the service shall not be executed.
             * And E_NOT_OK shall be returned.
             */
        }
        SchM_Exit_UdpNm_Context();
    }

    return ret;
}
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Sets the NM coordinator ID in the control bit vector of
 *                     each NM message.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmCoordBits, 2 bit value to set the NM coordinator ID in
 *                     the control bit vector of each NM message
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting the coordinator ID bits has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_SetCoordBits(NetworkHandleType nmChannelHandle, uint8 nmCoordBits)
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_SETCOORDBITS, nmChannelHandle))
#endif
    {
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);

        UdpNm_SetTxPduCbvBit(chIndex, nmCoordBits << 1u); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        ret = E_OK;
    }
    return ret;
}
#endif /*STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT*/

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Set the NM Coordinator Sleep Ready bit in the Control
 *                     Bit Vector.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmSleepReadyBit, Value written to ReadySleep Bit in CBV
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Set of remote sleep indication bit has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit)
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_SETSLEEPREADYBIT, nmChannelHandle))
#endif
    {

        /* @req SWS_UdpNm_00321 */
        /*
         * UdpNm shall set the "NM Coordinator Sleep Ready Bit" bit to
         * passed value and trigger a single Network Management PDU.
         */
        UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);

        if (nmSleepReadyBit)
        {
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }
        else
        {
            UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }
        chRTPtr->sendNmMsgFlg = TRUE;
        ret = E_OK;
    }

    return ret;
}
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT)
/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      TxPduId, Identifier of the PDU to be transmitted
 *                     PduInfoPtr, Length of and pointer to the PDU data and
 *                                 pointer to Meta Data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
Std_ReturnType UdpNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateTxPduId(UDPNM_SERVICE_ID_TRANSMIT, TxPduId))
#endif
    {
        P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
        chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[0];
        UdpNm_InnerChannelType* chRTPtr;
        uint8 chIndex;

        for (chIndex = 0; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            const UdpNm_UserDataTxPduType* userDataPduPtr = chCfgPtr[chIndex].UserDataTxPdu;
            if ((userDataPduPtr != NULL_PTR) && (userDataPduPtr->TxUserDataPduId == TxPduId))
            {
                break;
            }
        }

        if (chIndex < UDPNM_NUMBER_OF_CHANNEL)
        {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON) && (STD_ON == UDPNM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            ApplicationType applicationID = GetApplicationID();

            if (applicationID
                != ComM_GetChannelApplicationID(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef))
            {
                UDPNM_DET_REPORT(UDPNM_SERVICE_ID_TRANSMIT, UDPNM_E_PARTITION);
            }
            else
#endif
            {
                chRTPtr = &UdpNm_ChRunTime[chIndex];
                /* @req SWS_UdpNm_00464 */
                if ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState)
                    || (NM_STATE_REPEAT_MESSAGE == chRTPtr->udpNmState))
                {
                    /*
                     * Request an additional transmission of the NM PDU with the current
                     * user data.
                     */
                    chRTPtr->sendNmMsgFlg = TRUE;

                    ret = E_OK;
                }
            }
        }
    }
    UDPNM_UNUSED_PARAMETER(PduInfoPtr);
    return ret;
}
#endif

#define UDPNM_STOP_SEC_CODE
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_UDPNMSOADIFTXCONFIRMATION_CALLBACK_CODE
#include "UdpNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms
 *                     the transmission of an IPDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      TxPduId, ID of the I-PDU that has been transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_SoAdIfTxConfirmation(PduIdType TxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateTxPduId(UDPNM_SERVICE_ID_IFTXCONFIRMATION, TxPduId))
#endif
    {
        P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
        chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[0];
        UdpNm_InnerChannelType* chRTPtr;
        uint8 chIndex;

        for (chIndex = 0; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            if (TxPduId == chCfgPtr->TxPdu->TxConfirmationPduId)
            {
                break;
            }
            chCfgPtr++;
        }

#if (UDPNM_DEV_ERROR_DETECT == STD_ON) && (STD_ON == UDPNM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != ComM_GetChannelApplicationID(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef))
        {
            UDPNM_DET_REPORT(UDPNM_SERVICE_ID_IFTXCONFIRMATION, UDPNM_E_PARTITION);
        }
        else
#endif
        {
            SchM_Enter_UdpNm_Context();
            chRTPtr = &UdpNm_ChRunTime[chIndex];

            /* @req SWS_UdpNm_00099 */
            /* Nm pdu transmit success,restart the nm-timeout timer */
            if (NM_MODE_NETWORK == chRTPtr->udpnmMode)
            {
                chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;
            }

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
            chRTPtr->msgTimeoutTimer = 0u;
            chRTPtr->msgToutFlg = FALSE;
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */
            SchM_Exit_UdpNm_Context();

#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT)
#if (0u < UDPNM_USERDATA_TX_PDU_NUM)
            if (NULL_PTR != chCfgPtr->UserDataTxPdu)
            {
                /* @req SWS_UdpNm_00316 */
                /* Call PduR_UdpNmTxConfirmation */
                PduR_UdpNmTxConfirmation(chCfgPtr->UserDataTxPdu->UpperLayerPduId);
            }
#endif /*0 < UDPNM_USERDATA_TX_PDU_NUM */
#endif /* STD_ON == UDPNM_COM_USERDATA_SUPPORT */
        }
    }
}
#define UDPNM_STOP_SEC_UDPNMSOADIFTXCONFIRMATION_CALLBACK_CODE
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_UDPNMSOADIFRXINDICATION_CALLBACK_CODE
#include "UdpNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      RxPduId, ID of the received PDU.
 *                     PduInfoPtr, Contains the length (SduLength) of the
 *                     received I-PDU and a pointer to a buffer (SduDataPtr)
 *                     containing the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_SoAdIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateRxPdu(UDPNM_SERVICE_ID_RXINDICATION, RxPduId, PduInfoPtr))
#endif
    {
        const UdpNm_ChannelConfigType* chCfgPtr;
        UdpNm_InnerChannelType* chRTPtr;
        uint8 chIndex;
        PduIdType pduIdx;

        for (chIndex = 0; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            {
                chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
                for (pduIdx = 0; pduIdx < chCfgPtr->RxPduNum; pduIdx++)
                {
                    if (RxPduId == chCfgPtr->RxPdu[pduIdx].RxPduId)
                    {
                        break;
                    }
                }

                if (pduIdx < chCfgPtr->RxPduNum)
                {
                    break;
                }
            }
        }
#if (UDPNM_DEV_ERROR_DETECT == STD_ON) && (STD_ON == UDPNM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != ComM_GetChannelApplicationID(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef))
        {
            UDPNM_DET_REPORT(UDPNM_SERVICE_ID_RXINDICATION, UDPNM_E_PARTITION);
        }
        else
#endif
        {
            chRTPtr = &UdpNm_ChRunTime[chIndex];

/* @req SWS_UdpNm_00337, @req SWS_UdpNm_00339*/
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
            if (UdpNm_RxPnFilter(chCfgPtr, chRTPtr, PduInfoPtr))
#endif
            {
                SchM_Enter_UdpNm_Context();
                PduLengthType copyLen =
                    (PduInfoPtr->SduLength > UDPNM_PDU_LENGTH) ? UDPNM_PDU_LENGTH : PduInfoPtr->SduLength;
                /* @req SWS_UdpNm_00035 */
                (void)ILib_memcpy(chRTPtr->rxPduData, PduInfoPtr->SduDataPtr, copyLen);
                chRTPtr->rxPduFlg = TRUE;
                SchM_Exit_UdpNm_Context();

/* @req SWS_UdpNm_00037 */
/* Notify nm layer,if rx pdu */
#if (STD_ON == UDPNM_PDU_RX_INDICATION_ENABLED)
                Nm_PduRxIndication(chCfgPtr->ComMNetworkHandleRef);
#endif /* STD_ON == UDPNM_PDU_RX_INDICATION_ENABLED */
            }
        }
    }
}
#define UDPNM_STOP_SEC_UDPNMSOADIFRXINDICATION_CALLBACK_CODE
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_UDPNMSOADIFTRIGGERTRANSMIT_CALLBACK_CODE
#include "UdpNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module)
 *                     shall check whether the available data fits into the
 *                     buffer size reported by PduInfoPtr->SduLength.If it fits,
 *                     it shall copy its data into the buffer provided by
 *                     PduInfoPtr->SduDataPtr and update the length of the
 *                     actual copied data in PduInfoPtr->SduLength. If not, it
 *                     returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      TxPduId,ID of the SDU that is requested to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr, Contains a pointer to a buffer (SduDataPtr)
 *                     to where the SDU data shall be copied, and the available
 *                     buffer size in SduLengh. On return, the service will
 *                     indicate the length of the copied SDU data in SduLength.
 * Return              E_OK: SDU has been copied and SduLength indicates the
 *                           number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must
 *                               not be used since it may contain a NULL pointer
 *                               or point to invalid data.
 */
/******************************************************************************/
/* PRQA S 1532, 3673 ++ */ /* MISRA Rule 8.7 Dir 8.13 */
Std_ReturnType UdpNm_SoAdIfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1532, 3673 -- */ /* MISRA Rule 8.7 Dir 8.13 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidateTxPdu(UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
        chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[0];
        UdpNm_InnerChannelType* chRTPtr; /* PRQA S 3678 */ /* VL_QAC_3678 */
        uint8 chIndex;

        if (PduInfoPtr->SduLength >= chCfgPtr->UdpNmPduLength)
        {
            for (chIndex = 0; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                if (TxPduId == chCfgPtr->TxPdu->TxConfirmationPduId)
                {
                    break;
                }
                chCfgPtr++;
            }

            if (chIndex < UDPNM_NUMBER_OF_CHANNEL)
            {
                chRTPtr = &UdpNm_ChRunTime[chIndex];

#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT)
                /* @req SWS_UdpNm_00377 */
                if (chCfgPtr->UserDataTxPdu != NULL_PTR)
                {
                    uint8 userData[UDPNM_USER_DATA_MAX_LENGTH] = {0};
                    PduInfoType pduInfo;
                    pduInfo.SduLength = chCfgPtr->UserDataLength;
                    pduInfo.SduDataPtr = userData;
                    if (PduR_UdpNmTriggerTransmit(chCfgPtr->UserDataTxPdu->UpperLayerPduId, &pduInfo) == E_OK)
                    {
                        SchM_Enter_UdpNm_Context();
                        (void)ILib_memcpy(
                            &chRTPtr->txPduData[chCfgPtr->UserDataOffset],
                            userData,
                            chCfgPtr->UserDataLength);
                        SchM_Exit_UdpNm_Context();
                    }
                }
#endif
                /* @req SWS_UdpNm_00378 */
                SchM_Enter_UdpNm_Context();
                (void)ILib_memcpy(PduInfoPtr->SduDataPtr, chRTPtr->txPduData, chCfgPtr->UdpNmPduLength);
                SchM_Exit_UdpNm_Context();

                ret = E_OK;
            }
        }
    }

    return ret;
}
#define UDPNM_STOP_SEC_UDPNMSOADIFTRIGGERTRANSMIT_CALLBACK_CODE
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_CODE
#include "UdpNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Main function of the UdpNm which processes the algorithm
 *                     describes in that document.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      channel, channel of the UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_MainFunction(NetworkHandleType channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if ((UdpNm_InitStatus == UDPNM_INIT) && (channel < UDPNM_NUMBER_OF_CHANNEL))
    {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON) && (STD_ON == UDPNM_MULTIPLE_PARTITION_USED)
        /* Multiple partition check. */
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != ComM_GetChannelApplicationID(UdpNm_CfgPtr->ChannelConfig[channel].ComMNetworkHandleRef))
        {
            UDPNM_DET_REPORT(UDPNM_SERVICE_ID_MAINFUNCTION, UDPNM_E_PARTITION);
        }
        else
#endif
        {

            UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[channel];
            if (chRTPtr->rxPduFlg)
            {
                chRTPtr->rxPduFlg = FALSE;
                chRTPtr->rxPduExtFlg = TRUE;
                UdpNm_RxDataMainHandle(channel);
            }

            UdpNm_TimerManagement(channel);
            UdpNm_StateManagement(channel);
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
            UdpNm_MsgcycleTimerManage(channel);
            UdpNm_SendMsgMainHandle(channel);
#endif /* UDPNM_PASSIVE_MODE_ENABLED == STD_OFF */
            /* Clear Flags */
            chRTPtr->rxPduExtFlg = FALSE;
#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
            chRTPtr->repeatMessageRequest = FALSE;
#endif
            chRTPtr->nmToutFlg = FALSE;
            chRTPtr->stateToutFlg = FALSE;
        }
    }
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
static inline boolean UdpNm_TestAndClear(boolean* flagPtr)
{
    boolean flag = *flagPtr;
    if (flag)
    {
        *flagPtr = FALSE;
    }
    return flag;
}

/******************************************************************************/
/*
 * Brief               Handle of Car Wakeup function
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED)
static FUNC(void, UDPNM_CODE) UdpNm_RxCarWakeUpHadle(uint8 chIndex)
{
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    uint8 carWakeupMask = (1u << (chCfgPtr->CarWakeUpBitPosition));
    SchM_Enter_UdpNm_Context();
    uint8 carWakeupByte = chRTPtr->rxPduData[chCfgPtr->CarWakeUpBytePosition];
    SchM_Exit_UdpNm_Context();

#if (STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED)
    if (UdpNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpRxEnabled && ((carWakeupByte & carWakeupMask) > 0u))
    {
#if (STD_ON == UDPNM_CAR_WAKEUP_FILTER_ENABLED)
#if !defined(UDPNM_VER_4_2_2)
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpFilterEnabled)
#endif
        {
            /* @req SWS_UdpNm_00376 */
            if (UDPNM_PDU_OFF != chCfgPtr->PduNidPosition)
            {
                SchM_Enter_UdpNm_Context();
                uint8 rxNodeId = chRTPtr->rxPduData[(uint8)chCfgPtr->PduNidPosition];
                SchM_Exit_UdpNm_Context();

                if (UdpNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpFilterNodeId == rxNodeId)
                {
                    Nm_CarWakeUpIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
                }
            }
        }
#if !defined(UDPNM_VER_4_2_2)
        else
#endif
#endif /* STD_ON == UDPNM_CAR_WAKEUP_FILTER_ENABLED */
        {
            /* @req SWS_UdpNm_00374 */
            Nm_CarWakeUpIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED */
}
#endif /* STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED */

/******************************************************************************/
/*
 * Brief               Return channel index in UdpNm corresponding to
 *                     nmChanelHandle from ComM
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      nmChannelHandle, Channel index in ComM
 * Param-Name[out]     chIndexPtr, Pointer where the channel index in UdpNm
 *                                 shall be copied to
 * Param-Name[in/out]  None
 * Return              TRUE: ComM channel configed in UdpNm
                       FALSE: ComM channel not configed in UdpNm
 */
/******************************************************************************/
UDPNM_LOCAL UdpNm_ChannelIndexType UdpNm_FindChannelIndex(NetworkHandleType nmChannelHandle)
{
    uint8 chIndex;

    for (chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (nmChannelHandle == UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef)
        {
            break;
        }
    }

    return chIndex;
}

/******************************************************************************/
/*
 * Brief               Store new state and notify new state to NM module
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 *                     nmNewState, new state of UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_StateChange(uint8 chIndex, Nm_StateType nmNewState)
{
    SchM_Enter_UdpNm_Context();
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
#if (STD_ON == UDPNM_STATE_CHANGE_IND_ENABLED)
    Nm_StateType udpNmState = chRTPtr->udpNmState;
#endif /* STD_ON == UDPNM_STATE_CHANGE_IND_ENABLED */
    chRTPtr->udpNmState = nmNewState;
    SchM_Exit_UdpNm_Context();

#if (STD_ON == UDPNM_STATE_CHANGE_IND_ENABLED)
    /* @req SWS_UdpNm_00166 */
    /* UdpNm states shall be notified to the upper layer.*/
    Nm_StateChangeNotification(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef, udpNmState, nmNewState);
#endif /* STD_ON == UDPNM_STATE_CHANGE_IND_ENABLED */
}

/******************************************************************************/
/*
 * Brief               Process when enter from other state to repeat msg state
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 *                     isNetWorkRequest, Indicate whether enter repeate message
 *                                       state triggered by NetworkRequest
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_EntryRepeatMessageState(uint8 chIndex, boolean isNetWorkRequest)
{
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    /* Load UdpNmRepeatMessageTime to stateTimer */
    chRTPtr->stateTimer = chCfgPtr->RepeatMessageTime;

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    /* @req SWS_UdpNm_00100 */
    /* Enter Repeat Message State,transmission shall start*/
    if ((TRUE != isNetWorkRequest) || (0u == chCfgPtr->ImmediateNmTransmissions))
    {
        /* @req SWS_UdpNm_00005 */
        /*
         * If the Repeat Message State is not entered via UdpNm_NetworkRequest
         * OR UdpNmImmediateNmTransmissions is zero the transmission of NM PDU
         * shall be delayed by UDPNM_MSG_CYCLE_OFFSET after entering the
         * repeat message state.
         */
        chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleOffset;

        if (chRTPtr->msgTxCylceTimer == 0u)
        {
            chRTPtr->msgTxCylceTimer = 1;
        }

#if (STD_ON == UDPNM_IMMEDIATE_RESTART_ENABLED)
        if ((isNetWorkRequest) && (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->udpnmMode))
        {
            /* @req SWS_UdpNm_00122 */
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;
        }
#endif
    }
#if (STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED)
    else
    {
        /* @req SWS_UdpNm_00334 @req SWS_UdpNm_00454 @req SWS_UdpNm_00122 */
        /*
         * The NM PDUs shall be transmitted using UdpNmImmediateNmCycleTime
         * as cycle time.
         * The first NM PDU shall be triggered as soon as possible.
         */
        if ((NM_MODE_BUS_SLEEP == chRTPtr->udpnmMode) || (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->udpnmMode)
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
            || ((NM_MODE_NETWORK == chRTPtr->udpnmMode) && (chCfgPtr->PnHandleMultipleNetworkRequests))
#endif /* STD_ON == UDPNM_GLOBAL_PN_ENABLED */
        )
        {
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;

            chRTPtr->msgTxCylceTimer = chCfgPtr->ImmediateNmCycleTime;
            chRTPtr->immedMsgTxCnt = chCfgPtr->ImmediateNmTransmissions;
        }
    }
#endif
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    /* @req SWS_UdpNm_00007 */
    if ((NM_MODE_BUS_SLEEP == chRTPtr->udpnmMode) || (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->udpnmMode))
    {
        chRTPtr->retrySendMsgFlag = chCfgPtr->RetryFirstMessageRequest;
    }
#endif
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
    /* @req SWS_UdpNm_00152 */
    /*
     * Entry Repeat Message state from Normal Operation State or Ready
     * Sleep State,shall cancel remote notify.
     */
    if ((chRTPtr->remoteSleepInd)
        && ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState) || (NM_STATE_READY_SLEEP == chRTPtr->udpNmState)))
    {
        chRTPtr->remoteSleepInd = FALSE;
        Nm_RemoteSleepCancellation(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
#endif /*STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

    UdpNm_StateChange(chIndex, NM_STATE_REPEAT_MESSAGE);

    if (NM_MODE_NETWORK != chRTPtr->udpnmMode)
    {
#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
        UdpNm_ChRunTime[chIndex].coordReadyToSleepInd = UDPNM_CSR_IND_INIT;
        UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

        /* @req SWS_UdpNm_00096 */
        /* Network Mode is entered,NM-Timeout Timer started.*/
        chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

        /* @req SWS_UdpNm_00097 @req SWS_UdpNm_00093 */
        /* Network Mode is entered, notify the upper layer by Nm_NetworkMode.*/
        chRTPtr->udpnmMode = NM_MODE_NETWORK;
        Nm_NetworkMode(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
}

/******************************************************************************/
/*
 * Brief               Process used timers
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
static FUNC(void, UDPNM_CODE) UdpNm_MsgcycleTimerManage(uint8 chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    if ((chRTPtr->msgTxCylceTimer > 0u)
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
        /* @req SWS_UdpNm_00173 */
        && (chRTPtr->udpNmTxEnable)
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */
    )
    {
        chRTPtr->msgTxCylceTimer--;
        if ((0u == chRTPtr->msgTxCylceTimer)
            && ((UDPNM_NETWORK_REQUESTED == chRTPtr->netRequestStatus)
                || (NM_STATE_REPEAT_MESSAGE == chRTPtr->udpNmState)))
        {
            /* @req SWS_UdpNm_00032 */
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;
        }
    }
}

#endif

/******************************************************************************/
/*
 * Brief               Process used timers
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_TimerManagement(uint8 chIndex)
{
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
    chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

/* Message timeout timer */
#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    if (chRTPtr->msgTimeoutTimer > 0u)
    {
        chRTPtr->msgTimeoutTimer--;
        if (0u == chRTPtr->msgTimeoutTimer)
        {
            chRTPtr->msgToutFlg = TRUE;

            /* @req SWS_UdpNm_00379 */
            Nm_TxTimeoutException(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

/* NM time out timer */
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
    /* @req SWS_UdpNm_00174 */
    /* Transmit ability disable,shall stop nm timeout timer */
    if (chRTPtr->udpNmTxEnable)
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */
    {
        if (chRTPtr->nmTimeoutTimer > 0u)
        {
            chRTPtr->nmTimeoutTimer--;
            if (0u == chRTPtr->nmTimeoutTimer)
            {
                chRTPtr->nmToutFlg = TRUE;

                if ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState)
                    || (NM_STATE_REPEAT_MESSAGE == chRTPtr->udpNmState))
                {
                    /* @req SWS_UdpNm_00117,@req SWS_UdpNm_00101 */
                    /*
                     * When the NM-Timeout Timer expires in the Normal
                     * Operation State or Repeat Message State,the
                     * NM-Timeout Timer shall be restarted.
                     */
                    chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

                    /* @req SWS_UdpNm_00194 @req SWS_UdpNm_00193
                     * @req SWS_UdpNm_00465 */
                    /*
                     * When the NM-Timeout Timer expires in the Repeat Message
                     * State or Normal Operation State, the NM shall report
                     * UDPNM_E_NETWORK_TIMEOUT as Run Time Error.
                     */
                    (void)Det_ReportRuntimeError(
                        UDPNM_MODULE_ID,
                        UDPNM_INSTANCE_ID,
                        UDPNM_SERVICE_ID_MAINFUNCTION,
                        UDPNM_E_NETWORK_TIMEOUT);
                }
            }
        }
    }

    /* State timer */
    if (chRTPtr->stateTimer > 0u)
    {
        chRTPtr->stateTimer--;
        if (0u == chRTPtr->stateTimer)
        {
            chRTPtr->stateToutFlg = TRUE;
        }
    }

/* Remote sleep timer and detected logic calculate */
#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
    if ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState) && (chRTPtr->remoteTimer > 0u))
    {
        chRTPtr->remoteTimer--;
        if (0u == chRTPtr->remoteTimer)
        {
            /* @req SWS_UdpNm_00150 */
            chRTPtr->remoteSleepInd = TRUE;
            Nm_RemoteSleepIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    if (chCfgPtr->PnEraCalcEnabled)
    {
        UdpNm_PnEraResetTimerHandle(chIndex);
    }
#endif /* STD_ON == UDPNM_PN_ERA_CALC_ENABLED */

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    if (UDPNM_EIRA_HANDLE_CHANNEL == chIndex)
    {
        UdpNm_PnEiraResetTimerHandle();
    }
#endif /* STD_ON == UDPNM_PN_EIRA_CALC_ENABLED */
}

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
/******************************************************************************/
/*
 * Brief               Determine whether have relevant PN information to this
 *                     channel according CBV data and channel config data
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chCfgPtr, Channel config data pointer
 *                     chRTPtr, Channel runtime data pointer
 *                     PduInfoPtr, Received pduInfo pointer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              TRUE: Have relevant PN information
 *                     FALSE: No relevant PN information
 */
/******************************************************************************/
static FUNC(boolean, UDPNM_CODE) UdpNm_RxPnFilter(
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST) chCfgPtr,
    UdpNm_InnerChannelType* chRTPtr,
    P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr)
{
    boolean ret = TRUE;

    /*@req SWS_UdpNm_00328*/
    if (chCfgPtr->PnEnabled)
    {
        uint8 cbv = PduInfoPtr->SduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
        ret = chCfgPtr->AllNmMessagesKeepAwake;
        /* @req SWS_UdpNm_00331 */
        if ((cbv & UDPNM_CBV_BIT_PNI_MASK) > 0u)
        {
            uint8 pduIdx;
            P2CONST(UdpNm_PnInfoType, AUTOMATIC, UDPNM_APPL_CONST)
            pnInfoPtr = UdpNm_CfgPtr->PnInfo;
            P2CONST(UdpNm_PnFilterMaskByteType, AUTOMATIC, UDPNM_APPL_CONST)
            pnFilMaskByte = pnInfoPtr->PnFilterMaskByte;
            for (pduIdx = 0u; pduIdx < pnInfoPtr->UdpNmPnInfoLength; pduIdx++)
            {
                uint8 byteIndex = pnInfoPtr->UdpNmPnInfoOffset + pnFilMaskByte[pduIdx].PnFilterMaskByteIndex;
                if ((PduInfoPtr->SduDataPtr[byteIndex] & pnFilMaskByte[pduIdx].PnFilterMaskByteValue) > 0u)
                {
                    chRTPtr->pnRxIndFlg = TRUE;
                    ret = TRUE;
                    break;
                }
            }
        }
    }
    return ret;
}
#endif /* STD_ON == UDPNM_GLOBAL_PN_ENABLED */

#if (                                   \
    (STD_ON == UDPNM_GLOBAL_PN_ENABLED) \
    && ((STD_ON == UDPNM_PN_ERA_CALC_ENABLED) || (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)))
/******************************************************************************/
/*
 * Brief               Handle EIRA and ERA according received data
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_RxPnEiraEraHandle(uint8 chIndex)
{
    P2CONST(UdpNm_PnInfoType, AUTOMATIC, UDPNM_APPL_CONST)
    pnInfoPtr = UdpNm_CfgPtr->PnInfo;
#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    uint8* pnEiraInfoPtr = UdpNm_InnerGlobal.pnEiraInfo;
    uint16* pnEiraResetTimerPtr = UdpNm_InnerGlobal.pnEiraResetTimer;
#endif
    uint8 rxPnInfoDta;
    uint8 bitMask;
#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    boolean rxIndEraFlag = FALSE;
#endif
#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    boolean rxIndEiraFlag = FALSE;
#endif
    uint8 index;
    P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) rxPnDtaPtr;
    uint8 filterMaskData;
    uint8 pnIdx = 0u;
    uint8 find0To1;

    SchM_Enter_UdpNm_Context();
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex]; /* PRQA S 3678 */ /* VL_QAC_3678 */
#if UDPNM_PN_ERA_CALC_ENABLED == STD_ON
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
#endif
    rxPnDtaPtr = &chRTPtr->rxPduData[pnInfoPtr->UdpNmPnInfoOffset];

    /* Handle ERA and EIRA resetTimer */
    for (index = 0u; index < (pnInfoPtr->UdpNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            rxPnInfoDta = rxPnDtaPtr[index / 8u];

            if (0u != (rxPnInfoDta & bitMask))
            {
#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
                if (chCfgPtr->PnEraCalcEnabled)
                {
                    /* @req SWS_UdpNm_00359 */
                    /*
                     * A PN is requested by reception the monitoring for
                     * this PN shall be restarted.
                     */
                    chRTPtr->pnEraResetTimer[pnIdx] = UdpNm_CfgPtr->PnResetTime / chCfgPtr->MainFunctionPeriod;
                }
#endif

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
                /* @req SWS_UdpNm_00349 */
                pnEiraResetTimerPtr[pnIdx] =
                    UdpNm_CfgPtr->PnResetTime
                    / UdpNm_CfgPtr->ChannelConfig[UDPNM_EIRA_HANDLE_CHANNEL].MainFunctionPeriod;
#endif
            }
            pnIdx++;
        }
    }

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    if (chCfgPtr->PnEraCalcEnabled)
    {
        for (index = 0u; index < pnInfoPtr->UdpNmPnInfoLength; index++)
        {
            filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
            find0To1 = rxPnDtaPtr[index] & filterMaskData;
            find0To1 = find0To1 & (~chRTPtr->pnInfoEra[index]);

            if (find0To1 > 0u)
            {
                /* Find rx pdu pninfo bit 0--->1 */
                rxIndEraFlag = TRUE;
                break;
            }
        }
    }
#endif

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    for (index = 0u; index < pnInfoPtr->UdpNmPnInfoLength; index++)
    {
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
        find0To1 = rxPnDtaPtr[index] & filterMaskData;
        find0To1 = find0To1 & (~pnEiraInfoPtr[index]);

        if (find0To1 > 0u)
        {
            /* Find rx pdu pninfo bit 0--->1 */
            rxIndEiraFlag = TRUE;
            break;
        }
    }
#endif

    /* @req SWS_UdpNm_00347 @req SWS_UdpNm_00357 */
    /*
     * PNs are requested within received message, UdpNm shall store
     * the request information
     */
    for (index = 0u; index < pnInfoPtr->UdpNmPnInfoLength; index++)
    {
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
        uint8 relatedPnData = rxPnDtaPtr[index] & filterMaskData;

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
        if (chCfgPtr->PnEraCalcEnabled)
        {
            chRTPtr->pnInfoEra[index] |= relatedPnData;
        }
#endif

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
        pnEiraInfoPtr[index] |= relatedPnData;
#endif
    }
    SchM_Exit_UdpNm_Context();
    /* @req SWS_UdpNm_00352,@req SWS_UdpNm_00361 */
    /*
     * If the stored value for a PN is set to requested,UdpNm
     * shall inform upper layers.
     */
    PduIdType upRxPduId;
    PduInfoType PduInfo;
#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    if (rxIndEraFlag)
    {
        upRxPduId = chCfgPtr->PnEraRxNSduRef;
        PduInfo.SduLength = pnInfoPtr->UdpNmPnInfoLength;
        PduInfo.SduDataPtr = chRTPtr->pnInfoEra;

        PduR_UdpNmRxIndication(upRxPduId, &PduInfo);
    }
#endif

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    if (rxIndEiraFlag)
    {
        upRxPduId = UdpNm_CfgPtr->UdpNmPnEiraRxNSduRef;
        PduInfo.SduLength = pnInfoPtr->UdpNmPnInfoLength;
        PduInfo.SduDataPtr = pnEiraInfoPtr;

        PduR_UdpNmRxIndication(upRxPduId, &PduInfo);
    }
#endif
}
#endif

#if (STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
/******************************************************************************/
/*
 * Brief               Handle ERA reset timer,when timer expired,notify PduR
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_PnEraResetTimerHandle(uint8 chIndex)
{
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    const UdpNm_PnInfoType* pnInfoPtr = UdpNm_CfgPtr->PnInfo;
    uint8 bitMask;
    boolean eraIndFlag = FALSE;
    uint8 index;
    uint8 filterMaskData;
    uint8 pnIdx = 0u;

    for (index = 0u; index < (pnInfoPtr->UdpNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            if (chRTPtr->pnEraResetTimer[pnIdx] > 0u)
            {
                chRTPtr->pnEraResetTimer[pnIdx]--;
                if (0u == chRTPtr->pnEraResetTimer[pnIdx])
                {
                    /* @req SWS_UdpNm_00360 */
                    /*
                     * PN is not requested again within UdpNmPnResetTime
                     * then the corresponding stored value for this PN
                     * shall be set to not requested (value 0).
                     */
                    chRTPtr->pnInfoEra[index / 8u] &= (~bitMask);
                    eraIndFlag = TRUE;
                }
            }

            pnIdx++;
        }
    }

    /* @req SWS_UdpNm_00361 */
    if (eraIndFlag)
    {
        PduIdType upRxPduId = chCfgPtr->PnEraRxNSduRef;
        PduInfoType PduInfoPtr;
        PduInfoPtr.SduLength = pnInfoPtr->UdpNmPnInfoLength;
        PduInfoPtr.SduDataPtr = chRTPtr->pnInfoEra;

        PduR_UdpNmRxIndication(upRxPduId, &PduInfoPtr);
    }
}
#endif /*STD_ON == UDPNM_PN_ERA_CALC_ENABLED*/

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
/******************************************************************************/
/*
 * Brief               Handle EIRA reset timer,when timer expired,notify PduR
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_PnEiraResetTimerHandle(void)
{
    const UdpNm_PnInfoType* pnInfoPtr = UdpNm_CfgPtr->PnInfo;
    uint8* pnEiraInfoPtr = UdpNm_InnerGlobal.pnEiraInfo;
    uint16* pnEiraResetTimerPtr = UdpNm_InnerGlobal.pnEiraResetTimer;
    uint8 bitMask;
    boolean eiraIndFlag = FALSE;
    uint8 index;
    uint8 filterMaskData;
    uint8 pnIdx = 0u;

    for (index = 0u; index < (pnInfoPtr->UdpNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            if (pnEiraResetTimerPtr[pnIdx] > 0u)
            {
                pnEiraResetTimerPtr[pnIdx]--;
                if (0u == pnEiraResetTimerPtr[pnIdx])
                {
                    /* @req SWS_UdpNm_00351 */
                    /*
                     * PN is not requested again within UdpNmPnResetTime
                     * then the corresponding stored value for this PN
                     * shall be set to not requested (value 0).
                     */
                    pnEiraInfoPtr[index / 8u] &= (~bitMask);
                    eiraIndFlag = TRUE;
                }
            }

            pnIdx++;
        }
    }

    /* @req SWS_UdpNm_00352 */
    if (eiraIndFlag)
    {
        PduIdType upRxPduId = UdpNm_CfgPtr->UdpNmPnEiraRxNSduRef;
        PduInfoType PduInfoPtr;
        PduInfoPtr.SduLength = pnInfoPtr->UdpNmPnInfoLength;
        PduInfoPtr.SduDataPtr = pnEiraInfoPtr;

        PduR_UdpNmRxIndication(upRxPduId, &PduInfoPtr);
    }
}
#endif

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Transmit a NM frame
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 *                     addTransFlag, Additional transmission flag,
 *                     (TRUE,means not need to get user data from PduR)
 *                     (FALSE,means need to get user data from PduR)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK, Send NM pdu success
 *                     E_NOT_OK, Send NM pdu fail
 */
/******************************************************************************/
static FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SendNmPdu(uint8 chIndex)
{
    Std_ReturnType ret;
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
    if (!chRTPtr->udpNmTxEnable)
    {
        ret = E_NOT_OK;
    }
    else
#endif
    {
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];

#if UDPNM_PN_EIRA_CALC_ENABLED == STD_ON
        if (chCfgPtr->PnEnabled)
        {
            /* @req SWS_UdpNm_00348 @req SWS_UdpNm_00345 */
            UdpNm_TxPnEiraHandle(chCfgPtr->ComMNetworkHandleRef, chRTPtr->txPduData);
        }
#endif
/* @req SWS_UdpNm_00317 */
#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT) && (STD_OFF == UDPNM_IS_TRIGGER_TRANSMIT_IN_SOAD)
        if (chCfgPtr->UserDataTxPdu != NULL_PTR)
        {
            PduInfoType pduInfo;
            uint8 userData[UDPNM_USER_DATA_MAX_LENGTH] = {0};
            PduIdType pdurTxSduId = chCfgPtr->UserDataTxPdu->UpperLayerPduId;
            pduInfo.SduLength = chCfgPtr->UserDataLength;
            pduInfo.SduDataPtr = userData;
            if (PduR_UdpNmTriggerTransmit(pdurTxSduId, &pduInfo) == E_OK)
            {
                SchM_Enter_UdpNm_Context();
                (void)ILib_memcpy(&chRTPtr->txPduData[chCfgPtr->UserDataOffset], userData, chCfgPtr->UserDataLength);
                SchM_Exit_UdpNm_Context();
            }
        }
#endif

#if (0u < UDPNM_TX_PDU_NUMBER)
        PduInfoType pduInfo;
        PduIdType SoAdIfTxSduId = chCfgPtr->TxPdu->LowerLayerPduId;
        pduInfo.SduDataPtr = chRTPtr->txPduData;
        pduInfo.SduLength = chCfgPtr->UdpNmPduLength;

        /* @req SWS_UdpNm_00032 */
        ret = SoAd_IfTransmit(SoAdIfTxSduId, &pduInfo);
        if (E_OK == ret)
        {
            SchM_Enter_UdpNm_Context();
            chRTPtr->msgTimeoutTimer = chCfgPtr->MsgTimeoutTime;
            chRTPtr->msgToutFlg = FALSE;
            SchM_Exit_UdpNm_Context();
        }
#endif /* 0u < UDPNM_TX_PDU_NUMBER_MAX */
    }

    return ret;
}
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if UDPNM_PN_EIRA_CALC_ENABLED == STD_ON
static void UdpNm_TxPnEiraHandle(NetworkHandleType networkHandle, uint8* txDta)
{
    P2CONST(UdpNm_PnInfoType, AUTOMATIC, UDPNM_APPL_CONST)
    pnInfoPtr = UdpNm_CfgPtr->PnInfo;
    uint8* pnEiraInfoPtr = UdpNm_InnerGlobal.pnEiraInfo;
    uint16* pnEiraResetTimerPtr = UdpNm_InnerGlobal.pnEiraResetTimer;
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
    chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[0];
    boolean rxIndEiraFlag = FALSE;
    uint8 bitMask;
    uint8 txPnInfoDta, localPnInfoDta;
    uint8 index;
    uint8 pnIdx = 0u;
    uint8 filterMaskData;
    uint8 relatedPnData;

    SchM_Enter_UdpNm_Context();
    uint8* txPnDta = &(txDta[pnInfoPtr->UdpNmPnInfoOffset]);
    Nm_PncBitVectorTxIndication(networkHandle, txPnDta);

    for (index = 0u; index < (pnInfoPtr->UdpNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            txPnInfoDta = txPnDta[index / 8u];
            localPnInfoDta = pnEiraInfoPtr[index / 8u];

            if ((txPnInfoDta & bitMask) > 0u)
            {
                /* @req SWS_UdpNm_00349 */
                pnEiraResetTimerPtr[pnIdx] = UdpNm_CfgPtr->PnResetTime / chCfgPtr->MainFunctionPeriod;

                if ((FALSE == rxIndEiraFlag) && ((localPnInfoDta & bitMask) == 0u))
                {
                    rxIndEiraFlag = TRUE;
                }
            }

            pnIdx++;
        }
    }

    for (index = 0u; index < pnInfoPtr->UdpNmPnInfoLength; index++)
    {
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
        relatedPnData = txPnDta[index] & filterMaskData;
        if (relatedPnData > 0u)
        {
            pnEiraInfoPtr[index] |= relatedPnData;
        }
    }
    SchM_Exit_UdpNm_Context();

    /* @req SWS_UdpNm_00352 */
    if (rxIndEiraFlag)
    {
        PduIdType upRxPduId = UdpNm_CfgPtr->UdpNmPnEiraRxNSduRef;
        PduInfoType PduInfoPtr;
        PduInfoPtr.SduLength = pnInfoPtr->UdpNmPnInfoLength;
        PduInfoPtr.SduDataPtr = pnEiraInfoPtr;
        PduR_UdpNmRxIndication(upRxPduId, &PduInfoPtr);
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition of UdpNm
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_StateManagement(uint8 chIndex)
{
    const UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

    switch (chRTPtr->udpNmState)
    {
    case NM_STATE_BUS_SLEEP:
        UdpNm_BusSleepStateHandle(chIndex);
        break;

    case NM_STATE_PREPARE_BUS_SLEEP:
        UdpNm_PrepareBusSleepStateHandle(chIndex);
        break;

    case NM_STATE_REPEAT_MESSAGE:
        UdpNm_RepeatMessageStateHandle(chIndex);
        break;

    case NM_STATE_NORMAL_OPERATION:
        UdpNm_NormalOperationStateHandle(chIndex);
        break;

    case NM_STATE_READY_SLEEP:
        UdpNm_ReadySleepStateHandle(chIndex);
        break;

    default:
        /* Other state */
        break;
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bus Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_BusSleepStateHandle(uint8 chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

    /* @req SWS_UdpNm_00127 */
    if (chRTPtr->rxPduExtFlg)
    {
        Nm_NetworkStartIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }

    if (UdpNm_TestAndClear(&chRTPtr->passiveStartUp))
    {
        /* @req SWS_UdpNm_00128 */
        /* Entry repeat message Mode */
        UdpNm_EntryRepeatMessageState(chIndex, FALSE);
    }

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Network Request */
    if (UdpNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].ActiveWakeupBitEnabled)
        {
            /* @req SWS_UdpNm_00366 */
            /* Set ActiveWakeupBit in CBV */
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }

        /* @req SWS_UdpNm_00129 */
        /*
         * Network is requested in Bus-Sleep Mode, the UdpNm module shall
         * enter Repeat Message State.
         */
        UdpNm_EntryRepeatMessageState(chIndex, TRUE);
    }
#endif /* UDPNM_PASSIVE_MODE_ENABLED == STD_OFF */
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Prepare Bus Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_PrepareBusSleepStateHandle(uint8 chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
    if (UdpNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
        const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
        if (chCfgPtr->ActiveWakeupBitEnabled)
        {
            /* @req SWS_UdpNm_00366 */
            /* Set ActiveWakeupBit in CBV */
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }

        /* @req SWS_UdpNm_00123 */
        /* network  requset,will be entry Repeat Message state */
        UdpNm_EntryRepeatMessageState(chIndex, TRUE);
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }
#endif /* UDPNM_PASSIVE_MODE_ENABLED == STD_OFF */

    /* @req SWS_UdpNm_00128 SWS_UdpNm_00124 */
    if (UdpNm_TestAndClear(&chRTPtr->passiveStartUp) || chRTPtr->rxPduExtFlg)
    {
        UdpNm_EntryRepeatMessageState(chIndex, FALSE);
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }

    if (chRTPtr->stateToutFlg)
    {
        /* @req SWS_UdpNm_00115 */
        /* State timer timeout,enter bus-sleep mode */
        UdpNm_StateChange(chIndex, NM_STATE_BUS_SLEEP);
        chRTPtr->udpnmMode = NM_MODE_BUS_SLEEP;

        /* @SWS_UdpNm_00126 @req SWS_UdpNm_00093 */
        Nm_BusSleepMode(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }

exit:; /* do notiong */
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Repeat Message State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_RepeatMessageStateHandle(uint8 chIndex)
{
#if (                                                                                \
    ((STD_OFF == UDPNM_PASSIVE_MODE_ENABLED) && (STD_ON == UDPNM_GLOBAL_PN_ENABLED)) \
    || (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED))
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
    chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
#endif

    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    if (UdpNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
        /* @req SWS_UdpNm_00362 */
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            UdpNm_EntryRepeatMessageState(chIndex, TRUE);
            goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
        }
#endif /*STD_OFF == UDPNM_GLOBAL_PN_ENABLED*/
    }
#endif

    /* @req SWS_UdpNm_00102 */
    if (chRTPtr->stateToutFlg)
    {
#if (STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED)
        chRTPtr->immedMsgTxCnt = 0u;
#endif /* STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED */
        if (UDPNM_NETWORK_REQUESTED == chRTPtr->netRequestStatus)
        {
            /* @req SWS_UdpNm_00103 */
            /*
             * If the network has been requested,normal opertation state
             * shall be entered.
             */
            UdpNm_StateChange(chIndex, NM_STATE_NORMAL_OPERATION);

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_UdpNm_00150 */
            /* Enter NORMAL OPERATION,reset the RemoteSleepIndTimer */
            chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;
#endif /*STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED*/
        }
        else
        {
            /* @req SWS_UdpNm_00106 */
            /*
             * If the network has been released,Ready Sleep State
             * shall be entered
             */
            UdpNm_StateChange(chIndex, NM_STATE_READY_SLEEP);

            /* @req SWS_UdpNm_00108 @req SWS_UdpNm_00051 */
            /*
             * When the Ready Sleep State is entered from Repeat Message
             * State,transmission of NM PDUs shall be stopped.
             */
            chRTPtr->msgTxCylceTimer = 0u;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
        }

/* @req SWS_UdpNm_00107 */
/*
 * UdpNm shall clear the Repeat Message Bit when leaving
 * Repeat Message State.
 */
#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
        if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled)
        {
            UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }
#endif
    }

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) && (UDPNM_GLOBAL_PN_ENABLED == STD_ON)
exit:; /* do notiong */
#endif
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Normal Operation State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_NormalOperationStateHandle(uint8 chIndex)
{
#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED) || (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
#endif
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

    if (UdpNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
        /* @req SWS_UdpNm_00362 */
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            UdpNm_EntryRepeatMessageState(chIndex, TRUE);
            goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
        }
#endif /* UDPNM_GLOBAL_PN_ENABLED == STD_ON */
    }

    /* @req SWS_UdpNm_00118 */
    /* Left normal operation state,enter ready sleep state*/
    if (UDPNM_NETWORK_RELEASED == chRTPtr->netRequestStatus)
    {
        UdpNm_StateChange(chIndex, NM_STATE_READY_SLEEP);

        /* @req SWS_UdpNm_00108 @req SWS_UdpNm_00051 */
        /*
         * When the Ready Sleep State is entered from Normal Operation
         * State,transmission of NM PDUs shall be stopped.
         */
        chRTPtr->msgTxCylceTimer = 0u;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
        chRTPtr->retrySendMsgFlag = FALSE;
#endif
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }

#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
    if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled)
    {
        if (chRTPtr->repeatMessageRequest)
        {
            /* @req SWS_UdpNm_00120 */
            UdpNm_EntryRepeatMessageState(chIndex, FALSE);
            /* @req SWS_UdpNm_00121 */
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }
        else if (chRTPtr->rxPduExtFlg && (UDPNM_PDU_OFF != chCfgPtr->PduCbvPosition))
        {
            uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
            uint8 cbvDta = chRTPtr->rxPduData[cbvPos];
            /* @req SWS_UdpNm_00119 */
            /*
             * If UdpNmNodeDetectionEnabled is set to TRUE and Repeat
             * Message Request bit is received in Normal Operation State,
             * UdpNm shall enter Repeat Message State.
             */
            if ((cbvDta & UDPNM_CBV_BIT_RMP_MASK) > 0u)
            {
                UdpNm_EntryRepeatMessageState(chIndex, FALSE);
            }
        }
        else
        {
            /* Nothing to do here */
        }
    }
#endif /* STD_ON == UDPNM_NODE_DETECTION_ENABLED */

exit:; /* do nothing */
#endif
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Ready Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_ReadySleepStateHandle(uint8 chIndex)
{
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
    chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    boolean stateChanged = FALSE;

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    if (UdpNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            /* @req SWS_UdpNm_00362 */
            UdpNm_EntryRepeatMessageState(chIndex, TRUE);
        }
        else
#endif
        {
            /* @SWS_UdpNm_00110
             network is request ,will entry normal operation mode */
            UdpNm_StateChange(chIndex, NM_STATE_NORMAL_OPERATION);

/* @req SWS_UdpNm_00116 @req SWS_UdpNm_00006 */
/*
 * When the Normal Operation State is entered from Ready Sleep
 * State, transmission of NM PDUs shall be started immediately
 * unless passive mode is enabled or the NM message transmission
 * ability has been disabled.
 */
#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
            if (chRTPtr->udpNmTxEnable)
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */
            {
                chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleTime;
                chRTPtr->sendNmMsgFlg = TRUE;
            }

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_UdpNm_00150 */
            /* Enter NORMAL OPERATION,reset the remoteTimer */
            chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED*/
        }
        stateChanged = TRUE;
    }

#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
    if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled && chRTPtr->repeatMessageRequest && !stateChanged)
    {
        /* @req SWS_UdpNm_00112 */
        UdpNm_EntryRepeatMessageState(chIndex, FALSE);
        /* @req SWS_UdpNm_00113 */
        UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */

        stateChanged = TRUE;
    }
#endif /* STD_ON == UDPNM_NODE_DETECTION_ENABLED */
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

#if (STD_ON == UDPNM_NODE_DETECTION_ENABLED)
    if (UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled && chRTPtr->rxPduExtFlg
        && (UDPNM_PDU_OFF != chCfgPtr->PduCbvPosition) && !stateChanged)
    {
        uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
        uint8 cbvDta = chRTPtr->rxPduData[cbvPos];
        /* @req SWS_UdpNm_00111 */
        /*
         * If UdpNmNodeDetectionEnabled is set to TRUE and Repeat
         * Message Request bit is received in the Ready Sleep State,
         * UdpNm shall enter Repeat Message State.
         */
        if ((cbvDta & UDPNM_CBV_BIT_RMP_MASK) > 0u)
        {
            UdpNm_EntryRepeatMessageState(chIndex, FALSE);
        }
    }
#endif /* STD_ON == UDPNM_NODE_DETECTION_ENABLED */

    if (chRTPtr->nmToutFlg && !stateChanged)
    {
        UdpNm_EnterBusSleepModeHandle(chIndex);
    }
}

/******************************************************************************/
/*
 * Brief               Asynchronous process received data in MainFunction
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_RxDataMainHandle(uint8 chIndex)
{
    P2CONST(UdpNm_ChannelConfigType, AUTOMATIC, UDPNM_APPL_CONST)
    chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];

/* Detection of Remote Sleep Indication Handle */
#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
    /* @req SWS_UdpNm_00150 */
    /* Reset RemoteSleepInd timer */
    chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;

    /* @req SWS_UdpNm_00151*/
    /*
     * If an NM PDU is received in the Normal Operation State or Ready Sleep
     * State,shall notify the NM that some nodes in the cluster are not ready
     * to sleep anymore.
     */
    if (chRTPtr->remoteSleepInd
        && ((NM_STATE_NORMAL_OPERATION == chRTPtr->udpNmState) || (NM_STATE_READY_SLEEP == chRTPtr->udpNmState)))
    {
        chRTPtr->remoteSleepInd = FALSE;
        Nm_RemoteSleepCancellation(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

    /* @req SWS_UdpNm_00098 */
    /*
     * Successful reception of an NM PDU in Network Mode, the
     * NM-Timeout Timer shall be restarted.
     */
    if (NM_MODE_NETWORK == chRTPtr->udpnmMode)
    {
        chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;
    }

#if ((STD_ON == UDPNM_REPEAT_MSG_IND_ENABLED) && (STD_ON == UDPNM_NODE_DETECTION_ENABLED))
    /* @req SWS_UdpNm_00014 */
    /*
     * If UdpNmNodeDetectionEnabled and UdpNmRepeatMsgIndEnabled are set
     * to TRUE and Repeat Message Request bit is received, UdpNm module
     * shall call the callback function Nm_RepeatMessageIndication.
     */
    if ((UDPNM_PDU_OFF != chCfgPtr->PduCbvPosition) && UdpNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled
        && UdpNm_CfgPtr->PnInfo(chIndex))
    {
        uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
        uint8 cbvDta = chRTPtr->rxPduData[cbvPos];

        if ((cbvDta & UDPNM_CBV_BIT_RMP_MASK) > 0u)
        {
            Nm_RepeatMessageIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == UDPNM_REPEAT_MSG_IND_ENABLED && \
          STD_ON == UDPNM_NODE_DETECTION_ENABLED */

#if (STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED)
#if !defined(UDPNM_VER_4_2_2)
    if (UdpNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpRxEnabled)
#endif
    {
        UdpNm_RxCarWakeUpHadle(chIndex);
    }
#endif /* STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
    if (UDPNM_PDU_OFF != chCfgPtr->PduCbvPosition)
    {
        /* NM Coordinator synchronization support */
        UdpNm_CoordReadyToSleepIndHandle(chIndex);
    }
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

#if (                                   \
    (STD_ON == UDPNM_GLOBAL_PN_ENABLED) \
    && ((STD_ON == UDPNM_PN_ERA_CALC_ENABLED) || (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)))
    if (chCfgPtr->PnEnabled && chRTPtr->pnRxIndFlg)
    {
        /* @req SWS_UdpNm_00345 @req SWS_UdpNm_00358 */
        UdpNm_RxPnEiraEraHandle(chIndex);
    }
#endif
}

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
/******************************************************************************/
/*
 * Brief               Send message in MainFunction.The purpose of this function
 *                     is to aggregate multiple send requests,Only one frame
 *                     will be sent out when there are multiple send requests in
 *                     a MainFunction.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_SendMsgMainHandle(uint8 chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];

    if (chRTPtr->sendNmMsgFlg)
    {
        chRTPtr->sendNmMsgFlg = FALSE;
        if (UdpNm_SendNmPdu(chIndex) == E_OK)
        {
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->immedMsgTxCnt > 0u)
            {
                chRTPtr->immedMsgTxCnt--;
                if (chRTPtr->immedMsgTxCnt == 0u)
                {
                    chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleTime;
                }
                else
                {
                    chRTPtr->msgTxCylceTimer = chCfgPtr->ImmediateNmCycleTime;
                }
            }
            else
#endif
            {
                chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleTime;
            }
        }
        else
        {
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->immedMsgTxCnt > 0u)
            {
                chRTPtr->msgTxCylceTimer = 1u;
            }
            else
#endif
            {
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
                if (chRTPtr->retrySendMsgFlag)
                {
                    chRTPtr->msgTxCylceTimer = 1u;
                }
                else
#endif
                {
                    chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleTime;
                }
            }
        }
    }
}
#endif /* UDPNM_PASSIVE_MODE_ENABLED == STD_OFF */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               NM Coordinator synchronization support handle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_CoordReadyToSleepIndHandle(uint8 chIndex)
{
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(UdpNm_InnerChannelType, AUTOMATIC, UDPNM_APPL_DATA)
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    chRTPtr = &UdpNm_ChRunTime[chIndex];
    uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
    uint8 cbvDta = chRTPtr->rxPduData[cbvPos];

    if (0u == (cbvDta & UDPNM_CBV_BIT_CSR_MASK))
    {
        /* @req SWS_UdpNm_00320 */
        /* First reception csr=0 ,shall notify upper layer */
        if ((NM_MODE_NETWORK == chRTPtr->udpnmMode) && (UDPNM_CSR_IND_INDICATED == chRTPtr->coordReadyToSleepInd))
        {
            chRTPtr->coordReadyToSleepInd = UDPNM_CSR_IND_CANCELLED;
            /* notify upper layer */
            Nm_CoordReadyToSleepCancellation(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
    else
    {
        /* @req SWS_UdpNm_00364 */
        /* First reception csr=1,shall notify upper layer */
        if (((NM_MODE_NETWORK == chRTPtr->udpnmMode) && (UDPNM_CSR_IND_INIT == chRTPtr->coordReadyToSleepInd))
            || (UDPNM_CSR_IND_CANCELLED == chRTPtr->coordReadyToSleepInd))
        {
            chRTPtr->coordReadyToSleepInd = UDPNM_CSR_IND_INDICATED;
            /* notify upper layer */
            Nm_CoordReadyToSleepIndication(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

/******************************************************************************/
/*
 * Brief               Processes when enter Bus-Sleep Mode from other state
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, UDPNM_CODE) UdpNm_EnterBusSleepModeHandle(uint8 chIndex)
{
    const UdpNm_ChannelConfigType* chCfgPtr = &UdpNm_CfgPtr->ChannelConfig[chIndex];
    SchM_Enter_UdpNm_Context();
    UdpNm_InnerChannelType* chRTPtr = &UdpNm_ChRunTime[chIndex];
    /* @req SWS_UdpNm_00109 */
    /* NM-timeout timer expires,shall entry the Prepare Bus-sleep mode*/
    chRTPtr->udpnmMode = NM_MODE_PREPARE_BUS_SLEEP;
    SchM_Exit_UdpNm_Context();
    UdpNm_StateChange(chIndex, NM_STATE_PREPARE_BUS_SLEEP);

    /* @req SWS_UdpNm_00114 @req SWS_UdpNm_00093 */
    Nm_PrepareBusSleepMode(UdpNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);

    /* @req SWS_UdpNm_00115 */
    /*
     * If UdpNmStayInPbsEnabled is disabled UdpNm shall stay in the
     * Prepare Bus-Sleep Mode for a configurable amount of time
     * determined by the UdpNmWaitBusSleepTime.
     */
    if (FALSE == chCfgPtr->StayInPbsEnabled)
    {
        SchM_Enter_UdpNm_Context();
        chRTPtr->stateTimer = chCfgPtr->WaitBusSleepTime;
        SchM_Exit_UdpNm_Context();
    }

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    /* @req SWS_UdpNm_00367 */
    /* Clear activeWakeupbit in CBV */
    if (chCfgPtr->ActiveWakeupBitEnabled)
    {
        UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
    }
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */
}

#if UDPNM_DEV_ERROR_DETECT == STD_ON
static boolean UdpNm_ValidateInitStatus(uint8 apiId)
{
    boolean valid = UdpNm_InitStatus == UDPNM_INIT;
    if (!valid)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_UNINIT);
    }
    return valid;
}

static boolean UdpNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle)
{
    boolean valid = UdpNm_ValidateInitStatus(apiId);
    if (valid)
    {
        if (UdpNm_FindChannelIndex(networkHandle) >= UDPNM_NUMBER_OF_CHANNEL)
        {
            valid = FALSE;
            (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_CHANNEL);
        }
    }

#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
    if (valid)
    {
        if (GetApplicationID() != ComM_GetChannelApplicationID(networkHandle))
        {
            valid = FALSE;
            (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_PARTITION);
        }
    }
#endif
    return valid;
}

static boolean UdpNm_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = pointer != NULL_PTR;
    if (!valid)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_PARAM_POINTER);
    }
    return valid;
}

static boolean UdpNm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer)
{
    return UdpNm_ValidateNetworkHandle(apiId, networkHandle) && UdpNm_ValidatePointer(apiId, pointer);
}

static boolean UdpNm_ValidateNetworkHandlePointerAndPointer(
    uint8 apiId,
    NetworkHandleType networkHandle,
    const void* pointer1,
    const void* pointer2)
{
    return UdpNm_ValidateNetworkHandleAndPointer(apiId, networkHandle, pointer1)
           && UdpNm_ValidatePointer(apiId, pointer2);
}

static boolean UdpNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId)
{
    boolean valid = UdpNm_ValidateInitStatus(apiId);
    uint8 pduNum = 0u;

    switch (apiId)
    {
    case UDPNM_SERVICE_ID_TRANSMIT:
        pduNum = UDPNM_USERDATA_TX_PDU_NUM;
        break;
    case UDPNM_SERVICE_ID_IFTXCONFIRMATION:
    case UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT:
        pduNum = UDPNM_TX_PDU_NUMBER;
        break;

    default:
        /* Unreachable */
        break;
    }

    if (valid)
    {
        if (txPduId > pduNum)
        {
            valid = FALSE;
            (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_PDUID);
        }
    }
    return valid;
}

static boolean UdpNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr)
{
    return UdpNm_ValidateTxPduId(apiId, txPduId) && UdpNm_ValidatePointer(apiId, pduInfoPtr)
           && UdpNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
}

static boolean UdpNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean valid = UdpNm_ValidateInitStatus(apiId) && UdpNm_ValidatePointer(apiId, pduInfoPtr)
                    && UdpNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
    if (valid)
    {
        if (rxPduId >= UDPNM_RX_PDU_NUMBER)
        {
            valid = FALSE;
            (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_PDUID);
        }
    }
    return valid;
}

static boolean UdpNm_ValidateInit(const UdpNm_ConfigType* configPtr)
{
    boolean valid = UdpNm_InitStatus != UDPNM_INIT;
    if (!valid)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, UDPNM_SERVICE_ID_INIT, UDPNM_E_ALREADY_INITIALIZED);
    }
    else
    {
        valid = UdpNm_ValidatePointer(UDPNM_SERVICE_ID_INIT, configPtr);
    }
    return valid;
}

#endif

#define UDPNM_STOP_SEC_CODE
#include "UdpNm_MemMap.h"
