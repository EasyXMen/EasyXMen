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
**  FILENAME    : IpduM.c                                                    **
**                                                                           **
**  Created on  :                                                            **
**  Author      : darren.zhang                                               **
**  Vendor      :                                                            **
**  DESCRIPTION : IDPUM                                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.1    [20231008]  [fupeng.yu]     In IpduM_TransmitContainer, In case the PduR_IpduMTransmit() return E_NOT_OK,
 *                                        reset containedCnt of the container, Resolve the containedCnt of container
 *                                        increase when IpduM_Transmit() is called again.
 *  V2.0.2    [20231207]  [shengnan.sun]  Handle frame loss.
 *  V2.0.3    [20240229]  [shengnan.sun]  QAC_PH_2024 check.
 *  V2.0.4    [20240309]  [shengnan.sun]  Support multiple partition feature.
 *
 **********************************************************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:IpduM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 2877 MISRA Rule 4.1 .<br>
    Reason:for loop condition is a macro definition, which varies in different projs.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 0488 MISRA Rule 18.4 .<br>
    Reason:The offending items are array pointer operations,no effect and reserved
 */

/******************************************************************************
**                      Includes                                             **
******************************************************************************/
#include "IpduM_Cbk.h"
#if (STD_ON == IPDUM_RX_DIRECT_COM_INVOCATION)
#include "Com_Cbk.h"
#endif
#if (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
#include "Os.h"
#endif
#include "Det.h"
/******************************************************************************
**                      Imported Compiler Switch Check                       **
******************************************************************************/
#define IPDUM_C_AR_MAJOR_VERSION 4u
#define IPDUM_C_AR_MINOR_VERSION 5u
#define IPDUM_C_AR_PATCH_VERSION 0u
#define IPDUM_C_SW_MAJOR_VERSION 2u
#define IPDUM_C_SW_MINOR_VERSION 0u
#define IPDUM_C_SW_PATCH_VERSION 3u
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                                                                                \
    (IPDUM_C_AR_MAJOR_VERSION != IPDUM_H_AR_MAJOR_VERSION) || (IPDUM_C_AR_MINOR_VERSION != IPDUM_H_AR_MINOR_VERSION) \
    || (IPDUM_C_AR_PATCH_VERSION != IPDUM_H_AR_PATCH_VERSION))
#error " IpduM.c version mismatching with IpduM.h"
#endif
/******************************************************************************
**                      Private Macro Definitions                            **
******************************************************************************/
/* call api shall be typedef */
#if (STD_ON == IPDUM_RX_DIRECT_COM_INVOCATION)
/* Ipdum received I-PDU Multiplexing is rx and Tx-confirmation shall direct notify Com */
#define IPDUM_NotifyRxIndication   Com_RxIndication
#define IPDUM_NotifyTxConfirmation Com_TxConfirmation
#else
/* Ipdum received I-PDU Multiplexing is rx and Tx-confirmation need
notify PDUR router to upper layer */
#define IPDUM_NotifyRxIndication   PduR_IpduMRxIndication
#define IPDUM_NotifyTxConfirmation PduR_IpduMTxConfirmation
#endif /* STD_ON == IPDUM_RX_DIRECT_COM_INVOCATION */

/* clear section bit */
#define ClearBitInSection(data, startBit, endBit) \
    (data) = (data) & (uint8)(~((uint8)(0xffu << (startBit)) >> (7u - (endBit))))
/* hold section bit */
#define HoldBitInSection(data, startBit, endBit) \
    (data) = (data) & (uint8)(((uint8)(0xffu << (startBit))) >> (7u - (endBit)))
/* extract section bit */
#define ExtractBitInSection(data, startBit, endBit) \
    (data) = ((uint8)((data) << (7u - (endBit))) >> (7u - (endBit))) >> (startBit)
/******************************************************************************
**                      Private Type Definitions                             *
******************************************************************************/
typedef uint32 IpduM_uintx;
/******************************************************************************
**                      Private Constant Definitions                         **
******************************************************************************/

/******************************************************************************
**                      Private Variable Definitions                         **
******************************************************************************/
/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define IPDUM_START_SEC_VAR_INIT_BOOLEAN
#include "IpduM_MemMap.h"
static VAR(IpduM_StatusType, IPDUM_VAR_POWER_ON_INIT) IpduM_InitStauts = IPDUM_UNINIT;
#define IPDUM_STOP_SEC_VAR_INIT_BOOLEAN
#include "IpduM_MemMap.h"

/*PB configuration pointer*/
#define IPDUM_START_SEC_VAR_INIT_PTR
#include "IpduM_MemMap.h"
static P2CONST(IpduM_ConfigType, IPDUM_VAR, IPDUM_CONST) IpduM_CfgPtr = NULL_PTR;
#define IPDUM_STOP_SEC_VAR_INIT_PTR
#include "IpduM_MemMap.h"

#if (IPDUM_MAX_BUFFER_SIZE > 0u)
/*common buffer space, @ECUC_IpduM_00166*/
#define IPDUM_START_SEC_VAR_NO_INIT_8
#include "IpduM_MemMap.h"
static VAR(uint8, IPDUM_VAR_NOINIT) IpduM_DtaBuf[IPDUM_MAX_BUFFER_SIZE];
#define IPDUM_STOP_SEC_VAR_NO_INIT_8
#include "IpduM_MemMap.h"
#endif /* IPDUM_MAX_BUFFER_SIZE > 0u */

#if (IPDUM_MAX_PDU_BUF_SIZE > 0u)
/* simplex I-PDU buffer */
#define IPDUM_START_SEC_VAR_NO_INIT_8
#include "IpduM_MemMap.h"
static VAR(uint8, IPDUM_VAR_NOINIT) IpduM_PduBuf[IPDUM_MAX_PDU_BUF_SIZE];
#define IPDUM_STOP_SEC_VAR_NO_INIT_8
#include "IpduM_MemMap.h"
#endif /* IPDUM_MAX_PDU_BUF_SIZE > 0u */

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/*inner tx request runtime Variable */
#define IPDUM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
static VAR(IpduM_InnerTxRequtstType, IPDUM_VAR_NOINIT) IpduM_InnerTxReq[IPDUM_TX_REQUEST_NUMBER_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/*inner container transmit runtime Variable */
#define IPDUM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
static VAR(IpduM_InnerContainerTxType, IPDUM_VAR_NOINIT) IpduM_InnerContainerTx[IPDUM_CONTAINER_TX_NUMBER_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/*inner container rx runtime Variable */
#define IPDUM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
static VAR(IpduM_InnerContainerRxType, IPDUM_VAR_NOINIT) IpduM_InnerContainerRx[IPDUM_CONTAINER_RX_NUMBER_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */

#if (IPDUM_SUPPORT_TX_CONTAINED_NUM_MAX > 0u)
/* contained index buf by container queue */
#define IPDUM_START_SEC_VAR_NO_INIT_16
#include "IpduM_MemMap.h"
static VAR(uint16, IPDUM_VAR_NOINIT) IpduM_InnerContainedIndexRecode[IPDUM_SUPPORT_TX_CONTAINED_NUM_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_16
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_VAR_NO_INIT_16
#include "IpduM_MemMap.h"
static VAR(uint16, IPDUM_VAR_NOINIT) IpduM_ContainedPendingTxConfirmation[IPDUM_SUPPORT_TX_CONTAINED_NUM_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_16
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_MAX_NUM > 0u */

#if (IPDUM_CONTAINER_TX_QUEUE_NUM_MAX > 0u)
/* container queue handle control*/
#define IPDUM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
static VAR(IpduM_InnerContainerTxQueueHandleType, IPDUM_VAR_NOINIT)
    IpduM_InnerContainerTxQueueRecode[IPDUM_CONTAINER_TX_QUEUE_NUM_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_TX_QUEUE_MAX_SIZE > 0u */

#if (IPDUM_CONTAINER_RX_QUEUE_NUM_MAX > 0u)
/* container queue handle control*/
#define IPDUM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
static VAR(IpduM_InnerContainerRxQueueHandleType, IPDUM_VAR_NOINIT)
    IpduM_InnerContainerRxQueueRecode[IPDUM_CONTAINER_RX_QUEUE_NUM_MAX];
#define IPDUM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_RX_QUEUE_MAX_SIZE > 0u */
/******************************************************************************
**                      Private Function Definitions                         **
******************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* tx request inner Function */
/* ipdum receiver upper layer Multiplexer i-PDU need package
relevant tx request */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TransmitMultiplexLoadData(
    P2CONST(IpduM_UpTxPduType, AUTOMATIC, IPDUM_APPL_DATA) txPathway,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);
/* Multiplexer tx request transmit */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexTxSendPdu(IpduM_uintx txReqIndex);
/* Multiplexer assemble part pdu to tx request */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexTxTrigLoadData(IpduM_uintx txReqIndex);
/* Multiplexer handle tx confirmation */
static FUNC(void, IPDUM_CODE) IpduM_TxConfOfMultiplex(IpduM_uintx txReqIndex);
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
/* Multiplexer rx indiation inner function  */
/* Multiplexer rx indiation handle */
static FUNC(void, IPDUM_CODE)
    IpduM_RxMultiplexHandle(IpduM_uintx rxIndIndex, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);
/* Multiplexer RX indiation pdu find select value */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexFindSelectVal(
    IpduM_ByteOrderType byteOrder,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr,
    P2CONST(IpduMSelectorFieldType, AUTOMATIC, IPDUM_APPL_DATA) selectFieldPtr,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, IPDUM_APPL_DATA) selcetValPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif               /* IPDUM_RX_INDICA_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* tx container inner function */
/* transmit container i-pdu */
static FUNC(Std_ReturnType, IPDUM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpduM_TransmitContainer(IpduM_uintx containerIndex, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* this function handle in api IpduM_Transmit,shall handle contained for
upper layer data */
static FUNC(Std_ReturnType, IPDUM_CODE)
    IpduM_TransmitContained(IpduM_uintx containedIndex, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

/* container tx confirmation handle */
static FUNC(void, IPDUM_CODE) IpduM_TxConfOfContainer(IpduM_uintx containerIndex);

/* container tx i-pdu handle */
static FUNC(void, IPDUM_CODE) IpduM_ContainerTxHandle(IpduM_uintx index);

/* contained in container queue need be calculate position in buffer */
static FUNC(Std_ReturnType, IPDUM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpduM_ContainedInBufPosCalc(IpduM_uintx containedIndex, P2VAR(IpduM_uintx, AUTOMATIC, IPDUM_APPL_DATA) posInBufPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/* contained is LAST_IS_BEST shall find position in container queue buffer */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainedFindLastIsBestInBufPos(
    IpduM_uintx containedIndex,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(IpduM_uintx, AUTOMATIC, IPDUM_APPL_DATA) bufIndexPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/* LAST_IS_BEST contained need get pdu date by triggertransmit handle */
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainedTxTrigHandle(IpduM_uintx containerIndex);

/* delete contained in container queue buf */
static FUNC(uint16, IPDUM_CODE) IpduM_ContainedDeleteInBuf(
    IpduM_uintx containerIndex,
    P2CONST(IpduMContainedTxPduType, AUTOMATIC, IPDUM_APPL_DATA) containedTxPtr,
    IpduM_uintx bufIndex,
    uint16 txQueueLen);
/* fill contained data to container buf */
static FUNC(void, IPDUM_CODE) IpduM_ContainedFillToContainer(
    P2CONST(IpduMContainedTxPduType, AUTOMATIC, IPDUM_APPL_DATA) containedTxPtr,
    IpduM_uintx bufIndex,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/* free container queue ctrl valu */
static FUNC(boolean, IPDUM_CODE) IpduM_ContainerCheckQueueIsExit(IpduM_uintx containerIndex);
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/* in rx indiation handle container */
static FUNC(void, IPDUM_CODE) IpduM_ContainerRxIndication(
    IpduM_uintx containerIndex,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/* container rx i-pdu handle */
static FUNC(void, IPDUM_CODE) IpduM_ContainerRxHandle(IpduM_uintx containerIndex);

/* extracted contained in container rx pdu*/
static FUNC(void, IPDUM_CODE) IpduM_ContainedRxExtractedHandle(
    IpduM_uintx containerIndex,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dtaPtr,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    IpduM_uintx lstlen);
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */

#if ((IPDUM_RX_INDICA_NUMBER_MAX > 0u) || (IPDUM_TX_REQUEST_NUMBER_MAX > 0u))
/* Multiplexer pdu data info handle by segment */
static FUNC(Std_ReturnType, IPDUM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpudM_SegmentCopy(P2CONST(IpduM_PduTranslateType, AUTOMATIC, IPDUM_APPL_DATA) pduTransPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
/******************************************************************************
**                      Global Constant Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Variable Definitions                          **
******************************************************************************/

/******************************************************************************
**                      Global Function Definitions                          **
******************************************************************************/
/**
 * Initializes the I-PDU Multiplexer.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): config, Implementation specific
    structure with configuration parameters.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 * @SWS_IpduM_00033,@SWS_IpduM_00084
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_CODE)
IpduM_Init(P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_DATA) configPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    IpduM_uintx index;
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
    const IpduMContainerTxPduType* containerTxCfgPtr;
    IpduM_InnerContainerTxType* inContainerTxPtr;
    IpduM_uintx loop;
#endif
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    boolean errFlg = FALSE;
#if (STD_ON == IPDUM_REINIT_CHECK)
    if (IPDUM_UNINIT != IpduM_InitStauts)
    {
        errFlg = TRUE;
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_INIT, IPDUM_E_INIT_FAILED);
    }
#endif
    if (NULL_PTR == configPtr)
    {
        errFlg = TRUE;
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_INIT, IPDUM_E_PARAM_POINTER);
    }
    if (FALSE == errFlg)
#endif /*STD_ON == IPDUM_DEV_ERROR_DETECT*/
    {
        /* @SWS_IpduM_00033
        initalize all Global Variable */
        IpduM_CfgPtr = configPtr;
#if (IPDUM_MAX_BUFFER_SIZE > 0u)
        /* @SWS_IpduM_00067,@SWS_IpduM_00068,@SWS_IpduM_00143
        all init value of reference post-build default data,
        this action shall be completed by config tool */
        /* copy post build defualt value to inner buf */
        for (index = 0u; index < IPDUM_MAX_BUFFER_SIZE; index++)
        {
            IpduM_DtaBuf[index] = IpduMDefaultBufVal[index];
        }
#endif /* IPDUM_MAX_BUFFER_SIZE > 0u */
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
        for (index = 0u; index < IPDUM_TX_REQUEST_NUMBER_MAX; index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
        {
            /* @SWS_IpduM_00223
            dynamic part reference by this if nodynamic part
            was sent before */
            IpduM_InnerTxReq[index].lastTxDynamicPartIndex =
                IpduM_CfgPtr->IpduMTxRequestPtr[index].IpduMInitialDynamicPart;
            IpduM_InnerTxReq[index].txConfTimer = 0x0u;
            IpduM_InnerTxReq[index].isTrigFlg = FALSE;
        }
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
        for (index = 0u; index < IPDUM_CONTAINER_TX_NUMBER_MAX; index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
        {
            containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[index];
            inContainerTxPtr = &IpduM_InnerContainerTx[index];
            inContainerTxPtr->transTimer = 0x0u;
            inContainerTxPtr->txConfTimer = 0x0u;
            inContainerTxPtr->firstAddFlg = FALSE;
            inContainerTxPtr->transReadPoint = 0x0u;
            inContainerTxPtr->txCofReadPoint = 0x0u;
            inContainerTxPtr->writePoint = 0x0u;
            inContainerTxPtr->queueCnt = 0x0u;
            for (loop = containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
                 loop
                 < ((IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex
                    + (IpduM_uintx)containerTxCfgPtr->IpduMContainerQueueSize);
                 loop++)
            {
                IpduM_InnerContainerTxQueueRecode[loop].payloadSize = 0x0u;
                IpduM_InnerContainerTxQueueRecode[loop].containedStartPos =
                    containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
                IpduM_InnerContainerTxQueueRecode[loop].containedCnt = 0x0u;
                IpduM_InnerContainerTxQueueRecode[loop].containedTxConfirmStartPos =
                    containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
                IpduM_InnerContainerTxQueueRecode[loop].containedTxConfirmCnt = 0x0u;
            }
        }
        /* init contained index arry */
        for (loop = 0x0u; loop < IPDUM_SUPPORT_TX_CONTAINED_NUM_MAX; loop++)
        {
            IpduM_InnerContainedIndexRecode[loop] = IPDUM_UNUSED_UINT8;
            IpduM_ContainedPendingTxConfirmation[loop] = IPDUM_UNUSED_UINT8;
        }
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX > 0u */
#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
        for (index = 0u; index < IPDUM_CONTAINER_RX_NUMBER_MAX; index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
        {
            IpduM_InnerContainerRx[index].queueCnt = 0x0u;
            IpduM_InnerContainerRx[index].readPoint = 0x0u;
            IpduM_InnerContainerRx[index].writePoint = 0x0u;
        }
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */
        IpduM_InitStauts = IPDUM_INIT;
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#if (STD_ON == IPDUM_VERSION_INFO_API)
/**
 * Service returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): versioninfo, Pointer to where to store the version
    information of this module.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_CODE)
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
IpduM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, IPDUM_APPL_DATA) versioninfo)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        /*report DET error*/
        (void)
            Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_GETVERSIONINFO, IPDUM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == IPDUM_DEV_ERROR_DETECT*/
    {
        versioninfo->moduleID = IPDUM_MODULE_ID;
        versioninfo->sw_major_version = IPDUM_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = IPDUM_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = IPDUM_C_SW_PATCH_VERSION;
        versioninfo->vendorID = IPDUM_VENDOR_ID;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* STD_ON == IPDUM_VERSION_INFO_API */

/**
 * Service is called by the PDU-Router to request a transmission.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same PDU-ID. Reentrant for
    different PDU-ID.
 * Parameters(IN): PdumTxPduId, ID of I-PDU to be transmitted
                    PduInfoPtr,A pointer to a structure with I-PDU related
    data that shall be transmitted: data length and pointer to I-SDU buffer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType  E_OK: Transmit request is accepted
        E_NOT_OK: Transmit request is not accepted
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
FUNC(Std_ReturnType, IPDUM_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
IpduM_Transmit(PduIdType PdumTxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (IPDUM_INIT != IpduM_InitStauts)
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_UNINIT);
    }
    else if ((IPDUM_TX_UP_PDU_MAX <= PdumTxPduId) || (NULL_PTR == IpduM_CfgPtr->IpduMUpTxPduPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
    {
        if (
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
            (IPDUM_TRANSMIT_TYPE_STATICPART == IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].TxReqType) ||
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
            (IPDUM_TRANSMIT_TPYE_DYNAMIC == IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].TxReqType))
        {
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_TX_REQUEST_NUMBER_MAX <= IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMTxRequestPtr))
            {
                (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                ret = IpduM_TransmitMultiplexLoadData(&IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId], PduInfoPtr);
            }
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */
        }
        else if (IPDUM_TRANSMIT_TYPE_CONTAINED == IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].TxReqType)
        {
#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_CONTAINED_TX_NUMBER_MAX <= IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMContainedTxPduPtr))
            {
                (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                ret = IpduM_TransmitContained(IpduM_CfgPtr->IpduMUpTxPduPtr[PdumTxPduId].Index, PduInfoPtr);
            }
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX > 0u */
        }
        else
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/*Call-back notifications*/

/**
 * Indication of a received I-PDU from a lower layer communication
    interface module.
 * Service ID: 0x42
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): RxPduId, ID of the received I-PDU.
                    PduInfoPtr, Contains the length of the received I-PDU and a
        pointer to a buffer (SduDataPtr) containing the I-PDU.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_00041,@SWS_IpduM_00042,@SWS_IpduM_00217,@SWS_IpduM_00086
 */
#define IPDUM_START_SEC_IPDUMRXINDICATION_CALLBACK_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_RXINDICATION_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
IpduM_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (IPDUM_INIT != IpduM_InitStauts)
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_UNINIT);
    }
    else if ((IPDUM_RX_PDU_MAX <= RxPduId) || (NULL_PTR == IpduM_CfgPtr->IpduMRxPduPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
    {
        if (IPDUM_RXIND_TYPE_MULT == IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].RxIndType)
        {
#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_RX_INDICA_NUMBER_MAX <= IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMRxIndicationPtr))
            {
                (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                IpduM_RxMultiplexHandle(IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].Index, PduInfoPtr);
            }
#endif /* IPDUM_RX_INDICA_NUMBER_MAX > 0u */
        }
        else if (IPDUM_RXIND_TYPE_CONTAINER == IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].RxIndType)
        {
#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_CONTAINER_RX_NUMBER_MAX <= IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMContainerRxPduPtr))
            {
                (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                IpduM_ContainerRxIndication(IpduM_CfgPtr->IpduMRxPduPtr[RxPduId].Index, PduInfoPtr);
            }
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */
        }
        else
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM);
        }
    }
    return;
}
#define IPDUM_STOP_SEC_IPDUMRXINDICATION_CALLBACK_CODE
#include "IpduM_MemMap.h"

/**
 * The lower layer communication interface module confirms the transmission of
    an I-PDU.
 * Service ID: 0x40
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId, ID of the I-PDU that has been transmitted.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_00087,@SWS_IpduM_00088
 */
#define IPDUM_START_SEC_IPDUMTXCONFIRMATION_CALLBACK_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_TXCONFIRMATION_CODE)
IpduM_TxConfirmation(PduIdType TxPduId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (IPDUM_INIT != IpduM_InitStauts)
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_UNINIT);
    }
    else if ((IPDUM_TX_PDU_NUMBER_MAX <= TxPduId) || (NULL_PTR == IpduM_CfgPtr->IpduMTxPduPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_PARAM);
    }
    else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
    {
        if (IPDUM_TXCONF_TYPE_MULT == IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].txConfPduType)
        {
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_TX_REQUEST_NUMBER_MAX <= IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMTxRequestPtr))
            {
                (void)
                    Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT  */
            {
                IpduM_TxConfOfMultiplex(IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index);
            }
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */
        }
        else if (IPDUM_TXCONF_TYPE_CONTAINER == IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].txConfPduType)
        {
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_CONTAINER_TX_NUMBER_MAX <= IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMContainerTxPduPtr))
            {
                (void)
                    Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                IpduM_TxConfOfContainer(IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index);
            }
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX >0u */
        }
        else
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_PARAM);
        }
    }
    return;
}
#define IPDUM_STOP_SEC_IPDUMTXCONFIRMATION_CALLBACK_CODE
#include "IpduM_MemMap.h"

/**
 * IpduM_TriggerTransmit
 * Service ID: 0x41
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
 * Parameters(IN): TxPduId, ID of the received I-PDU.
 * Parameters(INOUT):  PduInfoPtr, Contains the length of the
        transmit I-PDU and a pointer to a buffer containing the I-PDU.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType E_OK: SDU data has been copied
        E_NOT_OK: No SDU data has been copied.
 *  @SWS_IpduM_00089,@SWS_IpduM_00090,@SWS_IpduM_00091
 */
#define IPDUM_START_SEC_IPDUMTRIGGERTRANSMIT_CALLBACK_CODE
#include "IpduM_MemMap.h"
FUNC(Std_ReturnType, IPDUM_TRIGGERTRANSMIT_CODE)
/* PRQA S 3432,1532 ++ */ /* MISRA Rule 20.7,8.7 */
IpduM_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
/* PRQA S 3432,1532 -- */ /* MISRA Rule 20.7,8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (IPDUM_INIT != IpduM_InitStauts)
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_UNINIT);
    }
    else if ((IPDUM_TX_PDU_NUMBER_MAX <= TxPduId) || (NULL_PTR == IpduM_CfgPtr->IpduMTxPduPtr))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        (void)Det_ReportError(
            IPDUM_MODULE_ID,
            IPDUM_INSTANCE_ID,
            IPDUM_SERVICE_ID_TRIGGERTRANSMIT,
            IPDUM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
    {
        if (IPDUM_TXCONF_TYPE_MULT == IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].txConfPduType)
        {
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_TX_REQUEST_NUMBER_MAX <= IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMTxRequestPtr))
            {
                (void)Det_ReportError(
                    IPDUM_MODULE_ID,
                    IPDUM_INSTANCE_ID,
                    IPDUM_SERVICE_ID_TRIGGERTRANSMIT,
                    IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {

                PduLengthType cnt;
                IpduM_uintx txRequestIndex = (IpduM_uintx)IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index;
                const IpduMTxRequestType* txRequstPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txRequestIndex];
                if ((PduInfoPtr->SduLength >= txRequstPtr->IpduMOutgoingPduRef->PduLen)
#if (STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF)
                    && (0u == IpduM_InnerTxReq[txRequestIndex].txConfTimer)
#endif /* STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF */
                )
                {
                    ret = IpduM_MultiplexTxTrigLoadData(txRequestIndex);
                }
                if (E_OK == ret)
                {
                    SchM_Enter_IpduM_Context();
                    PduInfoPtr->SduLength = txRequstPtr->IpduMOutgoingPduRef->PduLen;
                    for (cnt = 0u; cnt < PduInfoPtr->SduLength; cnt++)
                    {
                        PduInfoPtr->SduDataPtr[cnt] = IpduM_DtaBuf[(txRequstPtr->IpduMBufIndex) + cnt];
                    }
#if (STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF)
                    IpduM_InnerTxReq[txRequestIndex].txConfTimer = txRequstPtr->IpduMTxConfirmationTimeout;
#endif /* STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF */
                    SchM_Exit_IpduM_Context();
                }
            }
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */
        }
        else if (IPDUM_TXCONF_TYPE_CONTAINER == IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].txConfPduType)
        {
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            if ((IPDUM_CONTAINER_TX_NUMBER_MAX <= IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index)
                || (NULL_PTR == IpduM_CfgPtr->IpduMContainerTxPduPtr))
            {
                (void)Det_ReportError(
                    IPDUM_MODULE_ID,
                    IPDUM_INSTANCE_ID,
                    IPDUM_SERVICE_ID_TRIGGERTRANSMIT,
                    IPDUM_E_PARAM);
            }
            else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
            {
                ret = IpduM_TransmitContainer(IpduM_CfgPtr->IpduMTxPduPtr[TxPduId].Index, PduInfoPtr);
            }
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX >0u */
        }
        else
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_PARAM);
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_IPDUMTRIGGERTRANSMIT_CALLBACK_CODE
#include "IpduM_MemMap.h"

/*
Scheduled functions
*/
/**
 * Performs the processes of the activities that are not directly
    initiated by the calls from PDU-R.
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different instances. Non reentrant for the same instance.
 * Parameters(IN): TNA
 * Parameters(INOUT):  NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_91002
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_CODE)
IpduM_MainFunctionTx(IpduM_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if ((IPDUM_INIT == IpduM_InitStauts) && (mainFunctionId < IPDUM_MAINFUNCTION_TX_NUM_MAX))
    {
#if (STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != IpduM_CfgPtr->IpduMMainFuncTxPartitionRange[mainFunctionId])
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_MAINFUNCTIONTX, IPDUM_E_PARAM);
        }
        else
#endif
        {
#if ((IPDUM_TX_REQUEST_NUMBER_MAX > 0u) || (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u))
            IpduM_uintx index;
#endif
            if (IPDUM_INIT == IpduM_InitStauts)
            {
/* Multiplexer tx transmit handle */
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
#if (STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
                const IpduMTxRequestType* txRequstPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[0];
#endif
                for (index = 0u; (index < IPDUM_TX_REQUEST_NUMBER_MAX); index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
                {
#if (STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
                    if (applicationID != txRequstPtr[index].IpduMTxRequestPartition)
                    {
                        if (index == (IPDUM_TX_REQUEST_NUMBER_MAX - 1u))
                        {
                            (void)Det_ReportError(
                                IPDUM_MODULE_ID,
                                IPDUM_INSTANCE_ID,
                                IPDUM_SERVICE_ID_MAINFUNCTIONTX,
                                IPDUM_E_PARAM);
                        }
                        continue;
                    }
#endif
                    if (TRUE == IpduM_InnerTxReq[index].isTrigFlg)
                    {
                        if (E_OK == IpduM_MultiplexTxTrigLoadData(index))
                        {
                            if (E_OK == IpduM_MultiplexTxSendPdu(index))
                            {
                                IpduM_InnerTxReq[index].isTrigFlg = FALSE;
                            }
                        }
                        /* consider go wrong when trig get part i-pdu,shall clear trig */
                        else
                        {
                            IpduM_InnerTxReq[index].isTrigFlg = FALSE;
                        }
                    }
                    /* @SWS_IpduM_0023
                    allow new transmission for this special ipdu
                    after timeout is elapsed */
                    SchM_Enter_IpduM_Context();
                    if (0u < IpduM_InnerTxReq[index].txConfTimer)
                    {
                        IpduM_InnerTxReq[index].txConfTimer--;
                    }
                    SchM_Exit_IpduM_Context();
                }
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
                index = IpduM_GetStartOfMainFunctionTx(mainFunctionId);
                for (; index < IpduM_GetEndOfMainFunctionTx(mainFunctionId);
                     index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
                {
                    IpduM_ContainerTxHandle(index);
                }
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX > 0u */
            }
        }
        return;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/*
Scheduled functions
*/
/**
 * Performs the processes of the activities that are not directly
    initiated by the calls from PDU-R.
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different instances. Non reentrant for the same instance.
 * Parameters(IN): TNA
 * Parameters(INOUT):  NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_91001
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
FUNC(void, IPDUM_CODE)
IpduM_MainFunctionRx(IpduM_MainFunctionType mainFunctionId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    if ((IPDUM_INIT == IpduM_InitStauts) && (mainFunctionId < IPDUM_MAINFUNCTION_RX_NUM_MAX))
    {
#if (STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationID = GetApplicationID();
        if (applicationID != IpduM_CfgPtr->IpduMMainFuncRxPartitionRange[mainFunctionId])
        {
            (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_MAINFUNCTIONRX, IPDUM_E_PARAM);
        }
        else
#endif
        {
#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
            IpduM_uintx index = IpduM_GetStartOfMainFunctionRx(mainFunctionId);
            for (; index < IpduM_GetEndOfMainFunctionRx(mainFunctionId); index++) /* PRQA S 2877 */ /* MISRA Rule 4.1 */
            {
                if (0x0u < IpduM_InnerContainerRx[index].queueCnt)
                {
                    IpduM_ContainerRxHandle(index);
                }
            }
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX > 0u */
        }
        return;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0x0u)
/* ipdum receiver upper layer Multiplexer i-PDU need package
relevant tx request */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TransmitMultiplexLoadData(
    P2CONST(IpduM_UpTxPduType, AUTOMATIC, IPDUM_APPL_DATA) txPathway,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    IpduM_PduTranslateType pduTrans;
    PduInfoType outPdu;
    const IpduMTxRequestType* txReqPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txPathway->Index];
    const IpduM_TxPartType* txPartPtr = NULL_PTR;
    /* @SWS_IpduM_00020
    tx confirmation not elapsed,not allow new transmission request
    with a i-Pdu that belongs to same i-pdu*/
    /* @SWS_IpduM_00152
    the tx confirmation timer not elapsed the function IpduM_Transmit
    shall return E_NOT_OK */
    if (0u == IpduM_InnerTxReq[txPathway->Index].txConfTimer)
    {
        if ((IPDUM_TRANSMIT_TPYE_DYNAMIC == txPathway->TxReqType) && (NULL_PTR != txReqPtr->IpduMTxDynamicPartPtr))
        {
            /* @SWS_IpduM_00223
            store latest dynamic part sent by upper layer */
            IpduM_InnerTxReq[txPathway->Index].lastTxDynamicPartIndex = txPathway->handleId;
            txPartPtr = &txReqPtr->IpduMTxDynamicPartPtr[txPathway->handleId];
        }
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
        else if (
            (IPDUM_TRANSMIT_TYPE_STATICPART == txPathway->TxReqType) && (NULL_PTR != txReqPtr->IpduMTxStaticPartPtr))
        {
            txPartPtr = &txReqPtr->IpduMTxStaticPartPtr[txPathway->handleId];
        }
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
        else
        {
            ;
        }
    }
    if ((NULL_PTR != txPartPtr) && (txPartPtr->IpduMTxPduRef->PduLen == PduInfoPtr->SduLength))
    {
        /* @SWS_IpduM_00015
        merge corresponding a apart i-pdu to tx request */
        SchM_Enter_IpduM_Context();
        outPdu.SduDataPtr = &IpduM_DtaBuf[txReqPtr->IpduMBufIndex];
        outPdu.SduLength = (PduLengthType)txReqPtr->IpduMOutgoingPduRef->PduLen;
        pduTrans.byteOrd = txReqPtr->IpduMByteOrder;
        pduTrans.inputPduPtr = PduInfoPtr;
        pduTrans.outputPduPtr = &outPdu;
        SchM_Exit_IpduM_Context();
        pduTrans.segmentCnt = txPartPtr->IpduMTxSegmentCnt;
        pduTrans.segmentPtr = txPartPtr->IpduMTxSegmentPtr;
        ret = IpudM_SegmentCopy(&pduTrans);
        /* Check now part is need to trig */
        if (E_OK == ret)
        {
            /* @SWS_IpduM_00021
            send a transmission request Check this condition */
            if ((IPDUM_STATIC_OR_DYNAMIC_PART_TRIGGER == txReqPtr->IpduMTxTriggerMode) ||
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
                ((IPDUM_STATIC_PART_TRIGGER == txReqPtr->IpduMTxTriggerMode)
                 && (IPDUM_TRANSMIT_TYPE_STATICPART == txPathway->TxReqType))
                ||
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
                ((IPDUM_DYNAMIC_PART_TRIGGER == txReqPtr->IpduMTxTriggerMode)
                 && (IPDUM_TRANSMIT_TPYE_DYNAMIC == txPathway->TxReqType)))
            {
                IpduM_InnerTxReq[txPathway->Index].isTrigFlg = TRUE;
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0x0u */

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* transmit container i-pdu */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpduM_TransmitContainer(IpduM_uintx containerIndex, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    const IpduMContainerTxPduType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
    IpduM_InnerContainerTxType* innerContainerPtr = &IpduM_InnerContainerTx[containerIndex];
    PduInfoType PduInfo;
    IpduM_uintx queueIndex = (IpduM_uintx)innerContainerPtr->transReadPoint
                             + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
    IpduM_uintx bufIndex = (IpduM_uintx)containerTxCfgPtr->IpduMBufIndex
                           + ((IpduM_uintx)innerContainerPtr->transReadPoint
                              * (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen);
    IpduM_InnerContainerTxQueueHandleType* incIndexPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
    PduLengthType index;
    /* @SWS_IpduM_00192
    fill the pduInfo*/
    SchM_Enter_IpduM_Context();
    PduInfo.SduDataPtr = &IpduM_DtaBuf[bufIndex];
    PduInfo.SduLength = incIndexPtr->payloadSize;
    SchM_Exit_IpduM_Context();
    /* @SWS_IpduM_00194
    IPDUM_TRIGGERTRANSMIT copy old instance in queue,if queue is not empty,
    shall return E_NOT_OK */
    if ((0x0u == innerContainerPtr->txConfTimer) && (0u < incIndexPtr->payloadSize)
        && (0u < innerContainerPtr->queueCnt))
    {
        ret = IpduM_ContainedTxTrigHandle(containerIndex);
        if (NULL_PTR == PduInfoPtr)
        {
            PduInfo.SduLength = (PduLengthType)incIndexPtr->payloadSize;
            ret = PduR_IpduMTransmit(containerTxCfgPtr->IpduMContainerTxPduRef->PduId, &PduInfo);
            if (E_OK == ret)
            {
                /*store the TxPending Contained*/
                uint8 cnt;
                incIndexPtr->containedTxConfirmStartPos = incIndexPtr->containedStartPos;
                incIndexPtr->containedTxConfirmCnt = incIndexPtr->containedCnt;
                for (cnt = 0u; cnt < incIndexPtr->containedTxConfirmCnt; cnt++)
                {
                    IpduM_ContainedPendingTxConfirmation[(incIndexPtr->containedTxConfirmStartPos) + cnt] =
                        IpduM_InnerContainedIndexRecode[(incIndexPtr->containedStartPos) + cnt];
                }
            }
            /* reset containedCnt of the container */
            incIndexPtr->containedStartPos = containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
            incIndexPtr->containedCnt = 0x0u;
        }
        else
        {
            PduLengthType cnt;
            uint8 cnt1;
            /* point data to trigger call */
            SchM_Enter_IpduM_Context();
            PduInfoPtr->SduLength = incIndexPtr->payloadSize;
            for (cnt = 0u; cnt < PduInfoPtr->SduLength; cnt++)
            {
                PduInfoPtr->SduDataPtr[cnt] = IpduM_DtaBuf[bufIndex + cnt];
            }
            SchM_Exit_IpduM_Context();
            /*store the TxPending Contained*/
            incIndexPtr->containedTxConfirmStartPos = incIndexPtr->containedStartPos;
            incIndexPtr->containedTxConfirmCnt = incIndexPtr->containedCnt;
            for (cnt1 = 0u; cnt1 < incIndexPtr->containedTxConfirmCnt; cnt1++)
            {
                IpduM_ContainedPendingTxConfirmation[(incIndexPtr->containedTxConfirmStartPos) + cnt1] =
                    IpduM_InnerContainedIndexRecode[(incIndexPtr->containedStartPos) + cnt1];
            }
            /* reset payload size of the container */
            incIndexPtr->containedStartPos = containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
            incIndexPtr->containedCnt = 0x0u;
        }
    }
    if ((E_OK == ret) && (0u < incIndexPtr->payloadSize) && (0u < innerContainerPtr->queueCnt))
    {
        /* queue read point in transmit */
        /* reset payload size of the container */
        incIndexPtr->payloadSize = 0x0u;
        /* queue handle */
        SchM_Enter_IpduM_Context();
        if (0u < innerContainerPtr->queueCnt)
        {
            innerContainerPtr->transReadPoint++;
            if (containerTxCfgPtr->IpduMContainerQueueSize <= innerContainerPtr->transReadPoint)
            {
                innerContainerPtr->transReadPoint = 0x0u;
            }
        }
        /* transmit timer handle */
        innerContainerPtr->transTimer = 0x0u;
        SchM_Exit_IpduM_Context();
/* tx conf timer handle */
#if (STD_ON != IPUDM_TRIG_CONTAINER_SUPPORT_TXCONF)
        /* when container supprt tx conf,need fill txConfTimer otherwise,txconf timer equal 0*/
        if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
#endif /* STD_ON != IPUDM_TRIG_SUPPORT_TXCONF */
        {
            SchM_Enter_IpduM_Context();
            if (NULL_PTR != containerTxCfgPtr->IpduMContainerTxConfirmationTimeout)
            {
                innerContainerPtr->txConfTimer = *(containerTxCfgPtr->IpduMContainerTxConfirmationTimeout);
            }
            else
            {
                innerContainerPtr->txConfTimer = IPDUM_CONTAINER_DEFAULT_TXCONFTIMER;
            }
            SchM_Exit_IpduM_Context();
        }
        /* trigger transmit fill output data */
        if ((NULL_PTR != PduInfoPtr) && (IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode))
        {
#if (STD_ON != IPUDM_TRIG_CONTAINER_SUPPORT_TXCONF)
            /* when trigger transmit not support txconf,queue state shall be immediate handle */
            /* reset payload size of the container */
            incIndexPtr->containedTxConfirmStartPos = containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
            incIndexPtr->containedTxConfirmCnt = 0x0u;
            /* queue handle */
            if (0u < innerContainerPtr->queueCnt)
            {
                innerContainerPtr->queueCnt--;
            }
#endif /* STD_ON != IPUDM_TRIG_SUPPORT_TXCONF */
        }
        if ((IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            || ((NULL_PTR != PduInfoPtr) && (IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode)))
        {
            /* @SWS_IpduM_000191
            IPDUM_DIRECT, and PduR_IpduMTransmit returns E_OK
            remove that instance from the queue */
            /* @SWS_IpduM_00220
            TriggerTransmit call shall a instance will be drop form queue */
            /* @SWS_IpduM_000193
            IPDUM_TRIGGERTRANSMIT keep before buffer until
            IpduM_TriggerTransmit call */
            /* every send container wil clear pdu in buf,avoid
            next analysis header is unknow */
            if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            {
                SchM_Enter_IpduM_Context();
                for (index = 0u; (index < PduInfo.SduLength); index++)
                {
                    PduInfo.SduDataPtr[index] = 0x0u;
                }
                SchM_Exit_IpduM_Context();
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* this function handle in api IpduM_Transmit,shall handle contained for
upper layer data */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE)
    IpduM_TransmitContained(IpduM_uintx containedIndex, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    const IpduMContainedTxPduType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    const IpduMContainerTxPduType* containerTxCfgPtr =
        &IpduM_CfgPtr->IpduMContainerTxPduPtr[containedTxCfgPtr->ContainedTxInContainerPduRef];
    IpduM_InnerContainerTxType* innerContainerPtr =
        &IpduM_InnerContainerTx[containedTxCfgPtr->ContainedTxInContainerPduRef];
    IpduM_uintx lstLen = 0x0u;
/* input parameters check */
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if ((NULL_PTR == containedTxCfgPtr->IpduMContainedTxPduRef)
        || (PduInfoPtr->SduLength > containedTxCfgPtr->IpduMContainedTxPduRef->PduLen)
        || (IPDUM_CONTAINER_TX_NUMBER_MAX <= containedTxCfgPtr->ContainedTxInContainerPduRef))
    {
        (void)Det_ReportError(IPDUM_MODULE_ID, IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
    }
    else
#endif /* STD_ON == IPDUM_DEV_ERROR_DETECT */
    {
        if (0u == innerContainerPtr->txConfTimer)
        {
            /* last is best contained need update history */
            if (IPDUM_COLLECT_LAST_IS_BEST == containedTxCfgPtr->IpduMContainedTxPduCollectionSemantics)
            {
                /* find contained  position in container */
                ret = IpduM_ContainedFindLastIsBestInBufPos(containedIndex, &lstLen);
            }
            if (E_NOT_OK == ret)
            {
                /* @SWS_IpduM_00179
                IPDUM_COLLECT_QUEUED,this contained shall add to container */
                ret = IpduM_ContainedInBufPosCalc(containedIndex, &lstLen);
            }
            if (E_OK == ret)
            {
                /* @SWS_IpduM_00175
                    place the header of contained in front container */
                /* copy contained transmit data to global container buf */
                IpduM_ContainedFillToContainer(containedTxCfgPtr, lstLen, PduInfoPtr);
            }
        }
    }
    if (E_OK == ret)
    {
        /* contained trig condition check */
        IpduM_uintx queueIndex = (IpduM_uintx)innerContainerPtr->writePoint
                                 + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
        lstLen = IpduM_InnerContainerTxQueueRecode[queueIndex].payloadSize;
        /*  @SWS_IpduM_00180,@SWS_IpduM_00181
        trig check*/
        if ((IPDUM_TRIGGER_ALWAYS == containedTxCfgPtr->IpduMContainedTxPduTrigge)
            || ((NULL_PTR != containerTxCfgPtr->IpduMContainerTxSizeThreshold)
                && (lstLen >= *(containerTxCfgPtr->IpduMContainerTxSizeThreshold))))
        {
            innerContainerPtr->trigFlg = TRUE;
        }
        if (FALSE == innerContainerPtr->firstAddFlg)
        {
            innerContainerPtr->firstAddFlg = TRUE;
            if (TRUE == containerTxCfgPtr->IpduMContainerTxFirstContainedPduTrigger)
            {
                /* @SWS_IpduM_00201
                first trig */
                innerContainerPtr->trigFlg = TRUE;
            }
        }
        SchM_Enter_IpduM_Context();
        if (NULL_PTR != containerTxCfgPtr->IpduMContainerTxSendTimeout)
        {
            if (0x0u == innerContainerPtr->transTimer)
            {
                innerContainerPtr->transTimer = *(containerTxCfgPtr->IpduMContainerTxSendTimeout);
            }
            if ((NULL_PTR != containedTxCfgPtr->IpduMContainedTxPduSendTimeout)
                && (innerContainerPtr->transTimer > *(containedTxCfgPtr->IpduMContainedTxPduSendTimeout)))
            {
                innerContainerPtr->transTimer = *(containedTxCfgPtr->IpduMContainedTxPduSendTimeout);
            }
        }
        SchM_Exit_IpduM_Context();
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX > 0u */

/* this Function shall be realize by Macro */
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* Multiplexer pdu data info handle by segment */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE)
    IpudM_SegmentCopy(P2CONST(IpduM_PduTranslateType, AUTOMATIC, IPDUM_APPL_DATA) pduTransPtr)
{
    /* motolora big_endian
    eg: 13bit signal
    bit
    7   6   5   4   3   2   1   0
            msb<-----------------
    15  14  13  12  11  10  9   8
    -----------------------lsb
    */
    /* intel little_endian
    eg: 13bit signal
    bit
    7   6   5   4   3   2   1   0
    -----------------------lsb
    15  14  13  12  11  10  9   8
           msb<------------------
    */
    Std_ReturnType ret = E_OK;
    SegmentCntType i;
    uint8 intTmp, outTmp;
    uint16 bitCnt;
    uint16 bitPos;
    /* once copy bit len */
    uint8 onCoBitCnt;
    uint8 byteBitStartPos, byteBitEndPos;
    /* input Parameters check */
    if ((NULL_PTR == pduTransPtr) || (NULL_PTR == pduTransPtr->inputPduPtr) || (NULL_PTR == pduTransPtr->outputPduPtr)
        || (NULL_PTR == pduTransPtr->segmentPtr) || (NULL_PTR == pduTransPtr->inputPduPtr->SduDataPtr)
        || (NULL_PTR == pduTransPtr->outputPduPtr->SduDataPtr))
    {
        ret = E_NOT_OK;
    }
    /* endof input Parameters check */
    for (i = 0u; (E_OK == ret) && (i < (pduTransPtr->segmentCnt)); i++)
    {
        bitCnt = pduTransPtr->segmentPtr[i].IpduMSegmentLength;
        bitPos = pduTransPtr->segmentPtr[i].IpduMSegmentPosition;
        bitCnt >>= 3u;
        if ((bitCnt > pduTransPtr->inputPduPtr->SduLength) || (bitCnt > pduTransPtr->outputPduPtr->SduLength))
        {
            ret = E_NOT_OK;
        }
        bitCnt = pduTransPtr->segmentPtr[i].IpduMSegmentLength;
        while ((bitCnt != 0u) && (E_OK == ret))
        {
            SchM_Enter_IpduM_Context();
            intTmp = pduTransPtr->inputPduPtr->SduDataPtr[bitPos >> 3u];
            SchM_Exit_IpduM_Context();
            byteBitStartPos = (uint8)(bitPos & 0x07u);
            /* calculated end copy bit position in byte */
            if (0x8u > (byteBitStartPos + bitCnt))
            {
                byteBitEndPos = (uint8)(byteBitStartPos + bitCnt - 1u);
            }
            else
            {
                byteBitEndPos = 0x7u;
            }
            if ((0u != byteBitStartPos) || (byteBitEndPos != 0x07u))
            {
                /* bit data copy
                copy start position is greath then now position
                */
                outTmp = pduTransPtr->outputPduPtr->SduDataPtr[bitPos >> 3u];
                onCoBitCnt = byteBitEndPos - byteBitStartPos + 1u;
                HoldBitInSection(intTmp, byteBitStartPos, byteBitEndPos);
                ClearBitInSection(outTmp, byteBitStartPos, byteBitEndPos);
                outTmp |= intTmp;
                SchM_Enter_IpduM_Context();
                pduTransPtr->outputPduPtr->SduDataPtr[bitPos >> 3u] = outTmp;
                SchM_Exit_IpduM_Context();
                /* restart calculated bit position */
                if ((byteBitEndPos == 0x07u) && (IPDUM_BYTE_ORDER_BIG_ENDIAN == pduTransPtr->byteOrd))
                {
                    if (bitPos >= 8u)
                    {
                        bitPos = (bitPos - 8u) & 0xfff8u;
                    }
                }
                else
                {
                    bitPos += onCoBitCnt;
                }
            }
            else
            {
                onCoBitCnt = 8u;
                /* byte data copy */
                SchM_Enter_IpduM_Context();
                pduTransPtr->outputPduPtr->SduDataPtr[bitPos >> 3u] = intTmp;
                SchM_Exit_IpduM_Context();
                /* different endian bit position calculated */
                if (IPDUM_BYTE_ORDER_BIG_ENDIAN == pduTransPtr->byteOrd)
                {
                    if (bitPos >= 8u)
                    {
                        bitPos = bitPos - 0x8u;
                    }
                }
                else
                {
                    bitPos = bitPos + 0x8u;
                }
            }
            bitCnt -= onCoBitCnt;
            if ((bitPos > (uint16)(pduTransPtr->outputPduPtr->SduLength << 3u))
                || (bitPos > (uint16)(pduTransPtr->inputPduPtr->SduLength << 3u)))
            {
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* Multiplexer assemble part pdu to tx request */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexTxTrigLoadData(IpduM_uintx txReqIndex)
{
    Std_ReturnType ret = E_OK;
    PduInfoType trigGetPdu;
    PduInfoType outPdu;
    const IpduM_TxPartType* txPartPtr;
    const IpduMTxRequestType* txRequstPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txReqIndex];
    const IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxReq[txReqIndex];
    IpduM_PduTranslateType pduTrans;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    IpduM_uintx index;
    /* handle static part */
    if (NULL_PTR != txRequstPtr->IpduMTxStaticPartPtr)
    {
        for (index = 0u; ((index < txRequstPtr->IpduMTxStaticPartCnt) && (E_OK == ret)); index++)
        {
            txPartPtr = &txRequstPtr->IpduMTxStaticPartPtr[index];
            /* @SWS_IpduM_000168,@SWS_IpduM_000169
            update this part if IpduMJitUpdate configured to true */
            if (TRUE == txPartPtr->IpduMJitUpdate)
            {
#if (IPDUM_MAX_PDU_BUF_SIZE > 0u)
                SchM_Enter_IpduM_Context();
                trigGetPdu.SduDataPtr = IpduM_PduBuf;
                SchM_Exit_IpduM_Context();
#else
                trigGetPdu.SduDataPtr = NULL_PTR;
#endif /* IPDUM_MAX_PDU_BUF_SIZE > 0u */
                trigGetPdu.SduLength = txPartPtr->IpduMTxPduRef->PduLen;
                ret = PduR_IpduMTriggerTransmit(txPartPtr->IpduMTxPduRef->PduId, &trigGetPdu);
                if (E_OK == ret)
                {
                    SchM_Enter_IpduM_Context();
                    outPdu.SduDataPtr = &IpduM_DtaBuf[txRequstPtr->IpduMBufIndex];
                    outPdu.SduLength = (PduLengthType)txRequstPtr->IpduMOutgoingPduRef->PduLen;
                    pduTrans.byteOrd = txRequstPtr->IpduMByteOrder;
                    pduTrans.inputPduPtr = &trigGetPdu;
                    pduTrans.outputPduPtr = &outPdu;
                    pduTrans.segmentCnt = (uint8)(txPartPtr->IpduMTxSegmentCnt);
                    pduTrans.segmentPtr = txPartPtr->IpduMTxSegmentPtr;
                    SchM_Exit_IpduM_Context();
                    ret = IpudM_SegmentCopy(&pduTrans);
                }
            }
        }
    }
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
    if ((NULL_PTR != txRequstPtr->IpduMTxDynamicPartPtr) && (E_OK == ret))
    {
        /* @SWS_IpduM_000223
        dynamic part select latest send by upper layer */
        txPartPtr = &txRequstPtr->IpduMTxDynamicPartPtr[innerTxReqPtr->lastTxDynamicPartIndex];
        if (TRUE == txPartPtr->IpduMJitUpdate)
        {
#if (IPDUM_MAX_PDU_BUF_SIZE > 0u)
            SchM_Enter_IpduM_Context();
            trigGetPdu.SduDataPtr = IpduM_PduBuf;
            SchM_Exit_IpduM_Context();
#else
            trigGetPdu.SduDataPtr = NULL_PTR;
#endif /* IPDUM_MAX_PDU_BUF_SIZE > 0u */
            trigGetPdu.SduLength = txPartPtr->IpduMTxPduRef->PduLen;
            ret = PduR_IpduMTriggerTransmit(txPartPtr->IpduMTxPduRef->PduId, &trigGetPdu);
            if (E_OK == ret)
            {
                SchM_Enter_IpduM_Context();
                outPdu.SduDataPtr = &IpduM_DtaBuf[txRequstPtr->IpduMBufIndex];
                outPdu.SduLength = (PduLengthType)txRequstPtr->IpduMOutgoingPduRef->PduLen;
                pduTrans.byteOrd = txRequstPtr->IpduMByteOrder;
                pduTrans.inputPduPtr = &trigGetPdu;
                pduTrans.outputPduPtr = &outPdu;
                pduTrans.segmentCnt = (uint8)(txPartPtr->IpduMTxSegmentCnt);
                pduTrans.segmentPtr = txPartPtr->IpduMTxSegmentPtr;
                SchM_Exit_IpduM_Context();
                ret = IpudM_SegmentCopy(&pduTrans);
            }
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    /* @SWS_IpduM_00171,@SWS_IpduM_00172
    if any part via PduR_IpduMTriggerTransmit E_NOT_OK,
        shall return E_NOT_OK */
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* Multiplexer tx request transmit */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexTxSendPdu(IpduM_uintx txReqIndex)
{
    Std_ReturnType ret;
    PduInfoType trigGetPdu;
    const IpduMTxRequestType* txRequstPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txReqIndex];
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxReq[txReqIndex];
    SchM_Enter_IpduM_Context();
    trigGetPdu.SduDataPtr = &IpduM_DtaBuf[txRequstPtr->IpduMBufIndex];
    trigGetPdu.SduLength = (PduLengthType)txRequstPtr->IpduMOutgoingPduRef->PduLen;
    SchM_Exit_IpduM_Context();
    ret = PduR_IpduMTransmit(txRequstPtr->IpduMOutgoingPduRef->PduId, &trigGetPdu);
    if (E_OK == ret)
    {
        SchM_Enter_IpduM_Context();
        innerTxReqPtr->txConfTimer = txRequstPtr->IpduMTxConfirmationTimeout;
        SchM_Exit_IpduM_Context();
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* Multiplexer handle tx confirmation */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_TxConfOfMultiplex(IpduM_uintx txReqIndex)
{
    /* @SWS_IpduM_00022
    shall translate confirmation into the corresponding confirmation for I-pdu
    which were contained in the last sent out multiplexed */
    const IpduM_TxPartType* txPartPtr;
    const IpduMTxRequestType* txRequstPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txReqIndex];
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxReq[txReqIndex];
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    IpduM_uintx index;
#endif
    /* @SWS_IpduM_00024
    if a previously request transmit has been timed out,but is confirm now
        then shall discard */
    SchM_Enter_IpduM_Context();
    uint16 txConfTim = innerTxReqPtr->txConfTimer;
    SchM_Exit_IpduM_Context();
    if ((0x0u != txConfTim) || (0x0u == txRequstPtr->IpduMTxConfirmationTimeout)
#if (STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF)
        || (IPDUM_TRIGGER_MODE_NONE == txRequstPtr->IpduMTxTriggerMode)
#endif /* STD_ON == IPUDM_TRIG_MULT_SUPPORT_TXCONF */
    )
    {
        SchM_Enter_IpduM_Context();
        innerTxReqPtr->txConfTimer = 0x0u;
        SchM_Exit_IpduM_Context();
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
        if (NULL_PTR != txRequstPtr->IpduMTxStaticPartPtr)
        {
            for (index = 0u; (index < txRequstPtr->IpduMTxStaticPartCnt); index++)
            {
                txPartPtr = &txRequstPtr->IpduMTxStaticPartPtr[index];
                if (TRUE == txPartPtr->IpduMTxConfirmation)
                {
                    IPDUM_NotifyTxConfirmation(txPartPtr->IpduMTxPduRef->PduId);
                }
            }
        }
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
        if (NULL_PTR != txRequstPtr->IpduMTxDynamicPartPtr)
        {
            txPartPtr = &txRequstPtr->IpduMTxDynamicPartPtr[innerTxReqPtr->lastTxDynamicPartIndex];
            if (TRUE == txPartPtr->IpduMTxConfirmation)
            {
                IPDUM_NotifyTxConfirmation(txPartPtr->IpduMTxPduRef->PduId);
            }
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_TX_REQUEST_NUMBER_MAX > 0u */

#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
/* Multiplexer rx indiation handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE)
    IpduM_RxMultiplexHandle(IpduM_uintx rxIndIndex, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
    /* Variable declarations */
    IpduM_uintx index;
    const IpduMRxIndicationType* rxIndPtr = &IpduM_CfgPtr->IpduMRxIndicationPtr[rxIndIndex];
    Std_ReturnType ret;
    PduInfoType outputPdu;
    uint16 selcetVal = 0x0u;
    boolean findFlg = FALSE;
    /* Variable init */
    SchM_Enter_IpduM_Context();
    outputPdu.SduDataPtr = PduInfoPtr->SduDataPtr;
    SchM_Exit_IpduM_Context();
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /*
    @SWS_IpduM_00041
     handle static part */
    if (NULL_PTR != rxIndPtr->IpduMRxStaticPartPtr)
    {
        const IpduMRxStaticPartType* rxStaticPrtPtr;
        for (index = 0u; (index < rxIndPtr->IpduMRxStaticPartCnt); index++)
        {
            rxStaticPrtPtr = &rxIndPtr->IpduMRxStaticPartPtr[index];
            outputPdu.SduLength = rxStaticPrtPtr->IpduMOutgoingStaticPduRef->PduLen;
            IPDUM_NotifyRxIndication(rxStaticPrtPtr->IpduMOutgoingStaticPduRef->PduId, &outputPdu);
        }
    }
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
    /* @SWS_IpduM_00042
    handle Dynamic part */
    if (NULL_PTR != rxIndPtr->IpduMRxDynamicPartPtr)
    {
        const IpduMRxDynamicPartType* rxDynamicPartPtr;
        /* find selcet value */
        ret = IpduM_MultiplexFindSelectVal(
            rxIndPtr->IpduMByteOrder,
            PduInfoPtr,
            &rxIndPtr->IpduMSelectorField,
            &selcetVal);
        for (index = 0u; ((E_OK == ret) && (index < rxIndPtr->IpduMRxDynamicPartCnt) && (FALSE == findFlg)); index++)
        {
            rxDynamicPartPtr = &rxIndPtr->IpduMRxDynamicPartPtr[index];
            if (selcetVal == rxDynamicPartPtr->IpduMRxSelectorValue)
            {
                findFlg = TRUE;
            }
        }
        if (TRUE == findFlg)
        {
            outputPdu.SduLength = rxDynamicPartPtr->IpduMOutgoingDynamicPduRef->PduLen;
            IPDUM_NotifyRxIndication(rxDynamicPartPtr->IpduMOutgoingDynamicPduRef->PduId, &outputPdu);
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_RX_INDICA_NUMBER_MAX > 0u */

#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
/* Multiplexer RX indiation pdu find select value */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MultiplexFindSelectVal(
    IpduM_ByteOrderType byteOrder,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr,
    P2CONST(IpduMSelectorFieldType, AUTOMATIC, IPDUM_APPL_DATA) selectFieldPtr,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, IPDUM_APPL_DATA) selcetValPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    /* motolan big endian:
        0x11223344
        0x11 0x22 0x33 0x44
        low ----------> high
     */
    /* intel little_endian:
            0x11223344
            0x44 0x33 0x22 0x11
            low ----------> high
     */
    /*check parameters*/
    Std_ReturnType ret = E_OK;
    /* inner function is not need parameters check */
    uint8 tmp, onByteCopyBits;
    uint8 byteBitStartPos, byteBitEndPos;
    uint8 bitCnt = selectFieldPtr->IpduMSelectorFieldLength;
    uint16 bitPos = selectFieldPtr->IpduMSelectorFieldPosition;
    uint16 outSeletVal = 0x0u;
    while (0x0u != bitCnt)
    {
        SchM_Enter_IpduM_Context();
        tmp = pduInfoPtr->SduDataPtr[bitPos >> 3u];
        SchM_Exit_IpduM_Context();
        byteBitStartPos = (uint8)(bitPos & 0x07u);
        /* calculated end copy bit position in byte */
        if (0x8u > (byteBitStartPos + bitCnt))
        {
            byteBitEndPos = byteBitStartPos + bitCnt - 1u;
        }
        else
        {
            byteBitEndPos = 0x7u;
        }
        if ((0u != byteBitStartPos) || (byteBitEndPos != 0x07u))
        {
            /* bit data copy: copy start position is greath then now position*/
            ExtractBitInSection(tmp, byteBitStartPos, byteBitEndPos);
            /* restart calculated bit position */
            if ((byteBitEndPos == 0x07u) && (IPDUM_BYTE_ORDER_BIG_ENDIAN == byteOrder))
            {
                if (bitPos >= 8u)
                {
                    bitPos = (bitPos - 8u) & 0xfff8u;
                }
            }
            else
            {
                bitPos += ((uint16)byteBitEndPos - (uint16)byteBitStartPos + 1u);
            }
            /* calculate bit count */
            onByteCopyBits = (byteBitEndPos - byteBitStartPos + 1u);
        }
        else
        {
            onByteCopyBits = 8u;
            if (IPDUM_BYTE_ORDER_BIG_ENDIAN == byteOrder)
            {
                if (bitPos >= 8u)
                {
                    bitPos = bitPos - 0x8u;
                }
            }
            else
            {
                bitPos = bitPos + 0x8u;
            }
        }
        bitCnt -= onByteCopyBits;
        if (bitPos > (uint16)((pduInfoPtr->SduLength) << 3u))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* all endian ,first find value is lsb */
            outSeletVal >>= (byteBitEndPos - byteBitStartPos + 1u);
            outSeletVal |= (uint16)((uint16)tmp << (15u - (byteBitEndPos - byteBitStartPos)));
        }
    }
    /* remove lsb fill not used data */
    outSeletVal >>= (16u - selectFieldPtr->IpduMSelectorFieldLength);
    SchM_Enter_IpduM_Context();
    *selcetValPtr = outSeletVal;
    SchM_Exit_IpduM_Context();
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_RX_INDICA_NUMBER_MAX > 0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/* in rx indiation handle container */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE)
    IpduM_ContainerRxIndication(IpduM_uintx containerIndex, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
    const IpduMContainerRxPduType* containerRxPtr = &IpduM_CfgPtr->IpduMContainerRxPduPtr[containerIndex];
    IpduM_InnerContainerRxType* innerContainerRxPtr = &IpduM_InnerContainerRx[containerIndex];
    IpduM_uintx queueIndex =
        (IpduM_uintx)innerContainerRxPtr->writePoint + (IpduM_uintx)containerRxPtr->IpduMContainerRxQueueStartPosIndex;
    IpduM_InnerContainerRxQueueHandleType* inhIndexRxPtr = &IpduM_InnerContainerRxQueueRecode[queueIndex];
    IpduM_uintx index;
    /* data copy handle */
    if ((0u < PduInfoPtr->SduLength) && (PduInfoPtr->SduLength <= containerRxPtr->IpduMContainerRxPduRef->PduLen))
    {
        /* calculate byf index */
        IpduM_uintx bufIndex = (IpduM_uintx)containerRxPtr->IpduMBufIndex
                               + ((IpduM_uintx)innerContainerRxPtr->writePoint
                                  * (IpduM_uintx)containerRxPtr->IpduMContainerRxPduRef->PduLen);
        SchM_Enter_IpduM_Context();
        uint8* dtaBufPtr = &IpduM_DtaBuf[bufIndex];
        for (index = 0u; (index < PduInfoPtr->SduLength); index++)
        {
            dtaBufPtr[index] = PduInfoPtr->SduDataPtr[index];
        }
        inhIndexRxPtr->payloadSize = (uint16)PduInfoPtr->SduLength;
        /* queue handle */
        innerContainerRxPtr->writePoint++;
        if (innerContainerRxPtr->writePoint >= containerRxPtr->IpduMContainerQueueSize)
        {
            innerContainerRxPtr->writePoint = 0x0u;
        }
        innerContainerRxPtr->queueCnt++;
        if (innerContainerRxPtr->queueCnt > containerRxPtr->IpduMContainerQueueSize)
        {
            innerContainerRxPtr->queueCnt = containerRxPtr->IpduMContainerQueueSize;
            /* @SWS_IpduM_00212
            exceed IpduMContainerQueueSize
            shall report det */
            innerContainerRxPtr->readPoint++;
            if (innerContainerRxPtr->readPoint >= containerRxPtr->IpduMContainerQueueSize)
            {
                innerContainerRxPtr->readPoint = 0u;
            }
            (void)Det_ReportRuntimeError(
                IPDUM_MODULE_ID,
                IPDUM_INSTANCE_ID,
                IPDUM_SERVICE_ID_RXINDICATION,
                IPDUM_E_QUEUEOVFL);
        }
        SchM_Exit_IpduM_Context();
        /* @SWS_IpduM_00211
            IPDUM_PROCESSING_DEFERRED shall be queue
            next mainfunction handle */
        if (IPDUM_PROCESSING_IMMEDIATE == containerRxPtr->IpduMContainerPduProcessing)
        {
            IpduM_ContainerRxHandle(containerIndex);
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_ContainerRxHandle(IpduM_uintx containerIndex)
{
    const IpduMContainerRxPduType* containerRxPtr = &IpduM_CfgPtr->IpduMContainerRxPduPtr[containerIndex];
    IpduM_InnerContainerRxType* innerContainerRxPtr = &IpduM_InnerContainerRx[containerIndex];
    IpduM_uintx queueIndex =
        (IpduM_uintx)innerContainerRxPtr->readPoint + (IpduM_uintx)containerRxPtr->IpduMContainerRxQueueStartPosIndex;
    const IpduM_InnerContainerRxQueueHandleType* inhIndexRxPtr = &IpduM_InnerContainerRxQueueRecode[queueIndex];
    /* find buf index */
    IpduM_uintx bufIndex =
        (IpduM_uintx)containerRxPtr->IpduMBufIndex
        + ((IpduM_uintx)innerContainerRxPtr->readPoint * (IpduM_uintx)containerRxPtr->IpduMContainerRxPduRef->PduLen);
    IpduM_ContainedRxExtractedHandle(containerIndex, &IpduM_DtaBuf[bufIndex], inhIndexRxPtr->payloadSize);
    SchM_Enter_IpduM_Context();
    /*queue handle*/
    if (0u < innerContainerRxPtr->queueCnt)
    {
        innerContainerRxPtr->readPoint++;
        if (innerContainerRxPtr->readPoint >= containerRxPtr->IpduMContainerQueueSize)
        {
            innerContainerRxPtr->readPoint = 0x0u;
        }
        innerContainerRxPtr->queueCnt--;
    }
    SchM_Exit_IpduM_Context();
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/* extracted contained in container rx pdu*/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_ContainedRxExtractedHandle(
    IpduM_uintx containerIndex,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dtaPtr,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    IpduM_uintx lstlen)
{
    const IpduMContainerRxPduType* containerRxPtr = &IpduM_CfgPtr->IpduMContainerRxPduPtr[containerIndex];
    const IpduMContainedRxPduType* containedRxPtr;
    boolean stopFlg = FALSE, findFlg;
    IpduM_uintx index = 0x0u, loop;
    uint32 headerId, headerDlc;
    PduInfoType outPdu;
    P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) localDataPtr = dtaPtr;
    while ((index < lstlen) && (FALSE == stopFlg))
    {
        SchM_Enter_IpduM_Context();
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
        headerId = (uint32)(*localDataPtr);
        localDataPtr++;
        headerId <<= 8u;
        headerId |= (uint32)(*localDataPtr);
        localDataPtr++;
        headerId <<= 8u;
        headerId |= (uint32)(*localDataPtr);
        localDataPtr++;
        index += 3u;
        if (IPDUM_HEADERTYPE_LONG == containerRxPtr->IpduMContainerHeaderSize)
        {
            headerId <<= 8u;
            headerId |= (uint32)(*localDataPtr);
            localDataPtr++;
            index++;
        }
        headerDlc = (uint32)(*localDataPtr);
        localDataPtr++;
        index++;
        if (IPDUM_HEADERTYPE_LONG == containerRxPtr->IpduMContainerHeaderSize)
        {
            headerDlc <<= 8u;
            headerDlc |= (uint32)(*localDataPtr);
            localDataPtr++;
            headerDlc <<= 8u;
            headerDlc |= (uint32)(*localDataPtr);
            localDataPtr++;
            headerDlc <<= 8u;
            headerDlc |= (uint32)(*localDataPtr);
            localDataPtr++;
            index += 3u;
        }
#else  /* IPDUM_LITTLE_ENDIAN */

        headerId = (uint32)(*localDataPtr);
        localDataPtr++;
        headerId |= (uint32)(((uint32)(*localDataPtr)) << 8u);
        localDataPtr++;
        headerId |= (uint32)(((uint32)(*localDataPtr)) << 16u);
        localDataPtr++;
        index += 3u;
        if (IPDUM_HEADERTYPE_LONG == containerRxPtr->IpduMContainerHeaderSize)
        {
            headerId |= (uint32)(((uint32)(*localDataPtr)) << 24u);
            localDataPtr++;
            index++;
        }
        headerDlc = (uint32)(*localDataPtr);
        localDataPtr++;
        index++;
        if (IPDUM_HEADERTYPE_LONG == containerRxPtr->IpduMContainerHeaderSize)
        {
            headerDlc |= (uint32)(((uint32)(*localDataPtr)) << 8u);
            localDataPtr++;
            headerDlc |= (uint32)(((uint32)(*localDataPtr)) << 16u);
            localDataPtr++;
            headerDlc |= (uint32)(((uint32)(*localDataPtr)) << 24u);
            localDataPtr++;
            index += 3u;
        }
#endif /* IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER */
        SchM_Exit_IpduM_Context();
        if ((0x0u != headerId) && (0x0u != headerDlc))
        {
            findFlg = FALSE;
            for (loop = 0x0u; (IPDUM_CONTAINED_RX_NUMBER_MAX > loop) && (FALSE == findFlg); loop++)
            {
                containedRxPtr = &IpduM_CfgPtr->IpduMContainedRxPduPtr[loop];
                /* @SWS_IpduM_00208
                check length */
                if ((headerId == containedRxPtr->IpduMContainedPduHeaderId)
                    && (headerDlc == containedRxPtr->IpduMContainedRxPduRef->PduLen))
                {
                    findFlg = TRUE;
                }
            }
            if (TRUE == findFlg)
            {
                if ((headerDlc + index) > lstlen)
                {
                    /* @SWS_IpduM_00213
                    the payload length exceeds the remaining bytes,
                    processing shall be stop,and report det
                    */
                    /*report DET error*/
                    (void)Det_ReportRuntimeError(
                        IPDUM_MODULE_ID,
                        IPDUM_INSTANCE_ID,
                        IPDUM_SERVICE_ID_RXINDICATION,
                        IPDUM_E_HEADER);
                }
                else
                {
                    /* @SWS_IpduM_00205,@SWS_IpduM_00206
                    rx accept check */
                    if ((IPDUM_ACCEPT_ALL == containerRxPtr->IpduMContainerRxAcceptContainedPdu)
                        || ((IPDUM_ACCEPT_CONFIGURED == containerRxPtr->IpduMContainerRxAcceptContainedPdu)
                            && (containerIndex == containedRxPtr->IpduMContainedRxInContainerPduRef)))
                    {
                        /* @SWS_IpduM_00209
                        notify PDUR */
                        SchM_Enter_IpduM_Context();
                        outPdu.SduDataPtr = localDataPtr;
                        outPdu.SduLength = (PduLengthType)headerDlc;
                        SchM_Exit_IpduM_Context();
                        PduR_IpduMRxIndication(containedRxPtr->IpduMContainedRxPduRef->PduId, &outPdu);
                    }
                }
            }
            /* @SWS_IpduM_00207
            not match,shall be discarded silently */
            index += headerDlc;
            localDataPtr += headerDlc; /* PRQA S 0488 */ /* MISRA Rule 18.4 */
        }
        else
        {
            /* @SWS_IpduM_00210
            received Container PDU,etecting a header containing the ID 0
            shall be stopped */
            stopFlg = TRUE;
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_RX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* container tx confirmation handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_TxConfOfContainer(IpduM_uintx containerIndex)
{
    const IpduMContainerTxPduType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
    IpduM_InnerContainerTxType* innerContainerPtr = &IpduM_InnerContainerTx[containerIndex];
    IpduM_uintx index, containedIndex;
    const IpduMContainedTxPduType* containedTxCfgPtr;
    IpduM_uintx queueIndex = (IpduM_uintx)innerContainerPtr->txCofReadPoint
                             + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
    IpduM_InnerContainerTxQueueHandleType* incIndexPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
    /* @SWS_IpduM_00198
     discard unexpected tx confirmation */
    SchM_Enter_IpduM_Context();
    uint16 txConfTim = innerContainerPtr->txConfTimer;
    SchM_Exit_IpduM_Context();
    if (((0x0u < txConfTim) && (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode))
#if (STD_ON == IPUDM_TRIG_CONTAINER_SUPPORT_TXCONF)
        || (IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
#endif /* STD_ON == IPUDM_TRIG_SUPPORT_TXCONF */
    )
    {
        SchM_Enter_IpduM_Context();
        innerContainerPtr->txConfTimer = 0x0u;
        SchM_Exit_IpduM_Context();
        for (index = 0; index < incIndexPtr->containedTxConfirmCnt; index++)
        {
            containedIndex = IpduM_InnerContainedIndexRecode[incIndexPtr->containedTxConfirmStartPos + index];
            if (IPDUM_CONTAINED_TX_NUMBER_MAX > containedIndex)
            {
                containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
                /* @SWS_IpduM_00196,
                translate this confirmation  */
                if (TRUE == containedTxCfgPtr->IpduMContainedTxPduConfirmation)
                {
                    PduR_IpduMTxConfirmation(containedTxCfgPtr->IpduMContainedTxPduRef->PduId);
                }
            }
            /* clear contained index */
            IpduM_ContainedPendingTxConfirmation[incIndexPtr->containedTxConfirmStartPos + index] = IPDUM_UNUSED_UINT8;
            IpduM_InnerContainedIndexRecode[incIndexPtr->containedTxConfirmStartPos + index] = IPDUM_UNUSED_UINT8;
        }
        SchM_Enter_IpduM_Context();
        incIndexPtr->containedTxConfirmStartPos = containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex;
        incIndexPtr->containedTxConfirmCnt = 0u;
        /* queue handle */
        if (0u < innerContainerPtr->queueCnt)
        {
            innerContainerPtr->txCofReadPoint++;
            if (containerTxCfgPtr->IpduMContainerQueueSize <= innerContainerPtr->txCofReadPoint)
            {
                innerContainerPtr->txCofReadPoint = 0x0u;
            }
            innerContainerPtr->queueCnt--;
        }
        SchM_Exit_IpduM_Context();
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* container tx i-pdu handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_ContainerTxHandle(IpduM_uintx index)
{
    IpduM_InnerContainerTxType* innerContainerTxPtr;
    const IpduMContainerTxPduType* containerTxCfgPtr;

    SchM_Enter_IpduM_Context();
    innerContainerTxPtr = &IpduM_InnerContainerTx[index];
    containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[index];
    if (0x0u < innerContainerTxPtr->txConfTimer)
    {
        innerContainerTxPtr->txConfTimer--;
        if (0u == innerContainerTxPtr->txConfTimer)
        {
            /* @SWS_IpduM_000190
                if queue have old container,shall transmit this in next mainfunction */
            innerContainerTxPtr->trigFlg = IpduM_ContainerCheckQueueIsExit(index);
        }
    }
    if ((NULL_PTR != containerTxCfgPtr->IpduMContainerTxSendTimeout) && (0x0u < innerContainerTxPtr->transTimer))
    {
        innerContainerTxPtr->transTimer--;
        if (0x0u == innerContainerTxPtr->transTimer)
        {
            innerContainerTxPtr->trigFlg = TRUE;
            /* @SWS_IpduM_00186
            when transmission timer of container has elapse,this
            container shall be trigger */
        }
    }
    SchM_Exit_IpduM_Context();
    if (TRUE == innerContainerTxPtr->trigFlg)
    {
        /* @SWS_IpduM_00185
        If PduR_IpduMTransmit has returned E_NOT_OK,
            repeated in the next main function cycle */
        if (E_OK == IpduM_TransmitContainer(index, NULL_PTR))
        {
            innerContainerTxPtr->trigFlg = FALSE;
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* contained in container queue need be calculate position in buffer */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    IpduM_ContainedInBufPosCalc(IpduM_uintx containedIndex, P2VAR(IpduM_uintx, AUTOMATIC, IPDUM_APPL_DATA) posInBufPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    IpduM_uintx bufIndex;
    /* structure value init */
    const IpduMContainedTxPduType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    const IpduMContainerTxPduType* containerTxCfgPtr =
        &IpduM_CfgPtr->IpduMContainerTxPduPtr[containedTxCfgPtr->ContainedTxInContainerPduRef];
    IpduM_InnerContainerTxType* innerContainerPtr =
        &IpduM_InnerContainerTx[containedTxCfgPtr->ContainedTxInContainerPduRef];
    IpduM_uintx queueIndex =
        (IpduM_uintx)innerContainerPtr->writePoint + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
    IpduM_InnerContainerTxQueueHandleType* incIndexTxPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
    IpduM_uintx index = (IpduM_uintx)incIndexTxPtr->containedStartPos + (IpduM_uintx)incIndexTxPtr->containedCnt;
    /*calculate contained instance size*/
    IpduM_uintx containedLen = 0x4u + (IpduM_uintx)containedTxCfgPtr->IpduMContainedTxPduRef->PduLen;
    /* calculate this queue surplus size */
    IpduM_uintx surplusSize =
        ((IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen - (IpduM_uintx)incIndexTxPtr->payloadSize);
    if (IPDUM_HEADERTYPE_LONG == containerTxCfgPtr->IpduMContainerHeaderSize)
    {
        containedLen += 0x4u;
    }
    if (containedLen > surplusSize)
    {
        /* shall add next queue */
        /* @SWS_IpduM_00183
        IPDUM_TRIGGERTRANSMIT,adding a contained ipdu would exceed maximum
        size of the container,first container shall be queued */
        if ((IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            && (innerContainerPtr->queueCnt >= containerTxCfgPtr->IpduMContainerQueueSize))
        {
/* @SWS_IpduM_00182
IPDUM_DIRECT,add a contained would exceed maximum size of the
container,first the container shall be trigger,
the contained shall be add to a new instance of the container */
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
            (void)IpduM_TransmitContainer(containedTxCfgPtr->ContainedTxInContainerPduRef, NULL_PTR);
#endif
        }
        SchM_Enter_IpduM_Context();
        innerContainerPtr->writePoint++;
        if (innerContainerPtr->writePoint >= containerTxCfgPtr->IpduMContainerQueueSize)
        {
            innerContainerPtr->writePoint = 0u;
        }
        innerContainerPtr->queueCnt++;
        if (containerTxCfgPtr->IpduMContainerQueueSize < innerContainerPtr->queueCnt)
        {
            /* @SWS_IpduM_00199
            oldest instance shall be discarded */
            innerContainerPtr->queueCnt = containerTxCfgPtr->IpduMContainerQueueSize;
            innerContainerPtr->transReadPoint++;
            if (innerContainerPtr->transReadPoint >= containerTxCfgPtr->IpduMContainerQueueSize)
            {
                innerContainerPtr->transReadPoint = 0u;
            }
            (void)Det_ReportRuntimeError(
                IPDUM_MODULE_ID,
                IPDUM_INSTANCE_ID,
                IPDUM_SERVICE_ID_TRANSMIT,
                IPDUM_E_QUEUEOVFL);
        }
        queueIndex = (IpduM_uintx)innerContainerPtr->writePoint
                     + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
        incIndexTxPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
        incIndexTxPtr->containedStartPos = (uint8)index;
        incIndexTxPtr->containedCnt = 0x0u;
        incIndexTxPtr->payloadSize = 0x0u;
        SchM_Exit_IpduM_Context();
    }
    /* next handle */
    /* this hande shall be complete add contained */
    SchM_Enter_IpduM_Context();
    bufIndex =
        ((IpduM_uintx)containerTxCfgPtr->IpduMBufIndex
         + ((IpduM_uintx)innerContainerPtr->writePoint * (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen)
         + (IpduM_uintx)incIndexTxPtr->payloadSize);
    *posInBufPtr = bufIndex;
    IpduM_InnerContainedIndexRecode[index] = (uint16)containedIndex;
    incIndexTxPtr->containedCnt++;
    incIndexTxPtr->payloadSize += (uint16)containedLen;
    if ((0u < incIndexTxPtr->payloadSize) && (0u == innerContainerPtr->queueCnt))
    {
        innerContainerPtr->queueCnt = 1u;
    }
    SchM_Exit_IpduM_Context();
    return E_OK;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* contained is LAST_IS_BEST shall find position in container queue buffer */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainedFindLastIsBestInBufPos(
    IpduM_uintx containedIndex,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(IpduM_uintx, AUTOMATIC, IPDUM_APPL_DATA) bufIndexPtr)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 startPos;
    IpduM_uintx index, loop, queueIndex, cnt;
    /* structure value init */
    const IpduMContainedTxPduType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    const IpduMContainerTxPduType* containerTxCfgPtr =
        &IpduM_CfgPtr->IpduMContainerTxPduPtr[containedTxCfgPtr->ContainedTxInContainerPduRef];
    const IpduM_InnerContainerTxType* innerContainerPtr =
        &IpduM_InnerContainerTx[containedTxCfgPtr->ContainedTxInContainerPduRef];
    IpduM_uintx bufIndex =
        ((IpduM_uintx)containerTxCfgPtr->IpduMBufIndex
         + ((IpduM_uintx)innerContainerPtr->writePoint
            * (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen));
    IpduM_uintx containedIndexTmp;
    queueIndex = containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
    /* find contained in index queue position */
    for (index = 0x0u; (index < containerTxCfgPtr->IpduMContainerQueueSize) && (E_NOT_OK == ret); index++)
    {
        cnt = IpduM_InnerContainerTxQueueRecode[queueIndex + index].containedCnt;
        startPos = IpduM_InnerContainerTxQueueRecode[queueIndex + index].containedStartPos;
        for (loop = 0x0u; (loop < cnt) && (E_NOT_OK == ret); loop++)
        {
            if (containedIndex == IpduM_InnerContainedIndexRecode[startPos])
            {
                cnt = loop;
                queueIndex += index;
                ret = E_OK;
            }
            startPos++;
        }
    }
    if (E_OK == ret)
    {
        /* calculate this contained in global buffer position */
        startPos = IpduM_InnerContainerTxQueueRecode[queueIndex].containedStartPos;
        for (index = 0; index < cnt; index++)
        {
            containedIndexTmp = IpduM_InnerContainedIndexRecode[startPos];
            containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndexTmp];
            bufIndex += 4u;
            if (IPDUM_HEADERTYPE_LONG == containerTxCfgPtr->IpduMContainerHeaderSize)
            {
                bufIndex += 4u;
            }
            bufIndex += containedTxCfgPtr->IpduMContainedTxPduRef->PduLen;
            startPos++;
        }
        *bufIndexPtr = bufIndex;
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* LAST_IS_BEST contained need get pdu date by triggertransmit handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainedTxTrigHandle(IpduM_uintx containerIndex)
{
    Std_ReturnType ret;
    IpduM_uintx index, containedIndex, bufIndex;
    /* structure value init */
    const IpduMContainerTxPduType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
    const IpduM_InnerContainerTxType* innerContainerPtr = &IpduM_InnerContainerTx[containerIndex];
    const IpduMContainedTxPduType* containedTxCfgPtr;
    IpduM_uintx queueIndex =
        ((IpduM_uintx)innerContainerPtr->transReadPoint
         + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex);
    IpduM_InnerContainerTxQueueHandleType* incIndexPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
    PduInfoType pduInfo;
    bufIndex =
        ((IpduM_uintx)containerTxCfgPtr->IpduMBufIndex
         + ((IpduM_uintx)innerContainerPtr->transReadPoint
            * (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen));
    for (index = incIndexPtr->containedStartPos;
         index < ((IpduM_uintx)incIndexPtr->containedStartPos + (IpduM_uintx)incIndexPtr->containedCnt);
         index++)
    {
        containedIndex = IpduM_InnerContainedIndexRecode[index];
        if (IPDUM_CONTAINED_TX_NUMBER_MAX > containedIndex)
        {
            containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
            if (IPDUM_COLLECT_LAST_IS_BEST == containedTxCfgPtr->IpduMContainedTxPduCollectionSemantics)
            {
                /* @SWS_IpduM_00220,IPDUM_COLLECT_LAST_IS_BEST
                call PduR_IpduMTriggerTransmit */
                SchM_Enter_IpduM_Context();
                pduInfo.SduDataPtr = IpduM_PduBuf;
                pduInfo.SduLength = containedTxCfgPtr->IpduMContainedTxPduRef->PduLen;
                SchM_Exit_IpduM_Context();
                ret = PduR_IpduMTriggerTransmit(containedTxCfgPtr->IpduMContainedTxPduRef->PduId, &pduInfo);
                if ((E_OK != ret) || (pduInfo.SduLength != containedTxCfgPtr->IpduMContainedTxPduRef->PduLen))
                {
                    /* @SWS_IpduM_00222
                    delete contained in container */
                    pduInfo.SduLength = (PduLengthType)IpduM_ContainedDeleteInBuf(
                        containerIndex,
                        containedTxCfgPtr,
                        bufIndex,
                        incIndexPtr->payloadSize);
                    IpduM_InnerContainedIndexRecode[index] = IPDUM_UNUSED_UINT8;
                    incIndexPtr->payloadSize -= (uint16)pduInfo.SduLength;
                }
                else
                {
                    /* copy contained to container */
                    IpduM_ContainedFillToContainer(containedTxCfgPtr, bufIndex, &pduInfo);
                    /* jump header len */
                    bufIndex += 4u;
                    if (IPDUM_HEADERTYPE_LONG == containerTxCfgPtr->IpduMContainerHeaderSize)
                    {
                        bufIndex += 4u;
                    }
                    /* buf position clac offset */
                    bufIndex += containedTxCfgPtr->IpduMContainedTxPduRef->PduLen;
                }
            }
            else
            {
                /* jump header len */
                bufIndex += 4u;
                if (IPDUM_HEADERTYPE_LONG == containerTxCfgPtr->IpduMContainerHeaderSize)
                {
                    bufIndex += 4u;
                }
                /* buf position clac offset */
                bufIndex += containedTxCfgPtr->IpduMContainedTxPduRef->PduLen;
            }
        }
    }
    return E_OK;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* delete contained in container queue buf */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(uint16, IPDUM_CODE) IpduM_ContainedDeleteInBuf(
    IpduM_uintx containerIndex,
    P2CONST(IpduMContainedTxPduType, AUTOMATIC, IPDUM_APPL_DATA) containedTxPtr,
    IpduM_uintx bufIndex,
    uint16 txQueueLen)
{
    const IpduMContainerTxPduType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
    const IpduM_InnerContainerTxType* innerContainerPtr = &IpduM_InnerContainerTx[containerIndex];
    IpduM_uintx startPos =
        ((IpduM_uintx)containerTxCfgPtr->IpduMBufIndex
         + ((IpduM_uintx)innerContainerPtr->transReadPoint
            * (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxPduRef->PduLen));
    IpduM_uintx copyLen;
    SchM_Enter_IpduM_Context();
    uint8* dtaBuf = &IpduM_DtaBuf[bufIndex];
    SchM_Exit_IpduM_Context();
    uint16 deleteLen = 0x4u + containedTxPtr->IpduMContainedTxPduRef->PduLen;
    IpduM_uintx index;
    if (IPDUM_HEADERTYPE_LONG == containerTxCfgPtr->IpduMContainerHeaderSize)
    {
        deleteLen += 0x4u;
    }
    copyLen = (txQueueLen - (bufIndex - startPos + deleteLen));
    SchM_Enter_IpduM_Context();
    for (index = 0u; (index < copyLen); index++)
    {
        dtaBuf[index] = dtaBuf[index + deleteLen];
    }
    SchM_Exit_IpduM_Context();
    return deleteLen;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINED_TX_NUMBER_MAX > 0u)
/* fill contained data to container buf */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(void, IPDUM_CODE) IpduM_ContainedFillToContainer(
    P2CONST(IpduMContainedTxPduType, AUTOMATIC, IPDUM_APPL_DATA) containedTxPtr,
    IpduM_uintx bufIndex,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
    const IpduMContainerTxPduType* containerTxPtr =
        &IpduM_CfgPtr->IpduMContainerTxPduPtr[containedTxPtr->ContainedTxInContainerPduRef];
    SchM_Enter_IpduM_Context();
    uint8* destBufPtr = &IpduM_DtaBuf[bufIndex];
    uint32 headerId = containedTxPtr->IpduMContainedPduHeaderId;
    uint32 headerDlc = containedTxPtr->IpduMContainedTxPduRef->PduLen;
    IpduM_uintx index;
/* IPDUM_BYTE_ORDER_BIG_ENDIAN:
0x11223344
0x11 0x22 0x33 0x44
low ----------> high
*/
/* little_endian:
 0x11223344
 0x44 0x33 0x22 0x11
 low ----------> high
 */
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
    /* fill header ID */
    /* @SWS_IpduM_00177,@SWS_IpduM_00178
    filed in the byte order determined by this parameters */
    if (IPDUM_HEADERTYPE_LONG == containerTxPtr->IpduMContainerHeaderSize)
    {
        *destBufPtr = (uint8)(headerId >> 24u);
        destBufPtr++;
    }
    *destBufPtr = (uint8)(headerId >> 16u);
    destBufPtr++;
    *destBufPtr = (uint8)(headerId >> 8u);
    destBufPtr++;
    *destBufPtr = (uint8)(headerId);
    destBufPtr++;
    /* fill header dlc */
    if (IPDUM_HEADERTYPE_LONG == containerTxPtr->IpduMContainerHeaderSize)
    {
        *destBufPtr = (uint8)(headerDlc >> 24u);
        destBufPtr++;
        *destBufPtr = (uint8)(headerDlc >> 16u);
        destBufPtr++;
        *destBufPtr = (uint8)(headerDlc >> 8u);
        destBufPtr++;
    }
    *destBufPtr = (uint8)(headerDlc);
    destBufPtr++;
#else
    /* fill header ID */
    *destBufPtr = (uint8)(headerId);
    destBufPtr++;
    *destBufPtr = (uint8)(headerId >> 8u);
    destBufPtr++;
    *destBufPtr = (uint8)(headerId >> 16u);
    destBufPtr++;
    if (IPDUM_HEADERTYPE_LONG == containerTxPtr->IpduMContainerHeaderSize)
    {
        *destBufPtr = (uint8)(headerId >> 24u);
        destBufPtr++;
    }
    /* fill header dlc */
    *destBufPtr = (uint8)(headerDlc);
    destBufPtr++;
    if (IPDUM_HEADERTYPE_LONG == containerTxPtr->IpduMContainerHeaderSize)
    {
        *destBufPtr = (uint8)(headerDlc >> 8u);
        destBufPtr++;
        *destBufPtr = (uint8)(headerDlc >> 16u);
        destBufPtr++;
        *destBufPtr = (uint8)(headerDlc >> 24u);
        destBufPtr++;
    }
#endif /* IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER */
    for (index = 0u; (index < PduInfoPtr->SduLength); index++)
    {
        destBufPtr[index] = PduInfoPtr->SduDataPtr[index];
    }
    SchM_Exit_IpduM_Context();
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINED_TX_NUMBER_MAX >0u */

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* container tx confirmation handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
static FUNC(boolean, IPDUM_CODE) IpduM_ContainerCheckQueueIsExit(IpduM_uintx containerIndex)
{
    boolean findFlg = FALSE;
    const IpduMContainerTxPduType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
    IpduM_InnerContainerTxType* innerContainerPtr = &IpduM_InnerContainerTx[containerIndex];
    IpduM_uintx queueIndex;
    IpduM_InnerContainerTxQueueHandleType* incIndexPtr;
    /* force inner tx conf handle */
    if ((IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
#if (STD_ON == IPUDM_TRIG_CONTAINER_SUPPORT_TXCONF)
        || (IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
#endif /* STD_ON == IPUDM_TRIG_SUPPORT_TXCONF */
    )
    {
        queueIndex = (IpduM_uintx)innerContainerPtr->txCofReadPoint
                     + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
        incIndexPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
        /* reset payload size of the container */
        incIndexPtr->payloadSize = 0x0u;
        incIndexPtr->containedStartPos = (uint8)(containerTxCfgPtr->IpduMContainerIncContainedStartPosIndex);
        incIndexPtr->containedCnt = 0x0u;
        /* queue handle */
        if (0u < innerContainerPtr->queueCnt)
        {
            innerContainerPtr->txCofReadPoint++;
            if (containerTxCfgPtr->IpduMContainerQueueSize <= innerContainerPtr->txCofReadPoint)
            {
                innerContainerPtr->txCofReadPoint = 0x0u;
            }
            innerContainerPtr->queueCnt--;
        }
    }
    /* check queue exit not send container */
    queueIndex = (IpduM_uintx)innerContainerPtr->transReadPoint
                 + (IpduM_uintx)containerTxCfgPtr->IpduMContainerTxQueueStartPosIndex;
    incIndexPtr = &IpduM_InnerContainerTxQueueRecode[queueIndex];
    if ((0u < incIndexPtr->payloadSize) && (0u < innerContainerPtr->queueCnt))
    {
        findFlg = TRUE;
    }
    return findFlg;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif /* IPDUM_CONTAINER_TX_NUMBER_MAX >0u */
