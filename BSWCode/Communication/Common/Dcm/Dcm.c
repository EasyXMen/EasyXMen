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
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    :  Dcm.c                                                     **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Implementation of Dcm                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm.h"
#include "Dcm_Internal.h"
/*============================================================================*/

/*=======[V E R S I O N  C H E C K]===========================================*/
#ifndef DCM_AR_RELEASE_MAJOR_VERSION
#error " Dcm version miss"
#endif
#ifndef DCM_AR_RELEASE_MINOR_VERSION
#error " Dcm version miss"
#endif
#ifndef DCM_AR_RELEASE_PATCH_VERSION
#error " Dcm version miss"
#endif

#if ((2u != DCM_CFG_H_SW_MAJOR_VERSION) || (1u != DCM_CFG_H_SW_MINOR_VERSION) || (0u != DCM_CFG_H_SW_PATCH_VERSION))
#error " Dcm version mismatching"
#endif

/* PRQA S 1520 EOF */ /* VL_Dcm_1520 */
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
static FUNC(void, DCM_CODE) Dcm_MainFunction_PendingManage(void);

/*************************************************************************/
/*
 * Brief               <P2Server Timer background processing function>
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
static FUNC(void, DCM_CODE) Dcm_MainFunction_P2ServerTimer(void);

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
static FUNC(void, DCM_CODE) Dcm_GetUDSPhyPduID(P2VAR(PduIdType, AUTOMATIC, DCM_VAR) rxPhyPduId);

static FUNC(void, DCM_CODE) Dcm_BootloaderResponse(void);

static FUNC(void, DCM_CODE) Dcm_MainFunction_ConfirmProcess(void);

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define DCM_START_SEC_VAR_INIT_PTR
#include "Dcm_MemMap.h"
P2CONST(Dcm_CfgType, AUTOMATIC, DCM_CONST_PBCFG) DcmPbCfgPtr = NULL_PTR;
#define DCM_STOP_SEC_VAR_INIT_PTR
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_MkCtrlType, DCM_VAR_NOINIT) Dcm_MkCtrl;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
VAR(boolean, DCM_VAR_POWER_ON_INIT) gAppl_UpdataOK_ResponseFlag = FALSE;

VAR(boolean, DCM_VAR_POWER_ON_INIT) Dcm_ReqSetProgConditions = FALSE;

VAR(boolean, DCM_VAR_POWER_ON_INIT) Dcm_ConfirmNotifyFlag = FALSE;

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
VAR(boolean, DCM_VAR_POWER_ON_INIT) Dcm_NewSecFlag = FALSE;
#endif

VAR(boolean, DCM_VAR_POWER_ON_INIT) Dcm_NewSesFlag = FALSE;
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_ProgConditionsType, DCM_VAR_NOINIT) Dcm_ProgConditions;

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
VAR(Dcm_PageBufferDataType, DCM_VAR_NOINIT) Dcm_PageBufferData;
#endif

#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X34_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X35_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X36_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X38_ENABLED))
VAR(Dcm_DspProgramType, DCM_VAR_NOINIT) Dcm_DspProgram;
#endif
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_GENERIC_CONNECTION)
#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
VAR(uint8, DCM_VAR_NOINIT) Dcm_Metadata[DCM_DSLPROTOCOLROW_NUM_MAX][4];
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_GENERIC_CONNECTION */

#if (STD_ON == DCM_TIME_MAINFUNCTION_ENABLED)
#define DCM_START_SEC_VAR_POWER_ON_INIT_32
#include "Dcm_MemMap.h"
VAR(uint32, DCM_VAR_POWER_ON_INIT) Dcm_Timer = 0u;
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_32
#include "Dcm_MemMap.h"
#endif /*  STD_ON == DCM_TIME_MAINFUNCTION_ENABLED*/
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
 * ServiceId            <0x01>
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
FUNC(void, DCM_CODE) Dcm_Init(P2CONST(Dcm_CfgType, DCM_CONST, DCM_CONST_PBCFG) ConfigPtr)
{
    uint32                           Index;
    uint8                            MainConnectionNum = Dcm_GetMainConnectionNum();
    const Dcm_DslMainConnectionType* pDsl_Protocol_MainConnectionCfg;

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        DCM_DET_REPORTERROR(DCM_INIT_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        DcmPbCfgPtr                     = ConfigPtr;
        pDsl_Protocol_MainConnectionCfg = DcmPbCfgPtr->DslProtocolMainConnectionCfg;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        Dcm_QueuedIndex = 0u;
        DslInternal_InitChannelQueuedRequestCtrl();
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
        for (Index = 0; Index < DcmPbCfgPtr->pDcmDslCfg->DcmChannelCfg_Num; Index++)
        {
            DslInternal_InitChannelCtrl((uint8)Index);
        }
        for (Index = 0; Index < DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num; Index++)
        {
            DslInternal_InitMsgCtrl((uint8)Index);
        }
        for (Index = 0; Index < DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num; Index++)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_ProtocolCtrl[(uint8)Index].ProtocolId        = DCM_INVALID_UINT8;
            Dcm_ProtocolCtrl[(uint8)Index].ProtocolPri       = DCM_INVALID_UINT8;
            Dcm_ProtocolCtrl[(uint8)Index].MsgCtrlIndex      = DCM_INVALID_UINT8;
            Dcm_ProtocolCtrl[(uint8)Index].P2ServerMax       = 50UL;   /* ms converted to tick */
            Dcm_ProtocolCtrl[(uint8)Index].P2StarServerMax   = 5000UL; /* ms converted to tick */
            Dcm_ProtocolCtrl[(uint8)Index].P2ServerMin       = 0UL;
            Dcm_ProtocolCtrl[(uint8)Index].P2StarServerMin   = 0UL;
            Dcm_ProtocolCtrl[(uint8)Index].S3Server          = 5000UL; /* ms converted to tick */
            Dcm_ProtocolCtrl[(uint8)Index].Dcm_ProtocolState = DCM_PROTOCOL_INVALID;
            SchM_Exit_Dcm_ExclusiveArea();
            DslInternal_InitProtocolCtrl((uint8)Index);
        }
        Dcm_InterInit();
        for (Index = 0; Index < MainConnectionNum; Index++)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_CommCtrl[Index].Dcm_CommState        = DCM_COMM_NO_COMMUNICATION;
            Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
            Dcm_CommCtrl[Index].DcmDslProtocolComMChannelId =
                pDsl_Protocol_MainConnectionCfg[Index].DcmDslProtocolComMChannelId;
            SchM_Exit_Dcm_ExclusiveArea();
        }

        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_MkCtrl.Dcm_ActiveSes               = DCM_DEFAULT_SESSION;
        Dcm_MkCtrl.Dcm_ActiveSec               = DCM_SEC_LEV_LOCKED;
        Dcm_MkCtrl.Dcm_ActiveProtocol          = DCM_NO_ACTIVE_PROTOCOL;
        Dcm_MkCtrl.Dcm_ActiveNetwork           = DCM_INVALID_UINT8;
        Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId = DCM_INVALID_UINT8;
        Dcm_MkCtrl.Dcm_MkState                 = DCM_ON;
        Dcm_MkCtrl.Sid                         = DCM_INVALID_UINT8;
        Dcm_MkCtrl.ReqType                     = DCM_INVALID_UINT8;

        Dcm_OBDMessage.Length        = 0u;
        Dcm_FunctionalMessage.Length = 0u;
        SchM_Exit_Dcm_ExclusiveArea();

#if (STD_ON == DCM_GENERIC_CONNECTION)
        Dcm_MemSet((uint8*)Dcm_Metadata, 0u, (4u * DCM_DSLPROTOCOLROW_NUM_MAX));
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        DslInternal_InitPageBuffer();
#endif /* STD_ON == DCM_PAGEDBUFFER_ENABLED */
#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X34_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X35_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X36_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X38_ENABLED))
        DslInternal_InitDspProgramInfo();
#endif
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DDDID_STORAGE == STD_ON))
        Dcm_UDS0x2C_Init();
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DDDID_STORAGE == STD_ON */
    }
}

#if (STD_ON == DCM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to
 * where to store the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR) VersionInfo)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETVERSIONINFO_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == VersionInfo)
    {
        DCM_DET_REPORTERROR(DCM_GETVERSIONINFO_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfo->vendorID         = DCM_VENDOR_ID;
        VersionInfo->moduleID         = DCM_MODULE_ID;
        VersionInfo->sw_major_version = DCM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = DCM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = DCM_SW_PATCH_VERSION;
    }
}
#endif /* STD_ON == DCM_VERSION_INFO_API */
/*************************************************************************/
/*
 * Brief               <Function to get the VIN (as defined in SAE J1979-DA)>
 * ServiceId           <0x07>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <Data: Pointer to where to store the VIN>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType:E_OK: The Data pointer has been filled with valid VIN
                                       E_NOT_OK: The default VIN will be used in the DoIP>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetVin(P2VAR(uint8, AUTOMATIC, DCM_VAR) Data)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETVIN_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == Data)
    {
        DCM_DET_REPORTERROR(DCM_GETVIN_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (Dcm_VIN.Flag == TRUE)
        {
            Dcm_MemoryCopy(Dcm_VIN.DcmVIN, Data, 17);
            ret = E_OK;
        }
    }
    return ret;
}

#if (DCM_DEM_SUPPOTR == STD_ON)
/*************************************************************************/
/*
 * Brief               <Triggers on changes of the UDS DTC status byte.
 *                      Allows to trigger on ROE Event for subservice OnDTCStatusChanged.>
 * ServiceId           <0x2B>
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
FUNC(Std_ReturnType, DCM_CODE)
Dcm_DemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_DEMTRIGGERONDTCSTATUS_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        Dcm_InterDemTriggerOnDTCStatus(DTC, DTCStatusOld, DTCStatusNew);
        ret = E_OK;
    }
    return ret;
}
#endif
/*************************************************************************/
/*
 * Brief               <This function provides the active security level value.>
 * ServiceId           <0x0d>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SecLevel:Pointer to Active Security Level value >
 * Param-Name[in/out]   <None>
 * Return               <E_OK:
 *                      E_NOT_OK:>
 * PreCondition         <...>
 * CallByAPI            <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_GetSecurityLevel(P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR) SecLevel)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETSECURITYLEVEL_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == SecLevel)
    {
        DCM_DET_REPORTERROR(DCM_GETSECURITYLEVEL_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        *SecLevel = Dcm_MkCtrl.Dcm_ActiveSec;
        ret       = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <This function provides the active session control type value. >
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SesType:Active Session Control Type value>
 * Param-Name[in/out]  <None>
 * Return              <E_OK,E_NOT_OK>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_GetSesCtrlType(P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_VAR) SesType)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETSESCTRLTYPE_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == SesType)
    {
        DCM_DET_REPORTERROR(DCM_GETSESCTRLTYPE_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        *SesType = Dcm_MkCtrl.Dcm_ActiveSes;
        ret      = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <This function returns the active protocol name. >
 * ServiceId           <0x0f>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <description...>
 * Param-Name[out]     <ActiveProtocol:Active protocol type value >
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_GetActiveProtocol(P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR) ActiveProtocol)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETACTIVEPROTOCOL_ID, DCM_E_UNINIT);
    }
    else if (NULL_PTR == ActiveProtocol)
    {
        DCM_DET_REPORTERROR(DCM_GETACTIVEPROTOCOL_ID, DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        *ActiveProtocol = Dcm_MkCtrl.Dcm_ActiveProtocol;
        if ((DCM_OBD_ON_CAN == *ActiveProtocol) || (DCM_OBD_ON_FLEXRAY == *ActiveProtocol)
            || (DCM_OBD_ON_IP == *ActiveProtocol))
        {
            *ActiveProtocol = DCM_NO_ACTIVE_PROTOCOL;
        }
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <The call to this function allows the application
 *                      to reset the current session to Default session.>
 * ServiceId           <0x2a>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(void)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_RESETTODEFAULTSESSION_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
        DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes, TRUE);
        (void)SchM_Switch_DcmDiagnosticSessionControl(RTE_MODE_DcmDiagnosticSessionControl_DCM_DEFAULT_SESSION);
    }
    return E_OK;
}

/*************************************************************************/
/*
 * Brief               <The call to this function allows to trigger an
 *                      event linked to a ResponseOnEvent request.>
 * ServiceId           <0x2D>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <RoeEventId: Identifier of the event that is triggered>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerOnEvent(uint8 RoeEventId)
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_TRIGGERONEVENT_ID, DCM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else
#endif
    {
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
        ret = Dcm_UDS0x86_ChangeOfDataIdentifier(RoeEventId);
#else
        DCM_UNUSED(RoeEventId);
#endif
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Allows to activate and deactivate the call
 *                      of ComM_DCM_ActiveDiagnostic() function.>
 * ServiceId           <0x56>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <active If false Dcm shall not call ComM_DCM_ActiveDiagnostic().
 *                      If true Dcm will call ComM_DCM_ActiveDiagnostic().>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(boolean active)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          Index;
    uint8          MainConnectionNum = Dcm_GetMainConnectionNum();
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (DCM_ON != Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_SETACTIVEDIAGNOSTIC_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        if (FALSE == active)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            for (Index = 0; Index < MainConnectionNum; Index++)
            {
                Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic = DCM_COMM_NOT_ACTIVE;
            }
            SchM_Exit_Dcm_ExclusiveArea();
        }
        else
        {
            SchM_Enter_Dcm_ExclusiveArea();
            for (Index = 0; Index < MainConnectionNum; Index++)
            {
                Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
                ComM_DCM_ActiveDiagnostic(Dcm_CommCtrl[Index].DcmDslProtocolComMChannelId);
            }
            SchM_Exit_Dcm_ExclusiveArea();
        }
        ret = E_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <This service is used for processing the tasks of the main loop. >
 * ServiceId           <0x25>
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
FUNC(void, DCM_CODE) Dcm_MainFunction(void)
{
    if (DCM_ON == Dcm_MkCtrl.Dcm_MkState)
    {
#if (STD_ON == DCM_TIME_MAINFUNCTION_ENABLED)
        if (Dcm_Timer < (DCM_TIME_OVERFLOW - Dcm_GeneralCfg.DcmTaskTime))
        {
            Dcm_Timer += Dcm_GeneralCfg.DcmTaskTime;
        }
        else
        {
            Dcm_Timer = Dcm_GeneralCfg.DcmTaskTime - (DCM_TIME_OVERFLOW - Dcm_Timer);
        }
#endif /* STD_ON == DCM_TIME_MAINFUNCTION_ENABLED */
        /**************************************************/
        /*P2Server Timer background processing function*/
        Dcm_MainFunction_P2ServerTimer();

        Dcm_MainFunction_ConfirmProcess();

        /***************************************************/
        /*The Pending message background processing function*/
        Dcm_MainFunction_PendingManage();

        Dcm_InterMainFunction();

        Dcm_BootloaderResponse();
    }
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*************************************************************************/
/*
 * Brief               <This service is used for processing confirmation remaining operations of the main loop. >
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
static FUNC(void, DCM_CODE) Dcm_MainFunction_ConfirmProcess(void)
{
    const Dcm_DspSecurityRowType* pSecurityRow;
    uint8                         SecCfgIndex;
    Std_ReturnType                ret;

    if (Dcm_ConfirmNotifyFlag)
    {
        DsdInternal_ManufacturerConfirmation(Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId);
        DsdInternal_SupplierConfirmation(Dcm_MkCtrl.Dcm_ActiveProtocolCfgCtrlId);
        Dcm_ConfirmNotifyFlag = FALSE;
    }
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (Dcm_NewSecFlag)
    {
        Dcm_NewSecFlag = FALSE;
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
        /*check 2A is supported in NewSec*/
        Dcm_UDS0x2ACheckNewSecurity();
#endif
#if ((STD_ON == DCM_UDS_SERVICE0X2F_ENABLED) && (DCM_DSP_DID_FOR_2F_NUM > 0))
        Dcm_UDS0x2FCheckNewSecurity(Dcm_MkCtrl.Dcm_ActiveSec);
#endif
        ret = DslInternal_GetSecurityCfgBySecLevel(Dcm_SecCtrl.Dcm_ActiveSec, &SecCfgIndex);
        if (E_OK == ret)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_SecCtrl.Dcm_SubfunctionForSeed                        = 0u;
            Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_OFF;
            Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]              = 0u;
            SchM_Exit_Dcm_ExclusiveArea();
            pSecurityRow = &(DcmPbCfgPtr->pDcmDspCfg->pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
            if (pSecurityRow != NULL_PTR)
            {
                Dcm_DspSecurityUsePortType           DcmDspSecurityUsePort = pSecurityRow->DcmDspSecurityUsePort;
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
#endif
    if (Dcm_NewSesFlag)
    {
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
        /*check 2A is supported in NewSes*/
        Dcm_UDS0x2ACheckNewSession(Dcm_SesCtrl.Dcm_NewSes);
#endif
#if ((DCM_DSP_DID_FOR_2F_NUM > 0) && (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED))
        /*check 2F is supported in NewSes*/
        Dcm_UDS0x2FCheckNewSession(Dcm_SesCtrl.Dcm_NewSes);
#endif
        Dcm_NewSesFlag = FALSE;
    }
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
static FUNC(void, DCM_CODE) Dcm_MainFunction_PendingManage(void)
{
    uint8                        iloop;
    uint8                        MsgCtrlId;
    uint16                       SidTabCfgIndex;
    uint16                       SidTabServiceCfgIndex;
    Std_ReturnType               ret       = E_OK;
    Dcm_NegativeResponseCodeType errorCode = DCM_E_RESERVED;
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_DiagnosticServicex)
    (Dcm_OpStatusType OpStatus,
     uint8            ProtocolCtrlId,
     P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
    uint8                        tempIndex;
    PduIdType                    queuedPduRxPduId;
    uint8                        ConnectionCfgCtrlId;
    uint8                        TxChannelCfgCtrlId;
    const Dcm_DslProtocolRxType* pDsl_Protocol_Connection_RxCfg = DcmPbCfgPtr->DslProtocolConnectionRxCfg;
    const Dcm_DslConnectionType* pDsl_Protocol_ConnectionCfg    = DcmPbCfgPtr->DslProtocolConnectionCfg;
    uint8                        ProtocolCfgCtrlId;
    uint8                        MsgCtrlIndex;
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */
    const Dcm_DslProtocolType*        pDcmDslProtocol     = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol;
    const Dcm_DsdServiceTableCfgType* pDcmDsdServiceTable = DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable;

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
    SchM_Enter_Dcm_ExclusiveArea();
    if ((DCM_PENDING_REQUEST_READY == Dcm_QueuedRequestCtrl[0u].Dcm_PendingRequestState)
        && (DCM_PENDING_REQUEST_READY == Dcm_QueuedRequestCtrl[1u].Dcm_PendingRequestState))
    {
        tempIndex = (Dcm_QueuedIndex == 0u) ? 1u : 0u;
    }
    else
    {
        tempIndex = Dcm_QueuedIndex;
    }
    queuedPduRxPduId = Dcm_QueuedRequestCtrl[tempIndex].Dcm_QueuedRequestRxPduId;
    if (queuedPduRxPduId != 0xffu)
    {
        ConnectionCfgCtrlId = pDsl_Protocol_Connection_RxCfg[queuedPduRxPduId].DcmDslParentConnectionCtrlId;
        ProtocolCfgCtrlId   = pDsl_Protocol_ConnectionCfg[ConnectionCfgCtrlId].DcmDslParentProtocolRowCtrlId;
        MsgCtrlIndex        = ProtocolCfgCtrlId;
        TxChannelCfgCtrlId =
            pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCfgCtrlId].DcmDslProtocolTxBufferRef->Dcm_DslBufferId;
        if ((DCM_PENDING_REQUEST_READY == Dcm_QueuedRequestCtrl[tempIndex].Dcm_PendingRequestState)
            && (DCM_MSG_WAIT == Dcm_MsgCtrl[MsgCtrlIndex].Dcm_MsgState)
            && (DCM_CH_IDLE == Dcm_ChannelCtrl[TxChannelCfgCtrlId].Dcm_ChannelTxState))
        {
            Dcm_MsgCtrl[MsgCtrlIndex].Dcm_MsgState = DCM_MSG_RECEIVED;
            SchM_Exit_Dcm_ExclusiveArea();
            Dcm_MkCtrl.Dcm_ActiveNetwork =
                pDsl_Protocol_ConnectionCfg[ConnectionCfgCtrlId].pDcmDslMainConnection->DcmDslProtocolComMChannelId;
            Dcm_MsgCtrl[MsgCtrlIndex].MsgContext.ReqDataLen = Dcm_QueuedRequestCtrl[tempIndex].Dcm_QueuedRequestLength;
            Dcm_MsgCtrl[MsgCtrlIndex].MsgContext.pReqData   = Dcm_RequestQueuedBuffer[tempIndex];
            Dcm_ConnectionCtrl[ConnectionCfgCtrlId].Dcm_ConnectionActive = TRUE;
            Dcm_TpRxIndication(queuedPduRxPduId, E_OK);
            Dcm_QueuedRequestCtrl[tempIndex].Dcm_PendingRequestState = DCM_PENDING_REQUEST_PROCESS;
        }
        else
        {
            SchM_Exit_Dcm_ExclusiveArea();
        }
    }
    else
    {
        SchM_Exit_Dcm_ExclusiveArea();
    }
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

    /*Pending manage*/
    for (iloop = 0; iloop < pDcmDslProtocol->DcmDslProtocolRow_Num; iloop++)
    {
        MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
        if (DCM_INVALID_UINT8 != MsgCtrlId)
        {
            if (DCM_MSG_PROCESSED == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
            {
                if (DslInternal_RxIndicationDeal(iloop) == E_OK)
                {
                    /*when Message is "DCM_MSG_PROCESSED" status,App return E_PENDING"*/
                    (void)DsdInternal_SearchSidTabServiceIndex(
                        Dcm_MsgCtrl[MsgCtrlId].SID,
                        iloop,
                        &SidTabCfgIndex,
                        &SidTabServiceCfgIndex);
                    Dcm_DiagnosticServicex =
                        ((pDcmDsdServiceTable)[SidTabCfgIndex].pDcmDsdService)[SidTabServiceCfgIndex].DcmDsdSidTabFnc;
                    /*Enter the specific service handler*/
                    if (
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
                        (DCM_E_FORCE_RCRRP == Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret) &&
#endif
                        (DCM_FORCE_RCRRP_OK == Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus))
                    {
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = E_NOT_OK;
                    }
                    else if (
                        (DCM_PENDING == Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus)
                        || (DCM_INITIAL == Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus))
                    {
                        /*do nothing, not return.*/
                    }
                    else
                    {
                        /*DCM_CANCEL*/
                        ret = E_NOT_OK;
                    }

                    if (ret == E_OK)
                    {
                        ret = (*Dcm_DiagnosticServicex)(Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus, iloop, &errorCode);
                        if ((ret != E_OK) && (ret != DCM_E_FORCE_RCRRP) && (ret != DCM_E_PENDING))
                        {
                            if (DCM_E_RESERVED == errorCode)
                            {
                                if (ret == E_NOT_OK)
                                {
                                    DslInternal_ResetResource(iloop);
                                }
                                else
                                {
                                    DsdInternal_SetNrc(iloop, DCM_E_CONDITIONSNOTCORRECT);
                                    DsdInternal_ProcessingDone(iloop);
                                }
                            }
                            else
                            {
                                DsdInternal_SetNrc(iloop, errorCode);
                                DsdInternal_ProcessingDone(iloop);
                            }
                        }
                    }
                }
            }
            else if (
                (TRUE == Dcm_MsgCtrl[MsgCtrlId].SendFlag)
                && (DCM_MSG_TRANSMISSION == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
            {
                DslInternal_ProcessingDone(iloop);
            }
            else
            {
                /*Avoid Misra error*/
            }
        }
        Dcm_MainFunction_Pending2A(iloop);
    }
}

/*************************************************************************/
/*
 * Brief               <P2Server Timer background processing function>
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
static FUNC(void, DCM_CODE) Dcm_MainFunction_P2ServerTimer(void)
{
    uint8  iloop;
    uint8  MsgCtrlId;
    uint32 OldTimer;
    uint32 ExpireTimer;
    uint32 Timer;

    for (iloop = 0; iloop < DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->DcmDslProtocolRow_Num; iloop++)
    {
        MsgCtrlId = Dcm_ProtocolCtrl[iloop].MsgCtrlIndex;
        if ((DCM_INVALID_UINT8 != MsgCtrlId) && (DCM_P2TIMER_ON == (Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2State)))
        {
            OldTimer    = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2CurTimer;
            ExpireTimer = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2ExpiredTimer;
            Dcm_GetTimeSpan(OldTimer, &Timer);
            if ((Timer + Dcm_GeneralCfg.DcmTaskTime) >= ExpireTimer)
            {
                /*P2Server Timer timeout*/
                DslInternal_P2ServerTimeout(iloop);
            }
        }
    }
}

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
static FUNC(void, DCM_CODE) Dcm_GetUDSPhyPduID(P2VAR(PduIdType, AUTOMATIC, DCM_VAR) rxPhyPduId)
{
    uint8                         iloop;
    uint8                         index;
    const Dcm_DslProtocolRxType*  pDslProtocolRx;
    const Dcm_DslProtocolType*    pDcmDslProtocol    = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol;
    uint8                         ProtocolRow_Num    = pDcmDslProtocol->DcmDslProtocolRow_Num;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = pDcmDslProtocol->pDcmDslProtocolRow;

    if (NULL_PTR != rxPhyPduId)
    {
        for (iloop = 0u; iloop < ProtocolRow_Num; iloop++)
        {
            if (DCM_UDS_ON_CAN == pDcmDslProtocolRow[iloop].DcmDslProtocolID)
            {
                /** regard a protocol only have a connection here */
                const Dcm_DslMainConnectionType* pDcmDslMainConnection =
                    pDcmDslProtocolRow[iloop].pDcmDslConnection[0].pDcmDslMainConnection;
                uint8 ProtocolRx_Num = pDcmDslMainConnection->DcmDslProtocolRx_Num;
                pDslProtocolRx       = pDcmDslMainConnection->pDcmDslProtocolRx;
                for (index = 0u; index < ProtocolRx_Num; index++)
                {
                    if (DCM_PHYSICAL == pDslProtocolRx[index].DcmDslProtocolRxAddrType)
                    {
                        *rxPhyPduId = pDslProtocolRx[index].DcmDslProtocolRxPduId;
                        break;
                    }
                }
                break;
            }
        }
    }
}

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
static FUNC(void, DCM_CODE) Dcm_BootloaderResponse(void)
{
    PduInfoType    PduBuffer;
    PduInfoType*   PduInfo = &PduBuffer;
    PduIdType      pduRxId = 0;
    PduLengthType  availabeBufferLength;
    uint8          Data[2] = {0};
    Std_ReturnType ret     = E_OK;

    if (Dcm_ReqSetProgConditions == TRUE)
    {
        ret = Dcm_SetProgConditions(DCM_PENDING, &Dcm_ProgConditions);
        if (ret == E_OK)
        {
            /* By this mode switch the DCM informs the BswM to jump to the bootloader.*/
            (void)SchM_Switch_DcmEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
            Dcm_ReqSetProgConditions = FALSE;
        }
        else if (ret == E_NOT_OK)
        {
            Dcm_ReqSetProgConditions = FALSE;
        }
        else
        {
            /* idle */
        }
    }

    if (DCM_WARM_START == Dcm_GetProgConditions(&Dcm_ProgConditions))
    {
        if (Dcm_ProgConditions.ApplUpdated == TRUE)
        {
            gAppl_UpdataOK_ResponseFlag    = TRUE;
            Dcm_ProgConditions.ApplUpdated = FALSE;
            /*receive App updata flag,response 0x51 01*/
#if (DCM_BSWM_ENABLE == STD_ON)
            BswM_Dcm_ApplicationUpdated();
#endif
            Dcm_GetUDSPhyPduID(&pduRxId);
            PduInfo->SduLength = 0;
            (void)Dcm_StartOfReception(pduRxId, PduInfo, 2, &availabeBufferLength);
            Data[0]             = 0x10;
            Data[1]             = 0x83;
            PduInfo->SduDataPtr = Data;
            PduInfo->SduLength  = 2;
            (void)Dcm_CopyRxData(pduRxId, PduInfo, &availabeBufferLength);
            Dcm_TpRxIndication(pduRxId, E_OK);

            PduInfo->SduLength = 0;
            (void)Dcm_StartOfReception(pduRxId, PduInfo, 2, &availabeBufferLength);
            Data[0]             = 0x11;
            Data[1]             = 0x01;
            PduInfo->SduDataPtr = Data;
            PduInfo->SduLength  = 2;
            (void)Dcm_CopyRxData(pduRxId, PduInfo, &availabeBufferLength);
            Dcm_TpRxIndication(pduRxId, E_OK);
        }
    }
}

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
FUNC(void, DCM_CODE) DslInternal_InitPageBuffer(void)
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_PageBufferData.IsRecord            = FALSE;
    Dcm_PageBufferData.IsFilled            = FALSE;
    Dcm_PageBufferData.IsNextFilled        = FALSE;
    Dcm_PageBufferData.BufferOverFlowed    = FALSE;
    Dcm_PageBufferData.IsBufferOccupied    = FALSE;
    Dcm_PageBufferData.PagedBufferStarted  = FALSE;
    Dcm_PageBufferData.PagedBufferCanceled = FALSE;
    Dcm_PageBufferData.IloopOne            = 0u;
    Dcm_PageBufferData.TotalSize           = 0UL;
    Dcm_PageBufferData.ReqOffset           = 0u;
    Dcm_PageBufferData.ResOffset           = 0u;
    Dcm_PageBufferData.FilledSize          = 0u;
    Dcm_PageBufferData.NextFilledSize      = 0u;
    Dcm_PageBufferData.TimerStart          = FALSE;
    Dcm_PageBufferData.TimeOut             = FALSE;
    Dcm_PageBufferData.CurTimer            = 0UL;
    Dcm_PageBufferData.ExpiredTimer        = 0UL;
    Dcm_MemSet(Dcm_PageBufferData.PagedBuffer, 0u, DCM_CHANNEL_LENGTH);
    SchM_Exit_Dcm_ExclusiveArea();
}
#endif /* STD_ON == DCM_PAGEDBUFFER_ENABLED */

#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X34_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X35_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X36_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X38_ENABLED))
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
FUNC(void, DCM_CODE) DslInternal_InitDspProgramInfo(void)
{
    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_DspProgram.address           = 0UL;
    Dcm_DspProgram.blockId           = 0x00;
    Dcm_DspProgram.reqBlockId        = 0x00;
    Dcm_DspProgram.Status            = DCM_UDS0X36_INIT;
    Dcm_DspProgram.MemoryIdInfoIndex = 0xFF;
    Dcm_DspProgram.BlockLength       = 0;
    Dcm_DspProgram.MemorySize        = 0;
    Dcm_DspProgram.FirstFlag         = FALSE;
    Dcm_DspProgram.DcmLastloadSize   = 0;
    SchM_Exit_Dcm_ExclusiveArea();
}
#endif

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
