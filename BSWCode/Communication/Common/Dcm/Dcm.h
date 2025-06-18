
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
**************************************************************************** **
**                                                                           **
**  FILENAME    :  Dcm.h                                                     **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : declaration of Dcm                                         **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef DCM_H
#define DCM_H
/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*
 * <VERSION>  <DATE>       <AUTHOR>           <REVISION LOG>
 *  V1.0.0    2018-3-20    shushi              Initial version
 *  V1.0.1    2019-12-24   tao.yu              QAC check fix
 *  V1.0.2    2020-1-7     tao.yu              Commercial project problem modification
 *  V2.0.0    2021-8-25    tao.yu              release version
 *  V2.0.1    2022-4-1     xinrun.wang         move out timing function as callback
 *  V2.1.0    2022-8-18    tao.yu              change to PBPC mode,Implementation 0x29 service
 *  V2.1.1    2023-7-31    xinrun.wang         fix functional tester present interrupt behavior
 *  V2.1.2    2023-8-08    peng.wu             fix Big Endian and little Endian conversion CPT-6467
 *  V2.1.3    2023-8-10    xue.han             QAC Change
 *  V2.1.3    2023-8-11    Xinrun.Wang         Do not change current reqLen for functional req until tpRxIndication
 *  V2.1.4    2023-8-16    tao.yu              CPT-6531 Fix 0x29 Service Connection Index Values Error
 *  V2.1.5    2023-8-16    xinrun.wang         Handle status for startOfReception with info.SduLength > 0
 *  V2.1.6    2023-8-18    tao.yu              CPT-6579/CPT-6577 Fix 0x29 Service
 *  V2.1.7    2023-8-18    xinrun.wang         1. CPT-6585 Fix macro condition for supplier notification
 *                                             2. Add extra buffer for queued reqeust for switch
 *  V2.1.8    2023-8-21    tao.yu              CPT-6603 Fix 0x29 Service busy
 *  V2.1.9    2023-8-23    peng.wu             CPT-5999 Fix Dcm_UDS0x19_06SubDeal when DtcExtendedDataRecordNumber is
 *                                             0xFE
 *  V2.1.10   2023-8-24    xinrun.wang         check result E_OK for queued request with tpRxIndication
 *  V2.1.11   2023-8-24    peng.wu             add the validation of nrc 7E,33, Update length check in 0x86 service
 *  V2.1.12   2023-8-28    xinrun.wang/tao.yu  Check if functional request processing at startOfReception
 *                                             CPT-6716 Fix 0x29 Service 03 sub service ret init value
 *  V2.1.13   2023-8-31    peng.wu/tao.yu      Fix 0x36 service not responding to NRC73 CPT-6718
 *                                             CPT-6764/CPT-6766 Fix 0x29 Service and other Service subService check
 *  V2.1.14   2023-9-1     xinrun.wang         Check queued request for functional address req
 *  V2.1.15   2023-9-7     tao.yu              CPT-6811/CPT-6839 Fix 0x29 Service
 *                         xue.han             CPT-6812 Fix OBD0x06 Service SupportBuffer param Initial value and
 *                                             OBD_ReadPidValues when OBD0x02 Service is enable
 *                         peng.wu             CPT-6732 Add callout DataFormatIdentifier to services 0x34,0x35.
 *            2023-9-11    tao.yu              fix 0x36 service NRC,Fix 0x29 Service
 *            2023-9-13    tao.yu              Fix 0x29 Service White List
 *            2023-9-18    tao.yu              add new feature SWS_Dcm_01355 SWS_Dcm_01048, remove warning
 *            2023-10-10   xue.han             fix DataSize bit transform
 *            2023-10-24   peng.wu             CPT-7315, Fixed DidReadNvmFlag variable assignment in the 22 service
 *            2023-10-24   peng.wu             CPT-7315, Reset Dcm_0x22DidReadNvmFlag after it is used
 *            2023-11-09   xue.han             CPT-7528, Fixed DcmDsdSubServiceUsed
 *            2023-11-14   xue.han             CPT-7595, avoid Dcm_CheckProtocol to use released protocol
 *            2023-11-24   xue.han             1、CPT-7683, Fix DtcStatus in Service 19 04、19 06
 *                                             2、CPT-7707, Fix macro condition for Dcm_UDS0x2C_Init()
 *            2023-11-28   xue.han             QAC
 *            2023-11-30   xue.han             1、CPT-7747, Fix 0x22 service NRC13
                                               2、CPT-7653, Delete redundant Attempt Counter process
 *            2023-12-06   xue.han             CPT-7779, Fix 0x22 service SessionCheck
 *            2024-1-4     xinrun.wang         CPT-8017, Fix protocol switch, avoid unnecessary ComM notification
 *            2024-1-15    xue.han             Add Dem_DcmGetNumberOfFilteredDTC function call in OBD0x03,0x07,0x0A
 *            2024-01-31   xue.han             CPT-8175,CPT-8239 Fix 0x2F DcmDspDidSignalNum index error and NRC33
                                               CPT-7547,Fix 0x31 ReqLen Type Cast
 *  V2.1.16   2024-03-22   xue.han             clear 0210HuaYangproject compilation warnings
 *            2024-03-25   xue.han             CPT-8405/8406,Add handling of NRC24 and repeat request in service 0x36.
 *                                             CPT-8606/8625,Add handling of NRC71 and address offset in service 0x36.
 *            2024-03-26   tao.yu              fix schm
 *            2024-04-09   xue.han             CPT-8512,Stop 0x2A transfer when DCM_0x2C clear DDDID.
 *            2024-04-11   xinrun.wang         CPT-8728, data consistency check for queued request process
 *            2024-04-25   xue.han             CPT-8405/8625,fix repeat request and NRC71 in service 0x36.
 *                                             CPT-8731/8381,redesigned to get Dcm_FalseAcessCount at initialization.
 *            2024-05-06   xinrun.wang         0x31 variable length check correct for routine with no input
 *                         xue.han             CPT-8512,fix Dcm_UDS0x2A_DDDidStop()
 *            2024-5-29    xinrun.wang         fix AvailableBufferSize calculation for queued request in copyRxData.
 *            2024-05-31   xue.han             change MetaData to global variable to fit DCache
 *            2024-06-13   xue.han             change protocol start/stop position to fit multicore
 *            2024-06-19   xue.han             add some functional macro to reduce amount of code.
 *            2024-06-27   xue.han             change DcmDslProtocolRxTesterSourceAddr to fit Big-Enddian
 *            2024-07-10   xue.han             CPT-9354,change DDDID range,including boundary value
 *            2024-07-16   xue.han             CPT-9742,fix marco DCM_UDS0X31_STARTROUTINE_ENABLED
 *            2024-07-18   xue.han             CPT-8823,fix UDS0x14 pending response
 *            2024-07-24   xinrun.wang         fix functional address handling for queued request
 *            2024-08-02   xue.han             QAC
 *  V2.1.17   2024-08-26   xue.han             CPT-10221,fix Subnet handling in UDS service 0x28
 *                                             CPT-10250,fix Nrc72 in UDS service 0x2E
 *            2024-08-29   xue.han             CPT-10156, fix Dcm_SetProgConditions when programsession
 *                                             CPT-10148, fix Dcm_ActiveNetwork in RX ISR
 *  V2.1.18   2024-10-15   xue.han             CPT-10773,add NRC process if channel is NULL
 *                                             fix unreachable code
 *            2024-10-22   xue.han             CPT-10607,delete AE process
 *            2024-10-31   xue.han             CPT-10991,fix data types associated with DID
 *            2024-11-16   peng.wu             CPT-9403, fix DemFreezeFrameRecordNumber range
 *            2024-11-22   xue.han             QAC
 *            2024-11-23   xinrun.wang         Set Functional message length for all cases in startOfReception
 *            2024-11-23   xue.han             CPT-11373,fix 0x2F totaldatalength check
 *                                             CPT-11435,fix Dcm_GetUDSPhyPduID
 *            2024-11-25   xue.han             CPT-11635,fix getting RxPduId for UDS service 0x86
 *                                             fix Dcm exclusiveArea
 *            2024-12-12   xue.han             CPT-11934,fix DCM_DELAY_COMM_INACTIVE function
 *            2024-12-16   xinrun.wang         CPT-11958,fix 0x31 variable length check
 *            2024-12-17   xue.han             CPT-11943,fix mode switch trigger by received resetType
 *                                             CPT-11059,add handle of extra returnvalue from servicefunc
 *            2024-12-25   xue.han             CPT-11943,fix Dcm_UDS0x2A_SubFunSubDeal for UDS service 0x2A
 *            2025-03-12   xue.han             CPT-11232,fix processing consecutive requests with the same protocol.
 *                                             CPT-13207,fix global variable naming for ProgConditions.
 *                                             CPT-12870,fix Dcm_SetDeauthenticatedRole.
 *            2025-04-22   xue.han             CPT-14039,fix negative response NRC 0x22.
 *            2025-04-29   xue.han             CPT-13251,fix RTE_MODE_DcmDiagnosticSessionControl_DCM_DEFAULT_SESSION.
 *                                             CPT-14082,Add Dcm_AuthenticationRoleType Type
 *                                             CPT-14170,fix whitelist process for UDS service 0x29
 *  V2.1.19   2025-05-07   li.feng             CPT-14228,Fix the bug in the 0x29 authentication process.
 *            2025-05-12   peng.wu             CPT-13897,Fix OBD 04 service respond after clearing the flash
============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_AR_RELEASE_MAJOR_VERSION (0x04u)
#define DCM_AR_RELEASE_MINOR_VERSION (0x02u)
#define DCM_AR_RELEASE_PATCH_VERSION (0x02u)
#define DCM_SW_MAJOR_VERSION         (0x02u) /*Major Version*/
#define DCM_SW_MINOR_VERSION         (0x01u) /*Minor Version*/
#define DCM_SW_PATCH_VERSION         (0x13u) /*Patch version*/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dcm_Types.h"
#if (DCM_DEM_SUPPOTR == STD_ON)
#include "Dem_Types.h"
#endif
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
extern CONST(Dcm_CfgType, DCM_CONST) Dcm_Cfg;
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
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
extern FUNC(void, DCM_CODE) Dcm_Init(P2CONST(Dcm_CfgType, DCM_CONST, DCM_CONST_PBCFG) ConfigPtr);

#if (STD_ON == DCM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to where to store
 * the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DCM_CODE) Dcm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR) VersionInfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_VERSION_INFO_API */

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
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetVin(P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Std_ReturnType, DCM_CODE)
    Dcm_DemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew);
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
/* PRQA S 3432,3451,3449++ */ /* MISRA Rule 20.7,8.5 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevel(P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR) SecLevel);
/* PRQA S 3432,3451,3449-- */ /* MISRA Rule 20.7,8.5 */

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
/* PRQA S 1330,3451,3449++ */ /* MISRA Rule 8.3,8.5 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlType(P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_VAR) SesType);
/* PRQA S 1330,3451,3449++ */ /* MISRA Rule 8.3,8.5 */
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
/* PRQA S 3432,3451,3449++ */ /* MISRA Rule 20.7,8.5 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR) ActiveProtocol);
/* PRQA S 3432,3451,3449-- */ /* MISRA Rule 20.7,8.5 */

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
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
/* PRQA S 3451,3449++ */ /* MISRA Rule 8.5 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(void);
/* PRQA S 3451,3449-- */ /* MISRA Rule 8.5 */
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
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerOnEvent(uint8 RoeEventId);
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
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
/* PRQA S 3451,3449,0624++ */ /* MISRA Rule 20.7,8.5,8.3 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(boolean active);
/* PRQA S 3451,3449,0624-- */ /* MISRA Rule 20.7,8.5,8.3 */
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
/*************************************************************************/
/*
 * Brief               <Dcm_SetDeauthenticatedRole>
 * ServiceId           <0x79>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId,deauthenticatedRole>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_SetDeauthenticatedRole(uint16 connectionId, const Dcm_AuthenticationRoleType deauthenticatedRole);
#endif

#endif /* CANTP_H */
