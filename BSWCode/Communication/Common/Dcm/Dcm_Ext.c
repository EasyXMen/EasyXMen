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
**  FILENAME    : Dcm_Ext.c                                                  **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Implementation of Dcm Internal                             **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm_Internal.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <S3Timer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_S3Timer(void);
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <SecTimer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_SecTimer(void);

/*************************************************************************/
/*
 * Brief               <Read SecTimer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_ReadSecTimer(void);
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               <Page_Buffer Timer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_PageBufferTimer(void);
#endif /* DCM_PAGEDBUFFER_ENABLED == STD_ON */

#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED))
/*************************************************************************/
/*
 * Brief               <DsdInternal_DidCheck>
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
static FUNC(Std_ReturnType, DCM_CODE) DsdInternal_DidCheck(
    uint16 receiveDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) pRangeDidCfgIndex,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) pRangeDidFlag);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <DsdInternal_DDDIDcheckMemory>
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
static FUNC(Std_ReturnType, DCM_CODE) DsdInternal_DDDIDcheckMemory(uint32 addr, uint32 size);
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <initial of Security level control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_InitSecCtrl(void);
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <Set the session control state>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_SetSesCtrlType(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <The set NewSes corresponding configuration session time parameters>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <(*P2ServerTimer):P2ServerTimer which corresponding to NewSes
 *                      (*P2StarServerTimer):P2StarServerTimer which corresponding to NewSes>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DslInternal_GetSesTimingValues(
    Dcm_SesCtrlType NewSes,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) P2ServerTimer,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) P2StarServerTimer);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol
 * link layer parameters ,which is in protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_SetSesTimingValues(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <session change from default to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_DefaultToDefault(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <session change from default to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_DefaultToUndefault(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <session change from undefault to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_UndefaultToUndefault(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <session change from undefault to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_UndefaultTodefault(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <When the session is changed, the specific processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_SesTranslation(Dcm_SesCtrlType NewSes);

/*************************************************************************/
/*
 * Brief               <timeout of S3Timer>
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
static FUNC(void, DCM_CODE) DslInternal_S3ServerTimeout(void);

/*************************************************************************/
/*
 * Brief               <The initialization of session management control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_InitSesCtrl(void);
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    OBD_ReadPidValues_0x01(uint8 pid, uint8 ctrlIndex, P2VAR(uint32, AUTOMATIC, DCM_VAR) pBufOffset, boolean UDSCall);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DCM_DSP_PID_FUNC_ENABLED == STD_ON */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_VINType, DCM_VAR_POWER_ON_INIT) Dcm_VIN;

#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
VAR(Dcm_RoutineControlStateType, DCM_VAR_NOINIT) Dcm_RoutineControlState[DCM_DSP_ROUTINE_MAX_NUM];
#endif

VAR(Dcm_CommCtrlType, DCM_VAR_NOINIT) Dcm_CommCtrl[DCM_MAINCONNECTION_NUM];
/*Diagnostic Communication Control*/

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
VAR(Dcm_SecCtrlType, DCM_VAR_NOINIT) Dcm_SecCtrl; /*Security level management control block*/
#endif                                            /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
VAR(Dcm_SesCtrlCBType, DCM_VAR_NOINIT) Dcm_SesCtrl; /*Session management control block*/
#endif                                              /* STD_ON == DCM_SESSION_FUNC_ENABLED */

VAR(Dcm_ProtocolCtrlType, DCM_VAR_NOINIT) Dcm_ProtocolCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];
/*Protocol control block*/

VAR(Dcm_ChannelCtrlType, DCM_VAR_NOINIT) Dcm_ChannelCtrl[DCM_CHANNEL_NUM];
/*Channel control block*/

VAR(Dcm_MsgCtrlType, DCM_VAR_NOINIT) Dcm_MsgCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];
/*Message control block, corresponding to uds service*/

VAR(Dcm_ConnectionCtrlType, DCM_VAR_NOINIT) Dcm_ConnectionCtrl[DCM_CONNECTION_NUM];
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

/*Static channel allocation of resources*/
#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
VAR(uint8, DCM_VAR_POWER_ON_INIT) Dcm_Channel[DCM_CHANNEL_LENGTH];
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
uint8 Dcm_QueuedIndex;
VAR(uint8, DCM_VAR_POWER_ON_INIT) Dcm_RequestQueuedBuffer[2u][DCM_REQUEST_QUEUED_BUFFER_SIZE];
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_QueuedRequestCtrlType Dcm_QueuedRequestCtrl[2u];
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

#define DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_FunctionMessageType, DCM_VAR_NOINIT) Dcm_FunctionalMessage;
/*Function addressing packets resources,power on init*/
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_OBDMessageType, DCM_VAR_NOINIT) Dcm_OBDMessage;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(SchedulerQueueTransmitTypes, DCM_VAR_NOINIT)
Scheduler_0x2A_Transmit[DCM_PERIODICCONNECTION_NUM];
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif
/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief                <initialization of DCM module.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_InterInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 Index;
#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
    uint16 Offest = 0;
    uint8* VIN_Data;
    Dcm_NegativeResponseCodeType ErrorCode;
    Std_ReturnType ret;
#endif /* STD_ON == DCM_DSP_DATA_FUNC_ENABLED */

    for (Index = 0; Index < DCM_CONNECTION_NUM; Index++)
    {
        DslInternal_InitConnectionCtrl((uint8)Index);
    }
    /*Clear Buffer*/
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_MemSet(Dcm_Channel, 0x00, DCM_CHANNEL_LENGTH);
    SchM_Exit_Dcm_ExclusiveArea();
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    DslInternal_InitSesCtrl();
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    DslInternal_InitSecCtrl();
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
    SchM_Enter_Dcm_ExclusiveArea();
    for (Index = 0; Index < DCM_DSP_ROUTINE_MAX_NUM; Index++)
    {
        Dcm_RoutineControlState[Index] = DCM_DSP_ROUTINE_INIT;
    }
    SchM_Exit_Dcm_ExclusiveArea();
#endif /* STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED && DCM_DSP_ROUTINE_MAX_NUM > 0 */
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    Dcm_UDS0x86_Init();
#endif /* STD_ON == DCM_UDS_SERVICE0X86_ENABLED */
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
    Dcm_Scheduler_0x2A_Init();
#endif /* STD_ON == DCM_UDS_SERVICE0X2A_ENABLED */
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    Dcm_AuthenticateInit();
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */

    /*get VIN*/
    Dcm_VIN.Flag = FALSE;
#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
    const Dcm_DspDidType* DcmVinRef = Dcm_GeneralCfg.DcmVinRef;
    if (NULL_PTR != DcmVinRef)
    {
        uint16 DcmDspDidSignalNum = DcmVinRef->DcmDspDidSignalNum;
        const Dcm_DspDidSignalType* pDcmDspDidSignal = DcmVinRef->pDcmDspDidSignal;
        Dcm_VIN.Flag = TRUE;
        for (Index = 0; Index < 17u; Index++)
        {
            Dcm_VIN.Dcm_VIN[Index] = 0xFF;
        }
        for (Index = 0; (Index < DcmDspDidSignalNum) && (Dcm_VIN.Flag == TRUE); Index++)
        {
            const Dcm_DspDataType* pDidData = pDcmDspDidSignal[Index].pDcmDspDidData;
            VIN_Data = &(Dcm_VIN.Dcm_VIN[Offest]);
            if (NULL_PTR != pDidData->DcmDspDataReadFnc)
            {
                ret = pDidData->DcmDspDataReadFnc(DCM_INITIAL, VIN_Data, &ErrorCode);
                if (E_OK != ret)
                {
                    Dcm_VIN.Flag = FALSE;
                }
                else
                {
                    Offest += pDidData->DcmDspDataSize;
                }
            }
        }
    }
#endif
}
#if (DCM_DEM_SUPPOTR == STD_ON)
/*************************************************************************/
/*
 * Brief               <Triggers on changes of the UDS DTC status byte.
 *                      Allows to trigger on ROE Event for subservice OnDTCStatusChanged.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <DTC: This is the DTC the change trigger is assigned to.
                        DTCStatusOld: DTC status before change
                        DTCStatusNew: DTC status after change>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DCM_CODE)
Dcm_InterDemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    Dcm_UDS0x86_DTCStatusChange(DTCStatusOld, DTCStatusNew);
#else
    DCM_UNUSED(DTCStatusNew);
    DCM_UNUSED(DTCStatusOld);
#endif
    DCM_UNUSED(DTC);
}
#endif
/*************************************************************************/
/*
 * Brief               <This service is used for processing the tasks of the main loop. >
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
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_InterMainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    /*S3Timer background processing function*/
    Dcm_MainFunction_S3Timer();
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    /*SecTimer background processing function*/
    Dcm_MainFunction_SecTimer();
#endif
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    /*Page_Buffer Timer background processing function*/
    Dcm_MainFunction_PageBufferTimer();
#endif
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    /*ROE Timer check*/
    Dcm_UDS0x86_CheckTimer();
#endif
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    /*AuthenticationTimer check*/
    Dcm_MainFunction_AuthenticationTimer();
#endif
}

/*************************************************************************/
/*
 * Brief               <The Pending message background processing function>
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
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_MainFunction_Pending2A(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
    if (DCM_PROTOCOL_VALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
    {
        Dcm_MainFunction_Scheduler_0x2A(ProtocolCtrlId);
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

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
FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x27(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_ON == DCM_SECURITY_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X27_ENABLED))
    const Dcm_DspSecurityRowType* pSecurityRow;
    uint8 SecCfgIndex;
    uint8 MsgCtrlId;
    Std_ReturnType ret;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*securityAccess service*/
    if ((0x27u == Dcm_MsgCtrl[MsgCtrlId].SID) && (DCM_NEG_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
        && (DCM_E_GENERALREJECT == Dcm_MsgCtrl[MsgCtrlId].NRC))
    {
        /*confirmation to reject SecurityAccess service*/
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
    }
    if ((0x27u == Dcm_MsgCtrl[MsgCtrlId].SID)
        && ((DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle)
            || (DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle))
        && (DCM_SERVICE_KEY == Dcm_SecCtrl.Dcm_SecServiceState))
    {
        /*Compare key success*/
        DslInternal_SetSecurityLevel(Dcm_SecCtrl.Dcm_NewSec);
        DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
        ret = DslInternal_GetSecurityCfgBySecLevel(Dcm_SecCtrl.Dcm_ActiveSec, &SecCfgIndex);
        if (E_OK == ret)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0u;
            Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_OFF;
            Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] = 0u;
            SchM_Exit_Dcm_ExclusiveArea();
            pSecurityRow = &(DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
            if (pSecurityRow != NULL_PTR)
            {
                Dcm_DspSecurityUsePortType DcmDspSecurityUsePort = pSecurityRow->DcmDspSecurityUsePort;
                Dcm_SetSecurityAttemptCounterFncType Dcm_SetSecAttCounterFnc =
                    pSecurityRow->Dcm_SetSecurityAttemptCounterFnc;
                if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                    && ((
                        (USE_ASYNCH_FNC == DcmDspSecurityUsePort)
                        || (USE_ASYNCH_CLIENT_SERVER == DcmDspSecurityUsePort))))
                {
                    if (Dcm_SetSecAttCounterFnc != NULL_PTR)
                    {
                        (void)Dcm_SetSecAttCounterFnc(
                            Dcm_SecCtrl.Dcm_OpStatus,
                            Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]);
                    }
                }
            }
        }
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

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
FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x10(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) && ((STD_ON == DCM_UDS_SERVICE0X10_ENABLED)))
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 iloop;
    Dcm_OpStatusType OpStatus = DCM_INITIAL;
    const Dcm_DspSessionType* pDcm_DspSession = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSession;
    uint8 SessionRowNum = pDcm_DspSession->DcmDspSessionRow_Num;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*session control sevice confirmation*/
    if (((DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle) || (DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle))
        && (0x10u == Dcm_MsgCtrl[MsgCtrlId].SID))
    {
        for (iloop = 0; iloop < SessionRowNum; iloop++)
        {
            const Dcm_DspSessionRowType* pDcmDspSessionRow = &pDcm_DspSession->pDcmDspSessionRow[iloop];
            Dcm_DspSessionForBootType DcmDspSessionForBoot = pDcmDspSessionRow->DcmDspSessionForBoot;
            if (Dcm_SesCtrl.Dcm_NewSes == pDcmDspSessionRow->DcmDspSessionLevel)
            {
                if ((DcmDspSessionForBoot == DCM_SYS_BOOT_RESPAPP) || (DcmDspSessionForBoot == DCM_OEM_BOOT_RESPAPP))
                {
                    ret = Dcm_SetProgConditions(OpStatus, &Dcm_ProgConditions);
                    if (ret == E_OK)
                    {
                        /* By this mode switch the DCM informs the BswM to jump to the bootloader.*/
                        (void)SchM_Switch_DcmEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
                    }
                    else if (ret == DCM_E_PENDING)
                    {
                        Dcm_ReqSetProgConditions = TRUE;
                    }
                    else
                    {
                        /*idle*/
                    }
                }
            }
        }
        /*Uds 0x10 service*/
        DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

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
FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x11(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
    uint8 MsgCtrlId;
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (((DCM_POS_RSP == Dcm_MsgCtrl[MsgCtrlId].RspStyle) || (DCM_POS_RSP_SUPPRESS == Dcm_MsgCtrl[MsgCtrlId].RspStyle))
        && (0x11u == Dcm_MsgCtrl[MsgCtrlId].SID))
    {
        if (TRUE == gAppl_UpdataOK_ResponseFlag)
        {
            gAppl_UpdataOK_ResponseFlag = FALSE; /*clear app updata flag*/
            DslInternal_SesRefresh(DCM_DEFAULT_SESSION);
        }
        else
        {
            /*confirm to EcuReset service,call Mcu_PerformReset trigger reset*/
            (void)SchM_Switch_DcmEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
        }
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

/*************************************************************************/
/*
 * Brief               <Diagnostic request packet is whether
 * allowed in the current application environment>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant/Non Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) DsdInternal_ManufacturerIndication(uint8 ProtocolCtrlId)
{
    Std_ReturnType ret;
#if (STD_ON == DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED)
    uint8 MsgCtrlIndexx;
    uint16 PortIndex;
    uint8 NotAcceptNum = 0u;
    uint8 Env_NokNum = 0u;
    uint16 ReqDatalen;
    uint8 ReqType;
    uint8 Sid;
    PduIdType RxPduId;
    uint8 ConnectionCfgId;
    uint16 SourceAddress;
    boolean Flag = FALSE;
    uint8* pReqDatax;
    Dcm_NegativeResponseCodeType ErrorCode;
    Dcm_NegativeResponseCodeType FirstErrorCode = DCM_INVALID_UINT8;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 Noti_PortNum = pDcmDsdCfg->DcmDsdServiceReqManufacturerNoti_PortNum;
    const Dcm_DsdServiceReqManuNotiType* pDcmDsdServiceReqManufacturerNoti =
        pDcmDsdCfg->pDcmDsdServiceReqManufacturerNoti;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
    pReqDatax = &Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData[0];
    ReqDatalen = (uint16)(Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen - 1u);
    ReqType = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
    /*find out the source address of request message */
    RxPduId = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
    ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[RxPduId].DcmDslParentConnectionCtrlId;
    SourceAddress =
        pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolRxTesterSourceAddr;
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    SourceAddress = (SourceAddress << 8u) | (SourceAddress >> 8u);
#endif
    for (PortIndex = 0; (PortIndex < Noti_PortNum) && (FALSE == Flag); PortIndex++)
    {
        if (NULL_PTR == ((pDcmDsdServiceReqManufacturerNoti)[PortIndex].Indication))
        {
            Flag = TRUE;
        }
        else
        {
            ret = (*((pDcmDsdServiceReqManufacturerNoti)[PortIndex]
                         .Indication))(Sid, pReqDatax, ReqDatalen, ReqType, SourceAddress, &ErrorCode);
            switch (ret)
            {
            case E_OK:
                break;
            case E_REQUEST_NOT_ACCEPTED:
                NotAcceptNum++;
                break;
            case E_NOT_OK:
            default:
                if (FirstErrorCode == DCM_INVALID_UINT8)
                {
                    FirstErrorCode = ErrorCode;
                }
                Env_NokNum++;
                break;
            }
        }
    }
    if (0u != NotAcceptNum)
    {
        /*the request is not accepted*/
        DsdInternal_ManufacturerConfirmation(ProtocolCtrlId);
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    else if ((0u != Env_NokNum) || (TRUE == Flag))
    {
        DsdInternal_SetNrc(ProtocolCtrlId, FirstErrorCode);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    else
    {
        ret = E_OK;
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
    ret = E_OK;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_ManufacturerConfirmation>
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
FUNC(void, DCM_CODE) DsdInternal_ManufacturerConfirmation(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED)
    uint8 MsgCtrlIndexx;
    uint16 PortIndex;
    uint8 Sid;
    uint8 ReqType;
    PduIdType RxPduId;
    uint8 ConnectionCfgId;
    uint16 SourceAddress;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 Noti_PortNum = pDcmDsdCfg->DcmDsdServiceReqManufacturerNoti_PortNum;
    const Dcm_DsdServiceReqManuNotiType* pDcmDsdServiceReqManufacturerNoti =
        pDcmDsdCfg->pDcmDsdServiceReqManufacturerNoti;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
    ReqType = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
    /*find out the source address of request message */
    RxPduId = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
    ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[RxPduId].DcmDslParentConnectionCtrlId;
    SourceAddress =
        pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolRxTesterSourceAddr;
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    SourceAddress = (SourceAddress << 8u) | (SourceAddress >> 8u);
#endif
    for (PortIndex = 0; PortIndex < Noti_PortNum; PortIndex++)
    {
        (void)(*((pDcmDsdServiceReqManufacturerNoti)[PortIndex].Confirmation))(
            Sid,
            ReqType,
            SourceAddress,
            DCM_RES_NEG_NOT_OK);
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_SupplierIndication>
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
FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SupplierIndication(uint8 ProtocolCtrlId)
{
    Std_ReturnType ret;
#if (STD_ON == DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED)
    uint8 MsgCtrlIndexx;
    uint16 PortIndex;
    uint8 NotAcceptNum = 0u;
    uint8 Env_NokNum = 0u;
    uint16 ReqDatalen;
    uint8 ReqType;
    uint8 Sid;
    PduIdType RxPduId;
    uint8 ConnectionCfgId;
    uint16 SourceAddress;
    boolean Flag = FALSE;
    uint8* pReqDatax;
    Dcm_NegativeResponseCodeType ErrorCode;
    Dcm_NegativeResponseCodeType FirstErrorCode = DCM_INVALID_UINT8;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 Noti_PortNum = pDcmDsdCfg->DcmDsdServiceReqSupplierNoti_PortNum;
    const Dcm_DsdServiceReqSuppNotiType* pDcmDsdServiceReqSupplierNoti = pDcmDsdCfg->pDcmDsdServiceReqSupplierNoti;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
    pReqDatax = &Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.pReqData[0];
    ReqDatalen = (uint16)(Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.ReqDataLen - 1u);
    ReqType = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
    /*find out the source address of request message */
    RxPduId = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
    ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[RxPduId].DcmDslParentConnectionCtrlId;
    SourceAddress =
        pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolRxTesterSourceAddr;
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    SourceAddress = (SourceAddress << 8u) | (SourceAddress >> 8u);
#endif
    for (PortIndex = 0; (PortIndex < Noti_PortNum) && (FALSE == Flag); PortIndex++)
    {
        if (NULL_PTR == ((pDcmDsdServiceReqSupplierNoti)[PortIndex].Indication))
        {
            Flag = TRUE;
        }
        else
        {
            ret = (*((pDcmDsdServiceReqSupplierNoti)[PortIndex]
                         .Indication))(Sid, pReqDatax, ReqDatalen, ReqType, SourceAddress, &ErrorCode);
            switch (ret)
            {
            case E_OK:
                break;
            case E_REQUEST_NOT_ACCEPTED:
                NotAcceptNum++;
                break;
            case E_NOT_OK:
            default:
                if (FirstErrorCode == DCM_INVALID_UINT8)
                {
                    FirstErrorCode = ErrorCode;
                }
                Env_NokNum++;
                break;
            }
        }
    }
    if (0u != NotAcceptNum)
    {
        /*the request is not accepted*/
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    else if ((0u != Env_NokNum) || (TRUE == Flag))
    {
        DsdInternal_SetNrc(ProtocolCtrlId, FirstErrorCode);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    else
    {
        ret = E_OK;
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
    ret = E_OK;
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_SupplierConfirmation>
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
FUNC(void, DCM_CODE) DsdInternal_SupplierConfirmation(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED)
    uint8 MsgCtrlIndexx;
    uint16 PortIndex;
    uint8 Sid;
    uint8 ReqType;
    PduIdType RxPduId;
    uint8 ConnectionCfgId;
    uint16 SourceAddress;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 Noti_PortNum = pDcmDsdCfg->DcmDsdServiceReqSupplierNoti_PortNum;
    const Dcm_DsdServiceReqSuppNotiType* pDcmDsdServiceReqSupplierNoti = pDcmDsdCfg->pDcmDsdServiceReqSupplierNoti;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
    ReqType = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.MsgAddInfo.ReqType;
    /*find out the source address of request message */
    RxPduId = Dcm_MsgCtrl[MsgCtrlIndexx].MsgContext.DcmRxPduId;
    ConnectionCfgId = pDsl_Protocol_Connection_RxCfg[RxPduId].DcmDslParentConnectionCtrlId;
    SourceAddress =
        pDsl_Protocol_ConnectionCfg[ConnectionCfgId].pDcmDslMainConnection->DcmDslProtocolRxTesterSourceAddr;
    for (PortIndex = 0; PortIndex < Noti_PortNum; PortIndex++)
    {
        (void)(*(
            (pDcmDsdServiceReqSupplierNoti)[PortIndex].Confirmation))(Sid, ReqType, SourceAddress, DCM_RES_NEG_NOT_OK);
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <Determine the diagnostic request
 * packet is allowed in the current session state.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_SesCheck(uint16 SidTabIndex, uint16 SidTabServieCfgIndex)
{
    uint8 ActiveSes;
    uint8 SesRefNum;
    uint8 SesCfgIndex;
    boolean Flag = FALSE;
    Std_ReturnType ret = E_OK;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable = DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pDcmDsdService =
        &pDcmDsdServiceTable[SidTabIndex].pDcmDsdService[SidTabServieCfgIndex];
    const uint8* pDcmDsdSessionLevelRef = pDcmDsdService->pDcmDsdSessionLevelRef;

    SesRefNum = pDcmDsdService->DcmDsdSessionLevel_Num;
    if (0u != SesRefNum)
    {
        /*get Currently valid session state*/
        ActiveSes = Dcm_MkCtrl.Dcm_ActiveSes;
        for (SesCfgIndex = 0; (SesCfgIndex < SesRefNum) && (FALSE == Flag); SesCfgIndex++)
        {
            if (ActiveSes == (pDcmDsdSessionLevelRef[SesCfgIndex]))
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*this service is not supported in the current valid session status*/
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_SubSesCheck>
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
DsdInternal_SubSesCheck(
    uint8 ProtocolCtrlId,
    uint8 Sid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 ActiveSes;
    uint8 SubSesRefNum;
    uint8 SubServiceNum;
    uint8 SesCfgIndex;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    uint16 SubServieCfgIndex;
    boolean Flag = FALSE;
    Std_ReturnType ret;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable = DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pDcmDsdService;
    const Dcm_DsdSubServiceCfgType* pDcmDsdSubService;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid, ProtocolCtrlId, &SidTabCfgIndex, &SidTabServiceCfgIndex);
    if (E_OK == ret)
    {
        ret = DsdInternal_SearchSidTabSubServiceIndex(
            ProtocolCtrlId,
            SidTabCfgIndex,
            SidTabServiceCfgIndex,
            &SubServieCfgIndex);
    }
    if (E_OK == ret)
    {
        pDcmDsdService = &pDcmDsdServiceTable[SidTabCfgIndex].pDcmDsdService[SidTabServiceCfgIndex];
        SubServiceNum = pDcmDsdService->DcmDsdSubService_Num;
        if (0u != SubServiceNum)
        {
            pDcmDsdSubService = &pDcmDsdService->DcmDsdSubService[SubServieCfgIndex];
            SubSesRefNum = pDcmDsdSubService->DcmDsdSubServiceSessionLevel_Num;
            if (0u != SubSesRefNum)
            {
                /*get Currently valid session state*/
                ActiveSes = Dcm_MkCtrl.Dcm_ActiveSes;
                for (SesCfgIndex = 0; (SesCfgIndex < SubSesRefNum) && (FALSE == Flag); SesCfgIndex++)
                {
                    if (ActiveSes == pDcmDsdSubService->DcmDsdSubServiceSessionLevelRef[SesCfgIndex])
                    {
                        Flag = TRUE;
                    }
                }
                if (FALSE == Flag)
                {
                    /*this service is not supported in the current valid session status*/
                    ret = E_NOT_OK;
                }
            }
        }
    }
    if (E_NOT_OK == ret)
    {
        /*the current session does not support the request service,send NRC = 0x7E*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
    }
    return ret;
}

#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <DsdInternal_DidSessionCheck>
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
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DsdInternal_DidSessionCheck(uint16 DidInfoCfgIndex, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 Index;
    boolean Flag = FALSE;
    const Dcm_DspDidReadType* pDspDidRead;
    Std_ReturnType ret = E_OK;
    uint8 DidReadSessionRefNum;

    pDspDidRead = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead;
    DidReadSessionRefNum = pDspDidRead->DcmDspDidReadSessionRefNum;
    if (NULL_PTR == pDspDidRead)
    {
        /*if the DcmDspDidRead of required Did is not configured,send NRC 0x31*/
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    else
    {
        if (DidReadSessionRefNum != 0u)
        {
            for (Index = 0; (Index < (DidReadSessionRefNum)) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidRead->pDcmDspDidReadSessionRow[Index])
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                /*the reading processing is not supported in current security level,send NRC 0x31*/
                (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <determine the diagnostic request packet is
 * allowed in the current security level>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number;
 *                      Sid: Service Id;>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_SecurityCheck(uint16 SidTabIndex, uint16 SidTabServieCfgIndex) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 SecRefNum;
    uint8 SecCfgIndex;
    const uint8* pSecCfg;
    boolean Flag = FALSE;
    Std_ReturnType ret = E_OK;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable = &DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable[SidTabIndex];
    const Dcm_DsdServiceCfgType* pDcmDsdService = &pDcmDsdServiceTable->pDcmDsdService[SidTabServieCfgIndex];

    /*get Currently valid security level*/
    SecRefNum = pDcmDsdService->DcmDsdSecurityLevel_Num;
    if (SecRefNum != 0u)
    {
        pSecCfg = pDcmDsdService->pDcmDsdSecurityLevelRef;
        for (SecCfgIndex = 0; (SecCfgIndex < SecRefNum) && (FALSE == Flag); SecCfgIndex++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSec == pSecCfg[SecCfgIndex])
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*this service is not supported in the current valid security level*/
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_SubSecurityCheck>
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
DsdInternal_SubSecurityCheck(
    uint8 ProtocolCtrlId,
    uint8 Sid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 ActiveSec;
    uint8 SubSecRefNum;
    uint8 SubServiceNum;
    uint8 SecCfgIndex;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    uint16 SubServieCfgIndex;
    const Dcm_DsdSubServiceCfgType* pSubService;
    boolean Flag = FALSE;
    Std_ReturnType ret;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pDcmDsdService;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid, ProtocolCtrlId, &SidTabCfgIndex, &SidTabServiceCfgIndex);
    if (E_OK == ret)
    {
        ret = DsdInternal_SearchSidTabSubServiceIndex(
            ProtocolCtrlId,
            SidTabCfgIndex,
            SidTabServiceCfgIndex,
            &SubServieCfgIndex);
        if (E_OK == ret)
        {
            pDcmDsdServiceTable = &DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable[SidTabCfgIndex];
            pDcmDsdService = &pDcmDsdServiceTable->pDcmDsdService[SidTabServiceCfgIndex];
            /*get Currently valid security level*/
            ActiveSec = Dcm_MkCtrl.Dcm_ActiveSec;
            SubServiceNum = pDcmDsdService->DcmDsdSubService_Num;
            if (0u != SubServiceNum)
            {
                pSubService = &pDcmDsdService->DcmDsdSubService[SubServieCfgIndex];
                SubSecRefNum = pSubService->DcmDsdSubServiceSecurityLevel_Num;
                if (0u != SubSecRefNum)
                {
                    for (SecCfgIndex = 0; (SecCfgIndex < SubSecRefNum) && (FALSE == Flag); SecCfgIndex++)
                    {
                        if (ActiveSec == pSubService->DcmDsdSubServiceSecurityLevelRef[SecCfgIndex])
                        {
                            Flag = TRUE;
                        }
                    }
                    if (FALSE == Flag)
                    {
                        /*this service is not supported in the current valid security level*/
                        ret = E_NOT_OK;
                    }
                }
            }
        }
    }
    if (E_NOT_OK == ret)
    {
        /*the current security does not support the request service,send NRC = 0x33*/
        *ErrorCode = DCM_E_SECURITYACCESSDENIED;
    }
    return ret;
}

#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
/*************************************************************************/
/*
 * Brief               <Determine the diagnostic request
 * packet is allowed in the Authentication state.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DsdInternal_AuthenticationCheck(uint8 ProtocolCtrlId, uint16 SidTabIndex, uint16 SidTabServieCfgIndex)
{
    uint8 RoleRefNum;
    Std_ReturnType ret;
    const Dcm_DsdServiceCfgType* pDcmDsdService =
        &DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable[SidTabIndex].pDcmDsdService[SidTabServieCfgIndex];

    RoleRefNum = pDcmDsdService->DcmDsdServiceRoleRef_Num;
    const uint8* RoleRef = pDcmDsdService->DcmDsdServiceRoleRef;
    /*check Currently valid Role state*/
    ret = DspInternal_RoleCheck(ProtocolCtrlId, RoleRef, RoleRefNum);
    if (E_NOT_OK == ret)
    {
        ret = DspInternal_WhiteListServiceCheck(ProtocolCtrlId);
    }
    return ret;
}
#endif

#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <DsdInternal_DidSecurityCheck>
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,Rule 8.7 */
DsdInternal_DidSecurityCheck(uint16 DidInfoCfgIndex, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,Rule 8.7 */
{
    const Dcm_DspDidReadType* pDspDidRead;
    uint8 SecNum;
    uint8 Index;
    Std_ReturnType ret = E_OK;
    boolean Flag = FALSE;

    pDspDidRead = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead;
    if (NULL_PTR == pDspDidRead)
    {
        /*if the DcmDspDidRead of required Did is not configured,send NRC 0x31*/
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    else
    {
        SecNum = pDspDidRead->DcmDspDidReadSecurityLevelRefNum;
        if (SecNum != 0u)
        {
            for (Index = 0; (Index < SecNum) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidRead->pDcmDspDidReadSecurityLevelRow[Index])
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                /*:the reading processing is not supported in current
                 * security level,send NRC 0x33*/
                *pNrc = DCM_E_SECURITYACCESSDENIED;
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED))
/*************************************************************************/
/*
 * Brief               <DsdInternal_DDDIDcheckPerSourceDID>
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
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DsdInternal_DDDIDcheckPerSourceDID(uint16 DDDidIdx, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    const Dcm_DDDidElementsDataTypes* DDDid;
    uint8 DDDidNum;
    uint8 iloop;
    uint16 Did;
    uint8 RangeDidCfgIndex = 0;
    uint16 DidCfgIndex;
    boolean RangeDidFlag;
    uint16 DidInfoCfgIndex;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;

    if ((Dcm_DspCfgPtr->DcmDspDDDIDcheckPerSourceDID != NULL_PTR)
        && (TRUE == *Dcm_DspCfgPtr->DcmDspDDDIDcheckPerSourceDID))
    {
        DDDidNum = Dcm_DDDid[DDDidIdx].DDDIDNumOfElements;
        DDDid = Dcm_DDDid[DDDidIdx].DcmDspAlternativeArgumentData;
        for (iloop = 0; (iloop < DDDidNum) && (E_OK == ret); iloop++)
        {
            if (DDDid->Subfunction == DCM_UDS0X2C_01_DDBYDID)
            {
                Did = (uint16)(DDDid->Data & 0xFFFFu);
                ret = DsdInternal_DidCheck(Did, &RangeDidCfgIndex, &DidCfgIndex, &RangeDidFlag);
                if (E_OK == ret)
                {
                    const Dcm_DspDidRangeType* pDcmDspDidRange = &Dcm_DspCfgPtr->pDcmDspDidRange[RangeDidCfgIndex];
                    const Dcm_DspDidType* pDcmDspDid = &Dcm_DspCfgPtr->pDcmDspDid[DidCfgIndex];
                    if (TRUE == RangeDidFlag)
                    {
                        DidInfoCfgIndex = pDcmDspDidRange->DcmDspDidRangeInfoIndex;
                    }
                    else
                    {
                        DidInfoCfgIndex = pDcmDspDid->DcmDspDidInfoIndex;
                    }
                    /*check the current session*/
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
                    ret = DsdInternal_DidSessionCheck(DidInfoCfgIndex, pNrc);
#endif
                    /*check the current security level*/
#if ((STD_ON == DCM_SECURITY_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
                    if (E_OK == ret)
                    {
                        ret = DsdInternal_DidSecurityCheck(DidInfoCfgIndex, pNrc);
                    }
#endif
                }
            }
            else if (DDDid->Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
            {
                ret = DsdInternal_DDDIDcheckMemory(DDDid->Data, DDDid->Size);
                if (ret == E_NOT_OK)
                {
                    (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
                }
            }
            else
            {
                /*idle*/
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <Setting the security level>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSec:To be changed the security level>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_SetSecurityLevel(Dcm_SecLevelType NewSec) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_SecCtrl.Dcm_ActiveSec = NewSec;
    Dcm_MkCtrl.Dcm_ActiveSec = NewSec;
    SchM_Exit_Dcm_ExclusiveArea();

#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
    /*check 2A is supported in NewSec*/
    Dcm_UDS0x2ACheckNewSecurity();
#endif

#if ((DCM_DSP_DID_FOR_2F_NUM > 0) && (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED))
    /*check 2F is supported in NewSec*/
    Dcm_UDS0x2FCheckNewSecurity(NewSec);
#endif
    DslInternal_InitDspProgramInfo();
}

/*************************************************************************/
/*
 * Brief               <Set SecurityAccess service process ,receives seed/key status>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Status:State to be modified>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_SetSecurityAccessStatus(Dcm_SecServiceStateType Status)
{

    Dcm_SecCtrl.Dcm_SecServiceState = Status;
}

/*************************************************************************/
/*
 * Brief               <DslInternal_GetSecurityCfgBySecLevel>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <Status:State to be modified>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DslInternal_GetSecurityCfgBySecLevel(Dcm_SecLevelType Dcm_SecLevel, P2VAR(uint8, AUTOMATIC, DCM_VAR) SecCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;
    uint8 SecurityRow_Num = pDcm_DspSecurity->DcmDspSecurityRow_Num;
    const Dcm_DspSecurityRowType* pDspSecurityRow = pDcm_DspSecurity->pDcm_DspSecurityRow;

    for (Index = 0; (Index < SecurityRow_Num) && (E_NOT_OK == ret); Index++)
    {
        if (Dcm_SecLevel == pDspSecurityRow[Index].DcmDspSecurityLevel)
        {
            *SecCfgIndex = Index;
            ret = E_OK;
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <start S3Timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_S3ServerStart(uint8 connectionId)
{
    Dcm_ResetTime(&Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTimer);
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTimer = 5000UL; /* ms converted to tick */
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State = DCM_S3TIMER_ON;
    Dcm_SesCtrl.Dcm_S3Ctrl.connectionId = connectionId;
    SchM_Exit_Dcm_ExclusiveArea();
}

/*************************************************************************/
/*
 * Brief               <stop S3Timer>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_S3ServerStop(void) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State = DCM_S3TIMER_OFF;
}

/*************************************************************************/
/*
 * Brief               <Refresh session>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:Session control state to be refreshed>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_SesRefresh(Dcm_SesCtrlType NewSes)
{
    DslInternal_SesTranslation(NewSes);
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
    /*check 2A is supported in NewSes*/
    Dcm_UDS0x2ACheckNewSession(NewSes);
#endif
#if ((DCM_DSP_DID_FOR_2F_NUM > 0) && (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED))
    /*check 2F is supported in NewSes*/
    Dcm_UDS0x2FCheckNewSession(NewSes);
#endif
#if (STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
    /*check 28 is supported in NewSes*/
    DspInternalUDS0x28_CheckNewSes(NewSes);
#endif
}
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

/*************************************************************************/
/*
 * Brief               <DslInternal_InitConnectionCtrl>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:Session control state to be refreshed>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_InitConnectionCtrl(uint8 ConnectionCtrlId)
{
    if (ConnectionCtrlId < DCM_CONNECTION_NUM)
    {
        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_ConnectionCtrl[ConnectionCtrlId].Dcm_ConnectionCfgIndex = DCM_INVALID_UINT8;
        Dcm_ConnectionCtrl[ConnectionCtrlId].Dcm_ConnectionActive = FALSE;
        SchM_Exit_Dcm_ExclusiveArea();
    }
}

/*************************************************************************/
/*
 * Brief               <Initializing the corresponding protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:Protocol control block index number.>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <Dcm_init()>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_InitProtocolCtrl(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8 Index;
    const Dcm_DspSessionType* pDcm_DspSession = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSession;
    Dcm_ProtocolCtrlType* pProtocolCtrl = &Dcm_ProtocolCtrl[ProtocolCtrlId];
    const Dcm_DspSessionRowType* pDcmDspSessionRow = pDcm_DspSession->pDcmDspSessionRow;

    for (Index = 0; Index < (pDcm_DspSession->DcmDspSessionRow_Num); Index++)
    {
        if (DCM_DEFAULT_SESSION == (pDcm_DspSession->pDcmDspSessionRow)[Index].DcmDspSessionLevel)
        {
            /*in DcmDspSessionRow,find the default session*/
            pProtocolCtrl->P2ServerMax = pDcmDspSessionRow[Index].DcmDspSessionP2ServerMax;
            pProtocolCtrl->P2StarServerMax = pDcmDspSessionRow[Index].DcmDspSessionP2StarServerMax;
            break;
        }
    }
#else
    DCM_UNUSED(ProtocolCtrlId);
#endif
}

/*************************************************************************/
/*
 * Brief               <DslInternal_ProtocolStartSession>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DslInternal_ProtocolStartSession(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
    DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    (void)SchM_Switch_DcmDiagnosticSessionControl(RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION);
#endif
}

/*************************************************************************/
/*
 * Brief               <Dcm_GetChannelNum>
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
FUNC(uint8, DCM_CODE) Dcm_GetChannelNum(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    return DCM_CHANNEL_NUM;
}

/*************************************************************************/
/*
 * Brief               <Dcm_GetChannelNum>
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
FUNC(uint8, DCM_CODE) Dcm_GetProtocolMsgNum(void)
{
    return DCM_DSLPROTOCOLROW_NUM_MAX;
}

/*************************************************************************/
/*
 * Brief               <Dcm_restarts3timer>
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
FUNC(void, DCM_CODE) Dcm_RestartS3timer(uint8 connectionId)
{
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    uint8 Index;
    boolean Flag = FALSE;
    uint8 ProtocolMsgNum = Dcm_GetProtocolMsgNum();
#if !DCM_DELAY_COMM_INACTIVE
    if (DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
#endif
    {
        for (Index = 0; (Index < ProtocolMsgNum) && (FALSE == Flag); Index++)
        {
            if (DCM_MSG_WAIT != Dcm_MsgCtrl[Index].Dcm_MsgState)
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*restart s3timer */
            DslInternal_S3ServerStart(connectionId);
        }
    }
#endif
}

/*************************************************************************/
/*
 * Brief               <Dcm_StopS3timer>
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
FUNC(void, DCM_CODE) Dcm_StopS3timer(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    /*Stop S3Server timer*/
#if !DCM_DELAY_COMM_INACTIVE
    if (DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
#endif
    {
        DslInternal_S3ServerStop();
    }
#endif
}

/*************************************************************************/
/*
 * Brief               <Dcm_StartOfReception_GetLen>
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
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(uint32, DCM_CODE)
Dcm_StartOfReception_GetLen(uint8 ServiceType, uint8 RxChannelCfgCtrlId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint32 len = 0;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    switch (ServiceType)
    {
#if (DCM_UDS_FUNC_ENABLED == STD_ON)
    case DCM_UDS:
        len = (pDcmDslCfg->pDcmChannelCfg)[RxChannelCfgCtrlId].Dcm_DslBufferSize;
        break;
#endif
#if (DCM_OBD_FUNC_ENABLED == STD_ON)
    case DCM_OBD:
        len = 8u;
        break;
#endif
    default:
        /*idle*/
        break;
    }
    return len;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called at the start of receiving an N-SDU. The N-SDU might
 be fragmented into multiple N-PDUs (FF with one or more following CFs) or might consist of a single
 N-PDU (SF)>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 *                      TpSduLength: This length identifies the overall number of bytes to be
 received.>
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType
 * containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK,BUFREQ_E_NOT_OK,BUFREQ_E_OVFL,BUFREQ_E_BUSY>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_InterStartOfReception(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 ConnectionCfgCtrlId;
    uint8 ProtocolCfgCtrlId;
    uint8 MsgCtrlIndex;
    uint8 RxChannelCfgCtrlId;
    Dcm_ProtocolType ProtocolId;
    uint8 ServiceType;
    uint32 Offset;
    Dcm_DslProtocolRxAddrType RxAddrType;
    BufReq_ReturnType bufRet;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg = DcmPbCfgPtr->DslProtocolConnectionCfg;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslBufferType* pDcmChannelCfg;

    ConnectionCfgCtrlId = pDsl_Protocol_Connection_RxCfg[id].DcmDslParentConnectionCtrlId;
    ProtocolCfgCtrlId = pDsl_Protocol_ConnectionCfg[ConnectionCfgCtrlId].DcmDslParentProtocolRowCtrlId;
    MsgCtrlIndex = ProtocolCfgCtrlId;
    RxChannelCfgCtrlId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId]
                             .DcmDslProtocolRxBufferRef->Dcm_DslBufferId;
    RxAddrType = pDsl_Protocol_Connection_RxCfg[id].DcmDslProtocolRxAddrType;
    ProtocolId = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId].DcmDslProtocolID;
    pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[RxChannelCfgCtrlId];
    ServiceType = Dcm_ClassifyServiceType(ProtocolId);

#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    bufRet = Dcm_UDS0x86_CheckProcessRoe(ProtocolCfgCtrlId);
    if (bufRet == BUFREQ_OK)
    {
#endif
        bufRet = Dcm_StartOfReception_Checklength(id, info, TpSduLength, bufferSizePtr);
        if (bufRet == BUFREQ_OK)
        {
            Offset = pDcmChannelCfg->offset;
            if ((info != NULL_PTR) && (0u < info->SduLength))
            {
                if (DCM_FUNCTIONAL == RxAddrType)
                {
                    switch (ServiceType)
                    {
                    case DCM_UDS: {
                        /*The UDS protocol functions addressing message, bypass treatment*/
                        Dcm_MemoryCopy(info->SduDataPtr, &(Dcm_FunctionalMessage.Buffer[0]), info->SduLength);
                        Dcm_FunctionalMessage.Length = info->SduLength;
                        /*save request length*/
                        bufRet = BUFREQ_OK;
                        break;
                    }
                    case DCM_OBD: {
                        /*The OBD protocol only have functions addressing message*/
                        Dcm_MemoryCopy(info->SduDataPtr, &(Dcm_OBDMessage.Buffer[0]), info->SduLength);
                        Dcm_OBDMessage.Length = info->SduLength; /*save request length*/
                        bufRet = BUFREQ_OK;
                        break;
                    }
                    default:
                        /*idle*/
                        break;
                    }
                }
                else
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                    if (DCM_PENDING_REQUEST_RECEIVE != Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_PendingRequestState)
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
                {
                    /*, lock buffer*/
                    SchM_Enter_Dcm_ExclusiveArea();
                    Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_ChannelRxState = DCM_CH_OCCUPIED;
                    Dcm_MemoryCopy(info->SduDataPtr, &Dcm_Channel[Offset], info->SduLength);
                    SchM_Exit_Dcm_ExclusiveArea();
                    Offset = Offset + (uint32)info->SduLength; /* PRQA S 2983 */ /* MISRA Rule 2.2 */
                    bufRet = BUFREQ_OK;
                }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
                else
                {
                    SchM_Enter_Dcm_ExclusiveArea();
                    Dcm_MemoryCopy(info->SduDataPtr, (uint8*)Dcm_RequestQueuedBuffer, info->SduLength);
                    Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_QueuedRequestBufferCunrentPosition += info->SduLength;
                    SchM_Exit_Dcm_ExclusiveArea();
                    bufRet = BUFREQ_OK;
                }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
            }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            if ((DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState)
                && (DCM_PENDING_REQUEST_NONE == Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState))
            {
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
                SchM_Enter_Dcm_ExclusiveArea();
                if (DCM_PHYSICAL == RxAddrType)
                {
                    Dcm_MsgCtrl[MsgCtrlIndex].MsgContext.ReqDataLen = TpSduLength;
                    Dcm_MsgCtrl[MsgCtrlIndex].MsgContext.pReqData = &Dcm_Channel[pDcmChannelCfg->offset];
                    Dcm_ChannelCtrl[RxChannelCfgCtrlId].Dcm_BufferCunrentPosition = Offset;
                }
                else
                {
                    Dcm_FunctionalMessage.Length = info->SduLength;
                }
                Dcm_ConnectionCtrl[ConnectionCfgCtrlId].Dcm_ConnectionActive = TRUE;
                SchM_Exit_Dcm_ExclusiveArea();
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            }
            else if (DCM_PHYSICAL == RxAddrType)
            {
                SchM_Enter_Dcm_ExclusiveArea();
                Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_QueuedRequestLength = TpSduLength;
                Dcm_QueuedRequestCtrl[Dcm_QueuedIndex].Dcm_QueuedRequestRxPduId = id;
                SchM_Exit_Dcm_ExclusiveArea();
            }
            else
            {
                Dcm_FunctionalMessage.Length = info->SduLength;
            }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
        }
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    }
#endif
    return bufRet;
}
#define DCM_STOP_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType
 * containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
Dcm_CopyRxData_SessionDeal(
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    uint8 ServiceType,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Sid,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) SubFunction,
    uint8 ProtocolCfgId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType bufRet = BUFREQ_E_NOT_OK;

    switch (ServiceType)
    {
    case DCM_UDS:
        /*The UDS protocol functions addressing message, bypass treatment*/
        Dcm_MemoryCopy(
            info->SduDataPtr,
            &(Dcm_FunctionalMessage.Buffer[Dcm_FunctionalMessage.Length]),
            info->SduLength);
        Dcm_FunctionalMessage.Length += info->SduLength; /*save request length*/
        *Sid = Dcm_FunctionalMessage.Buffer[0];
        *SubFunction = Dcm_FunctionalMessage.Buffer[1];
        bufRet = BUFREQ_OK;
        break;
    case DCM_OBD:
        /*The OBD protocol only have functions addressing message*/
        Dcm_MemoryCopy(info->SduDataPtr, &(Dcm_OBDMessage.Buffer[Dcm_OBDMessage.Length]), info->SduLength);
        Dcm_OBDMessage.Length += info->SduLength; /*save request length*/
        bufRet = BUFREQ_OK;
        break;
    default:
        /*idle*/
        break;
    }
#if (STD_OFF == DCM_REQUEST_QUEUED_ENABLED)
    Dcm_MsgCtrlType* pDcm_MsgCtrl = &Dcm_MsgCtrl[ProtocolCfgId];
    if ((DCM_MSG_WAIT != pDcm_MsgCtrl->Dcm_MsgState)
        && ((*Sid != 0x3Eu) || (*SubFunction != 0x80u) || (0x02u != Dcm_FunctionalMessage.Length)))
    {
        bufRet = BUFREQ_E_NOT_OK;
        /*shall clear function buffer*/
        Dcm_FunctionalMessage.Length = 0;
        Dcm_FunctionalMessage.ConcurrentTesterFailed = TRUE;
    }
#endif
    return bufRet;
}
#define DCM_STOP_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_SetTxPduId>
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
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DCM_CODE)
Dcm_TpRxIndication_SetTxPduId(uint8 MsgCtrlIndexx, P2CONST(Dcm_DslConnectionType, AUTOMATIC, DCM_CONST) pDslConnection)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    if (TRUE == Dcm_UDS0x86_GetTxPduId(&Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId))
#endif
    {
        Dcm_MsgCtrl[MsgCtrlIndexx].DcmTxPduId =
            pDslConnection->pDcmDslMainConnection->pDcmDslProtocolTx->DcmDslTxPduRPduId;
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_CheckUDS0x86_TxConfirmation>
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
FUNC(boolean, DCM_CODE) Dcm_CheckUDS0x86_TxConfirmation(uint8 ProtocolCfgCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean ret;
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    ret = Dcm_UDS0x86_TxConfirmation(ProtocolCfgCtrlId);
#else
    ret = TRUE;
    DCM_UNUSED(ProtocolCfgCtrlId);
#endif
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <The lower layer communication interface module
 *                       confirms the transmission of an IPDU.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define DCM_START_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_TxConfirmation_2A(PduIdType DcmTxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
    uint8 index;

    for (index = 0; index < DCM_PERIODICCONNECTION_NUM; index++)
    {
        if (Scheduler_0x2A_Transmit[index].PduId == DcmTxPduId)
        {
            Scheduler_0x2A_Transmit[index].Transmit = FALSE;
        }
    }
#else
    DCM_UNUSED(DcmTxPduId);
#endif
}
#define DCM_STOP_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
/*************************************************************************/
/*
 * Brief               <>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <MsgPtr,MsgLen,DcmRxPduId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(BufReq_ReturnType, DCM_CODE)
DslInternal_ResponseOnOneEvent(P2VAR(PduInfoType, AUTOMATIC, DCM_VAR) PduInfo, PduIdType DcmRxPduId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    PduLengthType availabeBufferLength;
    PduLengthType DataLength = PduInfo->SduLength;
    BufReq_ReturnType ret;
    PduInfo->SduLength = 0;
    ret = Dcm_InterStartOfReception(DcmRxPduId, PduInfo, DataLength, &availabeBufferLength);
    PduInfo->SduLength = DataLength;
    if (ret == BUFREQ_OK)
    {
        ret = Dcm_CopyRxData(DcmRxPduId, PduInfo, &availabeBufferLength);
    }
    if (ret == BUFREQ_OK)
    {
        Dcm_TpRxIndication(DcmRxPduId, E_OK);
    }
    else
    {
        Dcm_TpRxIndication(DcmRxPduId, E_NOT_OK);
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <Dcm_GetConnectionNum>
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
FUNC(uint8, DCM_CODE) Dcm_GetConnectionNum(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    return DCM_CONNECTION_NUM;
}

#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
OBD_ReadPidValues(const OBD_ReadPidValuesTypes* OBD_ReadPid, uint32* pBufOffset, uint8* pNoFindPidNum)
{
    uint32 txOffset = *pBufOffset;
    uint32 Offset = 0;
    uint32 SumOffset = 0;
    uint32 BufferOffset = 0;
    boolean Find = FALSE;
    uint8 iloop;
    uint8 ctrlIndex = 0;
    uint8 Buffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 txBuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint16 buffersize = 0;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspPidDataType* pDcmDspPidData;
    const Dcm_DspPidType* pDcmDspPid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspPid;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;

    for (iloop = 0; (FALSE == Find) && (iloop < Dcm_DspCfgPtr->DcmDspPidNum); iloop++)
    {
        if ((OBD_ReadPid->pid == pDcmDspPid[iloop].DcmDspPidIdentifier)
            && ((((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                  || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                 && (OBD_ReadPid->SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                || (((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                     || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                    && (OBD_ReadPid->SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
            && (TRUE == pDcmDspPid[iloop].DcmDspPidUsed))
        {
            Find = TRUE;
            ctrlIndex = iloop;
        }
    }

    if (TRUE == Find)
    {
        Dcm_MemSet(&txBuffer[Offset], 0x00, pDcmDspPid[ctrlIndex].DcmDspPidSize);
        switch (OBD_ReadPid->SID)
        {
        case SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA:
            ret = OBD_ReadPidValues_0x01(OBD_ReadPid->pid, ctrlIndex, pBufOffset, OBD_ReadPid->UDSCall);
            break;
#if (STD_ON == DCM_OBD_SERVICE0X02_ENABLED)
        case SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA:
            pDcmDspPidData = pDcmDspPid[ctrlIndex].pDcmDspPidData;
            if (pDcmDspPidData != NULL_PTR)
            {
                for (iloop = 0; iloop < pDcmDspPid[iloop].pDcmDspPidDataNum; iloop++)
                {
                    Offset = BufferOffset;
                    if (E_OK
                        == Dem_DcmReadDataOfOBDFreezeFrame(
                            OBD_ReadPid->pid,
                            (uint8)pDcmDspPidData->DcmDspPidDataPos,
                            &Buffer[Offset],
                            &buffersize))
                    {
                        SumOffset += (uint32)buffersize;
                    }
                    Dcm_MemoryCopy(&Buffer[BufferOffset], &txBuffer[BufferOffset], buffersize);
                    BufferOffset += buffersize;
                    pDcmDspPidData++;
                }
            }
            if ((BufferOffset <= (uint32)pDcmDspPid[ctrlIndex].DcmDspPidSize) && (SumOffset != 0u))
            {
                SchM_Enter_Dcm_ExclusiveArea();
                if (TRUE == OBD_ReadPid->UDSCall)
                {
                    Dcm_Channel[txOffset] = 0xFF;
                    txOffset++;
                }
                Dcm_Channel[txOffset] = OBD_ReadPid->pid;
                txOffset++;
                Dcm_Channel[txOffset] = 0x00;
                txOffset++;
                Dcm_MemoryCopy(txBuffer, &Dcm_Channel[txOffset], pDcmDspPid[ctrlIndex].DcmDspPidSize);
                SchM_Exit_Dcm_ExclusiveArea();
                txOffset += (uint32)pDcmDspPid[ctrlIndex].DcmDspPidSize;
                *pBufOffset = txOffset;
                ret = E_OK;
            }
            break;
#endif /* STD_ON == DCM_OBD_SERVICE0X02_ENABLED */
        default:
            /*idle*/
            break;
        }
    }
    else
    {
        (*pNoFindPidNum)++;
        ret = E_OK; /* can not find pid,return E_OK */
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               <Check supported Parameter id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
FUNC(void, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
OBD_CheckSupportedPIDs(uint8 pid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf, uint8 SID)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    boolean Find = FALSE;
    const Dcm_DspPidType* pDcmDspPid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspPid;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    uint8 DcmDspPidNum = Dcm_DspCfgPtr->DcmDspPidNum;

    /* check parameter */
    if (NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
#if (STD_ON == DCM_DSP_PID_FUNC_ENABLED)
        for (iloop = 0; iloop < DcmDspPidNum; iloop++)
        {
            if (((pDcmDspPid[iloop].DcmDspPidIdentifier >= (pid + 0x01u))
                 && (pDcmDspPid[iloop].DcmDspPidIdentifier <= (pid + 0x20u)))
                && ((((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                      || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                     && (SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                    || (((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                         || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
                && (TRUE == pDcmDspPid[iloop].DcmDspPidUsed))

            {
                *pDataBuf |= (uint32)1u << (0x20u - (pDcmDspPid[iloop].DcmDspPidIdentifier - pid));
            }

            if (((pid == pDcmDspPid[iloop].DcmDspPidIdentifier) || (pid == 0x0u))
                && ((((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                      || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                     && (SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                    || (((pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                         || (pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
                && (TRUE == pDcmDspPid[iloop].DcmDspPidUsed))
            {
                Find = TRUE;
            }
        }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
#endif
    }
}
#endif /* DCM_DSP_PID_FUNC_ENABLED == STD_ON */

#if (DCM_DSP_REQUESTCONTROL_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Check supported test id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <none>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, DCM_CODE) OBD_CheckSupportedTIDs(uint8 Tid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    boolean Find = FALSE;
    const Dcm_DspRequestControlType* pDcmDspRequestControl = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRequestControl;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    uint8 RequestControlNum = Dcm_DspCfgPtr->DcmDspRequestControlNum;

    /* check parameter */
    if (NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
        for (iloop = 0; iloop < RequestControlNum; iloop++)
        {
            uint8 RequestControlTestId = pDcmDspRequestControl[iloop].DcmDspRequestControlTestId;
            if ((RequestControlTestId >= (Tid + 0x01u)) && (RequestControlTestId <= (Tid + 0x20u)))
            {
                *pDataBuf |= (uint32)1u << (0x20u - (RequestControlTestId - Tid));
            }

            if ((RequestControlTestId == Tid) || (Tid == 0x0u))
            {
                Find = TRUE;
            }
        }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
    }
}

/******************************************************************************/
/*
 * Brief               <find config index number of TID>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
OBD_FindCfgIndexOfTestID(uint8 testID, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspRequestControlType* pDcmDspRequestControl = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRequestControl;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;

    /* check parameter */
    if (NULL_PTR != pCfgIndex)
    {
        for (iloop = 0; (iloop < Dcm_DspCfgPtr->DcmDspRequestControlNum) && (ret == E_NOT_OK); iloop++)
        {
            if (testID == pDcmDspRequestControl[iloop].DcmDspRequestControlTestId)
            {
                (*pCfgIndex) = iloop;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif

#if (DCM_DSP_VEHINFO_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Set Availability InfoType Value>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <InfoType && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
FUNC(void, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
OBD_SetAvailabilityInfoTypeValue(uint8 InfoType, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 iloop;
    boolean Find = FALSE;
    const Dcm_DspVehInfoType* pDcmDspVehInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspVehInfo;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    uint8 VehInfoInfoType;

    /* check parameter */
    if (NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
        for (iloop = 0; iloop < Dcm_DspCfgPtr->DcmDspVehInfoNum; iloop++)
        {
            VehInfoInfoType = pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType;
            if ((VehInfoInfoType >= (InfoType + 0x01u)) && (VehInfoInfoType <= (InfoType + 0x20u)))
            {
                *pDataBuf |= (uint32)1u << (0x20u - (VehInfoInfoType - InfoType));
            }

            if ((VehInfoInfoType == InfoType) || (InfoType == 0x0u))
            {
                Find = TRUE;
            }
        }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
    }
    return;
}

/******************************************************************************/
/*
 * Brief               <find config index number of VehInfo>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
OBD_FindCfgIndexOfVehInfo(uint8 vehInfo, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 iloop;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspVehInfoType* pDcmDspVehInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspVehInfo;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    uint8 DcmDspVehInfoNum = Dcm_DspCfgPtr->DcmDspVehInfoNum;

    /* check parameter */
    if (NULL_PTR != pCfgIndex)
    {
        for (iloop = 0; (iloop < DcmDspVehInfoNum) && (E_NOT_OK == ret); iloop++)
        {
            if (vehInfo == pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType)
            {
                (*pCfgIndex) = iloop;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <Dcm_GetRxIdNum>
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
FUNC(uint8, DCM_CODE) Dcm_GetRxIdNum(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    return DCM_DSL_RX_ID_NUM;
}

/*************************************************************************/
/*
 * Brief               <Dcm_GetTxIdNum>
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
FUNC(uint8, DCM_CODE) Dcm_GetTxIdNum(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    return DCM_DSL_TX_ID_NUM;
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <S3Timer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_S3Timer(void)
{
    uint32 OldTimer;
    uint32 ExpireTimer;
    uint32 Timer;

    /*S3Server timer*/
    if (DCM_S3TIMER_ON == (Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State))
    {
        OldTimer = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTimer;
        ExpireTimer = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTimer;
        Dcm_GetTimeSpan(OldTimer, &Timer);
        if (Timer >= ExpireTimer)
        {
            /*S3Server Timer timeout*/
            DslInternal_S3ServerTimeout();
        }
    }
}
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <SecTimer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_SecTimer(void)
{
    uint32 OldTimer;
    uint32 ExpireTimer;
    uint32 Timer;
    uint8 SecCfgIndex;
    const Dcm_DspSecurityRowType* pSecurityRow;
    Dcm_SecTimerCtrlType* pDcm_RunDlyCtrl = &Dcm_SecCtrl.Dcm_RunDlyCtrl;
    const Dcm_DspSecurityType* pDcm_DspSecurity = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity;

    /***************************************/
    if (0u == (Dcm_SecCtrl.Dcm_SecFlag & DCM_SEC_CALL_OVER_MASK))
    {
        Dcm_MainFunction_ReadSecTimer();
    }
    else
    {
        /*security timer*/
        for (SecCfgIndex = 0; SecCfgIndex < DCM_SECURITY_NUM; SecCfgIndex++)
        {
            if (DCM_SECTIMER_ON == (pDcm_RunDlyCtrl->Dcm_SecTimerState[SecCfgIndex]))
            {
                OldTimer = pDcm_RunDlyCtrl->Dcm_SecCurTimer[SecCfgIndex];
                ExpireTimer = pDcm_RunDlyCtrl->Dcm_SecExpiredTimer[SecCfgIndex];
                Dcm_GetTimeSpan(OldTimer, &Timer);
                if (Timer >= ExpireTimer)
                {
                    /*security timeout*/
                    SchM_Enter_Dcm_ExclusiveArea();
                    pDcm_RunDlyCtrl->Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_OFF;
                    /*********************************/
                    Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] = 0;
                    Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0;
                    Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_IDLE;
                    SchM_Exit_Dcm_ExclusiveArea();
                    pSecurityRow = &(pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
                    if (pSecurityRow != NULL_PTR)
                    {
                        Dcm_DspSecurityUsePortType DcmDspSecurityUsePort = pSecurityRow->DcmDspSecurityUsePort;
                        Dcm_SetSecurityAttemptCounterFncType Dcm_SetSecAttCounterFnc =
                            pSecurityRow->Dcm_SetSecurityAttemptCounterFnc;
                        if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                            && ((USE_ASYNCH_FNC == DcmDspSecurityUsePort)
                                || (USE_ASYNCH_CLIENT_SERVER == DcmDspSecurityUsePort)))
                        {
                            if (Dcm_SetSecAttCounterFnc != NULL_PTR)
                            {
                                (void)Dcm_SetSecAttCounterFnc(
                                    Dcm_SecCtrl.Dcm_OpStatus,
                                    Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*************************************************************************/
/*
 * Brief               <Read SecTimer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_ReadSecTimer(void)
{
    uint8 SecCfgIndex;
    const Dcm_DspSecurityRowType* pSecurityRow = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity->pDcm_DspSecurityRow;
    Std_ReturnType ret;
    uint8* pDcmFalseAcessCount = Dcm_SecCtrl.Dcm_FalseAcessCount;

#if (DCM_SECURITY_MAX_READOUT_TIME != 0)
    if (Dcm_SecCtrl.Dcm_MaxReadoutTime < DCM_SECURITY_MAX_READOUT_TIME)
    {
        Dcm_SecCtrl.Dcm_MaxReadoutTime++;
#endif
        for (SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex; (SecCfgIndex < DCM_SECURITY_NUM) && (pSecurityRow != NULL_PTR);
             SecCfgIndex++)
        {
            if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                && ((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                    || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
            {
                if (pSecurityRow->Dcm_GetSecurityAttemptCounterFnc != NULL_PTR)
                {
                    Dcm_OpStatusType OpStatus = DCM_PENDING;
                    if ((Dcm_SecCtrl.Dcm_SecFlag & DCM_SEC_CALL_AGAIN_MASK) == 0u)
                    {
                        OpStatus = DCM_INITIAL;
                    }
                    ret = pSecurityRow->Dcm_GetSecurityAttemptCounterFnc(OpStatus, &pDcmFalseAcessCount[SecCfgIndex]);
                    if (DCM_E_PENDING == ret)
                    {
                        Dcm_SecCtrl.Dcm_SecFlag |= DCM_SEC_CALL_AGAIN_MASK;
                        break;
                    }
                    else
                    {
                        Dcm_SecCtrl.Dcm_SecFlag &= (~DCM_SEC_CALL_AGAIN_MASK);
                        Dcm_SecCtrl.Dcm_SecCfgIndex++;
                    }
                    if (E_NOT_OK == ret)
                    {
                        pDcmFalseAcessCount[SecCfgIndex] = pSecurityRow->DcmDspSecurityNumAttDelay;
                    }
                    if (pDcmFalseAcessCount[SecCfgIndex] >= pSecurityRow->DcmDspSecurityNumAttDelay)
                    {
                        Dcm_ResetTime(&Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTimer[SecCfgIndex]);
                        Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTimer[SecCfgIndex] =
                            (pSecurityRow->DcmDspSecurityDelayTime > pSecurityRow->DcmDspSecurityDelayTimeOnBoot)
                                ? pSecurityRow->DcmDspSecurityDelayTime
                                : pSecurityRow->DcmDspSecurityDelayTimeOnBoot;
                        Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_ON;
                    }
                }
            }
            else
            {
                Dcm_SecCtrl.Dcm_SecCfgIndex++;
            }
            pSecurityRow++;
        }
        if (Dcm_SecCtrl.Dcm_SecCfgIndex == DCM_SECURITY_NUM)
        {
            Dcm_SecCtrl.Dcm_SecFlag |= DCM_SEC_CALL_OVER_MASK;
        }
#if (DCM_SECURITY_MAX_READOUT_TIME != 0u)
    }
#endif
    else
    {
        SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex;
        pSecurityRow = &(DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
        pSecurityRow->Dcm_GetSecurityAttemptCounterFnc(DCM_CANCEL, &pDcmFalseAcessCount[SecCfgIndex]);

        for (SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex; (SecCfgIndex < DCM_SECURITY_NUM); SecCfgIndex++)
        {
            if ((pSecurityRow != NULL_PTR) && (TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                && ((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                    || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
            {
                pDcmFalseAcessCount[SecCfgIndex] = pSecurityRow->DcmDspSecurityNumAttDelay;
                Dcm_ResetTime(&Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTimer[SecCfgIndex]);
                Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTimer[SecCfgIndex] =
                    (pSecurityRow->DcmDspSecurityDelayTime > pSecurityRow->DcmDspSecurityDelayTimeOnBoot)
                        ? pSecurityRow->DcmDspSecurityDelayTime
                        : pSecurityRow->DcmDspSecurityDelayTimeOnBoot;
                Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_ON;
            }
            pSecurityRow++;
        }
        Dcm_SecCtrl.Dcm_SecFlag |= DCM_SEC_CALL_OVER_MASK;
    }
}
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               <Page_Buffer Timer background processing function>
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
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_MainFunction_PageBufferTimer(void)
{
    uint8 iloop;
    uint8 MsgCtrlId;
    uint32 OldTimer;
    uint32 ExpireTimer;
    uint32 Timer;

    for (iloop = 0; iloop < DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num; iloop++)
    {
        MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
        if ((DCM_INVALID_UINT8 != MsgCtrlId) && (TRUE == Dcm_PageBufferData.TimerStart))
        {
            OldTimer = Dcm_PageBufferData.CurTimer;
            ExpireTimer = Dcm_PageBufferData.ExpiredTimer;
            Dcm_GetTimeSpan(OldTimer, &Timer);
            if (Timer >= ExpireTimer)
            {
                /*PageBuffer Timer timeout*/
                Dcm_PageBufferData.TimeOut = TRUE;
            }
        }
    }
}
#endif /* DCM_PAGEDBUFFER_ENABLED == STD_ON */

#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED))
/*************************************************************************/
/*
 * Brief               <DsdInternal_DidCheck>
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
static FUNC(Std_ReturnType, DCM_CODE) DsdInternal_DidCheck(
    uint16 receiveDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) pRangeDidCfgIndex,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) pRangeDidFlag)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Dcm_DidSupportedType DidSupported;
    uint16 Index;
    uint8 Idx;
    boolean Flag = FALSE;
    boolean Flagx = FALSE;
    Std_ReturnType ret = E_NOT_OK;
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
    uint16 iloop;
#endif
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    uint8 DcmDspDidRangeNum = Dcm_DspCfgPtr->DcmDspDidRangeNum;
    uint16 DcmDspDidNum = Dcm_DspCfgPtr->DcmDspDidNum;
    const Dcm_DspDidType* pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo;
    const Dcm_DspDidRangeType* pDcmDspDidRange;

    *pRangeDidFlag = FALSE;
    /*first check whether the receiveDid is single did*/
    for (Index = 0; (Index < DcmDspDidNum) && (FALSE == Flag); Index++)
    {
        pDcmDspDid = &Dcm_DspCfgPtr->pDcmDspDid[Index];
        pDcmDspDidInfo = &Dcm_DspCfgPtr->pDcmDspDidInfo[pDcmDspDid->DcmDspDidInfoIndex];
        /*single did check*/
        if ((receiveDid == pDcmDspDid->DcmDspDidId) && (TRUE == pDcmDspDid->DcmDspDidUsed))
        {
            if ((receiveDid >= 0xF200u) && (receiveDid <= 0xF3FFu)
                && (pDcmDspDidInfo->DcmDspDidDynamicallyDefined == TRUE)
                && (pDcmDspDidInfo->DcmDspDDDIDMaxElements > 0u))
            {
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (Flag == FALSE); iloop++)
                {
                    if (pDcmDspDid->DcmDspDidId == Dcm_DDDid[iloop].DDDid)
                    {
                        Flag = TRUE;
                        (*pDidCfgIndex) = Index;
                        ret = E_OK;
                    }
                }
#endif
            }
            else
            {
                Flag = TRUE;
                (*pDidCfgIndex) = Index;
                ret = E_OK;
            }
        }
    }
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    /*if the receiveDid is not single did,the check whether the receiveDid is range did*/
    if (FALSE == Flag)
    {
        /*range did check*/
        if ((receiveDid < 0xF200u) || (receiveDid > 0xF3FFu))
        {
            /*range did can not be DDDid*/
            for (Idx = 0; (Idx < DcmDspDidRangeNum) && (FALSE == Flagx); Idx++)
            {
                pDcmDspDidRange = &Dcm_DspCfgPtr->pDcmDspDidRange[Idx];
                /*this range not have gaps*/
                if ((receiveDid >= pDcmDspDidRange->DcmDspDidRangeIdentifierLowerLimit)
                    && (receiveDid <= pDcmDspDidRange->DcmDspDidRangeIdentifierUpperLimit))
                {
                    if (TRUE == pDcmDspDidRange->DcmDspDidRangeHasGaps)
                    {
                        if (pDcmDspDidRange->DcmDspDidRangeIsDidAvailableFnc != NULL_PTR)
                        {
                            ret = pDcmDspDidRange->DcmDspDidRangeIsDidAvailableFnc(
                                receiveDid,
                                DCM_INITIAL,
                                &DidSupported);
                            if ((ret == E_OK) && (DCM_DID_SUPPORTED == DidSupported))
                            {
                                *pRangeDidCfgIndex = Idx;
                                *pRangeDidFlag = TRUE;
                                Flagx = TRUE;
                            }
                        }
                    }
                    else
                    {
                        *pRangeDidCfgIndex = Idx;
                        *pRangeDidFlag = TRUE;
                        Flagx = TRUE;
                    }
                }
            }
        }
        if (FALSE == Flagx)
        {
            ret = E_NOT_OK;
        }
        else
        {
            (*pDidCfgIndex) = Index;
            ret = E_OK;
        }
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DsdInternal_DDDIDcheckMemory>
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
static FUNC(Std_ReturnType, DCM_CODE) DsdInternal_DDDIDcheckMemory(uint32 addr, uint32 size)
{
    Std_ReturnType ret = E_OK;
    uint8 IdInfoIndex;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryIdInfoType* pDcmDspMemoryIdInfo;
    uint8 RangeInfoIndex;
    uint8 index;
    uint8 Num;
    boolean Find;
    const Dcm_DspCfgType* Dcm_DspCfgPtr = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspMemoryType* pDcmDspMemory = Dcm_DspCfgPtr->pDcmDspMemory;
    uint8 DcmDspMemoryIdInfoNum = pDcmDspMemory->DcmDspMemoryIdInfoNum;
    uint8 MemoryRangeInfo_Num;

    for (IdInfoIndex = 0u; (E_OK == ret) && (IdInfoIndex < DcmDspMemoryIdInfoNum); IdInfoIndex++)
    {
        pDcmDspMemoryIdInfo = &(pDcmDspMemory->DcmDspMemoryIdInfo[IdInfoIndex]);
        if (pDcmDspMemoryIdInfo != NULL_PTR)
        {
            MemoryRangeInfo_Num = pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfoNum;
            for (RangeInfoIndex = 0; (RangeInfoIndex < MemoryRangeInfo_Num) && (E_OK == ret); RangeInfoIndex++)
            {
                pDcmDspReadMemoryRangeInfo = &(pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfo[RangeInfoIndex]);
                Num = pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeSessionLevelRefNum;

                if ((addr >= pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeLow)
                    && ((addr + size) <= pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeHigh) && (Num != 0u))
                {
                    Find = FALSE;
                    for (index = 0; (index < Num) && (Find == FALSE); index++)
                    {
                        if (Dcm_MkCtrl.Dcm_ActiveSes
                            == pDcmDspReadMemoryRangeInfo->pDcmDspReadMemoryRangeSessionLevelRow[index])
                        {
                            Find = TRUE;
                        }
                    }
                    if (Find == FALSE)
                    {
                        ret = E_NOT_OK;
                    }
                }
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <initial of Security level control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <Dcm_Init()>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_InitSecCtrl(void)
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0u;
    Dcm_SecCtrl.Dcm_ActiveSec = DCM_SEC_LEV_LOCKED;
    Dcm_SecCtrl.Dcm_NewSec = DCM_SEC_LEV_LOCKED;
    Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_IDLE;
    Dcm_SecCtrl.Dcm_SecFlag = (Dcm_SecCtrl.Dcm_SecFlag & 0x00u) | DCM_SEC_RESUMECALL_MASK;
    Dcm_SecCtrl.Dcm_MaxReadoutTime = 0u;
    Dcm_SecCtrl.Dcm_SecCfgIndex = 0u;
    Dcm_SecCtrl.Dcm_OpStatus = DCM_INITIAL;
    for (uint8 index = 0; index < DCM_SECURITY_NUM; index++)
    {
        Dcm_SecCtrl.Dcm_FalseAcessCount[index] = 0;
    }
    SchM_Exit_Dcm_ExclusiveArea();
}

#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*************************************************************************/
/*
 * Brief               <Set the session control state>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_SetSesCtrlType(Dcm_SesCtrlType NewSes)
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_SesCtrl.Dcm_ActiveSes = NewSes;
    Dcm_MkCtrl.Dcm_ActiveSes = NewSes;
    SchM_Exit_Dcm_ExclusiveArea();
}

/*************************************************************************/
/*
 * Brief               <The set NewSes corresponding configuration session time parameters>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <(*P2ServerTimer):P2ServerTimer which corresponding to NewSes
 *                      (*P2StarServerTimer):P2StarServerTimer which corresponding to NewSes>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DslInternal_GetSesTimingValues(
    Dcm_SesCtrlType NewSes,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) P2ServerTimer,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) P2StarServerTimer)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 Number;
    uint8 Index;
    boolean Flag;
    const Dcm_DspSessionRowType* pDspSessionRow;
    Std_ReturnType ret = E_OK;
    const Dcm_DspSessionType* pDcm_DspSession = DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSession;

    if ((NULL_PTR != P2ServerTimer) && (NULL_PTR != P2StarServerTimer))
    {
        pDspSessionRow = pDcm_DspSession->pDcmDspSessionRow;
        Number = pDcm_DspSession->DcmDspSessionRow_Num;
        Flag = FALSE;
        for (Index = 0; (Index < Number) && (FALSE == Flag); Index++)
        {
            if (NewSes == (pDspSessionRow[Index].DcmDspSessionLevel))
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*do not configured DcmDspSessionRow container ,or
             * do not find the corresponding NewSes in DcmDspSessionRows*/
            ret = E_NOT_OK;
        }
        else
        {
            *P2ServerTimer = (uint32)pDspSessionRow[Index - 1u].DcmDspSessionP2ServerMax;
            *P2StarServerTimer = (uint32)pDspSessionRow[Index - 1u].DcmDspSessionP2StarServerMax;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Set the module support each protocol
 * link layer parameters ,which is in protocol control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_SetSesTimingValues(Dcm_SesCtrlType NewSes)
{
    uint32 P2ServerTimer;
    uint32 P2StarServerTimer;
    Std_ReturnType ret;

    /*Get the new session configuration corresponding to the time parameters of Dsp module*/
    ret = DslInternal_GetSesTimingValues(NewSes, &P2ServerTimer, &P2StarServerTimer);
    if (E_OK == ret)
    {
        /*set all protocols which are configured,corresponding
         * link layer Time parameters of Dsl module*/
        DslInternal_SetProtocolLinkLayerTimer(P2ServerTimer, P2StarServerTimer);
    }
}

/*************************************************************************/
/*
 * Brief               <session change from default to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_DefaultToDefault(Dcm_SesCtrlType NewSes)
{
    DslInternal_SetSesCtrlType(NewSes);

    DslInternal_SetSesTimingValues(NewSes);
}

/*************************************************************************/
/*
 * Brief               <session change from default to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_DefaultToUndefault(Dcm_SesCtrlType NewSes)
{
    NetworkHandleType NetworkChannel;
    uint8 Index;
    NetworkChannel = Dcm_MkCtrl.Dcm_ActiveNetwork;
    uint8 MainConnectionNum = Dcm_GetMainConnectionNum();

    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_UNDEFAULT;

    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);
    /* Notice Comm module into "Full Communication" */
    for (Index = 0; Index < MainConnectionNum; Index++)
    {
        if ((DCM_COMM_ACTIVE == Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic)
            && (NetworkChannel == Dcm_CommCtrl[Index].DcmDslProtocolComMChannelId))
        {
            ComM_DCM_ActiveDiagnostic(NetworkChannel);
        }
    }
    /************************************************************
     * The session state from the default to the undefault,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
}

/*************************************************************************/
/*
 * Brief               <session change from undefault to undefault>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_UndefaultToUndefault(Dcm_SesCtrlType NewSes)
{
    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
    DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
#endif
    /************************************************************
     * The session state from the undefault to the undefault,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
}

/*************************************************************************/
/*
 * Brief               <session change from undefault to default>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_UndefaultTodefault(Dcm_SesCtrlType NewSes)
{
#if !DCM_DELAY_COMM_INACTIVE
    NetworkHandleType NetworkChannel;
    NetworkChannel = Dcm_MkCtrl.Dcm_ActiveNetwork;
#endif
    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_DEFAULT;

    DslInternal_SetSesCtrlType(NewSes);
    DslInternal_SetSesTimingValues(NewSes);

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
    DslInternal_SetSecurityAccessStatus(DCM_SERVICE_IDLE);
#endif

#if ((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X85_ENABLED))
    /*The update of the DTC status will be re-enabled*/
    Rte_EnableAllDtcsRecord();
#endif

#if !DCM_DELAY_COMM_INACTIVE
    /*Notice Comm Module exit "Full Communication"*/
    ComM_DCM_InactiveDiagnostic(NetworkChannel);
    /************************************************************
     * The session state from the undefault to the default,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
    /*this is add for FOTA,when download is break,notifi FOTA*/
    (void)DslInternal_ProtocolStop(Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId);
#endif
}

/*************************************************************************/
/*
 * Brief               <When the session is changed, the specific processing>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <NewSes:the Latest session status>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) DslInternal_SesTranslation(Dcm_SesCtrlType NewSes)
{

#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
    Dcm_UDS0x86_SesTranslation(NewSes);
#endif

    DslInternal_InitDspProgramInfo();
    if (DCM_SESSION_DEFAULT == Dcm_SesCtrl.Dcm_SessionState)
    {
        if (DCM_DEFAULT_SESSION == NewSes)
        {
            /*from the default session to the default session */
            DslInternal_DefaultToDefault(NewSes);
        }
        else
        {
            /*from the default session to the undefault session */
            DslInternal_DefaultToUndefault(NewSes);
        }
    }
    else
    {
        if (DCM_DEFAULT_SESSION == NewSes)
        {
            /*from the undefault session to the default session */
            DslInternal_UndefaultTodefault(NewSes);
        }
        else
        {
            /*from the undefault session to the undefault session */
            DslInternal_UndefaultToUndefault(NewSes);
        }
    }
}

/*************************************************************************/
/*
 * Brief               <timeout of S3Timer>
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
static FUNC(void, DCM_CODE) DslInternal_S3ServerTimeout(void)
{

    Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
    DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
    /************************************************************
     * The session state from the default to the default,
     * specific needs to be done temporarily unclear, to be added
     ************************************************************/
#if DCM_DELAY_COMM_INACTIVE
    if ((DCM_S3TIMER_ON == Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State) && (Dcm_MkCtrl.Dcm_ActiveNetwork != DCM_INVALID_UINT8))
    {
        /*Notice ComM exit "FULL Communication"*/
        ComM_DCM_InactiveDiagnostic(Dcm_MkCtrl.Dcm_ActiveNetwork);
        Dcm_MkCtrl.Dcm_ActiveNetwork = DCM_INVALID_UINT8;
    }
#endif
    /***stop S3Timer***/
    DslInternal_S3ServerStop();
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    /*SWS_Dcm_01483 Fallback to deauthenticated session on S3server timeout
    If the Dcm is in a non-default session and a S3server timeout occurs, the Dcm shall
    perform a transition from authenticated into deauthenticated state on the authentication
    state assigned to that connection which was in a non-default session.*/
    uint8 connectionId = Dcm_GetconnectionIndex(Dcm_SesCtrl.Dcm_S3Ctrl.connectionId);
    Dcm_SetDeAuthenticate(connectionId);
#endif
}

/*************************************************************************/
/*
 * Brief               <The initialization of session management control block>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static inline FUNC(void, DCM_CODE) DslInternal_InitSesCtrl(void)
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_SesCtrl.Dcm_ActiveSes = DCM_DEFAULT_SESSION;
    Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTimer = DCM_INVALID_UINT32;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTimer = DCM_INVALID_UINT32;
    Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State = DCM_S3TIMER_OFF;
    Dcm_SesCtrl.Dcm_S3Ctrl.connectionId = 0;
    Dcm_SesCtrl.Dcm_SessionState = DCM_SESSION_DEFAULT;
    SchM_Exit_Dcm_ExclusiveArea();
}
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

/*************************************************************************/
/*
 * Brief               <Dcm_TpRxIndication_3E80>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_TpRxIndication_3E80(uint8 connectionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_UDS_FUNC_ENABLED)
    uint8 Sid;
    uint8 SubFunction;

    Sid = Dcm_FunctionalMessage.Buffer[0];
    SubFunction = Dcm_FunctionalMessage.Buffer[1];
    if ((0x3Eu == Sid) && (0x80u == SubFunction) && (0x02u == Dcm_FunctionalMessage.Length))
    {
        /*This is TesterPresent Request */
        /*****************S3Serer timer****************/
        Dcm_RestartS3timer(connectionId);
        /*shall clear function buffer*/
        Dcm_FunctionalMessage.Length = 0;
        ret = E_NOT_OK;
    }
#endif
    return ret;
}

#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    OBD_ReadPidValues_0x01(uint8 pid, uint8 ctrlIndex, P2VAR(uint32, AUTOMATIC, DCM_VAR) pBufOffset, boolean UDSCall)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 txOffset = *pBufOffset;
    uint32 Offset = 0;
    uint8 iloop;
    uint8 Buffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 txBuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    Std_ReturnType ret = E_NOT_OK;
    uint8 pDcmDspPidDataNum;

    const Dcm_DspPidType* pDcmDspPid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspPid;
    const Dcm_DspPidDataType* pDcmDspPidData = pDcmDspPid[ctrlIndex].pDcmDspPidData;
    Dcm_PidReadDataFncType DcmDspPidDataReadFnc = pDcmDspPidData->DcmDspPidService01->DcmDspPidDataReadFnc;
    const Dcm_DspPidDataSupportInfoType* pDcmDspPidDataSupportInfo;

    if (pDcmDspPidData != NULL_PTR)
    {
        pDcmDspPidDataNum = pDcmDspPid[ctrlIndex].pDcmDspPidDataNum;
        pDcmDspPidDataSupportInfo = pDcmDspPidData->pDcmDspPidDataSupportInfo;
        for (iloop = 0; iloop < pDcmDspPidDataNum; iloop++)
        {
            if (DcmDspPidDataReadFnc != NULL_PTR)
            {
                const Dcm_DspPidSupportInfoType* DcmDspPidSupportInfo;
                ret = (DcmDspPidDataReadFnc)(&Buffer[Offset]);
                if (E_OK == ret)
                {
                    if (pDcmDspPidDataSupportInfo != NULL_PTR)
                    {
                        DcmDspPidSupportInfo = pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoRef;
                        OBD_BITS_SET(txBuffer, pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoBit);
                        Dcm_MemoryCopy(
                            &Buffer[Offset],
                            &txBuffer[DcmDspPidSupportInfo->DcmDspPidSupportInfoPos],
                            DcmDspPidSupportInfo->DcmDspPidSupportInfoLen);
                    }
                    else
                    {
                        Dcm_MemoryCopy(&Buffer[Offset], &txBuffer[Offset], pDcmDspPidData->DcmDspPidDataSize);
                    }
                }
                else
                {
                    if (pDcmDspPidDataSupportInfo != NULL_PTR)
                    {
                        DcmDspPidSupportInfo = pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoRef;
                        OBD_BITS_SET(txBuffer, pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoBit);
                        Dcm_MemSet(
                            &txBuffer[DcmDspPidSupportInfo->DcmDspPidSupportInfoPos],
                            0x00,
                            DcmDspPidSupportInfo->DcmDspPidSupportInfoLen);
                    }
                    else
                    {
                        Dcm_MemSet(&txBuffer[Offset], 0x00, pDcmDspPidData->DcmDspPidDataSize);
                    }
                }
            }
            Offset += (uint32)pDcmDspPidData->DcmDspPidDataSize;
            pDcmDspPidData++;
        }
    }
    if (Offset <= (uint32)pDcmDspPid[ctrlIndex].DcmDspPidSize)
    {
        SchM_Enter_Dcm_ExclusiveArea();
        if (TRUE == UDSCall)
        {
            Dcm_Channel[txOffset] = 0xFF;
            txOffset++;
        }
        Dcm_Channel[txOffset] = pid;
        txOffset++;
        Dcm_MemoryCopy(txBuffer, &Dcm_Channel[txOffset], pDcmDspPid[ctrlIndex].DcmDspPidSize);
        SchM_Exit_Dcm_ExclusiveArea();
        txOffset += (uint32)pDcmDspPid[ctrlIndex].DcmDspPidSize;
        *pBufOffset = txOffset;
        ret = E_OK;
    }
    return ret;
}
#endif /* DCM_DSP_PID_FUNC_ENABLED == STD_ON */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
