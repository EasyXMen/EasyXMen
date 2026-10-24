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
 ************************************************************************************************************************
 **
 **  @file               : BswM_Internal.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_INTERNAL_H
#define BSWM_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "BswM.h"
#include "BswM_Cfg.h"
#include "SchM_BswM.h"
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /* BSWM_DEV_ERROR_DETECT == STD_ON */
#include "BswM_Arti.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/** clear set request state */
#define BSWM_EVENT_RQST_STATE_CLEARED 0x00u
/** set request state */
#define BSWM_EVENT_RQST_STATE_SET     0x01u

/** u8 invalid value definition */
#define BSWM_INVALID_U8  (uint8)(0xffu)
/** u16 invalid value definition */
#define BSWM_INVALID_U16 (uint16)(0xffffu)
/** u32 invalid value definition */
#define BSWM_INVALID_U32 (uint32)(0xffffffffuL)

/** u8 invalid value definition */
#define BSWM_INVALID_LONG_U8 (0xffuL)

/** max number of action list */
#define BSWM_MAX_ACTIONLIST_NUM BSWM_INVALID_U16

#if !defined(BSWM_LOCAL)
/** BSWM_LOCAL is used to define local variables */
#define BSWM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#if !defined(BSWM_LOCAL_INLINE)
/** BSWM_LOCAL_INLINE is used to define local inline functions */
#define BSWM_LOCAL_INLINE static inline
#endif

/* ================================================ type definitions ================================================ */
/**
 * @brief type definition for event request port Pre-compile time configure
 *
 */
typedef struct
{
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON) /*EventRequestPort configuration exists*/
#if (BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON)
    const BswM_EventRqstPortIdxType* comMInitRstIdxPtr; /**> BswMComMInitReset. @range None */
#endif                                                  /*BSWM_EVENT_COMM_INITIATE_RESET_LIST_ENABLE == STD_ON*/

#if (BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON)
    const BswM_EventRqstPortIdxType* dcmAppUpdateIndIdxPtr; /**> BswMDcmAppUpdateIndication. @range None */
#endif /*BSWM_EVENT_DCM_APPLICATION_UPDATED_INDICATION_ENABLE == STD_ON*/

#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)
    uint16                           numOfModeSwiErrEv; /**> BswMModeSwitchErrorEvent. @range 0~0xFFFF */
    const BswM_EventRqstPortIdxType* modeSwiErrEvPtr;   /**> BswMModeSwitchErrorEvent. @range None */
#endif                                                  /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/

#if (BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON)
    NetworkHandleType                numOfNmWkCh;      /**> BswMNmCarWakeUpIndication. @range 0~0xFFFF */
    const BswM_EventRqstPortIdxType* nmWkEvPortIdxPtr; /**> BswMNmCarWakeUpIndication. @range None */
#endif                                                 /*BSWM_EVENT_NM_CAR_WAKEUP_INDICATION_ENABLE == STD_ON*/

#if (BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON)
    const BswM_EventRqstPortIdxType* parRestartIdxPtr; /**> BswMNmCarWakeUpIndication. @range None */
#endif                                                 /*BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON*/

#if (BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON)
    uint16                           numOfModeSwiAckEv; /**> BswMModeSwitchAckNotification. @range 0~0xFFFF */
    const BswM_EventRqstPortIdxType* modeSwiAckNotiPtr; /**> BswMModeSwitchAckNotification. @range None */
#endif                                                  /*BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON*/

#else  /*EventRequestPort configuration don't exist*/
    uint8 dummy; /**> dummy. @range None */
#endif /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
} BswM_EvRqstPortPCCfgType;

/**
 * @brief type definition for mode request port Pre-compile time configure
 *
 */
typedef struct
{
#if (BSWM_MODE_RQSTPORT_ENABLE == STD_ON) /*ModeRequestPort configuration exists*/
#if (BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON)
    uint8 numOfBswModeNoti; /**> BswMBswModeNotification. @range 0~0xFF */
#endif                      /*BSWM_BSWMODENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
    NetworkHandleType                 numOfCanSMInd;  /**> BswMCanSMIndication. @range 0~0xFFFF */
    const CanSM_BswMCurrentStateType* canSMInitState; /**> BswMCanSMIndication. @range None */
#endif                                                /*BSWM_CANSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
    NetworkHandleType    numOfComMInd; /**> BswMComMIndication. @range 0~0xFFFF */
    const ComM_ModeType* comMInitMode; /**> BswMComMIndication. @range None */
#endif                                 /* BSWM_COMM_ENABLED == STD_ON */

#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
    PNCHandleType           numOfComMPncInd; /**> BswMComMPncRequest. @range 0~0xFFFF */
    const ComM_PncModeType* comMPncInitMode; /**> BswMComMPncRequest. @range None */
#endif                                       /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/

#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
    NetworkHandleType                numOfDcmComModeReq; /*BswMDcmComModeRequest. @range 0~0xFFFF */
    const Dcm_CommunicationModeType* dcmInitMode;        /**> BswMDcmComModeRequest. @range None  */
#endif                                                   /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
    uint8                     numOfEcuMRunRqstSource; /**> BswMEcuMRUNRequestIndication. @range 0~0xFF  */
    const EcuM_RunStatusType* ecumRunRqstInitstate;   /**> BswMEcuMRUNRequestIndication. @range None */
#endif                                                /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/

#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
    EcuM_WakeupSourceType        numOfEcuMWakeupSource; /**> BswMEcuMWakeupSource. @range 0~0xFF */
    const EcuM_WakeupStatusType* ecumWkInitstate;       /**> BswMEcuMWakeupSource. @range None */
#endif                                                  /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/

#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
    EthIf_SwitchPortGroupIdxType numOfEthIfPG;     /**> BswMEthIfPortGroupLinkStateChg. @range 0~0xFF */
    const EthTrcv_LinkStateType* ethIfPGInitState; /**> BswMEthIfPortGroupLinkStateChg. @range None */
#endif                                             /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/

#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
    NetworkHandleType                 numOfEthSMInd;  /**> BswMEthSMIndication. @range 0~0xFFFF */
    const EthSM_NetworkModeStateType* ethsmInitState; /**> BswMEthSMIndication. @range None */
#endif                                                /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
    BswM_UserType numOfGenericReq; /**> BswMGenericRequest. @range 0~0xFF */
    const uint16* genRqstInitMode; /**> BswMGenericRequest. @range None */
#endif                             /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
    NetworkHandleType numOfj1939DcmCh;    /**> BswMJ1939DcmBroadcastStatus. @range 0~0xFFFF */
    const boolean*    j1939DcmInitStatus; /**> BswMJ1939DcmBroadcastStatus. @range None */
#endif                                    /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/

#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
    NetworkHandleType   numOfJ1939NmInd;  /**> BswMJ1939NmIndication. @range 0~0xFFFF */
    const Nm_StateType* j1939NmInitState; /**> BswMJ1939NmIndication. @range None */
#endif                                    /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
    NetworkHandleType     numOfLinSMInd;  /**> BswMLinSMIndication. @range 0~0xFFFF */
    const LinSM_ModeType* linsmInitState; /**> BswMLinSMIndication. @range None */
#endif                                    /*BSWM_LINSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
    LinIf_SchHandleType numOfLinSchInd; /**> BswMLinScheduleIndication. @range 0~0xFFFF */
#endif                                  /* BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON */

#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
    NetworkHandleType numOfLinTpModeReq; /**> BswMLinTpModeRequest. @range 0~0xFFFF */
    const LinTp_Mode* lintpRqstInitMode; /**> BswMLinTpModeRequest. @range None */
#endif                                   /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
    NetworkHandleType   numOfNmChannel; /**> BswMNmStateChangeNotification. @range 0~0xFFFF */
    const Nm_StateType* nmInitStatus;   /**> BswMNmStateChangeNotification. @range None */
#endif                                  /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
    uint16                       numOfNvMJob;    /**> BswMNvMJobModeIndication. @range 0~0xFFFF */
    const NvM_RequestResultType* nvmJobInitMode; /**> BswMNvMJobModeIndication. @range None */
#endif                                           /*BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON*/

#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
    NvM_BlockIdType              numOfNvMBlock; /**> BswMNvMRequest. @range 0~0xFFFF */
    const NvM_RequestResultType* nvmInitMode;   /**> BswMNvMRequest. @range None */
#endif                                          /*BSWM_NVMREQUEST_ENABLED == STD_ON*/

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
    uint16 numOfSdCliSrvCurState;                               /**> BswMSdClientServiceCurrentState. @range 0~0xFFFF */
    const Sd_ClientServiceCurrentStateType* curClientInitState; /**> BswMSdClientServiceCurrentState. @range None */
#endif                                                          /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
    uint16 numOfSdConsumedEvGrpCurrState; /**> BswMSdConsumedEventGroupCurrentState. @range 0~0xFFFF */
    const Sd_ConsumedEventGroupCurrentStateType*
        consEvGroupInitState; /**> BswMSdConsumedEventGroupCurrentState. @range None */
#endif                        /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
    uint16 numOfSdEvHdlerCurrState;                             /**> BswMSdEventHandlerCurrentState. @range 0~0xFFFF */
    const Sd_EventHandlerCurrentStateType* evHandlerInitStatus; /**> BswMSdEventHandlerCurrentState. @range None */
#endif                                                          /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SOADSOCONMODECHG_ENABLED == STD_ON)
    uint16                    numOfSoadSoConMode;    /**> BswMSoAdSoConModeChg. @range 0~0xFFFF */
    const SoAd_SoConModeType* curSoComModeInitState; /**> BswMSoAdSoConModeChg. @range None */
#endif                                               /* BSWM_SOADSOCONMODECHG_ENABLED == STD_ON */

#if (BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON)
    uint8 numOfSwcModeNoti; /**> BswMSwcModeNotification. @range 0~0xFF */
#endif                      /*BSWM_SWCMODENOTIFICATION_ENABLED == STD_ON */

#if (BSWM_SWCMODEREQUEST_ENABLED == STD_ON)
    uint8 numOfSwcModeReq; /**> BswMSwcModeRequest. @range 0~0xFF */
#endif                     /*BSWM_SWCMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
    BswM_TimerIdxType numOfBswMTimer; /**> BswMTimer. @range 0~0xFF */
#endif                                /*BSWM_TIMER_ENABLED == STD_ON*/

#else  /*ModeRequestPort configuration don't exist*/
    uint8 dummy; /**> dummy. @range 0~0xFF */
#endif /*BSWM_MODE_RQSTPORT_ENABLE == STD_ON*/
} BswM_ModeRqstPortPCCfgType;

/**
 * @brief type definition of runtime information for BswMModeRequestPort
 *
 */
typedef struct
{
#if (BSWM_CANSMINDICATION_ENABLED == STD_ON)
    /**> BswMCanSMIndication. @range None */
    CanSM_BswMCurrentStateType* canSMIndiStatus;
#endif /*BSWM_CANSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_COMMINDICATION_ENABLED == STD_ON)
    /**> BswMComMIndication. @range None */
    ComM_ModeType* comMIndiStatus;
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_COMMPNCREQUEST_ENABLED == STD_ON)
    /**> BswMComMPncRequest. @range None */
    ComM_PncModeType* comMPncReqStatus;
#endif /*BSWM_COMMPNCREQUEST_ENABLED == STD_ON*/

#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
    /**> BswMDcmComModeRequest. @range None */
    Dcm_CommunicationModeType* dcmComModeReqStatus;
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON)
    /**> BswMEcuMRUNRequestIndication. @range None */
    EcuM_RunStatusType* ecuMRunRqstStatus;
#endif /*BSWM_ECUMRUNREQUESTINDICATION_ENABLED == STD_ON*/

#if (BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON)
    /**> BswMEcuMWakeupSource. @range None */
    EcuM_WakeupStatusType* ecuMWakeupSrcStatus;
#endif /*BSWM_ECUMWAKEUPSOURCE_ENABLED == STD_ON*/

#if (BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON)
    /**> BswMEthIfPortGroupLinkStateChg. @range None */
    EthTrcv_LinkStateType* ethIfPGStatus;
#endif /*BSWM_ETHIFPORTGROUPLINKSTATECHG_ENABLED == STD_ON*/

#if (BSWM_ETHSMINDICATION_ENABLED == STD_ON)
    /**> BswMEthSMIndication. @range None */
    EthSM_NetworkModeStateType* ethSMIndiStatus;
#endif /*BSWM_ETHSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_GENERICREQUEST_ENABLED == STD_ON)
    /**> BswMGenericRequest. @range None */
    uint16* genericReqStatus;
#endif /*BSWM_GENERICREQUEST_ENABLED == STD_ON*/

#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
    /**> BswMJ1939DcmBroadcastStatus. @range None */
    boolean* j1939DcmBrdcstStatus;
#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/

#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
    /**> BswMJ1939NmIndication. @range None */
    Nm_StateType* j1939NmIndiStatus;
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSMINDICATION_ENABLED == STD_ON)
    /**> BswMLinSMIndication. @range None */
    LinSM_ModeType* linSMIndiStatus;
#endif /*BSWM_LINSMINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON)
    /**> BswMLinScheduleIndication. @range None */
    LinIf_SchHandleType* linSchIndiStatus;
#endif /*BSWM_LINSCHEDULEINDICATION_ENABLED == STD_ON*/

#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
    /**> BswMLinTpModeRequest. @range None */
    LinTp_Mode* linTpModeReqStatus;
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
    /**> BswMNmStateChangeNotification. @range None */
    Nm_StateType* NmModeReqStatus;
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

#if (BSWM_NVMJOBMODEINDICATION_ENABLED == STD_ON)
    /**> BswMNvMJobModeIndication. @range None */
    NvM_RequestResultType* nvMJobModeIndiStatus;
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
    /**> BswMNvMRequest. @range None */
    NvM_RequestResultType* nvMReqStatus;
#endif /*BSWM_NVMREQUEST_ENABLED == STD_ON*/

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
    /**> BswMSdClientServiceCurrentState. @range None */
    Sd_ClientServiceCurrentStateType* sdCliServiceCurrStaStatus;
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
    /**> BswMSdConsumedEventGroupCurrentState. @range None */
    Sd_ConsumedEventGroupCurrentStateType* sdConsumedEvGrpCurrStaStatus;
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
    /**> BswMSdEventHandlerCurrentState. @range None */
    Sd_EventHandlerCurrentStateType* sdEvHdlerCurrStaStatus;
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SOADSOCONMODECHG_ENABLED == STD_ON)
    /**> The state of the SoAd socket connection. @range None */
    SoAd_SoConModeType* soAdSoConModeState;
#endif /*BSWM_SOADSOCONMODECHG_ENABLED == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
    BswM_TimerEnumType* timerStatus; /**> BswMTimer. @range None */
    uint32*             timerCycle;  /**> BswMTimer. @range None */
#endif                               /*BSWM_TIMER_ENABLED == STD_ON*/
} BswM_ModeRqstPortRuntimeType;

/*type definition of runtime information for BswMEventRequestPort*/
typedef uint8 BswM_EventRquestPortRuntimeType;

/**
 * @brief type definition for BSWM partition Pre-compile time configure
 *
 */
typedef struct
{
#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partitionId;                     /**> id of this partition. @range 0 ~ 0xFFFF */
#endif                                               /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)           /*EventRequestPort configuration exists*/
    BswM_EventRqstPortIdxType       evRqstSrcNum;    /**> number of event request source. @range 0 ~ 0xFF */
    const BswM_EvRqstPortPCCfgType* evRqstPCCfg;     /**> pointer to event request port configuration. @range None */
#endif                                               /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
    const BswM_ModeRqstPortPCCfgType* modeRqstPCCfg; /**> pointer to mode request port configuration. @range None */
#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)           /*EventRequestPort configuration exists*/
    BswM_EventRquestPortRuntimeType* eventRqstPortRunPtr; /**> pointer to runtime information. @range None */
#endif                                                    /*BSWM_EVENT_RQSTPORT_ENABLE == STD_ON*/
    BswM_ModeRqstPortRuntimeType* modeRqstPortRunPtr;     /**> pointer to runtime information. @range None */
} BswM_PartitionPCCfgType;

/**
 * @brief runtime information for BSWM module in a core
 *
 */
typedef struct
{
    BswM_InitStatusType bswmInitState; /**> Initial state of BSWM module. @range 0 ~ 1*/
#if (BSWM_ECUM_ENABLED == STD_ON)
    EcuM_StateType ecumState; /**> State of EcuM module. @range 0 ~ 0xFF */
#endif                        /*BSWM_ECUM_ENABLED == STD_ON*/
} BswM_RuntimeStatusType;

/**
 * @brief type definition for BSWM Pre-compile time configuration root in core
 *
 */
typedef struct
{
#if (BSWM_MAX_NUM_CORE > 1u)
    CoreIdType bswmCoreId; /**> the core these BSWM belong to. @range 0 ~ 0xFF */
#endif
    uint8                   numOfPartitions;  /**> Number of partitions in this core. @range 0 ~ 0xFF */
    BswM_RuntimeStatusType* runtimeStatusPtr; /**>  Each core state of BswM is recorded*. @range None */
} BswM_PCCfgType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (BSWM_CLONE_VARIABLE_SUPPORT_ENABLE == STD_ON)
#define BSWM_START_SEC_VAR_CLONE_UNSPECIFIED
#include "BswM_MemMap.h"
extern BswM_RuntimeStatusType BswM_RuntimeStatus;
#define BSWM_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "BswM_MemMap.h"
#endif /*BSWM_CLONE_VARIABLE_SUPPORT_ENABLE == STD_ON*/

/* ========================================= external function declarations ========================================= */

#if (BSWM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Checks the initialization status of the BswM core.
 * @param[in] sid : Service ID for the DET error reporting.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized.
 * @retval E_NOT_OK : The BswM core is not initialized, and a DET error is reported.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66256
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_ChkBswMInitStatus(uint8 sid);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_MULTI_PARTITION_ENABLED == STD_ON)
/**
 * @brief Checks if the BswM core is initialized for partition restart.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized for partition restart.
 * @retval E_NOT_OK : The BswM core is not initialized for partition restart.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66257
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkParRestart(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
/**
 * @brief Checks the BswM core initialization status and validates the CanSM current state.
 * @param[in] CurrentState : The current state of the CanSM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the CanSM current state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the CanSM current state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66258
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkCanSMInd(CanSM_BswMCurrentStateType CurrentState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
/**
 * @brief Checks the BswM core initialization status and validates the ComM requested mode.
 * @param[in] RequestedMode : The requested mode for ComM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the ComM requested mode is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the ComM requested mode is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66259
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkComMInd(ComM_ModeType RequestedMode);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
/**
 * @brief Checks the BswM core initialization status and validates the ComM current PNC mode.
 * @param[in] CurrentPncMode : The current PNC mode for ComM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the ComM current PNC mode is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the ComM current PNC mode is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66261
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkComMPnc(ComM_PncModeType CurrentPncMode);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/

/**
 * @brief Checks the BswM core initialization status for initiating a reset.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and ready for reset.
 * @retval E_NOT_OK : The BswM core is not initialized or not ready for reset.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66263
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkComMInitRst(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
/**
 * @brief Checks the BswM core initialization status for DCM application update.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and ready for DCM application update.
 * @retval E_NOT_OK : The BswM core is not initialized or not ready for DCM application update.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66264
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkDcmUpdate(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the current communication mode state of DCM and validates the requested mode.
 * @param[in] RequestedMode : The requested DCM communication mode.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the requested mode is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the requested mode is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66265
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkDcmModeCurState(Dcm_CommunicationModeType RequestedMode);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
/**
 * @brief Checks the current wake-up status of EcuM and validates the provided state.
 * @param[in] state : The current wake-up status of EcuM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the provided state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the provided state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66266
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEcuMCurWakeUp(EcuM_WakeupStatusType state);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Validates the requested EcuM state and the current run status.
 * @param[in] State : The requested EcuM state.
 * @param[in] CurrentStatus : The current run status of EcuM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized, the requested state is valid, and the current status is within range.
 * @retval E_NOT_OK : The BswM core is not initialized, the requested state is out of range, or the current status is
 * out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66267
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkRequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the initialization status of BswM for the current EcuM state.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66269
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEcuMCurState(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
/**
 * @brief Validates the Ethernet port group link state.
 * @param[in] PortGroupState : The link state of the Ethernet port group.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the port group state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the port group state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66270
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEthIfPortGru(EthTrcv_LinkStateType PortGroupState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
/**
 * @brief Validates the current state of the Ethernet State Machine (EthSM).
 * @param[in] CurrentState : The current state of the EthSM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66271
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEthSMInd(EthSM_NetworkModeStateType CurrentState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status of the J1939 DCM broadcast feature.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the J1939 DCM broadcast feature is ready.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66273
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkJ1939DcmBroad(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status of the J1939 Network Management (Nm) state change notification.
 * @param[in] NmState : The state for the NetWork Management Protocol.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the J1939 Nm state change notification is ready.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66274
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkJ1939NmInd(Nm_StateType NmState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status of the LIN State Machine (LinSM) current schedule.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the LIN SM current schedule is ready.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66275
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkLinSMCurSch(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the initialization status and validity of the LIN State Machine (LinSM) current state.
 * @param[in] CurrentState : The current state of the LIN State Machine.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66276
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkLinSMInd(LinSM_ModeType CurrentState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status and validity of the LIN Transfer Protocol (LinTp) requested mode.
 * @param[in] LinTpRequestedMode : The requested mode for the LIN Transfer Protocol.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the requested mode is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the requested mode is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66277
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkLinTpRqst(LinTp_Mode LinTpRequestedMode);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status for the Network Management (Nm) Car Wakeup Indication.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the Car Wakeup Indication can proceed.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66278
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkNmCarWkUp(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the initialization status for the Network Management (Nm) State Change Notification.
 * @param[in] currentState : The current state of Network Management.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the State Change Notification can proceed.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66279
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkNmStateChange(Nm_StateType currentState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
/**
 * @brief Checks the current NvM block mode.
 * @param[in] CurrentBlockMode : The current block mode of NvM.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current block mode is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current block mode is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66280
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkCurNvmBlockMode(NvM_RequestResultType CurrentBlockMode);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the current NvM job mode.
 * @param[in] MultiBlockRequest : The multi-block request type for NvM operations.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the multi-block request type is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the multi-block request type is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66281
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkCurNvmJobMode(NvM_MultiBlockRequestType MultiBlockRequest);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
/**
 * @brief Checks the validity of a generic request from a specific user.
 * @param[in] requestingUser : The user making the generic request.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the requesting user is valid.
 * @retval E_NOT_OK : The BswM core is not initialized, the configuration is invalid, or the requesting user is out of
 * range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66282
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkGenericRqst(BswM_UserType requestingUser);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
/**
 * @brief Checks the validity of the current state of a client service.
 * @param[in] CurrentClientState : The current state of the client service.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current client state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current client state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66283
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkClientCurState(Sd_ClientServiceCurrentStateType CurrentClientState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the validity of the current state of a consumed event group.
 * @param[in] ConsumedEventGroupState : The current state of the consumed event group.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current consumed event group state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current consumed event group state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66284
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEvGruCurState(Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Checks the validity of the current state of an event handler.
 * @param[in] EventHandlerStatus : The current state of the event handler.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized and the current event handler state is valid.
 * @retval E_NOT_OK : The BswM core is not initialized or the current event handler state is out of range.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66285
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkEvHandlerCurState(Sd_EventHandlerCurrentStateType EventHandlerStatus);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_SOAD_ENABLED == STD_ON)
/**
 * @brief Checks the initialization status of the BswM core for SoAd socket connection mode change.
 * @return Std_ReturnType
 * @retval E_OK : The BswM core is initialized.
 * @retval E_NOT_OK : The BswM core is not initialized.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66286
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Std_ReturnType BswM_DetChkSoAdSoConModeChg(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SOAD_ENABLED == STD_ON*/
#endif /*BSWM_DEV_ERROR_DETECT == STD_ON*/

#if (BSWM_CANSM_ENABLED == STD_ON)
/**
 * @brief Gets the current indication status of the CanSM module.
 * @param[in] cansmChIdx : The channel index of the CanSM module.
 * @return CanSM_BswMCurrentStateType
 * @retval CANSM_BSWM_NO_COMMUNICATION : No communication is established.
 * @retval Other values : The current indication status of the CanSM module.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66229
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
CanSM_BswMCurrentStateType BswM_GetCanSmIndStatus(NetworkHandleType cansmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Initializes the request ports for the CanSM module.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66228
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitCanSMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_CANSM_ENABLED == STD_ON*/

#if (BSWM_COMM_ENABLED == STD_ON)
/**
 * @brief Initializes the request ports for the ComM module.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66235
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitComMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the ComM reset initialization status.
 * @return BswM_EventRquestPortRuntimeType The reset initialization status.
 * @retval BSWM_INVALID_U8 If the status cannot be retrieved.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66241
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetComMResetInitStatus(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_COMM_PNC_ENABLED == STD_ON)
/**
 * @brief Gets the ComM Pnc status for a given Pnc handle index.
 * @param[in] pncIdx : Index of the Pnc handle for which the status is requested.
 * @return ComM_PncModeType The Pnc status.
 * @retval COMM_PNC_NO_COMMUNICATION If the status cannot be retrieved or no communication.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66240
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
ComM_PncModeType BswM_GetComMPncStatus(PNCHandleType pncIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_COMM_PNC_ENABLED == STD_ON*/

/**
 * @brief Gets the ComM indication status for a given network channel index.
 * @param[in] comChIdx : Index of the network channel for which the status is requested.
 * @return ComM_ModeType The indication status.
 * @retval BSWM_INVALID_U8 If the status cannot be retrieved.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66242
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
ComM_ModeType BswM_GetComMIndStatus(NetworkHandleType comChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_COMM_ENABLED == STD_ON*/

#if (BSWM_DCM_ENABLED == STD_ON)
#if (BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON)
/**
 * @brief Initializes the DCM request ports with the initial modes.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66243
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitDcmRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the updated status of the DCM application.
 * @return BswM_EventRquestPortRuntimeType The updated status of the DCM application.
 * @retval BSWM_INVALID_U8 If the status cannot be retrieved or the feature is disabled.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66254
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetDcmAppUpdatedStatus(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current communication mode status of a DCM channel.
 * @param[in] dcmChIdx : The index of the DCM channel for which the current mode status is requested.
 * @return Dcm_CommunicationModeType The current communication mode status of the DCM channel.
 * @retval BSWM_INVALID_U8 If the status cannot be retrieved or the configuration pointer is invalid.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66252
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Dcm_CommunicationModeType BswM_GetDcmCurModeStatus(NetworkHandleType dcmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_DCMCOMMODEREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_DCM_ENABLED == STD_ON*/

#if (BSWM_ECUM_ENABLED == STD_ON)
/**
 * @brief Initializes the EcuM request ports.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66287
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitEcuMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the status of a specified EcuM wakeup source.
 * @param[in] wkSrcIdx : Index of the wakeup source whose status is to be retrieved.
 * @return EcuM_WakeupStatusType
 * @retval BSWM_INVALID_U8 : If the partition configuration retrieval fails or the index is invalid.
 * @retval Valid status : The actual status of the specified wakeup source.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66289
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
EcuM_WakeupStatusType BswM_GetEcuMWakeSrcStatus(EcuM_WakeupSourceType wkSrcIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the run request status of a specified EcuM state.
 * @param[in] rqstIdx : Index of the state whose run request status is to be retrieved.
 * @return EcuM_RunStatusType
 * @retval BSWM_INVALID_U8 : If the partition configuration retrieval fails or the index is invalid.
 * @retval Valid status : The actual run request status of the specified state.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66291
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
EcuM_RunStatusType BswM_GetEcuMRunRqstStatus(EcuM_StateType rqstIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current state of the EcuM.
 * @param[in] None
 * @return EcuM_StateType
 * @retval Valid state : The current state of the EcuM.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66293
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
EcuM_StateType BswM_GetEcuMState(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#if (BSWM_ETHIF_ENABLED == STD_ON)
/**
 * @brief Initializes the Ethernet Interface request ports.
 * @param[in] None
 * @return None
 * @retval None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66294
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitEthIfRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the link state status of an Ethernet Interface port group.
 * @param[in] ethIdx : Index of the Ethernet Interface port group.
 * @return EthTrcv_LinkStateType
 * @retval ETHTRCV_LINK_STATE_DOWN : The port group is down.
 * @retval Other values : The current link state of the port group.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66295
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
EthTrcv_LinkStateType BswM_GetEthIfGruStateStatus(EthIf_SwitchPortGroupIdxType ethIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ETHIF_ENABLED == STD_ON*/

#if (BSWM_ETHSM_ENABLED == STD_ON)
/**
 * @brief This function initializes the EthSM request ports by setting their initial states based on configuration data.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66297
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitEthSMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current state of an Ethernet State Machine (EthSM) channel.
 * @param[in] ethsmChIdx : Index of the EthSM channel.
 * @return EthSM_NetworkModeStateType
 * @retval ETHSM_STATE_OFFLINE : The EthSM channel is offline.
 * @retval Other values : The current state of the EthSM channel.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66298
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
EthSM_NetworkModeStateType BswM_GetEthSmIndStatus(NetworkHandleType ethsmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_ETHSM_ENABLED == STD_ON*/

#if (BSWM_J1939DCM_ENABLED == STD_ON)
#if (BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON)
/**
 * @brief This function initializes the J1939 DCM broadcast status based on the initial configuration.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66310
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitJ1939DcmRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the broadcast status of a J1939 channel.
 * @param[in] j1939ChIdx : Index of the J1939 channel.
 * @return boolean
 * @retval TRUE : The J1939 channel is in broadcast status.
 * @retval FALSE : The J1939 channel is not in broadcast status.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66311
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
boolean BswM_GetJ1939ChBroadStatus(NetworkHandleType j1939ChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_J1939DCMBROADCASTSTATUS_ENABLED == STD_ON*/
#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#if (BSWM_J1939NM_ENABLED == STD_ON)
#if (BSWM_J1939NMINDICATION_ENABLED == STD_ON)
/**
 * @brief This function initializes the J1939 Network Management request ports by setting the initial states.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66314
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitJ1939NmRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the J1939 Network Management status.
 * @param[in] j1939ChIdx : Index of the J1939 channel.
 * @return Nm_StateType
 * @retval NM_STATE_UNINIT : The J1939 channel is uninitialized.
 * @retval Other values : Current state of the J1939 channel.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66315
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Nm_StateType BswM_GetJ1939NmStatus(NetworkHandleType j1939ChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_J1939NMINDICATION_ENABLED == STD_ON*/
#endif /*BSWM_J1939NM_ENABLED == STD_ON*/

#if (BSWM_GENERIC_REQUEST_ENABLED == STD_ON)
/**
 * @brief Gets the status of a generic request port.
 * @param[in] userIdx : Index of the user for which the generic request status is to be retrieved.
 * @return uint16
 * @retval BSWM_INVALID_U16 : If the partition configuration pointer is invalid.
 * @retval genericReqStatus : The status of the generic request port for the specified user.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66222
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
uint16 BswM_GetGeneRqstStatus(BswM_UserType userIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_GENERIC_REQUEST_ENABLED == STD_ON*/

#if (BSWM_LINSM_ENABLED == STD_ON)
/**
 * @brief This function initializes the LIN State Machine request ports by setting their initial states.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66317
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitLinSMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the status of the LIN State Machine indication.
 * @param[in] linsmChIdx : Index of the LIN State Machine channel.
 * @return LinSM_ModeType
 * @retval BSWM_INVALID_U8 : If the partition configuration pointer is invalid.
 * @retval linSMIndiStatus : The current status of the LIN State Machine indication for the specified channel.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66318
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
LinSM_ModeType BswM_GetLinSmIndStatus(NetworkHandleType linsmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current schedule of the LIN State Machine.
 * @param[in] linsmChIdx : Index of the LIN State Machine channel.
 * @return LinIf_SchHandleType
 * @retval BSWM_INVALID_U8 : If the partition configuration pointer is invalid.
 * @retval linSchIndiStatus : The current schedule handle of the LIN State Machine for the specified channel.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66321
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
LinIf_SchHandleType BswM_GetLinSmCurSchedule(NetworkHandleType linsmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#if (BSWM_LINTP_ENABLED == STD_ON)
#if (BSWM_LINTPMODEREQUEST_ENABLED == STD_ON)
/**
 * @brief This function initializes the LIN TP request ports by setting their initial modes based on the configuration.
 * @return void
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66322
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitLinTpRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the requested mode of the LIN TP channel.
 * @param[in] linTpChIdx : Index of the LIN TP channel.
 * @return LinTp_Mode
 * @retval LINTP_APPLICATIVE_SCHEDULE : Default mode if the partition configuration pointer is invalid.
 * @retval linTpModeReqStatus : The requested mode of the LIN TP channel for the specified index.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66323
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
LinTp_Mode BswM_GetLinTpRqstMode(NetworkHandleType linTpChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_LINTPMODEREQUEST_ENABLED == STD_ON*/
#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#if (BSWM_NM_ENABLED == STD_ON)
#if (BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON)
/**
 * @brief This function initializes the NM request ports by setting the initial status of each NM channel.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66326
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitNmRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current Network Management (NM) state for a given NM channel index.
 * @param[in] nmChIdx : Index of the NM channel.
 * @return Nm_StateType
 * @retval NM_STATE_UNINIT : If the partition configuration pointer is invalid.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66327
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Nm_StateType BswM_GetCurNmState(uint8 nmChIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NMSTATECHANGENOTIFICATION_ENABLED == STD_ON*/

/**
 * @brief Gets the Network Management (NM) wakeup indication event request port runtime type.
 * @param[in] nmIdx : Index of the NM channel.
 * @return BswM_EventRquestPortRuntimeType
 * @retval BSWM_INVALID_U8 : Invalid value if the feature is disabled or the partition configuration pointer is invalid.
 * @retval eventRqstPortRunPtr[evIdx] : The event request port runtime type for the specified NM channel index.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66329
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetNmWkInd(NetworkHandleType nmIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NM_ENABLED == STD_ON*/

#if (BSWM_NVM_ENABLED == STD_ON)
/**
 * @brief Initializes the NvM request ports for the BswM module.
 * @details This function initializes the NvM job mode indication and NvM block request status arrays based on the
 * configuration.
 * @note This function is only executed if either BSWM_NVMJOBMODEINDICATION_ENABLED or BSWM_NVMREQUEST_ENABLED is set to
 * STD_ON.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66330
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitNvMRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_NVMREQUEST_ENABLED == STD_ON)
/**
 * @brief Gets the current mode of an NvM block.
 * @param[in] blockIdx : The ID of the NvM block whose mode is to be retrieved.
 * @return NvM_RequestResultType
 * @retval NVM_REQ_OK : The block mode was successfully retrieved.
 * @retval Other values : The block mode could not be retrieved.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66334
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
NvM_RequestResultType BswM_GetCurNvmBlockMode(NvM_BlockIdType blockIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NVMREQUEST_ENABLED == STD_ON*/

/**
 * @brief Gets the current mode of an NvM job.
 * @param[in] jobIdx : The index of the NvM job whose mode is to be retrieved.
 * @return NvM_RequestResultType
 * @retval NVM_REQ_OK : The job mode was successfully retrieved.
 * @retval Other values : The job mode could not be retrieved.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66332
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
NvM_RequestResultType BswM_GetCurNvmJobMode(uint16 jobIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_NVM_ENABLED == STD_ON*/

#if (BSWM_SD_ENABLED == STD_ON)
/**
 * @brief Initializes the state of SD (Software Development) request ports.
 * @details This function initializes the current states of SD client services, consumed event groups, and event
 * handlers based on configuration settings.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66335
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitSDRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON)
/**
 * @brief Retrieves the current state of a client service.
 * @param[in] cliIdx : Index of the client service whose current state is to be retrieved.
 * @return Sd_ClientServiceCurrentStateType
 * @retval SD_CLIENT_SERVICE_DOWN : The client service is in the down state.
 * @retval Other values : The actual current state of the client service.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66337
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Sd_ClientServiceCurrentStateType BswM_GetClientServiceCurState(uint16 cliIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SDCLIENTSERVICECURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON)
/**
 * @brief Retrieves the current state of a consumed event group.
 * @param[in] evIdx : Index of the consumed event group whose current state is to be retrieved.
 * @return Sd_ConsumedEventGroupCurrentStateType
 * @retval SD_CONSUMED_EVENTGROUP_DOWN : The consumed event group is in the down state.
 * @retval Other values : The actual current state of the consumed event group.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66339
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Sd_ConsumedEventGroupCurrentStateType BswM_GetEvGruCurState(uint16 evIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SDCONSUMEDEVENTGROUPCURRENTSTATE_ENABLED == STD_ON*/

#if (BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON)
/**
 * @brief Retrieves the current state of an event handler.
 * @param[in] handlerIdx : Index of the event handler whose current state is to be retrieved.
 * @return Sd_EventHandlerCurrentStateType
 * @retval SD_EVENT_HANDLER_RELEASED : The event handler is in the released state.
 * @retval Other values : The actual current state of the event handler.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66341
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
Sd_EventHandlerCurrentStateType BswM_GetEvHandlerCurState(uint16 handlerIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SDEVENTHANDLERCURRENTSTATE_ENABLED == STD_ON*/
#endif /*BSWM_SD_ENABLED == STD_ON*/

#if (BSWM_SOAD_ENABLED == STD_ON)
/**
 * @brief This function initializes the SoAd request ports by setting the initial states of the SoAd communication
 * modes.
 * @return void
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66342
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitSoAdRequestPorts(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the current state of a SoAd communication mode.
 * @param[in] SoConIdx : Index of the SoAd communication mode whose current state is to be retrieved.
 * @return SoAd_SoConModeType
 * @retval SOAD_SOCON_OFFLINE : The SoAd communication mode is offline.
 * @retval Other values : The actual current state of the SoAd communication mode.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66343
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
SoAd_SoConModeType BswM_GetSoComModeCurState(uint16 SoConIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_SOAD_ENABLED == STD_ON*/

#if (BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON)

/**
 * @brief Gets the mode switch error notification for a specific port.
 * @param[in] portId : Identifier of the port for which the mode switch error notification is to be retrieved.
 * @return BswM_EventRquestPortRuntimeType
 * @retval BSWM_INVALID_U8 : Invalid or no mode switch error notification found.
 * @retval Other values : The actual mode switch error notification value.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66349
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetModeSwiErrNoti(uint16 portId);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_EVENT_MODE_SWITCH_ERROR_EVENT_ENABLE == STD_ON*/

#if (BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON)

/**
 * @brief Gets the mode switch acknowledgment notification for a specific port.
 * @param[in] portId : Identifier of the port for which the mode switch acknowledgment notification is to be retrieved.
 * @return BswM_EventRquestPortRuntimeType
 * @retval BSWM_INVALID_U8 : Invalid or no mode switch acknowledgment notification found.
 * @retval Other values : The actual mode switch acknowledgment notification value.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66350
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetModeSwiAckNoti(uint16 portId);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_EVENT_SWITCH_ACK_NOTIFICATION_ENABLE == STD_ON*/

#if ((BSWM_MULTI_PARTITION_ENABLED == STD_ON) && (BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON))
/**
 * @brief Gets the partition restart status.
 * @return BswM_EventRquestPortRuntimeType
 * @retval BSWM_INVALID_U8 : Invalid or no partition restart status found.
 * @retval Other values : The actual partition restart status value.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66225
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_EventRquestPortRuntimeType BswM_GetParRestartStatus(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_MULTI_PARTITION_ENABLED == STD_ON && BSWM_EVENT_PARTITION_RESTARTED_ENABLE == STD_ON*/

#if (BSWM_TIMER_ENABLED == STD_ON)
/**
 * @brief This function initializes all BswM timers by setting their status to stopped and their cycle to 0.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66351
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_InitBswMTimer(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Gets the status of a specific BswM timer.
 * @param[in] timerIdx : Index of the BswM timer.
 * @return BswM_TimerEnumType
 * @retval BSWM_TIMER_START : Timer is started.
 * @retval BSWM_TIMER_STOP : Timer is stopped.
 * @retval Other values : Other possible timer statuses.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66352
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
BswM_TimerEnumType BswM_GerTimerStatus(BswM_TimerIdxType timerIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_TIMER_ENABLED == STD_ON*/

#if (BSWM_EVENT_RQSTPORT_ENABLE == STD_ON)
/**
 * @brief This function clears the event request port for a specific event source and index.
 * @param evRqstLcCfgPtr The Lcfg point of event source for which the request port is to be cleared.
 * @param index The index of the event request port to be cleared.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-XXXX
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_ClearEventRequestPort(const BswM_RqstPortLcCfgType* evRqstLcCfgPtr, BswM_EventRqstPortIdxType evIdx);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif

/**
 * @brief Retrieves the configuration pointers for a specific partition.
 * @param[out] bswmPartPCCfg : Pointer to the BswM Partition PC configuration.
 * @param[out] bswmPartLCfg : Pointer to the BswM Partition LC configuration.
 * @return boolean
 * @retval TRUE : Configuration pointers are successfully retrieved.
 * @retval FALSE : No matching partition ID found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66353
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
boolean BswM_GetPartitionLcPCCfgPtr(
#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
    const BswM_PartitionPbConfigType** bswmPartPBCfg,
#endif /* BSWM_PB_CONFIG_ENABLED == STD_ON */
    const BswM_PartitionPCCfgType** bswmPartPCCfg,
    const BswM_PartitionLCfgType**  bswmPartLCfg);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Retrieves the configuration pointer for a specific partition.
 * @param[out] bswmPartPCCfg : Pointer to the BswM Partition PC configuration.
 * @return boolean
 * @retval TRUE : Configuration pointer is successfully retrieved.
 * @retval FALSE : No matching partition ID found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66354
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
boolean BswM_GetPartitionPCCfgPtr(const BswM_PartitionPCCfgType** bswmPartPCCfg);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_CLONE_VARIABLE_SUPPORT_ENABLE == STD_OFF)
/**
 * @brief Retrieves the configuration pointer for a specific core.
 * @param[out] bswmCorePCCfgPtr : Pointer to the BswM Core PC configuration.
 * @return boolean
 * @retval TRUE : Configuration pointer is successfully retrieved.
 * @retval FALSE : No matching core ID found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66355
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
boolean BswM_GetCorePCCfgPtr(const BswM_PCCfgType** bswmCorePCCfgPtr);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /* BSWM_CLONE_VARIABLE_SUPPORT_ENABLE == STD_ON */

/**
 * @brief Checks the initialization status of the current core.
 * @return boolean
 * @retval TRUE : The core is initialized.
 * @retval FALSE : The core is not initialized or the configuration pointer is not found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69360
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
boolean BswM_GetCoreInitStatus(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**
 * @brief Executes the arbitration rule for a given partition and rule index.
 * @param[in] bswmPartLCfgPtr : Pointer to the BswM Partition configuration.
 * @param[in] ruleIdx : Index of the rule to be arbitrated.
 * @param[in] nestedTag : Flag indicating whether nested execution is allowed.
 * @return void
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-66226
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void BswM_ArbitrateRule(const BswM_PartitionLCfgType* bswmPartLCfgPtr, BswM_RuleIndexType ruleIdx, boolean nestedTag);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_PB_CONFIG_ENABLED == STD_ON)
/**
 * @brief Retrieves the PB configuration tags.
 * @return uint8 : The PB configuration tags.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69361
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
uint8 BswM_GetPBConfigTags(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#endif /*BSWM_PB_CONFIG_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_INTERNAL_H */
