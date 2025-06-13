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
********************************************************************************
**                                                                            **
**  FILENAME    : CanIf_Internal.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of CANIF               **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* Refer to CanIf.h */

#ifndef CANIF_INTERNAL_H
#define CANIF_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf.h"
#include "SchM_CanIf.h"
#include "istd_lib.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(CANIF_PUBLISHED_INFORMATION)
#define CANIF_PUBLISHED_INFORMATION
#define CANIF_MODULE_ID                   60u
#define CANIF_VENDOR_ID                   62u
#define CANIF_AR_RELEASE_MAJOR_VERSION    4u
#define CANIF_AR_RELEASE_MINOR_VERSION    5u
#define CANIF_AR_RELEASE_REVISION_VERSION 0u
#define CANIF_SW_MAJOR_VERSION            2u
#define CANIF_SW_MINOR_VERSION            2u
#define CANIF_SW_PATCH_VERSION            3u
#elif ((CANIF_SW_MAJOR_VERSION != 2u) || (CANIF_SW_MINOR_VERSION != 2u) || (CANIF_SW_PATCH_VERSION != 3u))
#error "CanIf: Mismatch in Software Version"
#endif

#define CANIF_NOUSED(paramter) (void)(paramter)

/*******************************************************************************
**                      Global Data Declarations                              **
*******************************************************************************/
#if !defined(CANIF_LOCAL)
#define CANIF_LOCAL static
#else
#undef LOCAL_INLINE
#define LOCAL_INLINE static inline
#endif

/*Can Trcv Configuration*/
#if (CANIF_TRCV_NUMBER > 0u)
extern CONST(CanIf_TrcvCfgType, CANIF_CONST) CanIf_TrcvCfgData[CANIF_TRCV_NUMBER];
#endif

/*Can Trcv API*/
#if (CANIF_TRCVDRIVER_NUMBER > 0u)
extern CONST(Can_TrcvApiType, CANIF_CONST) Can_TrcvApi[CANIF_TRCVDRIVER_NUMBER];
#endif

/* Callout functions with respect to the upper layers. */
extern CONST(CanIf_DispatchConfigType, CANIF_CONST) CanIf_DispatchConfigData;

extern P2CONST(CanIf_ConfigType, CANIF_VAR, CANIF_CONST) CanIf_ConfigStd;

/*Can Controller Configuration*/
extern CONST(CanIf_ControllerCfgType, CANIF_CONST) CanIf_CtrlCfgData[CANIF_CANCONTROLLER_NUMBER];

/*Can Driver API*/
extern CONST(Can_DriverApiType, CANIF_CONST) Can_DriverApi[CANIF_CANDRIVER_NUMBER];

#if (CANIF_TXBUFFER_NUMBER > 0u)
extern VAR(CanIf_TxBufferType, CANIF_VAR) CanIf_TxBuffer[CANIF_TXBUFFER_NUMBER];
#endif

#if (CANIF_RXBUFFER_NUMBER > 0u)
extern VAR(CanIf_RxPduBufferType, CANIF_VAR) CanIf_RxBuffer[CANIF_RXBUFFER_NUMBER];
#endif

extern CanIfRxPduUserRxIndicationNameApiType CanIf_UpRxIndicationArray[CANIF_RXINDICATION_FUNC_NUMBER];
extern CanIfTxPduUserTxConfirmationNameApiType CanIf_UpTxConfirmationArray[CANIF_TXCONFIRMATION_FUNC_NUMBER];
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
extern CanIfTxPduUserTriggerTransmitNameApiType CanIf_UpTriggerTransmitArray[CANIF_TRIGGER_TRANSMIT_FUNC_NUMBER];
#endif /* STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT */
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CANIF_TXPDU(TxPdu_Index)                  CanIf_ConfigStd->CanIfTxPduConfigRef[(TxPdu_Index)]
#define CANIF_TXBUFFER_SIZE(TxBuffer_Index)       CanIf_ConfigStd->CanIfBufferSize[(TxBuffer_Index)]
#define CANIF_HTH(Hth_Index)                      CanIf_ConfigStd->CanIfHthCfgRef[(Hth_Index)]
#define CANIF_RXPDU(RxPdu_Index)                  CanIf_ConfigStd->CanIfRxPduConfigRef[(RxPdu_Index)]
#define CANIF_HRH(Hrh_Index)                      CanIf_ConfigStd->CanIfHrhCfgRef[(Hrh_Index)]

#define CANIF_CONTROLLER_DRV_ID(Controller_Index) CanIf_CtrlCfgData[(Controller_Index)].CanDriverId

#if (STD_ON == CANIF_META_DATA_SUPPORT)
/*MetaData and CanId transition*/
LOCAL_INLINE FUNC(CanIf_IdType, CANIF_CODE) CanIf_MetaDataToCanId(P2CONST(uint8, AUTOMATIC, CANIF_APPL_DATA) meteData)
{
#if (TRUE == CPU_32_WITH_16_ADR)
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    return (CanIf_IdType)(meteData[0u] & CANIF_UNUSED_UINT8) | ((CanIf_IdType)(meteData[1u] & CANIF_UNUSED_UINT8) << 8u)
           | ((CanIf_IdType)(meteData[2u] & CANIF_UNUSED_UINT8) << 16u)
           | ((CanIf_IdType)(meteData[3u] & CANIF_UNUSED_UINT8) << 24u);
#else
    return (CanIf_IdType)(meteData[3u] & CANIF_UNUSED_UINT8) | ((CanIf_IdType)(meteData[2u] & CANIF_UNUSED_UINT8) << 8u)
           | ((CanIf_IdType)(meteData[1u] & CANIF_UNUSED_UINT8) << 16u)
           | ((CanIf_IdType)(meteData[0u] & CANIF_UNUSED_UINT8) << 24u);
#endif /* LOW_BYTE_FIRST == CPU_BYTE_ORDER */
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    return (CanIf_IdType)meteData[0u] | ((CanIf_IdType)meteData[1u] << 8u) | ((CanIf_IdType)meteData[2u] << 16u)
           | ((CanIf_IdType)meteData[3u] << 24u);
#else
    return (CanIf_IdType)meteData[3u] | ((CanIf_IdType)meteData[2u] << 8u) | ((CanIf_IdType)meteData[1u] << 16u)
           | ((CanIf_IdType)meteData[0u] << 24u);
#endif /* LOW_BYTE_FIRST == CPU_BYTE_ORDER */
#endif
}

LOCAL_INLINE FUNC(void, CANIF_CODE)
    CanIf_CanIdToMetaData(CanIf_IdType canId, P2VAR(uint8, AUTOMATIC, CANIF_APPL_DATA) meteData) /* PRQA S 3432 */
{
#if (TRUE == CPU_32_WITH_16_ADR)
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    meteData[0u] = (uint8)(canId & CANIF_UNUSED_UINT8);
    meteData[1u] = (uint8)(canId >> 8u & CANIF_UNUSED_UINT8);
    meteData[2u] = (uint8)(canId >> 16u & CANIF_UNUSED_UINT8);
    meteData[3u] = (uint8)(canId >> 24u & CANIF_UNUSED_UINT8);
#else
    meteData[3u] = (uint8)(canId & CANIF_UNUSED_UINT8);
    meteData[2u] = (uint8)(canId >> 8u & CANIF_UNUSED_UINT8);
    meteData[1u] = (uint8)(canId >> 16u & CANIF_UNUSED_UINT8);
    meteData[0u] = (uint8)(canId >> 24u & CANIF_UNUSED_UINT8);
#endif /* LOW_BYTE_FIRST == CPU_BYTE_ORDER */
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    meteData[0u] = (uint8)(canId);
    meteData[1u] = (uint8)(canId >> 8u);
    meteData[2u] = (uint8)(canId >> 16u);
    meteData[3u] = (uint8)(canId >> 24u);
#else
    meteData[3u] = (uint8)(canId);
    meteData[2u] = (uint8)(canId >> 8u);
    meteData[1u] = (uint8)(canId >> 16u);
    meteData[0u] = (uint8)(canId >> 24u);
#endif /* LOW_BYTE_FIRST == CPU_BYTE_ORDER */
#endif
}
#endif /* STD_ON == CANIF_META_DATA_SUPPORT */

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
extern FUNC(void, CANIF_CODE) CanIf_SetRxNotifStatusHandle(PduIdType CanIfRxSduId);

extern FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadRxNotifStatusHandle(PduIdType CanIfRxSduId);
#endif /*STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API*/

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
extern FUNC(void, CANIF_CODE) CanIf_SetTxNotifStatusHandle(PduIdType CanIfTxSduId);

extern FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadTxNotifStatusHandle(PduIdType CanIfTxSduId);
#endif /*STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API*/

#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
extern FUNC(void, CANIF_CODE) CanIf_FreeRxTxNotifyStatus(uint8 Controller);

extern FUNC(void, CANIF_CODE) CanIf_InitRxTxNotifyStatus(VAR(void, AUTOMATIC));
#endif

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
extern FUNC(void, CANIF_CODE) CanIf_InitTxBuffer(VAR(void, AUTOMATIC));

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxBusyHandle(P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr);

extern FUNC(void, CANIF_CODE) CanIf_FreeControllerTxBuffer(uint8 Controller);

extern FUNC(void, CANIF_CODE) CanIf_TransmitBufferedPdu(PduIdType CanTxPduId);
#endif /*STD_ON == CANIF_PUBLIC_TX_BUFFERING*/

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
extern FUNC(void, CANIF_CODE) CanIf_InitRxBuffer(VAR(void, AUTOMATIC));

extern FUNC(void, CANIF_CODE) CanIf_FreeControllerRxBuffer(uint8 Controller);
#endif /*STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API*/

#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
extern FUNC(void, CANIF_CODE) CanIf_SetDynamicTxIdHandle(PduIdType CanIfTxSduId, Can_IdType CanId);
#endif /*STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API*/

extern FUNC(void, CANIF_CODE) CanIf_GetBaseCanIdHandle(
    PduIdType CanTxPduId,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
#endif
    P2VAR(CanIf_IdType, AUTOMATIC, AUTOMATIC) CanId); /* PRQA S 3432 */

extern FUNC(void, CANIF_CODE) CanIf_FreeDynamicTxPduCanIds(uint8 Controller);

extern FUNC(void, CANIF_CODE) CanIf_InitDynamicTxPduCanId(VAR(void, AUTOMATIC));

#endif /* end of CANIF_INTERNAL_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
