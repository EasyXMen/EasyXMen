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
 **  @file               : SecOC.c
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation for SecOC
 **
 ***********************************************************************************************************************/
/* PRQA S 6540, 6520 EOF */ /* VL_MTR_SecOC_STTPP, VL_MTR_SecOC_STVAR */
/* PRQA S 3415 EOF */       /* VL_SecOC_3415 */
/* =================================================== inclusions =================================================== */

#include "SecOC_Internal.h"
#include "SecOC_Lcfg.h"
#include "PduR_SecOC.h"
#include "Det.h"
#include "Csm.h"
#include "IStdLib.h"
#if (SECOC_SECURITY_EVENT_REPORTING_ENABLED == STD_ON)
#include "IdsM.h"
#endif /* SECOC_SECURITY_EVENT_REPORTING_ENABLED  == STD_ON */
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
#include "SecOC_Callout.h"
#else
#include "Rte_SecOC.h"
#endif /* SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE */

/* ===================================================== macros ===================================================== */

/** SecOC version information */
#define SECOC_C_AR_MAJOR_VERSION 4u
/** SecOC version information */
#define SECOC_C_AR_MINOR_VERSION 9u
/** SecOC version information */
#define SECOC_C_AR_PATCH_VERSION 0u
/** SecOC version information */
#define SECOC_C_SW_MAJOR_VERSION 2u
/** SecOC version information */
#define SECOC_C_SW_MINOR_VERSION 2u
/** SecOC version information */
#define SECOC_C_SW_PATCH_VERSION 3u

#if (SECOC_C_AR_MAJOR_VERSION != SECOC_H_AR_MAJOR_VERSION)
#error "SecOC.c : Mismatch in Specification Major Version"
#endif
#if (SECOC_C_AR_MINOR_VERSION != SECOC_H_AR_MINOR_VERSION)
#error "SecOC.c : Mismatch in Specification _MINOR Version"
#endif
#if (SECOC_C_AR_PATCH_VERSION != SECOC_H_AR_PATCH_VERSION)
#error "SecOC.c : Mismatch in Specification PATCH Version"
#endif
#if (SECOC_C_SW_MAJOR_VERSION != SECOC_H_SW_MAJOR_VERSION)
#error "SecOC.c : Mismatch in Software Major Version"
#endif
#if (SECOC_C_SW_MINOR_VERSION != SECOC_H_SW_MINOR_VERSION)
#error "SecOC.c : Mismatch in Software MINOR Version"
#endif

/**
 * @brief Retrieves the minimum value between two values.
 * @param[in] a : First value.
 * @param[in] b : Second value.
 * @return uint16 : Minimum value between a and b.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace           CPD-PlaceHolder
 *
 */
#define SECOC_MIN(a, b) (((a) < (b)) ? (a) : (b)) /* PRQA S 3472 */ /* VL_SecOC_3472 */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"

/**
 * @brief Retrieves the Application ID.
 * @return ApplicationType
 * @retval SECOC_CONST_0U : Default application ID when SECOC_MULTIPLE_PARTITION_ENABLED is not enabled.
 * @retval Application ID : Application ID retrieved from GetApplicationID() when SECOC_MULTIPLE_PARTITION_ENABLED is
 * enabled.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70368
 */
SECOC_LOCAL ApplicationType SecOC_GetApplicationID(void);

/**
 * @brief Checks if the specified application is in the initialization state.
 * @param[in] appId : The application ID to check.
 * @return SecOC_StateType
 * @retval SECOC_UNINIT : The application is not initialized.
 * @retval Other State : The current initialization state of the application.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70369
 */
SECOC_LOCAL SecOC_StateType SecOC_isInitState(ApplicationType appId);

#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)
/**
 * @brief Sets a specific transmit event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be set.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70370
 */
SECOC_LOCAL void SECOC_SET_TX_EVENT(uint16 i, uint16 e);

/**
 * @brief Clears a specific transmit event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be cleared.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70371
 */
SECOC_LOCAL void SECOC_CLR_TX_EVENT(uint16 i, uint16 e);

/**
 * @brief Checks if a specific event is a transmit event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be checked.
 * @return boolean : TRUE if the event is a transmit event, FALSE otherwise.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70372
 */
SECOC_LOCAL boolean SECOC_IS_TX_EVENT(uint16 i, uint16 e);
/* PRQA S 5016 ++ */ /* VL_SecOC_5016 */
/**
 * @brief Retrieves the pointer to the processing configuration for a specific transmit PDU.
 * @param[in] idx : Index of the transmit PDU.
 * @return SecOC_PbTxPduProcessingType* : Pointer to the processing configuration for the specified transmit PDU.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70373
 */
SECOC_LOCAL const SecOC_PbTxPduProcessingType* SecOC_GetTxPduProcessingPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the processing data for a specific transmit PDU.
 * @param[in] idx : Index of the transmit PDU.
 * @return SecOC_TxPduProcessingType* : Pointer to the processing data for the specified transmit PDU.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70374
 */
SECOC_LOCAL const SecOC_TxPduProcessingType* SecOC_GetTxPduProcessingDataPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the authentication PDU layer for a specific transmit PDU.
 * @param[in] idx : Index of the transmit PDU.
 * @return SecOC_TxAuthenticPduLayerType* : Pointer to the authentication PDU layer for the specified transmit PDU.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70375
 */
SECOC_LOCAL const SecOC_TxAuthenticPduLayerType* SecOC_GetTxAuthenticPduLayerPtr(uint16 idx);
/* PRQA S 5016 -- */

/**
 * @brief Retrieves the application ID for a specific transmit channel.
 * @param[in] idx : Index of the transmit channel.
 * @return ApplicationType : Application ID for the specified transmit channel.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70376
 */
SECOC_LOCAL ApplicationType SecOC_GetTxChannelApplicationId(uint16 idx);

/* PRQA S 5016 ++ */ /* VL_SecOC_5016 */
/**
 * @brief Retrieves the pointer to the transmit Secured PDU Layer for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Secured PDU Layer is requested.
 * @return SecOC_TxSecuredPduLayerType : Pointer to the transmit Secured PDU Layer.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70377
 */
SECOC_LOCAL const SecOC_TxSecuredPduLayerType* SecOC_GetTxSecuredPduLayerPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the transmit Secured PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Secured PDU is requested.
 * @return SecOC_TxSecuredPduType : Pointer to the transmit Secured PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70378
 */
SECOC_LOCAL const SecOC_TxSecuredPduType* SecOC_GetTxSecuredPduPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the transmit Secured PDU Collection for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Secured PDU Collection is requested.
 * @return SecOC_TxSecuredPduCollectionType : Pointer to the transmit Secured PDU Collection.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70379
 */
SECOC_LOCAL const SecOC_TxSecuredPduCollectionType* SecOC_GetTxSecuredPduCollectionPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the transmit Authentic PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Authentic PDU is requested.
 * @return SecOC_TxAuthenticPduType : Pointer to the transmit Authentic PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70380
 */
SECOC_LOCAL const SecOC_TxAuthenticPduType* SecOC_GetTxAuthenticPduPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the transmit Cryptographic PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Cryptographic PDU is requested.
 * @return SecOC_TxCryptographicPduType : Pointer to the transmit Cryptographic PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70381
 */
SECOC_LOCAL const SecOC_TxCryptographicPduType* SecOC_GetTxCryptographicPduPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the transmit Use Message Link for a specific index.
 * @param[in] idx : Index for which the pointer to the transmit Use Message Link is requested.
 * @return SecOC_UseMessageLinkType : Pointer to the transmit Use Message Link.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70382
 */
SECOC_LOCAL const SecOC_UseMessageLinkType* SecOC_GetTxUseMessageLinkPtr(uint16 idx);
/* PRQA S 5016 -- */

/**
 * @brief       Finds the index of the process transmission PDU (Protocol Data Unit) based on the given TxPduId.
 * @param[in]   TxPduId : The ID of the transmission PDU to be found.
 * @param[in]   startIdx : The start index of the process transmission PDU.
 * @param[in]   endIdx : The end index of the process transmission PDU.
 * @return      uint16
 * @retval      The index of the process transmission PDU if found, otherwise a specific value indicating not found.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70383
 */
SECOC_LOCAL uint16 SecOC_FindProcessTxPduIndex(PduIdType TxPduId, uint16 startIdx, uint16 endIdx);

/**
 * @brief Generates authenticator used to create Secured I-PDU.
 * @param[in] index : SECOC Channel.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70384
 */
SECOC_LOCAL void SecOC_TxGenerateAuthenticator(uint16 index);

/**
 * @brief Calculates the length of DataToAuthenticator.
 * @param[in] index : SECOC Channel.
 * @return uint16 : The length of DataToAuthenticator.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70385
 */
SECOC_LOCAL uint32 SecOC_TxCalcDataToAuthenticatorLength(uint16 index);

/**
 * @brief Constructs the DataToAuthenticator.
 * @param[in] index : SECOC Channel.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70386
 */
SECOC_LOCAL Std_ReturnType SecOC_TxConstructDataToAuthenticator(uint16 index);

/**
 * @brief Generates the Authenticator retry process.
 * @param[in] index : SECOC Channel.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70387
 */
SECOC_LOCAL void SecOC_TxGenerateAuthenticatorRetry(uint16 index);

/**
 * @brief Builds the Authenticator fail process.
 * @param[in] index : SECOC Channel.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70388
 */
SECOC_LOCAL void SecOC_TxBuildAuthenticatorFailHandle(uint16 index);

/**
 * @brief Sends the Secured I-PDU.
 * @param[in] index : SecOC Channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70389
 */
SECOC_LOCAL void SecOC_TxSendSecuredIPDU(uint16 index);

/**
 * @brief Constructs the Secured I-PDU to be transmitted.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70390
 */
SECOC_LOCAL void SecOC_TxConstructSecuredIPDU(uint16 index);

/**
 * @brief Constructs the Authentic I-PDU to be transmitted.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70391
 */
SECOC_LOCAL void SecOC_TxConstructAuthenticIPDU(uint16 index);

/**
 * @brief Constructs the Cryptographic I-PDU to be transmitted.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70392
 */
SECOC_LOCAL void SecOC_TxConstructCryptographicIPDU(uint16 index);

/**
 * @brief Broadcasts the Secured I-PDU to the lower layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70393
 */
SECOC_LOCAL void SecOC_TxBroadcastSecuredIPDU(uint16 index);

/**
 * @brief Broadcasts the Authentic I-PDU to the lower layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70394
 */
SECOC_LOCAL void SecOC_TxBroadcastAuthenticIPDU(uint16 index);

/**
 * @brief Broadcasts the Cryptographic I-PDU to the lower layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70395
 */
SECOC_LOCAL void SecOC_TxBroadcastCryptographicIPDU(uint16 index);

#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
/**
 * @brief Acquires the data from the lower layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70396
 */
SECOC_LOCAL void SecOC_TxTpAcquireData(uint16 index);
#endif

/**
 * @brief Resets the runtime data to default values.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70397
 */
SECOC_LOCAL void SecOC_TxChannelReset(uint16 index);
#endif /* SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U */

#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
/**
 * @brief Sets a specific receive event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be set.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70398
 */
SECOC_LOCAL void SECOC_SET_RX_EVENT(uint16 i, uint16 e);

/**
 * @brief Clears a specific receive event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be cleared.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70399
 */
SECOC_LOCAL void SECOC_CLR_RX_EVENT(uint16 i, uint16 e);

/**
 * @brief Checks if a specific event is a receive event.
 * @param[in] i : Index of the event.
 * @param[in] e : Event to be checked.
 * @return boolean : TRUE if the event is a receive event, FALSE otherwise.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70400
 */
SECOC_LOCAL boolean SECOC_IS_RX_EVENT(uint16 i, uint16 e);
/* PRQA S 5016 ++ */ /* VL_SecOC_5016 */
/**
 * @brief Retrieves the pointer to the receive PDU Processing for a specific index.
 * @param[in] idx : Index for which the pointer to the receive PDU Processing is requested.
 * @return SecOC_PbRxPduProcessingType : Pointer to the receive PDU Processing.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70401
 */
SECOC_LOCAL const SecOC_PbRxPduProcessingType* SecOC_GetRxPduProcessingPtr(uint16 idx);

/**
 * @brief Retrieves the application ID for a specific receive channel.
 * @param[in] idx : Index of the receive channel.
 * @return ApplicationType : Application ID for the specified receive channel.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70402
 */
SECOC_LOCAL ApplicationType SecOC_GetRxChannelApplicationId(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive PDU Processing data for a specific index.
 * @param[in] idx : Index for which the pointer to the receive PDU Processing data is requested.
 * @return SecOC_RxPduProcessingType : Pointer to the receive PDU Processing data.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70403
 */
SECOC_LOCAL const SecOC_RxPduProcessingType* SecOC_GetRxPduProcessingDataPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive Authentic PDU Layer for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Authentic PDU Layer is requested.
 * @return SecOC_RxAuthenticPduLayerType : Pointer to the receive Authentic PDU Layer.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70404
 */
SECOC_LOCAL const SecOC_RxAuthenticPduLayerType* SecOC_GetRxAuthenticPduLayerPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive Secured PDU Layer for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Secured PDU Layer is requested.
 * @return SecOC_RxSecuredPduLayerType : Pointer to the receive Secured PDU Layer.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70405
 */
SECOC_LOCAL const SecOC_RxSecuredPduLayerType* SecOC_GetRxSecuredPduLayerPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive secured PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the receive secured PDU is requested.
 * @return SecOC_RxSecuredPduType : Pointer to the receive secured PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70406
 */
SECOC_LOCAL const SecOC_RxSecuredPduType* SecOC_GetRxSecuredPduPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive Secured PDU Collection for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Secured PDU Collection is requested.
 * @return SecOC_RxSecuredPduCollectionType : Pointer to the receive Secured PDU Collection.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70407
 */
SECOC_LOCAL const SecOC_RxSecuredPduCollectionType* SecOC_GetRxSecuredPduCollectionPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive Authentic PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Authentic PDU is requested.
 * @return SecOC_RxAuthenticPduType : Pointer to the receive Authentic PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70408
 */
SECOC_LOCAL const SecOC_RxAuthenticPduType* SecOC_GetRxAuthenticPduPtr(uint16 idx);

/**
 * @brief Retrieves the pointer to the receive Cryptographic PDU for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Cryptographic PDU is requested.
 * @return SecOC_RxCryptographicPduType : Pointer to the receive Cryptographic PDU.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70409
 */
SECOC_LOCAL const SecOC_RxCryptographicPduType* SecOC_GetRxCryptographicPduPtr(uint16 idx);
/* PRQA S 5016 -- */
#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
/**
 * @brief Processes the Secured I-PDU receive indication.
 * @param[in] index : SecOC channel index.
 * @param[in] RxPduId : ID of the received PDU.
 * @param[in] PduInfoPtr : Contains the length (SduLength) of the received PDU and a pointer to a buffer (SduDataPtr)
 * containing the PDU.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70410
 */
SECOC_LOCAL void SecOC_RxIfSecuredPduIndication(uint16 index, PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief Processes the Authentic I-PDU receive indication.
 * @param[in] index : SecOC channel index.
 * @param[in] PduInfoPtr : Contains the length (SduLength) of the received PDU and a pointer to a buffer (SduDataPtr)
 * containing the PDU.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70411
 */
SECOC_LOCAL void SecOC_RxIfAuthenticPduIndication(uint16 index, const PduInfoType* PduInfoPtr);

/**
 * @brief Processes the Cryptographic I-PDU receive indication.
 * @param[in] index : SecOC channel index.
 * @param[in] PduInfoPtr : Contains the length (SduLength) of the received PDU and a pointer to a buffer (SduDataPtr)
 * containing the PDU.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70412
 */
SECOC_LOCAL void SecOC_RxIfCryptographicPduIndication(uint16 index, const PduInfoType* PduInfoPtr);
#endif /* SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U */

/**
 * @brief Parses the received Secured I-PDU.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70413
 */
SECOC_LOCAL void SecOC_RxParseSecuredIpdu(uint16 index);

/**
 * @brief Performs the verification process.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70414
 */
SECOC_LOCAL void SecOC_RxVerifyProcee(uint16 index);

/**
 * @brief Verifies the received Secured I-PDU.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70415
 */
SECOC_LOCAL void SecOC_RxVerifyAuthenticationInfo(uint16 index);

/**
 * @brief Calculates the length of DataToAuthenticator.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70416
 */
SECOC_LOCAL uint32 SecOC_RxCalcDataToAuthenticatorLength(uint16 index);

/**
 * @brief Constructs DataToAuthenticator.
 * @param[in] index : SecOC channel index.
 * @return Std_ReturnType
 * @retval E_OK : Construct DataToAuthenticator success
 * @retval E_NOT_OK : Construct DataToAuthenticator fail
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70417
 */
SECOC_LOCAL Std_ReturnType SecOC_RxConstructDataToAuthenticator(uint16 index);

/**
 * @brief Builds the DataToAuthenticator retry process.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70418
 */
SECOC_LOCAL void SecOC_RxAuthenticationBuildRetry(uint16 index);

/**
 * @brief Verifies the Authentication information retry process.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70419
 */
SECOC_LOCAL void SecOC_RxVerifyAuthenticInfoRetry(uint16 index);

#if (SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U)
/**
 * @brief Checks the message link.
 * @param[in] index : SecOC channel index.
 * @return Std_ReturnType
 * @retval E_OK : Message link check success
 * @retval E_NOT_OK : Message link check fail
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70420
 */
SECOC_LOCAL Std_ReturnType SecOC_RxMessageLinkCheck(uint16 index);
/* PRQA S 5016 ++ */ /* VL_SecOC_5016 */
/**
 * @brief Retrieves the pointer to the receive Use Message Link for a specific index.
 * @param[in] idx : Index for which the pointer to the receive Use Message Link is requested.
 * @return SecOC_UseMessageLinkType : Pointer to the receive Use Message Link.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70421
 */
SECOC_LOCAL const SecOC_UseMessageLinkType* SecOC_GetRxUseMessageLinkPtr(uint16 idx);
/* PRQA S 5016 -- */
#endif /* SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U */

/**
 * @brief Sends the Authentic PDU to the upper layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70422
 */
SECOC_LOCAL void SecOC_RxPassAuthenticPduToUpperLayer(uint16 index);

#if (SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U)
/**
 * @brief Sends the Authentic PDU (TP) to the upper layer.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70423
 */
SECOC_LOCAL void SecOC_RxTransTpDataToUpperLayer(uint16 index);

/**
 * @brief Starts the receive Secured TP PDU process.
 * @param[in] index : SecOC channel index.
 * @param[in] id : Identification of the I-PDU.
 * @param[in] info : Pointer to a PduInfoType structure containing the payload data (without protocol information) and
 * payload length.
 * @param[in] TpSduLength : Total length of the N-SDU to be received.
 * @param[out] bufferSizePtr : Available receive buffer in the receiving module.
 * @return BufReq_ReturnType
 * @retval BUFREQ_OK : Connection has been accepted.
 * @retval BUFREQ_E_NOT_OK : Connection has been rejected.
 * @retval BUFREQ_E_OVFL : No buffer of the required length can be provided.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70424
 */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpSecuredPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief Starts the receive Authentic TP PDU process.
 * @param[in] index : SecOC channel index.
 * @param[in] id : Identification of the I-PDU.
 * @param[in] info : Pointer to a PduInfoType structure containing the payload data (without protocol information) and
 * payload length.
 * @param[in] TpSduLength : Total length of the N-SDU to be received.
 * @param[out] bufferSizePtr : Available receive buffer in the receiving module.
 * @return BufReq_ReturnType
 * @retval BUFREQ_OK : Connection has been accepted.
 * @retval BUFREQ_E_NOT_OK : Connection has been rejected.
 * @retval BUFREQ_E_OVFL : No buffer of the required length can be provided.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70425
 */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpAuthenticPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief Starts the receive Cryptographic TP PDU process.
 * @param[in] index : SecOC channel index.
 * @param[in] id : Identification of the I-PDU.
 * @param[in] info : Pointer to a PduInfoType structure containing the payload data (without protocol information) and
 * payload length.
 * @param[in] TpSduLength : Total length of the N-SDU to be received.
 * @param[out] bufferSizePtr : Available receive buffer in the receiving module.
 * @return BufReq_ReturnType
 * @retval BUFREQ_OK : Connection has been accepted.
 * @retval BUFREQ_E_NOT_OK : Connection has been rejected.
 * @retval BUFREQ_E_OVFL : No buffer of the required length can be provided.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70426
 */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpCryptographicPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);
#endif /* SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U */

#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
/**
 * @brief Loads the queued PDU to start the process.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70427
 */
SECOC_LOCAL void SecOC_RxLoadQueuedPdu(uint16 index);
#endif

/**
 * @brief Reports the verify status to other modules using the configured callout function.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70428
 */
SECOC_LOCAL void SecOC_RxReportVerifyStatus(uint16 index);

/**
 * @brief Resets the channel runtime data to default values.
 * @param[in] index : SecOC channel index.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70429
 */
SECOC_LOCAL void SecOC_RxChannelReset(uint16 index);

/**
 * @brief Calculates the runtime PDU length.
 * @param[in] headerLength : Header length (runtime PDU length in header).
 * @param[in] headerPtr : Pointer to the buffer of Header.
 * @return uint16 : Runtime PDU length.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70430
 */
SECOC_LOCAL PduLengthType SecOC_RxGetPduLength(uint8 headerLength, const uint8* headerPtr);
#endif /* SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U */

/**
 * @brief Switches the data byte order.
 * @param[in] dataPtr : Pointer to data to be converted.
 * @param[in] size : Data length in bytes.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70431
 */
SECOC_LOCAL void SecOC_EndianSwap(uint8* dataPtr, uint8 size);

/**
 * @brief Shifts the array to the left.
 * @param[in] numArr : Pointer to the number array.
 * @param[in] numLen : Length of the number array.
 * @param[in] shiftBits : Number of bits to shift.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70432
 */
SECOC_LOCAL void SecOC_ArrayShiftLeft(uint8* numArr, uint16 numLen, uint16 shiftBits);

/**
 * @brief Shifts the array to the right.
 * @param[in] numArr : Pointer to the number array.
 * @param[in] numLen : Length of the number array.
 * @param[in] shiftBits : Number of bits to shift.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70433
 */
SECOC_LOCAL void SecOC_ArrayShiftRight(uint8* numArr, uint16 numLen, uint16 shiftBits);

/**
 * @brief Performs the bitwise OR operation on two number arrays.
 * @param[in] numArr1 : Pointer to the first number array.
 * @param[in] numArr2 : Pointer to the second number array.
 * @param[in] numLen : Length of the number array.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70434
 */
SECOC_LOCAL void SecOC_ArrayOrOp(uint8* numArr1, const uint8* numArr2, uint16 numLen);

/**
 * @brief Clears the low bits of a number array by shifting.
 * @param[in] numArr : Pointer to the number array.
 * @param[in] numLen : Length of the number array.
 * @param[in] shiftBits : Number of bits to shift.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70435
 */
SECOC_LOCAL void SecOC_ClearLowBit(uint8* numArr, uint16 numLen, uint16 shiftBits);

/**
 * @brief Saves the input PduInfo to the queue.
 * @param[in] index : SECOC Channel index.
 * @param[in] RxPduId : PduId of received message.
 * @param[in] PduInfoPtr : PduLength and PDU buffer.
 * @return boolean
 * @retval TRUE : Save data to queue success
 * @retval FALSE : Save data to queue failed
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70436
 */
SECOC_LOCAL void SecOC_DataInQueue(uint16 index, PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief Retrieves the input PduInfo from the queue.
 * @param[in] index : SECOC Channel index.
 * @param[out] RxPduIdPtr : PduId of received message.
 * @param[out] PduInfoPtr : PduLength and PDU buffer.
 * @return Std_ReturnType
 * @retval E_OK : Get data from queue success
 * @retval E_NOT_OK : Get data from queue failed
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70437
 */
SECOC_LOCAL Std_ReturnType SecOC_DataOutQueue(uint16 index, PduIdType* RxPduIdPtr, PduInfoType* PduInfoPtr);

/**
 * @brief Copies bits from the source to the destination.
 * @param[in] src : Pointer to the source buffer.
 * @param[inout] dst : Pointer to the destination buffer.
 * @param[in] srcIndex : Starting bit index in the source buffer.
 * @param[in] bitlength : Number of bits to copy.
 * @return None
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace       CPD-70438
 */
static void SecOC_CopyBitsFromSourceToDestination(const uint8* src, uint8* dst, uint16 srcIndex, uint16 bitlength);

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
/**
 * @brief Reports a DET error.
 * @param[in] ApiId : API ID of the function where the error occurred.
 * @param[in] ErrorId : DET error ID.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70439
 */
SECOC_LOCAL_INLINE void SECOC_DET_REPORT(uint8 ApiId, uint8 ErrorId);

/**
 * @brief Reports a DET runtime error.
 * @param[in] ApiId : API ID of the function where the error occurred.
 * @param[in] ErrorId : DET error ID.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-70440
 */
SECOC_LOCAL_INLINE void SECOC_DET_REPORT_RUNTIMEERROR(uint8 ApiId, uint8 ErrorId);
#else
#define SECOC_DET_REPORT(ApiId, ErrorId)
#define SECOC_DET_REPORT_RUNTIMEERROR(ApiId, ErrorId)
#endif

#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"
/* ============================================ internal data definitions =========================================== */

#define SECOC_START_SEC_VAR_INIT_PTR
#include "SecOC_MemMap.h"
/** @brief Pointer to configuration data. @range NA */
SECOC_LOCAL const SecOC_ConfigType* SecOC_ConfigPtr = NULL_PTR;
#define SECOC_STOP_SEC_VAR_INIT_PTR
#include "SecOC_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"

/**
 * Initialize the runtime parameters of the SecOC module.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
void SecOC_Init(const SecOC_ConfigType* config)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    uint8 detErrorId = E_OK;
    if (NULL_PTR == config)
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        uint16 index;
        /*Save the configuration pointer */
        if (NULL_PTR == SecOC_ConfigPtr)
        {
            SecOC_ConfigPtr = config;
        }
        ApplicationType  partId      = SecOC_GetApplicationID();
        SecOC_StateType* lInitStatus = SecOC_GetInitStatusPtr(partId);
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
        if (lInitStatus == NULL_PTR)
        {
            detErrorId = SECOC_E_ERROR_PARTITION;
        }
        else if ((*lInitStatus) == SECOC_INIT)
        {
            detErrorId = SECOC_E_INIT_FAILED;
        }
        else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
        {
#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)
            for (index = 0u; index < SECOC_TX_PDU_PRO_NUM; index++)
            {
                if (SecOC_GetTxChannelApplicationId(index) == partId)
                {
                    SecOC_TxChannelReset(index);
                }
            }
#endif /* SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U */

#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
            for (index = 0u; index < SECOC_RX_PDU_PRO_NUM; index++)
            {
                if (SecOC_GetRxChannelApplicationId(index) == partId)
                {
                    SecOC_RxChannelReset(index);
                    SecOC_RxRtDataPtr(index)->queueManage.freeQueSize =
                        (uint16)SecOC_GetRxPduProcessingDataPtr(index)->SecOCReceptionPduQueueSize;
                    SecOC_RxRtDataPtr(index)->queueManage.freeQueStartIdx = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->queueManage.queueFrameCnt   = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->queueManage.queueHead       = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->queueManage.queueTail       = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->queueManage.queueInfoIdx    = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->rxOverrideStatus            = SECOC_OVERRIDE_CANCEL;
                    SecOC_RxRtDataPtr(index)->rxNumOfMsgToOverride        = SECOC_CONST_0U;
                    SecOC_RxRtDataPtr(index)->rxBakNumOfMsgToOverride     = SECOC_CONST_0U;
                }
            }
#endif /* SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U */
            /*Set the Init status */
            *lInitStatus = SECOC_INIT;
        }
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_INIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}

/**
 * This service is used to deinitialize the SecOC module and clear all receive
 * buffers and send buffers.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
void SecOC_DeInit(void)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    ApplicationType  partId      = SecOC_GetApplicationID();
    SecOC_StateType* lInitStatus = SecOC_GetInitStatusPtr(partId);
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    uint8 detErrorId = E_OK;
    if (lInitStatus == NULL_PTR)
    {
        detErrorId = SECOC_E_ERROR_PARTITION;
    }
    else if ((*lInitStatus) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        uint16 index;

#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)
        for (index = 0u; index < SECOC_TX_PDU_PRO_NUM; index++)
        {
            if (SecOC_GetTxChannelApplicationId(index) == partId)
            {
                SecOC_TxChannelReset(index);
            }
        }
#endif /* SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U */

#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
        for (index = 0u; index < SECOC_RX_PDU_PRO_NUM; index++)
        {
            if (SecOC_GetRxChannelApplicationId(index) == partId)
            {
                SecOC_RxChannelReset(index);
                SecOC_RxRtDataPtr(index)->rxOverrideStatus        = SECOC_OVERRIDE_CANCEL;
                SecOC_RxRtDataPtr(index)->rxNumOfMsgToOverride    = SECOC_CONST_0U;
                SecOC_RxRtDataPtr(index)->rxBakNumOfMsgToOverride = SECOC_CONST_0U;
            }
        }
#endif /* SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U */

        /*Set the Init status */
        *lInitStatus = SECOC_UNINIT;
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_DEINIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}

/**
 * This service is used to obtain the version information of the SecOC module.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
void SecOC_GetVersionInfo(Std_VersionInfoType* versioninfo)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (SECOC_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        SECOC_DET_REPORT(SECOC_GETVERSIONINFO_ID, SECOC_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->vendorID         = SECOC_VENDOR_ID;
        versioninfo->moduleID         = SECOC_MODULE_ID;
        versioninfo->sw_major_version = SECOC_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = SECOC_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = SECOC_C_SW_PATCH_VERSION;
    }
}

/**
 * This service is used by the upper module to request the transmission of
 * PDU and put the PDU information into the buffer of SecOC.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    /*not initialized*/
    uint8 detErrorId = E_OK;
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*TxPduId error*/
    else if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    else if (PduInfoPtr == NULL_PTR)
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U)
        const SecOC_PbTxPduProcessingType*       txPduProcessingPtr     = SecOC_GetTxPduProcessingPtr(TxPduId);
        const SecOC_TxPduProcessingType*         txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(TxPduId);
        const SecOC_SameBufferPduCollectionType* sameBufferPtr          = txPduProcessingDataPtr->SecOCSameBufferPduRef;

        /* Not use sameBufferCollection */
        if ((txPduProcessingPtr->SecOCAuthenticPduBuffLength >= PduInfoPtr->SduLength)
            && ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U])))
        {
            (void)IStdLib_MemCpy(
                txPduProcessingPtr->SecOCAuthenticPduBufferRef,
                PduInfoPtr->SduDataPtr,
                PduInfoPtr->SduLength);
            if (NULL_PTR != sameBufferPtr)
            {
                sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
            }
            result = E_OK;
        }
        else
        {
            result = E_NOT_OK;
        }

        if (E_OK == result)
        {
            SchM_Enter_SecOC_TxData();
            SecOC_TxRtDataPtr(TxPduId)->txPduStatus = SECOC_TX_CALC;
            SECOC_SET_TX_EVENT(TxPduId, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
            SecOC_TxRtDataPtr(TxPduId)->txPduLength = PduInfoPtr->SduLength;
            /* @req SWS_SecOC_00225,SWS_SecOC_00226 */
            SecOC_TxRtDataPtr(TxPduId)->txAuthBuildCnt = SECOC_CONST_0U;
            SchM_Exit_SecOC_TxData();
        }
#endif /* SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U */
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_IFTRANSMIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}

/**
 * This service is used by the upper module to request the transmission of PDU.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_TpTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    uint8 detErrorId = E_OK;
    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*TxPduId error*/
    /* PRQA S 2991,2995,2996 ++ */ /* VL_SecOC_2991,VL_SecOC_2995,VL_SecOC_2996 */
    else if (!((TxPduId >= SECOC_TX_IF_PDU_NUM) && (TxPduId < SECOC_TX_PDU_PRO_NUM)))
    /* PRQA S 2991,2995,2996 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
    else if (PduInfoPtr == NULL_PTR)
    /* PRQA S 2880 -- */
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
        const SecOC_PbTxPduProcessingType* txPduProcessingPtr    = SecOC_GetTxPduProcessingPtr(TxPduId);
        const SecOC_TxPduSecuredAreaType*  SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
        uint32                             txPduSecuredAreaLength;
        if (NULL_PTR != SecOCTxPduSecuredArea)
        {
            /* @req SWS_SecOC_00313 */
            txPduSecuredAreaLength =
                SecOCTxPduSecuredArea->SecOCSecuredTxPduLength + SecOCTxPduSecuredArea->SecOCSecuredTxPduOffset;
            if (txPduSecuredAreaLength > PduInfoPtr->SduLength)
            {
                result = E_NOT_OK;
            }
        }

        if (result == E_OK)
        {
            SchM_Enter_SecOC_TxData();
            SecOC_TxRtDataPtr(TxPduId)->txPduStatus = SECOC_TX_CALC;
            SECOC_SET_TX_EVENT(TxPduId, SECOC_TX_REQUEST);
            SecOC_TxRtDataPtr(TxPduId)->txPduLength = PduInfoPtr->SduLength;
            /* @req SWS_SecOC_00225,SWS_SecOC_00226 */
            SecOC_TxRtDataPtr(TxPduId)->txAuthBuildCnt = SECOC_CONST_0U;
            SchM_Exit_SecOC_TxData();
        }
#else
        (void)(TxPduId);
        (void)(PduInfoPtr);
#endif /* SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U */
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_TPTRANSMIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}

/**
 * Requests cancellation of an ongoing transmission of a PDU in a lower layer
 * communication module.After a successful cancellation,the buffer of the Tx
 * PDU is emptied.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_IfCancelTransmit(PduIdType TxPduId)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    /*not initialized*/
    uint8 detErrorId = E_OK;
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    else if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U)
        const SecOC_TxSecuredPduType*       txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(TxPduId);
        const SecOC_TxAuthenticPduType*     txAuthenticPduPtr;
        const SecOC_TxCryptographicPduType* txCryptographicPduPtr;
        Std_ReturnType                      result2 = E_OK;

        if (NULL_PTR != txSecuredPduPtr)
        {
            result = PduR_SecOCCancelTransmit(txSecuredPduPtr->SecOCTxPduRAsLowerLayerId);
        }
        else
        {
            txAuthenticPduPtr     = SecOC_GetTxAuthenticPduPtr(TxPduId);
            txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(TxPduId);
            result                = PduR_SecOCCancelTransmit(txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId);
            result2               = PduR_SecOCCancelTransmit(txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_TxChannelReset(TxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_IFCANCELTRANSMIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}

/**
 * Requests cancellation of an ongoing transmission of a PDU in a lower layer
 * communication module.After a successful cancellation,the buffer of the Tx
 * PDU is emptied.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_TpCancelTransmit(PduIdType TxPduId)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    /*not initialized*/
    uint8 detErrorId = E_OK;
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((TxPduId < SECOC_TX_IF_PDU_NUM) || (TxPduId >= SECOC_TX_PDU_PRO_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
        const SecOC_TxSecuredPduType*       txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(TxPduId);
        const SecOC_TxAuthenticPduType*     txAuthenticPduPtr;
        const SecOC_TxCryptographicPduType* txCryptographicPduPtr;
        Std_ReturnType                      result2 = E_OK;

        if (NULL_PTR != txSecuredPduPtr)
        {
            result = PduR_SecOCCancelTransmit(txSecuredPduPtr->SecOCTxPduRAsLowerLayerId);
        }
        else
        {
            txAuthenticPduPtr     = SecOC_GetTxAuthenticPduPtr(TxPduId);
            txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(TxPduId);
            result                = PduR_SecOCCancelTransmit(txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId);
            result2               = PduR_SecOCCancelTransmit(txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_TxChannelReset(TxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#else
        /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
        (void)(TxPduId);
        /* PRQA S 2880 -- */
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_TPCANCELTRANSMIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}

/**
 * Requests cancellation of an ongoing reception of a PDU in a lower layer
 * transport protocol module.After a successful cancellation,the buffer of
 * the Rx PDU is cleared.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_TpCancelReceive(PduIdType RxPduId)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    /*not initialized*/
    uint8 detErrorId = E_OK;
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((RxPduId < SECOC_RX_IF_PDU_NUM) || (RxPduId >= SECOC_RX_PDU_PRO_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
        const SecOC_RxSecuredPduType*       rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(RxPduId);
        const SecOC_RxAuthenticPduType*     rxAuthenticPduPtr;
        const SecOC_RxCryptographicPduType* rxCryptographicPduPtr;
        Std_ReturnType                      result2 = E_OK;

        if (NULL_PTR != rxSecuredPduPtr)
        {
            result = PduR_SecOCCancelReceive(rxSecuredPduPtr->SecOCRxPduRAsLowerLayerPduId);
        }
        else
        {
            rxAuthenticPduPtr     = SecOC_GetRxAuthenticPduPtr(RxPduId);
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(RxPduId);
            result                = PduR_SecOCCancelReceive(rxAuthenticPduPtr->SecOCRxPduRAsLowerLayerPduId);
            result2               = PduR_SecOCCancelReceive(rxCryptographicPduPtr->SecOCRxPduRAsLowerLayerPduId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_RxChannelReset(RxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#else
        /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
        (void)(RxPduId);
        /* PRQA S 2880 -- */
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_TPCANCELRECEIVE_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}

/**
 * This service provides the ability to force specific behaviour of SecOc:
 * accept or drop an I-PDU with or without performing the verification of
 * authenticator or independent of the authenticator verification result,
 * and force a specific result for SecOC_VerificationResultType allowing
 * additional fault handling in the application.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
Std_ReturnType SecOC_VerifyStatusOverride(
    uint16                   ValueID,
    SecOC_OverrideStatusType overrideStatus,
    uint8                    numberOfMessagesToOverride)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    uint8 detErrorId = E_OK;
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        const SecOC_PbRxPduProcessingType* rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(SECOC_CONST_0U);
        uint16                             index;
        SchM_Enter_SecOC_RxData();
        for (index = SECOC_CONST_0U; index < SECOC_RX_PDU_PRO_NUM; index++)
        {
#if (STD_ON == SECOC_OVERRIDE_STATUS_WITH_DATA_ID)
            if (rxPduProcessingPtr[index].SecOCDataId == ValueID)
#else
            if (rxPduProcessingPtr[index].SecOCFreshnessValueId == ValueID)
#endif
            {
                if ((SECOC_OVERRIDE_DROP_UNTIL_NOTICE == overrideStatus)
                    || (SECOC_OVERRIDE_DROP_UNTIL_LIMIT == overrideStatus) || (SECOC_OVERRIDE_CANCEL == overrideStatus))
                {
                    SecOC_RxRtDataPtr(index)->rxOverrideStatus     = overrideStatus;
                    SecOC_RxRtDataPtr(index)->rxNumOfMsgToOverride = numberOfMessagesToOverride;
                    ret                                            = E_OK;
                }
#if (STD_ON == SECOC_ENABLE_FORCED_PASS_OVERRIDE)
                else
                {
                    SecOC_RxRtDataPtr(index)->rxOverrideStatus     = overrideStatus;
                    SecOC_RxRtDataPtr(index)->rxNumOfMsgToOverride = numberOfMessagesToOverride;
                    ret                                            = E_OK;
                }
#endif /* STD_ON == SECOC_ENABLE_FORCED_PASS_OVERRIDE */
            }
        }
        SchM_Exit_SecOC_RxData();
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_VERIFYSTATUSOVERRIDE_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return ret;
}

#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
/**
 * The service provides the ability to enable the sending of un-authenticated
 * PDU to lower layer.When the interface corresponding to the freshness value
 * is enabled, SecOC builds the authentication information with the default value.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020 ++ */             /* VL_MTR_SecOC_STLIN */
Std_ReturnType
    SecOC_SendDefaultAuthenticationInformation(uint16 FreshnessValueID, boolean sendDefaultAuthenticationInformation)
/* PRQA S 6020 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        SchM_Enter_SecOC_TxData();
        const SecOC_PbTxPduProcessingType* txPduProcessingPtr;
        uint16                             index;

        for (index = SECOC_CONST_0U; index < SECOC_TX_PDU_PRO_NUM; index++)
        {
            txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
            if (txPduProcessingPtr->SecOCFreshnessValueId == FreshnessValueID)
            {
                SecOC_TxRtDataPtr(index)->txDefaultAuthInfoEnable = sendDefaultAuthenticationInformation;
                ret                                               = E_OK;
            }
        }
        SchM_Exit_SecOC_TxData();
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_SENDDEFAULTAUTHENTICATIONINFORMATION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return ret;
}
#endif
#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * When the lower layer communication interface module receives the PDU,
 * it notifies SecOC to receive the PDU.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020, 1532, 5017 ++ */ /* VL_MTR_SecOC_STLIN, VL_QAC_OneFunRef, VL_SecOC_5017 */
void SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6020, 1532, 5017 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else if (RxPduId >= SECOC_RX_SEC_IF_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
        const SecOC_RxSecuredPduType*       rxSecuredPduPtr;
        const SecOC_RxAuthenticPduType*     rxAuthenticPduPtr;
        const SecOC_RxCryptographicPduType* rxCryptographicPduPtr;
        uint16                              index;
        boolean                             rxPduTag = FALSE;

        for (index = SECOC_CONST_0U; index < SECOC_RX_IF_PDU_NUM; index++)
        {
            rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
            if (NULL_PTR != rxSecuredPduPtr)
            {
                if (rxSecuredPduPtr->SecOCRxSecuredLayerPduId == RxPduId)
                {
                    SecOC_RxIfSecuredPduIndication(index, RxPduId, PduInfoPtr);
                    rxPduTag = TRUE;
                }
            }
            else
            {
                rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
                if (rxAuthenticPduPtr->SecOCRxAuthenticPduId == RxPduId)
                {
                    SecOC_RxIfAuthenticPduIndication(index, PduInfoPtr);
                    rxPduTag = TRUE;
                }

                rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
                if (rxCryptographicPduPtr->SecOCRxCryptographicPduId == RxPduId)
                {
                    SecOC_RxIfCryptographicPduIndication(index, PduInfoPtr);
                    rxPduTag = TRUE;
                }
            }
            if (rxPduTag)
            {
                break;
            }
        }
#endif /* SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U */
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_RXINDICATION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * Called after an I-PDU has been received via the TP API, the result indicates
 * whether the transmission was successful or not.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_TpRxIndication(PduIdType id, Std_ReturnType result)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((id >= SECOC_RX_SEC_PDU_NUM) || (id < SECOC_RX_SEC_IF_PDU_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U)

        SchM_Enter_SecOC_RxData();
        uint16  index;
        boolean cryptoTag = FALSE;
        /* PRQA S 0771 ++ */ /* VL_SecOC_0771 */
        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_PRO_NUM; index++)
        /* PRQA S 0771 -- */
        {
            if (SecOC_RxRtDataPtr(index)->curPduId == id)
            {
                break;
            }
            else if (SecOC_RxRtDataPtr(index)->curCryptoPduId == id)
            {
                cryptoTag = TRUE;
                break;
            }
        }
        SchM_Exit_SecOC_RxData();

        if ((index < SECOC_RX_PDU_PRO_NUM)
            && ((SECOC_IS_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER))
                || (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER)))
            && (((SECOC_CONST_0U == SecOC_RxRtDataPtr(index)->sduRemaining))
                || ((cryptoTag) && (SECOC_CONST_0U == SecOC_RxRtDataPtr(index)->sduCryptoRemaining))))
        {
            if (E_OK == result)
            {
                SchM_Enter_SecOC_RxData();
                const SecOC_RxSecuredPduType* rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
                if (NULL_PTR != rxSecuredPduPtr)
                {
                    /* @req SWS_SecOC_00085 */
                    SECOC_CLR_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                }
                else
                {
                    if (cryptoTag)
                    {
                        /* @req SWS_SecOC_00086 */
                        SECOC_CLR_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER);
                        SECOC_SET_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU);
                    }
                    else
                    {
                        SECOC_SET_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU);
                        SECOC_CLR_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    }
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                }
                SecOC_RxRtDataPtr(index)->rxPduStatus = SECOC_RX_CALC;
                SchM_Exit_SecOC_RxData();
            }
            else
            {
                /* @req SWS_SecOC_00089 */
                /* Error occur,Drop the Secured I-PDU and free all buffers */
                SecOC_RxChannelReset(index);
            }
        }
        else
        {
            /* @req SWS_SecOC_00084 */
            /* Reception of a Secured I-PDU not completed,Directly return */
        }
#else
        /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
        (void)(id);
        (void)(result);
        /* PRQA S 2880 -- */
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_TPRXINDICATION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * When the lower communication interface module completes the transmission
 * action of the PDU, it notifies the PduR of the transmission result and clears
 * the corresponding state and event of the PDU.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    else if (TxPduId >= SECOC_TX_SEC_IF_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U)

        SecOC_TxPduRunTimeType* txRtPtr;
        PduIdType               upLayerPduId;

        uint16 index = SecOC_FindProcessTxPduIndex(TxPduId, SECOC_CONST_0U, SECOC_TX_IF_PDU_NUM);

        if (index < SECOC_TX_IF_PDU_NUM)
        {
            const SecOC_PbTxPduProcessingType* txPduProcessingPtr     = SecOC_GetTxPduProcessingPtr(index);
            const SecOC_TxPduProcessingType*   txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
            const SecOC_TxSecuredPduType*      txSecuredPduPtr        = SecOC_GetTxSecuredPduPtr(index);
            /* @req SWS_SecOC_00063 */
            upLayerPduId = txPduProcessingPtr->SecOCTxAuthenticPduLayer->SecOCTxPduRAsUpLayerId;
            if ((NULL_PTR != txSecuredPduPtr)
                || ((SECOC_IS_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED))
                    && (SECOC_IS_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED))))
            {
                PduR_SecOCIfTxConfirmation(upLayerPduId, result);

                SchM_Enter_SecOC_TxData();
                /* @req SWS_SecOC_00064 */
                /* Free the buffer that contains the Secured I-PDU */
                txRtPtr             = SecOC_TxRtDataPtr(index);
                txRtPtr->txPduReady = FALSE;
                if (NULL_PTR != txPduProcessingDataPtr->SecOCSameBufferPduRef)
                {
                    *(txPduProcessingDataPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse) = FALSE;
                }

                /* Last process complete,clear the tx status */
                txRtPtr->txPduStatus    = SECOC_TX_NONE;
                txRtPtr->txAuthBuildCnt = SECOC_CONST_0U;
                /* Clear tx event */
                SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
                SchM_Exit_SecOC_TxData();
            }
        }

#endif /* SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U */
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_TXCONFIRMATION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * When the lower module completes the transmission action of the PDU through
 * the TP API, it notifies PduR of the transmission result and clears the
 * corresponding state and event of the PDU.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_TpTxConfirmation(PduIdType id, Std_ReturnType result)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    else if (id >= SECOC_TX_SEC_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > 0)
        uint16 index = SecOC_FindProcessTxPduIndex(id, SECOC_CONST_0U, SECOC_TX_PDU_PRO_NUM);

        if (index < SECOC_TX_PDU_PRO_NUM)
        {
            const SecOC_PbTxPduProcessingType*   txPduProcessingPtr     = SecOC_GetTxPduProcessingPtr(index);
            const SecOC_TxPduProcessingType*     txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
            const SecOC_TxSecuredPduType*        txSecuredPduPtr        = SecOC_GetTxSecuredPduPtr(index);
            const SecOC_TxAuthenticPduLayerType* txAuthenticPduLayerPtr = SecOC_GetTxAuthenticPduLayerPtr(index);
            PduIdType                            upLayerPduId = txAuthenticPduLayerPtr->SecOCTxPduRAsUpLayerId;
            if ((NULL_PTR != txSecuredPduPtr)
                || ((SECOC_IS_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED))
                    && (SECOC_IS_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED))))
            {
                /* @req SWS_SecOC_00074 */
                if (SECOC_IFPDU == txAuthenticPduLayerPtr->SecOCPduType)
                {
                    if (E_OK == result)
                    {
                        PduR_SecOCIfTxConfirmation(upLayerPduId, result);
                    }
                }
                else
                {
                    PduR_SecOCTpTxConfirmation(upLayerPduId, result);
                }
                SchM_Enter_SecOC_TxData();
                /* Free the buffer that contains the Secured I-PDU */
                SecOC_TxPduRunTimeType* txRtPtr = SecOC_TxRtDataPtr(index);
                txRtPtr->txPduReady             = FALSE;
                if (NULL_PTR != txPduProcessingDataPtr->SecOCSameBufferPduRef)
                {
                    *(txPduProcessingDataPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse) = FALSE;
                }

                /* Last process complete,clear the tx status */
                txRtPtr->txPduStatus    = SECOC_TX_NONE;
                txRtPtr->txAuthBuildCnt = SECOC_CONST_0U;
                /* Clear tx event */
                SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
                SchM_Exit_SecOC_TxData();
            }
        }
#else
        (void)(id);
        (void)(result);
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_TPTXCONFIRMATION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * The upper module PduR obtains the transmission data from SecOC.
 * When the length of the prepared data is less than PduInfoPtr->SduDataPtr,
 * SecOC copies the prepared data to PduInfoPtr->SduDataPtr and updates
 * PduInfoPtr->SduLength to the length of the prepared data.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020, 6080, 1532 ++ */ /* VL_MTR_SecOC_STLIN, VL_MTR_SecOC_STPTH, VL_QAC_OneFunRef */
/* PRQA S 5017 ++ */             /* VL_SecOC_5017 */
Std_ReturnType SecOC_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 5017 -- */
/* PRQA S 6020, 6080, 1532 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId     = SecOC_GetApplicationID();
    uint8           detErrorId = E_OK;

    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    else if (TxPduId >= SECOC_TX_SEC_IF_PDU_NUM)
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U)
        const SecOC_PbTxPduProcessingType*  txPduProcessingPtr;
        const SecOC_TxSecuredPduType*       txSecuredPduPtr;
        const SecOC_TxAuthenticPduType*     txAuthenticPduPtr     = NULL_PTR;
        const SecOC_TxCryptographicPduType* txCryptographicPduPtr = NULL_PTR;
        const SecOC_UseMessageLinkType*     txUseMessageLinkPtr   = NULL_PTR;
        const uint8*                        srcPtr                = NULL_PTR;
        uint16                              index;
        uint16                              securedPduLength = SECOC_CONST_0U;
        uint16                              authInfoTxLength;
        uint16                              msgLinkLen = SECOC_CONST_0U;
        uint8                               fvTruncLenInBits;
        uint8                               headerLenInBytes;
        boolean                             txSecuredPduFlag   = FALSE;
        boolean                             txAuthenticPduFlag = FALSE;
        boolean                             txPduTag           = FALSE;

        for (index = SECOC_CONST_0U; index < SECOC_TX_IF_PDU_NUM; index++)
        {
            txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
            txSecuredPduPtr    = SecOC_GetTxSecuredPduPtr(index);
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == TxPduId)
                {
                    txSecuredPduFlag = TRUE;
                    txPduTag         = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == TxPduId)
                {
                    txAuthenticPduFlag = TRUE;
                    txPduTag           = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == TxPduId)
                {
                    txPduTag = TRUE;
                }
            }
            if (txPduTag)
            {
                break;
            }
        }

        if ((index < SECOC_TX_IF_PDU_NUM) && (SecOC_TxRtDataPtr(index)->txPduReady))
        {
            /* Calculate pdu length */
            authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
            fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
            if (txSecuredPduFlag)
            {
                SchM_Enter_SecOC_TxData();
                /* PRQA S 2813 ++ */ /* VL_QAC_DerefNullPtr */
                headerLenInBytes = txSecuredPduPtr->SecOCAuthPduHeaderLength;
                /* PRQA S 2813 -- */
                securedPduLength = (uint16)headerLenInBytes + (uint16)SecOC_TxRtDataPtr(index)->txPduLength
                                   + SECOC_BIT_TO_BYTE(authInfoTxLength + (uint16)fvTruncLenInBits);
                if (securedPduLength < txSecuredPduPtr->SecOCTxSecuredPduBuffLength)
                {
                    /* Fill unused byte */
                    for (uint16 i = securedPduLength; i < txSecuredPduPtr->SecOCTxSecuredPduBuffLength; i++)
                    {
                        txSecuredPduPtr->SecOCTxSecuredPduBufferRef[i] =
                            SecOC_GetTxPduProcessingDataPtr(index)->SecOCTxPduUnusedAreasDefault;
                    }
                    securedPduLength = txSecuredPduPtr->SecOCTxSecuredPduBuffLength;
                }
                srcPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
                SchM_Exit_SecOC_TxData();
            }
            else if (txAuthenticPduFlag)
            {
                SchM_Enter_SecOC_TxData();
                /* PRQA S 2813 ++ */ /* VL_QAC_DerefNullPtr */
                headerLenInBytes = txAuthenticPduPtr->SecOCAuthPduHeaderLength;
                /* PRQA S 2813 -- */
                securedPduLength = (uint16)headerLenInBytes + (uint16)SecOC_TxRtDataPtr(index)->txPduLength;
                srcPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef; /* PRQA S 2813 */ /* VL_QAC_DerefNullPtr */
                SchM_Exit_SecOC_TxData();
            }
            else
            {
                txUseMessageLinkPtr = SecOC_GetTxUseMessageLinkPtr(index);
                if (NULL_PTR != txUseMessageLinkPtr)
                {
                    msgLinkLen = txUseMessageLinkPtr->SecOCMessageLinkLen;
                }
                securedPduLength = SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits + authInfoTxLength + msgLinkLen);
                /* PRQA S 2813 ++ */ /* VL_QAC_DerefNullPtr */
                if (txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength > securedPduLength)
                /* PRQA S 2813 -- */
                {
                    /* Fill unused byte */
                    /* PRQA S 2813 ++ */ /* VL_QAC_DerefNullPtr */
                    for (PduLengthType i = securedPduLength;
                         i < txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength;
                         i++)
                    {
                        txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef[i] =
                            SecOC_GetTxPduProcessingDataPtr(index)->SecOCTxPduUnusedAreasDefault;
                    }
                    securedPduLength = txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength;
                    /* PRQA S 2813 -- */
                }
                /* PRQA S 2813 ++ */ /* VL_QAC_DerefNullPtr */
                srcPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
                /* PRQA S 2813 -- */
            }

            if ((PduInfoPtr->SduLength >= securedPduLength) && (NULL_PTR != srcPtr))
            {
                /* @req SWS_SecOC_00068 */
                /* Copy the Secured I-PDU to the lower layer*/
                (void)IStdLib_MemCpy(PduInfoPtr->SduDataPtr, srcPtr, securedPduLength);
                PduInfoPtr->SduLength = securedPduLength;

                result = E_OK;
            }
            else
            {
                result = E_NOT_OK;
            }
        }
        else
        {
            /* @req SWS_SecOC_00150 */
            /* No Secured I-PDU available,Return E_NOT_OK */
            result = E_NOT_OK;
        }
#endif /* SECOC_TX_IF_PDU_NUM > SECOC_CONST_0U */
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_TRIGGERTRANSMIT_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * After receiving the PDU, the lower module PduR provides the received I-PDU
 * segment to the SecOC module, and the next segment of the PDU is provided for
 * each call.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
BufReq_ReturnType SecOC_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    BufReq_ReturnType result = BUFREQ_E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    uint8 detErrorId = E_OK;
    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((id >= SECOC_RX_SEC_PDU_NUM) || (id < SECOC_RX_SEC_IF_PDU_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    /* PRQA S 2880 -- */
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_RX_TP_PDU_NUM > 0)
        SchM_Enter_SecOC_RxData();

        uint16  index;
        boolean cryptoTag = FALSE;
        /* PRQA S 0771 ++ */ /* VL_SecOC_0771 */
        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_PRO_NUM; index++)
        /* PRQA S 0771 -- */
        {
            if (SecOC_RxRtDataPtr(index)->curPduId == id)
            {
                break;
            }
            else if (SecOC_RxRtDataPtr(index)->curCryptoPduId == id)
            {
                cryptoTag = TRUE;
                break;
            }
        }

        if ((index < SECOC_RX_PDU_PRO_NUM)
            && ((SECOC_IS_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER))
                || (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER))))
        {
            SecOC_RxPduRunTimeType* rxRtPtr = SecOC_RxRtDataPtr(index);
            if (SECOC_CONST_0U == info->SduLength)
            {
                /* Query the current amount of available buffer */
                if (cryptoTag)
                {
                    *bufferSizePtr = rxRtPtr->sduCryptoRemaining;
                }
                else
                {
                    *bufferSizePtr = rxRtPtr->sduRemaining;
                }
                result = BUFREQ_OK;
            }
            else
            {
                if (cryptoTag)
                {
                    if (rxRtPtr->sduCryptoRemaining >= info->SduLength)
                    {
                        /* @req SWS_SecOC_00083 */
                        (void)IStdLib_MemCpy(
                            &rxRtPtr->tpRxCryptoDestBufPtr[rxRtPtr->tpRxCryptoPduOffset],
                            info->SduDataPtr,
                            info->SduLength);
                        rxRtPtr->tpRxCryptoPduOffset += info->SduLength;
                        rxRtPtr->sduCryptoRemaining -= info->SduLength;
                        *bufferSizePtr = rxRtPtr->sduCryptoRemaining;
                        result         = BUFREQ_OK;
                    }
                }
                else
                {
                    if (rxRtPtr->sduRemaining >= info->SduLength)
                    {
                        /* @req SWS_SecOC_00083 */
                        (void)IStdLib_MemCpy(
                            &rxRtPtr->tpRxDestBufPtr[rxRtPtr->tpRxPduOffset],
                            info->SduDataPtr,
                            info->SduLength);
                        rxRtPtr->tpRxPduOffset += info->SduLength;
                        rxRtPtr->sduRemaining -= info->SduLength;
                        *bufferSizePtr = rxRtPtr->sduRemaining;
                        result         = BUFREQ_OK;
                    }
                }
            }
        }
        else
        {
            /* Not find coresponding runtime data,return BUFREQ_E_NOT_OK */
        }
        SchM_Exit_SecOC_RxData();
#else
        (void)(id);
        (void)(info);
        *bufferSizePtr = 0u;
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_COPYRXDATA_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return result;
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * This function is called by the upper-layer PduR to obtain the I-PDU segment
 * to be transferred. When retry->TpDataState is TP_DATARETRY, PduR refetches
 * the I-PDU segment offset from the current position by length retry->TxTpDataCnt.
 */
/* PRQA S 1503, 6010, 6060 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STCAL */
/* PRQA S 6070, 6010, 6080 ++ */ /* VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6050, 6030, 6020 ++ */ /* VL_MTR_SecOC_STM19, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
/* PRQA S 1532, 5017 ++ */       /* VL_QAC_OneFunRef, VL_SecOC_5017 */
/* PRQA S 2889 ++ */             /* VL_QAC_OneFunRef */
BufReq_ReturnType
    SecOC_CopyTxData(PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr)
/* PRQA S 2889 -- */
/* PRQA S 1532, 5017 -- */
/* PRQA S 6050, 6030, 6020 -- */
/* PRQA S 6070, 6010, 6080 -- */
/* PRQA S 1503, 6010, 6060 -- */
{
    BufReq_ReturnType bufferReqStatus = BUFREQ_E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    uint8 detErrorId = E_OK;
    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((id >= SECOC_TX_SEC_PDU_NUM) || (id < SECOC_TX_SEC_IF_PDU_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    /* PRQA S 2880 -- */
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
        const SecOC_TxSecuredPduType*       txSecuredPduPtr;
        const SecOC_TxAuthenticPduType*     txAuthenticPduPtr;
        const SecOC_TxCryptographicPduType* txCryptographicPduPtr;
        SecOC_TxPduRunTimeType*             txRtPtr;
        const uint8*                        srcPtr     = NULL_PTR;
        PduLengthType                       dataLength = SECOC_CONST_0U;
        uint16                              index;

        boolean       txPduTag       = FALSE;
        boolean       txCryptoPduTag = FALSE;
        PduLengthType lTxPduoffset;

        for (index = SECOC_TX_IF_PDU_NUM; index < SECOC_TX_PDU_PRO_NUM; index++)
        {
            txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
            txRtPtr         = SecOC_TxRtDataPtr(index);
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == id)
                {
                    dataLength   = txRtPtr->txSecuredPduLen;
                    srcPtr       = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
                    lTxPduoffset = txRtPtr->tpTxPduOffset;
                    txPduTag     = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == id)
                {
                    dataLength   = txRtPtr->txAuthenticPduLen;
                    srcPtr       = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
                    lTxPduoffset = txRtPtr->tpTxPduOffset;
                    txPduTag     = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == id)
                {
                    dataLength     = txRtPtr->txCryptographicPduLen;
                    srcPtr         = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
                    lTxPduoffset   = txRtPtr->tpTxCryptoPduOffset;
                    txCryptoPduTag = TRUE;
                }
            }
            if ((txPduTag) || (txCryptoPduTag))
            {
                break;
            }
        }
        if ((!txPduTag) && (!txCryptoPduTag))
        {
            return bufferReqStatus;
        }

        if (retry != NULL_PTR)
        {
            if (TP_DATARETRY == retry->TpDataState)
            {
                if (lTxPduoffset >= retry->TxTpDataCnt)
                {
                    lTxPduoffset -= retry->TxTpDataCnt;
                }
                else
                {
                    return BUFREQ_E_NOT_OK;
                }
            }
            else if (TP_DATACONF == retry->TpDataState)
            {
                if (retry->TxTpDataCnt == dataLength)
                {
                    /* TP transmit finished, Clear internal buffer */
                    return BUFREQ_OK;
                }
            }
            else
            {
                /* Nothing to do here */
            }
        }

        if (!txRtPtr->txPduReady)
        {
            return BUFREQ_E_BUSY;
        }

        if ((lTxPduoffset + info->SduLength) > dataLength)
        {
            return BUFREQ_E_NOT_OK;
        }

        if ((info->SduLength > SECOC_CONST_0U) && (NULL_PTR != srcPtr))
        {
            SchM_Enter_SecOC_TxData();
            /* @req SWS_SecOC_00073 */
            (void)IStdLib_MemCpy(info->SduDataPtr, &srcPtr[lTxPduoffset], info->SduLength);
            lTxPduoffset += info->SduLength;
            if (txPduTag)
            {
                txRtPtr->tpTxPduOffset = lTxPduoffset;
            }
            else
            {
                txRtPtr->tpTxCryptoPduOffset = lTxPduoffset;
            }

            SchM_Exit_SecOC_TxData();
        }
        *availableDataPtr = dataLength - lTxPduoffset;

        bufferReqStatus = BUFREQ_OK;
#else
        (void)(id);
        (void)(info);
        (void)(retry);
        *availableDataPtr = 0u;
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_COPYTXDATA_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return bufferReqStatus;
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * The PduR module calls this function to notify the SecOC module that the reception
 * process of the respective Secured I-PDU will start.When retry->TpDataState is
 * TP_DATARETRY, PduR refetches the I-PDU segment offset from the current position
 * by length retry->TxTpDataCnt.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 6080, 6050, 6030 ++ */ /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3, VL_MTR_SecOC_STMIF */
/* PRQA S 6020, 1532, 5017 ++ */ /* VL_MTR_SecOC_STLIN, VL_QAC_OneFunRef, VL_SecOC_5017 */
BufReq_ReturnType SecOC_StartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 6020, 1532, 5017 -- */
/* PRQA S 6080, 6050, 6030 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    BufReq_ReturnType bufReqRet = BUFREQ_E_NOT_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    uint8 detErrorId = E_OK;
    /*not initialized*/
    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    /*id error*/
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    else if ((id >= SECOC_RX_SEC_PDU_NUM) || (id < SECOC_RX_SEC_IF_PDU_NUM))
    /* PRQA S 2991,2995 -- */
    {
        detErrorId = SECOC_E_INVALID_PDU_SDU_ID;
    }
    /*NULL POINTER*/
    /* PRQA S 2880 ++ */ /* VL_SecOC_2880 */
    else if (NULL_PTR == bufferSizePtr)
    /* PRQA S 2880 -- */
    {
        detErrorId = SECOC_E_PARAM_POINTER;
    }
    else
#endif
    {
#if (SECOC_RX_TP_PDU_NUM > 0)
        const SecOC_PbRxPduProcessingType* rxPduProcessingPtr;
        const SecOC_RxPduProcessingType* rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(SECOC_RX_IF_PDU_NUM);
        const SecOC_RxSecuredPduType*    rxSecuredPduPtr;
        const SecOC_RxAuthenticPduType*  rxAuthenticPduPtr;
        const SecOC_RxCryptographicPduType* rxCryptographicPduPtr;
        uint16                              index;
        boolean                             rxPduTag = FALSE;

        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_PRO_NUM; index++)
        {
            rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
            rxSecuredPduPtr    = SecOC_GetRxSecuredPduPtr(index);
            if (NULL_PTR != rxSecuredPduPtr)
            {
                if (rxSecuredPduPtr->SecOCRxSecuredLayerPduId == id)
                {
                    PduLengthType lAuthenticLength = rxPduProcessingPtr->SecOCAuthenticPduBuffLength;

                    if ((rxSecuredPduPtr->SecOCAuthPduHeaderLength > 0u))
                    {
                        lAuthenticLength = 0u;
                        for (uint8 i = 0u; i < rxSecuredPduPtr->SecOCAuthPduHeaderLength; i++)
                        {
                            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
                            lAuthenticLength = (lAuthenticLength << (SECOC_CONST_8U * i)) | info->SduDataPtr[i];
                            /* PRQA S 4397 -- */
                        }
                    }
                    if (lAuthenticLength <= rxPduProcessingPtr->SecOCAuthenticPduBuffLength)
                    {
                        bufReqRet = SecOC_RxTpSecuredPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    }
                    else
                    {
                        bufReqRet = BUFREQ_E_NOT_OK;
                    }
                    rxPduTag = TRUE;
                }
            }
            else
            {
                rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
                if (rxAuthenticPduPtr->SecOCRxAuthenticPduId == id)
                {
                    PduLengthType lAuthenticLength = rxPduProcessingPtr->SecOCAuthenticPduBuffLength;

                    if ((rxAuthenticPduPtr->SecOCAuthPduHeaderLength > 0u))
                    {
                        lAuthenticLength = 0u;
                        for (uint8 i = 0u; i < rxAuthenticPduPtr->SecOCAuthPduHeaderLength; i++)
                        {
                            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
                            lAuthenticLength = (lAuthenticLength << (SECOC_CONST_8U * i)) | info->SduDataPtr[i];
                            /* PRQA S 4397 -- */
                        }
                    }
                    if (lAuthenticLength <= rxPduProcessingPtr->SecOCAuthenticPduBuffLength)
                    {
                        bufReqRet = SecOC_RxTpAuthenticPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    }
                    else
                    {
                        bufReqRet = BUFREQ_E_NOT_OK;
                    }
                    rxPduTag = TRUE;
                }

                rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
                if (rxCryptographicPduPtr->SecOCRxCryptographicPduId == id)
                {
                    bufReqRet = SecOC_RxTpCryptographicPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    rxPduTag  = TRUE;
                }
            }
            if (rxPduTag)
            {
                break;
            }
        }
#else
        (void)(id);
        (void)(info);
        (void)(TpSduLength);
        *bufferSizePtr = 0u;
#endif
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if (detErrorId != E_OK)
    /* PRQA S 2991,2995 -- */
    {
        SECOC_DET_REPORT(SECOC_STARTOFRECEPTION_ID, detErrorId);
    }
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    return bufReqRet;
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * CSM notifies SecOC that a Authenticator generate job has finished. If result
 * is equal to CRYPTO_E_BUSY, the generation of the authenticator is retried.
 * If the build fails, build failure handling is called.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_CsmGenerateJobFinishedIndication(const Crypto_JobType* job, Crypto_ResultType result)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    uint8 detErrorId = E_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        const SecOC_TxPduProcessingType* txPduProcessingDataPtr;
        uint16                           index;

        for (index = SECOC_CONST_0U; index < SECOC_TX_PDU_PRO_NUM; index++)
        {
            txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
            if (txPduProcessingDataPtr->SecOCTxAuthServiceConfigRef->SecOCJobId == job->jobId)
            {
                if (SECOC_IS_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_WAITING))
                {
                    if (E_OK == result)
                    {
                        SECOC_CLR_TX_EVENT(index, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
                        SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
                    }
                    else if (CRYPTO_E_BUSY == result)
                    {
                        /* @req SWS_SecOC_00151 */
                        /* CSM module reports a recoverable error,Initiate retry */
                        SecOC_TxGenerateAuthenticatorRetry(index);
                    }
                    else
                    {
                        /* @req SWS_SecOC_00229 */
                        SecOC_TxBuildAuthenticatorFailHandle(index);
                    }
                    break;
                }
            }
        }
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_CSMGENERATEJOBFINISHEDINDICATION_ID, detErrorId);
    }
#else
    (void)detErrorId;
#endif
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE_FAST
#include "SecOC_MemMap.h"
/**
 * CSM notifies SecOC that a Authenticator verify job has finished.If result
 * is equal to CRYPTO_E_BUSY, a new attempt is made to construct the authentication
 * message. If the validation fails, it is handled according to the coverage state.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_CsmVerifyJobFinishedIndication(const Crypto_JobType* job, Crypto_ResultType result)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
    uint8 detErrorId = E_OK;
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    ApplicationType partId = SecOC_GetApplicationID();

    if (SecOC_isInitState(partId) == SECOC_UNINIT)
    {
        detErrorId = SECOC_E_UNINIT;
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        const SecOC_RxPduProcessingType* rxPduProcessingDataPtr;
        SecOC_RxPduRunTimeType*          rxRtPtr = NULL_PTR;
        uint16                           index;

        for (index = SECOC_CONST_0U; index < SECOC_RX_PDU_PRO_NUM; index++)
        {
            rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
            if (rxPduProcessingDataPtr->SecOCRxAuthServiceConfigRef->SecOCJobId == job->jobId)
            {
                if (SECOC_IS_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_WAITING))
                {
                    rxRtPtr = SecOC_RxRtDataPtr(index);
                    if (E_OK == result)
                    {
                        SchM_Enter_SecOC_RxData();
                        SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                        SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);

                        /* @req SWS_SecOC_00239 */
                        /* Verification of the authenticator successfully executed,
                         * the authentication build counter shall be set to 0. */
                        rxRtPtr->rxAuthBuildAttempts = SECOC_CONST_0U;
                        SchM_Exit_SecOC_RxData();
                    }
                    else if (CRYPTO_E_BUSY == result)
                    {
                        /* @req SWS_SecOC_00237 */
                        /* Recoverable error occur */
                        SecOC_RxAuthenticationBuildRetry(index);
                    }
                    else
                    {
                        /* No-recoverable error */
                        /* @req SWS_SecOC_00241 */
                        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                            || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                                && (rxRtPtr->rxBakNumOfMsgToOverride > SECOC_CONST_0U))
                            || (SecOC_IgnoreVerificationResultIsEnabled))
                        {
                            SchM_Enter_SecOC_RxData();
                            SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                            /* Continue to send authentic I-PDU to upper layer */
                            SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                            SchM_Exit_SecOC_RxData();
                        }
                        else if (
                            (E_NOT_OK != result)
                            && (rxRtPtr->rxVerifyAttempts
                                <= SecOC_GetRxPduProcessingDataPtr(index)->SecOCAuthenticationVerifyAttempts))
                        {
                            /* @req SWS_SecOC_00239 */
                            /* Verification of the authenticator successfully executed but the
                             * verification failed. */
                            SecOC_RxVerifyAuthenticInfoRetry(index);
                        }
                        else
                        {
                            rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
                            /* Report verify status */
                            SecOC_RxReportVerifyStatus(index);
#if (SECOC_SECURITY_EVENT_REPORTING_ENABLED == STD_ON)
                            IdsM_SetSecurityEventWithContextData(
                                SECOC_SEV_SECOC_MAC_VERIFICATION_FAILED,
                                (const uint8*)&(SecOC_GetRxPduProcessingPtr(index)->SecOCDataId),
                                2u);
#endif /* SECOC_SECURITY_EVENT_REPORTING_ENABLED  == STD_ON */
                            /* Non-recoverable error occur */
                            SecOC_RxChannelReset(index);
                        }
                    }
                    break;
                }
            }
        }
    }
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (detErrorId != E_OK)
    {
        SECOC_DET_REPORT(SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID, detErrorId);
    }
#else
    (void)detErrorId;
#endif
}
#define SECOC_STOP_SEC_CODE_FAST
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"
/**
 * This function performs the processing of the SecOC module's authentication
 * and verification processing for the Rx path.Perform corresponding processing
 * according to the PDU status.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_MainFunctionRx(void)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
    uint16          index;
    ApplicationType partId = SecOC_GetApplicationID();
    if (SecOC_isInitState(partId) == SECOC_INIT)
    {
        for (index = SECOC_CONST_0U; index < SECOC_RX_PDU_PRO_NUM; index++)
        {
#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
            if (SECOC_RX_NONE == SecOC_RxRtDataPtr(index)->rxPduStatus)
            {
                SecOC_RxLoadQueuedPdu(index);
            }
#endif

            if (SECOC_RX_NONE != SecOC_RxRtDataPtr(index)->rxPduStatus)
            {
                switch (SecOC_RxRtDataPtr(index)->rxPduStatus)
                {
                case SECOC_RX_CALC:
                case SECOC_RX_RETRY:
                    /* @req SWS_SecOC_00079 */
                    SecOC_RxParseSecuredIpdu(index);
                    SecOC_RxVerifyProcee(index);
                    SecOC_RxPassAuthenticPduToUpperLayer(index);
#if (SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U)
                    SecOC_RxTransTpDataToUpperLayer(index);
#endif
                    break;

                default:
                    /* Nothing to do here */
                    break;
                }
            }
        }
    }
#endif /* SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U */
}

/**
 * This function performs the processing of the SecOC module's authentication
 * and verification processing for the Tx path.Perform corresponding processing
 * according to the PDU status.
 */
/* PRQA S 1503, 6070, 6010 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC */
/* PRQA S 1532, 6080, 6050 ++ */ /* VL_QAC_OneFunRef, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
/* PRQA S 6030, 6020 ++ */       /* VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STLIN */
void SecOC_MainFunctionTx(void)
/* PRQA S 6030, 6020 -- */
/* PRQA S 1532, 6080, 6050 -- */
/* PRQA S 1503, 6070, 6010 -- */
{
#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)
    ApplicationType partId = SecOC_GetApplicationID();
    uint16          index;

    if (SecOC_isInitState(partId) == SECOC_INIT)
    {
        /* @req SWS_SecOC_00060,SWS_SecOC_00061 */
        /* Process Tx Pdus */
        for (index = SECOC_CONST_0U; index < SECOC_TX_PDU_PRO_NUM; index++)
        {
            if (SECOC_TX_NONE != SecOC_TxRtDataPtr(index)->txPduStatus)
            {
                switch (SecOC_TxRtDataPtr(index)->txPduStatus)
                {
                case SECOC_TX_CALC:
                case SECOC_TX_RETRY:
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
                    SecOC_TxTpAcquireData(index);
#endif
                    /* @req SWS_SecOC_00060,SWS_SecOC_00065,
                     * SWS_SecOC_00070 */
                    SecOC_TxGenerateAuthenticator(index);
                    /* @req SWS_SecOC_00061,SWS_SecOC_00066,
                     * SWS_SecOC_00071  */
                    SecOC_TxSendSecuredIPDU(index);
                    break;

                default:
                    /* Nothing to do here*/
                    break;
                }
            }
        }
    }
#endif /* SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U */
}
#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"

SECOC_LOCAL ApplicationType SecOC_GetApplicationID(void)
{
    ApplicationType applicationId = SECOC_CONST_0U;
#if (SECOC_MULTIPLE_PARTITION_ENABLED == STD_ON)
    applicationId = GetApplicationID();
#endif
    return applicationId;
}

#if (SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U)

SECOC_LOCAL void SECOC_SET_TX_EVENT(uint16 i, uint16 e)
{
    (SecOC_TxRtDataPtr(i)->SecOC_TxEvent |= (e));
}

SECOC_LOCAL void SECOC_CLR_TX_EVENT(uint16 i, uint16 e)
{
    SecOC_TxRtDataPtr(i)->SecOC_TxEvent &= (uint16)(~(e));
}

SECOC_LOCAL boolean SECOC_IS_TX_EVENT(uint16 i, uint16 e)
{
    return ((SecOC_TxRtDataPtr(i)->SecOC_TxEvent & (e)) != SECOC_CONST_0U) ? TRUE : FALSE;
}

SECOC_LOCAL const SecOC_PbTxPduProcessingType* SecOC_GetTxPduProcessingPtr(uint16 idx)
{
    return (&SecOC_ConfigPtr->SecOC_PbTxPduProcessing[idx]);
}

SECOC_LOCAL const SecOC_TxPduProcessingType* SecOC_GetTxPduProcessingDataPtr(uint16 idx)
{
    return (&SecOC_TxPduProcessingData[idx]);
}

SECOC_LOCAL const SecOC_TxAuthenticPduLayerType* SecOC_GetTxAuthenticPduLayerPtr(uint16 idx)
{
    return (SecOC_GetTxPduProcessingPtr(idx)->SecOCTxAuthenticPduLayer);
}

SECOC_LOCAL ApplicationType SecOC_GetTxChannelApplicationId(uint16 idx)
{
    return (SecOC_GetTxPduProcessingPtr(idx)->ChannelApplicationId);
}

SECOC_LOCAL const SecOC_TxSecuredPduLayerType* SecOC_GetTxSecuredPduLayerPtr(uint16 idx)
{
    return (SecOC_GetTxPduProcessingPtr(idx)->SecOCTxSecuredPduLayer);
}

SECOC_LOCAL const SecOC_TxSecuredPduType* SecOC_GetTxSecuredPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduLayerPtr(idx)->SecOCTxSecuredPdu);
}

SECOC_LOCAL const SecOC_TxSecuredPduCollectionType* SecOC_GetTxSecuredPduCollectionPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduLayerPtr(idx)->SecOCTxSecuredPduCollection);
}

SECOC_LOCAL const SecOC_TxAuthenticPduType* SecOC_GetTxAuthenticPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCTxAuthenticPdu);
}

SECOC_LOCAL const SecOC_TxCryptographicPduType* SecOC_GetTxCryptographicPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCTxCryptographicPdu);
}

SECOC_LOCAL const SecOC_UseMessageLinkType* SecOC_GetTxUseMessageLinkPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCUseMessageLink);
}

SECOC_LOCAL uint16 SecOC_FindProcessTxPduIndex(PduIdType TxPduId, uint16 startIdx, uint16 endIdx)
{
    const SecOC_TxSecuredPduType*       txSecuredPduPtr;
    const SecOC_TxAuthenticPduType*     txAuthenticPduPtr;
    const SecOC_TxCryptographicPduType* txCryptographicPduPtr;
    uint16                              index;
    boolean                             txPduTag = FALSE;
    SchM_Enter_SecOC_TxData();
    for (index = startIdx; index < endIdx; index++)
    {
        txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
        if (NULL_PTR != txSecuredPduPtr)
        {
            if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == TxPduId)
            {
                txPduTag = TRUE;
            }
        }
        else
        {
            txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
            if (txAuthenticPduPtr->SecOCTxAuthenticPduId == TxPduId)
            {
                SECOC_SET_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED);
                txPduTag = TRUE;
            }

            txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
            if (txCryptographicPduPtr->SecOCTxCryptographicPduId == TxPduId)
            {
                SECOC_SET_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED);
                txPduTag = TRUE;
            }
        }
        if (txPduTag)
        {
            break;
        }
    }
    SchM_Exit_SecOC_TxData();
    return index;
}

/* PRQA S 5017, 6010, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxGenerateAuthenticator(uint16 index)
/* PRQA S 5017, 6010, 6070  -- */
{
    const SecOC_TxPduProcessingType* txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
    const SecOC_CsmJobType*          jobPtr                 = txPduProcessingDataPtr->SecOCTxAuthServiceConfigRef;
    SecOC_TxPduRunTimeType*          txRtPtr                = SecOC_TxRtDataPtr(index);
    uint32                           dataLength;
    Std_ReturnType                   ret;
    boolean                          retryFlag                = FALSE;
    boolean                          failFlag                 = FALSE;
    boolean                          getAutherenticatorBuffer = FALSE;
    ApplicationType                  aplId                    = SecOC_GetApplicationID();
    uint8*                           SecOC_DataToAuthenticator;
    getAutherenticatorBuffer = SecOC_GetTxDataToAuthenticator(aplId, &SecOC_DataToAuthenticator);
    if ((getAutherenticatorBuffer) && (SECOC_IS_TX_EVENT(index, SECOC_TX_REQUEST))
        && (SECOC_IS_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED)))
    {
        SECOC_CLR_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY);
        dataLength              = SecOC_TxCalcDataToAuthenticatorLength(index);
        ret                     = SecOC_TxConstructDataToAuthenticator(index);
        txRtPtr->txResultLength = SECOC_MAX_AUTHENTICATOR_LEN;
        if (E_OK == ret)
        {
            /* @req SWS_SecOC_00035 */
            switch (jobPtr->SecOCCsmPrimitives)
            {
#if (SECOC_CSMMAC_ENABLED == STD_ON)
            case SECOC_CSMMAC:
                ret = Csm_MacGenerate(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[SECOC_CONST_0U],
                    dataLength,
                    &txRtPtr->txResultBuffer[SECOC_CONST_0U],
                    &txRtPtr->txResultLength);
                break;
#endif /* SECOC_CSMMAC_ENABLED == STD_ON */

#if (SECOC_CSMSIGNATRUE_ENABLED == STD_ON)
            case SECOC_CSMSIGNATURE:
                ret = Csm_SignatureGenerate(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[SECOC_CONST_0U],
                    dataLength,
                    &txRtPtr->txResultBuffer[SECOC_CONST_0U],
                    &txRtPtr->txResultLength);
                break;
#endif /* SECOC_CSMSIGNATRUE_ENABLED == STD_ON */

            default:
                ret = E_NOT_OK;
                break;
            }

            if (E_OK == ret)
            {
                SchM_Enter_SecOC_TxData();

                if (SECOC_CRYPTO_PROCESSING_SYNC == jobPtr->SecOCCryptoProcessingType)
                {
                    SECOC_CLR_TX_EVENT(index, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
                    SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
                }
                else
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_WAITING);
                }
                SchM_Exit_SecOC_TxData();
            }
            else if (CRYPTO_E_BUSY == ret)
            {
                /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
                retryFlag = TRUE;
            }
            else
            {
                /* Non-recoverable error */
                failFlag = TRUE;
            }
        }
        else if (E_BUSY == ret)
        {
            /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
            retryFlag = TRUE;
        }
        else
        {
            /* Non-recoverable error */
            failFlag = TRUE;
        }

        if (retryFlag)
        {
            /* Initiate retry process */
            SecOC_TxGenerateAuthenticatorRetry(index);
        }

        if (failFlag)
        {
            /* @req SWS_SecOC_00229 */
            SecOC_TxBuildAuthenticatorFailHandle(index);
        }
    }
}

SECOC_LOCAL uint32 SecOC_TxCalcDataToAuthenticatorLength(uint16 index)
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr    = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxPduRunTimeType*      txRtPtr               = SecOC_TxRtDataPtr(index);
    const SecOC_TxPduSecuredAreaType*  SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
    uint32                             dataLength            = SECOC_DATAID_LEN;

    if (NULL_PTR != SecOCTxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + SecOCTxPduSecuredArea->SecOCSecuredTxPduLength;
    }
    else
    {
        SchM_Enter_SecOC_TxData();
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + txRtPtr->txPduLength;
        SchM_Exit_SecOC_TxData();
    }

    dataLength += SECOC_BIT_TO_BYTE((uint16)txPduProcessingPtr->SecOCFreshnessValueLength);

    return dataLength;
}
/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL Std_ReturnType SecOC_TxConstructDataToAuthenticator(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr     = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxPduProcessingType*   txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
    SecOC_TxPduRunTimeType*            txRtPtr                = SecOC_TxRtDataPtr(index);
    const uint8*                       authenticPduPtr;
    const SecOC_TxPduSecuredAreaType*  SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
    SecOC_FreshnessArrayType           truncatedFreshnessValue;
    uint32                             securedIPDULength;
    uint32                             securedIPDUOffset;
    uint32                             freshnessValueLen;
    uint32                             freshnessValueTruncLen;
    uint16                             dataId;
    Std_ReturnType                     ret;
    ApplicationType                    aplId = SecOC_GetApplicationID();
    uint8*                             SecOC_DataToAuthenticator;
    (void)SecOC_GetTxDataToAuthenticator(aplId, &SecOC_DataToAuthenticator);
    SchM_Enter_SecOC_Authentication();

    (void)IStdLib_MemSet(SecOC_DataToAuthenticator, 0u, SECOC_DATA_TO_AUTHENTICATOR_LENGTH);

    /* @req SWS_SecOC_00034 */
    /*
     * DataToAuthenticator = Data Identifier | secured part of the Authentic
     * I-PDU | Complete Freshness Value
     */

    /* Data Identifie */
    dataId = txPduProcessingPtr->SecOCDataId;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)

    SecOC_EndianSwap((uint8*)&dataId, sizeof(dataId));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
    (void)IStdLib_MemCpy(&SecOC_DataToAuthenticator[SECOC_CONST_0U], (uint8*)&dataId, SECOC_DATAID_LEN);

    /* Secured part of the Authentic */
    authenticPduPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;

    if (NULL_PTR != SecOCTxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = SecOCTxPduSecuredArea->SecOCSecuredTxPduLength;
        securedIPDUOffset = SecOCTxPduSecuredArea->SecOCSecuredTxPduOffset;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = txRtPtr->txPduLength;
        securedIPDUOffset = SECOC_CONST_0U;
    }

    (void)IStdLib_MemCpy(
        &SecOC_DataToAuthenticator[SECOC_DATAID_LEN],
        &authenticPduPtr[securedIPDUOffset],
        securedIPDULength);

    /* Complete Freshness Value */
    freshnessValueLen      = txPduProcessingPtr->SecOCFreshnessValueLength;
    freshnessValueTruncLen = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
    SchM_Exit_SecOC_Authentication();
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
    if (txPduProcessingDataPtr->SecOCProvideTxTruncatedFreshnessValue)
    {
        /* @req SWS_SecOC_00221,SWS_SecOC_00230 */
        ret = SecOC_GetTxFreshnessTruncData(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen,
            truncatedFreshnessValue,
            &freshnessValueTruncLen);
    }
    else
    {
        /* @req SWS_SecOC_00222,SWS_SecOC_00231 */
        ret = SecOC_GetTxFreshness(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen);
    }
#else
#if (STD_ON == SECOC_PROVIDE_TX_TRUNCATED_FRESHNESS_VALUE_ENABLED)
    if (txPduProcessingDataPtr->SecOCProvideTxTruncatedFreshnessValue)
    {
        /* @req SWS_SecOC_00223 */
        ret = Rte_Call_FreshnessManagement_GetTxFreshnessTruncData(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen,
            truncatedFreshnessValue,
            &freshnessValueTruncLen);
    }
    else
#endif /* STD_ON == SECOC_PROVIDE_TX_TRUNCATED_FRESHNESS_VALUE_ENABLED */
    {
#if (STD_ON == SECOC_PROVIDE_TX_TRUNCATED_FRESHNESS_VALUE_DISABLED)
        /* @req SWS_SecOC_00224 */
        ret = Rte_Call_FreshnessManagement_GetTxFreshness(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen);
#endif /* STD_ON == SECOC_PROVIDE_TX_TRUNCATED_FRESHNESS_VALUE_DISABLED */
    }
#endif
    SchM_Enter_SecOC_Authentication();
    if (E_OK == ret)
    {
        freshnessValueLen = SECOC_BIT_TO_BYTE((uint16)txPduProcessingPtr->SecOCFreshnessValueLength);
        (void)IStdLib_MemCpy(
            &SecOC_DataToAuthenticator[SECOC_DATAID_LEN + securedIPDULength],
            txRtPtr->txFreshnessValue,
            freshnessValueLen);
    }
    SchM_Exit_SecOC_Authentication();

    return ret;
}

SECOC_LOCAL void SecOC_TxGenerateAuthenticatorRetry(uint16 index)
{
    const SecOC_TxPduProcessingType* txPduProcessingPtr = SecOC_GetTxPduProcessingDataPtr(index);
    SecOC_TxPduRunTimeType*          txRtPtr            = SecOC_TxRtDataPtr(index);

    if (txRtPtr->txAuthBuildCnt < txPduProcessingPtr->SecOCAuthenticationBuildAttempts)
    {
        SchM_Enter_SecOC_TxData();
        /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
        txRtPtr->txAuthBuildCnt++;
        txRtPtr->txPduStatus = SECOC_TX_RETRY;
        SchM_Exit_SecOC_TxData();
    }
    else
    {
        /* @req SWS_SecOC_00229 */
        SecOC_TxBuildAuthenticatorFailHandle(index);

/* @req SWS_SecOC_00155 */
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
        SECOC_DET_REPORT(SECOC_MAINFUNCTIONTX_ID, SECOC_E_CRYPTO_FAILURE);
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    }
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxBuildAuthenticatorFailHandle(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_SameBufferPduCollectionType* sameBufferPtr =
        SecOC_GetTxPduProcessingDataPtr(index)->SecOCSameBufferPduRef;
    SecOC_TxPduRunTimeType*              txRtPtr                = SecOC_TxRtDataPtr(index);
    boolean                              cancelFlag             = FALSE;
    const SecOC_TxAuthenticPduLayerType* txAuthenticPduLayerPtr = SecOC_GetTxAuthenticPduLayerPtr(index);
    PduIdType                            upLayerPduId           = txAuthenticPduLayerPtr->SecOCTxPduRAsUpLayerId;
/* @req SWS_SecOC_00229 */
#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
    SchM_Enter_SecOC_TxData();
    if (txRtPtr->txDefaultAuthInfoEnable)
    {
        /* Use SecOCDefaultAuthenticationInformationPattern as Freshness Value
         * and Authenticator. */
        for (uint8 i = 0u; i < sizeof(txRtPtr->txFreshnessValue); i++)
        {
            txRtPtr->txFreshnessValue[i] = SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN;
        }

        for (uint8 i = 0u; i < SECOC_MAX_AUTHENTICATOR_LEN; i++)
        {
            txRtPtr->txResultBuffer[i] = SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN;
        }
        SECOC_CLR_TX_EVENT(index, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
        SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
    }
    else
    {
        /* Cancel transmission */
        cancelFlag = TRUE;
    }
    SchM_Exit_SecOC_TxData();
#else
    /*  Cancel transmission */
    cancelFlag = TRUE;
#endif

    if (cancelFlag)
    {
        SchM_Enter_SecOC_TxData();
        /*  Cancel transmission */
        SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
        txRtPtr->txPduStatus    = SECOC_TX_NONE;
        txRtPtr->txAuthBuildCnt = SECOC_CONST_0U;
        SchM_Exit_SecOC_TxData();

        if (txAuthenticPduLayerPtr->SecOCPduType == SECOC_TPPDU)
        {
            PduR_SecOCTpTxConfirmation(upLayerPduId, E_NOT_OK);
        }
        SchM_Enter_SecOC_TxData();
        /* Free the buffer that contains the Secured I-PDU */
        txRtPtr->txPduReady = FALSE;
        SchM_Exit_SecOC_TxData();
        if (NULL_PTR != sameBufferPtr)
        {
            *(sameBufferPtr->SecOCSameBufferInUse) = FALSE;
        }
    }
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxSendSecuredIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_TxSecuredPduType* txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
    uint16                        freshnessValueId;

    if ((SECOC_IS_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED))
        || (SECOC_IS_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY)))
    {
        SECOC_CLR_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
        if (NULL_PTR != txSecuredPduPtr)
        {
            /* SecOC transmit the Secured I-PDU in one message */
            if (!SECOC_IS_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY))
            {
                SecOC_TxConstructSecuredIPDU(index);
            }

            SecOC_TxBroadcastSecuredIPDU(index);
        }
        else
        {
            /* @req SWS_SecOC_00201,SWS_SecOC_00202 */
            /*
             * Use Secured Pdu Collection: SecOC shall transmit the Secured I-PDU as
             * two messages: The original Authentic I-PDU and a separate
             * Cryptographic I-PDU.
             */

            /* Send original Authentic I-PDU */
            if (!SECOC_IS_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY))
            {
                SecOC_TxConstructAuthenticIPDU(index);
            }
            SecOC_TxBroadcastAuthenticIPDU(index);

            /* Send Cryptographic I-PDU */
            if (!SECOC_IS_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY))
            {
                SecOC_TxConstructCryptographicIPDU(index);
            }
            SecOC_TxBroadcastCryptographicIPDU(index);
        }
        SecOC_TxPduRunTimeType* txRtPtr = SecOC_TxRtDataPtr(index);
        if (SECOC_MAX_TRANSMITRETRIES_NUM > txRtPtr->retryTransmitNum)
        {
            SECOC_SET_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY);
        }
        else
        {
            txRtPtr->txPduStatus = SECOC_TX_NONE;
            SECOC_CLR_TX_EVENT(index, SECOC_TX_TRANSMIT_RETRY);
        }

        /* @req SWS_SecOC_00232,SWS_SecOC_00233 */
        /* Indicate the Secured I-PDU has been initiated for transmission.*/
        if (SecOC_GetTxPduProcessingDataPtr(index)->SecOCUseTxConfirmation)
        {
            freshnessValueId = SecOC_GetTxPduProcessingPtr(index)->SecOCFreshnessValueId;
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
            (void)SecOC_SPduTxConfirmation(freshnessValueId);
#else
            (void)Rte_Call_FreshnessManagement_SPduTxConfirmation(freshnessValueId);
#endif
        }
    }
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxConstructSecuredIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxSecuredPduType*      txSecuredPduPtr    = SecOC_GetTxSecuredPduPtr(index);
    SecOC_TxPduRunTimeType*            txRtPtr            = SecOC_TxRtDataPtr(index);
    uint8*                             destPtr;
    const uint8*                       srcPtr;
    uint32                             offset = SECOC_CONST_0U;
    PduLengthType                      pduLength;
    uint16                             authInfoTruncLenInBytes;
    uint8                              fvLenInBytes;
    uint8                              fvLenInBits;
    uint8                              fvTruncLenInBytes;
    uint8                              fvTruncLenInBits = SECOC_CONST_0U;
    uint8                              authInfoShiftBits;
    uint16                             fvOffsetBits = SECOC_CONST_0U;

    SchM_Enter_SecOC_TxData();

    destPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
    srcPtr  = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
    (void)IStdLib_MemSet(destPtr, 0u, txSecuredPduPtr->SecOCTxSecuredPduBuffLength);

    /* Construct Secured I-PDU */
    /* @req SWS_SecOC_00037 */
    /* SecuredPDU = SecuredIPDUHeader(optional) | AuthenticIPDU | FreshnessValue
     * [SecOCFreshnessValueTruncLength] (optional) | Authenticator
     * [SecOCAuthInfoTruncLength] */

    /* @req SWS_SecOC_00262 */
    /* Header */
    if (txSecuredPduPtr->SecOCAuthPduHeaderLength > SECOC_CONST_0U)
    {
        pduLength = txRtPtr->txPduLength;

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        SecOC_EndianSwap((uint8*)&pduLength, sizeof(pduLength));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
        /* Takes the effective low byte of the value */
        uint8 pduHeaderOffset = (uint8)sizeof(pduLength) - txSecuredPduPtr->SecOCAuthPduHeaderLength;
        (void)IStdLib_MemCpy(
            &destPtr[offset],
            /* PRQA S 0488 ++ */ /* VL_SecOC_0488 */
                ((uint8*)&pduLength) + pduHeaderOffset,
            /* PRQA S 0488 -- */
            txSecuredPduPtr->SecOCAuthPduHeaderLength);
        /* PRQA S 2986 ++ */ /* VL_SecOC_2986 */
        offset += (uint32)txSecuredPduPtr->SecOCAuthPduHeaderLength;
        /* PRQA S 2986 -- */
    }

    /* Authentic I-PDU */
    (void)IStdLib_MemCpy(&destPtr[offset], srcPtr, txRtPtr->txPduLength);
    offset += txRtPtr->txPduLength;

    /* Freshness value */
    /* @req SWS_SecOC_00094 */
    if (txPduProcessingPtr->SecOCFreshnessValueTruncLength > SECOC_CONST_0U)
    {
        fvLenInBits       = txPduProcessingPtr->SecOCFreshnessValueLength;
        fvTruncLenInBits  = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
        fvLenInBytes      = (uint8)SECOC_BIT_TO_BYTE(fvLenInBits);
        fvTruncLenInBytes = (uint8)SECOC_BIT_TO_BYTE(fvTruncLenInBits);
        if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((fvLenInBits))))
        {
            fvOffsetBits = SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT(fvLenInBits);
        }

        fvOffsetBits = ((uint16)fvLenInBits - (uint16)fvTruncLenInBits) + fvOffsetBits;

        SecOC_ArrayShiftLeft(txRtPtr->txFreshnessValue, fvLenInBytes, fvOffsetBits);
        (void)IStdLib_MemCpy(&destPtr[offset], txRtPtr->txFreshnessValue, fvTruncLenInBytes);
        if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits))))
        {
            offset += ((uint32)fvTruncLenInBytes - 1uL);
            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
            destPtr[offset] &= (SECOC_CONST_FFU << (SECOC_CONST_8U - (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits)))));
            /* PRQA S 4397 -- */
        }
        else
        {
            offset += fvTruncLenInBytes;
        }
    }

    /* Authenticator */
    /* @req SWS_SecOC_00036 */
    /* Truncate the resulting Authenticator down to the number of bits specified
     * by SecOCAuthInfoTruncLength. */
    authInfoShiftBits = SECOC_MOD_FROM_EIGHT((fvTruncLenInBits));
    authInfoTruncLenInBytes =
        SECOC_BIT_TO_BYTE(txPduProcessingPtr->SecOCAuthInfoTruncLength + (uint16)authInfoShiftBits);
    SecOC_ArrayShiftRight(&txRtPtr->txResultBuffer[SECOC_CONST_0U], authInfoTruncLenInBytes, authInfoShiftBits);
    if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((txPduProcessingPtr->SecOCAuthInfoTruncLength + fvTruncLenInBits))))
    {
        SecOC_ClearLowBit(
            &txRtPtr->txResultBuffer[SECOC_CONST_0U],
            (uint8)authInfoTruncLenInBytes,
            (uint16)(SECOC_CONST_8U
                     - (uint16)SECOC_MOD_FROM_EIGHT(
                         (txPduProcessingPtr->SecOCAuthInfoTruncLength + (uint16)fvTruncLenInBits))));
    }

    SecOC_ArrayOrOp(&destPtr[offset], &txRtPtr->txResultBuffer[SECOC_CONST_0U], authInfoTruncLenInBytes);
    SchM_Exit_SecOC_TxData();
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxConstructAuthenticIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxAuthenticPduType*    txAuthenticPduPtr  = SecOC_GetTxAuthenticPduPtr(index);
    const SecOC_TxPduRunTimeType*      txRtPtr            = SecOC_TxRtDataPtr(index);
    uint8*                             destPtr;
    const uint8*                       srcPtr;
    uint32                             offset = SECOC_CONST_0U;
    PduLengthType                      pduLength;

    SchM_Enter_SecOC_TxData();

    destPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
    srcPtr  = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
    (void)IStdLib_MemSet(destPtr, 0u, txAuthenticPduPtr->SecOCTxColAuthenticPduBuffLength);

    /* Construct Orignal Authentic I-PDU */
    /* @req SWS_SecOC_00262 */
    /* Header */
    if (txAuthenticPduPtr->SecOCAuthPduHeaderLength > SECOC_CONST_0U)
    {
        pduLength = txRtPtr->txPduLength;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        SecOC_EndianSwap((uint8*)&pduLength, sizeof(pduLength));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

        uint8 pduHeaderOffset = (uint8)sizeof(pduLength) - (uint8)txAuthenticPduPtr->SecOCAuthPduHeaderLength;
        (void)IStdLib_MemCpy(
            &destPtr[offset],
            /* PRQA S 0488 ++ */ /* VL_SecOC_0488 */
                ((uint8*)&pduLength) + pduHeaderOffset,
            /* PRQA S 0488 -- */
            txAuthenticPduPtr->SecOCAuthPduHeaderLength);
        /* PRQA S 2986 ++ */ /* VL_SecOC_2986 */
        offset += (uint32)txAuthenticPduPtr->SecOCAuthPduHeaderLength;
        /* PRQA S 2986 -- */
    }

    /* Authentic I-PDU */
    (void)IStdLib_MemCpy(&destPtr[offset], srcPtr, txRtPtr->txPduLength);
    SchM_Exit_SecOC_TxData();
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxConstructCryptographicIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType*  txPduProcessingPtr    = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxCryptographicPduType* txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
#if (SECOC_MAX_MSG_LINK_LEN > 0)
    const SecOC_UseMessageLinkType* messageLinkPtr = SecOC_GetTxUseMessageLinkPtr(index);
#endif
    SecOC_TxPduRunTimeType* txRtPtr = SecOC_TxRtDataPtr(index);
    uint8*                  destPtr;
    uint16                  offset = SECOC_CONST_0U;
    uint16                  authInfoTruncLenInBytes;
    uint16                  authInfoTruncLenInBits;
    uint8                   fvLenInBytes;
    uint8                   fvLenInBits;
    uint8                   fvTruncLenInBytes;
    uint8                   fvTruncLenInBits = SECOC_CONST_0U;
    uint8                   authInfoShiftBits;

    destPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
    (void)IStdLib_MemSet(destPtr, 0u, txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength);

    /* @req SWS_SecOC_00209 */
    /* Cryptographic I-PDU = Authentication Data | Message Linker */
    /* Freshness value */
    uint16 fvOffsetBits = SECOC_CONST_0U;
    if (txPduProcessingPtr->SecOCFreshnessValueTruncLength > SECOC_CONST_0U)
    {
        fvLenInBits       = txPduProcessingPtr->SecOCFreshnessValueLength;
        fvTruncLenInBits  = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
        fvLenInBytes      = (uint8)SECOC_BIT_TO_BYTE((uint16)fvLenInBits);
        fvTruncLenInBytes = (uint8)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits);

        if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((fvLenInBits))))
        {
            fvOffsetBits = SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT(fvLenInBits);
        }

        fvOffsetBits = ((uint16)fvLenInBits - (uint16)fvTruncLenInBits) + fvOffsetBits;

        SchM_Enter_SecOC_TxData();
        SecOC_ArrayShiftLeft(txRtPtr->txFreshnessValue, fvLenInBytes, fvOffsetBits);
        (void)IStdLib_MemCpy(&destPtr[offset], txRtPtr->txFreshnessValue, fvTruncLenInBytes);
        SchM_Exit_SecOC_TxData();
        if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits))))
        {
            offset = ((uint16)fvTruncLenInBytes - 1u);
            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
            destPtr[offset] &= (SECOC_CONST_FFU << (SECOC_CONST_8U - (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits)))));
            /* PRQA S 4397 -- */
        }
        else
        {
            offset = fvTruncLenInBytes;
        }
    }
    SchM_Enter_SecOC_TxData();
    /* Authenticator */
    authInfoShiftBits       = SECOC_MOD_FROM_EIGHT((fvTruncLenInBits));
    authInfoTruncLenInBits  = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    authInfoTruncLenInBytes = SECOC_BIT_TO_BYTE(authInfoTruncLenInBits + (uint16)authInfoShiftBits);
    SecOC_ArrayShiftRight(&txRtPtr->txResultBuffer[SECOC_CONST_0U], authInfoTruncLenInBytes, authInfoShiftBits);
    if (SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + fvTruncLenInBits)) > SECOC_CONST_0U)
    {
        SecOC_ClearLowBit(
            &txRtPtr->txResultBuffer[SECOC_CONST_0U],
            authInfoTruncLenInBytes,
            (uint16)(SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + fvTruncLenInBits))));
    }
    SecOC_ArrayOrOp(&destPtr[offset], &txRtPtr->txResultBuffer[SECOC_CONST_0U], authInfoTruncLenInBytes);
    if (SECOC_CONST_0U != (SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + authInfoShiftBits))))
    {
        offset += (authInfoTruncLenInBytes - 1u);
        /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
        destPtr[offset] &=
            (SECOC_CONST_FFU
             << (SECOC_CONST_8U - (SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + authInfoShiftBits)))));
        /* PRQA S 4397 -- */
    }
#if (SECOC_MAX_MSG_LINK_LEN > 0)
    else
    {
        offset += authInfoTruncLenInBytes;
    }

    /* @req SWS_SecOC_00210 */
    /* Message Linker */
    const uint8* srcPtr;
    srcPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
    if (NULL_PTR != messageLinkPtr)
    {
        uint16 msgLinkShiftBits;
        uint16 msgLinkLenInBytes;
        uint16 msgLinkLenInBits;
        uint16 msgLinkPos;
        uint8  lMsgLinkBuffer[SECOC_MAX_MSG_LINK_LEN] = {0};
        uint16 lMsgMod;
        msgLinkLenInBits  = messageLinkPtr->SecOCMessageLinkLen;
        msgLinkPos        = messageLinkPtr->SecOCMessageLinkPos;
        msgLinkShiftBits  = (uint8)(SECOC_MOD_FROM_EIGHT((fvTruncLenInBits + authInfoTruncLenInBits)));
        msgLinkLenInBytes = SECOC_BIT_TO_BYTE(msgLinkLenInBits + (uint16)msgLinkShiftBits);
        lMsgMod           = SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT(msgLinkLenInBits);
        SecOC_CopyBitsFromSourceToDestination(srcPtr, lMsgLinkBuffer, msgLinkPos, msgLinkLenInBits);
        if (SECOC_CONST_8U != lMsgMod)
        {
            if (msgLinkShiftBits > lMsgMod)
            {
                SecOC_ArrayShiftRight(lMsgLinkBuffer, msgLinkLenInBytes, (uint16)(msgLinkShiftBits - lMsgMod));
            }
            else
            {
                SecOC_ArrayShiftLeft(lMsgLinkBuffer, msgLinkLenInBytes, (uint16)(lMsgMod - msgLinkShiftBits));
            }
        }
        SecOC_ArrayOrOp(&destPtr[offset], lMsgLinkBuffer, msgLinkLenInBytes);
    }
#endif /* SECOC_MAX_MSG_LINK_LEN > 0 */
    SchM_Exit_SecOC_TxData();
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxBroadcastSecuredIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxSecuredPduType*      txSecuredPduPtr    = SecOC_GetTxSecuredPduPtr(index);
    SecOC_TxPduRunTimeType*            txRtPtr            = SecOC_TxRtDataPtr(index);
    PduIdType                          pduId              = txSecuredPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType                        pduInfo;
    uint16                             authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    uint8                              fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
    uint8                              headerLenInBytes = txSecuredPduPtr->SecOCAuthPduHeaderLength;

    SchM_Enter_SecOC_TxData();
    uint16 SecuredIpduLength = (uint16)headerLenInBytes + (uint16)txRtPtr->txPduLength
                               + SECOC_BIT_TO_BYTE(authInfoTxLength + (uint16)fvTruncLenInBits);
    if (SecuredIpduLength < txSecuredPduPtr->SecOCTxSecuredPduBuffLength)
    {
        pduInfo.SduLength = txSecuredPduPtr->SecOCTxSecuredPduBuffLength;
        /* Fill unused byte */
        for (uint16 i = SecuredIpduLength; i < pduInfo.SduLength; i++)
        {
            txSecuredPduPtr->SecOCTxSecuredPduBufferRef[i] =
                SecOC_GetTxPduProcessingDataPtr(index)->SecOCTxPduUnusedAreasDefault;
        }
    }
    else
    {
        pduInfo.SduLength = SecuredIpduLength;
    }
    SchM_Exit_SecOC_TxData();

    pduInfo.SduDataPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;

    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    if (E_OK != PduR_SecOCTransmit(pduId, &pduInfo))
    {
        txRtPtr->retryTransmitNum++;
    }
    else
    {
        txRtPtr->txPduStatus = SECOC_TX_TRANS;
    }

    SchM_Enter_SecOC_TxData();
    txRtPtr->txSecuredPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady      = TRUE;
/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxPduOffset = SECOC_CONST_0U;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

SECOC_LOCAL void SecOC_TxBroadcastAuthenticIPDU(uint16 index)
{
    const SecOC_TxAuthenticPduType* txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
    SecOC_TxPduRunTimeType*         txRtPtr           = SecOC_TxRtDataPtr(index);
    PduIdType                       pduId             = txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType                     pduInfo;
    uint8                           headerLenInBytes = txAuthenticPduPtr->SecOCAuthPduHeaderLength;
    SchM_Enter_SecOC_TxData();
    pduInfo.SduLength = headerLenInBytes + txRtPtr->txPduLength;
    SchM_Exit_SecOC_TxData();
    pduInfo.SduDataPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;

    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    (void)PduR_SecOCTransmit(pduId, &pduInfo);

    SchM_Enter_SecOC_TxData();
    txRtPtr->txAuthenticPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady        = TRUE;
/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxPduOffset = SECOC_CONST_0U;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxBroadcastCryptographicIPDU(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType*  txPduProcessingPtr    = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxCryptographicPduType* txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
    const SecOC_UseMessageLinkType*     messageLinkPtr        = SecOC_GetTxUseMessageLinkPtr(index);
    SecOC_TxPduRunTimeType*             txRtPtr               = SecOC_TxRtDataPtr(index);
    PduIdType                           pduId                 = txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType                         pduInfo;
    uint16                              msgLinkLen;
    uint16                              authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    uint8                               fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;

    if (NULL_PTR != messageLinkPtr)
    {
        msgLinkLen = messageLinkPtr->SecOCMessageLinkLen;
    }
    else
    {
        msgLinkLen = SECOC_CONST_0U;
    }
    pduInfo.SduLength = (uint16)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits + authInfoTxLength + msgLinkLen);
    if (txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength > pduInfo.SduLength)
    {
        /* Fill unused byte */
        for (PduLengthType i = pduInfo.SduLength; i < txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength; i++)
        {
            txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef[i] =
                SecOC_GetTxPduProcessingDataPtr(index)->SecOCTxPduUnusedAreasDefault;
        }
        pduInfo.SduLength = txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength;
    }

    pduInfo.SduDataPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;

    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    if (E_OK != PduR_SecOCTransmit(pduId, &pduInfo))
    {
        txRtPtr->retryTransmitNum++;
    }
    else
    {
        txRtPtr->txPduStatus = SECOC_TX_TRANS;
    }

    SchM_Enter_SecOC_TxData();
    txRtPtr->txCryptographicPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady            = TRUE;

/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxCryptoPduOffset = SECOC_CONST_0U;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL  */
SECOC_LOCAL void SecOC_TxTpAcquireData(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbTxPduProcessingType* txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    const SecOC_TxPduRunTimeType*      txRtPtr            = SecOC_TxRtDataPtr(index);
    BufReq_ReturnType                  bufReqRet;
    PduLengthType                      availableDataLen;
    PduInfoType                        pduInfo;
    PduIdType                          pduId;
    uint16                             offset = SECOC_CONST_0U;

    if ((SECOC_IS_TX_EVENT(index, SECOC_TX_REQUEST)) && (!SECOC_IS_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED)))
    {
        SchM_Enter_SecOC_TxData();
        pduInfo.SduDataPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
        pduInfo.SduLength  = txRtPtr->txPduLength;
        SchM_Exit_SecOC_TxData();

        /* @req SWS_SecOC_00254 */
        /* Copy data from upper layer */
        pduId     = txPduProcessingPtr->SecOCTxAuthenticPduLayer->SecOCTxPduRAsUpLayerId;
        bufReqRet = PduR_SecOCTpCopyTxData(pduId, &pduInfo, NULL_PTR, &availableDataLen);
        if (BUFREQ_E_BUSY == bufReqRet)
        {
            /* @req SWS_SecOC_00260 */
            /* Retry to acquire data in next main function */
        }
        else if (BUFREQ_E_NOT_OK == bufReqRet)
        {
            /* @req SWS_SecOC_00266 */
            /* Abort transmission*/
            PduR_SecOCTpTxConfirmation(pduId, E_NOT_OK);
            /* Channel Reset */
            SecOC_TxChannelReset(index);
        }
        else
        {
            /* BUFREQ_OK */
            SECOC_SET_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED);
        }
    }
}
#endif

SECOC_LOCAL void SecOC_TxChannelReset(uint16 index)
{
    const SecOC_TxPduProcessingType* txPduProcessingDataPtr = SecOC_GetTxPduProcessingDataPtr(index);
    SecOC_TxPduRunTimeType*          txRtPtr                = SecOC_TxRtDataPtr(index);

    SchM_Enter_SecOC_TxData();
    txRtPtr->txPduStatus           = SECOC_TX_NONE;
    txRtPtr->txPduLength           = SECOC_CONST_0U;
    txRtPtr->txSecuredPduLen       = SECOC_CONST_0U;
    txRtPtr->txAuthenticPduLen     = SECOC_CONST_0U;
    txRtPtr->txCryptographicPduLen = SECOC_CONST_0U;
    txRtPtr->retryTransmitNum      = SECOC_CONST_0U;
    (void)IStdLib_MemSet(txRtPtr->txFreshnessValue, 0u, SECOC_MAX_FRESHNESS_SIZE);
    txRtPtr->txPduReady     = FALSE;
    txRtPtr->txAuthBuildCnt = SECOC_CONST_0U;
    (void)IStdLib_MemSet(txRtPtr->txResultBuffer, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
    txRtPtr->txResultLength          = SECOC_CONST_0U;
    txRtPtr->txDefaultAuthInfoEnable = FALSE;
#if (SECOC_TX_TP_PDU_NUM > SECOC_CONST_0U)
    txRtPtr->tpTxPduOffset       = SECOC_CONST_0U;
    txRtPtr->tpTxCryptoPduOffset = SECOC_CONST_0U;
#endif

    /* Unlock Samebuffer */
    if (NULL_PTR != txPduProcessingDataPtr->SecOCSameBufferPduRef)
    {
        txPduProcessingDataPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse[0] = FALSE;
    }

    /* Clear tx event */
    SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
    SchM_Exit_SecOC_TxData();
}
#endif /* SECOC_TX_PDU_PRO_NUM > SECOC_CONST_0U */

#if (SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U)
SECOC_LOCAL void SECOC_SET_RX_EVENT(uint16 i, uint16 e)
{
    (SecOC_RxRtDataPtr(i)->SecOC_RxEvent |= (e));
}

SECOC_LOCAL void SECOC_CLR_RX_EVENT(uint16 i, uint16 e)
{
    /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
    (SecOC_RxRtDataPtr(i)->SecOC_RxEvent &= ~(e));
    /* PRQA S 4397 -- */
}

SECOC_LOCAL boolean SECOC_IS_RX_EVENT(uint16 i, uint16 e)
{
    return ((SecOC_RxRtDataPtr(i)->SecOC_RxEvent & (e)) != SECOC_CONST_0U) ? TRUE : FALSE;
}

SECOC_LOCAL const SecOC_PbRxPduProcessingType* SecOC_GetRxPduProcessingPtr(uint16 idx)
{
    return &SecOC_ConfigPtr->SecOC_PbRxPduProcessing[idx];
}

SECOC_LOCAL ApplicationType SecOC_GetRxChannelApplicationId(uint16 idx)
{
    return (SecOC_GetRxPduProcessingPtr(idx)->ChannelApplicationId);
}

SECOC_LOCAL const SecOC_RxPduProcessingType* SecOC_GetRxPduProcessingDataPtr(uint16 idx)
{
    return (const SecOC_RxPduProcessingType*)&SecOC_RxPduProcessingData[idx];
}

SECOC_LOCAL const SecOC_RxAuthenticPduLayerType* SecOC_GetRxAuthenticPduLayerPtr(uint16 idx)
{
    return (SecOC_GetRxPduProcessingPtr(idx)->SecOCRxAuthenticPduLayer);
}

SECOC_LOCAL const SecOC_RxSecuredPduLayerType* SecOC_GetRxSecuredPduLayerPtr(uint16 idx)
{
    return (SecOC_GetRxPduProcessingPtr(idx)->SecOCRxSecuredPduLayer);
}

SECOC_LOCAL const SecOC_RxSecuredPduType* SecOC_GetRxSecuredPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduLayerPtr(idx)->SecOCRxSecuredPdu);
}

SECOC_LOCAL const SecOC_RxSecuredPduCollectionType* SecOC_GetRxSecuredPduCollectionPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduLayerPtr(idx)->SecOCRxSecuredPduCollection);
}

SECOC_LOCAL const SecOC_RxAuthenticPduType* SecOC_GetRxAuthenticPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCRxAuthenticPdu);
}

SECOC_LOCAL const SecOC_RxCryptographicPduType* SecOC_GetRxCryptographicPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCRxCryptographicPdu);
}

#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
/* PRQA S 5017, 6010, 6030, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STCAL */
SECOC_LOCAL void SecOC_RxIfSecuredPduIndication(uint16 index, PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 5017, 6010, 6030, 6070 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxSecuredPduType*            rxSecuredPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    PduInfoType                              pduInfo;

    rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if ((rxSecuredPduPtr->SecOCDynamicLength)
        || ((!rxSecuredPduPtr->SecOCDynamicLength) && (PduInfoPtr->SduLength >= rxSecuredPduPtr->SecOCPduLength)))
    /* PRQA S 2991,2995 -- */
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength  = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxSecuredPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxSecuredPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_00216 */
                /* Queue the Secured I-PDU for a subsequent processing */
                SecOC_DataInQueue(index, RxPduId, &pduInfo);
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            destPtr     = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
            interBufLen = rxSecuredPduPtr->SecOCRxSecuredPduBuffLength;

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U]))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)IStdLib_MemCpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_SECURED_PDU);
                    rxRtPtr->rxPduStatus     = SECOC_RX_CALC;
                    rxRtPtr->rxSecuredPduLen = pduInfo.SduLength;
                    /* @req SWS_SecOC_00235 */
                    rxRtPtr->rxVerifyAttempts    = SECOC_CONST_0U;
                    rxRtPtr->rxAuthBuildAttempts = SECOC_CONST_0U;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}

/* PRQA S 5017, 6010, 6030, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STCAL */
/* PRQA S 6080 ++ */                   /* VL_MTR_SecOC_STPTH */
SECOC_LOCAL void SecOC_RxIfAuthenticPduIndication(uint16 index, const PduInfoType* PduInfoPtr)
/* PRQA S 6080 -- */
/* PRQA S 5017, 6010, 6030, 6070 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxAuthenticPduType*          rxAuthenticPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    PduInfoType                              pduInfo;

    rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if ((rxAuthenticPduPtr->SecOCDynamicLength)
        || ((!rxAuthenticPduPtr->SecOCDynamicLength) && (PduInfoPtr->SduLength >= rxAuthenticPduPtr->SecOCPduLength)))
    /* PRQA S 2991,2995 -- */
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength  = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxAuthenticPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxAuthenticPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                rxRtPtr->rxPduStatus       = SECOC_RX_NONE;
                rxRtPtr->curPduId          = SECOC_CONST_0U;
                rxRtPtr->rxAuthenticPduLen = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxParsedFreshnessValue, 0u, SECOC_CONST_8U);
                (void)IStdLib_MemSet(rxRtPtr->rxParsedAuthenticator, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
                rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
                rxRtPtr->rxAuthBuildAttempts   = SECOC_CONST_0U;
                rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxFreshnessValue, 0u, sizeof(rxRtPtr->rxFreshnessValue));
                SECOC_CLR_RX_EVENT(
                    index,
                    SECOC_RX_INDICATION | SECOC_RX_AUTHENTIC_PDU | SECOC_RX_PARSED_MESSAGE
                        | SECOC_RX_VERIFY_FINISH_IND_WAITING);
                if (NULL_PTR != sameBufferPtr)
                {
                    sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
                }
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            destPtr     = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
            interBufLen = rxAuthenticPduPtr->SecOCRxColAuthenticPduBuffLength;

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U]))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)IStdLib_MemCpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU);
                    if ((SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
                        || (((SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                             || (SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus))
                            && (rxRtPtr->rxNumOfMsgToOverride > SECOC_CONST_0U)))
                    {
                        rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    }
                    rxRtPtr->rxAuthenticPduLen = pduInfo.SduLength;
                    rxRtPtr->rxVerifyAttempts  = SECOC_CONST_0U;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}

/* PRQA S 5017, 6010, 6030, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STCAL */
/* PRQA S 6080 ++ */                   /* VL_MTR_SecOC_STPTH */
SECOC_LOCAL void SecOC_RxIfCryptographicPduIndication(uint16 index, const PduInfoType* PduInfoPtr)
/* PRQA S 6080 -- */
/* PRQA S 5017, 6010, 6030, 6070 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxCryptographicPduType*      rxCryptographicPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    PduInfoType                              pduInfo;

    rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
    /* PRQA S 2991,2995 ++ */ /* VL_SecOC_2991,VL_SecOC_2995 */
    if ((rxCryptographicPduPtr->SecOCDynamicLength)
        || ((!rxCryptographicPduPtr->SecOCDynamicLength)
            && (PduInfoPtr->SduLength >= rxCryptographicPduPtr->SecOCPduLength)))
    /* PRQA S 2991,2995 -- */
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength  = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxCryptographicPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxCryptographicPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                rxRtPtr->rxPduStatus           = SECOC_RX_NONE;
                rxRtPtr->curPduId              = SECOC_CONST_0U;
                rxRtPtr->rxCryptographicPduLen = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxParsedFreshnessValue, 0u, SECOC_CONST_8U);
                (void)IStdLib_MemSet(rxRtPtr->rxParsedAuthenticator, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
                rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
                rxRtPtr->rxAuthBuildAttempts   = SECOC_CONST_0U;
                rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxFreshnessValue, 0u, sizeof(rxRtPtr->rxFreshnessValue));
                SECOC_CLR_RX_EVENT(
                    index,
                    SECOC_RX_INDICATION | SECOC_RX_CRYPTOGRAPHIC_PDU | SECOC_RX_PARSED_MESSAGE
                        | SECOC_RX_VERIFY_FINISH_IND_WAITING);
                if (NULL_PTR != sameBufferPtr)
                {
                    sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
                }
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            destPtr     = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
            interBufLen = rxCryptographicPduPtr->SecOCRxCryptographicPduBuffLength;

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U])
                    || (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU)))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)IStdLib_MemCpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU);
                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU))
                    {
                        rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    }
                    rxRtPtr->rxCryptographicPduLen = pduInfo.SduLength;
                    rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}
#endif /* SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U */

/* PRQA S 5017, 6010, 6020, 6030 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STLIN, VL_MTR_SecOC_STMIF */
/* PRQA S 6070, 6080, 6050 ++ */       /* VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
SECOC_LOCAL void SecOC_RxParseSecuredIpdu(uint16 index)
/* PRQA S 6070, 6080, 6050 -- */
/* PRQA S 5017, 6010, 6020, 6030 -- */
{
    const SecOC_PbRxPduProcessingType*   rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxAuthenticPduLayerType* rxAuthenticPduLayerPtr;
#if (SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U)
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
#endif
    const SecOC_RxSecuredPduType*       rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
    const SecOC_RxAuthenticPduType*     rxAuthenticPduPtr;
    const SecOC_RxCryptographicPduType* rxCryptographicPduPtr;
    SecOC_RxPduRunTimeType*             rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                              rxSecuredPduBufPtr;
    const uint8*                        rxAuthenticPduBufPtr;
    uint8*                              rxCryptographicPduBufPtr;
    PduLengthType                       authPduLen;
    PduLengthType                       offset;
    PduLengthType                       cfgAuthPduLen = rxPduProcessingPtr->SecOCRxAuthenticPduLayer->SecOCPduLength;
    uint8                               headerLen;
    boolean                             securedRxPduVerification;
    boolean                             dropMsgFlag      = FALSE;
    uint8                               fvTruncLenInBits = rxPduProcessingPtr->SecOCFreshnessValueTruncLength;
    uint8                               fvLenInBytes     = (uint8)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits);

    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
    /* If SecOCRxSecuredPduCollection is used then SecOC shall not perform any
     * verification until it has received both the Authentic I-PDU and
     * Cryptographic I-PDU which make up the Secured I-PDU.*/
    if (((SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION)) && (SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU)))
        || ((SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION)) && (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU))
            && (SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU)))
        || (((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
             && (rxRtPtr->rxNumOfMsgToOverride > SECOC_CONST_0U))
            || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)))
    {
#if (SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U)
        /* @req SWS_SecOC_00211 */
        if (E_OK == SecOC_RxMessageLinkCheck(index))
#endif /* SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U */
        {
            /* @req SWS_SecOC_00042 */
            /* Upon reception of a secured I-PDU,SecOC shall parse the Authentic
             * I-PDU, the Freshness Value and the Authenticator from it. */
            if (NULL_PTR != rxSecuredPduPtr)
            {
                SchM_Enter_SecOC_RxData();
                securedRxPduVerification = rxSecuredPduPtr->SecOCSecuredRxPduVerification;

                rxRtPtr->rxParsedAuthPduBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
                rxSecuredPduBufPtr             = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
                headerLen                      = rxSecuredPduPtr->SecOCAuthPduHeaderLength;
                SchM_Exit_SecOC_RxData();
                if (SECOC_CONST_0U == headerLen)
                {
                    /* @req SWS_SecOC_00257 */
                    /* Extract the Authentic I-PDU use configured length */
                    if (!rxSecuredPduPtr->SecOCDynamicLength)
                    {
                        rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
                        authPduLen             = rxAuthenticPduLayerPtr->SecOCPduLength;
                    }
                    else
                    {
                        /* @req SWS_SecOC_00258 */
                        /* Extract the Authentic I-PDU use received length */
                        authPduLen = rxRtPtr->rxSecuredPduLen - rxPduProcessingPtr->SecOCAuthFrsInfoLength;
                    }
                }
                else
                {
                    /* @req SWS_SecOC_00259 */
                    /* Extract the Authentic I-PDU use runtime length */
                    authPduLen = SecOC_RxGetPduLength(headerLen, rxSecuredPduBufPtr);
                    /* @req SWS_SecOC_00263 */
                    /* Runtime length of Authentic I-PDU in the Header is longer
                     * than configured length,Discard the IPDU */
                    if (authPduLen > cfgAuthPduLen)
                    {
                        dropMsgFlag = TRUE;
                    }
                }

                if (!dropMsgFlag)
                {
                    SchM_Enter_SecOC_RxData();
                    offset = headerLen;
                    /* Parse Authentic PDU */
                    (void)IStdLib_MemCpy(rxRtPtr->rxParsedAuthPduBufPtr, &rxSecuredPduBufPtr[offset], authPduLen);
                    offset += authPduLen;
                    /* Parse Freshness value */
                    (void)IStdLib_MemCpy(
                        rxRtPtr->rxParsedFreshnessValue,
                        &rxSecuredPduBufPtr[offset],
                        SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueTruncLength));
                    if (SECOC_CONST_0U != SECOC_MOD_FROM_EIGHT(fvTruncLenInBits))
                    {
                        SecOC_ArrayShiftRight(
                            rxRtPtr->rxParsedFreshnessValue,
                            fvLenInBytes,
                            (SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT((uint16)fvTruncLenInBits)));
                    }
                    /* Parse Authenticator */
                    SecOC_ArrayShiftLeft(
                        &rxSecuredPduBufPtr[offset],
                        rxPduProcessingPtr->SecOCAuthFrsInfoLength,
                        rxPduProcessingPtr->SecOCFreshnessValueTruncLength);
                    (void)IStdLib_MemCpy(
                        rxRtPtr->rxParsedAuthenticator,
                        &rxSecuredPduBufPtr[offset],
                        (uint32)SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength));
                    SchM_Exit_SecOC_RxData();
                }
            }
            else
            {
                rxAuthenticPduPtr        = SecOC_GetRxAuthenticPduPtr(index);
                rxCryptographicPduPtr    = SecOC_GetRxCryptographicPduPtr(index);
                securedRxPduVerification = rxPduProcessingPtr->SecOCRxSecuredPduLayer->SecOCRxSecuredPduCollection
                                               ->SecOCSecuredRxPduVerification;
                rxRtPtr->rxParsedAuthPduBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
                rxAuthenticPduBufPtr           = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
                rxCryptographicPduBufPtr       = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;

                headerLen = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
                if (SECOC_CONST_0U == headerLen)
                {
                    /* @req SWS_SecOC_00257 */
                    /* Extract the Authentic I-PDU use configured length */
                    if (!rxAuthenticPduPtr->SecOCDynamicLength)
                    {
                        authPduLen = rxAuthenticPduPtr->SecOCPduLength;
                    }
                    else
                    {
                        /* @req SWS_SecOC_00258 */
                        /* Extract the Authentic I-PDU use received length */
                        authPduLen = rxRtPtr->rxAuthenticPduLen;
                    }
                }
                else
                {
                    /* @req SWS_SecOC_00259 */
                    /* Extract the Authentic I-PDU use runtime length */
                    authPduLen = SecOC_RxGetPduLength(headerLen, rxAuthenticPduBufPtr);
                    /* @req SWS_SecOC_00263 */
                    /* Runtime length of Authentic I-PDU in the Header is longer
                     * than configured length,Discard the IPDU */
                    if (authPduLen > cfgAuthPduLen)
                    {
                        dropMsgFlag = TRUE;
                    }
                }

                if (!dropMsgFlag)
                {
                    offset = headerLen;
                    SchM_Enter_SecOC_RxData();
                    /* Parse Authentic PDU */
                    (void)IStdLib_MemCpy(rxRtPtr->rxParsedAuthPduBufPtr, &rxAuthenticPduBufPtr[offset], authPduLen);

                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
                    {
                        /* Parse Freshness value */
                        (void)IStdLib_MemCpy(
                            rxRtPtr->rxParsedFreshnessValue,
                            rxCryptographicPduBufPtr,
                            SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueTruncLength));
                        if (SECOC_CONST_0U != SECOC_MOD_FROM_EIGHT(fvTruncLenInBits))
                        {
                            SecOC_ArrayShiftRight(
                                rxRtPtr->rxParsedFreshnessValue,
                                fvLenInBytes,
                                (SECOC_CONST_8U - (uint16)SECOC_MOD_FROM_EIGHT((uint16)fvTruncLenInBits)));
                        }
                        /* Parse Authenticator */
                        SecOC_ArrayShiftLeft(
                            rxCryptographicPduBufPtr,
                            rxPduProcessingPtr->SecOCAuthFrsInfoLength,
                            rxPduProcessingPtr->SecOCFreshnessValueTruncLength);
                        (void)IStdLib_MemCpy(
                            rxRtPtr->rxParsedAuthenticator,
                            rxCryptographicPduBufPtr,
                            SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength));
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
            /* Save Parsed Authentic Pdu Length */
            rxRtPtr->rxParsedAuthPduLen = authPduLen;

            SchM_Enter_SecOC_RxData();
            if (securedRxPduVerification)
            {
                SECOC_SET_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
            }
            else
            {
                /* @req SWS_SecOC_00265 */
                /* SecOC extract the Authentic I-PDU without Authentication*/
                rxRtPtr->rxVerifyResult     = CRYPTO_E_VER_NOT_OK;
                rxRtPtr->rxSkipVerification = TRUE;
                SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            }
            SchM_Exit_SecOC_RxData();
        }
#if (SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U)
        else
        {
            /* Message Link check not passed */
            /* @req SWS_SecOC_00208 */
            /* Use SecOCRxSecuredPduCollection,check may passed when
             * Authentic I-PDU or Cryptographic PDU Update. */
            rxRtPtr->rxPduStatus = SECOC_RX_NONE;
            if (NULL_PTR != sameBufferPtr)
            {
                sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
            }
        }
#endif /* SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U */

        if (dropMsgFlag)
        {
            /* Channel reset */
            SecOC_RxChannelReset(index);
        }
        SchM_Enter_SecOC_RxData();
        SECOC_CLR_RX_EVENT(index, SECOC_RX_INDICATION);
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        rxRtPtr->rxPduStatus = SECOC_RX_NONE;
    }
}

SECOC_LOCAL void SecOC_RxVerifyProcee(uint16 index)
{
    SecOC_RxPduRunTimeType* rxRtPtr = SecOC_RxRtDataPtr(index);

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE))
    {
        /* Handle numberOfMessageOverride counter */
        rxRtPtr->rxBakNumOfMsgToOverride = rxRtPtr->rxNumOfMsgToOverride;

        if (((SECOC_OVERRIDE_DROP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
             && (rxRtPtr->rxNumOfMsgToOverride == SECOC_CONST_0U))
            || (SECOC_OVERRIDE_CANCEL == rxRtPtr->rxOverrideStatus)
            || (SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || ((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxNumOfMsgToOverride == SECOC_CONST_0U))
            || (SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus))
        {
            /* Performe authenticator verification */
            SecOC_RxVerifyAuthenticationInfo(index);
        }
        else
        {
            /* Not performe authenticator verification */
            SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE | SECOC_RX_CRYPTOGRAPHIC_PDU);
            rxRtPtr->rxVeryfyResult = SECOC_NO_VERIFICATION;

            if ((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus))
            {
                /* I-PDU is sent to upper layer */
                rxRtPtr->rxVerifyResult = CRYPTO_E_VER_NOT_OK;
                SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            }
            else
            {
                /* Drop message */
                /* Report verify status */
                SecOC_RxReportVerifyStatus(index);
                if (NULL_PTR != SecOC_GetRxPduProcessingDataPtr(index)->SecOCSameBufferPduRef)
                {
                    SecOC_GetRxPduProcessingDataPtr(index)
                        ->SecOCSameBufferPduRef->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
                }
                rxRtPtr->rxPduStatus = SECOC_RX_NONE;
            }
        }

        /* Handle numberOfMessageOverride counter */
        if ((rxRtPtr->rxNumOfMsgToOverride > SECOC_CONST_0U) && (SECOC_RX_RETRY != rxRtPtr->rxPduStatus))
        {
            rxRtPtr->rxNumOfMsgToOverride--;
        }
    }
}
/* PRQA S 6070, 6030 ++ */ /* VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STMIF */
SECOC_LOCAL void SecOC_RxVerifyAuthenticationInfo(uint16 index)
/* PRQA S 6070, 6030 -- */
{
    const SecOC_PbRxPduProcessingType* rxPduProcessingPtr     = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxPduProcessingType*   rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    SecOC_RxPduRunTimeType*            rxRtPtr                = SecOC_RxRtDataPtr(index);
    const SecOC_CsmJobType*            jobPtr                 = rxPduProcessingDataPtr->SecOCRxAuthServiceConfigRef;
    uint32                             dataLength;
    Std_ReturnType                     ret;
    ApplicationType                    aplId = SecOC_GetApplicationID();
    uint8*                             SecOC_DataToAuthenticator;
    boolean getAutherenticatorBuffer = SecOC_GetRxDataToAuthenticator(aplId, &SecOC_DataToAuthenticator);

    if ((getAutherenticatorBuffer) && (SECOC_IS_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE)))
    {
        dataLength = SecOC_RxCalcDataToAuthenticatorLength(index);
        ret        = SecOC_RxConstructDataToAuthenticator(index);
        if (E_OK == ret)
        {
            /* @req SWS_SecOC_00047 */
            switch (jobPtr->SecOCCsmPrimitives)
            {
#if (SECOC_CSMMAC_ENABLED == STD_ON)
            case SECOC_CSMMAC:
                ret = Csm_MacVerify(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[SECOC_CONST_0U],
                    dataLength,
                    rxRtPtr->rxParsedAuthenticator,
                    rxPduProcessingPtr->SecOCAuthInfoTruncLength,
                    &rxRtPtr->rxVerifyResult);
                break;
#endif /* SECOC_CSMMAC_ENABLED == STD_ON */

#if (SECOC_CSMSIGNATRUE_ENABLED == STD_ON)
            case SECOC_CSMSIGNATURE:
                ret = Csm_SignatureVerify(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[SECOC_CONST_0U],
                    dataLength,
                    rxRtPtr->rxParsedAuthenticator,
                    SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength),
                    &rxRtPtr->rxVerifyResult);
                break;
#endif /* SECOC_CSMSIGNATRUE_ENABLED == STD_ON */

            default:
                /* Nothing to do here*/
                break;
            }

            if (E_OK == ret)
            {
                if (SECOC_CRYPTO_PROCESSING_SYNC == jobPtr->SecOCCryptoProcessingType)
                {
                    SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                    /* @req SWS_SecOC_00239 */
                    /* Verification of the authenticator successfully executed,
                     * the authentication build counter shall be set to 0. */
                    rxRtPtr->rxAuthBuildAttempts = SECOC_CONST_0U;
                }
                else
                {
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_WAITING);
                }
            }
            else if (CRYPTO_E_BUSY == ret)
            {
                /* @req SWS_SecOC_00121,SWS_SecOC_00237 */
                /* Recoverable error */
                SecOC_RxAuthenticationBuildRetry(index);
            }
            else
            {
                /* No-recoverable error */
                /* @req SWS_SecOC_00241 */
                if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                    || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                        && (rxRtPtr->rxBakNumOfMsgToOverride > SECOC_CONST_0U))
                    || (SecOC_IgnoreVerificationResultIsEnabled))
                {
                    /* Continue to send authentic I-PDU to upper layer */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                }
                else if (
                    (E_NOT_OK != ret)
                    && (rxRtPtr->rxVerifyAttempts
                        <= SecOC_GetRxPduProcessingDataPtr(index)->SecOCAuthenticationVerifyAttempts))
                {
                    /* @req SWS_SecOC_00239 */
                    /* Verification of the authenticator successfully executed but the
                     * verification failed. */
                    SecOC_RxVerifyAuthenticInfoRetry(index);
                }
                else
                {
                    rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
                    /* Report verify status */
                    SecOC_RxReportVerifyStatus(index);
#if (SECOC_SECURITY_EVENT_REPORTING_ENABLED == STD_ON)
                    uint16 dataId = rxPduProcessingPtr->SecOCDataId;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
                    SecOC_EndianSwap((uint8*)&dataId, sizeof(dataId));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
                    IdsM_SetSecurityEventWithContextData(
                        SECOC_SEV_SECOC_MAC_VERIFICATION_FAILED,
                        (const uint8*)&dataId,
                        SECOC_DATAID_LEN);
#endif /* SECOC_SECURITY_EVENT_REPORTING_ENABLED  == STD_ON */
                    /* Non-recoverable error occur */
                    SecOC_RxChannelReset(index);
                }
            }
        }
    }
}

SECOC_LOCAL uint32 SecOC_RxCalcDataToAuthenticatorLength(uint16 index)
{
    const SecOC_PbRxPduProcessingType* rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxPduRunTimeType*      rxRtPtr            = SecOC_RxRtDataPtr(index);
    uint32                             dataLength         = SECOC_DATAID_LEN;

    if (NULL_PTR != rxPduProcessingPtr->SecOCRxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduLength;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + rxRtPtr->rxParsedAuthPduLen;
    }

    dataLength += SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueLength);

    return dataLength;
}

/* PRQA S 6070, 6020 ++ */ /* VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STLIN */
SECOC_LOCAL Std_ReturnType SecOC_RxConstructDataToAuthenticator(uint16 index)
/* PRQA S 6070, 6020 -- */
{
    const SecOC_PbRxPduProcessingType* rxPduProcessingPtr     = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxPduProcessingType*   rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    SecOC_RxPduRunTimeType*            rxRtPtr                = SecOC_RxRtDataPtr(index);
    const SecOC_RxSecuredPduType*      rxSecuredPduPtr        = SecOC_GetRxSecuredPduPtr(index);
    const SecOC_RxAuthenticPduType*    rxAuthenticPduPtr;
    const uint8*                       authenticPduPtr;
    const uint8*                       authDataFreshnessPtr;
    uint32                             securedIPDULength;
    uint32                             securedIPDUOffset;
    uint32                             freshnessValueLen;
    uint16                             dataId;
    uint8                              headerLen;
    Std_ReturnType                     ret;
    ApplicationType                    aplId = SecOC_GetApplicationID();
    uint8*                             SecOC_DataToAuthenticator;
    (void)SecOC_GetRxDataToAuthenticator(aplId, &SecOC_DataToAuthenticator);

    SchM_Enter_SecOC_Authentication();

    (void)IStdLib_MemSet(SecOC_DataToAuthenticator, 0u, SECOC_DATA_TO_AUTHENTICATOR_LENGTH);
    /* @req SWS_SecOC_00046 */
    /*
     * DataToAuthenticator = Data Identifier | secured part of the Authentic
     * I-PDU | Complete Freshness Value
     */

    /* Data Identifie */
    dataId = rxPduProcessingPtr->SecOCDataId;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SecOC_EndianSwap((uint8*)&dataId, sizeof(dataId));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
    (void)IStdLib_MemCpy(&SecOC_DataToAuthenticator[0], (uint8*)&dataId, SECOC_DATAID_LEN);

    /* Secured part of the Authentic */
    if (NULL_PTR != rxPduProcessingPtr->SecOCRxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduLength;
        securedIPDUOffset = rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduOffset;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct
         * DataToAuthenticator.*/
        securedIPDULength = rxRtPtr->rxParsedAuthPduLen;
        securedIPDUOffset = SECOC_CONST_0U;
    }

    (void)IStdLib_MemCpy(
        &SecOC_DataToAuthenticator[SECOC_DATAID_LEN],
        &rxRtPtr->rxParsedAuthPduBufPtr[securedIPDUOffset],
        securedIPDULength);

    SchM_Exit_SecOC_Authentication();
    /* Complete Freshness Value */
    /* @req SWS_SecOC_00219 */
    /* SecOC use a part of the Authentic I-PDU as freshness */
    if (rxPduProcessingDataPtr->SecOCAuthDataFreshnessLen > SECOC_CONST_0U)
    {
        if (NULL_PTR != rxSecuredPduPtr)
        {
            headerLen       = rxSecuredPduPtr->SecOCAuthPduHeaderLength;
            authenticPduPtr = &rxSecuredPduPtr->SecOCRxSecuredPduBufferRef[headerLen];
        }
        else
        {
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
            headerLen         = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
            authenticPduPtr   = &rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef[headerLen];
        }
        /* Get the byte address of the start bit */
        SecOC_CopyBitsFromSourceToDestination(
            authenticPduPtr,
            rxPduProcessingPtr->SecOCRxAuthDataFreshnessBufferRef,
            rxPduProcessingDataPtr->SecOCAuthDataFreshnessStartPosition,
            rxPduProcessingDataPtr->SecOCAuthDataFreshnessLen);
        authDataFreshnessPtr = rxPduProcessingPtr->SecOCRxAuthDataFreshnessBufferRef;
    }
    else
    {
        authDataFreshnessPtr = NULL_PTR;
    }

    freshnessValueLen = rxPduProcessingPtr->SecOCFreshnessValueLength;
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
    if (rxPduProcessingDataPtr->SecOCUseAuthDataFreshness)
    {
        /* @req SWS_SecOC_00246,SWS_SecOC_00249 */
        ret = SecOC_GetRxFreshnessAuthData(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            authDataFreshnessPtr,
            rxPduProcessingDataPtr->SecOCAuthDataFreshnessLen,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
    else
    {
        /* @req SWS_SecOC_00247,SWS_SecOC_00250 */
        ret = SecOC_GetRxFreshness(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
#else
#if (STD_ON == SECOC_USE_AUTHDATA_FRESHNESS_ENABLED)
    if (rxPduProcessingDataPtr->SecOCUseAuthDataFreshness)
    {
        /* @req SWS_SecOC_00244 */
        ret = Rte_Call_FreshnessManagement_GetRxFreshnessAuthData(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            authDataFreshnessPtr,
            rxPduProcessingDataPtr->SecOCAuthDataFreshnessLen,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
    else
#endif /* STD_ON == SECOC_USE_AUTHDATA_FRESHNESS_ENABLED */
    {
#if (STD_ON == SECOC_USE_AUTHDATA_FRESHNESS_DISABLED)
        /* @req SWS_SecOC_00245 */
        ret = Rte_Call_FreshnessManagement_GetRxFreshness(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
#endif /* STD_ON == SECOC_USE_AUTHDATA_FRESHNESS_DISABLED */
    }
#endif

    if (E_OK == ret)
    {
        SchM_Enter_SecOC_Authentication();
        freshnessValueLen = (uint32)SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueLength);
        (void)IStdLib_MemCpy(
            &SecOC_DataToAuthenticator[SECOC_DATAID_LEN + securedIPDULength],
            rxRtPtr->rxFreshnessValue,
            freshnessValueLen);
        SchM_Exit_SecOC_Authentication();
    }
    else if (E_BUSY == ret)
    {
        /* @req SWS_SecOC_00236 */
        SecOC_RxAuthenticationBuildRetry(index);
    }
    else
    {
        /* @req SWS_SecOC_00256 */
        rxRtPtr->rxVeryfyResult = SECOC_FRESHNESSFAILURE;
        /* Report verify status */
        SecOC_RxReportVerifyStatus(index);
#if (SECOC_SECURITY_EVENT_REPORTING_ENABLED == STD_ON)
        IdsM_SetSecurityEvent(SECOC_SEV_SECOC_FRESHNESS_NOT_AVAILABLE);
#endif /* SECOC_SECURITY_EVENT_REPORTING_ENABLED  == STD_ON */

        /* Get freshness value return E_NOT_OK,Reset channel */
        SecOC_RxChannelReset(index);
    }

    return ret;
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL */
SECOC_LOCAL void SecOC_RxAuthenticationBuildRetry(uint16 index)
/* PRQA S 6070 -- */
{
    SecOC_RxPduRunTimeType* rxRtPtr = SecOC_RxRtDataPtr(index);

    /* @req SWS_SecOC_00236,SWS_SecOC_00237,SWS_SecOC_00238 */
    if (rxRtPtr->rxAuthBuildAttempts < SecOC_GetRxPduProcessingDataPtr(index)->SecOCAuthenticationBuildAttempts)
    {
        SchM_Enter_SecOC_RxData();
        rxRtPtr->rxAuthBuildAttempts++;
        rxRtPtr->rxPduStatus = SECOC_RX_RETRY;
#if (SECOC_PROPAGATE_ONLY_FINALVERIFICATION_STATUS_ENABLED == STD_OFF)
        rxRtPtr->rxVeryfyResult = SECOC_AUTHENTICATIONBUILDFAILURE;
        SecOC_RxReportVerifyStatus(index);
#endif /* SECOC_PROPAGATE_ONLY_FINALVERIFICATION_STATUS_ENABLED == STD_OFF */
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00240 */
        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxBakNumOfMsgToOverride > SECOC_CONST_0U)))
        {
            SchM_Enter_SecOC_RxData();
            /* According override setup, send authentic IPDU to upper layer */
            rxRtPtr->rxVerifyResult = CRYPTO_E_VER_NOT_OK;
            SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            SchM_Exit_SecOC_RxData();
        }
        else
        {
            SchM_Enter_SecOC_RxData();
            /* @req SWS_SecOC_00240 */
            rxRtPtr->rxVeryfyResult = SECOC_AUTHENTICATIONBUILDFAILURE;
            SchM_Exit_SecOC_RxData();
            /* Report verify status */
            SecOC_RxReportVerifyStatus(index);

            /* Authentication Build retry fail */
            if (SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU))
            {
                /* Not use SecOCRxSecuredPduCollection, channel reset */
                SecOC_RxChannelReset(index);
            }
            else
            {
                SchM_Enter_SecOC_RxData();
                /* Use SecOCRxSecuredPduCollection,Authentication build may
                 * success when Authentic I-PDU or Cryptographic PDU Update */
                SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                SchM_Exit_SecOC_RxData();
            }
        }

        /* @req SWS_SecOC_00248 */
        /* Report runtime error to DET */
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
        SECOC_DET_REPORT_RUNTIMEERROR(SECOC_MAINFUNCTIONRX_ID, SECOC_E_FRESHNESS_FAILURE);
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    }
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL */
SECOC_LOCAL void SecOC_RxVerifyAuthenticInfoRetry(uint16 index)
/* PRQA S 6070 -- */
{
    SecOC_RxPduRunTimeType* rxRtPtr = SecOC_RxRtDataPtr(index);

    /* @req SWS_SecOC_00239 */
    if (rxRtPtr->rxVerifyAttempts < SecOC_GetRxPduProcessingDataPtr(index)->SecOCAuthenticationVerifyAttempts)
    {
        SchM_Enter_SecOC_RxData();
        rxRtPtr->rxVerifyAttempts++;
        rxRtPtr->rxPduStatus = SECOC_RX_RETRY;
#if (SECOC_PROPAGATE_ONLY_FINALVERIFICATION_STATUS_ENABLED == STD_OFF)
        rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
        SecOC_RxReportVerifyStatus(index);
#endif /* SECOC_PROPAGATE_ONLY_FINALVERIFICATION_STATUS_ENABLED == STD_OFF */
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        SchM_Enter_SecOC_RxData();
        /* @req SWS_SecOC_00241 */
        rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
        SchM_Exit_SecOC_RxData();
        /* Report verify status */
        SecOC_RxReportVerifyStatus(index);

        /* Verify Authentication retry fail */
        if (SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU))
        {
            /* Not use SecOCRxSecuredPduCollection, channel reset */
            SecOC_RxChannelReset(index);
        }
        else
        {
            SchM_Enter_SecOC_RxData();
            /* Use SecOCRxSecuredPduCollection,Verify authentication may
               success when Authentic I-PDU or Cryptographic PDU Update */
            SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
            SchM_Exit_SecOC_RxData();
        }
    }
}

#if (SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U)

SECOC_LOCAL const SecOC_UseMessageLinkType* SecOC_GetRxUseMessageLinkPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCUseMessageLink);
}

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL */
SECOC_LOCAL Std_ReturnType SecOC_RxMessageLinkCheck(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbRxPduProcessingType*       rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr =
        SecOC_GetRxPduProcessingDataPtr(index)->SecOCSameBufferPduRef;
    const SecOC_RxPduRunTimeType*           rxRtPtr                   = SecOC_RxRtDataPtr(index);
    const SecOC_RxSecuredPduCollectionType* rxSecuredPduCollectionPtr = SecOC_GetRxSecuredPduCollectionPtr(index);
    const SecOC_UseMessageLinkType*         msgLinkPtr;
    const SecOC_RxAuthenticPduType*         rxAuthenticPduPtr;
    const SecOC_RxCryptographicPduType*     rxCryptographicPduPtr;
    uint8*                                  tBufPtr;
    const uint8*                            cryptoPduPtr;
    uint8                                   msgLinkArray[SECOC_MAX_MSG_LINK_LEN]       = {0};
    uint8                                   msgCryptoLinkArray[SECOC_MAX_MSG_LINK_LEN] = {0};
    uint16                                  offsetBits;
    PduLengthType                           authPduLen;
    uint16                                  msgLinkStartPos;
    uint16                                  msgLinkLenInByte;
    uint8                                   headerLen;
    Std_ReturnType                          ret = E_OK;

    if ((NULL_PTR != rxSecuredPduCollectionPtr))
    {
        SchM_Enter_SecOC_RxData();
        msgLinkPtr = SecOC_GetRxUseMessageLinkPtr(index);
        if ((NULL_PTR != msgLinkPtr) && (SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION))
            && (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU))
            && (SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU)))
        {
            /* Pickup message link from Authentic I-PDU */
            rxAuthenticPduPtr     = SecOC_GetRxAuthenticPduPtr(index);
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
            headerLen             = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
            authPduLen            = rxRtPtr->rxAuthenticPduLen - (PduLengthType)headerLen;

            /* Copy Authentic I-PDU to temporary buffer */
            tBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
            (void)IStdLib_MemCpy(
                tBufPtr,
                &rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef[headerLen],
                (uint32)authPduLen);
            cryptoPduPtr = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;

            /* Copy message link from Authentic I-PDU to temporary buffer */
            SecOC_CopyBitsFromSourceToDestination(
                tBufPtr,
                msgLinkArray,
                msgLinkPtr->SecOCMessageLinkPos,
                msgLinkPtr->SecOCMessageLinkLen);

            /* Pickup message link from Cryptographic Pdu */
            offsetBits =
                rxPduProcessingPtr->SecOCAuthInfoTruncLength + rxPduProcessingPtr->SecOCFreshnessValueTruncLength;
            SecOC_CopyBitsFromSourceToDestination(
                cryptoPduPtr,
                msgCryptoLinkArray,
                offsetBits,
                msgLinkPtr->SecOCMessageLinkLen);

            /* Compare message link */
            msgLinkLenInByte = SECOC_BIT_TO_BYTE(msgLinkPtr->SecOCMessageLinkLen);
            if (0u != IStdLib_MemCmp(msgCryptoLinkArray, msgLinkArray, msgLinkLenInByte))
            {
                ret = E_NOT_OK;
            }
        }
        SchM_Exit_SecOC_RxData();
    }

    return ret;
}
#endif /* SECOC_MAX_MSG_LINK_LEN > SECOC_CONST_0U */

/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL */
SECOC_LOCAL void SecOC_RxPassAuthenticPduToUpperLayer(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_RxAuthenticPduLayerType* rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
    SecOC_RxPduRunTimeType*              rxRtPtr                = SecOC_RxRtDataPtr(index);
    PduInfoType                          pduInfo;
    PduIdType                            pduId;
#if (SECOC_RX_TP_PDU_NUM > 0)
    BufReq_ReturnType bufReqRet;
#endif

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED))
    {
        SECOC_CLR_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);

        /* @req SWS_SecOC_00081,SWS_SecOC_00088 */
        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || (((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                 || (SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus))
                && (rxRtPtr->rxBakNumOfMsgToOverride > SECOC_CONST_0U))
            || ((SECOC_OVERRIDE_DROP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxBakNumOfMsgToOverride == SECOC_CONST_0U))
            || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || (CRYPTO_E_VER_OK == rxRtPtr->rxVerifyResult) || (SecOC_IgnoreVerificationResultIsEnabled)
            || (rxRtPtr->rxSkipVerification))
        {
            SchM_Enter_SecOC_RxData();
            /* I-PDU is sent to upper layer*/
            pduInfo.SduLength  = rxRtPtr->rxParsedAuthPduLen;
            pduInfo.SduDataPtr = rxRtPtr->rxParsedAuthPduBufPtr;
            SchM_Exit_SecOC_RxData();
            pduId = rxAuthenticPduLayerPtr->SecOCRxPduRAsUpLayerId;
            /* @req SWS_SecOC_00050,SWS_SecOC_00080 */
            if (SECOC_IFPDU == rxAuthenticPduLayerPtr->SecOCPduType)
            {
                /* @req SWS_SecOC_00080,SWS_SecOC_00086 */
                PduR_SecOCRxIndication(pduId, &pduInfo);
                /* @req SWS_SecOC_00207 */
                /* Channel Reset */
                SecOC_RxChannelReset(index);
            }
#if (SECOC_RX_TP_PDU_NUM > 0)
            else
            {
                /* @req SWS_SecOC_00082 */
                /* Upper layer is TP pdu,Notify upper layer start of reception*/
                SchM_Enter_SecOC_RxData();
                rxRtPtr->upLayerBufSize = SECOC_CONST_0U;
                rxRtPtr->sduRemaining   = rxRtPtr->rxParsedAuthPduLen;
                SchM_Exit_SecOC_RxData();
                bufReqRet = PduR_SecOCTpStartOfReception(pduId, NULL_PTR, pduInfo.SduLength, &rxRtPtr->upLayerBufSize);
                if ((BUFREQ_OK == bufReqRet) && (rxRtPtr->upLayerBufSize > SECOC_CONST_0U))
                {
                    SchM_Enter_SecOC_RxData();
                    SECOC_SET_RX_EVENT(index, SECOC_RX_TRANS_TP_DATA_TO_UPPER);
                    SchM_Exit_SecOC_RxData();
                }
                else
                {
                    /* PduR return error,Abort transmit data to upper layer */
                    PduR_SecOCTpRxIndication(pduId, E_NOT_OK);
                    /* Channel Reset */
                    SecOC_RxChannelReset(index);
                }
            }
#endif
        }

        SchM_Enter_SecOC_RxData();
        if (CRYPTO_E_VER_OK == rxRtPtr->rxVerifyResult)
        {
            /* @req SWS_SecOC_00242 */
            rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONSUCCESS;
        }
        else
        {
            if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                    && (rxRtPtr->rxBakNumOfMsgToOverride > SECOC_CONST_0U)))
            {
                rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE_OVERWRITTEN;
            }
            else if (
                (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE != rxRtPtr->rxOverrideStatus)
                && (SECOC_OVERRIDE_SKIP_UNTIL_LIMIT != rxRtPtr->rxOverrideStatus))
            {
                rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
            }
            else
            {
                /* Do nothing */
            }
        }
        SchM_Exit_SecOC_RxData();
        /* Report verify status */
        SecOC_RxReportVerifyStatus(index);
    }
}

#if (SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U)
/* PRQA S 6070 ++ */ /* VL_MTR_SecOC_STCAL */
SECOC_LOCAL void SecOC_RxTransTpDataToUpperLayer(uint16 index)
/* PRQA S 6070 -- */
{
    const SecOC_PbRxPduProcessingType*   rxPduProcessingPtr     = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxAuthenticPduLayerType* rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
    SecOC_RxPduRunTimeType*              rxRtPtr                = SecOC_RxRtDataPtr(index);
    BufReq_ReturnType                    bufReqRet;
    PduInfoType                          pduInfo;
    PduLengthType                        pduLen;
    PduLengthType                        offset = rxRtPtr->rxParsedAuthPduLen - rxRtPtr->sduRemaining;
    PduIdType                            pduId;
    boolean                              quitFlag = FALSE;

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_TRANS_TP_DATA_TO_UPPER))
    {
        SchM_Enter_SecOC_RxData();
        pduLen = SECOC_MIN(rxRtPtr->sduRemaining, rxRtPtr->upLayerBufSize);
        pduId  = rxAuthenticPduLayerPtr->SecOCRxPduRAsUpLayerId;

        /* @req SWS_SecOC_00086 */
        /* Copy data to upper layer complete */
        pduInfo.SduDataPtr = &rxRtPtr->rxParsedAuthPduBufPtr[offset];
        pduInfo.SduLength  = pduLen;
        SchM_Exit_SecOC_RxData();
        bufReqRet = PduR_SecOCTpCopyRxData(pduId, &pduInfo, &rxRtPtr->upLayerBufSize);
        if (BUFREQ_OK == bufReqRet)
        {
            SchM_Enter_SecOC_RxData();
            rxRtPtr->sduRemaining -= pduLen;
            SchM_Exit_SecOC_RxData();
            if (SECOC_CONST_0U == rxRtPtr->sduRemaining)
            {
                /* Notify upper layer data copy finished */
                PduR_SecOCTpRxIndication(pduId, E_OK);
                /* @req SWS_SecOC_00207 */
                /* Channel Reset */
                SecOC_RxChannelReset(index);
            }
            else
            {
                if (SECOC_CONST_0U == rxRtPtr->upLayerBufSize)
                {
                    /* PduR return error,Abort transmit data to upper layer */
                    quitFlag = TRUE;
                }
            }
        }
        else
        {
            /* @req SWS_SecOC_00267 */
            /* PduR return BUFREQ_E_NOT_OK,Abort transmit data to upper layer */
            quitFlag = TRUE;
        }

        if (quitFlag)
        {
            /* @req SWS_SecOC_00267 */
            /* Notify PduR with E_NOT_OK */
            PduR_SecOCTpRxIndication(pduId, E_NOT_OK);
            /* Channel Reset */
            SecOC_RxChannelReset(index);
        }
    }
}

/* PRQA S 5017, 6010, 6030, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STCAL */
/* PRQA S 6080 ++ */                   /* VL_MTR_SecOC_STPTH */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpSecuredPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 6080 -- */
/* PRQA S 5017, 6010, 6030, 6070 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxSecuredPduType*            rxSecuredPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    BufReq_ReturnType                        bufReqRet = BUFREQ_E_NOT_OK;

    if (SECOC_CONST_0U != TpSduLength)
    {
        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* Not support tp data */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }
        SchM_Enter_SecOC_RxData();
        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);

            destPtr     = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
            interBufLen = rxSecuredPduPtr->SecOCRxSecuredPduBuffLength;

            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U]))
                {
                    rxRtPtr->sduRemaining  = TpSduLength;
                    rxRtPtr->tpRxPduOffset = SECOC_CONST_0U;
                    if (NULL_PTR != info)
                    {
                        (void)IStdLib_MemCpy(destPtr, info->SduDataPtr, info->SduLength);
                        rxRtPtr->sduRemaining -= info->SduLength;
                        rxRtPtr->tpRxPduOffset += info->SduLength;
                    }

                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_SECURED_PDU);
                    rxRtPtr->rxPduStatus      = SECOC_RX_CALC;
                    rxRtPtr->rxSecuredPduLen  = TpSduLength;
                    rxRtPtr->rxVerifyAttempts = SECOC_CONST_0U;
                    rxRtPtr->curPduId         = id;
                    rxRtPtr->tpRxDestBufPtr   = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
        }
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0,return BUFREQ_E_NOT_OK and no further action
         * shall be taken.
         */
    }

    return bufReqRet;
}

/* PRQA S 5017, 6010, 6030, 6070 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STMIF, VL_MTR_SecOC_STCAL */
/* PRQA S 6080, 6050 ++ */             /* VL_MTR_SecOC_STPTH, VL_MTR_SecOC_STST3 */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpAuthenticPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 6080, 6050 -- */
/* PRQA S 5017, 6010, 6030, 6070 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxAuthenticPduType*          rxAuthenticPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    BufReq_ReturnType                        bufReqRet = BUFREQ_E_NOT_OK;

    if (SECOC_CONST_0U != TpSduLength)
    {
        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                /* Reset runtime data */
                rxRtPtr->rxSkipVerification = FALSE;
                rxRtPtr->rxPduStatus        = SECOC_RX_NONE;
                rxRtPtr->curPduId           = SECOC_CONST_0U;
                rxRtPtr->rxSecuredPduLen    = SECOC_CONST_0U;
                rxRtPtr->rxAuthenticPduLen  = SECOC_CONST_0U;
                rxRtPtr->rxParsedAuthPduLen = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxParsedFreshnessValue, 0u, SECOC_CONST_8U);
                (void)IStdLib_MemSet(rxRtPtr->rxParsedAuthenticator, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
                rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
                rxRtPtr->rxAuthBuildAttempts   = SECOC_CONST_0U;
                rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxFreshnessValue, 0u, sizeof(rxRtPtr->rxFreshnessValue));
                rxRtPtr->tpRxDestBufPtr = NULL_PTR;
                rxRtPtr->sduRemaining   = SECOC_CONST_0U;
                rxRtPtr->upLayerBufSize = SECOC_CONST_0U;
                rxRtPtr->tpRxPduOffset  = SECOC_CONST_0U;

                /* Unlock Samebuffer */
                if (NULL_PTR != sameBufferPtr)
                {
                    sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
                }
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }
        SchM_Enter_SecOC_RxData();
        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);

            destPtr     = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
            interBufLen = rxAuthenticPduPtr->SecOCRxColAuthenticPduBuffLength;

            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U]))
                {
                    rxRtPtr->sduRemaining  = TpSduLength;
                    rxRtPtr->tpRxPduOffset = SECOC_CONST_0U;
                    if (NULL_PTR != info)
                    {
                        (void)IStdLib_MemCpy(destPtr, info->SduDataPtr, info->SduLength);
                        rxRtPtr->sduRemaining -= info->SduLength;
                        rxRtPtr->tpRxPduOffset += info->SduLength;
                    }

                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    rxRtPtr->rxPduStatus       = SECOC_RX_CALC;
                    rxRtPtr->rxAuthenticPduLen = TpSduLength;
                    rxRtPtr->rxVerifyAttempts  = SECOC_CONST_0U;
                    rxRtPtr->curPduId          = id;
                    rxRtPtr->tpRxDestBufPtr    = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
        }
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0,return BUFREQ_E_NOT_OK and no further action
         * shall be taken.
         */
    }

    return bufReqRet;
}

/* PRQA S 5017, 6070, 6010, 6050 ++ */ /* VL_SecOC_5017, VL_MTR_SecOC_STCAL, VL_MTR_SecOC_STCYC, VL_MTR_SecOC_STST3 */
/* PRQA S 6030 ++ */                   /* VL_MTR_SecOC_STMIF */
SECOC_LOCAL BufReq_ReturnType SecOC_RxTpCryptographicPduStartOfReception(
    uint16             index,
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 6030 -- */
/* PRQA S 5017, 6070, 6010, 6050 -- */
{
    const SecOC_RxPduProcessingType*         rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    const SecOC_SameBufferPduCollectionType* sameBufferPtr          = rxPduProcessingDataPtr->SecOCSameBufferPduRef;
    const SecOC_RxCryptographicPduType*      rxCryptographicPduPtr;
    SecOC_RxPduRunTimeType*                  rxRtPtr = SecOC_RxRtDataPtr(index);
    uint8*                                   destPtr;
    PduLengthType                            interBufLen;
    BufReq_ReturnType                        bufReqRet = BUFREQ_E_NOT_OK;

    if ((SECOC_CONST_0U != TpSduLength) && (NULL_PTR != info) && (info->SduLength <= TpSduLength))
    {
        if ((SECOC_RX_NONE != rxRtPtr->rxPduStatus) && (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER)))
        {
            switch (rxPduProcessingDataPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                rxRtPtr->rxSkipVerification    = FALSE;
                rxRtPtr->rxPduStatus           = SECOC_RX_NONE;
                rxRtPtr->curCryptoPduId        = SECOC_CONST_0U;
                rxRtPtr->rxSecuredPduLen       = SECOC_CONST_0U;
                rxRtPtr->rxCryptographicPduLen = SECOC_CONST_0U;
                rxRtPtr->rxParsedAuthPduLen    = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxParsedFreshnessValue, 0u, SECOC_CONST_8U);
                (void)IStdLib_MemSet(rxRtPtr->rxParsedAuthenticator, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
                rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
                rxRtPtr->rxAuthBuildAttempts   = SECOC_CONST_0U;
                rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                (void)IStdLib_MemSet(rxRtPtr->rxFreshnessValue, 0u, sizeof(rxRtPtr->rxFreshnessValue));

                rxRtPtr->upLayerBufSize       = SECOC_CONST_0U;
                rxRtPtr->tpRxCryptoDestBufPtr = NULL_PTR;
                rxRtPtr->sduCryptoRemaining   = SECOC_CONST_0U;
                rxRtPtr->tpRxCryptoPduOffset  = SECOC_CONST_0U;

                /* Unlock Samebuffer */
                if (NULL_PTR != sameBufferPtr)
                {
                    sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = FALSE;
                }
                SECOC_CLR_RX_EVENT(
                    index,
                    SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER | SECOC_RX_VERIFY_FINISH_IND_WAITING | SECOC_RX_PARSED_MESSAGE
                        | SECOC_RX_CRYPTOGRAPHIC_PDU | SECOC_RX_INDICATION);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if ((SECOC_RX_NONE == rxRtPtr->rxPduStatus) || (!SECOC_IS_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER)))
        {
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
            SchM_Enter_SecOC_RxData();
            destPtr     = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
            interBufLen = rxCryptographicPduPtr->SecOCRxCryptographicPduBuffLength;

            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (!sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U])
                    || (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER))
                    || (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU)))
                {
                    rxRtPtr->sduCryptoRemaining  = TpSduLength;
                    rxRtPtr->tpRxCryptoPduOffset = SECOC_CONST_0U;

                    (void)IStdLib_MemCpy(destPtr, info->SduDataPtr, info->SduLength);
                    rxRtPtr->sduCryptoRemaining -= info->SduLength;
                    rxRtPtr->tpRxCryptoPduOffset += info->SduLength;
                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_CRYPTO_DATA_FROM_LOWER);
                    rxRtPtr->rxPduStatus           = SECOC_RX_CALC;
                    rxRtPtr->rxCryptographicPduLen = TpSduLength;
                    rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
                    rxRtPtr->curCryptoPduId        = id;
                    rxRtPtr->tpRxCryptoDestBufPtr  = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[SECOC_CONST_0U] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
            SchM_Exit_SecOC_RxData();
        }
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0 or info->SduLength less than TpSduLength,return
         * BUFREQ_E_NOT_OK and no further action shall be taken.
         */
    }

    return bufReqRet;
}
#endif /* SECOC_RX_TP_PDU_NUM > SECOC_CONST_0U */

#if (SECOC_RX_IF_PDU_NUM > SECOC_CONST_0U)
SECOC_LOCAL void SecOC_RxLoadQueuedPdu(uint16 index)
{
    uint8          SecOC_TempMsgBuf[SECOC_MAX_RX_SECURED_MSG_LENGTH];
    PduInfoType    pduInfo;
    PduIdType      pduId;
    Std_ReturnType ret;

    pduInfo.SduLength  = SECOC_MAX_RX_SECURED_MSG_LENGTH;
    pduInfo.SduDataPtr = SecOC_TempMsgBuf;
    ret                = SecOC_DataOutQueue(index, &pduId, &pduInfo);
    if (E_OK == ret)
    {
        SecOC_RxIfSecuredPduIndication(index, pduId, &pduInfo);
    }
}
#endif

SECOC_LOCAL void SecOC_RxReportVerifyStatus(uint16 index)
{
#if (SECOC_VERIFICATION_STATUS_CALLOUT_NUM > 0) || (SECOC_QUERY_FRESHNESS_VALUE == SECOC_RTE)
    const SecOC_PbRxPduProcessingType* rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    const SecOC_RxPduRunTimeType*      rxRtPtr            = SecOC_RxRtDataPtr(index);
    SecOC_VerificationStatusType       verificationStatus;
    uint16                             idx;
    boolean                            notifyFlag = FALSE;

    /* @req SWS_SecOC_00048,SWS_SecOC_00243 */
    /* Notify other modules on the verification status of the most recently
     * received Secured I-PDU */

    SchM_Enter_SecOC_RxData();
    verificationStatus.freshnessValueID   = rxPduProcessingPtr->SecOCFreshnessValueId;
    verificationStatus.verificationStatus = rxRtPtr->rxVeryfyResult;
    verificationStatus.secOCDataId        = rxPduProcessingPtr->SecOCDataId;
    SchM_Exit_SecOC_RxData();
#if (SECOC_VERIFICATION_STATUS_CALLOUT_NUM > 0)
    /* Both True and False AuthenticationStatus is propagated to SWC */
    if (SECOC_BOTH == rxPduProcessingPtr->SecOCVerificationStatusPropagationMode)
    {
        notifyFlag = TRUE;
    }
    /* Only False AuthenticationStatus is propagated to SWC */
    else if (SECOC_FAILURE_ONLY == rxPduProcessingPtr->SecOCVerificationStatusPropagationMode)
    {
        if ((SECOC_VERIFICATIONFAILURE == rxRtPtr->rxVeryfyResult)
            || (SECOC_FRESHNESSFAILURE == rxRtPtr->rxVeryfyResult)
            || (SECOC_AUTHENTICATIONBUILDFAILURE == rxRtPtr->rxVeryfyResult))
        {
            notifyFlag = TRUE;
        }
    }
    else
    {
        /* No AuthenticationStatus is propagated to SWC */
    }

    if (notifyFlag)
    {
        for (idx = SECOC_CONST_0U; idx < SECOC_VERIFICATION_STATUS_CALLOUT_NUM; idx++)
        {
            SecOC_VerificationStatusCalloutData[idx](verificationStatus);
        }
    }
#endif /* SECOC_VERIFICATION_STATUS_CALLOUT_NUM > 0 */

#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_RTE)
    boolean notifySwcFlag = FALSE;
    (void)Rte_Write_VerificationStatus_verificationStatus(&verificationStatus);
    /* Both True and False AuthenticationStatus is propagated to SWC */
    if (SECOC_BOTH == rxPduProcessingPtr->SecOCClientServerVerificationStatusPropagationMode)
    {
        notifySwcFlag = TRUE;
    }
    /* Only False AuthenticationStatus is propagated to SWC */
    else if (SECOC_FAILURE_ONLY == rxPduProcessingPtr->SecOCClientServerVerificationStatusPropagationMode)
    {
        if ((SECOC_VERIFICATIONFAILURE == rxRtPtr->rxVeryfyResult)
            || (SECOC_FRESHNESSFAILURE == rxRtPtr->rxVeryfyResult)
            || (SECOC_AUTHENTICATIONBUILDFAILURE == rxRtPtr->rxVeryfyResult))
        {
            notifySwcFlag = TRUE;
        }
    }
    else
    {
        /* No AuthenticationStatus is propagated to SWC */
    }
    if (notifySwcFlag)
    {
        (void)Rte_Call_VerificationStatusNotification_VerifyStatus(
            (const SecOC_VerificationStatusType*)&verificationStatus);
    }
#endif
#else
    (void)index;
#endif /* SECOC_VERIFICATION_STATUS_CALLOUT_NUM > 0 || SECOC_QUERY_FRESHNESS_VALUE == SECOC_RTE */
}

SECOC_LOCAL void SecOC_RxChannelReset(uint16 index)
{
    SchM_Enter_SecOC_RxData();
    const SecOC_RxPduProcessingType* rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    SecOC_RxPduRunTimeType*          rxRtPtr                = SecOC_RxRtDataPtr(index);

    /* Reset runtime data */
    rxRtPtr->rxSkipVerification    = FALSE;
    rxRtPtr->rxPduStatus           = SECOC_RX_NONE;
    rxRtPtr->curPduId              = SECOC_CONST_0U;
    rxRtPtr->curCryptoPduId        = SECOC_CONST_0U;
    rxRtPtr->rxSecuredPduLen       = SECOC_CONST_0U;
    rxRtPtr->rxAuthenticPduLen     = SECOC_CONST_0U;
    rxRtPtr->rxCryptographicPduLen = SECOC_CONST_0U;
    rxRtPtr->rxParsedAuthPduLen    = SECOC_CONST_0U;
    (void)IStdLib_MemSet(rxRtPtr->rxParsedFreshnessValue, 0u, SECOC_CONST_8U);
    (void)IStdLib_MemSet(rxRtPtr->rxParsedAuthenticator, 0u, SECOC_MAX_AUTHENTICATOR_LEN);
    rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
    rxRtPtr->rxAuthBuildAttempts   = SECOC_CONST_0U;
    rxRtPtr->rxVerifyAttempts      = SECOC_CONST_0U;
    (void)IStdLib_MemSet(rxRtPtr->rxFreshnessValue, 0u, sizeof(rxRtPtr->rxFreshnessValue));
    if ((SECOC_OVERRIDE_SKIP_UNTIL_NOTICE != rxRtPtr->rxOverrideStatus)
        && (SECOC_OVERRIDE_SKIP_UNTIL_LIMIT != rxRtPtr->rxOverrideStatus))
    {
        rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
    }
#if (SECOC_RX_TP_PDU_NUM > 0)
    rxRtPtr->tpRxDestBufPtr       = NULL_PTR;
    rxRtPtr->sduRemaining         = SECOC_CONST_0U;
    rxRtPtr->upLayerBufSize       = SECOC_CONST_0U;
    rxRtPtr->tpRxPduOffset        = SECOC_CONST_0U;
    rxRtPtr->tpRxCryptoDestBufPtr = NULL_PTR;
    rxRtPtr->sduCryptoRemaining   = SECOC_CONST_0U;
    rxRtPtr->tpRxCryptoPduOffset  = SECOC_CONST_0U;
#endif

    /* Unlock Samebuffer */
    if (NULL_PTR != rxPduProcessingDataPtr->SecOCSameBufferPduRef)
    {
        rxPduProcessingDataPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse[0] = FALSE;
    }

    /* Clear rx event */
    SECOC_CLR_RX_EVENT(index, SECOC_EVENT_RX_ALL);
    SchM_Exit_SecOC_RxData();
}

SECOC_LOCAL PduLengthType SecOC_RxGetPduLength(uint8 headerLength, const uint8* headerPtr)
{
    uint32 pduLength = SECOC_CONST_0U;
    uint8* ptr       = (uint8*)&pduLength;

    (void)IStdLib_MemCpy(&ptr[SECOC_CONST_4U - headerLength], headerPtr, headerLength);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SecOC_EndianSwap(ptr, SECOC_CONST_4U);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return ((PduLengthType)pduLength);
}
#endif /* SECOC_RX_PDU_PRO_NUM > SECOC_CONST_0U */

SECOC_LOCAL void SecOC_EndianSwap(uint8* dataPtr, uint8 size)
{
    uint8 i;
    uint8 temp;
    uint8 halfSize = size / SECOC_CONST_2U;
    uint8 tempSize = size - SECOC_CONST_1U;
    uint8 tempIndex;

    for (i = SECOC_CONST_0U; i < halfSize; i++)
    {
        tempIndex          = tempSize - i;
        temp               = dataPtr[tempIndex];
        dataPtr[tempIndex] = dataPtr[i];
        dataPtr[i]         = temp;
    }
}

SECOC_LOCAL void SecOC_ArrayShiftLeft(uint8* numArr, uint16 numLen, uint16 shiftBits)
{
    uint16 leftNumByte   = SECOC_BIT_TO_BYTE(((numLen * SECOC_CONST_8U) - shiftBits));
    uint16 shiftPerByte  = SECOC_MOD_FROM_EIGHT(shiftBits);
    uint16 firstOpNumIdx = numLen - leftNumByte;
    uint16 i;

    /* Array Shift Left */
    for (i = SECOC_CONST_0U; i < leftNumByte; i++)
    {
        if ((firstOpNumIdx + i + 1u) < numLen)
        {
            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
            numArr[i] = (numArr[firstOpNumIdx + i] << shiftPerByte)
                        | (numArr[firstOpNumIdx + i + 1u] >> (SECOC_CONST_8U - shiftPerByte));
            /* PRQA S 4397 -- */
        }
        else
        {
            numArr[i] = (numArr[firstOpNumIdx + i] << shiftPerByte);
        }
    }
    /* Clear tail unused bytes */
    for (i = leftNumByte; i < numLen; i++)
    {
        numArr[i] = SECOC_CONST_0U;
    }
}

SECOC_LOCAL void SecOC_ArrayShiftRight(uint8* numArr, uint16 numLen, uint16 shiftBits)
{
    uint16 leftNumByte   = SECOC_BIT_TO_BYTE(((numLen * SECOC_CONST_8U) - shiftBits));
    uint16 shiftPerByte  = SECOC_MOD_FROM_EIGHT(shiftBits);
    uint16 firstOpNumIdx = leftNumByte - 1u;
    uint16 i;

    /* Array Shift Right */
    for (i = SECOC_CONST_0U; i < leftNumByte; i++)
    {
        if ((firstOpNumIdx - i) >= (uint16)1u)
        {
            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
            numArr[numLen - 1u - i] = (numArr[firstOpNumIdx - i] >> shiftPerByte)
                                      | (numArr[firstOpNumIdx - i - 1u] << (SECOC_CONST_8U - shiftPerByte));
            /* PRQA S 4397 -- */
        }
        else
        {
            numArr[numLen - 1u - i] = numArr[firstOpNumIdx - i] >> shiftPerByte;
        }
    }
    /* Clear head unused bytes */
    for (i = SECOC_CONST_0U; i < (numLen - leftNumByte); i++)
    {
        numArr[i] = SECOC_CONST_0U;
    }
}

SECOC_LOCAL void SecOC_ArrayOrOp(uint8* numArr1, const uint8* numArr2, uint16 numLen)
{
    /* Note: The length of numArr1 must equal to numArr2 */
    uint16 i;

    for (i = SECOC_CONST_0U; i < numLen; i++)
    {
        numArr1[i] = numArr1[i] | numArr2[i];
    }
}

SECOC_LOCAL void SecOC_ClearLowBit(uint8* numArr, uint16 numLen, uint16 shiftBits)
{
    uint16 leftNumByte = SECOC_BIT_TO_BYTE(((numLen * SECOC_CONST_8U) - shiftBits));
    /* Clear head unused bytes */
    if (SECOC_CONST_0U != SECOC_MOD_FROM_EIGHT(shiftBits))
    {
        /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
        numArr[leftNumByte - 1u] &= (~((1u << shiftBits) - 1u));
        /* PRQA S 4397 -- */
    }
}

SECOC_LOCAL void SecOC_DataInQueue(uint16 index, PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    const SecOC_RxPduProcessingType* rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    SecOC_RxPduRunTimeType*          rxRtPtr                = SecOC_RxRtDataPtr(index);
    SecOC_QueueManageType*           queueManagePtr         = &rxRtPtr->queueManage;
    uint8*                           rxQueueBufPtr          = rxPduProcessingDataPtr->SecOCReceptionQueueRef;
    SecOC_QueueInfoType*             queueInfoPtr           = rxPduProcessingDataPtr->SecOCReceptionQueueInfoRef;
    PduLengthType                    needBufSize;
    uint16                           queueSize = rxPduProcessingDataPtr->SecOCReceptionPduQueueSize;
    uint16                           offset    = queueManagePtr->freeQueStartIdx;

    /* @req SWS_SecOC_00216 */
    needBufSize = PduInfoPtr->SduLength;
    SchM_Enter_SecOC_RxData();
    if ((queueManagePtr->freeQueSize >= needBufSize)
        && (queueManagePtr->queueFrameCnt < rxPduProcessingDataPtr->SecOCReceptionQueueSize))
    {
        /* Build queue information */
        /* Save queue information to ReceptionQueueBuffer */
        queueInfoPtr[queueManagePtr->queueTail].pduId     = RxPduId;
        queueInfoPtr[queueManagePtr->queueTail].pduLength = PduInfoPtr->SduLength;

        /* Save received Secured I-PDU to ReceptionQueueBuffer */
        if ((queueSize - offset) >= (uint16)PduInfoPtr->SduLength)
        {
            (void)IStdLib_MemCpy(&rxQueueBufPtr[offset], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
            offset += (uint16)PduInfoPtr->SduLength;
        }

        /* Update queue manage info */
        queueManagePtr->queueFrameCnt++;
        /* PRQA S 2834 ++ */ /* VL_SecOC_2834 */
        queueManagePtr->queueTail = (queueManagePtr->queueTail + 1u) % rxPduProcessingDataPtr->SecOCReceptionQueueSize;
        /* PRQA S 2834 -- */
        queueManagePtr->freeQueStartIdx = offset;
        queueManagePtr->freeQueSize -= needBufSize;
    }
    else
    {
        /* Free buffer not enough,rejecte the Secured I-PDU */
    }
    SchM_Exit_SecOC_RxData();
}

SECOC_LOCAL Std_ReturnType SecOC_DataOutQueue(uint16 index, PduIdType* RxPduIdPtr, PduInfoType* PduInfoPtr)
{
    const SecOC_RxPduProcessingType* rxPduProcessingDataPtr = SecOC_GetRxPduProcessingDataPtr(index);
    SecOC_RxPduRunTimeType*          rxRtPtr                = SecOC_RxRtDataPtr(index);
    SecOC_QueueManageType*           queueManagePtr         = &rxRtPtr->queueManage;
    const uint8*                     rxQueueBufPtr          = rxPduProcessingDataPtr->SecOCReceptionQueueRef;
    const SecOC_QueueInfoType*       queueInfoPtr           = rxPduProcessingDataPtr->SecOCReceptionQueueInfoRef;
    uint16                           queueSize              = rxPduProcessingDataPtr->SecOCReceptionPduQueueSize;
    uint16                           offset                 = queueManagePtr->queueInfoIdx;
    PduLengthType                    lPduLength;
    Std_ReturnType                   ret = E_NOT_OK;

    SchM_Enter_SecOC_RxData();
    if (queueManagePtr->queueFrameCnt > SECOC_CONST_0U)
    {
        *RxPduIdPtr = queueInfoPtr[queueManagePtr->queueHead].pduId;
        lPduLength  = queueInfoPtr[queueManagePtr->queueHead].pduLength;

        if (PduInfoPtr->SduLength >= lPduLength)
        {
            /* Output PduLength and Secured I-PDU to PduInfoPtr */
            PduInfoPtr->SduLength = lPduLength;
            if ((queueSize - offset) >= (uint16)PduInfoPtr->SduLength)
            {
                (void)IStdLib_MemCpy(PduInfoPtr->SduDataPtr, &rxQueueBufPtr[offset], PduInfoPtr->SduLength);
                offset += (uint16)PduInfoPtr->SduLength;
            }

            /* Update queue manage info */
            /* PRQA S 2834 ++ */ /* VL_SecOC_2834 */
            queueManagePtr->queueHead =
                (queueManagePtr->queueHead + 1u) % rxPduProcessingDataPtr->SecOCReceptionQueueSize;
            /* PRQA S 2834 -- */
            queueManagePtr->queueFrameCnt--;
            queueManagePtr->freeQueStartIdx = offset;
            queueManagePtr->freeQueSize += lPduLength;
            queueManagePtr->queueInfoIdx = offset;

            ret = E_OK;
        }
        else
        {
            /* Buffer not enough, return with E_NOT_OK */
            /* Return to E_NOT_OK; */
        }
    }
    SchM_Exit_SecOC_RxData();

    return ret;
}

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
SECOC_LOCAL_INLINE void SECOC_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SECOC_MODULE_ID, SECOC_INSTANCE_ID, (ApiId), (ErrorId));
}
SECOC_LOCAL_INLINE void SECOC_DET_REPORT_RUNTIMEERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(SECOC_MODULE_ID, SECOC_INSTANCE_ID, (ApiId), (ErrorId));
}

#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */

SECOC_LOCAL SecOC_StateType SecOC_isInitState(ApplicationType appId)
{
    SecOC_StateType        initStatus = SECOC_UNINIT;
    const SecOC_StateType* status;
    status = SecOC_GetInitStatusPtr(appId);
    if (NULL_PTR != status)
    {
        initStatus = *status;
    }
    return initStatus;
}

SECOC_LOCAL void SecOC_CopyBitsFromSourceToDestination(const uint8* src, uint8* dst, uint16 srcIndex, uint16 bitlength)
{
    uint16 startByteIndex      = SECOC_DIV_SHIFT(srcIndex, SECOC_CONST_3U);
    uint8  startBitIndex       = SECOC_MOD_FROM_EIGHT(srcIndex);
    uint16 copyBytes           = SECOC_DIV_SHIFT(bitlength, SECOC_CONST_3U);
    uint8  copyBits            = SECOC_MOD_FROM_EIGHT(bitlength);
    uint8  startRemainBitIndex = SECOC_CONST_8U - startBitIndex;
    uint8  maskReminbits;
    uint8  tempMaskHighBits;
    uint16 tempindex;
    /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
    uint8 mask = (1u << copyBits) - 1u;
    /* PRQA S 4397 -- */

    if (startRemainBitIndex > copyBits)
    {
        maskReminbits    = startRemainBitIndex - copyBits;
        tempMaskHighBits = SECOC_CONST_8U - maskReminbits;
        if (copyBits != 0u)
        {
            dst[0u] = (src[startByteIndex] >> (maskReminbits)) & mask;
        }

        for (uint16 i = 0u; i < copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            if (copyBits != 0u)
            {
                /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
                dst[i + 1u] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
                /* PRQA S 4397 -- */
            }
            else
            {
                if (tempMaskHighBits == 0u)
                {
                    dst[i] = src[tempindex];
                }
                else
                {
                    /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
                    dst[i] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
                    /* PRQA S 4397 -- */
                }
            }
        }
    }
    else
    {
        maskReminbits    = copyBits - startRemainBitIndex;
        tempMaskHighBits = SECOC_CONST_8U - maskReminbits;
        /* Process all data less than one byte */
        /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
        dst[0u] = (src[startByteIndex] << (maskReminbits)) & mask;
        /* PRQA S 4397 -- */
        if (maskReminbits > 0u)
        {
            /* PRQA S 4397, 2985 ++ */ /* VL_SecOC_4397, VL_SecOC_2985 */
            dst[0u] |= ((src[startByteIndex + 1u]) >> tempMaskHighBits) & ((1u << maskReminbits) - 1u);
            /* PRQA S 4397, 2985 -- */
        }

        for (uint16 i = 1u; i <= copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            /* PRQA S 4397 ++ */ /* VL_SecOC_4397 */
            dst[i] = (src[tempindex] << maskReminbits) | (src[tempindex + 1u] >> tempMaskHighBits);
            /* PRQA S 4397 -- */
        }
    }
}

#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"
