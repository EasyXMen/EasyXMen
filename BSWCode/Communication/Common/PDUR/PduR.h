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
********************************************************************************
**                                                                            **
**  FILENAME    : PduR.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of PDUR                **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19_11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE>   <AUTHOR>       <REVISION LOG>
 *  V2.0.0   20200622 zhengfei.li    Initial version (Upgrade according to the R19_11 standards)
 *  V2.0.1   20211103 zhengfei.li    add "PDUR_" to some Macro Name
 *  V2.1.0   20211217 zhengfei.li    PB/PC configuration parameter split
 *  V2.2.0   20231215 tong.zhao      Upgrade according to the R19_11 standards
 *                                    (Project regression)
 *                                    (jira:CPT-7994)(CPD-33702)
 *  V2.2.1   20240109 xiaojian.liang Support TP Gateway 1:n, fifo and forwarding to up layers
 *  V2.2.2   20240319 tong.zhao      (jira:CPT-8495)
 *      PduR_DestPduState can't recovery in gateway when dest bus-off(no TxConfirmation)
 *  V2.2.3   20240522 tong.zhao      (jira:CPT-9076)
 *      Modify compilation errors when det is disable
 *  V2.3.0   20240105  tong.zhao     Muliticore adapt
 *  V2.3.1   20240529  tong.zhao
 *      PduR_DestPduState can't recovery,when Tp GW data is received again,before receiving txconfirmation.
 *  V2.3.2   20240627  tong.zhao
 *      Optimize flash
 *  V2.3.3   20240723  tong.zhao     Fix Bug
 *      PduR can not forward tx confirmation to upper layer in TriggerTransmit.
 *  V2.3.4   20241211  tong.zhao
 *      1)member "PduRDestTxBufferRef" change to "PduRDestQueueIdRef" in type "PduRDestPduType".
 *      2)remove macro "PDUR_BUFFER_ENABLED".
 *  V2.3.5   20250223  tong.zhao      Add feature Tp Gw N:1,If Gw N:1,If Rx N:1
 *  V2.3.6   20250909  tong.zhao      (jira:CPT-15752)
 *     In Tp Gateway, the Dest state has not been successfully set yet, while the lower layer starts copy tx data, which
 *     leads to data loss in the gateway.
 *
 ******************************************************************************/

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:PduR<br>
    RuleSorce:puhua_rules-2024.12.rcf

    \li VL_PduR_2812
      Reason: Configuration data design is required.
      Risk: None
      Prevention: None

    \li VL_PduR_3678
      Reason: Under another configuration of PduR object referceced by pointer is changeable.
      Risk: None
      Prevention: None

    \li VL_PduR_Logical
      Reason:Necessary logical design for while(1) and have "break" to stop.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_PduR_1505
      Reason: In order to make the module code structure clear, the functions are classified.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_MTR_PduR_STVAR
      Reason: The function prototype is defined in AUTOSAR standard.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_PduR_STCYC
      Reason: No separation of functionality into sub-functions because there is a higher requirement to minimize the
  stack and runtime usage applied to the code.
      Risk: Understandability and testability can become too complex.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_PduR_STCDN
      Reason: Special file, with fewer annotations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */
#ifndef PDUR_H
#define PDUR_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(PDUR_PUBLISHED_INFORMATION)
#define PDUR_PUBLISHED_INFORMATION
#define PDUR_MODULE_ID                   51u
#define PDUR_VENDOR_ID                   62u
#define PDUR_AR_RELEASE_MAJOR_VERSION    4u
#define PDUR_AR_RELEASE_MINOR_VERSION    5u
#define PDUR_AR_RELEASE_REVISION_VERSION 0u
#define PDUR_SW_MAJOR_VERSION            2u
#define PDUR_SW_MINOR_VERSION            3u
#define PDUR_SW_PATCH_VERSION            6u
#elif ((PDUR_SW_MAJOR_VERSION != 2u) || (PDUR_SW_MINOR_VERSION != 3u) || (PDUR_SW_PATCH_VERSION != 6u))
#error "PduR: Mismatch in Software Version"
#endif

#define PDUR_INSTANCE_ID 0u

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
/* General function id */
#define PDUR_TPCOPYTXDATA_ID       ((uint8)0x43u)
#define PDUR_TPRXINDICATION_ID     ((uint8)0x45u)
#define PDUR_TRANSMIT_ID           ((uint8)0x49u)
#define PDUR_CANCELTRANSMIT_ID     ((uint8)0x4Au)
#define PDUR_CANCELRECEIVE_ID      ((uint8)0x4Cu)
#define PDUR_INIT_ID               ((uint8)0xF0u)
#define PDUR_GETVERSIONINFO_ID     ((uint8)0xF1u)
#define PDUR_GETCONFIGURATIONID_ID ((uint8)0xF2u)
#define PDUR_ENABLEROUTING_ID      ((uint8)0xF3u)

/* Error Classification */
#define PDUR_E_INIT_FAILED                   ((uint8)0x00u)
#define PDUR_E_UNINIT                        ((uint8)0x01u)
#define PDUR_E_PDU_ID_INVALID                ((uint8)0x02u)
#define PDUR_E_ROUTING_PATH_GROUP_ID_INVALID ((uint8)0x08u)
#define PDUR_E_PARAM_POINTER                 ((uint8)0x09u)
#endif /* STD_ON == PDUR_DEV_ERROR_DETECT */

/*The API and DET Ref to Det_ReportRuntimeError*/
/* General function id */
#define PDUR_IFTXCONFIRMATION_ID   ((uint8)0x40u)
#define PDUR_IFTRIGGERTRANSMIT_ID  ((uint8)0x41u)
#define PDUR_IFRXINDICATION_ID     ((uint8)0x42u)
#define PDUR_TPCOPYRXDATA_ID       ((uint8)0x44u)
#define PDUR_TPSTARTOFRECEPTION_ID ((uint8)0x46u)
#define PDUR_TPTXCONFIRMATION_ID   ((uint8)0x48u)
#define PDUR_DISABLEROUTING_ID     ((uint8)0xF4u)

/* Error Classification */
#define PDUR_E_TP_TX_REQ_REJECTED (0x03u)
#define PDUR_E_PDU_INSTANCES_LOST (0x0Au)
#define PDUR_E_NO_BUFFER          (0x0Bu)

/*the Private Macro Definition*/
#define PDUR_UNUSED_UINT8       (0xFFu)
#define PDUR_UNUSED_UINT16      (0xFFFFu)
#define PDUR_UNUSED_UINT32      (0xFFFFFFFFu)
#define PDUR_NOUSED(paramter)   (void)(paramter)
#define PDUR_PDU_LENGTH_INVALID (PduLengthType)(~(PduLengthType)(0))

#define PDUR_ROUTEPATHGROUP_CFG(id) PduR_ConfigStd->PduRRoutingPathGroupRef[id]

#define PDUR_DESTPDU_CFG(id) PduR_ConfigStd->PduRDestPduRef[id]

#define PDUR_SRCEPDU_CFG(id) PduR_ConfigStd->PduRSrcPduRef[id]

#define PDUR_ROUTINGPATH_CFG(id) \
    PduR_ConfigStd->PduRRoutingTableRef[PduR_ConfigStd->PduRConfigId].PduRRoutingPathRef[id]

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/* General functions provided by the PDU Router */
extern void PduR_Init(const PduR_PBConfigType* ConfigPtr);

#if (STD_ON == PDUR_VERSION_INFO_API)
extern void PduR_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif /* STD_ON == PDUR_VERSION_INFO_API */

extern PduR_PBConfigIdType PduR_GetConfigurationId(void);
extern void                PduR_EnableRouting(PduR_RoutingPathGroupIdType id);
extern void                PduR_DisableRouting(PduR_RoutingPathGroupIdType id, boolean initialize);

/* Configurable interfaces definitions for interaction with upper layer module */
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
extern Std_ReturnType PduR_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_CancelTransmit(PduIdType TxPduId);
#endif /* PDUR_CANCEL_TRANSMIT == STD_ON */

#if (PDUR_CANCEL_RECEIVE == STD_ON)
extern Std_ReturnType PduR_CancelReceive(PduIdType RxPduId);
#endif /* PDUR_CANCEL_RECEIVE == STD_ON */

#if (PDUR_RX_INDICATION == STD_ON)
extern void PduR_IfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
#endif /* PDUR_RX_INDICATION == STD_ON */

#if (PDUR_TX_CONFIRMATION == STD_ON)
extern void PduR_IfTxConfirmation(PduIdType TxPduId);
#endif /* PDUR_TX_CONFIRMATION == STD_ON */

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
extern Std_ReturnType PduR_IfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#endif /* PDUR_TRIGGER_TRANSMIT == STD_ON */

#if (PDUR_TP_COPYRXDATA == STD_ON)
extern BufReq_ReturnType PduR_TpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);
#endif /* PDUR_TP_COPYRXDATA == STD_ON */

#if (PDUR_TP_RXINDICATION == STD_ON)
extern void PduR_TpRxIndication(PduIdType id, Std_ReturnType result);
#endif /* PDUR_TP_RXINDICATION == STD_ON */

#if (PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON)
extern BufReq_ReturnType PduR_TpStartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);
#endif /* PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON */

#if (PDUR_TP_COPYTXDATA == STD_ON)
extern BufReq_ReturnType PduR_TpCopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);
#endif /* PDUR_TP_COPYTXDATA == STD_ON */

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
extern void PduR_TpTxConfirmation(PduIdType id, Std_ReturnType result);
#endif /* PDUR_TP_TXCONFIRMATION == STD_ON */

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*PduR PB Configuration Run Time point parameter*/
extern P2CONST(PduR_PBConfigType, PDUR_VAR, PDUR_CONST) PduR_ConfigStd;
/*define in PduR_PBcfg.c*/
extern CONST(PduR_PBConfigType, PDUR_CONST_PBCFG) PduR_PBConfigData;

#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif /* end of PDUR_H */
/*******************************************************************************
**                      End Of File                                           **
*******************************************************************************/
