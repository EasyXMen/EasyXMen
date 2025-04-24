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
**  FILENAME    : BswM_AvbAction.c                                            **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_Internal.h"
#if (BSWM_NM_ENABLED == STD_ON)
#include "Nm.h"
#endif /*BSWM_NM_ENABLED == STD_ON*/
#if (BSWM_COMM_ENABLED == STD_ON)
#include "ComM_BswM.h"
#endif /*BSWM_COMM_ENABLED == STD_ON*/
#if (BSWM_RTE_ENABLED == STD_ON)
#include "Rte_Main.h"
#endif /*BSWM_RTE_ENABLED == STD_ON*/
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

#if (BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON)
/*Action: Clear event request port*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoClrEventRqst(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    BswM_EventRqstPortIdxType clrEvIdx;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    clrEvIdx = actionLCfgPtr->clrEvRqstRef[actionIdx];
    BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].eventRqstPortRunPtr[clrEvIdx] = BSWM_EVENT_IS_CLEARED;
    return ret;
}
#endif /*BSWM_ACTION_CLEAREVENTREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON)
/*Action: ComM Allow COM*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMAllowCom(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ComM_CommunicationAllowed(
        actionLCfgPtr->comMAllowComActions[actionIdx].channel,
        actionLCfgPtr->comMAllowComActions[actionIdx].comAllowed);
    return ret;
}
#endif /*BSWM_ACTION_COMMALLOWCOM_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON)
/*Action: ComM Mode limitation*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMModeLimit(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = ComM_LimitChannelToNoComMode(
        actionLCfgPtr->comMModeLimitActions[actionIdx].channel,
        actionLCfgPtr->comMModeLimitActions[actionIdx].limitToNo);

    return ret;
}
#endif /*BSWM_ACTION_COMMMODELIMITATION_ENABLED == STD_ON*/

#if (BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON)
/*Action: ComM Mode Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoComMModeSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = ComM_RequestComMode(
        actionLCfgPtr->comMModeSwitchActions[actionIdx].user,
        actionLCfgPtr->comMModeSwitchActions[actionIdx].comMode);

    return ret;
}
#endif /*BSWM_ACTION_COMMMODESWITCH_ENABLED == STD_ON*/

#if (BSWM_ECUM_VERSION == BSWM_V_4_2_2)
/*Action: EcuM Go Down*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoDown(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = EcuM_GoDown(actionLCfgPtr->ecuMGoDownHaltPollActions[actionIdx].goDownUser);
    return ret;
}

/*Action: EcuM Go Halt*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoHalt(ApplicationType partIdx, uint8 actionIdx)
{
    Std_ReturnType ret = E_OK;

    BSWM_AVOID_WARNING(partIdx);
    BSWM_AVOID_WARNING(actionIdx);
    ret = EcuM_GoHalt();
    return ret;
}

/*Action: EcuM Go Poll*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoPoll(ApplicationType partIdx, uint8 actionIdx)
{
    Std_ReturnType ret = E_OK;

    BSWM_AVOID_WARNING(partIdx);
    BSWM_AVOID_WARNING(actionIdx);
    ret = EcuM_GoPoll();
    return ret;
}
#else /*BSWM_ECUM_VERSION == BSWM_V_R19_11*/
#if (BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON)
/*Action: BswMEcuMGoDownHaltPoll*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMGoDownHaltPoll(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = EcuM_GoDownHaltPoll(actionLCfgPtr->ecuMGoDownHaltPollActions[actionIdx].goDownUser);
    return ret;
}
#endif /*BSWM_ACTION_ECUMGODOWNHALTPOLL_ENABLED == STD_ON*/
#endif /*BSWM_ECUM_VERSION == BSWM_V_4_2_2*/

#if (BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON)
/*Action: BswMEcuMDriverInitListBswM*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMInitBswM(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    EcuM_AL_DriverInitBswM(actionLCfgPtr->ecuMDrvInitListActions[actionIdx].ecumDrvInitList);
    return ret;
}
#endif /*BSWM_ACTION_ECUMDRIVERINITLIST_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON)
/*Action: EcuM Select Shut Down Target*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMSelectShutTgt(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = EcuM_SelectShutdownTarget(
        actionLCfgPtr->ecuMSelectShutTgtActions[actionIdx].target,
        actionLCfgPtr->ecuMSelectShutTgtActions[actionIdx].mode);
    return ret;
}
#endif /*BSWM_ACTION_ECUMSELECTSHUTDOWNTARGET_ENABLED == STD_ON*/

#if (BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON)
/*Action: EcuM State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEcuMStateSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    EcuM_SetState(actionLCfgPtr->ecuMStateSwitchActions[actionIdx].ecuMState);
    return ret;
}
#endif /*BSWM_ACTION_ECUMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON)
/*Action: EthIf switch port group link state*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoEthIfSwiPortGruRqst(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = EthIf_SwitchPortGroupRequestMode(
        actionLCfgPtr->ethifSwiPortGruRqstActions[actionIdx].ethSwiGruIdx,
        actionLCfgPtr->ethifSwiPortGruRqstActions[actionIdx].ethTrcvMode);
    return ret;
}
#endif /*BSWM_ACTION_ETHIFSWITCHPORTGROUPREQUESTMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON)
/*Action: FrSM All Slots*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoFrSMAllSlots(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = FrSm_AllSlots(actionLCfgPtr->frSMAllSlotsActions[actionIdx].channel);
    return ret;
}
#endif /*BSWM_ACTION_FRSMALLSLOTS_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON)
/*Action: J1939Dcm State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoJ1939DcmStateSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = J1939Dcm_SetState(
        actionLCfgPtr->j1939DcmStateSwitchActions[actionIdx].channel,
        actionLCfgPtr->j1939DcmStateSwitchActions[actionIdx].node,
        actionLCfgPtr->j1939DcmStateSwitchActions[actionIdx].state);
    return ret;
}
#endif /*BSWM_ACTION_J1939DCMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON)
/*Action: Lin Schedule Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoLinScheduleSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    /*call the action function with configured parameters*/
    ret = LinSM_ScheduleRequest(
        actionLCfgPtr->linSchdlSwitchActionLCfgs[actionIdx].network,
        actionLCfgPtr->linSchdlSwitchActionLCfgs[actionIdx].schedule);
    return ret;
}
#endif /*BSWM_ACTION_LINSCHEDULESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Client Service Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdCliServiceModeReq(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    /*call the action function with configured parameters*/
    ret = Sd_ClientServiceSetState(
        actionLCfgPtr->sdCliServiceModeReqActions[actionIdx].clientServiceHandleId,
        actionLCfgPtr->sdCliServiceModeReqActions[actionIdx].state);
    return ret;
}
#endif /*BSWM_ACTION_SDCLIENTSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Consumed Event Group Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdEvGrpModeReq(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    /*call the action function with configured parameters*/
    ret = Sd_ConsumedEventGroupSetState(
        actionLCfgPtr->sdConsumedEvGrpModeReqActions[actionIdx].sdConsumedEventGroupHandleId,
        actionLCfgPtr->sdConsumedEvGrpModeReqActions[actionIdx].state);
    return ret;
}
#endif /*BSWM_ACTION_SDCONSUMEDEVENTGROUPMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON)
/*Action: SD Server Service Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSdSvrServiceModeReq(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    /*call the action function with configured parameters*/
    ret = Sd_ServerServiceSetState(
        actionLCfgPtr->sdServerServiceModeReqActions[actionIdx].sdServerServiceHandleId,
        actionLCfgPtr->sdServerServiceModeReqActions[actionIdx].state);
    return ret;
}
#endif /*BSWM_ACTION_SDSERVERSERVICEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_NMCONTROL_ENABLED == STD_ON)
/*ACtion: NM control*/
FUNC(Std_ReturnType, BSWM_CODE) BswM_DoNMControl(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    if (BSWM_NM_DISABLE == actionLCfgPtr->nmControlActions[actionIdx].action)
    {
        ret = Nm_DisableCommunication(actionLCfgPtr->nmControlActions[actionIdx].nmChannel);
    }
    else
    {
        ret = Nm_EnableCommunication(actionLCfgPtr->nmControlActions[actionIdx].nmChannel);
    }
    return ret;
}
#endif /*BSWM_ACTION_NMCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON)
/*Action: COM deadline monitor control*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoDMControl(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;
    BswM_ComIpduGruType numOfEnableRecDmGru;
    BswM_ComIpduGruType numOfDisableRecDmGru;
    BswM_ComIpduGruType gruIdx;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    numOfEnableRecDmGru = actionLCfgPtr->dmCtrlActionLCfgs[actionIdx].numOfEnableRecDmGru;
    for (gruIdx = 0; gruIdx < numOfEnableRecDmGru; gruIdx++)
    {
        /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
        Com_SetIpduGroup(
            *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].comRxDmIpduGroupVector),
            actionLCfgPtr->dmCtrlActionLCfgs[actionIdx].enableRecDmGruId[gruIdx],
            TRUE);
        /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
    }
    numOfDisableRecDmGru = actionLCfgPtr->dmCtrlActionLCfgs[actionIdx].numOfDisableRecDmGru;
    for (gruIdx = 0; gruIdx < numOfDisableRecDmGru; gruIdx++)
    {
        /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
        Com_SetIpduGroup(
            *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].comRxDmIpduGroupVector),
            actionLCfgPtr->dmCtrlActionLCfgs[actionIdx].disableRecDmGruId[gruIdx],
            FALSE);
        /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
    }
    /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
    Com_ReceptionDMControl(*(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].comRxDmIpduGroupVector));
    /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */

    return ret;
}
#endif /*BSWM_ACTION_DEADLINEMONITORINGCONTROL_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON)
/*Action: PDU Group Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoPduGrpSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;
    BswM_ComIpduGruType numOfEnableIpduGru;
    BswM_ComIpduGruType numOfDisableIpduGru;
    BswM_ComIpduGruType gruIdx;
    uint32 index;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    numOfEnableIpduGru = actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].numOfEnableIpduGru;
    for (gruIdx = 0; gruIdx < numOfEnableIpduGru; gruIdx++)
    {
        /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
        Com_SetIpduGroup(
            *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupAllVecotr),
            actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].enableIpduGruId[gruIdx],
            TRUE);
        /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
        if ((boolean)TRUE == actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].initialize)
        {
            /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
            Com_SetIpduGroup(
                *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupReinitVecotr),
                actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].enableIpduGruId[gruIdx],
                TRUE);
            /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
        }
    }
    numOfDisableIpduGru = actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].numOfDisableIpduGru;
    for (gruIdx = 0; gruIdx < numOfDisableIpduGru; gruIdx++)
    {
        /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
        Com_SetIpduGroup(
            *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupAllVecotr),
            actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].disableIpduGruId[gruIdx],
            FALSE);

        Com_SetIpduGroup(
            *(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupReinitVecotr),
            actionLCfgPtr->pduGrpSwitchActionLCfgs[actionIdx].disableIpduGruId[gruIdx],
            FALSE);
        /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
    }
    /*1.First call of Com_IpduGroupControl, the vector for BswMPduGroupSwitchReinit == true*/
    /* PRQA S 2784 ++ */ /* MISRA Rule 17.5 */
    Com_IpduGroupControl(*(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupReinitVecotr), TRUE);
    /*2.For the second call of Com_IpduGroupControl, the other vector shall be used*/
    Com_IpduGroupControl(*(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupAllVecotr), FALSE);
    /* PRQA S 2784 -- */ /* MISRA Rule 17.5 */
    /*3.the vector for BswMPduGroupSwitchReinit == true shall be overwritten with the value of the other vector.*/
    for (index = 0u; index < sizeof(Com_IpduGroupVector); index++)
    {
        ((uint8*)(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupReinitVecotr))[index] =
            ((uint8*)(BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].ipduGroupAllVecotr))[index];
    }
    return ret;
}
#endif /*BSWM_ACTION_PDUGROUPSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON)
/*Action: Switch IPDU Mode*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSwitchIpduMode(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    Com_SwitchIpduTxMode(
        actionLCfgPtr->switchIpduModeActionLCfgs[actionIdx].pduId,
        actionLCfgPtr->switchIpduModeActionLCfgs[actionIdx].mode);

    return ret;
}
#endif /*BSWM_ACTION_SWITCHIPDUMODE_ENABLED == STD_ON*/

#if (BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON)
/*Action: Trigger IPDU Send*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoTriggerIpduSend(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    uint8 pduNum;
    uint8 index;
    Std_ReturnType ret = E_OK;
    Std_ReturnType triRet;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    pduNum = actionLCfgPtr->triggerIpduSendActionLCfgs[actionIdx].numOfIpdu;
    for (index = 0; index < pduNum; index++)
    {
        triRet = Com_TriggerIPDUSend(actionLCfgPtr->triggerIpduSendActionLCfgs[actionIdx].pduIds[index]);
        if ((Std_ReturnType)E_OK != triRet)
        {
            ret = E_NOT_OK;
        }
    }

    return ret;
}
#endif /*BSWM_ACTION_TRIGGERIPDUSEND_ENABLED == STD_ON*/

#if (BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON)
/*Action: J1939Rm State Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoJ1939RmStateSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = J1939Rm_SetState(
        actionLCfgPtr->j1939RmStateSwitchActions[actionIdx].channel,
        actionLCfgPtr->j1939RmStateSwitchActions[actionIdx].node,
        actionLCfgPtr->j1939RmStateSwitchActions[actionIdx].state);
    return ret;
}
#endif /*BSWM_ACTION_J1939RMSTATESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON)
/*Action: PDU Router Control*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoPduRouterControl(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;
    uint8 pduGrpNum;
    uint8 index = 0;
    boolean initialize = FALSE;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    pduGrpNum = actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].numOfPathGrp;

    if (actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].initialize != NULL_PTR)
    {
        initialize = *(actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].initialize);
    }
    for (index = 0; index < pduGrpNum; index++)
    {
        if (BSWM_PDUR_DISABLE == actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].action)
        {
            PduR_DisableRouting(actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].ids[index], initialize);
        }
        else
        {
            PduR_EnableRouting(actionLCfgPtr->pdurCtrlActionLCfgs[actionIdx].ids[index]);
        }
    }
    return ret;
}
#endif /*BSWM_ACTION_PDUROUTERCONTROL_ENABLED == STD_ON*/

#if (BSWM_MAX_NUM_CORE > 1u)
#if (BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoCoreHaltMode(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;
    StatusType osRet;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    osRet = ControlIdle(
        actionLCfgPtr->coreHaltModeActions[actionIdx].coreId,
        actionLCfgPtr->coreHaltModeActions[actionIdx].IdleMode);
    if (E_OK != osRet)
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /*BSWM_ACTION_COREHALTMODE_ENABLED == STD_ON*/
#endif /*BSWM_MAX_NUM_CORE > 1*/

#if (BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON)
/*Action: Call user function*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoUserCallout(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    actionLCfgPtr->userCalloutActions[actionIdx].userCalloutFctPtr();
    return ret;
}
#endif /*BSWM_ACTION_USERCALLOUT_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON)
/*Action: RTE Mode Request*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteModeRequest(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = actionLCfgPtr->rteModeRequestActions[actionIdx].rteRequestFctPtr(
        actionLCfgPtr->rteModeRequestActions[actionIdx].mode);
    return ret;
}
#endif /*BSWM_ACTION_RTEMODEREQUEST_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESWITCH_ENABLED == STD_ON)
/*Action: RTE Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret = E_OK;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret = actionLCfgPtr->rteSwitchActions[actionIdx].rteSwitchFctPtr(actionLCfgPtr->rteSwitchActions[actionIdx].mode);
    return ret;
}
#endif /*BSWM_ACTION_RTESWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESTART_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteStart(void)
{
    return Rte_Start();
}
#endif /*BSWM_ACTION_RTESTART_ENABLED == STD_ON*/

#if (BSWM_ACTION_RTESTOP_ENABLED == STD_ON)
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoRteStop(void)
{
    return Rte_Stop();
}
#endif /*BSWM_ACTION_RTESTOP_ENABLED == STD_ON*/

#if (BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON)
/*ACtion: SchM Switch*/
FUNC(Std_ReturnType, BSWM_CODE)
BswM_DoSchMSwitch(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    Std_ReturnType ret;

    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    ret =
        actionLCfgPtr->schMSwitchActions[actionIdx].schMSwitchFctPtr(actionLCfgPtr->schMSwitchActions[actionIdx].mode);
    return ret;
}
#endif /*BSWM_ACTION_SCHMSWITCH_ENABLED == STD_ON*/

#if (BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON)
/*Action: Timer control*/
FUNC(Std_ReturnType, BSWM_CODE) BswM_DoTimerControl(ApplicationType partIdx, uint8 actionIdx)
{
    P2CONST(BswM_ModeRqstPortRuntimeType, AUTOMATIC, BSWM_VAR_CLEARED) modeRqstPtr;
    P2CONST(BswM_ActionItemsLCfgType, AUTOMATIC, BSWM_CONST) actionLCfgPtr;
    BswM_TimerIdxType timerRef;
    BswM_TimerEnumType timerAct;

    modeRqstPtr = BswM_RuntimeStatus.bswmPartPCCfgs[partIdx].modeRqstPortRunPtr;
    actionLCfgPtr = BswM_RuntimeStatus.bswmPartLCfgs[partIdx].acItemsLCfg;
    timerRef = actionLCfgPtr->timerAction[actionIdx].timerRef;
    timerAct = actionLCfgPtr->timerAction[actionIdx].timerAct;
    if (BSWM_TIMER_START == timerAct)
    {
        modeRqstPtr->timerStatus[timerRef] = BSWM_TIMER_START;
        modeRqstPtr->timerCycle[timerRef] = actionLCfgPtr->timerAction[actionIdx].timerVal;
    }
    else if (BSWM_TIMER_STOP == timerAct)
    {
        modeRqstPtr->timerStatus[timerRef] = BSWM_TIMER_STOP;
    }
    else
    {
        /*Do nothing*/
    }
    return E_OK;
}
#endif /*BSWM_ACTION_TIMERCONTROL_ENABLED == STD_ON*/

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
