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
**  FILENAME    : BswM_Internal.h                                             **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef BSWM_INTERNAL_H
#define BSWM_INTERNAL_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM.h"
#include "BswM_Cfg.h"
#include "SchM_BswM.h"
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define BSWM_EVENT_RQST_STATE_CLEARED 0x00U
#define BSWM_EVENT_RQST_STATE_SET     0x01U

#define BSWM_AVOID_WARNING(var)       ((void)var)

#define BSWM_INVALID_U8               (uint8)(0xffU)
#define BSWM_INVALID_U16              (uint16)(0xffffu)
#define BSWM_INVALID_U32              (uint32)(0xffffffffu)

#if !defined(BSWM_PARA_UNUSED)
#define BSWM_PARA_UNUSED(x) ((void)(x))
#endif /*BSWM_PARA_UNUSED*/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*type definition for event request port Pre-compile time configure*/
typedef struct
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON) /*EventRequestPort configuration exists*/
#if (BSWM_COMM_ENABLED == STD_ON)
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) comMInitRstIdxPtr;
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) dcmAppUpdateIndIdxPtr;
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
    /*BswMModeSwitchErrorEvent*/
    uint16 numOfModeSwiErrEv;
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) modeSwiErrEvPtr;
#endif /*BSWM_RTE_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
    /*BswMNmCarWakeUpIndication*/
    NetworkHandleType numOfNmWkCh;
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) nmWkEvPortIdxPtr;
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) parRestartIdxPtr;
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_WDGM_ENABLED == STD_ON)
    uint8 numOfWdgmRstEv;
    P2CONST(BswM_EventRqstPortIdxType, TYPEDEF, BSWM_CONST) wdgmRqstParRstEvIdxPtr;
#endif /*BSWM_WDGM_ENABLED == STD_ON*/

#else  /*EventRequestPort configuration don't exist*/
    uint8 dummy;
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
} BswM_EvRqstPortPCCfgType;

/*type definition for mode request port Pre-compile time configure*/
typedef struct
{
#if (BSWM_SCHM_ENABLED == STD_ON)
    /*BswMBswModeNotification*/
    BswM_ModeGroupType numOfBswModeNoti;
    P2CONST(BswM_ModeType, TYPEDEF, BSWM_CONST) bswInitMode;
#endif /*BSWM_SCHM_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
    uint8 numOfCanSMIcomInd;                                   /*BswMCanSMIcomIndication*/
    P2CONST(IcomConfigIdType, TYPEDEF, BSWM_CONST) icomInitId; /*Pretended Networking*/
#endif                                                         /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
    NetworkHandleType numOfCanSMInd;                           /*BswMCanSMIndication*/
    P2CONST(CanSM_BswMCurrentStateType, TYPEDEF, BSWM_CONST) canSMInitState;
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
    NetworkHandleType numOfComMInd; /*BswMComMIndication*/
    P2CONST(ComM_ModeType, TYPEDEF, BSWM_CONST) comMInitMode;
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
    PNCHandleType numOfComMPncInd;
    P2CONST(ComM_PncModeType, TYPEDEF, BSWM_CONST) comMPncInitMode;
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
    NetworkHandleType numOfDcmComModeReq; /*BswMDcmComModeRequest*/
    P2CONST(Dcm_CommunicationModeType, TYPEDEF, BSWM_CONST) dcmInitMode;
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
    EcuM_WakeupSourceType numOfEcuMWakeupSource; /*BswMEcuMWakeupSource*/
    P2CONST(EcuM_WakeupStatusType, TYPEDEF, BSWM_CONST) ecumWkInitstate;
    uint8 numOfEcuMRunRqstSource; /*BswMEcuMRUNRequestIndication*/
    P2CONST(EcuM_RunStatusType, TYPEDEF, BSWM_CONST) ecumRunRqstInitstate;
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
    EthIf_SwitchPortGroupIdxType numOfEthIfPG; /*BswMEthIfPortGroupLinkStateChg*/
    P2CONST(EthTrcv_LinkStateType, TYPEDEF, BSWM_CONST) ethIfPGInitState;
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
    NetworkHandleType numOfEthSMInd; /*BswMEthSMIndication*/
    P2CONST(EthSM_NetworkModeStateType, TYPEDEF, BSWM_CONST) ethsmInitState;
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
    NetworkHandleType numOfFrSMInd; /*BswMFrSMIndication*/
    P2CONST(FrSM_BswM_StateType, TYPEDEF, BSWM_CONST) frsmInitState;
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
    BswM_UserType numOfGenericReq; /*BswMGenericRequest*/
    P2CONST(uint16, TYPEDEF, BSWM_CONST) genRqstInitMode;
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
    NetworkHandleType numOfj1939DcmCh; /*BswMJ1939DcmBroadcastStatus*/
    P2CONST(boolean, TYPEDEF, BSWM_CONST) j1939DcmInitStatus;
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
    NetworkHandleType numOfJ1939NmInd; /*BswMJ1939NmIndication*/
    P2CONST(Nm_StateType, TYPEDEF, BSWM_CONST) j1939NmInitState;
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
    NetworkHandleType numOfLinSMInd; /*BswMLinSMIndication*/
    P2CONST(LinSM_ModeType, TYPEDEF, BSWM_CONST) linsmInitState;
    LinIf_SchHandleType numOfLinSchInd; /*BswMLinScheduleIndication*/
#endif                                  /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
    NetworkHandleType numOfLinTpModeReq; /*BswMLinTpModeRequest*/
    P2CONST(LinTp_Mode, TYPEDEF, BSWM_CONST) lintpRqstInitMode;
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
    uint8 numOfNvMJob; /*BswMNvMJobModeIndication*/
    P2CONST(NvM_RequestResultType, TYPEDEF, BSWM_CONST) nvmJobInitMode;
    NvM_BlockIdType numOfNvMBlock; /*BswMNvMRequest*/
    P2CONST(NvM_RequestResultType, TYPEDEF, BSWM_CONST) nvmInitMode;
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
    uint16 numOfSdCliSrvCurState; /*BswMSdClientServiceCurrentState*/
    P2CONST(Sd_ClientServiceCurrentStateType, TYPEDEF, BSWM_CONST) curClientInitState;
    uint16 numOfSdConsumedEvGrpCurrState; /*BswMSdConsumedEventGroupCurrentState*/
    P2CONST(Sd_ConsumedEventGroupCurrentStateType, TYPEDEF, BSWM_CONST) consEvGroupInitState;
    uint16 numOfSdEvHdlerCurrState; /*BswMSdEventHandlerCurrentState*/
    P2CONST(Sd_EventHandlerCurrentStateType, TYPEDEF, BSWM_CONST) evHandlerInitStatus;
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
    /*BswMSwcModeNotification*/
    BswM_ModeGroupType numOfSwcModeNoti;
    P2CONST(BswM_ModeType, TYPEDEF, BSWM_CONST) swcInitMode;
    /*BswMSwcModeRequest*/
    BswM_ModeGroupType numOfSwcModeReq;
    P2CONST(BswM_ModeType, TYPEDEF, BSWM_CONST) swcRqstInitMode;
#endif /*#(BSWM_RTE_ENABLED == STD_ON)*/

#if (BSWM_TIMER_ENABLED == STD_ON)
    BswM_TimerIdxType numOfBswMTimer;
#endif /*BSWM_TIMER_ENABLED == STD_ON*/

#if (BSWM_MODE_RQSTPORT_ENABLE == STD_OFF) /*ModeRequestPort configuration \ don't exist*/
    uint8 dummy;
#endif /*BSWM_MODE_RQSTPORT_ENABLE == STD_OFF*/
} BswM_ModeRqstPortPCCfgType;

/*type definition of runtime information for BswMModeRequestPort*/
typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#if (BSWM_SCHM_ENABLED == STD_ON)
    /*BswMBswModeNotification*/
    P2VAR(BswM_ModeType, TYPEDEF, BSWM_VAR_NO_INIT_SINGLE_8) bswMode;
#endif /*BSWM_SCHM_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
    /*BswMCanSMIcomIndication*/
    P2VAR(IcomConfigIdType, TYPEDEF, BSWM_VAR_CLEARED) canSMIcomIndiStatus;
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
    /*BswMCanSMIndication*/
    P2VAR(CanSM_BswMCurrentStateType, TYPEDEF, BSWM_VAR_CLEARED) canSMIndiStatus;
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
    /*BswMComMIndication*/
    P2VAR(ComM_ModeType, TYPEDEF, BSWM_VAR_CLEARED) comMIndiStatus;
#if (BSWM_COMM_PNC_ENABLED == STD_ON)
    /*BswMComMPncRequest*/
    P2VAR(ComM_PncModeType, TYPEDEF, BSWM_VAR_CLEARED) comMPncReqStatus;
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
    /*BswMDcmComModeRequest*/
    P2VAR(Dcm_CommunicationModeType, TYPEDEF, BSWM_VAR_CLEARED) dcmComModeReqStatus;
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
    /*BswMEcuMWakeupSource*/
    P2VAR(EcuM_WakeupStatusType, TYPEDEF, BSWM_VAR_CLEARED) ecuMWakeupSrcStatus;
    /*BswMEcuMRUNRequestIndication*/
    P2VAR(EcuM_RunStatusType, TYPEDEF, BSWM_VAR_CLEARED) ecuMRunRqstStatus;
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
    /*BswMEthIfPortGroupLinkStateChg*/
    P2VAR(EthTrcv_LinkStateType, TYPEDEF, BSWM_VAR_CLEARED) ethIfPGStatus;
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
    /*BswMEthSMIndication*/
    P2VAR(EthSM_NetworkModeStateType, TYPEDEF, BSWM_VAR_CLEARED) ethSMIndiStatus;
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
    /*BswMFrSMIndication*/
    P2VAR(FrSM_BswM_StateType, TYPEDEF, BSWM_VAR_CLEARED) frSMIndiStatus;
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
    /*BswMGenericRequest*/
    P2VAR(uint16, TYPEDEF, BSWM_VAR_CLEARED) genericReqStatus;
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
    /*BswMJ1939DcmBroadcastStatus*/
    P2VAR(boolean, TYPEDEF, BSWM_VAR_CLEARED) j1939DcmBrdcstStatus;
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
    /*BswMJ1939NmIndication*/
    P2VAR(Nm_StateType, TYPEDEF, BSWM_VAR_CLEARED) j1939NmIndiStatus;
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
    /*BswMLinSMIndication*/
    P2VAR(LinSM_ModeType, TYPEDEF, BSWM_VAR_CLEARED) linSMIndiStatus;
    /*BswMLinScheduleIndication*/
    P2VAR(LinIf_SchHandleType, TYPEDEF, BSWM_VAR_CLEARED) linSchIndiStatus;
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
    /*BswMLinTpModeRequest*/
    P2VAR(LinTp_Mode, TYPEDEF, BSWM_VAR_CLEARED) linTpModeReqStatus;
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
    /*BswMNvMJobModeIndication*/
    P2VAR(NvM_RequestResultType, TYPEDEF, BSWM_VAR_CLEARED) nvMJobModeIndiStatus;
    /*BswMNvMRequest*/
    P2VAR(NvM_RequestResultType, TYPEDEF, BSWM_VAR_CLEARED) nvMReqStatus;
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
    /*BswMSdClientServiceCurrentState*/
    P2VAR(Sd_ClientServiceCurrentStateType, TYPEDEF, BSWM_VAR_CLEARED) sdCliServiceCurrStaStatus;
    /*BswMSdConsumedEventGroupCurrentState*/
    P2VAR(Sd_ConsumedEventGroupCurrentStateType, TYPEDEF, BSWM_VAR_CLEARED) sdConsumedEvGrpCurrStaStatus;
    /*BswMSdEventHandlerCurrentState*/
    P2VAR(Sd_EventHandlerCurrentStateType, TYPEDEF, BSWM_VAR_CLEARED) sdEvHdlerCurrStaStatus;
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
    /*BswMSwcModeNotification*/
    P2VAR(BswM_ModeType, TYPEDEF, BSWM_VAR_CLEARED) swcModeNotiStatus;
    /*BswMSwcModeRequest*/
    P2VAR(BswM_ModeType, TYPEDEF, BSWM_VAR_CLEARED) swcModeRqstStatus;
#endif /*BSWM_RTE_ENABLED == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
    P2VAR(BswM_TimerEnumType, TYPEDEF, BSWM_VAR_CLEARED) timerStatus;
    P2VAR(uint32, TYPEDEF, BSWM_VAR_CLEARED) timerCycle;
#endif                   /*BSWM_TIMER_ENABLED == STD_ON*/
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} BswM_ModeRqstPortRuntimeType;

/*type definition of runtime information for BswMEventRequestPort*/
typedef uint8 BswM_EventRquestPortRuntimeType;

/*type definition for BSWM partition Pre-compile time configure*/
typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partitionId; /*id of this partition*/
#endif                           /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/
    BswM_EventRqstPortIdxType evRqstSrcNum;
    P2CONST(BswM_EvRqstPortPCCfgType, TYPEDEF, BSWM_CONST) evRqstPCCfg;
    P2CONST(BswM_ModeRqstPortPCCfgType, TYPEDEF, BSWM_CONST) modeRqstPCCfg;
    P2VAR(BswM_EventRquestPortRuntimeType, TYPEDEF, BSWM_VAR_CLEARED) eventRqstPortRunPtr;
    P2VAR(BswM_ModeRqstPortRuntimeType, TYPEDEF, BSWM_VAR_CLEARED) modeRqstPortRunPtr;
#if (BSWM_COM_ENABLED == STD_ON)
#if (BSWM_COM_VERSION == BSWM_V_4_2_2)
    P2VAR(Com_IpduGroupVector, TYPEDEF, BSWM_VAR_CLEARED) comRxDmIpduGroupVector;
    P2VAR(Com_IpduGroupVector, TYPEDEF, BSWM_VAR_CLEARED) ipduGroupReinitVecotr;
    P2VAR(Com_IpduGroupVector, TYPEDEF, BSWM_VAR_CLEARED) ipduGroupAllVecotr;
#else
    /*BSWM_COM_VERSION == BSWM_V_R19_11*/
#endif                   /*BSWM_COM_VERSION == BSWM_V_4_2_2*/
#endif                   /*BSWM_COM_ENABLED == STD_ON */
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} BswM_PartitionPCCfgType;

/*type definition for BSWM Pre-compile time configuration root in core*/
typedef struct
{
#if (BSWM_MAX_NUM_CORE > 1u)
    CoreIdType bswmCoreId; /*the core these BSWM belong to*/
#endif
    uint8 numOfPartitions; /*Number of partitions in this core*/
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
} BswM_PCCfgType;

/*runtime information for BSWM module in a core*/
typedef struct
{
    uint8 numOfPartitions;
    BswM_InitStatusType bswmInitState;
    P2CONST(BswM_PartitionPCCfgType, TYPEDEF, BSWM_CONST) bswmPartPCCfgs;
    P2CONST(BswM_PartitionLCfgType, TYPEDEF, BSWM_CONST) bswmPartLCfgs;
#if (BSWM_ECUM_ENABLED == STD_ON)
    EcuM_StateType ecumState;
#endif /*BSWM_ECUM_ENABLED == STD_ON*/
} BswM_RuntimeStatusType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONSTP2CONST(BswM_PCCfgType, BSWM_CONST, BSWM_CONST) BswM_CorePCCfg;

/*runtime status information of BSWM partition on this local core. If it is
 * mulit-core ECU, every CORE running BSW will have a core-specific status*/
extern volatile VAR(BswM_RuntimeStatusType, BSWM_CLEARED) BswM_RuntimeStatus;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/*Det check functions*/
FUNC(Std_ReturnType, BSWM_CANSM_CODE)
BswM_ChkBswMInitStatus(uint8 sid);

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkParRestart(void);
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCanSMInd(CanSM_BswMCurrentStateType CurrentState);

#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCanSMIcom(NetworkHandleType Network);
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMInd(ComM_ModeType RequestedMode);

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMPnc(ComM_PncModeType CurrentPncMode);
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkComMInitRst(void);
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkDcmUpdate(void);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkDcmModeCurState(Dcm_CommunicationModeType RequestedMode);
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEcuMCurWakeUp(EcuM_WakeupStatusType state);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkRequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEcuMCurState(void);
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEthIfPortGru(EthTrcv_LinkStateType PortGroupState);
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEthSMInd(EthSM_NetworkModeStateType CurrentState);
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkFrSMInd(FrSM_BswM_StateType CurrentState);
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkJ1939DcmBroad(void);
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkJ1939NmInd(void);
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinSMCurSch(void);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinSMInd(LinSM_ModeType CurrentState);
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkLinTpRqst(LinTp_Mode LinTpRequestedMode);
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkNmCarWkUp(void);
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCurNvmBlockMode(NvM_RequestResultType CurrentBlockMode);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkCurNvmJobMode(NvM_MultiBlockRequestType MultiBlockRequest);
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkGenericRqst(BswM_UserType requestingUser);
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkClientCurState(Sd_ClientServiceCurrentStateType CurrentClientState);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEvGruCurState(Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState);

FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkEvHandlerCurState(Sd_EventHandlerCurrentStateType EventHandlerStatus);
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_WDGM_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DetChkWgmParRst(void);
#endif /*BSWM_WDGM_ENABLED == STD_ON*/

/*Get current partition index*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
FUNC(boolean, BSWM_CODE)
BswM_GetPartitionIdx(P2VAR(ApplicationType, AUTOMATIC, BSWM_VAR_CLEARED) partitionIdx);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#if (BSWM_SCHM_ENABLED == STD_ON)
/*Initialize BswMBswModeNotification request port*/
FUNC(void, BSWM_CANSM_CODE)
BswM_InitBswModeRqstPort(void);

/*Get bsw indicate status*/
FUNC(BswM_ModeType, BSWM_CANSM_CODE)
BswM_GetBswModeIndStatus(BswM_ModeGroupType bswChIdx);
#endif /*BSWM_SCHM_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
/*CanSM*/
#if (BSWM_CANSM_ICOM_ENABLED == STD_ON)
/*Get cansm icom configuration index*/
FUNC(IcomConfigIdType, BSWM_CANSM_CODE)
BswM_GetCanSmIcomCfgId(NetworkHandleType icomIdx);
#endif /*BSWM_CANSM_ICOM_ENABLED == STD_ON*/

/*Get cansm indicate status*/
FUNC(CanSM_BswMCurrentStateType, BSWM_CANSM_CODE)
BswM_GetCanSmIndStatus(NetworkHandleType cansmChIdx);

/*Initialize BswMCanSMIndication and BswMCanSMIcomIndication request port*/
FUNC(void, BSWM_CANSM_CODE)
BswM_InitCanSMRequestPorts(void);
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
/*ComM*/
/*Initialize BswMComMIndication and BswMComMPncRequest request port*/
FUNC(void, BSWM_COMM_CODE)
BswM_InitComMRequestPorts(void);

/*Get comM initiateReset status*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_COMM_CODE)
BswM_GetComMResetInitStatus(void);

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
/*Get ComM Pnc status*/
FUNC(ComM_PncModeType, BSWM_COMM_CODE)
BswM_GetComMPncStatus(PNCHandleType pncIdx);
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/

/*Get ComM mode status*/
FUNC(ComM_ModeType, BSWM_COMM_CODE)
BswM_GetComMIndStatus(NetworkHandleType comChIdx);
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
/*Dcm*/
/*Initialize BswMDcmComModeRequest request port*/
FUNC(void, BSWM_DCM_CODE)
BswM_InitDcmRequestPorts(void);

/*Get BswM_Dcm_ApplicationUpdated status*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_DCM_CODE)
BswM_GetDcmAppUpdatedStatus(void);

/*Get BswM_Dcm_CommunicationMode_CurrentState status*/
FUNC(Dcm_CommunicationModeType, BSWM_DCM_CODE)
BswM_GetDcmCurModeStatus(NetworkHandleType dcmChIdx);
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
/*EcuM*/
/*Initialize BswMEcuMWakeupSource request port*/
FUNC(void, BSWM_ECUM_CODE)
BswM_InitEcuMRequestPorts(void);

/*Get current wake up source status*/
FUNC(EcuM_WakeupStatusType, BSWM_ECUM_CODE)
BswM_GetEcuMWakeSrcStatus(EcuM_WakeupSourceType wkSrcIdx);

FUNC(EcuM_RunStatusType, BSWM_ECUM_CODE)
BswM_GetEcuMRunRqstStatus(EcuM_StateType rqstIdx);

FUNC(EcuM_StateType, BSWM_ECUM_CODE)
BswM_GetEcuMState(void);
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
/*EthIf*/
FUNC(void, BSWM_ETHIF_CODE)
BswM_InitEthIfRequestPorts(void);

/*Get specified ethif port group link state*/
FUNC(EthTrcv_LinkStateType, BSWM_ETHIF_CODE)
BswM_GetEthIfGruStateStatus(EthIf_SwitchPortGroupIdxType ethIdx);
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
/*EthSM*/
/*Initialize BswMEthSMIndication request port*/
FUNC(void, BSWM_ETHSM_CODE)
BswM_InitEthSMRequestPorts(void);

/*Get ethsm indicate status*/
FUNC(EthSM_NetworkModeStateType, BSWM_ETHSM_CODE)
BswM_GetEthSmIndStatus(NetworkHandleType ethsmChIdx);
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_FRSM_ENABLED == STD_ON)
/*FlexRay*/
/*Initialize BswMFrSMIndication request port*/
FUNC(void, BSWM_FRSM_CODE)
BswM_InitFrSMRequestPorts(void);

/*Get frsm indicate status*/
FUNC(FrSM_BswM_StateType, BSWM_FRSM_CODE)
BswM_GetFrSmIndStatus(NetworkHandleType frsmChIdx);
#endif /*BSWM_FRSM_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
/*J1939*/
/*Initialize BswMJ1939DcmBroadcastStatus request port*/
FUNC(void, BSWM_J1939DCM_CODE)
BswM_InitJ1939DcmRequestPorts(void);

/*Get J1939 channel broad status*/
FUNC(boolean, BSWM_J1939DCM_CODE)
BswM_GetJ1939ChBroadStatus(NetworkHandleType j1939ChIdx);
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
/*J1939NM*/
/*Initialize BswMJ1939NmIndication request port*/
FUNC(void, BSWM_J1939NM_CODE)
BswM_InitJ1939NmRequestPorts(void);

/*Get J1939Nm indicate status*/
FUNC(Nm_StateType, BSWM_J1939NM_CODE)
BswM_GetJ1939NmStatus(NetworkHandleType j1939ChIdx);
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
/*Generic request*/
/*Initialize BswMGenericRequest request port*/
FUNC(void, BSWM_CODE)
BswM_InitGenericRequestPorts(void);

/*Get generic request status*/
FUNC(uint16, BSWM_CODE)
BswM_GetGeneRqstStatus(BswM_UserType userIdx);
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
/*Lin*/
/*Initialize BswMLinSMIndication and BswMLinScheduleIndication request port*/
FUNC(void, BSWM_LINSM_CODE)
BswM_InitLinSMRequestPorts(void);

/*Get linsm indicate status*/
FUNC(LinSM_ModeType, BSWM_FRSM_CODE)
BswM_GetLinSmIndStatus(NetworkHandleType linsmChIdx);

/*Get linsm current schedule*/
FUNC(LinIf_SchHandleType, BSWM_FRSM_CODE)
BswM_GetLinSmCurSchedule(NetworkHandleType linsmChIdx);
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
/*Initialize BswMLinTpModeRequest request port*/
FUNC(void, BSWM_LINTP_CODE)
BswM_InitLinTpRequestPorts(void);

/*Get linTp request mode*/
FUNC(LinTp_Mode, BSWM_LINTP_CODE)
BswM_GetLinTpRqstMode(NetworkHandleType linTpChIdx);
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
/*Nvm*/
/*Initialize BswMNvMJobModeIndication and BswMNvMRequest request port*/
FUNC(void, BSWM_NVM_CODE)
BswM_InitNvMRequestPorts(void);

/*Get nvm current block mode*/
FUNC(NvM_RequestResultType, BSWM_NVM_CODE)
BswM_GetCurNvmBlockMode(uint8 blockIdx);

/*Get nvm current Job mode*/
FUNC(NvM_RequestResultType, BSWM_NVM_CODE)
BswM_GetCurNvmJobMode(uint8 jobIdx);
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
/*Sd*/
/*Initialize sd related request port*/
FUNC(void, BSWM_SD_CODE)
BswM_InitSDRequestPorts(void);

/*Get Client Service Current State*/
FUNC(Sd_ClientServiceCurrentStateType, BSWM_NVM_CODE)
BswM_GetClientServiceCurState(uint16 cliIdx);

/*Get Consumed Event Group Current State*/
FUNC(Sd_ConsumedEventGroupCurrentStateType, BSWM_NVM_CODE)
BswM_GetEvGruCurState(uint16 evIdx);

/*Get Event Handler Current State*/
FUNC(Sd_EventHandlerCurrentStateType, BSWM_NVM_CODE)
BswM_GetEvHandlerCurState(uint16 handlerIdx);
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_RTE_ENABLED == STD_ON)
/*Swc*/
/*Initialize swc related request port*/
FUNC(void, BSWM_SWC_CODE)
BswM_InitSwcRequestPorts(void);

/*Get swc indicate status*/
FUNC(BswM_ModeType, BSWM_CANSM_CODE)
BswM_GetSwcModeIndStatus(BswM_ModeGroupType swcChIdx);

/*Get swc request status*/
FUNC(BswM_ModeType, BSWM_CANSM_CODE)
BswM_GetSwcRqstStatus(BswM_ModeGroupType swcChIdx);

/*Get BswMModeSwitchErrorEvent*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_NMIF_CODE)
BswM_GetModeSwiErrNoti(uint16 portId);
#endif /*BSWM_RTE_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
/*Event request port*/
/*Get Nm wake-up indication*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_NMIF_CODE)
BswM_GetNmWkInd(NetworkHandleType nmIdx);
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
/*Get BswM_BswMPartitionRestarted status*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_DCM_CODE)
BswM_GetParRestartStatus(void);
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_WDGM_ENABLED == STD_ON)
/*Get BswM_WdgM_RequestPartitionReset*/
FUNC(BswM_EventRquestPortRuntimeType, BSWM_NMIF_CODE)
BswM_GetWgmParReset(uint8 wdgIdx);
#endif /*BSWM_WDGM_ENABLED == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
/*BswM Timer*/
FUNC(void, BSWM_CODE) BswM_InitBswMTimer(void);

FUNC(BswM_TimerEnumType, BSWM_CODE) BswM_GerTimerStatus(BswM_TimerIdxType timerIdx);
#endif /*BSWM_TIMER_ENABLED == STD_ON*/

/*Execute the arbitration the specific rule in a specific partition*/
FUNC(void, BSWM_CODE)
BswM_ArbitrateRule(BswM_RuleIndexType ruleIdx, ApplicationType partIdx);

/*Available action*/
#if (BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON)
/*Action: Clear event request port*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoClrEventRqst(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON)
/*Action: ComM Allow COM*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMAllowCom(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON)
/*Action: ComM Mode limitation*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMModeLimit(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON)
/*Action: ComM Mode Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMModeSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON*/

#if (BSWM_ECUM_VERSION == BSWM_V_4_2_2)
/*Action: EcuM Go Down*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoDown(ApplicationType partIdx, uint8 actionIdx);

/*Action: EcuM Go Halt*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoHalt(ApplicationType partIdx, uint8 actionIdx);

/*Action: EcuM Go Poll*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoPoll(ApplicationType partIdx, uint8 actionIdx);
#else
#if (BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON)
/*Action: BswMEcuMGoDownHaltPoll*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoDownHaltPoll(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON*/
#endif /*BSWM_ECUM_VERSION == BSWM_V_4_2_2*/

#if (BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON)
/*Action: BswMEcuMDriverInitListBswM*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMInitBswM(ApplicationType partIdx, uint8 actionIdx);
#endif

#if (BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON)
/*Action: EcuM Select Shut Down Target*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMSelectShutTgt(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON)
/*Action: EcuM State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMStateSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON)
/*Action: EthIf switch port group link state*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEthIfSwiPortGruRqst(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON)
/*Action: FrSM All Slots*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoFrSMAllSlots(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON)
/*Action: J1939Dcm State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoJ1939DcmStateSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON)
/*Action: Lin Schedule Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoLinScheduleSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Client Service Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdCliServiceModeReq(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Consumed Event Group Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdEvGrpModeReq(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Server Service Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdSvrServiceModeReq(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_NMCONTROL_ENABLED == STD_ON)
/* Action: nm control */
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoNMControl(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_NMCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON)
/*Action: COM deadline monitor control*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoDMControl(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON)
/*Action: PDU Group Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoPduGrpSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON)
/*Action: Switch IPDU Mode*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSwitchIpduMode(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON)
/*Action: Trigger IPDU Send*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoTriggerIpduSend(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON)
/*Action: J1939Rm State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoJ1939RmStateSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON)
/*Action: PDU Router Control*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoPduRouterControl(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON)
/*Action: BswMCoreHaltMode*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoCoreHaltMode(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON)
/*Action: Call user function*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoUserCallout(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON)
/*Action: RTE Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteModeRequest(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESWITCH_ENABLED == STD_ON)
/*Action: RTE Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_RTESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESTART_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteStart(void);
#endif /*BSWM_ACTION_RTESTART_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESTOP_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteStop(void);
#endif /*BSWM_ACTION_RTESTOP_ENABLED == STD_ON*/

#if (BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSchMSwitch(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON)
/*Action: Timer control*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoTimerControl(ApplicationType partIdx, uint8 actionIdx);
#endif /*BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON*/

#endif /* BSWM_INTERNAL_H */
