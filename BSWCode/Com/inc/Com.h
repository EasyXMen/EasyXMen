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
 **  @file               : Com.h
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of Com
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.0.0    20200706  zhengfei.li   Initial version
 *                                    (Upgrade according to the R19_11 standards)
 *  V2.0.1    20211108  zhengfei.li   Optimize enumeration element names, macro names
 *  V2.1.0    20211217  zhengfei.li   PB/PC configuration parameter split
 *  V2.1.1    20230111  xiaojian.liang Multicore support
 *  V2.1.2    20231009  fupeng.yu     1.support ComRxDataTimeoutAction seted to SUBSTITUTE
 *                                    2.add IPDU based Reception Deadline Monitoring
 *                                    3.add MDT handling in com_txconfirmation
 *  V2.1.3    20240122  shengnan.sun  1.Code optimization.
 *                                    2.Reference library function from IStdLib.h.
 *  V2.1.4    20240329  shengnan.sun  Modify Com_SetTxSignalBuff and Com_GetTxSignalValue.
 *  V2.1.5    20240415  shengnan.sun  1.Resolve RxGroupSignal gateway error.
 *                                    2.ComDataInvalidAction is NOTIFY, do not repalce InitValue.
 *  V2.1.6    20240422  shengnan.sun  RxTimeout handle with update bit disable.
 *  V2.1.7    20240423  shengnan.sun  Resolve TxModeRepetitionPeriod is error after the
 *                                    ComMinimumDelayTime timer timeout, when ComTxModeMode is DIRECT or MIXED.
 *  V2.1.8    20240425  shengnan.sun  Modify Com_StartOfReception to copy data.
 *  V2.1.9    20240524  shengnan.sun  Add multiCore code.
 *  V2.1.10   20240614  shengnan.sun  Modify the compilation condition of Com_RxSignalBuffHandle.
 *  V2.1.11   20240703  shengnan.sun  Add the COM_MULTIPLE_PARTITION_USED judgment to reference os.h file.
 *  V2.1.12   20240723  shengnan.sun  Compatible with 16-bit addressable CPU.
 *  V2.1.13   20240815  shengnan.sun  Generated code optimization.
 *  v2.2.0    20241105  shengnan.sun  R23-11 development first release.
 *  V2.2.1    20250418  tong.zhao     Modify the logic of Com_TriggerTransmitHandle about calloutfunction.
 *  V2.2.2    20250414  tong.zhao     Fix for the issues of the Com multi-core and update-bit signal gateway.
 *  V2.2.3    20250428  tong.zhao     Fix bug the build error in Com_GwNotArrayTxSignalHandle and generate code
 *  V2.3.0    20250429  rongbo.hu     Code optimization.
 *  V2.3.1    20250528  tong.zhao     Modify notify and error notify about Signal and SignalGroup
 *  V2.3.2    20250619  tong.zhao     Fix bug the signal value init logical.
 *  V2.3.3    20250620  tong.zhao     Remove the restriction that the sending condition for the periodic Pdu in the If
 *                                    cycle is based on receiving the previous transmission's Confirmation.
 *  V2.3.4    20251225  jianyu.yang   Add procedure to clear TxIpduStatePtr->RptNum in Com_MainFunction_SendPdu to meet
 SWS_Com_00467.
 *  V2.3.5    20260119  jianyu.yang   Modify transmiting condition in Com_TriggerIPDUSend only valid in TP type and fix
 build bug in Com_ResetTxIpduMDT.
 *  V2.3.6    20260228  jianyu.yang   Modify buiding conditions of functions related to Gateway.
 *  V2.3.7    20260322  jianyu.yang   Put TMC of TxSignal,TxGroupSignal and DescriptionSignal in to a same buffer.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Com<br>
  RuleSource:puhua-rule.rcf 3.0.0

    \li VL_Com_UntrustedMethod
      Reason:The parameter(inout) will be changed in the function.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_InitReturnVar
      Reason:Init may be necessary for return variant.
      Risk:There may be redundant assignments.
      Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_Com_5004
      Reason:Function Pointers are not used for data copying.
      Risk:Null pointer is used to copy data will result undefined behavior.
      Prevention:The reliability of the function is guaranteed by the design.

    \li VL_Com_NullPtrValue
      Reason:A null pointer may be assigned or compared.
      Risk:Any attempt to perform arithmetic operations on it will result in undefined behavior.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Com_ReferencedOnlyOne
      Reason:In order to make the module code structure clear, the functions are classified.
      Risk:There may be redundant assignments.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Com_Cyclomatic
      Reason:High cyclomatic complexity indicates inadequate modularization or too much logic in one function.
      Risk:There are risks associated with complex functions.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Com_OperationRedundant
      Reason:This operation is necessary.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_BooleanType
      Reason:Necessary type conversions.
      Risk:An expression of 'essentially Boolean' type is being cast to unsigned type.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Com_NeverModify
      Reason:The parameter will be modified, it could not be qulified as "const".
      Risk:This parameter is declared as an array may be misused.
      Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_Com_PointerObjectType
      Reason:Necessary type conversions.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_DataConverteToWider
      Reason:Data conversion to a wider range is necessary for accumulation.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_ConstPointer
      Reason:The const "RxDynSignalLength" parameter will not lead to modification.
      Risk:This parameter is declared as an array may be misused.
      Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_Com_ResultAlwaysTrue
    Reason:The result will be changed to false when the parameter is modified.
    Risk:The test branch may be incomplete.
    Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_Com_SwitchRedundant
      Reason:this switch is neccesary.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_ConversionSigned
      Reason:Necessary type conversions.
      Risk:An operand of type signed short is being converted to type signed int.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Com_Unreachable
      Reason:The function checks if the array boundary is exceeded.
      Risk:Dereference of an invalid pointer value.
      Prevention:Use Det detection and test of the different configurations.

    \li VL_Com_NegtiveValueType
      Reason:Necessary macro definition
      Risk:A constant expression with negative value is being define an unsigned type macro.
      Prevention:Macros are defined when outside the scope of a type,Correctness and reliability have been guaranteed.

    \li VL_Com_ShiftOperation
      Reason:Necessary left shift operation.
      Risk:High order bits will be lost.
      Prevention:Code inspection and test of the different configurations in the component test.

    \li VL_Com_BoolPromoted
      Reason:Designed boolean type operation.
      Risk:No risk.
      Prevention:None.

    \li VL_Com_1505
      Reason: In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Com_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Com_0686
      Reason: To resolve the compilation warnings.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_1504
      Reason:In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_1514
      Reason:In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_0791
      Reason:design needs, namelength set to 63 in C99.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_3206
      Reason: Variables are used in User Callout code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_1712
      Reason: xternal identifiers have the same first '${n}' characters '${name}'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Com_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
              location.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Com_STVAR
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Com_STPTH
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Com_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Com_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Com_STPAR
      Reason: The external API of the module is defined by the AUTOSAR standard, resulting in its metric being above the
  threshold
      Risk: For the target uC, the stack usage and runtime are too high
      Prevention: When testing the result code on the target uC, the user must check the stack usage in the project
  context

    \li VL_MTR_Com_STCYC
      Reason: Due to the implementation requirements of some diagnostic message functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Com_STECT
      Reason: This is violated under extreme conditions due to the total number of variables such as different services
  and protocols and user configuration
      Risk: The code is difficult to maintain and the data flow is complex
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

#ifndef COM_H_
#define COM_H_

/* =================================================== inclusions =================================================== */
#include "Com_Types.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
#define COM_MODULE_ID                   (50u)
#define COM_VENDOR_ID                   (62u)
#define COM_AR_RELEASE_MAJOR_VERSION    (4u)
#define COM_AR_RELEASE_MINOR_VERSION    (9u)
#define COM_AR_RELEASE_REVISION_VERSION (0u)
#define COM_SW_MAJOR_VERSION            (2u)
#define COM_SW_MINOR_VERSION            (3u)
#define COM_SW_PATCH_VERSION            (6u)
/* ===================================================== macros ===================================================== */
#define COM_SERVICE_NOT_AVAILABLE ((uint8)0x80u)
#define COM_BUSY                  ((uint8)0x81u)
#define COM_INSTANCE_ID           (0x00u)

/*Service ID*/
#define COM_INIT_ID                        ((uint8)0x01u)
#define COM_DEINIT_ID                      ((uint8)0x02u)
#define COM_IPDUGROUPSTART_ID              ((uint8)0x03u)
#define COM_IPDUGROUPSTOP_ID               ((uint8)0x04u)
#define COM_ENABLERECEPTIONDM_ID           ((uint8)0x06u)
#define COM_DISABLERECEPTIONDM_ID          ((uint8)0x05u)
#define COM_GETSTATUS_ID                   ((uint8)0x07u)
#define COM_GETVERSIONINFO_ID              ((uint8)0x09u)
#define COM_SENDSIGNAL_ID                  ((uint8)0x0Au)
#define COM_SENDDYNSIGNAL_ID               ((uint8)0x21u)
#define COM_RECEIVESIGNAL_ID               ((uint8)0x0Bu)
#define COM_RECEIVEDYNSIGNAL_ID            ((uint8)0x22u)
#define COM_SENDSIGNALGROUP_ID             ((uint8)0x0Du)
#define COM_RECEIVESIGNALGROUP_ID          ((uint8)0x0Eu)
#define COM_SENDSIGNALGROUPARRAY_ID        ((uint8)0x23u)
#define COM_RECEIVESIGNALGROUPARRAY_ID     ((uint8)0x24u)
#define COM_INVALIDATESIGNAL_ID            ((uint8)0x10u)
#define COM_INVALIDATESIGNALGROUP_ID       ((uint8)0x1Bu)
#define COM_TRIGGERIPDUSEND_ID             ((uint8)0x17u)
#define COM_TRIGGERIPDUSENDWITHMETADATA_ID ((uint8)0x28u)
#define COM_SWITCHIPDUTXMODE_ID            ((uint8)0x27u)
#define COM_TRIGGERTRANSMIT_ID             ((uint8)0x41u)
#define COM_RXINDICATION_ID                ((uint8)0x42u)
#define COM_TPRXINDICATION_ID              ((uint8)0x45u)
#define COM_TXCONFIRMATION_ID              ((uint8)0x40u)
#define COM_TPTXCONFIRMATION_ID            ((uint8)0x48u)
#define COM_STARTOFRECEPTION_ID            ((uint8)0x46u)
#define COM_COPYRXDATA_ID                  ((uint8)0x44u)
#define COM_COPYTXDATA_ID                  ((uint8)0x43u)
#define COM_MAINFUNCTIONRX_ID              ((uint8)0x18u)
#define COM_MAINFUNCTIONROUTESIGNALS_ID    ((uint8)0x1Au)

#if (STD_ON == COM_DEV_ERROR_DETECT)
/* Error detection */
#define COM_E_NONE          ((uint8)0x00u)
#define COM_E_PARAM         ((uint8)0x01u)
#define COM_E_UNINIT        ((uint8)0x02u)
#define COM_E_PARAM_POINTER ((uint8)0x03u)
#define COM_E_INIT_FAILED   ((uint8)0x04u)
#define COM_E_PARTITION     ((uint8)0x05u)
#endif
#define COM_MAINFUNCTIONTX_ID      (0x19u)
#define COM_E_SKIPPED_TRANSMISSION (0x05u)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
/* partition list */
extern const uint16 Com_PartitionList[COM_MAX_PARTITION_NUMBER];
#endif
/* ========================================= external function declarations ========================================= */
/* PRQA S 5015 ++ */ /* VL_Com_UntrustedMethod */
/**
 * @brief         This service initializes internal and external interfaces and variables of the AUTOSAR COM
 *                module layer for the further processing. After calling this function the inter-ECU
 *                communication is still disabled.
 * @param[in]     config: Pointer to the AUTOSAR COM module's configuration data.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-61720
 */
extern void Com_Init(const Com_ConfigType* config);
/**
 * @brief         This service stops the inter-ECU communication. All started
 *                I-PDU groups are stopped and have to be started again, if
 *                needed, after Com_Init is called. By a call to Com_DeInit
 *                the AUTOSAR COM module is put into an not initialized state.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-61721
 */
extern void Com_DeInit(void);

#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
/**
 * @brief         Starts a preconfigured I-PDU group.
 * @param[in]     IpduGroupId: Id of I-PDU group to be started
 * @param[in]     initialize: flag to request initialization of the data in the I-PDUs of this I-PDU group
 * @reentrant     Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group
 * @synchronous   TRUE
 * @trace         CPD-61722
 */
extern void Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId, boolean initialize);
#endif

#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
/**
 * @brief         Stops a preconfigured I-PDU group.
 * @param[in]     IpduGroupId: Id of I-PDU group to be stopped
 * @reentrant     Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group
 * @synchronous   TRUE
 * @trace         CPD-61723
 */
extern void Com_IpduGroupStop(Com_IpduGroupIdType IpduGroupId);
#endif
/**
 * @brief         Enables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 * @param[in]     IpduGroupId: Id of I-PDU group where reception DM shall be enabled.
 * @reentrant     Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group
 * @synchronous   TRUE
 * @trace         CPD-66058
 */
extern void Com_EnableReceptionDM(Com_IpduGroupIdType IpduGroupId);
/**
 * @brief         Disables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 * @param[in]     IpduGroupId: Id of I-PDU group where reception DM shall be disabled.
 * @reentrant     Reentrant for different I-PDU groups. Non reentrant for the same I-PDU group
 * @synchronous   TRUE
 * @trace         CPD-61725
 */
extern void Com_DisableReceptionDM(Com_IpduGroupIdType IpduGroupId);
/**
 * @brief         Returns the status of the AUTOSAR COM module.
 * @retval        COM_UNINIT: the AUTOSAR COM module is not initialized and not usable
 * @retval        COM_INIT: the AUTOSAR COM module is initialized and usable
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61726
 */
extern Com_StatusType Com_GetStatus(void);

#if (STD_ON == COM_VERSION_INFO_API)
/**
 * @param[in]     versioninfo: Pointer to where to store the version information of this module.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61727
 */
extern void Com_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif
/**
 * @brief         The service Com_SendSignal updates the signal(include group signal) object
 *                identified by SignalId with the signal referenced by the SignalDataPtr parameter.
 * @param[in]     SignalId: Id of signal to be sent.
 * @param[in]     SignalDataPtr: Reference to the signal data to be transmitted.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development
 * error)
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     Non Reentrant for the same signal. Reentrant for different signals.
 * @synchronous   FALSE
 * @trace         CPD-61728
 */
extern uint8 Com_SendSignal(Com_SignalIdType SignalId, const void* SignalDataPtr);
/**
 * @brief         The service Com_SendDynSignal updates the signal object identified by SignalId
 *                with the signal referenced by the SignalDataPtr parameter.
 * @param[in]     SignalId: Id of signal to be sent.
 * @param[in]     SignalDataPtr: Reference to the signal data to be transmitted.
 * @param[in]     Length: Length of the dynamic length signal
 * @retval        E_OK: service has been accepted
 * @retval        E_NOT_OK: in case the Length is greater than the configured ComSignalLength of this sent signal
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 * @retval        COM_BUSY: in case the TP-Buffer is locked
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-61729
 */
extern uint8 Com_SendDynSignal(Com_SignalIdType SignalId, const void* SignalDataPtr, uint16 Length);
/**
 * @brief         Com_ReceiveSignal copies the data of the signal identified by SignalId to the location specified
 *                by SignalDataPtr.
 * @param[in]     SignalId: Id of signal to be received.
 * @param[in]     SignalDataPtr: Reference to the location where the received signal data shall be stored
 * @retval        E_OK: service has been accepted
 * @retval        E_NOT_OK: in case the Length is greater than the configured ComSignalLength of this sent signal
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61750
 */
extern uint8 Com_ReceiveSignal(Com_SignalIdType SignalId, void* SignalDataPtr);
/**
 * @brief         Com_ReceiveDynSignal copies the data of the signal identified by SignalId to the
 *                location specified by SignalDataPtr and stores the length of the dynamical length
 *                signal at the position given by the Length parameter.
 * @param[in]     SignalId: Id of signal to be received.
 * @param[in]     SignalDataPtr: Reference to the location where the received signal data shall be stored
 * @param[inout]  Length: in: maximum length that could be received;out: length of the dynamic length signal
 * @retval        E_OK: service has been accepted
 * @retval        E_NOT_OK: in case the Length (as in-parameter) is smaller than the received length of the dynamic
 * length signal
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 * @retval        COM_BUSY: in case the TP-Buffer is locked
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61751
 */
extern uint8 Com_ReceiveDynSignal(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length);
/**
 * @brief         The service Com_SendSignalGroup copies the content of the associated shadow buffer to the
 *                associated I-PDU.
 * @param[in]     SignalGroupId: Id of signal group to be sent.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development
 * error)
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-61730
 */
extern uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId);
/**
 * @brief         The service Com_ReceiveSignalGroup copies the received signal group from the I-PDU to the shadow
 * buffer.
 * @param[in]     SignalGroupId: Id of signal group to be received.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stoppedailed due to development error)
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61731
 */
extern uint8 Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId);

#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
/**
 * @brief         The service Com_SendSignalGroupArray copies the content of the provided
 *                SignalGroupArrayPtr to the associated I-PDU. The provided data shall
 *                correspond to the array representation of the signal group.
 * @param[in]     SignalGroupId: Id of signal group to be sent.
 * @param[in]     SignalGroupArrayPtr: Reference to the signal group array.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-61732
 */
extern uint8 Com_SendSignalGroupArray(Com_SignalGroupIdType SignalGroupId, const uint8* SignalGroupArrayPtr);
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
/**
 * @brief         The service Com_ReceiveSignalGroupArray copies the received signal group
 *                array representation from the PDU to the location designated by SignalGroupArrayPtr.
 * @param[in]     SignalGroupId: Id of signal group to be sent.
 * @param[in]     SignalGroupArrayPtr: Reference to the signal group array.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped
 * @retval        COM_BUSY: in case the TP-Buffer is locked for large data types handling
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-61733
 */
extern uint8 Com_ReceiveSignalGroupArray(Com_SignalGroupIdType SignalGroupId, uint8* SignalGroupArrayPtr);
#endif
/**
 * @brief         The service Com_InvalidateSignal invalidates the signal with the given SignalId
 *                by setting its value to its configured ComSignalDataInvalidValue.
 * @param[in]     SignalId: Id of signal to be invalidated.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue
 *                is configured for the given signalId
 * @retval        COM_BUSY: in case the TP-Buffer is locked
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-61734
 */
extern uint8 Com_InvalidateSignal(Com_SignalIdType SignalId);
/**
 * @brief         The service Com_InvalidateSignalGroup invalidates all group signals of the signal
 *                group with the given SignalGroupId by setting their values to their configured
 *                ComSignalDataInvalidValues.
 * @param[in]     SignalGroupId: Id of signal group to be invalidated.
 * @retval        E_OK: service has been accepted
 * @retval        COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue
 *                is configured for any of the group signals
 * @retval        COM_BUSY: in case the TP-Buffer is locked
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-61735
 */
extern uint8 Com_InvalidateSignalGroup(Com_SignalGroupIdType SignalGroupId);
/**
 * @brief         By a call to Com_TriggerIPDUSend the I-PDU with the given ID is triggered for transmission.
 * @param[in]     PduId: The I-PDU-ID of the I-PDU that shall be triggered for sending
 * @retval        E_OK: I-PDU was triggered for transmission
 * @retval        E_NOT_OK: I-PDU is stopped, the transmission could not be triggered
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-61736
 */
extern Std_ReturnType Com_TriggerIPDUSend(PduIdType PduId);
#if (STD_ON == COM_METADATA_SUPPORT)
/**
 * @brief         By a call to Com_TriggerIPDUSendWithMetaData the AUTOSAR COM module
 *                updates its internal metadata for the I-PDU with the given ID by copying the
 *                metadata from the given position and with respect to the globally configured metadata
 *                length of the I-PDU. Then the I-PDU is triggered for transmission.
 * @param[in]     PduId: The I-PDU-ID of the I-PDU that shall be triggered for sending
 * @param[in]     MetaData: A pointer to the metadata for the triggered send-request
 * @retval        E_OK: I-PDU was triggered for transmission
 * @retval        E_NOT_OK: I-PDU is stopped, the transmission could not be triggered
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-61737
 */
/* PRQA S 1712 ++ */ /* VL_Com_1712 */
extern Std_ReturnType Com_TriggerIPDUSendWithMetaData(PduIdType PduId, const uint8* MetaData);
/* PRQA S 1712 -- */
#endif
/**
 * @brief         The service Com_SwitchIpduTxMode sets the transmission mode of the I-PDU referenced by PduId to
 *                Mode. In case the transmission mode changes, the new mode shall immediately be effective. In case
 *                the requested transmission mode was already active for this I-PDU, the call will have no effect.
 * @param[in]     PduId: Id of the I-PDU of which the transmission mode shall be changed.
 * @param[in]     Mode: the transmission mode that shall be set.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-61738
 */
extern void Com_SwitchIpduTxMode(PduIdType PduId, boolean Mode);
/**
 * @brief         This function performs the processing of the AUTOSAR COM module's receive processing that
 *                are not directly handled within the COM's functions invoked by the PDU-R, for example
 *                Com_RxIndication.
 * @param[in]     RxMainFunctionId: MainfunctionId for Rx.
 * @reentrant     Reentrant for different instances. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-61747
 */
extern void Com_MainFunctionRx(Com_MainFunctionIdType RxMainFunctionId);
/**
 * @brief         This function performs the processing of the AUTOSAR COM module's transmission activities that
 *                are not directly handled within the COM's function invoked by the RTE, for example Com_SendSignal.
 * @param[in]     TxMainFunctionId: MainfunctionId for Tx.
 * @reentrant     Reentrant for different instances. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-61748
 */
extern void Com_MainFunctionTx(Com_MainFunctionIdType TxMainFunctionId);
/**
 * @brief         Calls the signal gateway part of the AUTOSAR COM module to forward received signals to be routed.
 * @param[in]     RouteSignalsMainFunctionId: MainfunctionId for RouteSignals.
 * @reentrant     Reentrant for different instances. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-66392
 */
extern void Com_MainFunctionRouteSignals(Com_MainFunctionIdType RouteSignalsMainFunctionId);

/* PRQA S 5015 -- */
#ifdef __cplusplus
}
#endif

#endif
