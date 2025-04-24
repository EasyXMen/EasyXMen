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
**  FILENAME    : Dcm_Ext.h                                                  **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Declaration of Dcm Internal                                **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef DCM_EXT_H_
#define DCM_EXT_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm_Types.h"
#if (DCM_DEM_SUPPOTR == STD_ON)
#include "Dem_Types.h"
#endif
/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
static inline void OBD_BITS_SET(uint8* p, uint8 n)
{
    ((p)[(n) >> 3u] |= (uint8)(1u << ((n) & 7u)));
}

/* Service request packet length defined */
#define DCM_UDS0X01_REQ_DATA_LENGTH_MIN (2U)
/*0x01 Service request packet min length defined*/
#define DCM_UDS0X01_REQ_DATA_LENGTH_MAX (7U)
/*0x01 Service request packet max length defined*/
#define DCM_UDS0X02_REQ_DATA_LENGTH_MIN (3U)
/*0x02 Service request packet min length defined*/
#define DCM_UDS0X02_REQ_DATA_LENGTH_MAX (7U)
/*0x02 Service request packet max length defined*/
#define DCM_UDS0X03_REQ_DATA_LENGTH (1U)
/*0x03 Service request packet length defined*/
#define DCM_UDS0X04_REQ_DATA_LENGTH (1U)
/*0x04 Service request packet length defined*/
#define DCM_UDS0X06_REQ_DATA_LENGTH_MIN (2U)
/*0x06 Service request packet min length defined*/
#define DCM_UDS0X06_REQ_DATA_LENGTH_MAX (7U)
/*0x06 Service request packet max length defined*/
#define DCM_UDS0X07_REQ_DATA_LENGTH (1U)
/*0x07 Service request packet length defined*/
#define DCM_UDS0X08_REQ_DATA_LENGTH_MIN (2U)
/*0x08 Service request packet min length defined*/
#define DCM_UDS0X08_REQ_DATA_LENGTH_MAX (7U)
/*0x08 Service request packet max length defined*/
#define DCM_UDS0X09_REQ_DATA_LENGTH_MIN (2U)
/*0x09 Service request packet min length defined*/
#define DCM_UDS0X09_REQ_DATA_LENGTH_MAX (7U)
/*0x09 Service request packet max length defined*/
#define DCM_UDS0X0A_REQ_DATA_LENGTH (1U)
/*0x0A Service request packet length defined*/

/*SID Table*/
#define SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA     0x01u
#define SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA           0x02u
#define SID_REQUEST_EMISSION_RELATED_DTC                   0x03u
#define SID_CLEAR_EMISSION_RELATED_DIAGNOSTIC_INFORMATION  0x04u
#define SID_REQUEST_OXYGEN_SENSOR_MONITORING_TEST_RESULTS  0x05u
#define SID_REQUEST_OBD_MONITORING_TEST_RESULTS            0x06u
#define SID_REQUEST_EMISSION_RELATED_DTC_DURING_CURRENT_DC 0x07u /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#define SID_REQUEST_CONTROL                                0x08u
#define SID_REQUEST_VEHICLE_INFORMATION                    0x09u
#define SID_REQUEST_PERMANENT_EMISSION_RELATED_DTC         0x0Au

/**************************************************************************
 **********************Service request packet length defined***************
 **************************************************************************/
/* have subservice Service request packet length defined*/
#define DCM_UDS_REQ_DATA_MINLENGTH  (2U)

#define DCM_UDS0X10_REQ_DATA_LENGTH (2U)
/*0x10 Service request packet length defined*/
#define DCM_UDS0X11_REQ_DATA_LENGTH (2U)
/*0x11 Service request packet length defined*/
#define DCM_UDS0X14_REQ_DATA_LENGTH (4U)
/*0x14 Service request packet length defined*/
#define DCM_UDS0X19_REQ_DATA_MINLENGTH (2U)
/*0x19 Service request packet length defined*/
#define DCM_UDS0X19_SUBFUNC0X01_REQ_DATA_LENGTH (3U)
/*sub-function 0x01 request message length*/
#define DCM_UDS0X19_SUBFUNC0X02_REQ_DATA_LENGTH (3U)
/*sub-function 0x02 request message length*/
#define DCM_UDS0X19_SUBFUNC0X03_REQ_DATA_LENGTH (2U)
/*sub-function 0x03 request message length*/
#define DCM_UDS0X19_SUBFUNC0X04_REQ_DATA_LENGTH (6U)
/*sub-function 0x04 request message length*/
#define DCM_UDS0X19_SUBFUNC0X06_REQ_DATA_LENGTH (6U)
/*sub-function 0x06 request message length*/
#define DCM_UDS0X19_SUBFUNC0X0A_REQ_DATA_LENGTH (2U)
/*sub-function 0x0A request message length*/
#define DCM_UDS0X19_SUBFUNC0X0E_REQ_DATA_LENGTH (2U)
/*sub-function 0x0A request message length*/
#define DCM_UDS0X22_REQ_DATA_MINLENGTH (3U)
/*0x22 Service request packet length defined*/
#define DCM_UDS0X23_REQ_DATA_MINLENGTH (4u)
/*0x23 Service request packet length defined*/
#define DCM_UDS0X24_REQ_DATA_LENGTH (3u)
/*0x24 Service request packet length defined*/
#define DCM_UDS0X27_REQ_DATA_MINLENGTH (2U)
/*0x27 Service request packet length defined*/
#define DCM_UDS0X28_REQ_DATA_MINLENGTH (3U)
/*0x28 request message length*/
#define DCM_UDS0X28_REQ_DATA_MAXLENGTH (5U)
/*0x28 request message length*/
#define DCM_UDS0X29_REQ_DATA_MINLENGTH (2U)
/*0x29 request message length*/
#define DCM_UDS0X2A_SUBFUNC0X04_REQ_DATA_MINLENGTH (2U)
/*0x2A 04 Service request packet length defined*/
#define DCM_UDS0X2A_REQ_DATA_MINLENGTH (3U)
/*0x2A other Service request packet length defined*/
#define DCM_UDS0X2C_REQ_DATA_MINLENGTH (2U)
/*0x2C Service request packet length defined*/
#define DCM_UDS0X2C_SUBFUNC0X01_REQ_DATA_MINLENGTH (8U)
/*0x2C Service request packet length defined*/
#define DCM_UDS0X2C_SUBFUNC0X02_REQ_DATA_MINLENGTH (7U)
/*0x2C Service request packet length defined*/
#define DCM_UDS0X2C_SUBFUNC0X03_REQ_DATA_MINLENGTH (4U)
/*0x2C Service request packet length defined*/
#define DCM_UDS0X2E_REQ_DATA_MINLENGTH (4U)
/*0x2E Service request packet length defined*/
#define DCM_UDS0X2F_REQ_DATA_MINLENGTH (4U)
/*0x2F Service request packet length defined*/
#define DCM_UDS0X31_REQ_DATA_MINLENGTH (4U)
/*0x31 Service request packet length defined*/
#define DCM_UDS0X34_REQ_DATA_MINLENGTH (5u)
/*0x34 Service request packet length defined*/
#define DCM_UDS0X35_REQ_DATA_MINLENGTH (5u)
/*0x35 Service request packet length defined*/
#define DCM_UDS0X36_REQ_DATA_UPLOAD_MINLENGTH (2u)
/*0x36 Service request upload packet length defined*/
#define DCM_UDS0X36_REQ_DATA_DOWNLOAD_MINLENGTH (3u)
/*0x36 Service request download packet length defined*/
#define DCM_UDS0X38_REQ_DATA_MINLENGTH (5u)
/*0x38 Service request packet length defined*/
#define DCM_UDS0X3D_REQ_DATA_MINLENGTH (5u)
/*0x3D Service request packet length defined*/
#define DCM_UDS0X3E_REQ_DATA_LENGTH (2U)
/*0x3E Service request packet length defined*/
#define DCM_UDS0X85_REQ_DATA_MINLENGTH (2U)
/*0x85 request message minimum length*/
#define DCM_UDS0X85_REQ_DATA_DTC_MINLENGTH (5U)
/*0x85 request message minimum length with DTC*/
#define DCM_UDS0X86_REQ_DATA_MINLENGTH (3U)
/*0x86 request message minimum length*/
/**********************************************************************
 *             UDS 0x19 service sub-function
 **********************************************************************/
/*UDS 0x19 service,sub-function defined*/
#define DCM_REPORTNUMBEROFDTCBYSTATUSMASK (0x01u)
/*reportNumberOfDTCByStatusMask */
#define DCM_REPORTDTCBYSTATUSMASK (0x02u)
/*reportDTCByStatusMask*/
#define DCM_REPORTREPORTDTCSNAPSHOTIDENTIFICATION (0x03u)
/*reportDTCSnapshotIdentification*/
#define DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER (0x04u)
/*reportDTCSnapshotRecordByDTCNumber*/
#define DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER (0x06u)
/*reportDTCExtendedDataRecordByDTCNumber */
#define DCM_REPORTSUPPORTEDDTC (0x0Au)
/*reportSupportedDTC*/
#define DCM_REPORTMOSTRECENTCONFIRMEDDTC (0x0Eu)
/*reportMostRecentConfirmedDTC*/
/***********************************************************************
              UDS 0x2F service InputOutputControlParameter define
 ***********************************************************************/
#define DCM_UDS0X2F_RETURNCONTROLTOECU  (0u) /*ReturnControlToEcu*/
#define DCM_UDS0X2F_RESETTODEFAULT      (1u) /*ResetToDefault*/
#define DCM_UDS0X2F_FREEZECURRENTSTATE  (2u) /*FreezeCurrentState*/
#define DCM_UDS0X2F_SHORTTERMADJUSTMENT (3u) /*ShortTermAdjustment*/

/**********************************************************************
 *            UDS 0x2C service sub-function define
 **********************************************************************/
#define DCM_UDS0X2C_01_DDBYDID    (1u) /*defineByIdentifier*/
#define DCM_UDS0X2C_02_DDBYMEMORY (2u) /*defineByMemoryAddress*/
#define DCM_UDS0X2C_03_CLEARDDDID (3u)
/*clearDynamicallyDefinedDataIdentifier*/

/**********************************************************************
 *            UDS 0x2A service sub-function define
 **********************************************************************/
#define DCM_UDS0X2A_01_SLOW   (1u) /*sendAtSlowRate*/
#define DCM_UDS0X2A_02_MEDIUM (2u) /*sendAtMediumRate*/
#define DCM_UDS0X2A_03_FAST   (3u) /*sendAtFastRate*/
#define DCM_UDS0X2A_04_STOP   (4u) /*stopSending*/

/**********************************************************************
 *            UDS 0x31 service sub-function define
 **********************************************************************/
#define DCM_UDS0X31_STARTROUTINE (1u)          /*startRoutine*/
#define DCM_UDS0X31_STOPROUTINE  (2u)          /*stopRoutine*/
/* PRQA S 0791++*/                             /* MISRA Rule 5.4 */
#define DCM_UDS0X31_REQUESTROUTINERESULTS (3u) /*RequestRoutineResult*/
/* PRQA S 0791--*/                             /* MISRA Rule 5.4 */
/**********************************************************************
 *            UDS 0x3E service sub-function define
 **********************************************************************/
#define DCM_UDS0X3E_ZERO_SUBFUNCTION (0u) /*zero sub-function*/

/**********************************************************************
 *            UDS 0x85 service sub-function define
 **********************************************************************/
#define DCM_UDS0X85_ON  (1u) /*turn on the setting of DTC*/
#define DCM_UDS0X85_OFF (2u) /*turn off the setting of DTC*/

/**********************************************************************
 *            UDS 0x86 service sub-function define
 **********************************************************************/
#define DCM_UDS0X86_STOP_RESPONSE_ON_EVENT       (0u) /*stopResponseOnEvent*/
#define DCM_UDS0X86_ON_DTC_STATUS_CHANGE         (1u) /*onDTCStatusChange*/
#define DCM_UDS0X86_ON_CHANGE_OF_DATA_IDENTIFIER (3u) /*onChangeOfDataIdentifier*/
#define DCM_UDS0X86_REPORT_ACTIVATED_EVENTS      (4u) /*reportActivatedEvents*/
#define DCM_UDS0X86_START_RESPONSE_ON_EVENT      (5u) /*startResponseOnEvent*/
#define DCM_UDS0X86_CLEAR_RESPONSE_ON_EVENT      (6u) /*clearResponseOnEvent*/

/**********************************************************************
 *            UDS 0x28 service sub-function define
 **********************************************************************/
#define COMTYPE_NORM        (1u) /*normal communication message*/
#define COMTYPE_NORM_AND_NM (3u) /*NORM and NM communication message*/
/******************************END of dependence****************************************/

#if (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED)
typedef struct
{
    uint16 DDDid;
    uint8 DDDIDNumOfElements; /* DcmDspDDDIDMaxElements range：1-255 */
    Dcm_DDDidElementsDataTypes DcmDspAlternativeArgumentData[DCM_DSP_DDDID_ELEMENTS_MAX];
} Dcm_DDDidTypes;

extern VAR(Dcm_DDDidTypes, DCM_VAR_NOINIT) Dcm_DDDid[DCM_DSP_DDDID_MAX_NUMBER];
#endif

/*SID Table*/
#define SID_DIAGNOSTIC_SESSION_CONTROL         0x10u
#define SID_ECU_RESET                          0x11u
#define SID_CLEAR_DIAGNOSTIC_INFORMATION       0x14u
#define SID_READ_DTC_INFORMATION               0x19u
#define SID_READ_DATA_BY_IDENTIFIER            0x22u
#define SID_READ_DATA_BY_MEMORYADDRESS         0x23u
#define SID_READ_SCALING_DATA_BY_IDENTIFIER    0x24u
#define SID_SECURITY_ACCESS                    0x27u
#define SID_COMMUNICATION_CONTROL              0x28u
#define SID_AUTHENTICATION                     0x29u
#define SID_READ_DATA_BY_PERIODIC_IDENTIFER    0x2Au
#define SID_DYNAMICALLY_DEFINE_DATA_IDENTIFER  0x2Cu
#define SID_WRITE_DATA_BY_IDENTIFIER           0x2Eu
#define SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER 0x2Fu
#define SID_ROUTINE_CONTROL                    0x31u
#define SID_REQUEST_DOWNLOAD                   0x34u
#define SID_REQUEST_UPLOAD                     0x35u
#define SID_TRANSFER_DATA                      0x36u
#define SID_REQUEST_TRANSFER_EXIT              0x37u
#define SID_REQUEST_FILE_TRANSFER              0x38u
#define SID_WIRTE_DATA_BY_MEMORYADDRESS        0x3Du
#define SID_TESTER_PRESENT                     0x3Eu
#define SID_CONTROL_DTC_SETTING                0x85u
#define SID_RESPONSE_ON_EVENT                  0x86u
/*END OF SID Table*/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
extern VAR(Dcm_VINType, DCM_VAR_POWER_ON_INIT) Dcm_VIN;

#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
extern VAR(Dcm_RoutineControlStateType, DCM_VAR_NOINIT) Dcm_RoutineControlState[DCM_DSP_ROUTINE_MAX_NUM];
#endif

extern VAR(Dcm_CommCtrlType, DCM_VAR_NOINIT) Dcm_CommCtrl[DCM_MAINCONNECTION_NUM];
/*Diagnostic Communication Control*/

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
extern VAR(Dcm_SecCtrlType, DCM_VAR_NOINIT) Dcm_SecCtrl; /*Security level management control block*/
#endif                                                   /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
extern VAR(Dcm_SesCtrlCBType, DCM_VAR_NOINIT) Dcm_SesCtrl; /*Session management control block*/
#endif                                                     /* STD_ON == DCM_SESSION_FUNC_ENABLED */

extern VAR(Dcm_ProtocolCtrlType, DCM_VAR_NOINIT) Dcm_ProtocolCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];
/*Protocol control block*/

extern VAR(Dcm_ChannelCtrlType, DCM_VAR_NOINIT) Dcm_ChannelCtrl[DCM_CHANNEL_NUM];
/*Channel control block, one channel mapping one connection*/

extern VAR(Dcm_MsgCtrlType, DCM_VAR_NOINIT) Dcm_MsgCtrl[DCM_DSLPROTOCOLROW_NUM_MAX];
/*Message control block*/

extern VAR(Dcm_ConnectionCtrlType, DCM_VAR_NOINIT) Dcm_ConnectionCtrl[DCM_CONNECTION_NUM];

extern VAR(uint8, DCM_VAR_POWER_ON_INIT) Dcm_Channel[DCM_CHANNEL_LENGTH];

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
extern uint8 Dcm_QueuedIndex;
extern VAR(uint8, DCM_VAR_POWER_ON_INIT) Dcm_RequestQueuedBuffer[2u][DCM_REQUEST_QUEUED_BUFFER_SIZE];
extern Dcm_QueuedRequestCtrlType Dcm_QueuedRequestCtrl[2u];
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

extern VAR(Dcm_FunctionMessageType, DCM_VAR_NOINIT) Dcm_FunctionalMessage;
/*Function addressing packets resources,power on init*/

extern VAR(Dcm_OBDMessageType, DCM_VAR_NOINIT) Dcm_OBDMessage;

#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
extern VAR(SchedulerQueueTransmitTypes, DCM_VAR_NOINIT) Scheduler_0x2A_Transmit[DCM_PERIODICCONNECTION_NUM];
#endif

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

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
extern FUNC(void, DCM_CODE) Dcm_InterInit(void);

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
extern FUNC(void, DCM_CODE)
    Dcm_InterDemTriggerOnDTCStatus(uint32 DTC, Dem_UdsStatusByteType DTCStatusOld, Dem_UdsStatusByteType DTCStatusNew);
#endif /*  DCM_DEM_SUPPOTR == STD_ON*/
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
extern FUNC(void, DCM_CODE) Dcm_InterMainFunction(void);

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
extern FUNC(void, DCM_CODE) Dcm_MainFunction_Pending2A(uint8 ProtocolCtrlId);

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
/*************************************/
extern FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x27(uint8 ProtocolCtrlId);

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
extern FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x10(uint8 ProtocolCtrlId);

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
/* PRQA S 0777,0779++ */ /* MISRA Rule 5.1,5.2 */
extern FUNC(void, DCM_CODE) DspInternal_DcmConfirmation_0x11(uint8 ProtocolCtrlId);
/* PRQA S 0777,0779-- */ /* MISRA Rule 5.1,5.2 */

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_ManufacturerIndication(uint8 ProtocolCtrlId);

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
extern FUNC(void, DCM_CODE) DsdInternal_ManufacturerConfirmation(uint8 ProtocolCtrlId);

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SupplierIndication(uint8 ProtocolCtrlId);

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
extern FUNC(void, DCM_CODE) DsdInternal_SupplierConfirmation(uint8 ProtocolCtrlId);

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SesCheck(uint16 SidTabIndex, uint16 SidTabServieCfgIndex);

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SubSesCheck(
    uint8 ProtocolCtrlId,
    uint8 Sid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DsdInternal_DidSessionCheck(uint16 DidInfoCfgIndex, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
#endif              /* STD_ON == DCM_SESSION_FUNC_ENABLED */

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SecurityCheck(uint16 SidTabIndex, uint16 SidTabServieCfgIndex);

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
extern FUNC(Std_ReturnType, DCM_CODE) DsdInternal_SubSecurityCheck(
    uint8 ProtocolCtrlId,
    uint8 Sid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DsdInternal_DidSecurityCheck(uint16 DidInfoCfgIndex, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
#endif              /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

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
extern FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DsdInternal_DDDIDcheckPerSourceDID(uint16 DDDidIdx, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */

/*************************************************************************/
/*
 * Brief               <Dcm_GetMainConnectionNum>
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
static inline FUNC(uint8, DCM_CODE) Dcm_GetMainConnectionNum(void)
{
    return DCM_MAINCONNECTION_NUM;
}

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
extern FUNC(void, DCM_CODE) DslInternal_SetSecurityLevel(Dcm_SecLevelType NewSec);

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
extern FUNC(void, DCM_CODE) DslInternal_SetSecurityAccessStatus(Dcm_SecServiceStateType Status);

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
extern FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DslInternal_GetSecurityCfgBySecLevel(Dcm_SecLevelType Dcm_SecLevel, P2VAR(uint8, AUTOMATIC, DCM_VAR) SecCfgIndex);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
/*******************************************************************************
 **************Session Management sub-function function declaration*************
 *******************************************************************************/
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
extern FUNC(void, DCM_CODE) DslInternal_S3ServerStart(uint8 connectionId);

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
extern FUNC(void, DCM_CODE) DslInternal_S3ServerStop(void);

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
extern FUNC(void, DCM_CODE) DslInternal_SesRefresh(Dcm_SesCtrlType NewSes);
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
extern FUNC(void, DCM_CODE) DslInternal_InitConnectionCtrl(uint8 ConnectionCtrlId);

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
extern FUNC(void, DCM_CODE) DslInternal_InitProtocolCtrl(uint8 ProtocolCtrlId);

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
extern FUNC(void, DCM_CODE) DslInternal_ProtocolStartSession(void);

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
extern FUNC(uint8, DCM_CODE) Dcm_GetChannelNum(void);

/*************************************************************************/
/*
 * Brief               <Dcm_GetProtocolMsgNum>
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
extern FUNC(uint8, DCM_CODE) Dcm_GetProtocolMsgNum(void);

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
extern FUNC(void, DCM_CODE) Dcm_RestartS3timer(uint8 connectionId);

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
extern FUNC(void, DCM_CODE) Dcm_StopS3timer(void);

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
extern FUNC(uint32, DCM_CODE) Dcm_StartOfReception_GetLen(uint8 ServiceType, uint8 RxChannelCfgCtrlId);

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
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_InterStartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData_SessionDeal(
    P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
    uint8 ServiceType,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Sid,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) SubFunction,
    uint8 ProtocolCfgId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_TpRxIndication_3E80(uint8 connectionId);

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
extern FUNC(void, DCM_CODE) Dcm_TpRxIndication_SetTxPduId(
    uint8 MsgCtrlIndexx,
    P2CONST(Dcm_DslConnectionType, AUTOMATIC, DCM_CONST) pDslConnection);

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
extern FUNC(boolean, DCM_CODE) Dcm_CheckUDS0x86_TxConfirmation(uint8 ProtocolCfgCtrlId);

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
extern FUNC(void, DCM_CODE) Dcm_TxConfirmation_2A(PduIdType DcmTxPduId);

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
FUNC(BufReq_ReturnType, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DslInternal_ResponseOnOneEvent(P2VAR(PduInfoType, AUTOMATIC, DCM_VAR) PduInfo, PduIdType DcmRxPduId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
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
FUNC(uint8, DCM_CODE) Dcm_GetConnectionNum(void);

/*******************************************************************************
**                      Global Function declaration                           **
*******************************************************************************/
#if (STD_ON == DCM_OBD_FUNC_ENABLED)

#if (STD_ON == DCM_OBD_SERVICE0X01_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x01): Request current powertrain diagnostic data>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x01(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X01_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X02_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x02):  Request Power Train FreezeFrame Data>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x02(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X02_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X03_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x03):  Request emission-related diagnostic trouble codes>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x03(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X03_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X04_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x04):  Clear/reset emission-related diagnostic information>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x04(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X04_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X05_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x05):  Request oxygen sensor monitoring test results>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern Std_ReturnType Dcm_OBD0x05(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif

#if (STD_ON == DCM_OBD_SERVICE0X06_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x06): Request On-Board Monitoring Test-results
 *                                           for Specific Monitored Systems>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x06(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X06_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X07_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x07):  Request emission-related diagnostic trouble
 *                          codes detected during current or last completed driving cycle>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x07(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X07_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x08): Request Control of On-Board System, Test or Component>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x08(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X08_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X09_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x09): Request Vehicle Information>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x09(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X09_ENABLED*/

#if (STD_ON == DCM_OBD_SERVICE0X0A_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x0A):   Request emission-related diagnostic
 *                                              trouble codes with permanent status>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x0A(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*STD_ON == DCM_OBD_SERVICE0X0A_ENABLED*/
#endif              /*STD_ON == DCM_OBD_FUNC_ENABLED*/

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
OBD_ReadPidValues(const OBD_ReadPidValuesTypes* OBD_ReadPid, uint32* pBufOffset, uint8* pNoFindPidNum);

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
OBD_CheckSupportedPIDs(uint8 pid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf, uint8 SID);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif
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
FUNC(void, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
OBD_CheckSupportedTIDs(uint8 Tid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (DCM_DSP_REQUESTCONTROL_FUNC_ENABLED == STD_ON)
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
OBD_FindCfgIndexOfTestID(uint8 testID, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
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
OBD_SetAvailabilityInfoTypeValue(uint8 InfoType, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
FUNC(Std_ReturnType, DCM_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
OBD_FindCfgIndexOfVehInfo(uint8 vehInfo, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

/*************************************************************************/
/*
 * Brief               <Dcm_FillTo4bytes>
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
extern FUNC(void, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dcm_FillTo4bytes(P2VAR(uint8, AUTOMATIC, DCM_VAR) pDataBuf, P2CONST(uint32, AUTOMATIC, DCM_CONST) pData);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (STD_ON == DCM_UDS_FUNC_ENABLED)
/****************************** declarations *********************************/
#if (STD_ON == DCM_UDS_SERVICE0X10_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x10(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X10_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X11_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x11(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X11_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X27_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x27(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X27_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X28_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x28(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

extern FUNC(void, DCM_CODE) DspInternalUDS0x28_CheckNewSes(Dcm_SesCtrlType NewSes);
#endif /* STD_ON == DCM_UDS_SERVICE0X28_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

extern FUNC(void, DCM_CODE) Dcm_AuthenticateInit(void);

extern FUNC(void, DCM_CODE) Dcm_SetDeAuthenticate(uint8 connectionId);

extern FUNC(Std_ReturnType, DCM_CODE)
    DspInternal_RoleCheck(uint8 ProtocolCtrlId, P2CONST(uint8, AUTOMATIC, DCM_CONST) RoleRef, uint8 RoleNum);

extern FUNC(Std_ReturnType, DCM_CODE) DspInternal_WhiteListServiceCheck(uint8 ProtocolCtrlId);

extern FUNC(void, DCM_CODE) DspInternal_SetAuthenticationCtrlOn(uint8 connectionId);

extern FUNC(void, DCM_CODE) DspInternal_SetAuthenticationCtrlOff(uint8 connectionId);

extern FUNC(void, DCM_CODE) Dcm_MainFunction_AuthenticationTimer(void);

extern FUNC(uint8, DCM_CODE) Dcm_GetconnectionIndex(uint8 connectionId);

extern FUNC(Std_ReturnType, DCM_CODE) DspInternal_SubServiceAuthenticationCheck(
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

extern FUNC(Std_ReturnType, DCM_CODE)
    DsdInternal_AuthenticationCheck(uint8 ProtocolCtrlId, uint16 SidTabIndex, uint16 SidTabServieCfgIndex);

#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X22_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X2E_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED))
extern FUNC(Std_ReturnType, DCM_CODE) DspInternal_DidAuthenticationCheck(
    uint8 ProtocolCtrlId,
    uint16 RecDid,
    uint16 DidInfoCfgIdx,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /* STD_ON == DCM_UDS_SERVICE0X22_ENABLED  \
        || STD_ON == DCM_UDS_SERVICE0X2A_ENABLED \
        || STD_ON == DCM_UDS_SERVICE0X2E_ENABLED \
        || STD_ON == DCM_UDS_SERVICE0X2F_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) DspInternal_RidAuthenticationCheck(
    uint8 ProtocolCtrlId,
    uint8 RoutineCfgIdx,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /* STD_ON == DCM_UDS_SERVICE0X31_ENABLED */
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X3E_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x3E(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X3E_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X85_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x85(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X85_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
extern FUNC(void, DCM_CODE) Dcm_UDS0x86_DTCStatusChange(uint8 DTCStatusOld, uint8 DTCStatusNew);

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x86_ChangeOfDataIdentifier(uint8 RoeEventId);

extern FUNC(void, DCM_CODE) Dcm_UDS0x86_CheckTimer(void);

extern FUNC(void, DCM_CODE) Dcm_UDS0x86_SesTranslation(Dcm_SesCtrlType NewSes);

extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_UDS0x86_CheckProcessRoe(uint8 ProtocolCtrlId);

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(boolean, DCM_CODE) Dcm_UDS0x86_GetTxPduId(P2VAR(PduIdType, AUTOMATIC, DCM_VAR) pduTxId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

extern FUNC(boolean, DCM_CODE) Dcm_UDS0x86_TxConfirmation(uint8 ProtocolCtrlId);

FUNC(void, DCM_CODE) Dcm_UDS0x86_Init(void);

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x86(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X86_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X87_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x87(uint8 ProtocolCtrlId);
#endif /* STD_ON == DCM_UDS_SERVICE0X87_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X14_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x14(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X14_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x19(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X19_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X22_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x22(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X22_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X23_ENABLED)
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x23(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X23_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X24_ENABLED)
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x24(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X24_ENABLED */

extern FUNC(void, DCM_CODE) Dcm_MainFunction_Scheduler_0x2A(uint8 ProtocolCtrlId);

#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2A(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

extern FUNC(void, DCM_CODE) Dcm_Scheduler_0x2A_Init(void);

extern FUNC(void, DCM_CODE) Dcm_UDS0x2ACheckNewSession(Dcm_SesCtrlType NewSes);

extern FUNC(void, DCM_CODE) Dcm_UDS0x2ACheckNewSecurity(void);

#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
extern FUNC(void, DCM_CODE) Dcm_UDS0x2A_DDDidStop(uint16 iloop);
#endif /*DCM_DSP_DDDID_MAX_NUMBER > 0u  */

#endif /* STD_ON == DCM_UDS_SERVICE0X2A_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2C(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (DCM_DDDID_STORAGE == STD_ON)
extern FUNC(void, DCM_CODE) Dcm_UDS0x2C_Init(void);
#endif /* DCM_DDDID_STORAGE == STD_ON */
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X2E_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2E(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X2E_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2F(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (DCM_DSP_DID_FOR_2F_NUM > 0)
extern FUNC(void, DCM_CODE) Dcm_UDS0x2FCheckNewSession(Dcm_SesCtrlType NewSes);

extern FUNC(void, DCM_CODE) Dcm_UDS0x2FCheckNewSecurity(Dcm_SecLevelType NewSec);
#endif /* DCM_DSP_DID_FOR_2F_NUM > 0 */
#endif /* STD_ON == DCM_UDS_SERVICE0X2F_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x31(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X34_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x34(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X35_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x35(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X36_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x36(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X37_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x37(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X38_ENABLED)
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x38(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (STD_ON == DCM_UDS_SERVICE0X3D_ENABLED)
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x3D(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#endif /*end of 'STD_ON == DCM_UDS_FUNC_ENABLED'*/

#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X23_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X36_ENABLED))
extern Dcm_ReturnReadMemoryType Dcm_ReadMemory(
    Dcm_OpStatusType OpStatus,
    uint8 MemoryIdentifier,
    uint32 MemoryAddress,
    uint32 MemorySize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if ((STD_ON == DCM_UDS_SERVICE0X3D_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X36_ENABLED))
extern Dcm_ReturnWriteMemoryType Dcm_WriteMemory(
    Dcm_OpStatusType OpStatus,
    uint8 MemoryIdentifier,
    uint32 MemoryAddress,
    uint32 MemorySize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (DCM_UDS_SERVICE0X34_ENABLED == STD_ON)
/*for 0x34 service to request download*/
extern Std_ReturnType Dcm_ProcessRequestDownload(
    Dcm_OpStatusType OpStatus,
    uint8 DataFormatIdentifier,
    uint32 MemoryAddress,
    uint32 MemorySize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) BlockLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (DCM_UDS_SERVICE0X35_ENABLED == STD_ON)
/*for 0x35 service to request upload*/
extern Std_ReturnType Dcm_ProcessRequestUpload(
    Dcm_OpStatusType OpStatus,
    uint8 DataFormatIdentifier,
    uint32 MemoryAddress,
    uint32 MemorySize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) BlockLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if ((DCM_UDS_SERVICE0X35_ENABLED == STD_ON) || (DCM_UDS_SERVICE0X34_ENABLED == STD_ON))
/* service to check DataFormatIdentifier, send NRC31 if return E_NOT_OK*/
extern Std_ReturnType Dcm_DataFormatIdentifierCheck(uint8 DataFormatIdentifier);
#endif

#if (DCM_UDS_SERVICE0X37_ENABLED == STD_ON)
/*for 0x37 service to request transfer exit*/
extern Std_ReturnType Dcm_ProcessRequestTransferExit(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) transferRequestParameterRecord,
    uint32 transferRequestParameterRecordSize,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) transferResponseParameterRecord,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) transferResponseParameterRecordSize,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (DCM_UDS_SERVICE0X38_ENABLED == STD_ON)
/*for 0x38 service to request file transfer*/
extern Std_ReturnType Dcm_ProcessRequestFileTransfer(
    Dcm_OpStatusType OpStatus,
    uint8 modeofOperation,
    uint16 fileSizeParameterLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) filePathAndName,
    uint8 dataFormatIdentifier,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) fileSizeUncompressedOrDirInfoLength,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) fileSizeCompressed,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) BlockLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

#if (DCM_BSWM_ENABLE != STD_ON)
extern void Rte_DcmControlCommunicationMode(
    NetworkHandleType DcmDspComMChannelId,
    Dcm_CommunicationModeType RequestedMode);
#endif

#if ((STD_ON == DCM_UDS_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X85_ENABLED))
/*The update of the DTC status will be re-enabled*/
extern void Rte_EnableAllDtcsRecord(void);
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
extern FUNC(uint8, DCM_CODE) Dcm_GetRxIdNum(void);

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
extern FUNC(uint8, DCM_CODE) Dcm_GetTxIdNum(void);

#endif /* DCM_EXT_H_ */
