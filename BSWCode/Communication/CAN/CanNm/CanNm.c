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
**  FILENAME    : CanNm.c                                                     **
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
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V1.0.0    20190926  darren.zhang   Initial version(ff version 1.16)
 *  V1.0.1    20200311  lili.wang
 *      1.EIRA and ERA buffer initialization modification in CanNm_Init
 *      (simplified initialization complexity)
 *      2.CanNm_GetLocalNodeIdentifier compile abstract problem problem modification
 *      3.Solve the problem of closing the communication first and then releasing the
 *      network, resulting in the inability to release the network for hibernation
 *  V1.0.2    20200518  lili.wang
 *      1.in CanNm_TimerManagement modify the logic of Rp or No state NmTimer timeout
 *      error report to DET
 *  V1.0.3    20200615  lili.wang
 *      1.Compile abstract update
 *      2.Tool generates configuration structure declaration modification
 *  V2.0.0    2020-07-29  lili.wang    R19_11 initial version.
 *  V2.0.1    2020-10-19  lili.wang    QAC check,code update
 *  V2.0.2    2020-11-30  lili.wang    1.PduInfoType MetaDataPtr set to NULL_PTR
 *                                     2.Cantata detection modification
 *  V2.0.3    2020-12-11  lili.wang    cantata check,code update
 *  V2.0.4    2021-03-10  lili.wang
 *      1.UnitTest bug update,cannm immediate transmit and Pnresetime bug update
 *      2.4.2.2&R19-11 define switch control modify
 *      3.CanNmUserDataEnabled and CanNmComUserDataSupport switch control modify
 *  V2.0.5    2021-04-23  lili.wang
 *      1.Modify compilation errors
 *  V2.0.6    2021-04-26  lili.wang
 *      1. Solve the problem that data cannot be transmitted when UdpNmMsgCycleOffset
 *      is 0 and the immediate transmission is disabled.
 *  V2.0.7    2021-06-2  lili.wang
 *      1. when UDPNM_IMMEDIATE_RESTART_ENABLED is STD_ON,UDPNM_IMMEDIATE_TRANSMIT_ENABLED
 *      STD_OFF,modify compilation error
 *  V2.0.8    2021-06-9  lili.wang
 *      1. delete in ready sleep state call CanNm_NetworkRequest() switch to Normal Operation
 *      State.
 *      2. delete in CanNm_EnableCommunication clear immedMsgTxCnt.
 *      3. modify UserDataTxPdu macro switch
 *  V2.0.9    2021-08-5  lili.wang
 *      1.CanNm_RxPnFilter compilation error modification.
 *  V2.0.10    2021-08-10  lili.wang
 *      1.Bug modification of Nm coordination function.
 *      2.In ready sleep state,switch to Normal operation state when network request.
 *  V2.0.11    2021-08-16  lili.wang
 *      1.The state switching after the network is released is an asynchronous
 *      operation, and it is should judged whether the network is released or in
 *      the repeat msg state at the position where the message is sent.
 *  V2.0.12    2022-04-24  darren.zhang
 *      1.Adjust the header file inclusion order
 *  V2.0.13    2022-05-05  darren.zhang
 *      1.resolve build warnings
 *      2.ImmediateTxConf logic modified
 *  V2.0.14    2022-05-17  darren.zhang
 *      1.allow PduR_CanNmTriggerTransmit return E_NOT_OK transmit nm pdu
 *  V2.0.15    2022-12-16  darren.zhang
 *      1. CanNm_RxIndication Increases DET check of the length of the received pdu
 *  V2.0.16    2023-02-10  darren.zhang
 *      1. Solve the occasional frame loss problem
 *  V2.0.17    2023-06-12  xiaojian.liang
 *      Fixed issue CPT-644 CanNm_SetUserData null pointer exception
 *  V2.0.18   2023-06-30   darren.zhang
 *      modify network release handle
 *  V2.0.19   2023-07-24   xiaojian.liang
 *      Fixed CPT-6156 Nm_CarWakeUpIndication cannot be called when CanNmCarWakeUpFilterEnabled is FALSE
 *  V2.0.20   2023-09-25   xiaojian.liang
 *      Fixed CPT-6562 Compile failed when CanNmPassiveModeEnabled is TRUE
 *  V2.0.21   2023-10-16   xiaojian.liang
 *      1. Let API CanNm_GetPduData, CanNm_RepeatMessageRequest, CanNm_GetNodeIdentifier and
 *         CanNm_GetLocalNodeIdentifier always compiled
 *      2. Fixed CPT-7329 - Fixed missing network nequest or passive startup events
 *  V2.0.22   2023-11-20   xiaojian.liang
 *      Performance optimization.
 *      Fixed CPT-6562 Preprocessing could not be performed because the nonexistent macro CANNM_PN_ENABLED was used
 *      Fixed CPT-8002 CanNmNodeIdEnabled, CanNmNodeDetectionEnabled, CanNmRepeatMsgIndEnabled related features review
 *  V2.0.23   2024-01-31   xiaojian.liang
 *      CPT-8061 Source NID is always zero if CanNmNodeIdEnabled is true but CanNmNodeDetectionEnabled is false
 *      Replace the std library with the istd library
 *      CPD-33700 Multiple partition support
 *  V2.0.24   2024-10-30   xiaojian.liang
 *      Fixed CPT-10964 If CanNmComUserDataSupport is enabled the API CanNm_SetUserData shall not be available.
 *  V2.0.25   2024-11-27   caihong.liu
 *      1. Fixed CPT-10987 - Fixed CanNm_Transmit precompilation condition and PduId comparison
 *      2. Fixed CPT-11294 - Fixed The PduR_CanNmRxIndication function is called within Critical Section
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
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
#define CANNM_C_AR_RELEASE_MAJOR_VERSION 4u
#define CANNM_C_AR_RELEASE_MINOR_VERSION 5u
#define CANNM_C_AR_RELEASE_PATCH_VERSION 0u
#define CANNM_C_SW_MAJOR_VERSION         2u
#define CANNM_C_SW_MINOR_VERSION         0u
#define CANNM_C_SW_PATCH_VERSION         25u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanNm.h"
#include "CanNm_Internal.h"
#include "istd_lib.h"
#include "Det.h"
#include "Nm.h"
#include "SchM_CanNm.h"
#include "CanIf.h"
#include "CanSM_TxTimeoutException.h"

#if (STD_ON == CANNM_COM_USERDATA_SUPPORT) || (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
#include "PduR_CanNm.h"
#endif

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
#include "ComM.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (CANNM_C_AR_RELEASE_MAJOR_VERSION != CANNM_H_AR_RELEASE_MAJOR_VERSION)
#error "CanNm.c : Mismatch in Specification Major Version"
#endif
#if (CANNM_C_AR_RELEASE_MINOR_VERSION != CANNM_H_AR_RELEASE_MINOR_VERSION)
#error "CanNm.c : Mismatch in Specification _MINOR Version"
#endif
#if (CANNM_C_AR_RELEASE_PATCH_VERSION != CANNM_H_AR_RELEASE_PATCH_VERSION)
#error "CanNm.c : Mismatch in Specification PATCH Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANNM_UNUSED(v) (void)(v)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef uint8 CanNm_ChannelIndexType;

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
#if (STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_RxCarWakeUpHadle(uint8 chIndex);
#endif /* STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED */

static CanNm_ChannelIndexType CanNm_FindChannelIndex(NetworkHandleType nmChannelHandle);

static FUNC(void, CANNM_CODE) CanNm_StateChange(uint8 chIndex, Nm_StateType nmNewState);

static FUNC(void, CANNM_CODE) CanNm_EntryRepeatMessageState(uint8 chIndex, boolean isNetWorkRequest);

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_MsgcycleTimerManage(uint8 chIndex);
#endif /*STD_OFF == CANNM_PASSIVE_MODE_ENABLED*/

static FUNC(void, CANNM_CODE) CanNm_TimerManagement(uint8 chIndex);

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
static FUNC(boolean, CANNM_CODE) CanNm_RxPnFilter(
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) chCfgPtr,
    CanNm_InnerChannelType* chRTPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr);
#endif

#if (                                   \
    (STD_ON == CANNM_GLOBAL_PN_SUPPORT) \
    && ((STD_ON == CANNM_PN_ERA_CALC_ENABLED) || (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)))
static FUNC(void, CANNM_CODE) CanNm_RxPnEiraEraHandle(uint8 chIndex);
#endif

#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_PnEraResetTimerHandle(uint8 chIndex);
#endif /*STD_ON == CANNM_PN_ERA_CALC_ENABLED*/

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_PnEiraResetTimerHandle(void);
#endif

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
static FUNC(Std_ReturnType, CANNM_CODE) CanNm_SendNmPdu(uint8 chIndex);
#endif /*STD_OFF == CANNM_PASSIVE_MODE_ENABLED*/

#if CANNM_PN_EIRA_CALC_ENABLED == STD_ON
static void CanNm_TxPnEiraHandle(NetworkHandleType networkHandle, uint8* txDta);
#endif

static FUNC(void, CANNM_CODE) CanNm_StateManagement(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_BusSleepStateHandle(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_PrepareBusSleepStateHandle(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_RepeatMessageStateHandle(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_NormalOperationStateHandle(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_ReadySleepStateHandle(uint8 chIndex);

static FUNC(void, CANNM_CODE) CanNm_RxDataMainHandle(uint8 chIndex);

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
static FUNC(void, CANNM_CODE) CanNm_SendMsgMainHandle(uint8 chIndex);
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
static FUNC(void, CANNM_CODE) CanNm_CoordReadyToSleepIndHandle(uint8 chIndex);
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

static FUNC(void, CANNM_CODE) CanNm_EnterPrepareBusSleepModeHandle(uint8 chIndex);

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_InnerTxConfHandle(uint8 chIndex);
#endif

#if CANNM_DEV_ERROR_DETECT == STD_ON
static boolean CanNm_ValidateInitStatus(uint8 apiId);
static boolean CanNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle);
static boolean CanNm_ValidatePointer(uint8 apiId, const void* pointer);
static boolean CanNm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer);
static boolean CanNm_ValidateNetworkHandlePointerAndPointer(
    uint8 apiId,
    NetworkHandleType networkHandle,
    const void* pointer1,
    const void* pointer2);
static boolean CanNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId);
static boolean CanNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr);
static boolean CanNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
static boolean CanNm_ValidateInit(const CanNm_ConfigType* configPtr);
static boolean CanNm_ValidateDeInit(void);

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanNm_ValidatePointer
#endif

#endif
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/
#define CANNM_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanNm_MemMap.h"
static VAR(CanNm_InitStatusType, CANNM_VAR_POWER_ON_INIT) CanNm_InitStatus = CANNM_UNINIT;
#define CANNM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_INIT_PTR
#include "CanNm_MemMap.h"
static P2CONST(CanNm_ConfigType, AUTOMATIC, CANNM_APPL_CONST) CanNm_CfgPtr = NULL_PTR;
#define CANNM_STOP_SEC_VAR_INIT_PTR
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
static VAR(CanNm_InnerChannelType, CANNM_VAR) CanNm_ChRunTime[CANNM_NUMBER_OF_CHANNEL];

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT) && (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
static VAR(CanNm_InnerGlobalType, CANNM_VAR) CanNm_InnerGlobal;
#endif
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

/******************************************************************************/
/*
 * Brief               Initialize the CanNm module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanNmConfigPtr, Pointer to a selected configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_Init(const CanNm_ConfigType* CanNmConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 chIndex;
    uint8 loop;
    CanNm_InnerChannelType* chRTPtr;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateInit(CanNmConfigPtr))
#endif
    {
        /* @req SWS_CanNm_00060 */
        CanNm_CfgPtr = CanNmConfigPtr;
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
        const CanNm_PnInfoConfigType* pnInfoConfigPtr = CanNm_CfgPtr->PnInfo;
#endif
        for (chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            chRTPtr = &CanNm_ChRunTime[chIndex];

            /* @req SWS_CanNm_00141 */
            /* Set state to NM_STATE_BUS_SLEEP */
            chRTPtr->canNmState = NM_STATE_BUS_SLEEP;
            /* @req SWS_CanNm_00144 */
            /* Mode enter Bus_Sleep Mode */
            chRTPtr->cannmMode = NM_MODE_BUS_SLEEP;
            /* @req SWS_CanNm_00143 */
            /* Set network to released */
            chRTPtr->netRequestStatus = CANNM_NETWORK_RELEASED;

            /* Passive startup flag */
            chRTPtr->passiveStartUp = FALSE;

#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
            /* @req SWS_CanNm_00169*/
            chRTPtr->canNmTxEnable = TRUE;
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
            chRTPtr->coordReadyToSleepInd = CANNM_CSR_IND_INIT;
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
            chRTPtr->remoteSleepInd = FALSE;
            chRTPtr->remoteTimer = 0x0u;
            chRTPtr->remoteTimerpassed = 0x0u;
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED)
            chRTPtr->immedMsgTxCnt = 0u;
#endif /* STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED */

            chRTPtr->stateTimer = 0x0u;
            chRTPtr->stateToutFlg = FALSE;
            /* @req SWS_CanNm_00206 */
            /* Reset nm-timeout timer */
            chRTPtr->nmTimeoutTimer = 0x0u;
            chRTPtr->nmToutFlg = FALSE;

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
            /* @req SWS_CanNm_00061 */
            /* Stop the transmission nm pdu message cycle timer */
            chRTPtr->msgTxCylceTimer = 0u;
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
            chRTPtr->msgTimeoutTimer = 0u;
            chRTPtr->msgToutFlg = FALSE;
            chRTPtr->sendNmMsgFlg = FALSE;
            chRTPtr->netRequestFlg = FALSE;
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

            /* @req SWS_CanNm_00025 */
            /* Set each byte of user data to 0xff */
            for (loop = 0u; loop < CANNM_DEFAULT_PDU_LENGTH; loop++)
            {
                chRTPtr->txPduData[loop] = 0xffu;
            }

            const CanNm_ChannelConfigType* channelConfig = &CanNm_CfgPtr->ChannelConfig[chIndex];
#if CANNM_NODE_ID_ENABLED == STD_ON
            /* Set NodeId to config data */
            if (CANNM_PDU_OFF != channelConfig->PduNidPosition)
            {
                chRTPtr->txPduData[channelConfig->PduNidPosition] = channelConfig->NodeId;
            }
#endif /* CANNM_NODE_ID_ENABLED == STD_ON */

            /* Set CBV to config data */
            if (CANNM_PDU_OFF != channelConfig->PduCbvPosition)
            {
                chRTPtr->txPduData[(uint8)channelConfig->PduCbvPosition] = 0x00;
            }

/* pn fill PNI bit */
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
            /* @req SWS_CanNm_00413,@req SWS_CanNm_00414 */
            /* pnenable=true,shall set transmit PNI bit=1 */
            if (channelConfig->PnEnabled)
            {
                uint8 pnIdx;

                for (pnIdx = 0; pnIdx < pnInfoConfigPtr->CanNmPnInfoLength; pnIdx++)
                {
                    /*
                     * Because transmit immediately,the first msg should not
                     * request the PNC,so the PN Info data cannot set to 0xff.
                     */
                    chRTPtr->txPduData[pnInfoConfigPtr->CanNmPnInfoOffset + pnIdx] = 0x0u;
                }

                CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_PNI_MASK); /* PRQA S 3469 */ /* MISRA Rule Dir 4.9 */

                chRTPtr->pnRxIndFlg = FALSE;
                chRTPtr->pnFilterEnable = FALSE;
            }
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT */

#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
            /* Clear pn ERA info buffer */
            for (loop = 0u; loop < pnInfoConfigPtr->CanNmPnInfoLength; loop++)
            {
                chRTPtr->pnInfoEra[loop] = 0x0u;
            }

            /* Clear pn ERA reset timer */
            for (loop = 0u; loop < CANNM_PN_NUM; loop++)
            {
                chRTPtr->pnEraResetTimer[loop] = 0x0u;
            }
#endif
        }

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
        CanNm_InnerGlobal.pnEiraUpdate = FALSE;

        /* Clear pn EIRA info buffer */
        for (loop = 0u; loop < pnInfoConfigPtr->CanNmPnInfoLength; loop++)
        {
            CanNm_InnerGlobal.pnEiraInfo[loop] = 0x0u;
        }

        /* Clear pn EIRA reset timer */
        for (loop = 0u; loop < CANNM_PN_NUM; loop++)
        {
            CanNm_InnerGlobal.pnEiraResetTimer[loop] = 0x0u;
        }
#endif

        CanNm_InitStatus = CANNM_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               De-initializes the CanNm module.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_DeInit(void)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateDeInit())
#endif
    {
        CanNm_InitStatus = CANNM_UNINIT;
    }
}

/******************************************************************************/
/*
 * Brief               Passive startup of the AUTOSAR CAN NM.
 * ServiceId           0x01
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Passive startup of network management has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_PASSIVESTARUP, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        /* @req SWS_CanNm_00147
         * In network mode,shall not execute this service,
         * return E_NOT_OK.
         */
        if (NM_MODE_NETWORK != chRTPtr->cannmMode)
        {
            SchM_Enter_CanNm_Context();
            chRTPtr->passiveStartUp = TRUE;
            SchM_Exit_CanNm_Context();

            ret = E_OK;
        }
    }
    return ret;
}

/* @ SWS_CanNm_00257 */
/* optional avaliable */
#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Request the network, since ECU needs to communicate on
 *                     the bus.
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
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_NETWORKREQUEST, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        SchM_Enter_CanNm_Context();

        /* @req SWS_CanNm_00104 */
        /* Change network state to request */
        chRTPtr->netRequestStatus = CANNM_NETWORK_REQUESTED;
        chRTPtr->netRequestFlg = TRUE;
#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
        chRTPtr->coordReadyToSleepInd = CANNM_CSR_IND_INIT;
#endif
        SchM_Exit_CanNm_Context();

        ret = E_OK;
    }

    return ret;
}
#endif

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Release the network, since ECU doesn't have to communicate
 *                     on the bus.
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
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_NETWORKRELEASE, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        SchM_Enter_CanNm_Context();

        /* @req SWS_CanNm_00105 */
        /* Change network state to release */
        chRTPtr->netRequestStatus = CANNM_NETWORK_RELEASED;

        SchM_Exit_CanNm_Context();
        ret = E_OK;
    }

    return ret;
}
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

/* @SWS_CanNm_00262
optional avaliable */
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Disable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (28hex) service.
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Disabling of NM PDU transmission ability has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

/* @req SWS_CanNm_00307 */
/* In passive mode,the Service shall have no effect and return E_NOT_OK*/
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_DISABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        SchM_Enter_CanNm_Context();

        if (NM_MODE_NETWORK == chRTPtr->cannmMode)
        {
            /* @req SWS_CanNm_00170 */
            /* Disable the NM PDU transmission ability */
            chRTPtr->canNmTxEnable = FALSE;

            /* @req SWS_CanNm_00173 */
            /*
             * Stop the CanNm Message Cycle Timer in order to stop the
             * transmission of Network Management PDUs.
             */
            chRTPtr->msgTxCylceTimer = 0u;

            /* @req SWS_CanNm_00174 */
            /* Transmit ability disable,shall stop nm timeout timer */
            chRTPtr->nmTimeoutTimer = 0u;

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_CanNm_00175 */
            /*
             * When the NM PDU transmission ability is disabled,the
             * detection of Remote Sleep Indication Timer shall be
             * suspended.
             */
            chRTPtr->remoteTimerpassed = chRTPtr->remoteTimer;
            chRTPtr->remoteTimer = 0u;
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

            ret = E_OK;
        }
        else
        {
            /* @req SWS_CanNm_00172 */
            /* Current mode is not network mode,shall return E_NOT_OK */
        }

        SchM_Exit_CanNm_Context();
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */
    return ret;
}
#endif /* STD_OFF == CANNM_COM_CONTROL_ENABLED */

/* @SWS_CanNm_00264 */
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Enable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (28hex) service.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Enabling of NM PDU transmission ability has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

/* @req SWS_CanNm_00306 */
/* In passive mode,the Service shall have no effect and return E_NOT_OK*/
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_ENABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        SchM_Enter_CanNm_Context();
        if ((FALSE == chRTPtr->canNmTxEnable) && (NM_MODE_NETWORK == chRTPtr->cannmMode))
        {
            chRTPtr->canNmTxEnable = TRUE;
            /* @req SWS_CanNm_00178 */
            /* Start transmission of NM PDUs within the next NM
             * main function. */
            chRTPtr->msgTxCylceTimer = 1u;
            /* @req SWS_CanNm_00179 */
            /* Restart NM-Timeout Timer */
            chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

/* @req SWS_CanNm_00180 */
/* Resumed remote sleep indication timer */
#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
            chRTPtr->remoteTimer = chRTPtr->remoteTimerpassed;
            chRTPtr->remoteTimerpassed = 0u;
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */
            ret = E_OK;
        }
        else
        {
            /* @req SWS_CanNm_00177 */
            /* Transmission ability is enable,return E_NOT_OK */
            /* @req SWS_CanNm_00305 */
            /* Current mode is not network mode,return E_NOT_OK */
        }
        SchM_Exit_CanNm_Context();
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

    return ret;
}
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */

/* @SWS_CanNm_00266 optional avaliable
   @SWS_CanNm_00327 COM user data is not need this api */
#if (CANNM_USER_DATA_ENABLED == STD_ON) && (CANNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/******************************************************************************/
/*
 * Brief               Set user data for NM PDUs transmitted next on the bus.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmUserDataPtr,Pointer where the user data for the next
 *                     transmitted NM PDU shall be copied from
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting of user data has failed
 */
/******************************************************************************/
/* clang-format off */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
/* clang-format on */
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandleAndPointer(CANNM_SERVICE_ID_SETUSERDATA, nmChannelHandle, nmUserDataPtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
        P2VAR(CanNm_InnerChannelType, AUTOMATIC, CANNM_APPL_DATA)
        chRTPtr = &CanNm_ChRunTime[chIndex];
        /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

        /* @req SWS_CanNm_00159 */
        /* Set NM user data*/
        uint8 userDataLength = chCfgPtr->UserDataLength;
        uint8 userDataOffset = chCfgPtr->UserDataOffset;
        uint8* userDtaPtr = &chRTPtr->txPduData[userDataOffset];

        SchM_Enter_CanNm_Context();
        (void)ILib_memcpy(userDtaPtr, nmUserDataPtr, userDataLength);
        SchM_Exit_CanNm_Context();

        ret = E_OK;
    }

    return ret;
}
#endif

/* @SWS_CanNm_00268
option avaliable*/
#if CANNM_USER_DATA_ENABLED == STD_ON
/******************************************************************************/
/*
 * Brief               Get user data out of the most recently received NM PDU.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmUserDataPtr,Pointer where user data out of the most
 *                     recently received NM PDU shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of user data has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandleAndPointer(CANNM_SERVICE_ID_GETUSERDATA, nmChannelHandle, nmUserDataPtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        const CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        /* @req SWS_CanNm_00160 */
        /* Provided most recently received NM PDU user data*/
        uint8 userDataLength = chCfgPtr->UserDataLength;
        uint8 userDataOffset = chCfgPtr->UserDataOffset;
        const uint8* userDtaPtr = &chRTPtr->rxPduData[userDataOffset];

        SchM_Enter_CanNm_Context();
        (void)ILib_memcpy(nmUserDataPtr, userDtaPtr, userDataLength);
        SchM_Exit_CanNm_Context();

        ret = E_OK;
    }

    return ret;
}
#endif

/* @SWS_CanNm_00330 */
#if CANNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief       Requests transmission of a PDU.
 * @id          0x49
 * @synchronous Synchronous
 * @reentrancy  Reentrant for different PduIds. Non reentrant for the same PduId.
 * @param[in]   TxPduId    Identifier of the PDU to be transmitted
 * @param[in]   PduInfoPtr Length of and pointer to the PDU data and pointer to MetaData.
 * @retval      E_OK       Transmit request has been accepted.
 * @retval      E_NOT_OK   Transmit request has not been accepted.
 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateTxPduId(CANNM_SERVICE_ID_TRANSMIT, TxPduId))
#endif
    {
        CanNm_InnerChannelType* chRTPtr = NULL_PTR;
        uint8 chIndex;

        for (chIndex = 0; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_UserDataTxPduType* userDataPduPtr = CanNm_CfgPtr->ChannelConfig[chIndex].UserDataTxPdu;
            if ((userDataPduPtr != NULL_PTR) && (userDataPduPtr->TxUserDataPduId == TxPduId))
            {
                chRTPtr = &CanNm_ChRunTime[chIndex];
                break;
            }
        }
        if (chRTPtr != NULL_PTR)
        {
            /* @req SWS_CanNm_00464 */
            if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) || (NM_STATE_REPEAT_MESSAGE == chRTPtr->canNmState))
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

    CANNM_UNUSED(PduInfoPtr);
    return ret;
}
#endif

/**
 * @brief       Get node identifier out of the most recently received NM PDU.
 * @id          0x06
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     Pointer where node identifier out of the most recently received NM PDU shall be copied
 *                              to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of the node identifier out of the most recently received NM PDU has failed or is
 *                              not configured for this network handle.
 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_NODE_ID_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandleAndPointer(CANNM_SERVICE_ID_GETNODEIDENTIFIER, nmChannelHandle, nmNodeIdPtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        const CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        /* @req SWS_CanNm_00132 */
        if (chCfgPtr->NodeIdEnabled)
        {
            SchM_Enter_CanNm_Context();
            *nmNodeIdPtr = chRTPtr->rxPduData[(uint8)chCfgPtr->PduNidPosition];
            SchM_Exit_CanNm_Context();
            ret = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmNodeIdPtr);
#endif /* CANNM_NODE_ID_ENABLED == STD_ON */
    return ret;
}

/**
 * @brief       Get node identifier configured for the local node.
 * @id          0x07
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     Pointer where node identifier of the local node shall be copied to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of the node identifier of the local node has failed or is not configured for
 *                              this network handle.
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_NODE_ID_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandleAndPointer(CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmChannelHandle, nmNodeIdPtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        /* @req SWS_CanNm_00133 */
        if (chCfgPtr->NodeIdEnabled)
        {
            *nmNodeIdPtr = chCfgPtr->NodeId;

            ret = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmNodeIdPtr);
#endif /* CANNM_NODE_ID_ENABLED == STD_ON */
    return ret;
}

/**
 * @brief       Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 * @id          0x08
 * @synchronous Asynchronous
 * @reentrancy  Reentrant (but not for the same NM-channel)
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Setting of Repeat Message Request Bit has failed or is not configured for this network
 *                              handle.
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_REPEATMESSAGEREQUEST, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        if (CanNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled)
        {
            CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

            SchM_Enter_CanNm_Context();
            /* @req SWS_CanNm_00137
             * If the service is called in Repeat Message State,
             * Prepare Bus-Sleep Mode or Bus-Sleep Mode,the CanNm module
             * shall not execute the service and return E_NOT_OK.
             */
            if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) || (NM_STATE_READY_SLEEP == chRTPtr->canNmState))
            {
                chRTPtr->repeatMessageRequest = TRUE;
                ret = E_OK;
            }
            SchM_Exit_CanNm_Context();
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif
    return ret;
}

/**
 * @brief       Get the whole PDU data out of the most recently received NM PDU.
 * @id          0x0a
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmPduDataPtr    Pointer where NM PDU shall be copied to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of NM PDU Data has failed or is not configured for this network handle.
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_USER_DATA_ENABLED == STD_ON || CANNM_NODE_ID_ENABLED == STD_ON || CANNM_NODE_DETECTION_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandleAndPointer(CANNM_SERVICE_ID_GETPDUDATA, nmChannelHandle, nmPduDataPtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        /* @req SWS_CanNm_00138 @req SWS_CanNm_00407 */
        /* Provide whole payload of the most recently received NM PDU */
#if CANNM_USER_DATA_ENABLED == STD_OFF
        if (FALSE
#if CANNM_NODE_ID_ENABLED == STD_ON
            || CanNm_CfgPtr->ChannelConfig[chIndex].NodeIdEnabled
#endif /* CANNM_NODE_ID_ENABLED == STD_ON */
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
            || CanNm_CfgPtr->ChannelConfig[chIndex].NodeDetectionEnabled
#endif /* CANNM_NODE_DETECTION_ENABLED == STD_ON */
        )
#endif /* CANNM_USER_DATA_ENABLED == STD_OFF */
        {
            const CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
            SchM_Enter_CanNm_Context();
            (void)ILib_memcpy(nmPduDataPtr, chRTPtr->rxPduData, CANNM_DEFAULT_PDU_LENGTH);
            SchM_Exit_CanNm_Context();
            ret = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmPduDataPtr);
#endif /* CANNM_USER_DATA_ENABLED == STD_ON || CANNM_NODE_ID_ENABLED == STD_ON || CANNM_NODE_DETECTION_ENABLED == \
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
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandlePointerAndPointer(CANNM_SERVICE_ID_GETSTATE, nmChannelHandle, nmStatePtr, nmModePtr))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);

        SchM_Enter_CanNm_Context();
        *nmStatePtr = CanNm_ChRunTime[chIndex].canNmState;
        *nmModePtr = CanNm_ChRunTime[chIndex].cannmMode;
        SchM_Exit_CanNm_Context();
        ret = E_OK;
    }

    return ret;
}

#if CANNM_VERSION_INFO_API == STD_ON
/******************************************************************************/
/*
 * Brief               This service returns the version information of this module.
 * ServiceId           0xf1
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo, Pointer to where to store the version information
 *                     of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePointer(CANNM_SERVICE_ID_GETVERSIONINFO, versioninfo))
#endif
    {
        (versioninfo)->vendorID = CANNM_VENDOR_ID;
        (versioninfo)->moduleID = CANNM_MODULE_ID;
        (versioninfo)->sw_major_version = CANNM_C_SW_MAJOR_VERSION;
        (versioninfo)->sw_minor_version = CANNM_C_SW_MINOR_VERSION;
        (versioninfo)->sw_patch_version = CANNM_C_SW_PATCH_VERSION;
    }
}
#endif

/* optional avaliable @SWS_CanNm_00280 */
#if ((STD_ON == CANNM_BUS_SYNCHRONIZATION_ENABLED) && (STD_OFF == CANNM_PASSIVE_MODE_ENABLED))
/******************************************************************************/
/*
 * Brief               Request bus synchronization.
 * ServiceId           0xc0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of bus synchronization has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        if ((NM_MODE_NETWORK == chRTPtr->cannmMode)
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
            && chRTPtr->canNmTxEnable
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */
        )
        {
            /* @req SWS_CanNm_00130 */
            /*
             * This service shall trigger transmission of a single
             * Network Management PDU
             */
            chRTPtr->sendNmMsgFlg = TRUE;
            ret = E_OK;
        }
        else
        {
            /* @req SWS_CanNm_00187 */
            /*
             * If CanNm is in Bus-Sleep Mode and Prepare Bus-Sleep Mode
             * the Service shall not execute and Return E_NOT_OK.
             */

            /* @req SWS_CanNm_00181 */
            /*
             * If the NM PDU transmission ability is disabled,the service
             * shall return E_NOT_OK.
             */
        }
    }

    return ret;
}
#endif

/* optional avaliable @SWS_CanNm_00282 */
#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Check if remote sleep indication takes place or not.
 * ServiceId           0xd0
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmRemoteSleepIndPtr, Pointer where check result of remote
 *                     sleep indication shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Checking of remote sleep indication bits has
                       failed
 */
/******************************************************************************/
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

        SchM_Enter_CanNm_Context();
        if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) || (NM_STATE_READY_SLEEP == chRTPtr->canNmState))
        {
            *nmRemoteSleepIndPtr = chRTPtr->remoteSleepInd;
            ret = E_OK;
        }
        else
        {
            /* @req SWS_CanNm_00154 */
            /*
             * If CanNm in Bus-Sleep Mode, Prepare Bus-Sleep Mode or
             * Repeat Message State,the service shall not be executed.
             * And E_NOT_OK shall be returned.
             */
        }
        SchM_Exit_CanNm_Context();
    }

    return ret;
}
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

/* optional avaliable @SWS_CanNm_00340 */
#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Set the NM Coordinator Sleep Ready bit in the Control Bit
 *                     Vector.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmSleepReadyBit, Value written to ReadySleep Bit in CBV
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Writing of remote sleep indication bit has
                       failed
 */
/******************************************************************************/
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_SETSLEEPREADYBIT, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        /* @req SWS_CanNm_00321 */
        /*
         * CanNm shall set the "NM Coordinator Sleep Ready Bit" bit to
         * passed value and trigger a single Network Management PDU.
         */
        if (nmSleepReadyBit)
        {
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK);
        }
        else
        {
            CanNm_ClrTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK);
        }
        chRTPtr->sendNmMsgFlg = TRUE;
        ret = E_OK;
    }

    return ret;
}
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if ((STD_OFF == CANNM_IMMEDIATE_TXCONF_ENABLED) && (STD_OFF == CANNM_PASSIVE_MODE_ENABLED))
#define CANNM_START_SEC_CANNM_TXCONFIRMATION_CALLBACK_CODE
#include "CanNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms
 *                     the transmission of a PDU, or the failure to transmit a PDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same
 *                     PduId.
 * Param-Name[in]      TxPduId, ID of the PDU that has been transmitted.
 *                     result, E_OK: The PDU was transmitted. E_NOT_OK: Transmission
 *                     of the PDU failed.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_TxConfirmation(PduIdType TxPduID) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateTxPduId(CANNM_SERVICE_ID_TXCONFIRMATION, TxPduID))
#endif
    {
        uint8 chIndex;
        for (chIndex = 0; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            if (TxPduID == CanNm_CfgPtr->ChannelConfig[chIndex].TxPdu->TxConfirmationPduId)
            {
                break;
            }
        }
        CanNm_InnerTxConfHandle(chIndex);
    }
}
#define CANNM_STOP_SEC_CANNM_TXCONFIRMATION_CALLBACK_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CANNM_RXINDICATION_CALLBACK_CODE
#include "CanNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer communication
 *                     interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same
 *                     PduId.
 * Param-Name[in]      RxPduId, ID of the received PDU.
 *                     PduInfoPtr, Contains the length (SduLength) of the received
 *                     PDU, a pointer to a buffer (SduDataPtr) containing the PDU,
 *                     and the MetaData related to this PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateRxPdu(CANNM_SERVICE_ID_RXINDICATION, RxPduId, PduInfoPtr))
#endif
    {
        if (PduInfoPtr->SduLength == CANNM_DEFAULT_PDU_LENGTH)
        {
            P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) chCfgPtr;
            CanNm_InnerChannelType* chRTPtr;
            uint8 chIndex;
            PduIdType pduIdx;

            for (chIndex = 0; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
                for (pduIdx = 0; pduIdx < chCfgPtr->RxPduNum; pduIdx++)
                {
                    if (RxPduId == chCfgPtr->RxPdu[pduIdx].RxPduId)
                    {
                        break;
                    }
                }

                if (chCfgPtr->RxPduNum != pduIdx)
                {
                    break;
                }
            }

            chRTPtr = &CanNm_ChRunTime[chIndex];

/* @req SWS_CanNm_00337, @req SWS_CanNm_00339*/
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
            if (CanNm_RxPnFilter(chCfgPtr, chRTPtr, PduInfoPtr))
#endif
            {
                /* @req SWS_CanNm_00035 */
                (void)ILib_memcpy(chRTPtr->rxPduData, PduInfoPtr->SduDataPtr, (uint16)PduInfoPtr->SduLength);
                chRTPtr->rxPduFlg = TRUE;

/* @req SWS_CanNm_00037 */
/* Notify nm layer,if rx pdu */
#if (STD_ON == CANNM_PDU_RX_INDICATION_ENABLED)
                Nm_PduRxIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
#endif /* STD_ON == CANNM_PDU_RX_INDICATION_ENABLED */
            }
        }
    }
}
#define CANNM_STOP_SEC_CANNM_RXINDICATION_CALLBACK_CODE
#include "CanNm_MemMap.h"

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
#define CANNM_START_SEC_CANNM_CONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Enables the PN filter functionality on the indicated NM
 *                     channel.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY, nmChannelHandle))
#endif
    {
        CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
        if (chCfgPtr->PnEnabled)
        {
            /* @SWS_CanNm_00404
            enbale pdu filter ability */
            chRTPtr->pnFilterEnable = TRUE;
        }
    }
}
#define CANNM_STOP_SEC_CANNM_CONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CANNM_TRIGGERTRANSMIT_CALLBACK_CODE
#include "CanNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Within this API,the upper layer module (called module) shall
 *                     check whether the available data fits into the buffer size
 *                     reported by PduInfoPtr->SduLength. If it fits, it shall copy
 *                     its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                     and update the length of the actual copied data in
 *                     PduInfoPtr->SduLength.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same
 *                     PduId.
 * Param-Name[in]      TxPduId, ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr, Contains a pointer to a buffer (SduDataPtr) to
 *                     where the SDU data shall be copied, and the available buffer
 *                     size in SduLengh.
 * Return              E_OK: SDU has been copied and SduLength indicates the number
 *                     of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be
                       used since it may contain a NULL pointer or point to invalid
                       data.
 */
/******************************************************************************/
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr) /* PRQA S 3673 */ /* MISRA Dir 8.13 */
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidateTxPdu(CANNM_SERVICE_ID_TRIGGERTRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        CanNm_InnerChannelType* chRTPtr = NULL_PTR; /* PRQA S 3678 */ /* VL_QAC_3678 */
        uint8 chIndex;
#if (STD_ON == CANNM_COM_USERDATA_SUPPORT)
        P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) chCfgPtr;
#endif

        if (PduInfoPtr->SduLength >= CANNM_DEFAULT_PDU_LENGTH)
        {
            for (chIndex = 0; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                if (TxPduId == CanNm_CfgPtr->ChannelConfig[chIndex].TxPdu->TxConfirmationPduId)
                {
#if (STD_ON == CANNM_COM_USERDATA_SUPPORT)
                    chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
#endif
                    chRTPtr = &CanNm_ChRunTime[chIndex];
                    break;
                }
            }
            if (chRTPtr != NULL_PTR)
            {

#if (STD_ON == CANNM_COM_USERDATA_SUPPORT)
                /* @req SWS_CanNm_00350 */
                if (chCfgPtr->UserDataTxPdu != NULL_PTR)
                {
                    PduInfoType pduInfo;
                    uint8 userData[CANNM_USER_DATA_MAX_LENGTH] = {0};
                    pduInfo.SduLength = chCfgPtr->UserDataLength;
                    pduInfo.SduDataPtr = userData;
                    pduInfo.MetaDataPtr = NULL_PTR;
                    if (PduR_CanNmTriggerTransmit(chCfgPtr->UserDataTxPdu->UpperLayerPduId, &pduInfo) == E_OK)
                    {
                        SchM_Enter_CanNm_Context();
                        (void)ILib_memcpy(
                            &chRTPtr->txPduData[chCfgPtr->UserDataOffset],
                            userData,
                            chCfgPtr->UserDataLength);
                        SchM_Exit_CanNm_Context();
                    }
                }
#endif

                /* @req SWS_CanNm_00351 */
                SchM_Enter_CanNm_Context();
                (void)ILib_memcpy(PduInfoPtr->SduDataPtr, chRTPtr->txPduData, CANNM_DEFAULT_PDU_LENGTH);
                SchM_Exit_CanNm_Context();

                ret = E_OK;
            }
        }
    }

    return ret;
}
#define CANNM_STOP_SEC_CANNM_TRIGGERTRANSMIT_CALLBACK_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
/******************************************************************************/
/*
 * Brief               Main function of the CanNm which processes the algorithm
 *                     describes in that document.
 * ServiceId           0x13
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if (CanNm_InitStatus == CANNM_INIT)
    {
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif

        for (uint8 chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
            if (CanNm_CfgPtr->ChannelConfig[chIndex].applicationID != applicationID)
            {
                continue;
            }
#endif

            CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
            if (chRTPtr->rxPduFlg)
            {
                chRTPtr->rxPduFlg = FALSE;
                chRTPtr->rxPduExtFlg = TRUE;
                CanNm_RxDataMainHandle(chIndex);
            }

            CanNm_TimerManagement(chIndex);
            CanNm_StateManagement(chIndex);
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
            CanNm_MsgcycleTimerManage(chIndex);
            CanNm_SendMsgMainHandle(chIndex);
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */
            /* Clear Flags */
            chRTPtr->rxPduExtFlg = FALSE;
#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
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
static inline boolean CanNm_TestAndClear(boolean* flagPtr)
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
#if (STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_RxCarWakeUpHadle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(CanNm_InnerChannelType, AUTOMATIC, CANNM_APPL_DATA)
    chRTPtr = &CanNm_ChRunTime[chIndex];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 carWakeupMask = (1u << (chCfgPtr->CarWakeUpBitPosition));
    SchM_Enter_CanNm_Context();
    uint8 carWakeupByte = chRTPtr->rxPduData[chCfgPtr->CarWakeUpBytePosition];
    SchM_Exit_CanNm_Context();

#if (STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED)
    if (CanNm_GetCarWakeUpRxEnabled(chIndex) && ((carWakeupByte & carWakeupMask) > 0u))
    {
#if (STD_ON == CANNM_CAR_WAKEUP_FILTER_ENABLED)
        if (CanNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpFilterEnabled)
        {
            /* @req SWS_CanNm_00408 */
            if (CANNM_PDU_OFF != chCfgPtr->PduNidPosition)
            {
                SchM_Enter_CanNm_Context();
                uint8 rxNodeId = chRTPtr->rxPduData[(uint8)chCfgPtr->PduNidPosition];
                SchM_Exit_CanNm_Context();

                if (CanNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpFilterNodeId == rxNodeId)
                {
                    Nm_CarWakeUpIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
                }
            }
        }
        else
#endif /* STD_ON == CANNM_CAR_WAKEUP_FILTER_ENABLED */
        {
            /* @req SWS_CanNm_00406 */
            Nm_CarWakeUpIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED */
}
#endif /* STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED */

static CanNm_ChannelIndexType CanNm_FindChannelIndex(NetworkHandleType nmChannelHandle)
{
    CanNm_ChannelIndexType chIndex;
    for (chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (nmChannelHandle == CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef)
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
 * Param-Name[in]      chIndex, Channel index in CanNm
 *                     nmNewState, new state of CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_StateChange(uint8 chIndex, Nm_StateType nmNewState)
{
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
#if (STD_ON == CANNM_STATE_CHANGE_IND_ENABLED)
    /* @req SWS_CanNm_00166 */
    /* CanNm states shall be notified to the upper layer.*/
    Nm_StateChangeNotification(
        CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef,
        chRTPtr->canNmState,
        nmNewState);
#endif /* STD_ON == CANNM_STATE_CHANGE_IND_ENABLED */

    SchM_Enter_CanNm_Context();
    chRTPtr->canNmState = nmNewState;
    SchM_Exit_CanNm_Context();
}

/******************************************************************************/
/*
 * Brief               Process when enter from other state to repeat msg state
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 *                     isNetWorkRequest, Indicate whether enter repeate message
 *                                       state triggered by NetworkRequest
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_EntryRepeatMessageState(uint8 chIndex, boolean isNetWorkRequest)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    /* Load CanNmRepeatMessageTime to stateTimer */
    /* @req SWS_CanNm_00102 */
    chRTPtr->stateTimer = chCfgPtr->RepeatMessageTime;

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    /* @req SWS_CanNm_00100 */
    /* Enter Repeat Message State,transmission shall start*/
    if ((TRUE != isNetWorkRequest) || (0u == chCfgPtr->ImmediateNmTransmissions))
    {
        /* @req SWS_CanNm_00005 */
        /*
         * If the Repeat Message State is not entered via CanNm_NetworkRequest
         * OR CanNmImmediateNmTransmissions is zero the transmission of NM PDU
         * shall be delayed by CANNM_MSG_CYCLE_OFFSET after entering the
         * repeat message state.
         */
        chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleOffset;

        if (0u == chRTPtr->msgTxCylceTimer)
        {
            chRTPtr->msgTxCylceTimer = 1u;
        }

#if (STD_ON == CANNM_IMMEDIATE_RESTART_ENABLED)
        if (isNetWorkRequest && (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->cannmMode))
        {
            /* @req SWS_CanNm_00122 */
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;
        }
#endif
    }
#if (STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED)
    else
    {
        /* @req SWS_CanNm_00334 @req SWS_CanNm_00454 @req SWS_CanNm_00122 */
        /*
         * The NM PDUs shall be transmitted using CanNmImmediateNmCycleTime
         * as cycle time.
         * The first NM PDU shall be triggered as soon as possible.
         */
        if ((NM_MODE_BUS_SLEEP == chRTPtr->cannmMode) || (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->cannmMode)
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
            || ((NM_MODE_NETWORK == chRTPtr->cannmMode) && chCfgPtr->PnHandleMultipleNetworkRequests)
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT */
        )
        {
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;

            chRTPtr->msgTxCylceTimer = chCfgPtr->ImmediateNmCycleTime;
            chRTPtr->immedMsgTxCnt = chCfgPtr->ImmediateNmTransmissions;
        }
    }
#endif
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    /* @req SWS_CanNm_00007 */
    if ((NM_MODE_BUS_SLEEP == chRTPtr->cannmMode) || (NM_MODE_PREPARE_BUS_SLEEP == chRTPtr->cannmMode))
    {
        chRTPtr->retrySendMsgFlag = chCfgPtr->RetryFirstMessageRequest;
    }
#endif
#endif

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
    /* @req SWS_CanNm_00152 */
    /*
     * Entry Repeat Message state from Normal Operation State or Ready
     * Sleep State,shall cancel remote notify.
     */
    if (chRTPtr->remoteSleepInd
        && ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) || (NM_STATE_READY_SLEEP == chRTPtr->canNmState)))
    {
        chRTPtr->remoteSleepInd = FALSE;
        Nm_RemoteSleepCancellation(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
#endif /*STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

    CanNm_StateChange(chIndex, NM_STATE_REPEAT_MESSAGE);

    if (NM_MODE_NETWORK != chRTPtr->cannmMode)
    {

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
        CanNm_ChRunTime[chIndex].coordReadyToSleepInd = CANNM_CSR_IND_INIT;
        CanNm_ClrTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK);
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

        /* @req SWS_CanNm_00096 */
        /* Network Mode is entered,NM-Timeout Timer started.*/
        chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

        /* @req SWS_CanNm_00097 @req SWS_CanNm_00093 */
        /* Network Mode is entered, notify the upper layer by Nm_NetworkMode.*/
        chRTPtr->cannmMode = NM_MODE_NETWORK;
        Nm_NetworkMode(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
}

/******************************************************************************/
/*
 * Brief               Process used timers
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
static FUNC(void, CANNM_CODE) CanNm_MsgcycleTimerManage(uint8 chIndex)
{
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    if ((chRTPtr->msgTxCylceTimer > 0u)
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
        /* @req SWS_CanNm_00173 */
        && chRTPtr->canNmTxEnable
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */
    )
    {
        chRTPtr->msgTxCylceTimer--;
        if ((0u == chRTPtr->msgTxCylceTimer)
            && (((CANNM_NETWORK_REQUESTED == chRTPtr->netRequestStatus))
                || (NM_STATE_REPEAT_MESSAGE == chRTPtr->canNmState)))
        {
            /* @req SWS_CanNm_00032 */
            /* Set send NM message flag */
            chRTPtr->sendNmMsgFlg = TRUE;
        }
    }
}
#endif /* STD_ON == CANNM_PASSIVE_MODE_ENABLED */

/******************************************************************************/
/*
 * Brief               Process used timers
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_TimerManagement(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

/* Message timeout timer */
#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    if (chRTPtr->msgTimeoutTimer > 0u)
    {
        chRTPtr->msgTimeoutTimer--;
        if (0u == chRTPtr->msgTimeoutTimer)
        {
            chRTPtr->msgToutFlg = TRUE;

            /* @req SWS_CanNm_00446  @req SWS_CanNm_00066*/
            /*CanNmGlobalPnSupport = TRUE, Msg Tx timer expired,
             *call  Nm_TxTimeoutException  CanSM_TxTimeoutException*/

#if (STD_ON == CANNM_VER_4_2_2)
            Nm_TxTimeoutException(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
            CanSM_TxTimeoutException(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
#endif

#if (STD_ON == CANNM_VER_R19_11)
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
            if (chCfgPtr->PnEnabled)
            {
                Nm_TxTimeoutException(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
                CanSM_TxTimeoutException(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
            }
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT*/
#endif
        }
    }
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

/* NM time out timer */
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
    /* @req SWS_CanNm_00174 */
    /* Transmit ability disable,shall stop nm timeout timer */
    if (chRTPtr->canNmTxEnable)
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */
    {
        if (chRTPtr->nmTimeoutTimer > 0u)
        {
            chRTPtr->nmTimeoutTimer--;
            if (0u == chRTPtr->nmTimeoutTimer)
            {
                chRTPtr->nmToutFlg = TRUE;

                if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState)
                    || (NM_STATE_REPEAT_MESSAGE == chRTPtr->canNmState))
                {
                    /* @req SWS_CanNm_00117,@req SWS_CanNm_00101 */
                    /*
                     * When the NM-Timeout Timer expires in the Normal
                     * Operation State or Repeat Message State,the
                     * NM-Timeout Timer shall be restarted.
                     */
                    chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;

                    /* @req SWS_CanNm_00194 @req SWS_CanNm_00193*/
                    /*
                     * When the NM-Timeout Timer expires in the Repeat Message
                     * State or Normal Operation State, the NM shall report
                     * CANNM_E_NETWORK_TIMEOUT as Run Time Error.
                     */

                    (void)Det_ReportRuntimeError(
                        CANNM_MODULE_ID,
                        CANNM_INSTANCE_ID,
                        CANNM_SERVICE_ID_MAINFUNCTION,
                        CANNM_E_NETWORK_TIMEOUT);
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
#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
    if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) && (chRTPtr->remoteTimer > 0u))
    {
        chRTPtr->remoteTimer--;
        if (0u == chRTPtr->remoteTimer)
        {
            /* @req SWS_CanNm_00150 */
            chRTPtr->remoteSleepInd = TRUE;
            Nm_RemoteSleepIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
    if (chCfgPtr->PnEraCalcEnabled)
    {
        CanNm_PnEraResetTimerHandle(chIndex);
    }
#endif /* STD_ON == CANNM_PN_ERA_CALC_ENABLED */

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
    if (CANNM_EIRA_HANDLE_CHANNEL == chIndex)
    {
        CanNm_PnEiraResetTimerHandle();
    }
#endif /* STD_ON == CANNM_PN_EIRA_CALC_ENABLED */
}

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
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
static FUNC(boolean, CANNM_CODE) CanNm_RxPnFilter(
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) chCfgPtr,
    CanNm_InnerChannelType* chRTPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr)
{
    boolean ret = TRUE;

    /*@req SWS_CanNm_00409*/
    if (chCfgPtr->PnEnabled)
    {
        /* @req SWS_CanNm_00410 */
        if (chCfgPtr->AllNmMessagesKeepAwake)
        {
            chRTPtr->pnRxIndFlg = TRUE;
        }
        else if (CANNM_PDU_OFF != chCfgPtr->PduCbvPosition)
        {
            uint8 cbv = PduInfoPtr->SduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
            /*@req SWS_CanNm_00412 @req SWS_CanNm_00411*/
            if ((cbv & CANNM_CBV_BIT_PNI_MASK) > 0u)
            {
                if (chRTPtr->pnFilterEnable)
                {
                    uint8 pduIdx;
                    const CanNm_PnInfoConfigType* pnInfoPtr = CanNm_CfgPtr->PnInfo;
                    const CanNm_PnFilterMaskByteType* pnFilMaskByte = pnInfoPtr->PnFilterMaskByte;

                    ret = FALSE;
                    for (pduIdx = 0u; pduIdx < pnInfoPtr->CanNmPnFilterMaskByteNum; pduIdx++)
                    {
                        uint8 byteIndex = pnInfoPtr->CanNmPnInfoOffset + pnFilMaskByte[pduIdx].PnFilterMaskByteIndex;

                        if ((PduInfoPtr->SduDataPtr[byteIndex] & pnFilMaskByte[pduIdx].PnFilterMaskByteValue) > 0u)
                        {
                            chRTPtr->pnRxIndFlg = TRUE;
                            ret = TRUE;
                            break;
                        }
                    }
                }
                else
                {
                    /*@req SWS_CanNm_00404 note*/
                    chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;
                    chRTPtr->pnRxIndFlg = FALSE;
                }
            }
            else
            {
                /* not accept this nm pdu*/
                chRTPtr->pnRxIndFlg = FALSE;
                ret = FALSE;
            }
        }
        else
        {
            /*@req SWS_CanNm_00409*/
            /* Nothing to do here*/
        }
    }
    return ret;
}
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT*/

#if (                                   \
    (STD_ON == CANNM_GLOBAL_PN_SUPPORT) \
    && ((STD_ON == CANNM_PN_ERA_CALC_ENABLED) || (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)))
/******************************************************************************/
/*
 * Brief               Handle EIRA and ERA according received data
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_RxPnEiraEraHandle(uint8 chIndex)
{
#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
#endif
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    const CanNm_PnInfoConfigType* pnInfoPtr = CanNm_CfgPtr->PnInfo;
    uint16* pnEiraResetTimerPtr = CanNm_InnerGlobal.pnEiraResetTimer;
    uint8 rxPnInfoDta;
    uint8 bitMask;
    uint8 index;
    P2CONST(uint8, AUTOMATIC, CANNM_APPL_DATA) rxPnDtaPtr = &(chRTPtr->rxPduData[pnInfoPtr->CanNmPnInfoOffset]);
    uint8 filterMaskData;
    uint8 pnIdx = 0u;

    /* Handle ERA and EIRA resetTimer */
    for (index = 0u; index < (pnInfoPtr->CanNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            rxPnInfoDta = rxPnDtaPtr[index / 8u];

            if (0u != (rxPnInfoDta & bitMask))
            {
#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
                if (chCfgPtr->PnEraCalcEnabled)
                {
                    /* @req SWS_CanNm_00359 */
                    /*
                     * A PN is requested by reception the monitoring for
                     * this PN shall be restarted.
                     */
                    chRTPtr->pnEraResetTimer[pnIdx] = CanNm_CfgPtr->PnResetTime;
                }
#endif

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
                /* @req SWS_CanNm_00349 */
                SchM_Enter_CanNm_PnEiraCalc();
                pnEiraResetTimerPtr[pnIdx] = CanNm_CfgPtr->PnResetTime;
                SchM_Exit_CanNm_PnEiraCalc();
#endif
            }
            pnIdx++;
        }
    }

    /* @req SWS_CanNm_00347 @req SWS_CanNm_00357 */
    /*
     * PNs are requested within received message, CanNm shall store the request information
     */
#if CANNM_PN_ERA_CALC_ENABLED == STD_ON
    boolean rxIndEraFlag = FALSE;
#endif /* CANNM_PN_ERA_CALC_ENABLED == STD_ON */

#if CANNM_PN_EIRA_CALC_ENABLED == STD_ON
    boolean rxIndEiraFlag = FALSE;
    uint8* pnEiraInfoPtr = CanNm_InnerGlobal.pnEiraInfo;
#endif /* CANNM_PN_EIRA_CALC_ENABLED == STD_ON */

    SchM_Enter_CanNm_PnEiraCalc();
    for (index = 0u; index < pnInfoPtr->CanNmPnInfoLength; ++index)
    {
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
        uint8 relatedPnData = rxPnDtaPtr[index] & filterMaskData;

#if CANNM_PN_ERA_CALC_ENABLED == STD_ON
        if (chCfgPtr->PnEraCalcEnabled)
        {
            if (!rxIndEraFlag && ((relatedPnData & (~chRTPtr->pnInfoEra[index])) > 0u)) /* rx pdu pninfo bit 0--->1 */
            {
                rxIndEraFlag = TRUE;
            }
            chRTPtr->pnInfoEra[index] |= relatedPnData;
        }
#endif

#if CANNM_PN_EIRA_CALC_ENABLED == STD_ON
        if (!rxIndEiraFlag && ((relatedPnData & (~pnEiraInfoPtr[index])) > 0u)) /* rx pdu pninfo bit 0--->1 */
        {
            rxIndEiraFlag = TRUE;
            CanNm_InnerGlobal.pnEiraUpdate = TRUE;
        }
        pnEiraInfoPtr[index] |= relatedPnData;
#endif
    }
    SchM_Exit_CanNm_PnEiraCalc();

    /* @req SWS_CanNm_00352,@req SWS_CanNm_00361 */
    /*
     * If the stored value for a PN is set to requested,CanNm
     * shall inform upper layers.
     */
    PduIdType upRxPduId;
    PduInfoType PduInfo;
#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
    if (rxIndEraFlag)
    {
        upRxPduId = chCfgPtr->PnEraRxNSduRef;
        PduInfo.SduLength = pnInfoPtr->CanNmPnInfoLength;
        PduInfo.SduDataPtr = chRTPtr->pnInfoEra;
        PduInfo.MetaDataPtr = NULL_PTR;
        PduR_CanNmRxIndication(upRxPduId, &PduInfo);
    }
#endif
}
#endif

#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
/******************************************************************************/
/*
 * Brief               Handle ERA reset timer,when timer expired,notify PduR
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_PnEraResetTimerHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    const CanNm_PnInfoConfigType* pnInfoPtr = CanNm_CfgPtr->PnInfo;
    uint8 bitMask;
    boolean eraIndFlag = FALSE;
    uint8 index;
    uint8 filterMaskData;
    uint8 pnIdx = 0u;

    for (index = 0u; index < (pnInfoPtr->CanNmPnInfoLength * 8u); index++)
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
                    /* @req SWS_CanNm_00360 */
                    /*
                     * PN is not requested again within CanNmPnResetTime
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

    /* @req SWS_CanNm_00361 */
    if (eraIndFlag)
    {
        PduIdType upRxPduId = chCfgPtr->PnEraRxNSduRef;
        PduInfoType PduInfoPtr;
        PduInfoPtr.SduLength = pnInfoPtr->CanNmPnInfoLength;
        PduInfoPtr.SduDataPtr = chRTPtr->pnInfoEra;
        PduInfoPtr.MetaDataPtr = NULL_PTR;
        PduR_CanNmRxIndication(upRxPduId, &PduInfoPtr);
    }
}
#endif /*STD_ON == CANNM_PN_ERA_CALC_ENABLED*/

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
/******************************************************************************/
/*
 * Brief               Handle EIRA reset timer,when timer expired,notify PduR
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_PnEiraResetTimerHandle(void)
{
    const CanNm_PnInfoConfigType* pnInfoConfigPtr = CanNm_CfgPtr->PnInfo;
    uint8* pnEiraInfoPtr = CanNm_InnerGlobal.pnEiraInfo;
    uint16* pnEiraResetTimerPtr = CanNm_InnerGlobal.pnEiraResetTimer;
    uint8 bitMask;
    uint8 index;
    uint8 filterMaskData;
    uint8 pnIdx = 0u;

    SchM_Enter_CanNm_PnEiraCalc();
    for (index = 0u; index < (pnInfoConfigPtr->CanNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoConfigPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            if (pnEiraResetTimerPtr[pnIdx] > 0u)
            {
                pnEiraResetTimerPtr[pnIdx]--;
                if (0u == pnEiraResetTimerPtr[pnIdx])
                {
                    /* @req SWS_CanNm_00351 */
                    /*
                     * PN is not requested again within CanNmPnResetTime
                     * then the corresponding stored value for this PN
                     * shall be set to not requested (value 0).
                     */
                    pnEiraInfoPtr[index / 8u] &= (~bitMask);
                    CanNm_InnerGlobal.pnEiraUpdate = TRUE;
                }
            }

            pnIdx++;
        }
    }
    SchM_Exit_CanNm_PnEiraCalc();

    /* @req SWS_CanNm_00352 */
    if (CanNm_InnerGlobal.pnEiraUpdate)
    {
        CanNm_InnerGlobal.pnEiraUpdate = FALSE;
        PduIdType upRxPduId = CanNm_CfgPtr->CanNmPnEiraRxNSduRef;
        PduInfoType PduInfo;
        PduInfo.SduDataPtr = pnEiraInfoPtr;
        PduInfo.MetaDataPtr = NULL_PTR;
        PduInfo.SduLength = pnInfoConfigPtr->CanNmPnInfoLength;
        PduR_CanNmRxIndication(upRxPduId, &PduInfo);
    }
}
#endif

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
static FUNC(Std_ReturnType, CANNM_CODE) CanNm_SendNmPdu(uint8 chIndex)
{
    Std_ReturnType ret;
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
    if (FALSE == chRTPtr->canNmTxEnable)
    {
        ret = E_NOT_OK;
    }
    else
#endif
    {
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];

#if CANNM_PN_EIRA_CALC_ENABLED == STD_ON
        if (chCfgPtr->PnEnabled)
        {
            /* @req SWS_CanNm_00348 @req SWS_CanNm_00345 */
            CanNm_TxPnEiraHandle(chCfgPtr->ComMNetworkHandleRef, chRTPtr->txPduData);
        }
#endif

/* @req SWS_CanNm_00317 */
#if (STD_ON == CANNM_COM_USERDATA_SUPPORT)
        if ((!chCfgPtr->TxPdu->TxPduIsTriggerTrans) && (chCfgPtr->UserDataTxPdu != NULL_PTR))
        {
            PduInfoType pduInfo;
            uint8 userData[CANNM_USER_DATA_MAX_LENGTH] = {0};
            PduIdType pdurTxSduId = chCfgPtr->UserDataTxPdu->UpperLayerPduId;
            pduInfo.SduDataPtr = userData;
            pduInfo.SduLength = chCfgPtr->UserDataLength;
            pduInfo.MetaDataPtr = NULL_PTR;
            if (PduR_CanNmTriggerTransmit(pdurTxSduId, &pduInfo) == E_OK)
            {
                SchM_Enter_CanNm_Context();
                (void)ILib_memcpy(&chRTPtr->txPduData[chCfgPtr->UserDataOffset], userData, chCfgPtr->UserDataLength);
                SchM_Exit_CanNm_Context();
            }
        }
#endif /* STD_OFF==CANNM_COM_USERDATA_SUPPORT */

#if (0u < CANNM_TX_PDU_NUMBER)
        /* if PduR_CanNmTriggerTransmit return E_NOT_OK allow transmit nm pdu */
        PduInfoType pduInfo;
        PduIdType canIfTxSduId = chCfgPtr->TxPdu->LowerLayerPduId;
        pduInfo.SduDataPtr = chRTPtr->txPduData;
        pduInfo.SduLength = CANNM_DEFAULT_PDU_LENGTH;

        /* @req SWS_CanNm_00032 */
        ret = CanIf_Transmit(canIfTxSduId, &pduInfo);

        if ((uint8)E_OK == ret)
        {
#if (STD_OFF == CANNM_IMMEDIATE_TXCONF_ENABLED)
            chRTPtr->msgTimeoutTimer = chCfgPtr->MsgTimeoutTime;
            chRTPtr->msgToutFlg = FALSE;
#else
            CanNm_InnerTxConfHandle(chIndex);
#endif
        }
#endif /* 0u < CANNM_TX_PDU_NUMBER */
    }
    return ret;
}
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

#if CANNM_PN_EIRA_CALC_ENABLED == STD_ON
static void CanNm_TxPnEiraHandle(NetworkHandleType networkHandle, uint8* txDta)
{
    const CanNm_PnInfoConfigType* pnInfoPtr = CanNm_CfgPtr->PnInfo;
    uint8* pnEiraInfoPtr = CanNm_InnerGlobal.pnEiraInfo;
    uint16* pnEiraResetTimerPtr = CanNm_InnerGlobal.pnEiraResetTimer;
    boolean rxIndEiraFlag = FALSE;
    uint8 bitMask;
    uint8 txPnInfoDta, localPnInfoDta;
    uint8* txPnDta = &(txDta[pnInfoPtr->CanNmPnInfoOffset]);
    uint8 index;
    uint8 pnIdx = 0u;
    uint8 filterMaskData;
    uint8 relatedPnData;

    SchM_Enter_CanNm_PnEiraCalc();
    Nm_PncBitVectorTxIndication(networkHandle, txPnDta);

    for (index = 0u; index < (pnInfoPtr->CanNmPnInfoLength * 8u); index++)
    {
        bitMask = (0x1u << (index % 8u));
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index / 8u].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            txPnInfoDta = txPnDta[index / 8u];
            localPnInfoDta = pnEiraInfoPtr[index / 8u];

            if ((txPnInfoDta & bitMask) > 0u)
            {
                /* @req SWS_CanNm_00349 */
                pnEiraResetTimerPtr[pnIdx] = CanNm_CfgPtr->PnResetTime;

                if ((FALSE == rxIndEiraFlag) && ((localPnInfoDta & bitMask) == 0u))
                {
                    rxIndEiraFlag = TRUE;
                }
            }

            pnIdx++;
        }
    }

    for (index = 0u; index < pnInfoPtr->CanNmPnInfoLength; index++)
    {
        filterMaskData = pnInfoPtr->PnFilterMaskByte[index].PnFilterMaskByteValue;
        relatedPnData = txPnDta[index] & filterMaskData;
        if (relatedPnData > 0u)
        {
            pnEiraInfoPtr[index] |= relatedPnData;
        }
    }

    /* @req SWS_CanNm_00352 */
    if (rxIndEiraFlag)
    {
        CanNm_InnerGlobal.pnEiraUpdate = TRUE;
    }
    SchM_Exit_CanNm_PnEiraCalc();
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition of CanNm
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_StateManagement(uint8 chIndex)
{
    const CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    switch (chRTPtr->canNmState)
    {
    case NM_STATE_BUS_SLEEP:
        CanNm_BusSleepStateHandle(chIndex);
        break;

    case NM_STATE_PREPARE_BUS_SLEEP:
        CanNm_PrepareBusSleepStateHandle(chIndex);
        break;

    case NM_STATE_REPEAT_MESSAGE:
        CanNm_RepeatMessageStateHandle(chIndex);
        break;

    case NM_STATE_NORMAL_OPERATION:
        CanNm_NormalOperationStateHandle(chIndex);
        break;

    case NM_STATE_READY_SLEEP:
        CanNm_ReadySleepStateHandle(chIndex);
        break;

    default:
        /*nothing*/
        break;
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bus Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_BusSleepStateHandle(uint8 chIndex)
{
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

    /* @req SWS_CanNm_00127 */
    if (chRTPtr->rxPduExtFlg)
    {
        Nm_NetworkStartIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        (void)Det_ReportRuntimeError(
            CANNM_MODULE_ID,
            CANNM_INSTANCE_ID,
            CANNM_SERVICE_ID_RXINDICATION,
            CANNM_E_NET_START_IND);
    }

    boolean activeWakeUp = TRUE;
    if (CanNm_TestAndClear(&chRTPtr->passiveStartUp))
    {
        activeWakeUp = FALSE;
        /* @req SWS_CanNm_00128 */
        /* Entry repeat message Mode */
        CanNm_EntryRepeatMessageState(chIndex, FALSE);
    }

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Network Request */
    if (CanNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
        if (CanNm_CfgPtr->ChannelConfig[chIndex].ActiveWakeupBitEnabled && activeWakeUp)
        {
            /* @req SWS_CanNm_00401 */
            /* Set ActiveWakeupBit in CBV */
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Rule Dir 4.9 */
        }

        /* @req SWS_CanNm_00129 */
        /*
         * Network is requested in Bus-Sleep Mode, the CanNm module shall
         * enter Repeat Message State.
         */
        CanNm_EntryRepeatMessageState(chIndex, TRUE);
    }
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Prepare Bus Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_PrepareBusSleepStateHandle(uint8 chIndex)
{
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    if (CanNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
        const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
        if (chCfgPtr->ActiveWakeupBitEnabled)
        {
            /* @req SWS_CanNm_00401 */
            /* Set ActiveWakeupBit in CBV */
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Rule Dir 4.9 */
        }

        /* @req SWS_CanNm_00123 */
        /* network  requset,will be entry Repeat Message state */
        CanNm_EntryRepeatMessageState(chIndex, TRUE);
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */

    /* @req SWS_CanNm_00128 SWS_CanNm_00124*/
    if (CanNm_TestAndClear(&chRTPtr->passiveStartUp) || chRTPtr->rxPduExtFlg)
    {
        CanNm_EntryRepeatMessageState(chIndex, FALSE);
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }

    if (chRTPtr->stateToutFlg)
    {
        /* @req SWS_CanNm_00115 */
        /* State timer timeout,enter bus-sleep mode */
        CanNm_StateChange(chIndex, NM_STATE_BUS_SLEEP);
        chRTPtr->cannmMode = NM_MODE_BUS_SLEEP;

        /* @SWS_CanNm_00126 @req SWS_CanNm_00093 */
        Nm_BusSleepMode(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }

exit:; /* do notiong */
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Repeat Message State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_RepeatMessageStateHandle(uint8 chIndex)
{
#if ((STD_OFF == CANNM_PASSIVE_MODE_ENABLED) && (STD_ON == CANNM_GLOBAL_PN_SUPPORT))                \
    || ((STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED) && (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)) \
    || (CANNM_NODE_DETECTION_ENABLED == STD_ON)
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
#endif
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    if (CanNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            CanNm_EntryRepeatMessageState(chIndex, TRUE);
            goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
        }
#endif /*STD_ON == CANNM_GLOBAL_PN_SUPPORT*/
    }
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

    /* @req SWS_CanNm_00102 */
    if (chRTPtr->stateToutFlg)
    {
#if (STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED)
        chRTPtr->immedMsgTxCnt = 0u;
#endif /* STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED */
        if (CANNM_NETWORK_REQUESTED == chRTPtr->netRequestStatus)
        {
            /* @req SWS_CanNm_00103 */
            /*
             * If the network has been requested,normal opertation state
             * shall be entered.
             */
            CanNm_StateChange(chIndex, NM_STATE_NORMAL_OPERATION);

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_CanNm_00150 */
            /* Enter NORMAL OPERATION,reset the RemoteSleepIndTimer */
            chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;
#endif /*STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED*/
        }
        else
        {
            /* @req SWS_CanNm_00106 */
            /*
             * If the network has been released,Ready Sleep State
             * shall be entered
             */
            CanNm_StateChange(chIndex, NM_STATE_READY_SLEEP);

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
            /* @req SWS_CanNm_00108 @req SWS_CanNm_00051 */
            /*
             * When the Ready Sleep State is entered from Repeat Message
             * State,transmission of NM PDUs shall be stopped.
             */
            chRTPtr->msgTxCylceTimer = 0u;
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */
        }

/* @req SWS_CanNm_00107 */
/*
 * CanNm shall clear the Repeat Message Bit when leaving
 * Repeat Message State.
 */
#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
        if (chCfgPtr->NodeDetectionEnabled)
        {
            CanNm_ClrTxPduCbvBit(chIndex, CANNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
        }
#endif
    }

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_GLOBAL_PN_SUPPORT == STD_ON)
exit:; /* do notiong */
#endif
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Normal Operation State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_NormalOperationStateHandle(uint8 chIndex)
{
#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT) || (STD_ON == CANNM_NODE_DETECTION_ENABLED)
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
#endif
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

    if (CanNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        /* @req SWS_CanNm_00454 */
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            CanNm_EntryRepeatMessageState(chIndex, TRUE);
            goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
        }
#endif /* CANNM_GLOBAL_PN_SUPPORT == STD_ON */
    }

    /* @req SWS_CanNm_00118 */
    /* Left normal operation state,enter ready sleep state*/
    if (CANNM_NETWORK_RELEASED == chRTPtr->netRequestStatus)
    {
        CanNm_StateChange(chIndex, NM_STATE_READY_SLEEP);

        /* @req SWS_CanNm_00108 @req SWS_CanNm_00051 */
        /*
         * When the Ready Sleep State is entered from Normal Operation
         * State,transmission of NM PDUs shall be stopped.
         */
        chRTPtr->msgTxCylceTimer = 0u;
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
        chRTPtr->retrySendMsgFlag = FALSE;
#endif
        goto exit; /* PRQA S 2001 */ /* MISRA Rule 15.1 */
    }

#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
    if (chCfgPtr->NodeDetectionEnabled)
    {
        if (chRTPtr->repeatMessageRequest)
        {
            /* @req SWS_CanNm_00120 */
            CanNm_EntryRepeatMessageState(chIndex, FALSE);
            /* @req SWS_CanNm_00121 */
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Rule Dir 4.9 */
        }
        else if (chRTPtr->rxPduExtFlg && (CANNM_PDU_OFF != chCfgPtr->PduCbvPosition))
        {
            uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
            uint8 cbvDta = chRTPtr->rxPduData[cbvPos];
            /* @req SWS_CanNm_00119 */
            /*
             * If CanNmNodeDetectionEnabled is set to TRUE and Repeat
             * Message Request bit is received in Normal Operation State,
             * CanNm shall enter Repeat Message State.
             */
            if ((cbvDta & CANNM_CBV_BIT_RMP_MASK) > 0u)
            {
                CanNm_EntryRepeatMessageState(chIndex, FALSE);
            }
        }
        else
        {
            /* Nothing to do here */
        }
    }
#endif /* STD_ON == CANNM_NODE_DETECTION_ENABLED */

exit:; /* do something about busload reduction */

/* @req SWS_CanNm_00156  SWS_CanNm_00157*/
/*the busload reduction is only work in Normal operation mode*/
#if (STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED)
    if ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) && (chRTPtr->rxPduExtFlg))
    {
        /* @req SWS_CanNm_00069 */
        /* when busload reduction enbale,reload msgTxCylceTimer */
        if (CanNm_CfgPtr->ChannelConfig[chIndex].BusLoadReductionActive)
        {
            chRTPtr->msgTxCylceTimer = chCfgPtr->MsgReducedTime;
        }
    }
#endif /* STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED */
#endif
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Ready Sleep State
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_ReadySleepStateHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    boolean stateChanged = FALSE;

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    if (CanNm_TestAndClear(&chRTPtr->netRequestFlg))
    {
#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            /* @req SWS_CanNm_00454 */
            CanNm_EntryRepeatMessageState(chIndex, TRUE);
        }
        else
#endif
        {
            /* @SWS_CanNm_00110
             network is request ,will entry normal operation mode */
            CanNm_StateChange(chIndex, NM_STATE_NORMAL_OPERATION);

/* @req SWS_CanNm_00116 @req SWS_CanNm_00006 */
/*
 * When the Normal Operation State is entered from Ready Sleep
 * State, transmission of NM PDUs shall be started immediately
 * unless passive mode is enabled or the NM message transmission
 * ability has been disabled.
 */
#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
            if (chRTPtr->canNmTxEnable)
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */
            {
                chRTPtr->msgTxCylceTimer = chCfgPtr->MsgCycleTime;
                chRTPtr->sendNmMsgFlg = TRUE;
            }

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
            /* @req SWS_CanNm_00150 */
            /* Enter NORMAL OPERATION,reset the remoteTimer */
            chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;
#endif /* STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED*/
        }
        stateChanged = TRUE;
    }

#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
    if (chCfgPtr->NodeDetectionEnabled && chRTPtr->repeatMessageRequest && !stateChanged)
    {
        /* @req SWS_CanNm_00112 */
        CanNm_EntryRepeatMessageState(chIndex, FALSE);
        /* @req SWS_CanNm_00113 */
        CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_RMP_MASK); /* PRQA S 3469 */ /* MISRA Rule Dir 4.9 */

        stateChanged = TRUE;
    }
#endif /* STD_ON == CANNM_NODE_DETECTION_ENABLED */
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

#if (STD_ON == CANNM_NODE_DETECTION_ENABLED)
    if (chCfgPtr->NodeDetectionEnabled && chRTPtr->rxPduExtFlg && (CANNM_PDU_OFF != chCfgPtr->PduCbvPosition)
        && !stateChanged)
    {
        uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
        uint8 cbvDta = chRTPtr->rxPduData[cbvPos];
        /* @req SWS_CanNm_00111 */
        /*
         * If CanNmNodeDetectionEnabled is set to TRUE and Repeat
         * Message Request bit is received in the Ready Sleep State,
         * CanNm shall enter Repeat Message State.
         */
        if ((cbvDta & CANNM_CBV_BIT_RMP_MASK) > 0u)
        {
            CanNm_EntryRepeatMessageState(chIndex, FALSE);
        }
    }
#endif /* STD_ON == CANNM_NODE_DETECTION_ENABLED */

    if (chRTPtr->nmToutFlg && !stateChanged)
    {
        /* @req SWS_CanNm_00109 */
        CanNm_EnterPrepareBusSleepModeHandle(chIndex);
    }

#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
    if ((FALSE == chRTPtr->canNmTxEnable) && (CANNM_NETWORK_RELEASED == chRTPtr->netRequestStatus)
        && (FALSE == stateChanged))
    {
        /* @req SWS_CanNm_00170 note*/
        /*this function is delete in the R19-11,but I think this point
         *should reserve for wrong usage of communication control */
        CanNm_EnterPrepareBusSleepModeHandle(chIndex);
    }
#endif
}

/******************************************************************************/
/*
 * Brief               Asynchronous process received data in MainFunction
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_RxDataMainHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
/* Detection of Remote Sleep Indication Handle */
#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
    /* @req SWS_CanNm_00150 */
    /* Reset RemoteSleepInd timer */
    chRTPtr->remoteTimer = chCfgPtr->RemoteSleepIndTime;

    /* @req SWS_CanNm_00151*/
    /*
     * If an NM PDU is received in the Normal Operation State or Ready Sleep
     * State,shall notify the NM that some nodes in the cluster are not ready
     * to sleep anymore.
     */
    if (chRTPtr->remoteSleepInd
        && ((NM_STATE_NORMAL_OPERATION == chRTPtr->canNmState) || (NM_STATE_READY_SLEEP == chRTPtr->canNmState)))
    {
        chRTPtr->remoteSleepInd = FALSE;
        Nm_RemoteSleepCancellation(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
    }
#endif /* CANNM_REMOTE_SLEEP_IND_ENABLED */

    /* @req SWS_CanNm_00098 */
    /*
     * Successful reception of an NM PDU in Network Mode, the
     * NM-Timeout Timer shall be restarted.
     */
    if (NM_MODE_NETWORK == chRTPtr->cannmMode)
    {
        chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;
    }

#if ((STD_ON == CANNM_REPEAT_MSG_IND_ENABLED) && (STD_ON == CANNM_NODE_DETECTION_ENABLED))
    /* @req SWS_CanNm_00014 */
    /*
     * If CanNmNodeDetectionEnabled and CanNmRepeatMsgIndEnabled are set
     * to TRUE and Repeat Message Request bit is received, CanNm module
     * shall call the callback function Nm_RepeatMessageIndication.
     */
    if ((CANNM_PDU_OFF != chCfgPtr->PduCbvPosition) && chCfgPtr->NodeDetectionEnabled && chCfgPtr->RepeatMsgIndEnabled)
    {
        uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
        uint8 cbvDta = chRTPtr->rxPduData[cbvPos];

        if ((cbvDta & CANNM_CBV_BIT_RMP_MASK) > 0u)
        {
            Nm_RepeatMessageIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
#endif /* STD_ON == CANNM_REPEAT_MSG_IND_ENABLED && STD_ON == CANNM_NODE_DETECTION_ENABLED */

#if (STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED)
    if (CanNm_CfgPtr->ChannelConfig[chIndex].CarWakeUpRxEnabled)
    {
        CanNm_RxCarWakeUpHadle(chIndex);
    }
#endif /* STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED */

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
    if (CANNM_PDU_OFF != chCfgPtr->PduCbvPosition)
    {
        /* NM Coordinator synchronization support */
        CanNm_CoordReadyToSleepIndHandle(chIndex);
    }
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

#if (                                   \
    (STD_ON == CANNM_GLOBAL_PN_SUPPORT) \
    && ((STD_ON == CANNM_PN_ERA_CALC_ENABLED) || (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)))
    if (chCfgPtr->PnEnabled && chRTPtr->pnRxIndFlg)
    {
        /* @req SWS_CanNm_00428 @req SWS_CanNm_00429 */
        CanNm_RxPnEiraEraHandle(chIndex);
    }
#endif
}

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/******************************************************************************/
/*
 * Brief               Send message in MainFunction.The purpose of this function
 *                     is to aggregate multiple send requests,Only one frame
 *                     will be sent out when there are multiple send requests in
 *                     a MainFunction.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_SendMsgMainHandle(uint8 chIndex)
{
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];

    if (chRTPtr->sendNmMsgFlg)
    {
        chRTPtr->sendNmMsgFlg = FALSE;
        if (CanNm_SendNmPdu(chIndex) == E_OK)
        {
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->retrySendMsgFlag = FALSE;
#endif
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
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
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->immedMsgTxCnt > 0u)
            {
                chRTPtr->msgTxCylceTimer = 1u;
            }
            else
#endif
            {
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
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
#endif /* CANNM_PASSIVE_MODE_ENABLED == STD_OFF */

#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               NM Coordinator synchronization support handle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_CoordReadyToSleepIndHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(CanNm_InnerChannelType, AUTOMATIC, CANNM_APPL_DATA)
    chRTPtr = &CanNm_ChRunTime[chIndex];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint8 cbvPos = (uint8)chCfgPtr->PduCbvPosition;
    uint8 cbvDta = chRTPtr->rxPduData[cbvPos];

    if (0u == (cbvDta & CANNM_CBV_BIT_CSR_MASK))
    {
        /* @req SWS_CanNm_00348 */
        /* First reception csr=0 ,shall notify upper layer */
        if ((NM_MODE_NETWORK == chRTPtr->cannmMode) && (CANNM_CSR_IND_INDICATED == chRTPtr->coordReadyToSleepInd))
        {
            chRTPtr->coordReadyToSleepInd = CANNM_CSR_IND_CANCELLED;
            /* notify upper layer */
            Nm_CoordReadyToSleepCancellation(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
    else
    {
        /* @req SWS_CanNm_00341 */
        /* First reception csr=1,shall notify upper layer */
        if (((NM_MODE_NETWORK == chRTPtr->cannmMode) && (CANNM_CSR_IND_INIT == chRTPtr->coordReadyToSleepInd))
            || (CANNM_CSR_IND_CANCELLED == chRTPtr->coordReadyToSleepInd))
        {
            chRTPtr->coordReadyToSleepInd = CANNM_CSR_IND_INDICATED;
            /* notify upper layer */
            Nm_CoordReadyToSleepIndication(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif /* STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT */

/******************************************************************************/
/*
 * Brief               Processes when enter Bus-Sleep Mode from other state
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_EnterPrepareBusSleepModeHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];

    /* @req SWS_CanNm_00109 */
    /* NM-timeout timer expires,shall entry the Prepare Bus-sleep mode*/
    CanNm_StateChange(chIndex, NM_STATE_PREPARE_BUS_SLEEP);
    chRTPtr->cannmMode = NM_MODE_PREPARE_BUS_SLEEP;

    /* @req SWS_CanNm_00114 @req SWS_CanNm_00093 */
    Nm_PrepareBusSleepMode(CanNm_CfgPtr->ChannelConfig[chIndex].ComMNetworkHandleRef);

    /* @req SWS_CanNm_00115 */
    /*
     * If CanNmStayInPbsEnabled is disabled CanNm shall stay in the
     * Prepare Bus-Sleep Mode for a configurable amount of time
     * determined by the CanNmWaitBusSleepTime.
     */
    if (FALSE == chCfgPtr->StayInPbsEnabled)
    {
        chRTPtr->stateTimer = chCfgPtr->WaitBusSleepTime;
    }

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    /* @req SWS_CanNm_00367 */
    /* Clear activeWakeupbit in CBV */
    if (chCfgPtr->ActiveWakeupBitEnabled)
    {
        CanNm_ClrTxPduCbvBit(chIndex, CANNM_CBV_BIT_AW_MASK); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
    }
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */
}

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               inner txconf handle,call CanNm_TxConfirmation or transmit
 *                      ok
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      chIndex, Channel index in CanNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANNM_CODE) CanNm_InnerTxConfHandle(uint8 chIndex)
{
    const CanNm_ChannelConfigType* chCfgPtr = &CanNm_CfgPtr->ChannelConfig[chIndex];
    CanNm_InnerChannelType* chRTPtr = &CanNm_ChRunTime[chIndex];
    SchM_Enter_CanNm_Context();
    /* @req SWS_CanNm_00099 */
    /* Nm pdu transmit success,restart the nm-timeout timer */
    if (NM_MODE_NETWORK == chRTPtr->cannmMode)
    {
        chRTPtr->nmTimeoutTimer = chCfgPtr->TimeoutTime;
    }

    /* @req SWS_CanNm_00065 */
    chRTPtr->msgTimeoutTimer = 0u;
    chRTPtr->msgToutFlg = FALSE;
    SchM_Exit_CanNm_Context();

#if (STD_ON == CANNM_COM_USERDATA_SUPPORT)
#if (0u < CANNM_USERDATA_TX_PDU_NUM)
    if (NULL_PTR != chCfgPtr->UserDataTxPdu)
    {
        /* @req SWS_CanNm_00329 */
        /* Call PduR_CanNmTxConfirmation */
        PduR_CanNmTxConfirmation(chCfgPtr->UserDataTxPdu->UpperLayerPduId);
    }
#endif
#endif /* STD_ON == CANNM_COM_USERDATA_SUPPORT */
}
#endif

#if CANNM_DEV_ERROR_DETECT == STD_ON
static boolean CanNm_ValidateInitStatus(uint8 apiId)
{
    boolean valid = CanNm_InitStatus == CANNM_INIT;
    if (!valid)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_UNINIT);
    }
    return valid;
}

static boolean CanNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle)
{
    boolean valid = CanNm_ValidateInitStatus(apiId);
    if (valid)
    {
        if (CanNm_FindChannelIndex(networkHandle) >= CANNM_NUMBER_OF_CHANNEL)
        {
            valid = FALSE;
            (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_CHANNEL);
        }
    }

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    if (valid)
    {
        if (GetApplicationID() != ComM_GetChannelApplicationID(networkHandle))
        {
            valid = FALSE;
            (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PARTITION_CONTEXT);
        }
    }
#endif
    return valid;
}

static boolean CanNm_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = pointer != NULL_PTR;
    if (!valid)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_PARAM_POINTER);
    }
    return valid;
}

static boolean CanNm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer)
{
    return CanNm_ValidateNetworkHandle(apiId, networkHandle) && CanNm_ValidatePointer(apiId, pointer);
}

static boolean CanNm_ValidateNetworkHandlePointerAndPointer(
    uint8 apiId,
    NetworkHandleType networkHandle,
    const void* pointer1,
    const void* pointer2)
{
    return CanNm_ValidateNetworkHandleAndPointer(apiId, networkHandle, pointer1)
           && CanNm_ValidatePointer(apiId, pointer2);
}

static boolean CanNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId)
{
    boolean valid = CanNm_ValidateInitStatus(apiId);
    uint8 pduNum = 0u;

    switch (apiId)
    {
    case CANNM_SERVICE_ID_TRANSMIT:
        pduNum = CANNM_USERDATA_TX_PDU_NUM;
        break;
    case CANNM_SERVICE_ID_TRIGGERTRANSMIT:
    case CANNM_SERVICE_ID_TXCONFIRMATION:
        pduNum = CANNM_TX_PDU_NUMBER;
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
            (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PDUID);
        }
    }
    return valid;
}

static boolean CanNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr)
{
    return CanNm_ValidateTxPduId(apiId, txPduId) && CanNm_ValidatePointer(apiId, pduInfoPtr)
           && CanNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
}

static boolean CanNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean valid = CanNm_ValidateInitStatus(apiId) && CanNm_ValidatePointer(apiId, pduInfoPtr)
                    && CanNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
    if (valid)
    {
        if (rxPduId >= CANNM_RX_PDU_NUMBER)
        {
            valid = FALSE;
            (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PDUID);
        }
    }
    return valid;
}

static boolean CanNm_ValidateInit(const CanNm_ConfigType* configPtr)
{
    boolean valid = CanNm_InitStatus != CANNM_INIT;
    if (!valid)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, CANNM_SERVICE_ID_INIT, CANNM_E_ALREADY_INITIALIZED);
    }
    else
    {
        valid = CanNm_ValidatePointer(CANNM_SERVICE_ID_INIT, configPtr);
    }
    return valid;
}

static boolean CanNm_ValidateDeInit(void)
{
    boolean valid = CanNm_ValidateInitStatus(CANNM_SERVICE_ID_DEINIT);
    if (valid)
    {
        for (CanNm_ChannelIndexType index = 0u; index < CANNM_NUMBER_OF_CHANNEL; ++index)
        {
            if (CanNm_ChRunTime[index].canNmState != NM_STATE_BUS_SLEEP)
            {
                valid = FALSE;
                (void)Det_ReportError(
                    CANNM_MODULE_ID,
                    CANNM_INSTANCE_ID,
                    CANNM_SERVICE_ID_DEINIT,
                    CANNM_E_NOT_IN_BUS_SLEEP);
                break;
            }
        }
    }
    return valid;
}
#endif
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
