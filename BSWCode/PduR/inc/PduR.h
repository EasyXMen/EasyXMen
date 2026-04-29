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
 **  @file               : PduR.c
 **  @author             : zhengfei.li
 **  @date               : 2020/06/22
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of PduR
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 20200622    zhengfei.li    Initial version (Upgrade according to the R19_11 standards)
 *  V02.00.01 20211103    zhengfei.li    add "PDUR_" to some Macro Name
 *  V02.01.00 20211217    zhengfei.li    PB/PC configuration parameter split
 *  V02.02.00 20231215    tong.zhao      Upgrade according to the R19_11 standards(Project regression)
 *                                       (jira:CPT-7994)(CPD-33702)
 *  V02.02.01 20240109    xiaojian.liang Support TP Gateway 1:n, fifo and forwarding to up layers
 *  V02.02.02 20240319    tong.zhao      PduR_DestPduState can't recovery in gateway when dest bus-off(no
 *                                       TxConfirmation) (jira:CPT-8495)
 *  V02.02.03 20240522    tong.zhao      Modify compilation errors when det is disable.
 *                                       (jira:CPT-9076)
 *  V02.03.00 20240105    tong.zhao      Muliticore adapt.
 *  V02.03.01 20240529    tong.zhao      PduR_DestPduState can't recovery,when Tp GW data is received again,
 *                                       before receiving txconfirmation.
 *  V02.03.02 20240627    tong.zhao      Optimize flash
 *  V02.03.03 20240723    tong.zhao      PduR can not forward tx confirmation to upper layer in TriggerTransmit.
 *  V02.04.00 20241031    tong.zhao      Upgrade according to the R23_11 standards
 *  V02.04.01 20251011    tong.zhao      In Tp Gateway, the Dest state has not been successfully set yet, while the
 *                                       lower layer starts copy tx data, which leads to data loss in the gateway.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:PduR<br>
    RuleSorce:puhua_rules-2024.12.rcf

    \li VL_PduR_IndirectUse
      Reason:This pointer variable can be indirectly used in multiple functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_Logical
      Reason:Necessary logical design for while(1) and have "break" to stop.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_FuncPtrNonConst
      Reason:To adapt to multi-partition and non-partition condition.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_ConfigDesign
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_PduR_5016
      Reason: Parameter or return passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_0793
      Reason: Configuration data design is required.
      Risk: None
      Prevention: None

    \li VL_PduR_0791
      Reason: Configuration data design is required.
      Risk: None
      Prevention: None

    \li VL_PduR_2812
      Reason: Configuration data design is required.
      Risk: None
      Prevention: None

    \li VL_QAC_3313,VL_QAC_1336,VL_QAC_3332,VL_QAC_1252
      Reason: To resolve the compilation warnings.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_1334
      Reason: Only parameter name is different.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_PduR_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_0770
      Reason: Loop back retry logic requires early continuation to simplify error
              handling and avoid deep nesting.
      Risk: None control flow is well-defined and reviewed.
      Prevention: Verified by code review and unit tests; behaviour unchanged.

    \li VL_PduR_1505
      Reason: In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_MTR_PduR_STCAL
      Reason: The software structure is defined by the AUTOSAR standard. The standard compliance vote is above the
  measurement threshold. In addition, a typical way to reduce STCAL is to deepen function nesting, which increases the
  use of the call stack and runtime.
      Risk: Because of fanning out to many functions, understandability and testability
  may become overly complex.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_PduR_STMIF
      Reason: Functions handle specific tasks in the overall component behavior; Tasks have different scenarios to
  cover, depending on local conditions - this leads to deep nesting of control structures. Since there is more common
  code than different code, a higher nesting level is acceptable to keep the code.
      Risk: The code is hard to maintain.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_PduR_STM19
      Reason: it is not allowed for a function to have multiple exit points in principle, but it can help improve code
  performance in special case.
      Risk: The difficulty of function debugging has increased.
      Prevention: Fully test ensures
  correct functionality.

    \li VL_MTR_PduR_STCYC
      Reason: No separation of functionality into sub-functions because there is a higher requirement to minimize the
  stack and runtime usage applied to the code.
      Risk: Understandability and testability can become too complex.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_PduR_STVAR
      Reason: The function prototype is defined in AUTOSAR standard.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

#ifndef PDUR_H_
#define PDUR_H_

/* =================================================== inclusions =================================================== */
#include "PduR_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/** Published information */
#define PDUR_MODULE_ID                   (51u)
#define PDUR_VENDOR_ID                   (62u)
#define PDUR_AR_RELEASE_MAJOR_VERSION    (4u)
#define PDUR_AR_RELEASE_MINOR_VERSION    (9u)
#define PDUR_AR_RELEASE_REVISION_VERSION (0u)
#define PDUR_SW_MAJOR_VERSION            (2u)
#define PDUR_SW_MINOR_VERSION            (4u)
#define PDUR_SW_PATCH_VERSION            (1u)

/* ===================================================== macros ===================================================== */
#if (STD_OFF == PDUR_ZERO_COST_OPERATION)

/** General function id */
#define PDUR_TPCOPYTXDATA_ID       ((uint8)0x43u)
#define PDUR_TPRXINDICATION_ID     ((uint8)0x45u)
#define PDUR_TRANSMIT_ID           ((uint8)0x49u)
#define PDUR_CANCELTRANSMIT_ID     ((uint8)0x4Au)
#define PDUR_CANCELRECEIVE_ID      ((uint8)0x4Cu)
#define PDUR_GETVERSIONINFO_ID     ((uint8)0xF1u)
#define PDUR_GETCONFIGURATIONID_ID ((uint8)0xF2u)
#define PDUR_ENABLEROUTING_ID      ((uint8)0xF3u)

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
/** Error Classification */
#define PDUR_E_INIT_FAILED                   ((uint8)0x00u)
#define PDUR_E_UNINIT                        ((uint8)0x01u)
#define PDUR_E_PDU_ID_INVALID                ((uint8)0x02u)
#define PDUR_E_ROUTING_PATH_GROUP_ID_INVALID ((uint8)0x08u)
#define PDUR_E_PARAM_POINTER                 ((uint8)0x09u)
#endif

/** General function id */
#define PDUR_INIT_ID ((uint8)0xF0u)

/** The API and DET Ref to Det_ReportRuntimeError*/
/** General function id */
#define PDUR_IFTXCONFIRMATION_ID   ((uint8)0x40u)
#define PDUR_IFTRIGGERTRANSMIT_ID  ((uint8)0x41u)
#define PDUR_IFRXINDICATION_ID     ((uint8)0x42u)
#define PDUR_TPCOPYRXDATA_ID       ((uint8)0x44u)
#define PDUR_TPSTARTOFRECEPTION_ID ((uint8)0x46u)
#define PDUR_TPTXCONFIRMATION_ID   ((uint8)0x48u)
#define PDUR_DISABLEROUTING_ID     ((uint8)0xF4u)

/** Error Classification */
#define PDUR_E_TP_GW_TX_REQ_REJECTED (0x03u)
#define PDUR_E_PDU_INSTANCES_LOST    (0x0Au)
#define PDUR_E_NO_BUFFER             (0x0Bu)

/** the Private Macro Definition */
#define PDUR_UNUSED_UINT8  (0xFFu)
#define PDUR_UNUSED_UINT16 (0xFFFFu)
#define PDUR_UNUSED_UINT32 (0xFFFFFFFFu)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**
 * @brief PduR PB Configuration Run Time point parameter
 */
extern const PduR_PBConfigType* PduR_ConfigStd;

/* ========================================= external function declarations ========================================= */
/* General functions provided by the PDU Router */

/**
 *  @brief         Initializes the PDU Router
 *  @param[in]     ConfigPtr: Pointer to Post build configuration data.
 *  @reentrant     FALSE
 *  @synchronous   TRUE
 *  @trace         CPD-56794
 */
void PduR_Init(const PduR_PBConfigType* ConfigPtr);

#if (STD_ON == PDUR_VERSION_INFO_API)
/**
 *  @brief         Returns the version information of this module.
 *  @param[out]    versionInfo   : Pointer to where to store the version information of this module.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56795
 */
void PduR_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif

/**
 *  @brief         Returns the unique identifier of the post-build time configuration of the PDU Router
 *  @return        PduR_PBConfigIdType : Identifier of the post-build time configuration
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @return        PduR_PBConfigIdType
 *  @trace         CPD-56796
 */
PduR_PBConfigIdType PduR_GetConfigurationId(void);

/**
 *  @brief         Enables a routing path group.
 *  @param[in]     id : Identification of the routing path group. Routing path groups are defined in the PDU router
 *                      configuration.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56797
 */
void PduR_EnableRouting(PduR_RoutingPathGroupIdType id);

/**
 *  @brief         Disables a routing path group.
 *  @param[in]     id         : Identification of the routing path group. Routing path groups are defined in the PDU
 *                              router configuration.
 *  @param[in]     initialize : true  : initialize single buffers to the default value
 *                              false : retain current value of single buffers
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56798
 */
void PduR_DisableRouting(PduR_RoutingPathGroupIdType id, boolean initialize);

/* Configurable interfaces definitions for interaction with upper layer module */

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 *  @brief         Requests transmission of a PDU.
 *  @param[in]     TxPduId    : Identifier of the PDU to be transmitted
 *  @param[in]     PduInfoPtr : Length of and pointer to the PDU data and pointer to MetaData.
 *  @return        Std_ReturnType
 *  @retval        E_OK     : Transmit request has been accepted.
 *  @retval        E_NOT_OK : Transmit request has not been accepted.
 *  @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 *  @synchronous   TRUE
 *  @trace         CPD-56799
 */
Std_ReturnType PduR_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
/**
 *  @brief         Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
 *  @param[in]     TxPduId    : Identifier of the PDU to be transmitted
 *  @return        Std_ReturnType
 *  @retval        E_OK     : Cancellation was executed successfully by the destination module.
 *  @retval        E_NOT_OK : Cancellation was rejected by the destination module.
 *  @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 *  @synchronous   TRUE
 *  @trace         CPD-56800
 */
Std_ReturnType PduR_CancelTransmit(PduIdType TxPduId);
#endif

#if (PDUR_CANCEL_RECEIVE == STD_ON)
/**
 *  @brief         Requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module.
 *  @param[in]     RxPduId    : Identification of the PDU to be cancelled.
 *  @return        Std_ReturnType
 *  @retval        E_OK     : Cancellation was executed successfully by the destination module.
 *  @retval        E_NOT_OK : Cancellation was rejected by the destination module.
 *  @reentrant     FALSE
 *  @synchronous   TRUE
 *  @trace         CPD-56801
 */
Std_ReturnType PduR_CancelReceive(PduIdType RxPduId);
#endif

/* Configurable interfaces definitions for lower layer communication interface module interaction */

#if (PDUR_RX_INDICATION == STD_ON)
/**
 *  @brief         Indication of a received PDU from a lower layer communication interface module.
 *  @param[in]     RxPduId    : ID of the received PDU.
 *  @param[in]     PduInfoPtr : Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *                              containing the PDU, and the MetaData related to this PDU.
 *  @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId
 *  @synchronous   TRUE
 *  @trace         CPD-56802
 */
void PduR_IfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
#endif

#if (PDUR_TX_CONFIRMATION == STD_ON)
/**
 *  @brief         The lower layer communication interface module confirms the transmission of a PDU, or the failure to
 *                 transmit a PDU.
 *  @param[in]     TxPduId : ID of the PDU that has been transmitted.
 *  @param[in]     result : E_OK     : The PDU was transmitted.
 *                          E_NOT_OK : Transmission of the PDU failed.
 *  @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId
 *  @synchronous   TRUE
 *  @trace         CPD-56803
 */
void PduR_IfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
#endif

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
/**
 *  @brief         Within this API, the upper layer module (called module) shall check whether the available data fits
 *                 into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy its data into the
 *                 buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 *                 PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 *  @param[in]     TxPduId    : ID of the SDU that is requested to be transmitted.
 *  @param[inout]  PduInfoPtr : Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied,
 *                              and the available buffer size in SduLengh. On return, the service will indicate the
 *                              length of the copied SDU data in SduLength.
 *  @return        Std_ReturnType
 *  @retval        E_OK     : SDU has been copied and SduLength indicates the number of copied bytes.
 *  @retval        E_NOT_OK : No SDU data has been copied. PduInfoPtr must not be used since it may contain a NULL
 * pointer or point to invalid data.
 *  @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId
 *  @synchronous   TRUE
 *  @trace         CPD-56804
 */
Std_ReturnType PduR_IfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#endif

/* Configurable interfaces definitions for lower layer transport protocol module interaction */

#if (PDUR_TP_COPYRXDATA == STD_ON)
/**
 *  @brief         This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
 *                 Each call to this function provides the next part of the I-PDU data. The size of the remaining buffer
 *                 is written to the position indicated by bufferSizePtr.
 *  @param[in]     id            : Identification of the received I-PDU.
 *  @param[in]     info          : Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                 (SduLength). An SduLength of 0 can be used to query the current amount of available
 *                                 buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR.
 *  @param[out]    bufferSizePtr : Available receive buffer after data has been copied.
 *  @return        BufReq_ReturnType
 *  @retval        BUFREQ_OK       : Data copied successfully
 *  @retval        BUFREQ_E_NOT_OK : Data was not copied because an error occurred.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56805
 */
BufReq_ReturnType PduR_TpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);
#endif

#if (PDUR_TP_RXINDICATION == STD_ON)
/**
 *  @brief         Called after an I-PDU has been received via the TP API, the result indicates whether the transmission
 *                 was successful or not.
 *  @param[in]     id     : Identification of the received I-PDU.
 *  @param[in]     result : E_OK     : The PDU was received.
 *                          E_NOT_OK : Reception of the PDU failed.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56806
 */
void PduR_TpRxIndication(PduIdType id, Std_ReturnType result);
#endif

#if (PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON)
/**
 *  @brief         This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into
 *                 multiple N-PDUs (FF with one or more following CFs) or might consist of a single N-PDU (SF). The
 *                 service shall provide the currently available maximum buffer size when invoked with TpSdu Length
 *                 equal to 0
 *  @param[in]     id            : Identification of the I-PDU.
 *  @param[in]     info          : Pointer to a PduInfoType structure containing the payload data (without protocol
 *                                 information) and payload length of the first frame or single frame of a transport
 *                                 protocol I-PDU reception, and the MetaData related to this PDU. If neither
 *                                 first/single frame data nor MetaData are available, this parameter is set to
 *                                 NULL_PTR.
 *  @param[in]     TpSduLength   : Total length of the N-SDU to be received.
 *  @param[out]    bufferSizePtr : Available receive buffer in the receiving module. This parameter will be used to
 *                                 compute the Block Size (BS) in the transport protocol module.
 *  @return        BufReq_ReturnType
 *  @retval        BUFREQ_OK       : Connection has been accepted. bufferSizePtr indicates the available receive buffer;
 *                                   reception is continued. If no buffer of the requested size is available, a receive
 *                                   buffer size of 0 shall be indicated by bufferSizePtr.
 *  @retval        BUFREQ_E_NOT_OK : Connection has been rejected; reception is aborted. bufferSizePtr remains
 *                                   unchanged.
 *  @retval        BUFREQ_E_OVFL   : No buffer of the required length can be provided; reception is aborted.
 *                                   bufferSizePtr remains unchanged.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56807
 */
BufReq_ReturnType PduR_TpStartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);
#endif

#if (PDUR_TP_COPYTXDATA == STD_ON)
/**
 *  @brief         This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this
 *                 function provides the next part of the I-PDU data unless retry->TpDataState is TP_DATARETRY. In this
 *                 case the function restarts to copy the data beginning at the offset from the current position
 *                 indicated by retry->TxTpDataCnt. The size of the remaining data is written to the position indicated
 *                 by availableDataPtr.
 *  @param[in]     id                : Identification of the transmitted I-PDU.
 *  @param[in]     info              : Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                     (SduLength). If not enough transmit data is available, no data is copied by the
 *                                     upper layer module and BUFREQ_E_BUSY is returned. The lower layer module may
 *                                     retry the call. An SduLength of 0 can be used to indicate state changes in the
 *                                     retry parameter or to query the current amount of available data in the upper
 *                                     layer module. In this case, the Sdu DataPtr may be a NULL_PTR.
 *  @param[in]     retry             : This parameter is used to acknowledge transmitted data or to retransmit data
 *                                     after transmission problems. If the retry parameter is a NULL_PTR, it indicates
 *                                     that the transmit data can be removed from the buffer immediately after it has
 *                                     been copied. Otherwise, the retry parameter must point to a valid RetryInfoType
 *                                     element. If TpDataState indicates TP_CONFPENDING, the previously copied data must
 *                                     remain in the TP buffer to be available for error recovery. TP_DATACONF indicates
 *                                     that all data that has been copied before this call is confirmed and can be
 *                                     removed from the TP buffer. Data copied by this API call is excluded and will be
 *                                     confirmed later. TP_DATARETRY indicates that this API call shall copy previously
 *                                     copied data in order to recover from an error. In this case TxTpDataCnt specifies
 *                                     the offset in bytes from the current data copy position.
 *  @param[out]    availableDataPtr : Indicates the remaining number of bytes that are available in the upper layer
 *                                     module's Tx buffer. availableDataPtr can be used by TP modules that support
 *                                     dynamic payload lengths (e.g. FrIsoTp) to determine the size of the following
 *                                     CFs.
 *  @return        BufReq_ReturnType
 *  @retval        BUFREQ_OK       : Data has been copied to the transmit buffer completely as requested.
 *  @retval        BUFREQ_E_BUSY   : Request could not be fulfilled, because the required amount of Tx data is not
 *                                   available. The lower layer module may retry this call later on. No data has been
 *                                   copied.
 *  @retval        BUFREQ_E_NOT_OK : Data has not been copied. Request failed.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56808
 */
BufReq_ReturnType PduR_TpCopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);
#endif

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
/**
 *  @brief         This function is called after the I-PDU has been transmitted on its network, the result indicates
 *                 whether the transmission was successful or not.
 *  @param[in]     id     : Identification of the transmitted I-PDU.
 *  @param[in]     result : E_OK     : The PDU was transmitted.
 *                          E_NOT_OK : Transmission of the PDU failed.
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-56809
 */
void PduR_TpTxConfirmation(PduIdType id, Std_ReturnType result);
#endif

#endif /* end of PDUR_ZERO_COST_OPERATION*/

#ifdef __cplusplus
}
#endif

#endif
