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
 **  @file               : Dem.h
 **  @author             : tao.yu
 **  @date               : 2024/08/01
 **  @vendor             : isoft
 **  @description        : API declaration of Dem
 **
 ***********************************************************************************************************************/

#ifndef DEM_H
#define DEM_H
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V03.00.00 2024-08-01  tao.yu       Initial version
 *            2025-04-23  tao.yu       CPD-79306,CPT-14050, fix warning
 *            2025-04-29  tao.yu       It can be compiled successfully even without configuring NvRam.
 *  V03.00.01 2025-05-07  tao.yu       fix error in DEM_CLRRESP_NONVOLATILE_TRIGGER.
 *  V03.00.02 2025-06-04  tao.yu       CPT-14604, fix error in J1939FreezeFrame.
 *  V03.00.03 2025-08-11  haibin.shao  CPT-15512, fix error in DMxx macro symbols are used incorrectly.
 *  V03.00.04 2025-08-13  chao.sun     CPT-15552, fix error in Aging And Indicator.
 *            2025-08-22  tao.yu       fix error in DemSupportedObdUdsDtcSeparation is on.
 *            2025-08-27  chao.sun     CPT-15676 fix error in FreezeFrame.
 *  V03.00.05 2025-09-02  chao.sun     CPD-84308,fix error in Dem_EnableConditionInit and others.
 *            2025-09-08  chao.sun     CPD-84308,fix error in ExtendedData.
 *            2025-09-11  chao.sun     CPD-84308,fix error in satellite queue status handle
 *            2025-09-23  li.feng      CPD-84625,QAC issues
 *            2025-09-25  chao.sun     CPD-84308,Update handle in internal debounce
 *            2025-09-25  chao.sun     CPD-84308,Update handle in aging
 *  V03.00.06 2025-10-17  chao.sun     CPD-84308,DEM Module Optimization
 *  V03.00.07 2025-11-10  chao.sun     CPT-16318,Timing for counting the update of the failed cycle counter
 *  V03.00.08 2025-12-22  chao.sun     CPT-17177,Handling errors caused by unconfigured frozen frame compilation
 *            2025-12-30  chao.sun     CPT-17415,Add invalid index value check for memoryIndex
 *  V03.00.09 2026-01-07  chao.sun     CPT-17540,Dem_GetMonitorStatus adds an initialization check
 *            2026-01-08  chao.sun     In the Dem_SatellitePreInit interface, the partition ID is changed
 *                                     to be obtained via parameters.
 *  V03.00.10 2026-03-25  chao.sun     CPTASK-611,Fix compilation issues caused by incorrect modifications.
 *  V03.00.11 2026-04-04  tao.yu       CPT-18413, Fix the problem of excessive stack exception occupation.
 *            2026-04-10  chao.sun     CPT-18500, Set the value of DEM_FAILED_CYCLES to 0xFF.
 ====================================================================================================================*/

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dem<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Dem_0310
      Reason: Compatible with different types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0311
      Reason: Compatible with different types of functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0313
      Reason: Compatible with different types of functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0342
      Reason: Compatible with different types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0563
      Reason: Quickly get a pointer to the corresponding function that needs to be executed according to the target.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0635
      Reason: Display settings bit-width to accommodate special platforms.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0750
      Reason: Fast variable type conversion, Guaranteed data consistency, Low space consumption.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0759
      Reason: Fast variable type conversion, Guaranteed data consistency, Low space consumption.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0770
      Reason: Ability to quickly jump of next loops in different situations with different configurations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_0771
      Reason: Ability to quickly jump out of loops in different situations with different configurations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1039
      Reason: Configure the length of the generated array, it depends on the configuration, may be configured as 1
              length.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1252
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1258
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1259
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1290
      Reason: Other Module Macro Definition References
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1504
      Reason: Multi-partition variable design requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1505
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1531
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1533
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1751
      Reason: Multi-partition variable design requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1891
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2003
      Reason: Avoid duplicate code calls.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2109
      Reason: Necessary data processing.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2120
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2152
      Reason: Necessary signed number operations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2464
      Reason: Multiple messages need to be processed when looping.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2487
      Reason: Control variables that are pointers will self-increment in the pass function.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3103
      Reason: Necessary signed number operations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3218
      Reason: const function define.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3406
      Reason: Subcomponent structure is implemented in header files.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3432
      Reason: Simple macro parameter expressions are only used for conditional judgments and do not require parentheses.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3449
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3451
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3471
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3473
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3672
      Reason: Quickly get a pointer to the corresponding function that needs to be executed according to the target.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4304
      Reason: Necessary data processing.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4342
      Reason: Required type conversions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4399
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4461
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_3450
      Reason: Without a previous declaration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Dem_STCAL
      Reason: The software structure of Dem is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dem_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different DTCs, events, pids, dids and other configuration conditions, as well as some precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_Dem_STPAR
      Reason: The external API of Dem is defined by the AUTOSAR standard, resulting in its metric being above the
  threshold
      Risk: For the target uC, the stack usage and runtime are too high
      Prevention: When testing the result code on the target uC, the user must check the stack usage in the project
  context

    \li VL_MTR_Dem_STECT
      Reason: This is violated under extreme conditions due to the total number of variables such as different user
  configuration
      Risk: The code is difficult to maintain and the data flow is complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_Dem_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Dem_6510
      Reason: Some special files have less code or no code at all.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review + clear structure and well-commented code.

    \li VL_Dem_3132
      Reason: This cast is converting a complex floating point expression to an integral type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_1707
      Reason: Function '${name}' is not using the same aliases.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2053
      Reason: Special code design of the Dem.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4391
      Reason: In the code, it is necessary to convert a small range of unsigned integers into a larger range of
    unsigned integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_4491
      Reason: DataType is determined by the configuration.
      Risk: Code redundancy and may difficult to maintain
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_Dem_1753
      Reason: The function '${name}' with external linkage is declared but not defined within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Dem_1501
      Reason: The function '${name}' is declared but is not used within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Dem_4404
      Reason: An expression of 'essentially Boolean' type (%1s) is being converted to unsigned type, '%2s' on
  assignment. Risk: No risk. Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Dem_3120
      Reason: In the generated code, there are a lot of devil numbers, don't have to define a macro.
      Risk: The reader can derive the meaning of the number based on the annotation or structure type, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Dem_3332
      Reason: The corresponding macro definition has been obtained through the header file inclusion.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Dem_2742
      Reason: This 'if' controlling expression is a constant expression and its value is 'false'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

/* =================================================== inclusions =================================================== */
#include "Dem_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name DEM_VERSION_INFO
 * @{
 * @brief Identification and version information - decimal encoding
 */
#define DEM_VENDOR_ID                   (62U) /**< Value of vendor */
#define DEM_MODULE_ID                   (54U) /**< Value of module */
#define DEM_INSTANCE_ID                 (0U)  /**< Value of instance */
#define DEM_AR_RELEASE_MAJOR_VERSION    (4U)  /**< Value of Autosar major version */
#define DEM_AR_RELEASE_MINOR_VERSION    (9U)  /**< Value of Autosar minor version */
#define DEM_AR_RELEASE_REVISION_VERSION (0U)  /**< Value of Autosar patch version */
#define DEM_SW_MAJOR_VERSION            (3U)  /**< Value of module major version */
#define DEM_SW_MINOR_VERSION            (0U)  /**< Value of module minor version */
#define DEM_SW_PATCH_VERSION            (11U) /**< Value of module patch version */

/* ========================================= external function declarations ========================================= */
#if (DEM_VERSION_INFO_API == STD_ON)
/**
 * @brief         Returns the version information of this module. API Availability: This API will be available only if
 * ({ecuc(Dem/DemGeneral.DemVersionInfoApi)} == true)
 * @param[out]    versionInfo: Pointer to where to store the version information of this module.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61159
 */
extern void Dem_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif

/**
 * @brief         Initializes the internal states necessary to process events reported by BSW-modules.
 * @param[in]     ConfigPtr: Pointer to the configuration set in VARIANT-POST-BUILD.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-64411
 */
extern void Dem_PreInit(const Dem_ConfigType* ConfigPtr);

/**
 * @brief         Initializes or reinitializes this module.
 * @param[in]     ConfigPtr: Pointer to the configuration set in VARIANT-POST-BUILD.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61158
 */
extern void Dem_Init(const Dem_ConfigType* ConfigPtr);

/**
 * @brief         Shuts down this module.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61157
 */
extern void Dem_Shutdown(void);

#if (DEM_SUPPORT_CLEARDTC_API == STD_ON)
/**
 * @brief         Clears single DTCs, as well as groups of DTCs.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC successfully cleared
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist or clearing is restricted by
 *                               configuration to group of all DTCs only.
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_CLEAR_FAILED: DTC clearing failed
 * @retval        DEM_CLEAR_BUSY: Another client is currently clearing DTCs. The requested operation will not be
 *                                started and the caller shall try again at a later moment.
 * @retval        DEM_CLEAR_MEMORY_ERROR: An error occurred during erasing a memory location (e.g. if
 *                                        DemClearDTCBehavior is set to DEM_CLRRESP_NON-VOLATILE_FINISH and erasing
 *                                        of non-volatile-block failed).
 * @retval        DEM_PENDING: Clearing the DTCs is currently in progress. The caller shall call this function
 *                             again at a later moment.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client
 *                          is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61156
 */
extern Std_ReturnType Dem_ClearDTC(uint8 ClientId);
#endif
/* PRQA S 3449, 3451 ++ */ /* VL_QAC_MultiDeclaration, VL_QAC_MultiDeclaration */
/* PRQA S 1512, 1513 ++ */ /* VL_Dcm_1512,VL_QAC_MultiDeclaration */
#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
/**
 * @brief         Clears a prestored freeze frame of a specific event. This API can only be used through the RTE
 *                and therefore no declaration is exported via Dem.h. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemConfigSet/DemEventParameter.DemFFPrestorageSupported)} == true)
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Clear prestored freeze frame was successful
 * @retval        E_NOT_OK: Clear prestored freeze frame failed
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61155
 */
extern Std_ReturnType Dem_ClearPrestoredFreezeFrame(Dem_EventIdType EventId);
#endif

#if (DEM_COMPONENT_NUMBER > 0u)
/**
 * @brief         Gets the failed status of a DemComponent.
 * @param[in]     ComponentId: Identification of a DemComponent
 * @param[out]    ComponentFailed: TRUE: failed FALSE: not failed
 * @return        Std_ReturnType
 * @retval        E_OK: Getting "ComponentFailed" was successful
 * @retval        E_NOT_OK: Getting "ComponentFailed" was not successful
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61154
 */
extern Std_ReturnType Dem_GetComponentFailed(Dem_ComponentIdType ComponentId, boolean* ComponentFailed);
#endif

#if (STD_ON == DCM_UDS_0X14)
/**
 * @brief         Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: The DTC select parameter check is successful and the requested DTC or group of DTC in the
 *                      selected origin is selected for further operations.
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Checking the SelectDTC parameters is currently in progress. The caller shall call
 *                             this function again later.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client
 *                          is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61151
 */
extern Std_ReturnType Dem_GetDTCSelectionResult(uint8 ClientId);

/**
 * @brief         Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of
                  DTCs, respecting the settings if the Dem shall clear only all DTCs.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: The DTC select parameter check is successful and the requested DTC or group of DTC in the
 *                      selected origin is selected for further operations.
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Checking the SelectDTC parameters is currently in progress. The caller shall call
 *                             this function again later.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client
 *                          is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61152
 */
extern Std_ReturnType Dem_GetDTCSelectionResultForClearDTC(uint8 ClientId);
#endif

/**
 * @brief         Gets the current UDS status byte assigned to the DTC for the event
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[out]    UDSStatusByte: UDS DTC status byte of the requested event (refer to chapter "Status bit
 *                               support"). If the return value of the function call is E_NOT_OK, this parameter
 *                               does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Get of event status was successful
 * @retval        E_NOT_OK: Get of event status failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61153
 */
extern Std_ReturnType Dem_GetEventUdsStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType* UDSStatusByte);

/**
 * @brief         Gets the current monitor status for an event.
 * @param[in]     EventID: Identification of an event by assigned EventId.
 * @param[out]    MonitorStatus: Monitor status byte of the requested event. If the return value of the function
 *                               call is E_NOT_OK, this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Get monitor status was successful
 * @retval        E_NOT_OK: Get the monitor status failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61148
 */
extern Std_ReturnType Dem_GetMonitorStatus(Dem_EventIdType EventID, Dem_MonitorStatusType* MonitorStatus);

/**
 * @brief         Gets the debouncing status of an event. This function shall not be used for EventId with native
 *                debouncing within their functions. It is rather for EventIds using debouncing within the Dem.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[out]    DebouncingState: Bit 0 Temporarily Defective (corresponds to 0 < FDC < 127) Bit 1 finally
 *                                 Defective (corresponds to FDC = 127) Bit 2 temporarily healed
 *                                 (corresponds to -128 < FDC < 0) Bit 3 Test complete (corresponds to
 *                                 FDC = -128 or FDC = 127) Bit 4 DTR Update (= Test complete && Debouncing complete
 *                                 && enable conditions / storage conditions fulfilled)
 * @return        Std_ReturnType
 * @retval        E_OK: Get of debouncing status per event state successful
 * @retval        E_NOT_OK: Get of debouncing per event state failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61149
 */
extern Std_ReturnType Dem_GetDebouncingOfEvent(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState);

/* PRQA S 1707 ++ */ /* VL_Dem_1707 */
/**
 * @brief         Gets the DTC of an event.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC value.
 * @param[out]    DTCOfEvent: Receives the DTC value in respective format returned by this function. If the return
 *                            value of the function is other than E_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Get of DTC was successful
 * @retval        E_NOT_OK: The call was not successful
 * @retval        DEM_E_NO_DTC_AVAILABLE: there is no DTC configured in the requested format
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61150
 */
extern Std_ReturnType Dem_GetDTCOfEvent(Dem_EventIdType EventId, Dem_DTCFormatType DTCFormat, uint32* DTCOfEvent);

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Returns the suppression status of a specific DTC. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemGeneral.DemSuppressionSupport)} == DEM_DTC_SUPPRESSION)
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[out]    SuppressionStatus: Defines whether the respective DTC is suppressed (TRUE) or enabled (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful.
 * @retval        E_NOT_OK: Dem_SelectDTC was not called.
 * @retval        DEM_WRONG_DTC: No valid DTC or DTC group selected.
 * @retval        DEM_WRONG_DTCORIGIN: Wrong DTC origin selected.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61146
 */
extern Std_ReturnType Dem_GetDTCSuppression(uint8 ClientId, boolean* SuppressionStatus);
#endif

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
/**
 * @brief         Get the Event availability.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[out]    AvailableStatus: TRUE if the event is available. FALSE if the event is not available.
 * @return        Std_ReturnType
 * @retval        E_OK: Event availability has been obtained.
 * @retval        E_NOT_OK: Event availability cannot be obtained.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61147
 */
extern Std_ReturnType Dem_GetEventAvailable(Dem_EventIdType EventId, boolean* AvailableStatus);
#endif

/**
 * @brief         Gets the fault detection counter of an event. This API can only be used through the RTE, and
 *                therefore no declaration is exported via Dem.h.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[out]    FaultDetectionCounter: This parameter receives the Fault Detection Counter information of the
 *                                       requested EventId. If the return value of the function call is other
 *                                       than E_OK this parameter does not contain valid data. -128dec...127dec
 *                                       PASSED...FAILED according to ISO 14229-1
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request failed
 * @retval        DEM_E_NO_FDC_AVAILABLE: there is no fault detection counter available for the requested event
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61145
 */
extern Std_ReturnType Dem_GetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter);

#if (DEM_INDICATOR_NUMBER > 0u)
/**
 * @brief         Gets the indicator status derived from the UDS status. API Availability: This API will be
 *                available only if ({ecuc(Dem/DemGeneral/DemEventMemorySet/DemIndicator)} != NULL)
 * @param[in]     IndicatorId: Number of indicator
 * @param[out]    IndicatorStatus: Status of the indicator, like off, on, or blinking.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61144
 */
extern Std_ReturnType Dem_GetIndicatorStatus(uint8 IndicatorId, Dem_IndicatorStatusType* IndicatorStatus);
#endif

#if (DEM_FEATURE_SWC_GET_FF_ED == STD_ON)
/**
 * @brief         Gets the data of a freeze frame by event.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     RecordNumber: This parameter is a unique identifier for a freeze frame record as defined in
 *                              ISO14229-1. 0xFF means most recent freeze frame record is returned. 0x00 is only
 *                              supported if the Dem module supports WWH-OBD (refer to DemOBDSupport)
 * @param[in]     DataId: This parameter specifies the DID (ISO14229-1) that shall be copied to the destination buffer.
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            freeze frame data record shall be written to. The format is raw hexadecimal values
 *                            and contains no header-information.
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written data
 *                         bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation could not be performed
 * @retval        DEM_NO_SUCH_ELEMENT: The requested event data is not currently stored (but the request was
 *                                     valid) OR The requested record number is not supported by the event OR
 *                                     The requested DID is not supported by the freeze frame.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer size is too small.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61143
 */
extern Std_ReturnType Dem_GetEventFreezeFrameDataEx(
    Dem_EventIdType EventId,
    uint8           RecordNumber,
    uint16          DataId,
    uint8*          DestBuffer,
    uint16*         BufSize);

/**
 * @brief         Gets the data of an extended data record by event.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     RecordNumber: Identification of requested Extended data record. Valid values are between 0x01
 *                              and 0xEF as defined in ISO14229-1.
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            extended data data record shall be written to. The format is raw hexadecimal values
 *                            and contains no header-information.
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written data
 *                         bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation could not be performed
 * @retval        DEM_NO_SUCH_ELEMENT: The requested event data is not currently stored (but the request was
 *                                     valid) OR The requested record number is not supported by the event.
 * @retval        DEM_BUFFER_TOO_SMALL: The provided buffer size is too small.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61142
 */
extern Std_ReturnType
    Dem_GetEventExtendedDataRecordEx(Dem_EventIdType EventId, uint8 RecordNumber, uint8* DestBuffer, uint16* BufSize);
#endif

#if (DEM_USER_SUPPORT_OVFLIND_API == STD_ON)
/**
 * @brief         Gets the event memory overflow indication status.
 * @param[in]     ClientId: DemClientID identifying the DemEventMemorySet indicating in which event memory the
 *                          overflow has occurred.
 * @param[in]     DTCOrigin: If the Dem supports more than one event memory this parameter is used to select the
 *                           source memory the overflow indication shall be read from.
 * @param[out]    OverflowIndication: This parameter returns TRUE if the according event memory was overflowed,
 *                                    otherwise it returns FALSE.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed or is not supported
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61141
 */
extern Std_ReturnType
    Dem_GetEventMemoryOverflow(uint8 ClientId, Dem_DTCOriginType DTCOrigin, boolean* OverflowIndication);
/**
 * @brief         Returns the number of entries currently stored in the requested event memory.
 * @param[in]     ClientId: DemClientID identifying the DemEventMemorySet to which the requested event memory
 *                          belongs to.
 * @param[in]     DTCOrigin: If the Dem supports more than one event memory this parameter is used to select the
 *                           source memory the overflow indication shall be read from.
 * @param[out]    NumberOfEventMemoryEntries: Number of entries currently stored in the requested event memory.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61140
 */
extern Std_ReturnType
    Dem_GetNumberOfEventMemoryEntries(uint8 ClientId, Dem_DTCOriginType DTCOrigin, uint8* NumberOfEventMemoryEntries);
#endif
/**
 * @brief         Control the internal debounce counter/timer by BSW modules and SW-Cs. The event qualification
 *                will not be affected by these debounce state changes. This API is available for BSW modules as
 *                soon as Dem_PreInit has been completed (refer to SWS_Dem_00438 and SWS_Dem_00167).
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     DebounceResetStatus: Freeze or reset the internal debounce counter/timer of the specified event.
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61139
 */
extern Std_ReturnType
    Dem_ResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus);

/**
 * @brief         Resets the event failed status. This API can only be used through the RTE and therefore no
 *                declaration is exported via Dem.h.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Request to reset the event status was successful accepted.
 * @retval        E_NOT_OK: Request to reset the event status failed or is not allowed, because the event is
 *                          already tested in this operation cycle.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61138
 */
extern Std_ReturnType Dem_ResetEventStatus(Dem_EventIdType EventId);

/**
 * @brief         Resets the monitor failed status.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Request to reset the monitor status was successful and accepted.
 * @retval        E_NOT_OK: Request to reset the monitor status failed or is not allowed.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-64414
 */
extern Std_ReturnType Dem_ResetMonitorStatus(Dem_EventIdType EventId);

/**
 * @brief         Sets an operation cycle state. This API can only be used through the RTE and therefore no
 *                declaration is exported via Dem.h. The interface has an asynchronous behavior to avoid exceeding
 *                of typical timing requirements on APIs if a large number of events has to be processed and
 *                during the re-initializations of the related monitors. The asynchronous acknowledgements are the
 *                related InitMonitorForEvent callbacks.
 * @param[in]     OperationCycleId: Identification of operation cycle, like power cycle, driving cycle.
 * @return        Std_ReturnType
 * @retval        E_OK: Set of operation cycle was accepted and will be handled asynchronously
 * @retval        E_NOT_OK: Set of operation cycle was rejected
 * @reentrant     Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61137
 */
extern Std_ReturnType Dem_RestartOperationCycle(uint8 OperationCycleId);

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0u)
/**
 * @brief         Captures the freeze frame data for a specific event. This API can only be used through the RTE
 *                and therefore no declaration is exported via Dem.h. API Availability: This API will be available
 *                only if ({ecuc(Dem/DemConfigSet/DemEvent Parameter.DemFFPrestorageSupported)} == true)
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Freeze frame prestorage was successful
 * @retval        E_NOT_OK: Freeze frame prestorage failed
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61136
 */
extern Std_ReturnType Dem_PrestoreFreezeFrame(Dem_EventIdType EventId);
#endif
/* PRQA S 3449, 3451 -- */

#if (DEM_SUPPORT_SELECTDTC_API == STD_ON)
/**
 * @brief         Selects a DTC or DTC group as target for further operations.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTC: Defines the DTC in respective format that is selected. If the DTC fits to a DTC group
 *                     number, the DTC group is selected.
 * @param[in]     DTCFormat: Defines the input-format of the provided DTC value.
 * @param[in]     DTCOrigin: The event memory of the requested DTC or group of DTC.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC successfully selected.
 * @retval        DEM_BUSY: Another Dem_SelectDTC or Dem_SelectDTC dependent operation of this client is
 *                          currently in progress.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61135
 */
extern Std_ReturnType
    Dem_SelectDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);
#endif

#if (DEM_COMPONENT_NUMBER > 0u)
/**
 * @brief         Set the availability of a specific DemComponent.
 * @param[in]     ComponentId: Identification of a DemComponent.
 * @param[in]     AvailableStatus: This parameter specifies whether the respective Component shall be available
 *                                 (TRUE) or not (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: Operation was successful
 * @retval        E_NOT_OK: Operation failed
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61134
 */
extern Std_ReturnType Dem_SetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus);
#endif

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/**
 * @brief         Set the suppression status of a specific DTC. API Availability: This API will be available only
 *                if ({ecuc(Dem/DemGeneral.DemSuppressionSupport)} == DEM_DTC_SUPPRESSION)
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module
 * @param[in]     SuppressionStatus: This parameter specifies whether the respective DTC shall be disabled (TRUE)
 *                                   or enabled (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: The status of the DTC is correctly provided in the DTCStatus parameter.
 * @retval        E_NOT_OK: No DTC selected.
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Non Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61133
 */
extern Std_ReturnType Dem_SetDTCSuppression(uint8 ClientId, boolean SuppressionStatus);
#endif

#if (DEM_ENABLE_CONDITION_GROUP_NUMBER > 0u)
/**
 * @brief         Sets an enable condition. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemEnableCondition)} != NULL)
 * @param[in]     EnableConditionID: This parameter identifies the enable condition.
 * @param[in]     ConditionFulfilled: This parameter specifies whether the enable condition assigned to the
 *                                    EnableConditionID is fulfilled (TRUE) or not fulfilled (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: In case the enable condition could be set successfully the API call returns E_OK.
 * @retval        E_NOT_OK:If the setting of the enable condition failed the return value of the function is E_NOT_OK.
 * @reentrant     Reentrant
 * @synchronous   FALSE
 * @trace         CPD-61132
 */
extern Std_ReturnType Dem_SetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled);
#endif

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY)
/**
 * @brief         Set the available status of a specific Event.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     AvailableStatus: This parameter specifies whether the respective Event shall be available (TRUE)
 *                                 or not (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: Request to set the availability status was successful.
 * @retval        E_NOT_OK: Request to set the availability status not accepted.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   FALSE
 * @trace         CPD-61131
 */
extern Std_ReturnType Dem_SetEventAvailable(Dem_EventIdType EventId, boolean AvailableStatus);
#endif

#if (DEM_EVENT_CONFIRMATION_THRESHOLD_ADAPTABLE == STD_ON)
/**
 * @brief         Set the failure confirmation threshold of an event.
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     FailureCycleCounterThreshold: Failure cycle counter threshold of event to be set.
 * @return        Std_ReturnType
 * @retval        E_OK: Change of threshold was successful.
 * @retval        E_NOT_OK: Threshold cannot be changed as DemEventConfirmationThresholdCounterAdaptable is set
 *                          to FALSE for this event.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   FALSE
 * @trace         CPD-61130
 */
extern Std_ReturnType
    Dem_SetEventConfirmationThresholdCounter(Dem_EventIdType EventId, uint8 FailureCycleCounterThreshold);
#endif

#if (DEM_EVENT_REPORTING_STANDARD == STD_ON)
/**
 * @brief         Called by SW-Cs or BSW modules to report monitor status information to the Dem. BSW modules
 *                calling Dem_SetEventStatus can safely ignore the return value. This API will be available only
 *                if ({Dem/DemConfigSet/DemEventParameter/DemEventReportingType} == STANDARD_REPORTING)
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     EventStatus: Event status
 * @return        Std_ReturnType
 * @retval        E_OK: Set of event status was successful
 * @retval        E_NOT_OK: Event status setting or processing failed or could not be accepted.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61128
 */
extern Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus);
#endif

#if (DEM_EVENT_REPORTING_WITH_MONITOR == STD_ON)
/**
 * @brief         This API will be available only if ({Dem/DemConfigSet/DemEventParameter/DemEventReportingType}
 *                == STANDARD_REPORTING_WITH_MONITOR_DATA)
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @param[in]     EventStatus: Event status
 * @param[in]     monitorData0: -
 * @param[in]     monitorData1: -
 * @return        Std_ReturnType
 * @retval        E_OK: Set of event status was successful
 * @retval        E_NOT_OK: Event status setting or processing failed or could not be accepted.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61129
 */
extern Std_ReturnType Dem_SetEventStatusWithMonitorData(
    Dem_EventIdType     EventId,
    Dem_EventStatusType EventStatus,
    Dem_MonitorDataType monitorData0,
    Dem_MonitorDataType monitorData1);
#endif

#if (DEM_STORAGE_CONDITION_GROUP_NUMBER > 0u)
/**
 * @brief         Sets a storage condition.API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemStorageCondition)} != NULL)
 * @param[in]     StorageConditionID: This parameter identifies the storage condition.
 * @param[in]     ConditionFulfilled: This parameter specifies whether the storage condition assigned to the
 *                                    StorageConditionID is fulfilled (TRUE) or not fulfilled (FALSE).
 * @return        Std_ReturnType
 * @retval        E_OK: In case the storage condition could be set successfully the API call returns E_OK.
 * @retval        E_NOT_OK:If the setting of the storage condition failed the return value of the function is E_NOT_OK.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61123
 */
extern Std_ReturnType Dem_SetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled);
#endif

#if (DEM_USER_CONTROLLED_WIR == STD_ON)
/**
 * @brief         Sets the WIR status bit via failsafe SW-Cs. This API can only be used through the RTE and
 *                therefore no declaration is exported via Dem.h.
 * @param[in]     EventId: Identification of an event by assigned EventId. The Event Number is configured in the
 *                         DEM. Min.: 1 (0: Indication of no Event or Failure) Max.:Result of configuration of Event
 *                         Numbers in DEM (Max is either 255 or 65535)
 * @param[in]     WIRStatus: Requested status of event related WIR-bit (regarding to the current status of
 *                           function inhibition) WIRStatus = TRUE -> WIR-bit shall be set to "1" WIRStatus = FALSE ->
 *                           WIR-bit shall be set to "0"
 * @return        Std_ReturnType
 * @retval        E_OK: Request to set the WIR status was successful.
 * @retval        E_NOT_OK: Request to set the WIR status was not accepted (e.g. disabled controlDTCSetting) and
 *                           should be repeated.
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   FALSE
 * @trace         CPD-61125
 */
extern Std_ReturnType Dem_SetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus);
#endif

#if (DEM_SUPPORT_GETTRANSLATIONTYPE_API == STD_ON)
/**
 * @brief         Gets the supported DTC formats of the ECU.The supported formats are configured via Dem
 *                TypeOfDTCSupported.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Dem_DTCTranslationFormatType: Returns the configured DTC translation format. A combination of
 *                                              different DTC formats is not possible.
 * @retval        DEM_DTC_TRANSLATION_ISO15031_6: ISO15031-6 DTC format/SAE J2012-DA_DTCFormat_00 DTC format
 * @retval        DEM_DTC_TRANSLATION_ISO14229_1: ISO14229-1 DTC format
 * @retval        DEM_DTC_TRANSLATION_SAEJ1939_73: SAEJ1939-73 DTC format
 * @retval        DEM_DTC_TRANSLATION_ISO11992_4: ISO11992-4 DTC format
 * @retval        DEM_DTC_TRANSLATION_J2012DA_FORMAT_04: SAE_J2012-DA_DTCFormat_04 DTC format
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61122
 */
extern Dem_DTCTranslationFormatType Dem_GetTranslationType(uint8 ClientId);
#endif

#if (DEM_SUPPORT_GETDTCSTATUSAVAILABILITYMASK_API == STD_ON)
/**
 * @brief         Gets the DTC Status availability mask of the selected fault memory.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCOrigin: This parameter selects the event memory the DTCStatus AvailabilityMask is requested
 *                           for. Only the values DEM_DTC_ORIGIN_PRIMARY_MEMORY and
 *                           DEM_DTC_ORIGIN_USERDEFINED_MEMORY_<Name> are valid.
 * @param[out]    DTCStatusMask: DTCStatusMask The value DTCStatusMask indicates the supported DTC status bits
 *                               from the Dem. All supported information is indicated by setting the corresponding
 *                               status bit to 1. See ISO14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: Get of DTC status mask was successful
 * @retval        E_NOT_OK: Get of DTC status mask failed
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61121
 */
extern Std_ReturnType
    Dem_GetDTCStatusAvailabilityMask(uint8 ClientId, Dem_UdsStatusByteType* DTCStatusMask, Dem_DTCOriginType DTCOrigin);
#endif

#if (DEM_SUPPORT_GETSTATUSOFDTC_API == STD_ON)
/**
 * @brief         Gets the status of a DTC. For large configurations and DTC-calibration, the interface behavior
 *                can be asynchronous (splitting the DTC-search into segments). The DTCs of OBD Events Suppression
 *                shall be reported as Dem_WRONG_DTC.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTCStatus: This parameter receives the status information of the requested DTC. It follows the
 *                           format as defined in ISO14229-1 If the return value of the function call is other than
 *                           DEM_FILTERED_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: The status of the DTC is correctly provided in the DTCStatus parameter.
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Retrieving the DTC status is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @retval        DEM_NO_SUCH_ELEMENT: Selected DTC does not have an assigned DTC status.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61127
 */
extern Std_ReturnType Dem_GetStatusOfDTC(uint8 ClientId, uint8* DTCStatus);
#endif

#if (DEM_SUPPORT_GETSEVERITYOFDTC_API == STD_ON)
/**
 * @brief         Gets the severity of the requested DTC. For large configurations and DTC-calibration, the
 *                interface behavior can be asynchronous (splitting the DTC-search into segments).
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTCSeverity: This parameter contains the DTCSeverity according to ISO 14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: The DTC severity is correctly provided in the DTCSeverity parameter.
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Retrieving the DTC status is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @retval        DEM_NO_SUCH_ELEMENT: Selected DTC does not have an assigned DTC status.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61126
 */
extern Std_ReturnType Dem_GetSeverityOfDTC(Dem_DTCSeverityType* DTCSeverity, uint8 ClientId);

/**
 * @brief         Gets the functional unit of the requested DTC.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTCFunctionalUnit: Functional unit value of this DTC
 * @return        Std_ReturnType
 * @retval        E_OK: The DTC functional unit is provided in the parameter DTCFunctionalUnit.
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Retrieving the DTC status is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @retval        DEM_NO_SUCH_ELEMENT: Selected DTC does not have an assigned DTC status.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61124
 */
extern Std_ReturnType Dem_GetFunctionalUnitOfDTC(uint8 ClientId, uint8* DTCFunctionalUnit);
#endif

#if (DEM_SUPPORT_SETDTCFILTER_AND_GETNUMBER_API == STD_ON)
/**
 * @brief         Sets the DTC Filter. The server shall perform a bit-wise logical AND-ing operation between the
 *                parameter DTCStatusMask and the current UDS status in the server. In addition to the DTCStatus
 *                AvailabilityMask, the server shall return all DTCs for which the result of the AND-ing operation
 *                is non-zero [i.e. (statusOfDTC & DTCStatusMask) != 0]. The server shall process only the DTC
 *                Status bits that it is supporting. OBD Events Suppression shall be ignored for this computation.
 *                If no DTCs within the server match the masking criteria specified in the client request, no
 *                DTC or status information shall be provided following the DTCStatusAvailabilityMask byte in the
 *                positive response message
 *                (((statusOfDTC & DTCStatusMask) != 0) && ((severity & DTCSeverityMask) != 0)) == TRUE
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCStatusMask: Status-byte mask for DTC status-byte filtering Values: 0x00: Autosar-specific
 *                               value to deactivate the status-byte filtering (different meaning than in ISO 14229-1)
 *                               to report all supported DTCs (used for service 0x19 subfunctions 0x0A/0x15)
 *                               0x01..0xFF: Status-byte mask according to ISO 14229-1 DTCStatusMask (handed over by
 *                               Dcm from service request directly) to filter for DTCs with at least one status bit
 *                               set matching this status-byte mask
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC values for the sub-sequent API calls.
 *                           If passed value does not fit to Configuration, the DET error DEM_E_WRONG_CONFIGURATION
 *                           shall be reported, e.g. if DTCFormat "DEM_DTC_FORMAT_OBD" is passed, but OBD is not
 *                           supported per configuration.
 * @param[in]     DTCOrigin: If the Dem supports more than one event memory this parameter is used to select the
 *                           source memory the DTCs shall be read from. If passed value does not fit to Configuration,
 *                           the DET error DEM_E_WRONG_CONFIGURATION shall be reported.
 * @param[in]     FilterWithSeverity: This flag defines whether severity information (ref. to parameter below)
 *                                    shall be used for filtering. This is to allow for coexistence of DTCs with and
 *                                    without severity information. TRUE: severity information is used for filtering.
 *                                    FALSE: severity information is not used for filtering.
 * @param[in]     DTCSeverityMask: Contains the DTCSeverityMask according to ISO14229-1.
 * @param[in]     FilterForFaultDetectionCounter: This flag defines whether the fault detection counter
 *                                                information shall be used for filtering. This is to allow for
 *                                                coexistence of DTCs with and without fault detection counter
 *                                                information. If fault detection counter information is filter
 *                                                criteria, only those DTCs with a fault detection counter value
 *                                                between 1 and 0x7E shall be reported. Remark: If the event does not
 *                                                use the debouncing inside Dem, then the Dem must request this
 *                                                information via GetFaultDetectionCounter. TRUE: fault detection
 *                                                counter is used for filtering. FALSE: fault detection counter is
 *                                                not used for filtering.
 * @return        Std_ReturnType
 * @retval        E_OK: Filter was successfully set.
 * @retval        E_NOT_OK: Indicates a wrong DTCOrigin or DTCFormat
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61118
 */
extern Std_ReturnType Dem_SetDTCFilter(
    uint8               ClientId,
    uint8               DTCStatusMask,
    Dem_DTCFormatType   DTCFormat,
    Dem_DTCOriginType   DTCOrigin,
    boolean             FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean             FilterForFaultDetectionCounter);

/**
 * @brief         Gets the number of a filtered DTC.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    NumberOfFilteredDTC: The number of DTCs matching the defined status mask.
 * @return        Std_ReturnType
 * @retval        E_OK: Getting number of filtered DTCs was successful
 * @retval        E_NOT_OK: No DTC filter set
 * @retval        DEM_PENDING: Retrieving the DTC status is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61117
 */
extern Std_ReturnType Dem_GetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC);
#endif

#if (DEM_SUPPORT_GETNEXT_FILTEREDDTC_API == STD_ON)
/**
 * @brief         Gets the next filtered DTC matching the filter criteria. For UDS services, the interface has an
 *                asynchronous behavior, because a large number of DTCs has to be processed.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTC: Receives the DTC value in respective format of the filter returned by this function. If
 *                     the return value of the function is other than DEM_FILTERED_OK this parameter does not contain
 *                     valid data.
 * @param[out]    DTCStatus: This parameter receives the status information of the requested DTC. It follows the
 *                           format as defined in ISO14229-1 If the return value of the function call is other than
 *                           DEM_FILTERED_OK this parameter does not contain valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Returned next filtered element
 * @retval        E_NOT_OK: No DTC filter set
 * @retval        DEM_NO_SUCH_ELEMENT: No further element matching the filter criteria found
 * @retval        DEM_PENDING: The requested operation is currently in progress. The caller shall call this
 *                             function again at a later moment. Note that according to SWS_Dem_00653 this return
 *                             value is not always allowed.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61119
 */
extern Std_ReturnType Dem_GetNextFilteredDTC(uint8 ClientId, uint32* DTC, uint8* DTCStatus);
#endif

#if (DEM_SUPPORT_GETNEXT_FILTEREDDTCANDFDC_API == STD_ON)
/**
 * @brief         Gets the next filtered DTC and its associated Fault Detection Counter (FDC) matching the filter
 *                criteria. The interface has an asynchronous behavior, because a large number of DTCs has to be
 *                processed and the FDC might be received asynchronously from a SW-C, too.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTC: Receives the DTC value in respective format of the filter returned by this function. If
 *                     the return value of the function is other than DEM_FILTERED_OK this parameter does not contain
 *                     valid data.
 * @param[out]    DTCFaultDetectionCounter: This parameter receives the Fault Detection Counter information of the
 *                                          requested DTC. If the return value of the function call is other than
 *                                          DEM_FILTERED_OK this parameter does not contain valid data.
 *                                          -128dec...127dec PASSED...FAILED according to ISO 14229-1
 * @return        Std_ReturnType
 * @retval        E_OK: Returned next filtered element
 * @retval        E_NOT_OK: No DTC filter set
 * @retval        DEM_NO_SUCH_ELEMENT: No further element matching the filter criteria found
 * @retval        DEM_PENDING: The requested operation is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61120
 */
extern Std_ReturnType Dem_GetNextFilteredDTCAndFDC(uint8 ClientId, uint32* DTC, sint8* DTCFaultDetectionCounter);
#endif

#if (DEM_SUPPORT_GETNEXT_FILTEREDDTCANDSEVERITY_API == STD_ON)
/**
 * @brief         Gets the next filtered DTC and its associated Severity matching the filter criteria. The
 *                interface has an asynchronous behavior, because a large number of DTCs has to be processed.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTC: Receives the DTC value in respective format of the filter returned by this function. If
 *                     the return value of the function is other than DEM_FILTERED_OK this parameter does not contain
 *                     valid data.
 * @param[out]    DTCStatus: This parameter receives the status information of the requested DTC. It follows the
 *                           format as defined in ISO14229-1 If the return value of the function call is other than
 *                           DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]    DTCSeverity: This parameter contains the DTCSeverity according to ISO 14229-1.
 * @param[out]    DTCFunctionalUnit: Functional unit value of this DTC
 * @return        Std_ReturnType
 * @retval        E_OK: Returned next filtered element
 * @retval        E_NOT_OK: No DTC filter set
 * @retval        DEM_NO_SUCH_ELEMENT: No further element matching the filter criteria found
 * @retval        DEM_PENDING: The requested operation is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61115
 */
extern Std_ReturnType Dem_GetNextFilteredDTCAndSeverity(
    uint8                ClientId,
    uint32*              DTC,
    uint8*               DTCStatus,
    Dem_DTCSeverityType* DTCSeverity,
    uint8*               DTCFunctionalUnit);
#endif

#if (DEM_SUPPORT_SETFREEZEFRAMERECORD_AND_GET_API == STD_ON)
/**
 * @brief         Sets a freeze frame record filter.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCFormat: Defines the output-format of the requested DTC values
 * @return        Std_ReturnType
 * @retval        E_OK: Filter is accepted
 * @retval        E_NOT_OK: Wrong filter selected
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61116
 */
extern Std_ReturnType Dem_SetFreezeFrameRecordFilter(uint8 ClientId, Dem_DTCFormatType DTCFormat);

/**
 * @brief         Gets the next freeze frame record number and its associated DTC stored in the event memory. The
 *                interface has an asynchronous behavior, because NvRAM access might be required.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTC: DTC Receives the DTC value in respective format of the filter returned by this function.
 *                     If the return value of the function is other than E_OK this parameter does not contain valid
 *                     data.
 * @param[out]    RecordNumber: Freeze frame record number of the reported DTC (relative addressing). If the
 *                              return value of the function is other than E_OK this parameter does not contain
 *                              valid data.
 * @return        Std_ReturnType
 * @retval        E_OK: Returned next filtered element
 * @retval        DEM_NO_SUCH_ELEMENT: No further element (matching the filter criteria) found
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later. Only used by asynchronous interfaces.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61114
 */
extern Std_ReturnType Dem_GetNextFilteredRecord(uint8 ClientId, uint32* DTC, uint8* RecordNumber);
#endif

#if (DEM_SUPPORT_GETDTCOCCURRENCETIME_API == STD_ON)
/**
 * @brief         Gets the DTC by occurrence time. There is no explicit parameter for the DTC-origin as the origin
 *                always is DEM_DTC_ORIGIN_PRIMARY_MEMORY.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     DTCRequest: This parameter defines the request type of the DTC.
 * @param[out]    DTC: DTC Receives the DTC value in respective format of the filter returned by this function.
 *                     If the return value of the function is other than E_OK this parameter does not contain valid
 *                     data.
 * @return        Std_ReturnType
 * @retval        E_OK: Get of DTC was successful
 * @retval        E_NOT_OK: The call was not successful
 * @retval        DEM_NO_SUCH_ELEMENT: The requested element is not stored
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61113
 */
extern Std_ReturnType Dem_GetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC);
#endif

#if (DEM_SUPPORT_DISABLE_ENABLE_DTCRECORD_UPDATE_API == STD_ON)
/**
 * @brief         Disables the event memory update of a specific DTC (only one at one time).
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: Event memory update successfully disabled
 * @retval        E_NOT_OK: No DTC selected
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist or a group of DTC was selected
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @retval        DEM_PENDING: Disabling the DTC record update is currently in progress. The caller shall call
 *                             this function again at a later moment.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61109
 */
extern Std_ReturnType Dem_DisableDTCRecordUpdate(uint8 ClientId);

/**
 * @brief         Enables the event memory update of the DTC disabled by Dem_DisableDTCRecordUpdate() before.
 *                Note: As the call to Dem_EnableDTCRecordUpdate is the last action in the sequence of
 *                disabling/enabling a DTC, the caller is not interested in the return value. Therefore E_OK
 *                should be returned even if the DTC is enabled asynchronously.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: DTC record successfully updated.
 * @retval        E_NOT_OK: No DTC selected.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61110
 */
extern Std_ReturnType Dem_EnableDTCRecordUpdate(uint8 ClientId);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API == STD_ON)
/**
 * @brief         Gets the size of Extended Data Record by DTC selected by the call of Dem_SelectExtended DataRecord.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    SizeOfExtendedDataRecord: Size of the requested extended data record(s) including record number.
 *                                          The format for a single ExtendedDataRecord is:
 *                                          {RecordNumber, data[1], ..., data[N]}
 * @return        Std_ReturnType
 * @retval        E_OK: Size returned successfully
 * @retval        E_NOT_OK: Selection function is not called.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_WRONG_DTC: DTC value not existing
 * @retval        DEM_WRONG_DTCORIGIN: Wrong DTC origin
 * @retval        DEM_NO_SUCH_ELEMENT: Record number is not supported by configuration and therefore invalid
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61108
 */
extern Std_ReturnType Dem_GetSizeOfExtendedDataRecordSelection(uint8 ClientId, uint32* SizeOfExtendedDataRecord);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API == STD_ON)
/**
 * @brief         Gets the size of Extended Data Record by DTC selected by the call of Dem_SelectExtended DataRecord.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 * @return        Std_ReturnType
 * @retval        E_OK: Size returned successfully
 * @retval        E_NOT_OK: Selection function is not called.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_WRONG_DTC: DTC value not existing
 * @retval        DEM_WRONG_DTCORIGIN: Wrong DTC origin
 * @retval        DEM_NO_SUCH_ELEMENT: Record number is not supported by configuration and therefore invalid
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61107
 */
extern Std_ReturnType Dem_GetSizeOfFreezeFrameSelection(uint8 ClientId, uint32* SizeOfFreezeFrame);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API == STD_ON)
/**
 * @brief         Gets extended data record for the DTC selected by Dem_SelectExtendedDataRecord. The function
 *                stores the data in the provided DestBuffer.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            extended data record shall be written to. The format is:
 *                            {ExtendedDataRecordNumber, data[0], data[1], ..., data[n]}
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written
 *                         data bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Size and buffer successfully returned.
 * @retval        E_NOT_OK: Selection function is not called.
 * @retval        DEM_BUFFER_TOO_SMALL: provided buffer size too small.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_WRONG_DTC: DTC value not existing
 * @retval        DEM_WRONG_DTCORIGIN: Wrong DTC origin
 * @retval        DEM_NO_SUCH_ELEMENT: Found no (further) element matching the filter criteria
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61106
 */
extern Std_ReturnType Dem_GetNextExtendedDataRecord(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API == STD_ON)
/**
 * @brief         Gets extended data record for the DTC selected by Dem_SelectExtendedDataRecord. The function
 *                stores the data in the provided DestBuffer.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the
 *                            freeze frame data record shall be written to. The format is:
 *                            {RecordNumber, NumOfDIDs, DID[1], data[1], ..., DID[N], data[N]}
 * @param[inout]  BufSize: When the function is called this parameter contains the maximum number of data bytes
 *                         that can be written to the buffer. The function returns the actual number of written
 *                         data bytes in this parameter.
 * @return        Std_ReturnType
 * @retval        E_OK: Size and buffer successfully returned.
 * @retval        E_NOT_OK: Selection function is not called.
 * @retval        DEM_BUFFER_TOO_SMALL: provided buffer size too small.
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later.
 * @retval        DEM_WRONG_DTC: DTC value not existing
 * @retval        DEM_WRONG_DTCORIGIN: Wrong DTC origin
 * @retval        DEM_NO_SUCH_ELEMENT: Found no (further) element matching the filter criteria
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61102
 */
extern Std_ReturnType Dem_GetNextFreezeFrameData(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_EXTENDEDDATARECORD_API == STD_ON)
/**
 * @brief         Sets the filter to be used by Dem_GetNextExtendedDataRecord and
 *                Dem_GetSizeOfExtendedDataRecordSelection.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     ExtendedDataNumber: Identification/Number of requested extended data record. For primary fault
 *                                    memory the value of 0xFE is explictely allowed to request all regulated
 *                                    emissions OBD DTC extended data records. The value of 0xFF is explicitely
 *                                    allowed in all fault memories to retrieve the data of all extended datal
 *                                    records.
 * @return        Std_ReturnType
 * @retval        E_OK: Extended data record successfully selected.
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @retval        DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call
 *                             this function again at a later moment.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61106
 */
extern Std_ReturnType Dem_SelectExtendedDataRecord(uint8 ClientId, uint8 ExtendedDataNumber);
#endif

#if (DEM_SUPPORT_SELECT_GETSIZE_GETNEXT_FREEZEFRAMEDATA_API == STD_ON)
/**
 * @brief         Sets the filter to be used by Dem_GetNextFreezeFrameData and Dem_GetSizeOfFreezeFrame Selection.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]     RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF
 *                              is a placeholder referencing all snapshot records of the addressed DTC. The value
 *                              0x00 indicates the DTC-specific WWH-OBD snapshot record.
 * @return        Std_ReturnType
 * @retval        E_OK: Freeze frame data successfully selected.
 * @retval        DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @retval        DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @retval        DEM_PENDING: Selecting the freeze frame is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @retval        DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this
 *                          client is currently in progress.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61101
 */
extern Std_ReturnType Dem_SelectFreezeFrameData(uint8 ClientId, uint8 RecordNumber);
#endif

#if (DEM_SUPPORT_SETFREEZEFRAMERECORD_AND_GET_API == STD_ON)
/**
 * @brief         This function returns the number of all freeze frame records currently stored in the primary
 *                event memory
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    NumberOfFilteredRecords: Number of all freeze frame records currently stored in the primary
 *                                         event memory.
 * @return        Std_ReturnType
 * @retval        E_OK: Returned correctly the number of freeze frame records
 * @retval        DEM_PENDING: The requested value is calculated asynchronously and currently not available. The
 *                             caller can retry later
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61103
 */
extern Std_ReturnType Dem_GetNumberOfFreezeFrameRecords(uint8 ClientId, uint16* NumberOfFilteredRecords);
#endif

#if (STD_ON == DCM_UDS_0X85)
/**
 * @brief         Disables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: Returned next filtered element
 * @retval        DEM_PENDING: The requested operation is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61100
 */
extern Std_ReturnType Dem_DisableDTCSetting(uint8 ClientId);

/**
 * @brief         (Re)-Enables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @return        Std_ReturnType
 * @retval        E_OK: The operation was successful;
 * @retval        DEM_PENDING: The started operation is currently in progress. The caller shall call this
 *                             function again at a later moment.
 * @reentrant     Reentrant for different ClientIds, non reentrant for the same ClientId.
 * @synchronous   FALSE
 * @trace         CPD-61105
 */
extern Std_ReturnType Dem_EnableDTCSetting(uint8 ClientId);
#endif

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_OBD_READINESS_GROUP_HANDING == STD_ON)
/**
 * @brief         Service for reporting the event as disabled to the Dem for the PID $41 computation. API is
 *                needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     EventId: Identification of an event by assigned EventId.
 * @return        Std_ReturnType
 * @retval        E_OK: Set of event to disabled was successfull.
 * @retval        E_NOT_OK: Set of event disabled failed
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61066
 */
extern Std_ReturnType Dem_SetEventDisabled(Dem_EventIdType EventId);
#endif

#if (DEM_RATIO_NUMBER > 0u)
/**
 * @brief         Service for reporting that faults are possibly found because all conditions are fullfilled. API
 *                is needed in OBD-relevant ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     RatioID: Ratio Identifier reporting that a respective monitor could have found a fault only
 *                         used when interface option "API" is selected
 * @return        Std_ReturnType
 * @retval        E_OK: Report of IUMPR result was successfully reported
 * @reentrant     Reentrant for different EventIds. Non reentrant for the same EventId.
 * @synchronous   TRUE
 * @trace         CPD-61064
 */
extern Std_ReturnType Dem_RepIUMPRFaultDetect(Dem_RatioIdType RatioID);

/**
 * @brief         In order to communicate the status of the (additional) denominator conditions among the OBD
 *                relevant ECUs, the API is used to forward the condition status to a Dem of a particular ECU.
 *                API is needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     ConditionId: Identification of a IUMPR denominator condition ID (General Denominator, Cold
 *                             start, EVAP, 500mi).
 * @param[in]     ConditionStatus: Status of the IUMPR denominator condition (Not-reached, reached, not
 *                                 reachable / inhibited)
 * @return        Std_ReturnType
 * @retval        E_OK: Set of IUMPR denominator condition was successful
 * @retval        E_NOT_OK: Set of IUMPR denominator condition failed or could not be accepted.
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61065
 */
extern Std_ReturnType
    Dem_SetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus);

/**
 * @brief         In order to communicate the status of the (additional) denominator conditions among the OBD
 *                relevant ECUs, the API is used to forward the condition status to a Dem of a particular ECU.
 *                API is needed in OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     ConditionId: Identification of a IUMPR denominator condition ID (General Denominator, Cold
 *                             start, EVAP, 500mi).
 * @param[in]     ConditionStatus: Status of the IUMPR denominator condition (Not-reached, reached, not
 *                                 reachable / inhibited)
 * @return        Std_ReturnType
 * @retval        E_OK: Get of IUMPR denominator condition status was successful
 * @retval        E_NOT_OK: Get of condition status failed
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61067
 */
extern Std_ReturnType
    Dem_GetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType* ConditionStatus);

/**
 * @brief         Service is used to release a denominator of a specific monitor. API is needed in OBD-relevant
 *                ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     RatioID: Ratio Identifier reporting that specific denominator is released (for physical reasons
 *                         - e.g. temperature conditions or minimum activity)
 * @return        Std_ReturnType
 * @retval        E_OK report of IUMPR denominator status was successfully reported
 * @retval        E_NOT_OK: Report of IUMPR denominator status was not successfully reported
 * @reentrant     Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61062
 */
extern Std_ReturnType Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID);
#endif

#if (DEM_USER_SUPPORT_SET_PTO_API == STD_ON)
/**
 * @brief         API is needed in OBD-relevant ECUs only API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PtoStatus: sets the status of the PTO (TRUE==active; FALSE==inactive)
 * @return        Std_ReturnType
 * @retval        E_OK: Returns E_OK when the new PTO-status has been adopted by the Dem
 * @retval        E_NOT_OK: Returns E_NOT_OK in all other cases.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61063
 */
extern Std_ReturnType Dem_SetPtoStatus(boolean PtoStatus);
#endif

/**
 * @brief         Service to report the value of PID $01 computed by the Dem. API is needed in OBD relevant ECUs only
 * @param[out]    PID01value: Buffer containing the contents of PID $01 computed by the Dem.
 *                            The buffer is provided by the application with the size of 4 bytes.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61061
 */
extern Std_ReturnType Dem_ReadDataOfPID01(uint8* PID01value);

#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) && (DEM_OBD_CENTRALIZED_PID21_HANDING == STD_ON))
/**
 * @brief         Service to get the value of PID $21 from the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral/DemGeneral OBD.DemOBDCentralizedPID21Handling)} == true) &&
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} == DEM_OBD_MASTER_ECU)
 * @param[out]    PID21value: Content of PID $21 as raw hex value.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61060
 */
extern Std_ReturnType Dem_GetDataOfPID21(uint8* PID21value);
#endif

#if (DEM_USER_SUPPORT_SET_PID_API == STD_ON)
/**
 * @brief         Service to set the value of PID $21 in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PID21value: Buffer containing the contents of PID $21. The buffer is provided by the Dcm
 *                            with the appropriate size, i.e. during configuration, the Dcm identifies the required
 *                            size from the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61059
 */
extern Std_ReturnType Dem_SetDataOfPID21(const uint8* PID21value);

/**
 * @brief         Service to set the value of PID $31 in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PID31value: Buffer containing the contents of PID $31. The buffer is provided by the Dcm
 *                            with the appropriate size, i.e. during configuration, the Dcm identifies the required
 *                            size from the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61058
 */
extern Std_ReturnType Dem_SetDataOfPID31(const uint8* PID31value);

/**
 * @brief         Service to set the value of PID $4D in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PID4Dvalue: Buffer containing the contents of PID $4D. The buffer is provided by the Dcm
 *                            with the appropriate size, i.e. during configuration, the Dcm identifies the required
 *                            size from the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61057
 */
extern Std_ReturnType Dem_SetDataOfPID4D(const uint8* PID4Dvalue);

/**
 * @brief         Service to set the value of PID $4E in the Dem by a software component. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     PID4Evalue: Buffer containing the contents of PID $4E. The buffer is provided by the Dcm
 *                            with the appropriate size, i.e. during configuration, the Dcm identifies the required
 *                            size from the largest PID in order to configure a PIDBuffer.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61056
 */
extern Std_ReturnType Dem_SetDataOfPID4E(const uint8* PID4Evalue);
#endif
#endif

/**
 * @brief         Returns the qualification state of the dependent operation cycle. API Availability: This API
 *                will be available only if any of the
 *                ({ecuc(Dem/DemGeneral/DemOperationCycle.DemLeadingCycleRef)} != NULL)
 * @param[in]     OperationCycleId: Identification of a configured DemOperationCycle.
 * @param[out]    isQualified: TRUE: The dependent operation cylcle is qualified. FALSE: The qualification
 *                             conditions of the dependent operation cylcle have not been met.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61055
 */
extern Std_ReturnType Dem_GetCycleQualified(uint8 OperationCycleId, boolean* isQualified);

/**
 * @brief         Sets a dependent operation cycle as qualified, so it may be processed along with its leading cycle.
 * @param[in]     OperationCycleId: Identification of a configured DemOperationCycle.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned.
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-61054
 */
extern Std_ReturnType Dem_SetCycleQualified(uint8 OperationCycleId);

/* PRQA S 1707 -- */
/* PRQA S 1512, 1513 -- */
#if (DEM_SUPPORT_GETDTCSERVERITYAVAILABILITYMASK_API == STD_ON)
/**
 * @brief         Gets the DTC Severity availability mask.
 * @param[in]     ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]    DTCSeverityMask: DTCSeverityMask The value DTCSeverityMask indicates the supported DTC
 *                                 severity bits from the Dem. All supported information is indicated by setting the
 *                                 corresponding status bit to 1. See ISO14229-1.
 * @return        Std_ReturnType
 * @retval        E_OK: Get of DTC severity mask was successful
 * @retval        E_NOT_OK: Get of DTC severity mask failed
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61052
 */
extern Std_ReturnType Dem_GetDTCSeverityAvailabilityMask(uint8 ClientId, Dem_DTCSeverityType* DTCSeverityMask);
#endif

#if (DEM_OBDII_SUPPORT == STD_ON)
#if (DEM_DTR_NUMBER > 0u)
/**
 * @brief         Service to report the value of the B1 counter computed by the Dem. API is needed in WWH-OBD
 *                relevant ECUs only
 * @param[out]    B1Counter: Buffer containing the B1 counter. The buffer is provided by the application with
 *                           the size of 2 bytes.
 * @return        Std_ReturnType
 * @retval        Always E_OK is returned, as E_NOT_OK will never appear.
 * @reentrant     Re-entrant for different ClientIDs, Non re-entrant for same ClientId.
 * @synchronous   TRUE
 * @trace         CPD-61051
 */
extern Std_ReturnType Dem_GetB1Counter(uint16* B1Counter);
#endif
#endif

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
/**
 * @brief         Reports a DTR result with lower and upper limit. The internal eventstatus serves as master
 *                whether the DTR values are forwarded or ignored, also taking the DTRUpdateKind into account.
 *                The EventId that is related to the DTR is assigned per configuration (and derived from
 *                ServiceNeeds). Processing takes enable/storage conditions into account. API is needed in
 *                OBD-relevant ECUs only. API Availability: This API will be available only if
 *                ({ecuc(Dem/DemGeneral.DemOBDSupport)} != DEM_OBD_NO_OBD_SUPPORT)
 * @param[in]     DTRId: Identification of a DTR element by assigned DTRId.
 * @param[in]     TestResult: Test result of DTR
 * @param[in]     LowerLimit: Lower limit of DTR
 * @param[in]     UpperLimit: Upper limit of DTR
 * @param[in]     Ctrlval: Control value of the DTR to support its interpretation Dem-internally.
 * @return        Std_ReturnType
 * @retval        E_OK: Report of DTR result successful
 * @retval        E_NOT_OK: Report of DTR result failed
 * @reentrant     Reentrant for different DTRIds. Non reentrant for the same DTRId.
 * @synchronous   TRUE
 * @trace         CPD-61053
 */
extern Std_ReturnType
    Dem_SetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval);
#endif

/**
 * @brief         Processes all not event based Dem internal functions.
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-61037
 */
/* PRQA S 3449,3451 ++ */ /* VL_QAC_MultiDeclaration,VL_QAC_MultiDeclaration */
extern void Dem_MainFunction(void);
/* PRQA S 3449,3451 -- */

/* --------------------------------------- Multi-partition custom Global Functions ---------------------------------- */
#if (DEM_EVENT_PARTITION_NUMBER > 1u)
/**
 * @brief         Dem_SatellitePreInit is called directly for pre-initialization in case of single partition, or
 *                executed by the corresponding partition in case of multiple partitions.
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68116
 */
extern void Dem_SatellitePreInit(void);

/**
 * @brief         Dem_SatelliteInit is called directly for initialization in case of single partition, or
 *                executed by the corresponding partition in case of multiple partitions.
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68118
 */
extern void Dem_SatelliteInit(void);

/**
 * @brief         Cyclic DemSatellite timer task.
 *                Processes all time debounce of satellite
 * @reentrant     Non reentrant
 * @synchronous   TRUE
 * @trace         CPD-68120
 */
extern void Dem_SatelliteMainFunction(void);
#endif

#define Dem_ReportErrorStatus \
    Dem_SetEventStatus /**< Add Dem_ReportErrorStatus macro definition for compatibility with older versions */

#ifdef __cplusplus
}
#endif
#endif
