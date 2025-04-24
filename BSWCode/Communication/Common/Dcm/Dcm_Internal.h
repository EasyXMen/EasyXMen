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
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef COMMUNICATION_DCM_DCM_INTERNAL_H_
#define COMMUNICATION_DCM_DCM_INTERNAL_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dcm_Ext.h"
#include "Dcm_Types.h"
#include "Dcm_Cbk.h"
#include "SchM_Dcm.h"
#include "Rte_Dcm.h"
#include "PduR_Dcm.h"
#if (NVM_ENABLE == STD_ON)
#include "NvM_Types.h"
#include "NvM.h"
#endif
#if (DCM_BSWM_ENABLE == STD_ON)
#include "BswM_Dcm.h"
#endif
#include "ComM_Dcm.h"
#if (DCM_DEM_SUPPOTR == STD_ON)
#include "Dem.h"
#endif

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#define DCM_SEC_RESUMECALL_MASK 0x01u
#define DCM_SEC_CALL_OVER_MASK  0x02u
#define DCM_SEC_CALL_AGAIN_MASK 0x04u

#define DCM_OBD                 (0x00u)
#define DCM_UDS                 (0x01u)
#define DCM_ROE                 (0x02u)
#define DCM_PERIODIC            (0x03u)
#define DCM_NO_SERVICE          (0x04u)

#if (STD_ON == DCM_DEV_ERROR_DETECT)
static inline void DCM_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == DCM_DEV_ERROR_DETECT */

/*in default session, delay S3 time release network*/
#if !defined(DCM_DELAY_COMM_INACTIVE)
#define DCM_DELAY_COMM_INACTIVE 0
#endif

#define DCM_CPU_BYTE_ORDER CPU_BYTE_ORDER
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*DcmGeneral*/
extern CONST(Dcm_GeneralCfgType, DCM_CONST) Dcm_GeneralCfg;

extern VAR(Dcm_DspProgramType, DCM_VAR_NOINIT) Dcm_DspProgram;

extern P2CONST(Dcm_CfgType, AUTOMATIC, DCM_CONST_PBCFG) DcmPbCfgPtr;

extern VAR(Dcm_MkCtrlType, DCM_VAR_NOINIT) Dcm_MkCtrl; /*Module control resource*/

extern VAR(boolean, DCM_VAR_POWER_ON_INIT) gAppl_UpdataOK_ResponseFlag;

extern VAR(boolean, DCM_VAR_POWER_ON_INIT) Dcm_ReqSetProgConditions;

extern VAR(Dcm_ProgConditionsType, DCM_VAR_NOINIT) Dcm_ProgConditions;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
extern VAR(Dcm_PageBufferDataType, DCM_VAR_NOINIT) Dcm_PageBufferData;
#endif

#if (STD_ON == DCM_GENERIC_CONNECTION)
extern VAR(uint8, DCM_VAR_NOINIT) Dcm_Metadata[DCM_DSLPROTOCOLROW_NUM_MAX][4];
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern FUNC(void, DCM_CODE) DslInternal_InitPageBuffer(void);
#endif /* STD_ON == DCM_PAGEDBUFFER_ENABLED */

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DslInternal_RxIndicationDeal(uint8 ProtocolCfgCtrlId);
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DCM_CODE) Dcm_ResetTime(P2VAR(uint32, AUTOMATIC, DCM_VAR) TimerPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern Std_ReturnType Dcm_SetProgConditions(Dcm_OpStatusType OpStatus, Dcm_ProgConditionsType* ProgConditions);

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern Dcm_EcuStartModeType Dcm_GetProgConditions(Dcm_ProgConditionsType* ProgConditions);

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DCM_CODE) Dcm_GetTimeSpan(uint32 TimerPtr, P2VAR(uint32, AUTOMATIC, DCM_VAR) TimeSpanPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE)
/**********************************************************************/
/*
 * Brief               <Get Specific Cause Code when responding NRC22 >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <* code: the Specific Cause Code which defined by customer.
 * Default value:0xFFu >
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern Std_ReturnType Dcm_GetSpecificCauseCode(Dcm_SpecificCauseCodeType* code);
#endif /* STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE */

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
extern FUNC(void, DCM_CODE) DslInternal_InitDspProgramInfo(void);

/*************************************************************************/
/*
 * Brief               <The Dsp layer response to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DspInternal_DcmConfirmation(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Uds SubServices Check>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsSubServicesCheck(uint8 ProtocolCtrlId);

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <Dcm  Update  Page>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DspInternal_DcmUpdatePage(uint8 ProtocolCtrlId);
#endif

/****************************** declarations *********************************/
/*************************************************************************/
/*
 * Brief               <Find SID corresponding service configuration table index
 *                      and the service index in corresponding service configuration table.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Sid: Service Id;
 *                      ProtocolCtrlId: The corresponding protocol control block ID number>
 * Param-Name[out]     <(*pSidTabIndex):Request packet index number
 * in the service configuration table;
 *                      (*pSidTabServieCfgIndex): the service index
 *                      in corresponding service configuration table>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SearchSidTabServiceIndex(
    uint8 Sid,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSidTabIndex,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSidTabServieCfgIndex);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <DsdInternal_SearchSidTabSubServiceIndex>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SearchSidTabSubServiceIndex(
    uint8 ProtocolCtrlId,
    uint16 SidTabIndex,
    uint16 ServiceIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pSubServieCfgIndex);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
/*************************************************************************/
/*
 * Brief               <Set the negative response code NRC;>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Nrc: the negative response code;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DsdInternal_SetNrc(uint8 ProtocolCtrlId, Dcm_NegativeResponseCodeType Nrc)
{
    uint8 MsgCtrlIndexx;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_MsgCtrl[MsgCtrlIndexx].NRC = Nrc;
    Dcm_MsgCtrl[MsgCtrlIndexx].RspStyle = DCM_NEG_RSP;
}

/*************************************************************************/
/*
 * Brief               <The diagnostic request processing in DSD layer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_RxIndication(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Response to a diagnosis request processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DsdInternal_ProcessingDone(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Notify the DSD layer, respond to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DsdInternal_TxConfirmation(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck(const uint8 addressAndLengthFormatIdentifier);

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <DsdInternal_ProcessPage>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DsdInternal_ProcessPage(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <DsdInternal_StartPagedProcessing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DsdInternal_StartPagedProcessing(uint8 ProtocolCtrlId);
#endif /*STD_ON == DCM_PAGEDBUFFER_ENABLED*/

/*************************************************************************/
/*
 * Brief               <Start P2Timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE)
    DslInternal_P2ServerStart(uint8 ProtocolCtrlId) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_P2CtrlType* pDcm_P2Ctrl = &Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl;
    if (DCM_P2TIMER_OFF == Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2State)
    {
        pDcm_P2Ctrl->PendingNum = 0u;
        Dcm_ResetTime(&pDcm_P2Ctrl->Dcm_P2CurTimer);
        pDcm_P2Ctrl->Dcm_P2ExpiredTimer = Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax;
        pDcm_P2Ctrl->Dcm_P2State = DCM_P2TIMER_ON;
    }
}

/*************************************************************************/
/*
 * Brief               <stop P2timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_P2ServerStop(uint8 ProtocolCtrlId) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_P2CtrlType* pDcm_P2Ctrl = &Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl;
    if (DCM_P2TIMER_ON == pDcm_P2Ctrl->Dcm_P2State)
    {
        pDcm_P2Ctrl->Dcm_P2State = DCM_P2TIMER_OFF;
    }
}

/*************************************************************************/
/*
 * Brief               <P2Timer timeout processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DslInternal_P2ServerTimeout(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Judgment P2Timer run time is more than P2ServerMin>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
static inline FUNC(Std_ReturnType, DCM_CODE) DslInternal_MinIsLargerThanP2Server(uint8 ProtocolCtrlId)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint32 P2ServerMinx = Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMin;
    uint32 OldTimer = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2CurTimer;
    uint32 Timer;
    Std_ReturnType ret = E_OK;

    Dcm_GetTimeSpan(OldTimer, &Timer);
    if (Timer < P2ServerMinx)
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DslInternal_MaxIsLargerThanP2Server>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
static inline FUNC(Std_ReturnType, DCM_CODE) DslInternal_MaxIsLargerThanP2Server(uint8 ProtocolCtrlId)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint32 P2ServerMaxx = Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax;
    uint32 OldTimer = Dcm_MsgCtrl[MsgCtrlIndexx].Dcm_P2Ctrl.Dcm_P2CurTimer;
    uint32 Timer;
    Std_ReturnType ret = E_OK;

    Dcm_GetTimeSpan(OldTimer, &Timer);
    if (Timer < P2ServerMaxx)
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <protocal start>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DslInternal_ProtocolStart(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <protocal stop>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) DslInternal_ProtocolStop(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol link layer parameters of time>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <P2ServerTimer:
 *                      P2StarServerTimer:>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
static inline FUNC(void, DCM_CODE) DslInternal_SetProtocolLinkLayerTimer(uint32 P2ServerTimer, uint32 P2StarServerTimer)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 Num;
    uint8 ProtocolCtrlId;
    uint32 P2ServerTime;
    uint32 P2StarServerTime;
    uint32 P2ServerTimerBak;
    uint32 P2StarServerTimerBak;
    uint32 P2ServerTimerLimit;
    uint32 P2StarServerTimerLimit;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow;

    P2ServerTimerBak = P2ServerTimer;
    P2StarServerTimerBak = P2StarServerTimer;
    Num = pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num;
    for (ProtocolCtrlId = 0; ProtocolCtrlId < Num; ProtocolCtrlId++)
    {
        P2ServerTime = P2ServerTimerBak;
        P2StarServerTime = P2StarServerTimerBak;
        P2ServerTimerLimit = pDcmDslProtocolRow[ProtocolCtrlId].DcmTimStrP2ServerAdjust;
        P2StarServerTimerLimit = pDcmDslProtocolRow[ProtocolCtrlId].DcmTimStrP2StarServerAdjust;
        if (P2ServerTime > P2ServerTimerLimit)
        {
            P2ServerTime = P2ServerTime - P2ServerTimerLimit;
        }
        if (P2StarServerTime > P2StarServerTimerLimit)
        {
            P2StarServerTime = P2StarServerTime - P2StarServerTimerLimit;
        }
        Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMax = P2ServerTime;
        Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax = P2StarServerTime;
        Dcm_ProtocolCtrl[ProtocolCtrlId].P2ServerMin = 0UL;
        Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMin = 0UL;
        Dcm_ProtocolCtrl[ProtocolCtrlId].S3Server = 5000UL; /* ms converted to tick */
    }
}

/*************************************************************************/
/*
 * Brief               Utility function:copy memory data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Source memory address to copy from
 *                     Dest memory address to copy to
 *                     Length memory length to copy
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           CanTp Internal
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dcm_MemoryCopy(P2CONST(uint8, AUTOMATIC, DCM_CONST) Source, P2VAR(uint8, AUTOMATIC, DCM_VAR) Dest, uint32 Length)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 Cnt;
    for (Cnt = 0; Cnt < Length; Cnt++)
    {
        Dest[Cnt] = Source[Cnt];
    }
}

/*************************************************************************/
/*
 * Brief               <initialization message control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <MsgCtrlId:message control block ID Index>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_InitMsgCtrl(uint8 MsgCtrlId)
{
    if (MsgCtrlId < DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num)
    {
        Dcm_MsgCtrlType* pDcm_MsgCtrl = &Dcm_MsgCtrl[MsgCtrlId];
        pDcm_MsgCtrl->SID = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->Subfunction = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->NRC = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->SendFlag = FALSE;
        pDcm_MsgCtrl->RspStyle = DCM_POS_RSP;
        pDcm_MsgCtrl->DcmTxPduId = DCM_INVALID_PDUID;
        pDcm_MsgCtrl->Dcm_RxCtrlChannelIndex = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->Dcm_TxCtrlChannelIndex = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->MsgContext.DcmRxPduId = DCM_INVALID_PDUID;
        pDcm_MsgCtrl->MsgContext.pReqData = NULL_PTR;
        pDcm_MsgCtrl->MsgContext.ReqDataLen = 0;
        pDcm_MsgCtrl->MsgContext.pResData = NULL_PTR;
        pDcm_MsgCtrl->MsgContext.ResDataLen = DCM_INVALID_UINT32;
        pDcm_MsgCtrl->MsgContext.MsgAddInfo.ReqType = (uint8)DCM_PHYSICAL;
        pDcm_MsgCtrl->MsgContext.MsgAddInfo.SuppressPosResponse = FALSE;
        pDcm_MsgCtrl->MsgContext.MsgAddInfo.CancelOperation = FALSE;
        pDcm_MsgCtrl->MsgContext.ResMaxDataLen = DCM_INVALID_UINT32;
        pDcm_MsgCtrl->MsgContext.IdContext = DCM_INVALID_UINT8;
        pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2CurTimer = DCM_INVALID_UINT32;
        pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2ExpiredTimer = DCM_INVALID_UINT32;
        pDcm_MsgCtrl->Dcm_P2Ctrl.PendingNum = 0u;
        pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2State = DCM_P2TIMER_OFF;
        pDcm_MsgCtrl->Dcm_MsgState = DCM_MSG_WAIT;
        pDcm_MsgCtrl->Dcm_Ret = E_NOT_OK;
        pDcm_MsgCtrl->Dcm_OpStatus = DCM_INITIAL;
#if (STD_ON == DCM_GENERIC_CONNECTION)
        pDcm_MsgCtrl->Dcm_MetaData_SA = DCM_INVALID_UINT16;
        pDcm_MsgCtrl->Dcm_MetaData_TA = DCM_INVALID_UINT16;
        pDcm_MsgCtrl->Dcm_MetaData_SA_Backup = DCM_INVALID_UINT16;
        pDcm_MsgCtrl->Dcm_MetaData_TA_Backup = DCM_INVALID_UINT16;
#endif
    }
}

/*************************************************************************/
/*
 * Brief               <Initializing the corresponding channel control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ChannelCtrlId:channel control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_InitChannelCtrl(uint8 ChannelCtrlId)
{
    uint8 ChannelNum = Dcm_GetChannelNum();
    if (ChannelCtrlId < ChannelNum)
    {
        Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelCfgIndex = DCM_INVALID_UINT8;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        if (DCM_PENDING_REQUEST_PROCESS == Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState)
        {
            Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState = DCM_PENDING_REQUEST_NONE;
            Dcm_QueuedRequestCtrl[0u].Dcm_QueuedRequestBufferCunrentPosition = 0u;
        }
        else if (DCM_PENDING_REQUEST_PROCESS == Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState)
        {
            Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState = DCM_PENDING_REQUEST_NONE;
            Dcm_QueuedRequestCtrl[1u].Dcm_QueuedRequestBufferCunrentPosition = 0u;
        }
        else
        {
            /* idle */
        }
        if ((DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState)
            && (DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState))
        {
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelRxState = DCM_CH_IDLE;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
        Dcm_ChannelCtrl[ChannelCtrlId].Dcm_ChannelTxState = DCM_CH_IDLE;
        Dcm_ChannelCtrl[ChannelCtrlId].Dcm_BufferCunrentPosition = 0u;
        Dcm_ChannelCtrl[ChannelCtrlId].Dcm_BufferErasePosition = 0u;
    }
}

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
/*************************************************************************/
/*
 * Brief               <Initializing the corresponding channel queued reqeust control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ChannelCtrlId:channel control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
static inline FUNC(void, DCM_CODE) DslInternal_InitChannelQueuedRequestCtrl(void)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
{
    Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState = DCM_PENDING_REQUEST_NONE;
    Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState = DCM_PENDING_REQUEST_NONE;
    Dcm_QueuedRequestCtrl[0u].Dcm_QueuedRequestRxPduId = DCM_INVALID_PDUID;
    Dcm_QueuedRequestCtrl[1u].Dcm_QueuedRequestRxPduId = DCM_INVALID_PDUID;
    Dcm_QueuedRequestCtrl[0u].Dcm_QueuedRequestBufferCunrentPosition = 0u;
    Dcm_QueuedRequestCtrl[1u].Dcm_QueuedRequestBufferCunrentPosition = 0u;
    Dcm_QueuedRequestCtrl[0u].Dcm_QueuedRequestLength = 0u;
    Dcm_QueuedRequestCtrl[1u].Dcm_QueuedRequestLength = 0u;
}
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

/*************************************************************************/
/*
 * Brief               <Reset protocol control block members MsgCtrlIndex +
 *                      Reset corresponding message control block +
 *                      Reset corresponding receive / transmit channel control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:protocol control block ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DslInternal_ResetResource(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               <Dcm_StartOfReception_Checklength>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception_Checklength(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <Dcm_ClassifyServiceType>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(uint8, DCM_CODE) Dcm_ClassifyServiceType(Dcm_ProtocolType protocolType)
{
    uint8 serviceType;
    switch (protocolType)
    {
    case DCM_OBD_ON_CAN:
    case DCM_OBD_ON_FLEXRAY:
    case DCM_OBD_ON_IP:
        serviceType = DCM_OBD;
        break;
    case DCM_UDS_ON_CAN:
    case DCM_UDS_ON_FLEXRAY:
    case DCM_UDS_ON_IP:
        serviceType = DCM_UDS;
        break;
    case DCM_ROE_ON_CAN:
    case DCM_ROE_ON_FLEXRAY:
    case DCM_ROE_ON_IP:
        serviceType = DCM_ROE;
        break;
    case DCM_PERIODICTRANS_ON_CAN:
    case DCM_PERIODICTRANS_ON_FLEXRAY:
    case DCM_PERIODICTRANS_ON_IP:
        serviceType = DCM_PERIODIC;
        break;
    default:
        serviceType = DCM_NO_SERVICE;
        break;
    }
    return serviceType;
}

/*************************************************************************/
/*
 * Brief               <DSL layer sends the response function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <DsdInternal_ProcessingDone>
 */
/*************************************************************************/
extern FUNC(void, DCM_CODE) DslInternal_ProcessingDone(uint8 ProtocolCtrlId);

/*************************************************************************/
/*
 * Brief               MemSet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Val && Size
 * Param-Name[out]     none
 * Param-Name[in/out]  Dest
 * Return              none
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static inline FUNC(void, DCM_CODE) Dcm_MemSet(P2VAR(uint8, AUTOMATIC, DCM_VAR) Dest, uint8 Val, uint32 Size)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 Cnt = Size;
    while (Cnt > 0u)
    {
        Cnt--;
        Dest[Cnt] = Val;
    }
}

#endif /* COMMUNICATION_DCM_DCM_INTERNAL_H_ */
