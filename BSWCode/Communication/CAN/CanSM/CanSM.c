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
**  FILENAME    : CanSM.c                                                     **
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
 *  V2.0.0    2020-08-17  lili.wang    R19_11 initial version.
 *  V2.0.1    2020-10-19  lili.wang    QAC check,code update
 *  V2.0.2    2021-02-02  lili.wang
 *    1.Modify the timing mechanism for MainFunction timing.
 *    2.ComM_BusSM_ModeIndication pass parameter modification.
 *  V2.0.3    2021-02-23  lili.wang
 *    1.Modification of retry mechanism after CanIf request fails
 *    2.not include FreeRTTimer.h any more
 *  V2.0.4    2021-03-23  lili.wang
 *    1.Modify the macro control conditions of the Dem parameter,
 *    and some compilation warnings
 *  V2.0.5    2021-04-23  lili.wang
 *    1.delete CanSM_TxTimeoutException macro control CANSM_PNC_SUPPORT.
 *  V2.0.6    2021-06-16  lili.wang
 *    1.In CanSM_FullCom_S_CC_Started_Wait,get T_CC_STARTED_INDICATED,call
 *    CanIf_SetPduMode(controllerID,CANIF_ONLINE);
 *  V2.0.7    2021-06-24  lili.wang
 *    1.In CanSM_FullCom_S_CC_Started,get T_CC_STARTED_INDICATED,call
 *    CanIf_SetPduMode(controllerID,CANIF_ONLINE);
 *  V2.0.8    2022-03-17  darren
 *    1. Adjust header files and interface macro switches
 *  V2.0.9    2022-04-24  darren.zhang
 *    1.Adjust the header file inclusion order
 *  V2.0.10    2022-07-26  darren.zhang
 *    1.modify memap
 *  V2.0.11    2023-01-05  darren.zhang
 *    1.support pending comm reqeust
 *  V2.0.12    2023-07-24  xiaojian.liang
 *    1. Fixed CPT-6229, CanSM_GetCurrentComMode cannot return COMM_SILENT_COMMUNICATION when bussoff occurs
 *    2. Fixed CPT-472, Transition with best effort instead of at most one transition
 *  V2.0.13    2023-08-11  xiaojian.liang
 *    1. Fixed CPT-6413, The busoff event is lost in the S_TX_OFF state and can not be automatically recovered
 *    2. Fixed CPT-6482, ModeRequestRepetitionTime, BorTimeTxEnsured, BorTimeL2 and BorTimeL1 counts incorrectly
 *  V2.0.14    2023-11-29  xiaojian.liang
 *    1. Performance optimization.
 *    2. Fixed CPT-7745, Unable to retry if CanIf_SetControllerMode return E_NOT_OK
 *  V2.0.15    2024-04-28  xiaojian.liang
 *    1. CPT-8597  Runtime error CANSM_E_MODE_REQUEST_TIMEOUT depends on CanSMDevErrorDetect.
 *    2. CPT-8723  CanSM_DeInit still detect the CANSM_NO_COMMUNICATION error if CanSMDevErrorDetect is false.
 *    3. Removed Pretended Networking.
 *    4. Compilation error if CanIfPublicTxConfirmPollingSupport is set to true.
 *    5. Support multiple partition feature.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanSM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:The exception is that the number of loops is configurable.

    \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is global configuration data(1531) and API(1532).
 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CANSM_C_AR_RELEASE_MAJOR_VERSION 4u
#define CANSM_C_AR_RELEASE_MINOR_VERSION 5u
#define CANSM_C_AR_RELEASE_PATCH_VERSION 0u
#define CANSM_C_SW_MAJOR_VERSION         2u
#define CANSM_C_SW_MINOR_VERSION         0u
#define CANSM_C_SW_PATCH_VERSION         15u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanIf.h"
#include "CanSM.h"
#include "SchM_CanSM.h"
#include "ComM_BusSM.h"
#include "BswM_CanSM.h"
#if (STD_ON == CANSM_CANNM_CONF_PN_AVA)
#include "CanNm_Cbk.h"
#endif
#include "Det.h"
#include "CanSM_Cbk.h"
#include "CanSM_TxTimeoutException.h"

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
#include "ComM.h"
#include "Os.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (CANSM_C_AR_RELEASE_MAJOR_VERSION != CANSM_H_AR_RELEASE_MAJOR_VERSION)
#error "CanSM.c : Mismatch in Specification Major Version"
#endif
#if (CANSM_C_AR_RELEASE_MINOR_VERSION != CANSM_H_AR_RELEASE_MINOR_VERSION)
#error "CanSM.c : Mismatch in Specification _MINOR Version"
#endif
#if (CANSM_C_AR_RELEASE_PATCH_VERSION != CANSM_H_AR_RELEASE_PATCH_VERSION)
#error "CanSM.c : Mismatch in Specification PATCH Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#ifndef CANSM_LOCAL
#define CANSM_LOCAL static
#endif

#ifndef CANSM_LOCAL_INLINE
#define CANSM_LOCAL_INLINE static inline
#endif

#define CANSM_UNUSED(x)          (void)(x)

#define BUS_OFF_COUNTER_UP_LIMIT 255u

#define CANSM_NETWORK_CONTROLLER(NetworkHandle_Index) \
    (CanSM_ConfigPtr->CanSMManagerNetworkRef[NetworkHandle_Index].ControllerRef)

#define CANSM_MODEREQREPEAT_MAINFUNCTIONTIME CanSM_ConfigPtr->CanSMModeRequestRepetitionTime

#if (STD_ON == CANSM_DEV_ERROR_DETECT)
CANSM_LOCAL boolean CanSM_ValidateAllNetworksNoCom(void);
#endif

#define CANSM_DET_REPORTRUNTIMEERROR(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId), (ErrorId)))

#if CANSM_MULTIPLE_CONTROLLER_SUPPORT == STD_ON
#define CANSM_FOREACH_NETWORK_CONTROLLER_START(networkIndex, controllerId)                                   \
    uint8 netWorkRefControllerNumber = CANSM_NETWORK_CONTROLLER((networkIndex))->NetWorkRefControllerNumber; \
    for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)        \
    {                                                                                                        \
        (controllerId) = CANSM_NETWORK_CONTROLLER((networkIndex))->CanSMControllerId[controllerIndex];

#define CANSM_FOREACH_NETWORK_CONTROLLER_END() }
#else
#define CANSM_FOREACH_NETWORK_CONTROLLER_START(networkIndex, controllerId) \
    do                                                                     \
    {                                                                      \
        (controllerId) = CANSM_NETWORK_CONTROLLER((networkIndex))->CanSMControllerId[0];
#define CANSM_FOREACH_NETWORK_CONTROLLER_END() \
    }                                          \
    while (0)                                  \
        ;
#endif /* CANSM_MULTIPLE_CONTROLLER_SUPPORT == STD_ON */

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef uint8 CanSM_NetworkIndexType; /* The range is at least equal to the range of type NetworkHandleType */

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"

static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Sleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Sleep_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

#if (CANSM_TRCV_NUM > 0)
static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Standby(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Standby_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);
#endif

static boolean CanSM_BsmSNotInitialized(uint8 netID);
static boolean CanSM_DeinitPnNotSupported(uint8 netID);

#if (STD_ON == CANSM_PNC_SUPPORT)
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_Clear_WUF(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_Clear_WUF_Wait(CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Standby(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Standby_Wait(CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_CC_Sleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_S_CC_Sleep_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_Check_Wflag_InCCSleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_Check_Wflag_InCCSleepWait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_CheckWflag_InNotCCSleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Pn_CheckWFlag_InNotCCSleepWait(CanSM_NetWorkRunTimeType* networkPtr);

static boolean CanSM_DeinitPnSupported(uint8 netID);
#endif

static boolean CanSM_BsmSNoCom(uint8 netID);

#if (CANSM_TRCV_NUM > 0)
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr);
#endif

static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static boolean CanSM_BsmWuValidation(uint8 netID);

#if (CANSM_TRCV_NUM > 0)
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr);
#endif
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static boolean CanSM_BsmSPreFullCom(uint8 netID);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Bus_off_Check(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static void CanSM_FullCom_S_No_Bus_Off(uint8 netID, const CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Restart_CC(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Restart_CC_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Tx_Off(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static boolean CanSM_BsmSFullCom(uint8 netID);

#if (STD_ON == CANSM_SET_BAUDRATE_API)
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_Change_BR_Sync(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr);

static boolean CanSM_BsmSChangeBaudrate(uint8 netID);
#endif

static boolean CanSM_BsmSSilentCom(uint8 netID);

static boolean CanSM_BsmSSilentComBor(uint8 netID);

static FUNC(void, CANSM_CODE) CanSM_EFullComAction(uint8 netID);

static FUNC(void, CANSM_CODE) CanSM_EFullToSilentComAction(uint8 netID);

static FUNC(void, CANSM_CODE) CanSM_EPreNoComAction(uint8 netID);

static void CanSM_EBusOffAction(uint8 netID);

static void CanSM_SetControllerModeRepeat(
    uint8 netID,
    CanSM_NetWorkRunTimeType* networkPtr,
    Can_ControllerStateType requestContorllerMode);

CANSM_LOCAL boolean CanSM_CheckNetworkAllControllerMode(uint8 netID, Can_ControllerStateType controllerMode);
CANSM_LOCAL void CanSM_ModeRequestTimeout(uint8 netID);

static FUNC(void, CANSM_CODE) CanSM_TimerHandler(CanSM_NetWorkRunTimeType* networkPtr);

CANSM_LOCAL CanSM_NetworkIndexType CanSM_FindNetworkIndex(NetworkHandleType networkHandle);

CANSM_LOCAL void CanSM_ComMModeIndication(uint8 netID, ComM_ModeType ComMode, CanSM_RequestModeType RequestedComMode);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define CANSM_START_SEC_VAR_INIT_8
#include "CanSM_MemMap.h"
static VAR(CanSM_StateType, CANSM_VAR_POWER_ON_INIT) CanSM_Status = CANSM_UNINITED;
#define CANSM_STOP_SEC_VAR_INIT_8
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_VAR_NO_INIT_PTR
#include "CanSM_MemMap.h"
static P2CONST(CanSM_ConfigType, AUTOMATIC, CANSM_CONFIG_DATA) CanSM_ConfigPtr;
#define CANSM_STOP_SEC_VAR_NO_INIT_PTR
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanSM_MemMap.h"
static VAR(CanSM_NetWorkRunTimeType, CANSM_VAR) CanSM_NetWorkRunTime[CANSM_NETWORK_NUM];
static VAR(Can_ControllerStateType, CANSM_VAR) CanSM_ControllerMode[CANIF_CANCONTROLLER_NUMBER];
#define CANSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "CanSM_MemMap.h"
#if (STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT)
static VAR(boolean, CANSM_VAR) CanSM_EcuPassive = FALSE;
#endif
#define CANSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "CanSM_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service initializes the CanSM module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr, Pointer to init structure for the post build
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CODE)
CanSM_Init(P2CONST(CanSM_ConfigType, AUTOMATIC, CANSM_CONFIG_DATA) ConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 netLoop;
    uint8 controller;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_INIT, CANSM_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        if (CANSM_UNINITED == CanSM_Status)
        {
            CanSM_Status = CANSM_INITED;
            CanSM_ConfigPtr = ConfigPtr;
            /* initialize run time structure variable for every network */
            for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
#if (STD_ON == CANSM_PNC_SUPPORT)
                if ((NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef)
                    && ((boolean)TRUE
                        == CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef->CanSMCanTrcvPnEnabled))
                {
                    CanSM_NetWorkRunTime[netLoop].curBsmState = CANSM_BSM_DEINITPNSUPPORTED;
                    CanSM_NetWorkRunTime[netLoop].deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
                }
                else
#endif /*STD_ON == CANSM_PNC_SUPPORT*/
                {
                    CanSM_NetWorkRunTime[netLoop].curBsmState = CANSM_BSM_DEINITPNNOTSUPPORTED;
                    CanSM_NetWorkRunTime[netLoop].deinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
                }
                CanSM_NetWorkRunTime[netLoop].ModeRequestRepetitionTime = 0u;
                CanSM_NetWorkRunTime[netLoop].waitCanIfIndicatedStartTime = 0u;
                CanSM_NetWorkRunTime[netLoop].busOffEventStartTime = 0u;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
                CanSM_NetWorkRunTime[netLoop].borTimeTxEnsuredTime = 0u;
#endif
                CanSM_NetWorkRunTime[netLoop].busOffCounter = 0u;
                CanSM_NetWorkRunTime[netLoop].repeatCounter = 0u;
                CanSM_NetWorkRunTime[netLoop].busOffEvent = FALSE;
                CanSM_NetWorkRunTime[netLoop].canIfIndicated = T_NO_INDICATED;
                CanSM_NetWorkRunTime[netLoop].requestComMode = CANSM_NO_REQUEST;
                CanSM_NetWorkRunTime[netLoop].curComMode = COMM_NO_COMMUNICATION;
#if (STD_ON == CANSM_SET_BAUDRATE_API)
                CanSM_NetWorkRunTime[netLoop].baudRateConfigID = 0u;
#endif
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
                CanSM_NetWorkRunTime[netLoop].busOffDelayTime = 0u;
#endif
            }
            /* PRQA S 2877 ++ */ /* MISRA Dir 4.1 */
            for (controller = 0u; controller < (uint8)CANIF_CANCONTROLLER_NUMBER; controller++)
            /* PRQA S 2877 -- */ /* MISRA Dir 4.1 */
            {
                CanSM_ControllerMode[controller] = CAN_CS_UNINIT;
            }
        }
    }
    return;
}

/**
 * @brief         This service de-initializes the CanSM module.
 * @id            0x14
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant
 */
void CanSM_DeInit(void)
{
#if CANSM_DEV_ERROR_DETECT == STD_ON
    if (CanSM_ValidateAllNetworksNoCom())
#endif
    {
        for (CanSM_NetworkIndexType networkIndex = 0u; networkIndex < CANSM_NETWORK_NUM; ++networkIndex)
        {
            /*@req <SWS_CanSM_00658>*/
            CanSM_NetWorkRunTime[networkIndex].curBsmState = CANSM_BSM_S_NOT_INITIALIZED;
        }
        CanSM_Status = CANSM_UNINITED;
    }
}

#if (STD_ON == CANSM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               This service puts out the version information of this
 *                     module
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      NA
 * Param-Name[out]     VersionInfo, Pointer to where to store the version
 *                     information of this module
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CODE)
CanSM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (NULL_PTR == VersionInfo)
    {
        (void)
            Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_GETVERSIONINFO, CANSM_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        VersionInfo->moduleID = CANSM_MODULE_ID;
        VersionInfo->sw_major_version = CANSM_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = CANSM_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = CANSM_C_SW_PATCH_VERSION;
        VersionInfo->vendorID = CANSM_VENDOR_ID;
    }
    return;
}
#endif /*STD_ON == CANSM_VERSION_INFO_API*/

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/******************************************************************************/
/*
 * Brief               This service shall start an asynchronous process to change
 *                     the baud rate for the configured CAN controllers of a
 *                     certain CAN network.Depending on necessary baud rate
 *                     modifications the controllers might have to reset.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Networks. Non reentrant for the same
 *                     Network
 * Param-Name[in]      Network, Handle of the addressed CAN network for the baud
 *                     rate change
 *                     BaudRateConfigID, a baud rate configuration by ID
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Service request accepted, setting of new baud rate
 *                     started
 *                     E_NOT_OK: Service request not accepted
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetBaudrate(NetworkHandleType Network, uint16 BaudRateConfigID)
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(CanSM_NetWorkRunTimeType, AUTOMATIC, CANSM_VAR) networkPtr;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_SETBAUDRATE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(Network);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_SETBAUDRATE,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(Network))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_SETBAUDRATE,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            networkPtr = &CanSM_NetWorkRunTime[networkIndex];
            if ((CANSM_BSM_S_FULLCOM == networkPtr->curBsmState) && (FULLCOM_S_NO_BUS_OFF == networkPtr->fullComState))
            {
                networkPtr->curBsmState = CANSM_BSM_S_CHANGE_BAUDRATE;
                networkPtr->changeBaudrateState = CHANGEBR_CHANGE_BR_SYNC;
                BswM_CanSM_CurrentState(Network, CANSM_BSWM_CHANGE_BAUDRATE);
                networkPtr->baudRateConfigID = BaudRateConfigID;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif /*STD_ON == CANSM_SET_BAUDRATE_API*/

#if (STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT)
/******************************************************************************/
/*
 * Brief               This function can be used to set all CanSM channels of
 *                     the ECU to a receive only mode. This mode will be kept
 *                     either until it is set back, or the ECU is reset.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanSM_Passive, TRUE: set all CanSM channels to passive,
 *                     i.e. receive only
 *                     FALSE: set all CanSM channels back to non-passive
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted
 *                     E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_CODE)
CanSM_SetEcuPassive(boolean CanSM_Passive)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 netLoop;
    CanIf_PduModeType controllerPduMode = CANIF_OFFLINE;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_SETECUPASSIVE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        if (((boolean)TRUE == CanSM_Passive) && ((boolean)FALSE == CanSM_EcuPassive))
        {
            CanSM_EcuPassive = TRUE;
            for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                uint8 controllerId;
                CANSM_FOREACH_NETWORK_CONTROLLER_START(netLoop, controllerId)
                {
                    (void)CanIf_GetPduMode(controllerId, &controllerPduMode);
                    if (CANIF_ONLINE == controllerPduMode)
                    {
                        (void)CanIf_SetPduMode(controllerId, CANIF_TX_OFFLINE_ACTIVE);
                    }
                }
                CANSM_FOREACH_NETWORK_CONTROLLER_END()
            }
        }
        else if (((boolean)FALSE == CanSM_Passive) && ((boolean)TRUE == CanSM_EcuPassive))
        {
            CanSM_EcuPassive = FALSE;
            for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                uint8 controllerId;
                CANSM_FOREACH_NETWORK_CONTROLLER_START(netLoop, controllerId)
                {
                    (void)CanIf_GetPduMode(controllerId, &controllerPduMode);
                    if (CANIF_TX_OFFLINE_ACTIVE == controllerPduMode)
                    {
                        (void)CanIf_SetPduMode(controllerId, CANIF_ONLINE);
                    }
                }
                CANSM_FOREACH_NETWORK_CONTROLLER_END()
            }
        }
        else
        {
            /*nothing*/
        }
        ret = E_OK;
    }
    return ret;
}
#endif /*STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT*/

static FUNC(void, CANSM_CODE) CanSM_TimerHandler(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (networkPtr->ModeRequestRepetitionTime > 0u)
    {
        networkPtr->ModeRequestRepetitionTime--;
    }
    if (networkPtr->waitCanIfIndicatedStartTime > 0u)
    {
        networkPtr->waitCanIfIndicatedStartTime--;
    }
}

CANSM_LOCAL CanSM_NetworkIndexType CanSM_FindNetworkIndex(NetworkHandleType networkHandle)
{
    CanSM_NetworkIndexType networkIndex = 0u;
    for (; networkIndex < CANSM_NETWORK_NUM; ++networkIndex)
    {
        if (networkHandle == CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex].CanSMComMNetworkHandleRef)
        {
            break;
        }
    }
    return networkIndex;
}

CANSM_LOCAL void CanSM_ComMModeIndication(uint8 netID, ComM_ModeType ComMode, CanSM_RequestModeType RequestedComMode)
{
    if (RequestedComMode != CANSM_NO_REQUEST)
    {
        ComM_BusSM_ModeIndication(CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef, ComMode);
    }
}
/******************************************************************************/
/*
 * Brief               Scheduled function of the CanSM
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NA
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
void CanSM_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    static boolean (*const CanSM_DoBehaviors[])(uint8 netID) = {
        CanSM_BsmSNotInitialized,
        CanSM_DeinitPnNotSupported,
#if (STD_ON == CANSM_PNC_SUPPORT)
        CanSM_DeinitPnSupported,
#endif
        CanSM_BsmSNoCom,
        CanSM_BsmWuValidation,
        CanSM_BsmSPreFullCom,
        CanSM_BsmSFullCom,
#if (STD_ON == CANSM_SET_BAUDRATE_API)
        CanSM_BsmSChangeBaudrate,
#endif
        CanSM_BsmSSilentCom,
        CanSM_BsmSSilentComBor,
    };

    if (CANSM_UNINITED == CanSM_Status)
    {
        return;
    }

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */

    for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        if (ComM_GetChannelApplicationID(CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef)
            != applicationID)
        {
            continue;
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */

        CanSM_NetWorkRunTimeType* netRTPtr = &CanSM_NetWorkRunTime[netLoop];
        CanSM_TimerHandler(netRTPtr);
        while (CanSM_DoBehaviors[netRTPtr->curBsmState](netLoop))
        {
            ;
        }
    }
}

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType CanSM_RequestComMode(NetworkHandleType network, ComM_ModeType comMMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_REQUESTCOMMODE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(network);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_REQUESTCOMMODE,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(network))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_REQUESTCOMMODE,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[networkIndex];

            switch (comMMode)
            {
            case COMM_NO_COMMUNICATION:
                if ((CANSM_BSM_S_FULLCOM == networkPtr->curBsmState)
#if (STD_ON == CANSM_SET_BAUDRATE_API)
                    || (CANSM_BSM_S_CHANGE_BAUDRATE == networkPtr->curBsmState)
#endif
                    || (CANSM_BSM_S_SILENTCOM == networkPtr->curBsmState)
                    || (CANSM_BSM_S_SILENTCOM_BOR == networkPtr->curBsmState))
                {
                    networkPtr->requestComMode = CANSM_NO_COMMUNICATION;
                    ret = E_OK;
                }
                break;
            case COMM_SILENT_COMMUNICATION:
                if (CANSM_BSM_S_FULLCOM == networkPtr->curBsmState)
                {
                    networkPtr->requestComMode = CANSM_SILENT_COMMUNICATION;
                    CanSM_EFullToSilentComAction(networkIndex);
                    ret = E_OK;
                }
#if (STD_ON == CANSM_SET_BAUDRATE_API)
                else if (CANSM_BSM_S_CHANGE_BAUDRATE == networkPtr->curBsmState)
                {
                    networkPtr->requestComMode = CANSM_SILENT_COMMUNICATION;
                    ret = E_OK;
                }
#endif
                else
                {
                    ;
                }
                break;
            case COMM_FULL_COMMUNICATION:
                if (CANSM_BSM_S_SILENTCOM == networkPtr->curBsmState)
                {
                    networkPtr->requestComMode = CANSM_FULL_COMMUNICATION;
                    CanSM_EFullComAction(networkIndex);
                    ret = E_OK;
                }
                else if (
                    (CANSM_BSM_S_NOCOM == networkPtr->curBsmState)
                    || (CANSM_BSM_WUVALIDATION == networkPtr->curBsmState) ||
#if (STD_ON == CANSM_PNC_SUPPORT)
                    (CANSM_BSM_DEINITPNSUPPORTED == networkPtr->curBsmState) ||
#endif
                    (CANSM_BSM_DEINITPNNOTSUPPORTED == networkPtr->curBsmState))
                {
                    networkPtr->requestComMode = CANSM_FULL_COMMUNICATION;
                    ret = E_OK;
                }
                else
                {
                    /* do nothing */;
                }
                break;
            default:
                /* do nothing */
                break;
            }
        }
    }
    return ret;
}

/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType CanSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* comMModePtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_GETCURRENTCOMMODE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if (NULL_PTR == comMModePtr)
    {
        (void)Det_ReportError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_GETCURRENTCOMMODE,
            CANSM_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(network);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_GETCURRENTCOMMODE,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(network))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_GETCURRENTCOMMODE,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            *comMModePtr = CanSM_NetWorkRunTime[networkIndex].curComMode;
            ret = E_OK;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function shall be called by EcuM when a wakeup source
 *                     shall be started.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      network, Affected CAN network
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_ECUM_CODE)
CanSM_StartWakeupSource(NetworkHandleType network) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    CanSM_NetWorkRunTimeType* networkPtr;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_STARTWAKEUPSOURCE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(network);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_STARTWAKEUPSOURCE,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(network))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_STARTWAKEUPSOURCE,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            networkPtr = &CanSM_NetWorkRunTime[networkIndex];
            if ((CANSM_BSM_DEINITPNNOTSUPPORTED <= networkPtr->curBsmState)
                && (networkPtr->curBsmState <= CANSM_BSM_S_NOCOM))
            {
                networkPtr->curBsmState = CANSM_BSM_WUVALIDATION;
#if (CANSM_TRCV_NUM > 0)
                if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex].TrcvRef)
                {
                    networkPtr->wuValidationState = WUVALIDATION_S_TRCV_NORMAL;
                }
                else
#endif
                {
                    networkPtr->wuValidationState = WUVALIDATION_S_CC_STOPPED;
                }
                networkPtr->repeatCounter = 0u;
                ret = E_OK;
            }
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function shall be called by EcuM when a wakeup source
 *                     shall be stopped
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      network, Affected CAN network
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              E_OK: Request accepted E_NOT_OK: Request denied
 */
/******************************************************************************/
FUNC(Std_ReturnType, CANSM_ECUM_CODE)
CanSM_StopWakeupSource(NetworkHandleType network) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    CanSM_NetWorkRunTimeType* networkPtr;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_STOPWAKEUPSOURCE, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(network);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_STOPWAKEUPSOURCE,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(network))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_STOPWAKEUPSOURCE,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            networkPtr = &CanSM_NetWorkRunTime[networkIndex];
            if (CANSM_BSM_WUVALIDATION == networkPtr->curBsmState)
            {
#if (STD_ON == CANSM_PNC_SUPPORT)
                if ((NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex].TrcvRef)
                    && ((boolean)TRUE
                        == CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex].TrcvRef->CanSMCanTrcvPnEnabled))
                {
                    networkPtr->curBsmState = CANSM_BSM_DEINITPNSUPPORTED;
                    networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
                }
                else
#endif
                {
                    networkPtr->curBsmState = CANSM_BSM_DEINITPNNOTSUPPORTED;
                    networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
                }
                networkPtr->repeatCounter = 0u;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_CONTROLLERBUSOFF_CALLBACK_CODE
#include "CanSM_MemMap.h"
void CanSM_ControllerBusOff(uint8 ControllerId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_CONTROLLERBUSOFF, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        boolean findController = FALSE;
        /* PRQA S 2877 ++ */ /* MISRA Dir 4.1 */
        for (uint8 netLoop = 0u; (netLoop < CANSM_NETWORK_NUM) && !findController; netLoop++)
        /* PRQA S 2877 -- */ /* MISRA Dir 4.1 */
        {
            uint8 controllerId;
            CANSM_FOREACH_NETWORK_CONTROLLER_START(netLoop, controllerId)
            {
                if (ControllerId == controllerId)
                {
                    findController = TRUE;
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
                    {
                        CanSM_ControllerMode[controllerId] = CAN_CS_STOPPED;
                        CanSM_NetWorkRunTime[netLoop].busOffEvent = TRUE;
                    }
                    break;
                }
            }
            CANSM_FOREACH_NETWORK_CONTROLLER_END()
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (!findController)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                CANSM_E_PARAM_CONTROLLER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_CONTROLLERBUSOFF_CALLBACK_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_CONTROLLERMODEINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void CanSM_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 networkIndex;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_CONTROLLERMODEINDICATION,
            CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        boolean findController = FALSE;
        /* PRQA S 2877 ++ */ /* MISRA Dir 4.1 */
        for (networkIndex = 0u; (networkIndex < CANSM_NETWORK_NUM) && !findController; ++networkIndex)
        /* PRQA S 2877 -- */ /* MISRA Dir 4.1 */
        {
            uint8 controllerId;
            CANSM_FOREACH_NETWORK_CONTROLLER_START(networkIndex, controllerId)
            {
                if (ControllerId == controllerId)
                {
                    findController = TRUE;
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CONTROLLERMODEINDICATION,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
                    {
                        CanSM_ControllerMode[ControllerId] = ControllerMode;
                    }
                    break;
                }
            }
            CANSM_FOREACH_NETWORK_CONTROLLER_END()
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (!findController)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONTROLLERMODEINDICATION,
                CANSM_E_PARAM_CONTROLLER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_CONTROLLERMODEINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_TRANSCEIVERMODEINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback shall notify the CanSM module about a CAN
 *                     transceiver mode change.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      TransceiverId, CAN transceiver, whose mode has changed
 *                     TransceiverMode, Notified CAN transceiver mode
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void CanSM_TransceiverModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    CanSM_NetWorkRunTimeType* networkPtr = NULL_PTR;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION,
            CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        /* PRQA S 2877 ++ */ /* MISRA Dir 4.1 */
        for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        /* PRQA S 2877 -- */ /* MISRA Dir 4.1 */
        {
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef)
            {
                if (TransceiverId == CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef->CanSMTransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
                    {
                        networkPtr = &CanSM_NetWorkRunTime[netLoop];
                    }
                    break;
                }
            }
        }

        if (networkPtr != NULL_PTR)
        {
            switch (TransceiverMode)
            {
            case CANTRCV_TRCVMODE_NORMAL:
                networkPtr->canIfIndicated = T_TRCV_NORMAL_INDICATED;
                break;
            case CANTRCV_TRCVMODE_STANDBY:
                networkPtr->canIfIndicated = T_TRCV_STANDBY_INDICATED;
                break;
            default:
                /*This logic does not require any processing*/
                break;
            }
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_TRANSCEIVERMODEINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function shall notify the CanSM module, that the
 *                     CanNm has detected for the affected partial CAN network
 *                     CAN network a tx timeout exception, which shall be
 *                     recovered within the respective network state machine of
 *                     the CanSM module.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel, Affected CAN network
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_TXTIMEOUTEXCEPTION_CODE)
CanSM_TxTimeoutException(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    CanSM_NetWorkRunTimeType* networkPtr;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        CanSM_NetworkIndexType networkIndex = CanSM_FindNetworkIndex(Channel);
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (networkIndex >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION,
                CANSM_E_INVALID_NETWORK_HANDLE);
        }
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        else if (GetApplicationID() != ComM_GetChannelApplicationID(Channel))
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION,
                CANSM_E_INVALID_PARTITION_CONTEXT);
        }
#endif /* CANSM_MULTIPLE_PARTITION_USED == STD_ON */
        else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
        {
            networkPtr = &CanSM_NetWorkRunTime[networkIndex];
            if ((CANSM_BSM_S_FULLCOM == networkPtr->curBsmState) && (FULLCOM_S_NO_BUS_OFF == networkPtr->fullComState))
            {
                networkPtr->fullComState = FULLCOM_S_CC_STOPPED;
                networkPtr->repeatCounter = 0u;
            }
        }
    }
}
#define CANSM_STOP_SEC_CANSM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#include "CanSM_MemMap.h"

#if (STD_ON == CANSM_PNC_SUPPORT)
#define CANSM_START_SEC_CANSM_CLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback function shall indicate the CanIf_ClearTrcvWufFlag
 *                     API process end for the notified CAN Transceiver.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      Transceiver, Requested Transceiver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CLEARTRCVWUFFLAGINDICATION_CODE)
CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    CanSM_NetWorkRunTimeType* networkPtr = NULL_PTR;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION,
            CANSM_E_UNINIT);
    }
    else
#endif
    {
        for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef)
            {
                if (Transceiver == CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef->CanSMTransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
                    {
                        networkPtr = &CanSM_NetWorkRunTime[netLoop];
                    }
                    break;
                }
            }
        }
        if (networkPtr != NULL_PTR)
        {
            networkPtr->canIfIndicated = T_CLEAR_WUF_INDICATED;
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_CLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback function indicates the CheckTransceiverWakeFlag
 *                     API process end for the notified CAN Transceiver
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different CAN Transceivers
 * Param-Name[in]      Transceiver, Requested Transceiver
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_CODE)
CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    CanSM_NetWorkRunTimeType* networkPtr = NULL_PTR;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)Det_ReportError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION,
            CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef)
            {
                if (Transceiver == CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef->CanSMTransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
                    {
                        networkPtr = &CanSM_NetWorkRunTime[netLoop];
                    }
                    break;
                }
            }
        }
        if (networkPtr != NULL_PTR)
        {
            networkPtr->canIfIndicated = T_CHECK_WFLAG_INDICATED;
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_CALLBACK_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CANSM_CONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback function indicates that the transceiver is
 *                     running in PN communication mode
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TransceiverId, CAN transceiver, which was checked for PN
 *                     availability
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
FUNC(void, CANSM_CONFIRMPNAVAILABILITY_CODE)
CanSM_ConfirmPnAvailability(uint8 TransceiverId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 netLoop;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (CANSM_UNINITED == CanSM_Status)
    {
        (void)
            Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY, CANSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((boolean)TRUE == detNoErr)
#endif
    {
        for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef)
            {
                if (TransceiverId == CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].TrcvRef->CanSMTransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID()
                        != ComM_GetChannelApplicationID(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef))
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON */
#if CANSM_CANNM_CONF_PN_AVA == STD_ON
                    {
                        CanNm_ConfirmPnAvailability(
                            CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop].CanSMComMNetworkHandleRef);
                    }
#endif /* CANSM_CANNM_CONF_PN_AVA == STD_ON */
                    break;
                }
            }
        }

#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (netLoop >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}
#define CANSM_STOP_SEC_CANSM_CONFIRMPNAVAILABILITY_CALLBACK_CODE
#include "CanSM_MemMap.h"
#endif /*STD_ON == CANSM_PNC_SUPPORT*/

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter DEINITPNNOT_S_CC_STOPPED_WAIT state*/
    networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_SLEEP;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_CC_SLEEP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Sleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_SLEEP);

    /*enter DEINITPNNOT_S_CC_SLEEP_WAIT state*/
    networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_SLEEP_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_CC_SLEEP_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_CC_Sleep_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    ComM_ModeType ComMode;
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_SLEEP))
    {
#if (CANSM_TRCV_NUM > 0)
        if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef)
        {
            networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_NORMAL;
        }
        else
#endif
        {
            networkPtr->curBsmState = CANSM_BSM_S_NOCOM;
            ComMode = COMM_NO_COMMUNICATION;
            /*do E_NOCOM action*/
            CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
        }
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_SLEEP_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_SLEEP;
        }
    }
}

#if (CANSM_TRCV_NUM > 0)
/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_TRCV_NORMAL
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_NORMAL);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }

    /*enter DEINITPNNOT_S_TRCV_STANDBY state*/
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPNNOT_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_NORMAL_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_TRCV_NORMAL_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_NORMAL;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_TRCV_STANDBY
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Standby(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    ComM_ModeType ComMode;

    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_STANDBY);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }

    /*enter CANSM_BSM_S_NOCOM state*/
    if (T_TRCV_STANDBY_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->curBsmState = CANSM_BSM_S_NOCOM;
        ComMode = COMM_NO_COMMUNICATION;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPNNOT_S_TRCV_STANDBY_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPNNOT_S_TRCV_STANDBY_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_PnNot_Trcv_Standby_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    ComM_ModeType ComMode;
    if (T_TRCV_STANDBY_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->curBsmState = CANSM_BSM_S_NOCOM;
        ComMode = COMM_NO_COMMUNICATION;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*CANSM_BSM_T_TRCV_STANDBY_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY;
        }
    }
}
#endif

static boolean CanSM_BsmSNotInitialized(uint8 netID)
{
    CANSM_UNUSED(netID);
    return FALSE;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DeinitPnNotSupported
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_DeinitPnNotSupported(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_DeinitPnNotSupportedStateType lastState = networkPtr->deinitPnNotSupportedState;

    switch (networkPtr->deinitPnNotSupportedState)
    {
    case DEINITPNNOT_S_CC_STOPPED:
        CanSM_PnNot_CC_Stopped(netID, networkPtr);
        break;
    case DEINITPNNOT_S_CC_STOPPED_WAIT:
        /*enter DEINITPNNOT_S_CC_SLEEP state*/
        CanSM_PnNot_CC_Stopped_Wait(netID, networkPtr);
        break;
    case DEINITPNNOT_S_CC_SLEEP:
        CanSM_PnNot_CC_Sleep(netID, networkPtr);

        break;
    case DEINITPNNOT_S_CC_SLEEP_WAIT:
        /*enter DEINITPNNOT_S_TRCV_NORMAL or CANSM_BSM_S_NOCOM state*/
        CanSM_PnNot_CC_Sleep_Wait(netID, networkPtr);
        break;
#if (CANSM_TRCV_NUM > 0)
    case DEINITPNNOT_S_TRCV_NORMAL:
        CanSM_PnNot_Trcv_Normal(netID, networkPtr);
        break;
    case DEINITPNNOT_S_TRCV_NORMAL_WAIT:
        /*enter DEINITPNNOT_S_TRCV_STANDBY state*/
        CanSM_PnNot_Trcv_Normal_Wait(networkPtr);
        break;
    case DEINITPNNOT_S_TRCV_STANDBY:
        CanSM_PnNot_Trcv_Standby(netID, networkPtr);
        break;
    case DEINITPNNOT_S_TRCV_STANDBY_WAIT:
        /*enter CANSM_BSM_S_NOCOM state*/
        CanSM_PnNot_Trcv_Standby_Wait(netID, networkPtr);
        break;
#endif
    default:
        /*This logic does not require any processing*/
        break;
    }
    return (networkPtr->deinitPnNotSupportedState != lastState)
           || (networkPtr->curBsmState != CANSM_BSM_DEINITPNNOTSUPPORTED);
}

#if (STD_ON == CANSM_PNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_PN_CLEAR_WUF
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_Clear_WUF(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_ClearTrcvWufFlag(CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter DEINITPN_S_PN_CC_STOPPED state*/
    if (T_CLEAR_WUF_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CC_STOPPED;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_PN_CLEAR_WUF_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_PN_CLEAR_WUF_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_Clear_WUF_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_CLEAR_WUF_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CC_STOPPED;
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CLEAR_WUF_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_PN_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter DEINITPN_S_CC_STOPPED_WAIT state*/
    networkPtr->deinitPnSupportedState = DEINITPN_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Pn_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_NORMAL;
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_TRCV_NORMAL
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_NORMAL);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter DEINITPN_S_TRCV_STANDBY state*/
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_STANDBY;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_NORMAL_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_TRCV_NORMAL_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_STANDBY;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_NORMAL;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_TRCV_STANDBY
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Standby(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_STANDBY);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            networkPtr->repeatCounter++;
        }
    }
    /*enter DEINITPN_S_CC_SLEEP state*/
    if (T_TRCV_STANDBY_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_CC_SLEEP;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_TRCV_STANDBY_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_STANDBY_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_TRCV_STANDBY_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_Trcv_Standby_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_TRCV_STANDBY_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_CC_SLEEP;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*CANSM_BSM_T_TRCV_STANDBY_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnSupportedState = DEINITPN_S_TRCV_STANDBY;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CC_SLEEP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_CC_Sleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_SLEEP);

    /*enter DEINITPN_S_CC_SLEEP_WAIT state*/
    networkPtr->deinitPnSupportedState = DEINITPN_S_CC_SLEEP_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CC_SLEEP_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_S_CC_Sleep_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_SLEEP))
    {
        networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*CANSM_BSM_T_CC_SLEEP_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP;
            networkPtr->repeatCounter = 0u;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_Check_Wflag_InCCSleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    ComM_ModeType ComMode;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_CheckTrcvWakeFlag(CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter CANSM_BSM_S_NOCOM state*/
    if (T_CHECK_WFLAG_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->curBsmState = CANSM_BSM_S_NOCOM;
        ComMode = COMM_NO_COMMUNICATION;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_Check_Wflag_InCCSleepWait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    ComM_ModeType ComMode;
    if (T_CHECK_WFLAG_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->curBsmState = CANSM_BSM_S_NOCOM;
        ComMode = COMM_NO_COMMUNICATION;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CHECK_WFLAG_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_CheckWflag_InNotCCSleep(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_CheckTrcvWakeFlag(CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter DEINITPN_S_PN_CLEAR_WUF state*/
    if (T_CHECK_WFLAG_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
        networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Pn_CheckWFlag_InNotCCSleepWait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_CHECK_WFLAG_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CHECK_WFLAG_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->deinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in DeinitPnSupported
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_DeinitPnSupported(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_DeinitPnSupportedStateType lastState = networkPtr->deinitPnSupportedState;

    switch (networkPtr->deinitPnSupportedState)
    {
    case DEINITPN_S_PN_CLEAR_WUF:
        CanSM_Pn_S_Pn_Clear_WUF(netID, networkPtr);
        break;
    case DEINITPN_S_PN_CLEAR_WUF_WAIT:
        /*enter DEINITPN_S_PN_CC_STOPPED state*/
        CanSM_Pn_S_Pn_Clear_WUF_Wait(networkPtr);
        break;
    case DEINITPN_S_PN_CC_STOPPED:
        CanSM_Pn_S_Pn_CC_Stopped(netID, networkPtr);
        break;
    case DEINITPN_S_CC_STOPPED_WAIT:
        /*enter DEINITPN_S_TRCV_NORMAL state*/
        CanSM_Pn_S_Pn_CC_Stopped_Wait(netID, networkPtr);
        break;
    case DEINITPN_S_TRCV_NORMAL:
        CanSM_Pn_S_Trcv_Normal(netID, networkPtr);
        break;
    case DEINITPN_S_TRCV_NORMAL_WAIT:
        /*enter DEINITPN_S_TRCV_STANDBY state*/
        CanSM_Pn_S_Trcv_Normal_Wait(networkPtr);
        break;
    case DEINITPN_S_TRCV_STANDBY:
        CanSM_Pn_S_Trcv_Standby(netID, networkPtr);
        break;
    case DEINITPN_S_TRCV_STANDBY_WAIT:
        /*enter DEINITPN_S_CC_SLEEP state*/
        CanSM_Pn_S_Trcv_Standby_Wait(networkPtr);
        break;
    case DEINITPN_S_CC_SLEEP:
        CanSM_Pn_S_CC_Sleep(netID, networkPtr);
        break;
    case DEINITPN_S_CC_SLEEP_WAIT:
        /*enter DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP state*/
        CanSM_Pn_S_CC_Sleep_Wait(netID, networkPtr);
        break;
    case DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP:
        CanSM_Pn_Check_Wflag_InCCSleep(netID, networkPtr);
        break;
    case DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT:
        /*enter CANSM_BSM_S_NOCOM state*/
        CanSM_Pn_Check_Wflag_InCCSleepWait(netID, networkPtr);
        break;
    case DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP:
        CanSM_Pn_CheckWflag_InNotCCSleep(netID, networkPtr);
        break;
    case DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT:
        /*enter DEINITPN_S_PN_CLEAR_WUF state*/
        CanSM_Pn_CheckWFlag_InNotCCSleepWait(networkPtr);
        break;
    default:
        /*nothing*/
        break;
    }
    return (networkPtr->deinitPnSupportedState != lastState)
           || (networkPtr->curBsmState != CANSM_BSM_DEINITPNSUPPORTED);
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_NoCom
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSNoCom(uint8 netID)
{
    if (CANSM_FULL_COMMUNICATION == CanSM_NetWorkRunTime[netID].requestComMode)
    {
        /*enter CANSM_BSM_S_PRE_FULLCOM state*/
        CanSM_NetWorkRunTime[netID].curBsmState = CANSM_BSM_S_PRE_FULLCOM;
#if (CANSM_TRCV_NUM > 0)
        /*enter sub PREFULLCOM_S_TRCV_NORMAL state*/
        if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef)
        {
            CanSM_NetWorkRunTime[netID].preFullComState = PREFULLCOM_S_TRCV_NORMAL;
            CanSM_NetWorkRunTime[netID].repeatCounter = 0u;
        }
        /*enter sub PREFULLCOM_S_CC_STOPPED state*/
        else
#endif
        {
            CanSM_NetWorkRunTime[netID].preFullComState = PREFULLCOM_S_CC_STOPPED;
            CanSM_NetWorkRunTime[netID].repeatCounter = 0u;
        }
    }
    return CanSM_NetWorkRunTime[netID].curBsmState != CANSM_BSM_S_NOCOM;
}

#if (CANSM_TRCV_NUM > 0)
/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_TRCV_NORMAL
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_NORMAL);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter WUVALIDATION_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->wuValidationState = WUVALIDATION_S_CC_STOPPED;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter WUVALIDATION_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->wuValidationState = WUVALIDATION_S_TRCV_NORMAL_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_TRCV_NORMAL_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->wuValidationState = WUVALIDATION_S_CC_STOPPED;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->wuValidationState = WUVALIDATION_S_TRCV_NORMAL;
        }
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter WUVALIDATION_S_CC_STOPPED_WAIT state*/
    networkPtr->wuValidationState = WUVALIDATION_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->wuValidationState = WUVALIDATION_S_CC_STARTED;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->wuValidationState = WUVALIDATION_S_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_CC_STARTED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

    /*enter WUVALIDATION_S_CC_STARTED_WAIT state*/
    networkPtr->wuValidationState = WUVALIDATION_S_CC_STARTED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in WUVALIDATION_S_CC_STARTED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_WUVAL_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
    {
        networkPtr->wuValidationState = WUVALIDATION_WAIT_WUVALIDATION_LEAVE;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->wuValidationState = WUVALIDATION_S_CC_STARTED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_WuValidation
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmWuValidation(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_WUValidationStateType lastState = networkPtr->wuValidationState;

    if (CANSM_FULL_COMMUNICATION == networkPtr->requestComMode)
    {
        /*enter CANSM_BSM_S_PRE_FULLCOM state*/
        networkPtr->curBsmState = CANSM_BSM_S_PRE_FULLCOM;
#if (CANSM_TRCV_NUM > 0)
        /*enter sub PREFULLCOM_S_TRCV_NORMAL state*/
        if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef)
        {
            networkPtr->preFullComState = PREFULLCOM_S_TRCV_NORMAL;
            networkPtr->repeatCounter = 0u;
        }
        /*enter sub PREFULLCOM_S_CC_STOPPED state*/
        else
#endif
        {
            networkPtr->preFullComState = PREFULLCOM_S_CC_STOPPED;
            networkPtr->repeatCounter = 0u;
        }
    }
    else
    {
        switch (networkPtr->wuValidationState)
        {
#if (CANSM_TRCV_NUM > 0)
        case WUVALIDATION_S_TRCV_NORMAL:
            CanSM_WUVAL_S_Trcv_Normal(netID, networkPtr);
            break;
        case WUVALIDATION_S_TRCV_NORMAL_WAIT:
            /*enter WUVALIDATION_S_CC_STOPPED state*/
            CanSM_WUVAL_S_Trcv_Normal_Wait(networkPtr);
            break;
#endif
        case WUVALIDATION_S_CC_STOPPED:
            /*set all controllers of the network to request mode repeat*/
            CanSM_WUVAL_S_CC_Stopped(netID, networkPtr);
            break;
        case WUVALIDATION_S_CC_STOPPED_WAIT:
            /*enter WUVALIDATION_S_CC_STARTED state*/
            CanSM_WUVAL_S_CC_Stopped_Wait(netID, networkPtr);
            break;
        case WUVALIDATION_S_CC_STARTED:
            CanSM_WUVAL_S_CC_Started(netID, networkPtr);
            break;
        case WUVALIDATION_S_CC_STARTED_WAIT:
            /*enter WUVALIDATION_WAIT_WUVALIDATION_LEAVE state*/
            CanSM_WUVAL_S_CC_Started_Wait(netID, networkPtr);
            break;
        default:
            /*WUVALIDATION_WAIT_WUVALIDATION_LEAVE state will do nothing*/
            break;
        }
    }
    return (networkPtr->wuValidationState != lastState) || (networkPtr->curBsmState != CANSM_BSM_WUVALIDATION);
}

#if (CANSM_TRCV_NUM > 0)
/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_TRCV_NORMAL
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_Trcv_Normal(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == networkPtr->ModeRequestRepetitionTime)
        && (networkPtr->repeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMTransceiverId,
            CANTRCV_TRCVMODE_NORMAL);
        if ((uint8)E_NOT_OK == ret)
        {
            networkPtr->ModeRequestRepetitionTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
        }
        networkPtr->repeatCounter++;
    }
    /*enter PREFULLCOM_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->preFullComState = PREFULLCOM_S_CC_STOPPED;
        networkPtr->repeatCounter = 0u;
        networkPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (networkPtr->repeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            networkPtr->repeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR
                    != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->ModeReqTimeoutPara,
                        DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter PREFULLCOM_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                networkPtr->preFullComState = PREFULLCOM_S_TRCV_NORMAL_WAIT;
                networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_TRCV_NORMAL_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* networkPtr)
{
    /*enter PREFULLCOM_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == networkPtr->canIfIndicated)
    {
        networkPtr->preFullComState = PREFULLCOM_S_CC_STOPPED;
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            networkPtr->preFullComState = PREFULLCOM_S_TRCV_NORMAL;
        }
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter PREFULLCOM_S_CC_STOPPED_WAIT state*/
    networkPtr->preFullComState = PREFULLCOM_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*enter PREFULLCOM_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->preFullComState = PREFULLCOM_S_CC_STARTED;
        networkPtr->waitCanIfIndicatedStartTime = 0u;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->preFullComState = PREFULLCOM_S_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_CC_STARTED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

    /*enter PREFULLCOM_S_CC_STARTED_WAIT state*/
    networkPtr->preFullComState = PREFULLCOM_S_CC_STARTED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in PREFULLCOM_S_CC_STARTED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_Prefullcom_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*do E_FULL_COM action,enter CANSM_BSM_S_FULLCOM,sub FULLCOM_S_BUS_OFF_CHECK state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
    {
        CanSM_EFullComAction(netID);
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->preFullComState = PREFULLCOM_S_CC_STARTED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_Pre_FullCom
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSPreFullCom(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_PreFullComStateType lastState = networkPtr->preFullComState;

    switch (networkPtr->preFullComState)
    {
#if (CANSM_TRCV_NUM > 0)
    case PREFULLCOM_S_TRCV_NORMAL:
        CanSM_Prefullcom_S_Trcv_Normal(netID, networkPtr);
        break;
    case PREFULLCOM_S_TRCV_NORMAL_WAIT:
        CanSM_Prefullcom_S_Trcv_Normal_Wait(networkPtr);
        break;
#endif
    case PREFULLCOM_S_CC_STOPPED:
        CanSM_Prefullcom_S_CC_Stopped(netID, networkPtr);
        break;
    case PREFULLCOM_S_CC_STOPPED_WAIT:
        CanSM_Prefullcom_S_CC_Stopped_Wait(netID, networkPtr);
        break;
    case PREFULLCOM_S_CC_STARTED:
        CanSM_Prefullcom_S_CC_Started(netID, networkPtr);
        break;
    case PREFULLCOM_S_CC_STARTED_WAIT:
        CanSM_Prefullcom_S_CC_Started_Wait(netID, networkPtr);
        break;
    default:
        /*This logic does not require any processing*/
        break;
    }
    return (networkPtr->preFullComState != lastState) || (networkPtr->curBsmState != CANSM_BSM_S_PRE_FULLCOM);
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_BUS_OFF_CHECK
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Bus_off_Check(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    if (networkPtr->busOffEvent)
    {
        CanSM_EBusOffAction(netID);
    }
    else
    {
        boolean busOffPassive = TRUE;

#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
        if (!CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMBorTxConfirmationPolling)
#endif
        {
            if (networkPtr->borTimeTxEnsuredTime > 0u)
            {
                networkPtr->borTimeTxEnsuredTime--;
            }
            /*the time duration since the effect E_TX_ON is greater or equal the configuration parameter
             * CANSM_BOR_TIME_TX_ENSURED*/
            if (networkPtr->borTimeTxEnsuredTime != 0u)
            {
                busOffPassive = FALSE;
            }
        }
#endif

#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
        if (CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMBorTxConfirmationPolling)
#endif
        {
            uint8 controllerId;
            CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
            {
                if (CANIF_TX_RX_NOTIFICATION != CanIf_GetTxConfirmationState(controllerId))
                {
                    busOffPassive = FALSE;
                    break;
                }
            }
            CANSM_FOREACH_NETWORK_CONTROLLER_END()
        }
#endif
        if (busOffPassive)
        {
#if (STD_ON == CANSM_DEM_SUPPORT)
            if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
            {
                if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->BusOffPara)
                {
                    Dem_ReportErrorStatus(
                        *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->BusOffPara,
                        DEM_EVENT_STATUS_PASSED);
                }
            }
#endif
            networkPtr->fullComState = FULLCOM_S_NO_BUS_OFF;
            networkPtr->busOffCounter = 0u;
        }
    }
}

static void CanSM_FullCom_S_No_Bus_Off(uint8 netID, const CanSM_NetWorkRunTimeType* networkPtr)
{
    if (networkPtr->busOffEvent)
    {
        CanSM_EBusOffAction(netID);
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter FULLCOM_S_CC_STOPPED_WAIT state*/
    networkPtr->fullComState = FULLCOM_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*enter FULLCOM_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->fullComState = FULLCOM_S_CC_STARTED;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->fullComState = FULLCOM_S_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_CC_STARTED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

    /*enter FULLCOM_S_CC_STARTED_WAIT state*/
    networkPtr->fullComState = FULLCOM_S_CC_STARTED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_CC_STARTED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*enter FULLCOM_S_BUS_OFF_CHECK state,clear BusOff counter*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
    {
        networkPtr->fullComState = FULLCOM_S_NO_BUS_OFF;
        networkPtr->busOffCounter = 0u;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;

        uint8 controllerId;
        CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
        {
            (void)CanIf_SetPduMode(controllerId, CANIF_ONLINE);
        }
        CANSM_FOREACH_NETWORK_CONTROLLER_END()
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->fullComState = FULLCOM_S_CC_STARTED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_RESTART_CC
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Restart_CC(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

    /*enter FULLCOM_S_RESTART_CC_WAIT state*/
    networkPtr->fullComState = FULLCOM_S_RESTART_CC_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_RESTART_CC_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Restart_CC_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*enter FULLCOM_S_TX_OFF state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
    {
        networkPtr->fullComState = FULLCOM_S_TX_OFF;
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_RESTART_CC_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->fullComState = FULLCOM_S_RESTART_CC;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in FULLCOM_S_TX_OFF
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_FullCom_S_Tx_Off(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    uint32 busOffContinueTime;
    networkPtr->busOffEventStartTime++;
    const CanSM_ManagerNetworkType* managerNetworkConfig = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netID];
    if (networkPtr->busOffCounter >= managerNetworkConfig->CanSMBorCounterL1ToL2)
    {
        busOffContinueTime = managerNetworkConfig->CanSMBorTimeL2
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
                             + (uint32)networkPtr->busOffDelayTime
#endif
            ;
    }
    else
    {
        busOffContinueTime = managerNetworkConfig->CanSMBorTimeL1
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
                             + (uint32)networkPtr->busOffDelayTime
#endif
            ;
    }
    /*G_TX_ON state is OK*/
    if (networkPtr->busOffEventStartTime >= busOffContinueTime)
    {
/*do the E_TX_ON action*/
#if (STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT)
        if ((boolean)TRUE == CanSM_EcuPassive)
        {
            uint8 controllerId;
            CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
            {
                (void)CanIf_SetPduMode(controllerId, CANIF_TX_OFFLINE_ACTIVE);
            }
            CANSM_FOREACH_NETWORK_CONTROLLER_END()
        }
        else
#endif /*STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT*/
        {
            uint8 controllerId;
            CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
            {
                (void)CanIf_SetPduMode(controllerId, CANIF_ONLINE);
            }
            CANSM_FOREACH_NETWORK_CONTROLLER_END()
        }
        BswM_CanSM_CurrentState(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
            CANSM_BSWM_FULL_COMMUNICATION);
        networkPtr->curComMode = COMM_FULL_COMMUNICATION;
        CanSM_ComMModeIndication(netID, networkPtr->curComMode, networkPtr->requestComMode);
        /*enter FULLCOM_S_BUS_OFF_CHECK state*/
        networkPtr->fullComState = FULLCOM_S_BUS_OFF_CHECK;
        if (networkPtr->busOffCounter < BUS_OFF_COUNTER_UP_LIMIT)
        {
            networkPtr->busOffCounter++;
        }
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
        if ((boolean)FALSE == managerNetworkConfig->CanSMBorTxConfirmationPolling)
#endif
        {
            networkPtr->borTimeTxEnsuredTime = managerNetworkConfig->CanSMBorTimeTxEnsured;
        }
#endif
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_FullCom
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSFullCom(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_FullComStateType lastState = networkPtr->fullComState;

    if (CANSM_NO_COMMUNICATION == networkPtr->requestComMode)
    {
        CanSM_EPreNoComAction(netID);
    }
    else if (CANSM_SILENT_COMMUNICATION == networkPtr->requestComMode)
    {
        CanSM_EFullToSilentComAction(netID);
    }
    else
    {
        switch (networkPtr->fullComState)
        {
        case FULLCOM_S_BUS_OFF_CHECK:
            CanSM_FullCom_S_Bus_off_Check(netID, networkPtr);
            break;
        case FULLCOM_S_NO_BUS_OFF:
            /* keep the FULLCOM_S_NO_BUS_OFF state until T_TX_TIMEOUT_EXCEPTION,T_BUS_OFF,T_CHANGE_BR_REQUEST happen */
            CanSM_FullCom_S_No_Bus_Off(netID, networkPtr);
            break;
        case FULLCOM_S_CC_STOPPED:
            CanSM_FullCom_S_CC_Stopped(netID, networkPtr);
            break;
        case FULLCOM_S_CC_STOPPED_WAIT:
            CanSM_FullCom_S_CC_Stopped_Wait(netID, networkPtr);
            break;
        case FULLCOM_S_CC_STARTED:
            CanSM_FullCom_S_CC_Started(netID, networkPtr);
            break;
        case FULLCOM_S_CC_STARTED_WAIT:
            CanSM_FullCom_S_CC_Started_Wait(netID, networkPtr);
            break;
        case FULLCOM_S_RESTART_CC:
            CanSM_FullCom_S_Restart_CC(netID, networkPtr);
            break;
        case FULLCOM_S_RESTART_CC_WAIT:
            CanSM_FullCom_S_Restart_CC_Wait(netID, networkPtr);
            break;
        case FULLCOM_S_TX_OFF:
            CanSM_FullCom_S_Tx_Off(netID, networkPtr);
            break;
        default:
            /*This logic does not require any processing*/
            break;
        }
    }
    return (networkPtr->fullComState != lastState) || (networkPtr->curBsmState != CANSM_BSM_S_FULLCOM);
}

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/******************************************************************************/
/*
 * Brief               Handle state transition in Substate CHANGEBR_CHANGE_BR_SYNC
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_Change_BR_Sync(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    Std_ReturnType ret;
    ret = E_OK;

    uint8 controllerId;
    CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
    {
        if ((uint8)E_NOT_OK == CanIf_SetBaudrate(controllerId, networkPtr->baudRateConfigID))
        {
            ret = E_NOT_OK;
            break;
        }
    }
    CANSM_FOREACH_NETWORK_CONTROLLER_END()

    /*all CanIf_SetBaudrate requests returned with E_OK*/
    if ((uint8)E_OK == ret)
    {
        if (CANSM_NO_COMMUNICATION == networkPtr->requestComMode)
        {
            CanSM_EPreNoComAction(netID);
        }
        else if (CANSM_SILENT_COMMUNICATION == networkPtr->requestComMode)
        {
            CanSM_EFullToSilentComAction(netID);
        }
        else
        {
            CanSM_EFullComAction(netID);
        }
    }
    /*not all CanIf_SetBaudrate requests returned with E_OK,
     * enter CHANGEBR_S_CC_STOPPED state*/
    else
    {
        networkPtr->changeBaudrateState = CHANGEBR_S_CC_STOPPED;
        networkPtr->repeatCounter = 0u;
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Substate CHANGEBR_S_CC_STOPPED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Stopped(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STOPPED);

    /*enter CHANGEBR_S_CC_STOPPED_WAIT state*/
    networkPtr->changeBaudrateState = CHANGEBR_S_CC_STOPPED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Substate CHANGEBR_S_CC_STOPPED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Stopped_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    ComM_ModeType ComMode;
    /*enter CHANGEBR_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STOPPED))
    {
        networkPtr->changeBaudrateState = CHANGEBR_S_CC_STARTED;
        ComMode = COMM_NO_COMMUNICATION;
        /*do E_CHANGE_BAUDRATE action*/
        CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);

        uint8 controllerId;
        CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
        {
            (void)CanIf_SetBaudrate(controllerId, networkPtr->baudRateConfigID);
        }
        CANSM_FOREACH_NETWORK_CONTROLLER_END()

        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->changeBaudrateState = CHANGEBR_S_CC_STOPPED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Substate CHANGEBR_S_CC_STARTED
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Started(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

    /*enter CHANGEBR_S_CC_STARTED_WAIT state*/
    networkPtr->changeBaudrateState = CHANGEBR_S_CC_STARTED_WAIT;
    networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Substate CHANGEBR_S_CC_STARTED_WAIT
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     networkPtr,NetWork RunTime struct ptr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_ChangeBR_S_CC_Started_Wait(uint8 netID, CanSM_NetWorkRunTimeType* networkPtr)
{
    /*exit CANSM_BSM_S_CHANGE_BAUDRATE state*/
    if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
    {
        if (CANSM_NO_COMMUNICATION == networkPtr->requestComMode)
        {
            CanSM_EPreNoComAction(netID);
        }
        else if (CANSM_SILENT_COMMUNICATION == networkPtr->requestComMode)
        {
            CanSM_EFullToSilentComAction(netID);
        }
        else
        {
            CanSM_EFullComAction(netID);
        }
        networkPtr->waitCanIfIndicatedStartTime = 0;
        networkPtr->repeatCounter = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0u == networkPtr->waitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(netID);
            networkPtr->changeBaudrateState = CHANGEBR_S_CC_STARTED;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_Change_Baudrate
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSChangeBaudrate(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_ChangeBaudrateStateType lastState = networkPtr->changeBaudrateState;

    switch (networkPtr->changeBaudrateState)
    {
    case CHANGEBR_CHANGE_BR_SYNC:
        CanSM_ChangeBR_Change_BR_Sync(netID, networkPtr);
        break;
    case CHANGEBR_S_CC_STOPPED:
        CanSM_ChangeBR_S_CC_Stopped(netID, networkPtr);
        break;
    case CHANGEBR_S_CC_STOPPED_WAIT:
        CanSM_ChangeBR_S_CC_Stopped_Wait(netID, networkPtr);
        break;
    case CHANGEBR_S_CC_STARTED:
        CanSM_ChangeBR_S_CC_Started(netID, networkPtr);
        break;
    case CHANGEBR_S_CC_STARTED_WAIT:
        CanSM_ChangeBR_S_CC_Started_Wait(netID, networkPtr);
        break;
    default:
        /*nothing*/
        break;
    }
    return (networkPtr->changeBaudrateState != lastState) || (networkPtr->curBsmState != CANSM_BSM_S_CHANGE_BAUDRATE);
}
#endif

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_SilentCom
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSSilentCom(uint8 netID)
{
    if (CanSM_NetWorkRunTime[netID].busOffEvent)
    {
        CanSM_EBusOffAction(netID);
    }
    else if (CANSM_NO_COMMUNICATION == CanSM_NetWorkRunTime[netID].requestComMode)
    {
        CanSM_EPreNoComAction(netID);
    }
    else if (CANSM_FULL_COMMUNICATION == CanSM_NetWorkRunTime[netID].requestComMode)
    {
        CanSM_EFullComAction(netID);
    }
    else
    {
        /*This logic does not require any processing*/
    }
    return CanSM_NetWorkRunTime[netID].curBsmState != CANSM_BSM_S_SILENTCOM;
}

/******************************************************************************/
/*
 * Brief               Handle state transition in Bsm_S_SilentCom_Bor
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static boolean CanSM_BsmSSilentComBor(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    CanSM_SilentComBORStateType lastState = networkPtr->silentComBORState;

    if (CANSM_NO_COMMUNICATION == networkPtr->requestComMode)
    {
        CanSM_EPreNoComAction(netID);
    }
    else
    {
        switch (networkPtr->silentComBORState)
        {
        case SILENTBOR_S_RESTART_CC:
            /*set all controllers of the network to request mode repeat*/
            CanSM_SetControllerModeRepeat(netID, networkPtr, CAN_CS_STARTED);

            /*enter SILENTBOR_S_RESTART_CC_WAIT state*/
            networkPtr->silentComBORState = SILENTBOR_S_RESTART_CC_WAIT;
            networkPtr->waitCanIfIndicatedStartTime = CANSM_MODEREQREPEAT_MAINFUNCTIONTIME;
            break;
        case SILENTBOR_S_RESTART_CC_WAIT:
            /*exit CANSM_BSM_S_SILENTCOM_BOR state,enter CANSM_BSM_S_SILENTCOM state*/
            if (CanSM_CheckNetworkAllControllerMode(netID, CAN_CS_STARTED))
            {
                networkPtr->curBsmState = CANSM_BSM_S_SILENTCOM;
                networkPtr->waitCanIfIndicatedStartTime = 0;
                networkPtr->repeatCounter = 0u;
            }
            else
            {
                /*T_RESTART_CC_TIMEOUT*/
                if (0u == networkPtr->waitCanIfIndicatedStartTime)
                {
                    CanSM_ModeRequestTimeout(netID);
                    networkPtr->silentComBORState = SILENTBOR_S_RESTART_CC;
                }
            }
            break;
        default:
            /*This logic does not require any processing*/
            break;
        }
    }
    return (networkPtr->silentComBORState != lastState) || (networkPtr->curBsmState != CANSM_BSM_S_SILENTCOM_BOR);
}

/******************************************************************************/
/*
 * Brief               action of effect E_FULL_COM
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_EFullComAction(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    ComM_ModeType ComMode;
/*do E_FULL_COM action*/
#if (STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT)
    if ((boolean)TRUE == CanSM_EcuPassive)
    {
        uint8 controllerId;
        CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
        {
            (void)CanIf_SetPduMode(controllerId, CANIF_TX_OFFLINE_ACTIVE);
        }
        CANSM_FOREACH_NETWORK_CONTROLLER_END()
    }
    else
#endif /*STD_ON == CANSM_TX_OFFLINE_ACTIVE_SUPPORT*/
    {
        uint8 controllerId;
        CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
        {
            (void)CanIf_SetPduMode(controllerId, CANIF_ONLINE);
        }
        CANSM_FOREACH_NETWORK_CONTROLLER_END()
    }
    ComMode = COMM_FULL_COMMUNICATION;
    CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
    BswM_CanSM_CurrentState(
        CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
        CANSM_BSWM_FULL_COMMUNICATION);
    /*enter CANSM_BSM_S_FULLCOM,sub FULLCOM_S_BUS_OFF_CHECK state*/
    networkPtr->curBsmState = CANSM_BSM_S_FULLCOM;
    networkPtr->fullComState = FULLCOM_S_BUS_OFF_CHECK;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
    const CanSM_ManagerNetworkType* managerNetworkConfig = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netID];
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
    if ((boolean)FALSE == managerNetworkConfig->CanSMBorTxConfirmationPolling)
#endif
    {
        networkPtr->borTimeTxEnsuredTime = managerNetworkConfig->CanSMBorTimeTxEnsured;
    }
#endif
    /*Com Mode change to COMM_FULL_COMMUNICATION*/
    networkPtr->curComMode = COMM_FULL_COMMUNICATION;
}

/******************************************************************************/
/*
 * Brief               action of effect E_FULL_TO_SILENT_COM
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_EFullToSilentComAction(uint8 netID)
{
    ComM_ModeType ComMode;
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    /*enter CANSM_BSM_S_SILENTCOM state*/
    networkPtr->curBsmState = CANSM_BSM_S_SILENTCOM;
    /*do the E_FULL_TO_SILENT_COM action*/
    BswM_CanSM_CurrentState(
        CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
        CANSM_BSWM_SILENT_COMMUNICATION);

    uint8 controllerId;
    CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
    {
        (void)CanIf_SetPduMode(controllerId, CANIF_TX_OFFLINE);
    }
    CANSM_FOREACH_NETWORK_CONTROLLER_END()

    ComMode = COMM_SILENT_COMMUNICATION;
    CanSM_ComMModeIndication(netID, ComMode, networkPtr->requestComMode);
    networkPtr->curComMode = COMM_SILENT_COMMUNICATION;
}

/******************************************************************************/
/*
 * Brief               action of effect E_PRE_NOCOM
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CANSM_CODE) CanSM_EPreNoComAction(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
/*enter CANSM_BSM_S_PRE_NOCOM state*/
#if (STD_ON == CANSM_PNC_SUPPORT)
    if ((NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef)
        && ((boolean)TRUE == CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].TrcvRef->CanSMCanTrcvPnEnabled))
    {
        networkPtr->curBsmState = CANSM_BSM_DEINITPNSUPPORTED;
        networkPtr->deinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
    }
    else
#endif
    {
        networkPtr->curBsmState = CANSM_BSM_DEINITPNNOTSUPPORTED;
        networkPtr->deinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
    }
    networkPtr->repeatCounter = 0u;
    /*do the E_PRE_NOCOM action*/
    BswM_CanSM_CurrentState(
        CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
        CANSM_BSWM_NO_COMMUNICATION);
    networkPtr->curComMode = COMM_NO_COMMUNICATION;
}

static void CanSM_EBusOffAction(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    if (networkPtr->curBsmState == CANSM_BSM_S_FULLCOM)
    {
        BswM_CanSM_CurrentState(
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
            CANSM_BSWM_BUS_OFF);
        networkPtr->curComMode = COMM_SILENT_COMMUNICATION;
        CanSM_ComMModeIndication(netID, networkPtr->curComMode, networkPtr->requestComMode);
        networkPtr->fullComState = FULLCOM_S_RESTART_CC;

#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
        if (TRUE == CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMEnableBusOffDelay)
        {
            uint8 delayCycleNum = 0u;
            CANSM_GET_BUSOFF_DELAY_FUNCTION(
                CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMComMNetworkHandleRef,
                &delayCycleNum);
            networkPtr->busOffDelayTime = (uint16)delayCycleNum;
        }
#endif /*STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED*/
        networkPtr->busOffEventStartTime = 0u;
    }
    else /* networkPtr->curBsmState == CANSM_BSM_S_SILENTCOM */
    {
        networkPtr->curBsmState = CANSM_BSM_S_SILENTCOM_BOR;
        networkPtr->silentComBORState = SILENTBOR_S_RESTART_CC;
    }

    networkPtr->repeatCounter = 0u;

#if (STD_ON == CANSM_DEM_SUPPORT)
    if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs)
    {
        if (NULL_PTR != CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->BusOffPara)
        {
            Dem_ReportErrorStatus(
                *CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs->BusOffPara,
                DEM_EVENT_STATUS_PREFAILED);
        }
    }
#endif
}

/******************************************************************************/
/*
 * Brief               Switch the state of the controller
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different netID.
 * Param-Name[in]      netID, network Id
 *                     requestContorllerMode, requested controller state
 * Param-Name[out]     networkPtr,runtime network state pointer
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static void CanSM_SetControllerModeRepeat(
    uint8 netID,
    CanSM_NetWorkRunTimeType* networkPtr,
    Can_ControllerStateType requestContorllerMode)
{
    networkPtr->busOffEvent = FALSE;

    uint8 controllerId;
    CANSM_FOREACH_NETWORK_CONTROLLER_START(netID, controllerId)
    {
        if (requestContorllerMode != CanSM_ControllerMode[controllerId])
        {
            (void)CanIf_SetControllerMode(controllerId, requestContorllerMode);
        }
    }
    CANSM_FOREACH_NETWORK_CONTROLLER_END()
}

CANSM_LOCAL boolean CanSM_CheckNetworkAllControllerMode(uint8 netID, Can_ControllerStateType controllerMode)
{
    const CanSM_ControllerRefType* controllerRef = CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].ControllerRef;
    const uint8* controllerId = controllerRef->CanSMControllerId;
    const uint8* const controllerIdEnd = &controllerRef->CanSMControllerId[controllerRef->NetWorkRefControllerNumber];
    for (; controllerId < controllerIdEnd; ++controllerId)
    {
        if (CanSM_ControllerMode[*controllerId] != controllerMode)
        {
            return FALSE;
        }
    }
    return TRUE;
}

CANSM_LOCAL void CanSM_ModeRequestTimeout(uint8 netID)
{
    CanSM_NetWorkRunTimeType* networkPtr = &CanSM_NetWorkRunTime[netID];
    if (networkPtr->repeatCounter >= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
    {
        networkPtr->repeatCounter = 0u;
#if CANSM_DEM_SUPPORT == STD_ON
        const CanSM_DemEventParameterRefType* demEventParameterRef =
            CanSM_ConfigPtr->CanSMManagerNetworkRef[netID].CanSMDemEventParameterRefs;
        if (demEventParameterRef != NULL_PTR)
        {
            if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
            {
                Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
            }
        }
#endif
        (void)Det_ReportRuntimeError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_MAINFUNCTION,
            CANSM_E_MODE_REQUEST_TIMEOUT);
    }
    else
    {
        ++networkPtr->repeatCounter;
    }
}

#if CANSM_DEV_ERROR_DETECT == STD_ON
CANSM_LOCAL boolean CanSM_ValidateAllNetworksNoCom(void)
{
    uint8 error = CANSM_E_NO_ERROR;

    if (CanSM_Status == CANSM_UNINITED)
    {
        error = CANSM_E_UNINIT;
    }
    else
    {
        for (CanSM_NetworkIndexType networkIndex = 0u; networkIndex < CANSM_NETWORK_NUM; ++networkIndex)
        {
            if (CANSM_BSM_S_NOCOM != CanSM_NetWorkRunTime[networkIndex].curBsmState)
            {
                error = CANSM_E_NOT_IN_NO_COM; /* SWS_CanSM_00660 */
                break;
            }
        }
    }

    if (error != CANSM_E_NO_ERROR)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_DEINIT, error);
    }
    return error == CANSM_E_NO_ERROR;
}
#endif
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
