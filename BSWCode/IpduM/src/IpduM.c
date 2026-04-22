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
 **  @file               : IpduM.c
 **  @author             : darren.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : I-PDU Multiplexer
 **
 ***********************************************************************************************************************/
/* PRQA S 2985 EOF */ /* VL_IpduM_RedundantOperation */
/* PRQA S 2905 EOF */ /* VL_IpduM_2905 */
/* PRQA S 4461 EOF */ /* VL_IpduM_4461 */
/* =================================================== inclusions =================================================== */
#include "IpduM_Cbk.h"
#include "IpduM_Cfg.h"
#include "IpduM.h"
#include "IStdLib.h"
#include "SchM_IpduM.h"
#if (STD_ON == IPDUM_RX_DIRECT_COM_INVOCATION)
#include "Com_Cbk.h"
#endif
#include "Det.h"
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */

#if !defined(IPDUM_LOCAL_INLINE)
#define IPDUM_LOCAL_INLINE static inline
#endif

#if !defined(IPDUM_LOCAL)
#define IPDUM_LOCAL static /* PRQA S 3414 */ /* VL_IpduM_MacroDefineSpecifierKeyword */
#endif

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
#endif

#define IPDUM_UNUSED_CONTAINEDTX_INDEX ((PduIdType)(0xFFFFu))
#define IPDUM_UNUSED_UINT8             (255u)
#define IPDUM_BITSHIFT_THREE           (3u)
#define IPDUM_SHOT_HEADER_ID_SIZE      (3u)
#define IPDUM_SHOT_HEADER_DLC_SIZE     (1u)
#define IPDUM_LONG_HEADER_ID_SIZE      (4u)
#define IPDUM_LONG_HEADER_DLC_SIZE     (4u)
#define IPDUM_MAGIC_NUM_SEVEN          (7u)
#define IPDUM_MAGIC_NUM_EIGHT          (8u)
#define IPDUM_MAGIC_NUM_FIFTEEN        (15u)
#define IPDUM_MAGIC_NUM_SIXTEEN        (16u)
#define IPDUM_UINT8_MAX                ((uint8)0xFFu)
#define IPDUM_UINT16_DIVIDED_BY_EIGHT  (0xFFF8u)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/* PRQA S 1503,1532 ++*/       /* VL_QAC_NoUsedApi,  VL_QAC_OneFunRef */
/* PRQA S 6040,6030,6070 ++ */ /* VL_MTR_IpduM_QacSTPAR,VL_MTR_IpduM_QacSTMIF,VL_MTR_IpduM_QacSTCAL */
/* PRQA S 6520 EOF */          /* VL_MTR_IpduM_QacSTVAR */

#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
/* transmit part of multiplexed pdu */
IPDUM_LOCAL Std_ReturnType IpduM_TransmitPartOfMultiplex(PduIdType PduIndex, const PduInfoType* PduInfoPtr);

/* Multiplexer pdu data info handle by segment */
IPDUM_LOCAL Std_ReturnType IpduM_SegmentCopy(const IpduM_PduTranslateType* pduTransPtr);

#if (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0)
/* Multiplexer tx check Jit part to assemble pdu */
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexTxCheckJitUpdate(
    const IpduM_TxRequestCfgType*   txRequestPtr,
    const IpduM_InnerTxRequtstType* innerTxReqPtr,
    uint8                           triggerPartType);
#endif

/* Multiplexer tx request transmit */
IPDUM_LOCAL Std_ReturnType
    IpduM_MultiplexTxSendPdu(const IpduM_TxRequestCfgType* txRequestPtr, IpduM_InnerTxRequtstType* innerTxReqPtr);

/* handle trigger tansmit request of a multiplexed pdu */
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexedPduTriggerTransmit(
    const IpduM_TxRequestCfgType* txRequestPtr,
    const PduIdType               txRequestIndex,
    PduInfoType*                  PduInfoPtr);

/* Multiplexer handle tx confirmation */
IPDUM_LOCAL void IpduM_TxConfirmOfMultiplex(PduIdType txRequestIndex, Std_ReturnType result);

/* MultiplexedTx Init */
IPDUM_LOCAL void IpduM_MultiplexedTxInit(IpduM_PartitionIDType ipduMPartition);
#endif

#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
/* Multiplexer rx indiation handle */
IPDUM_LOCAL void IpduM_MultiplexRxIndication(PduIdType rxIndIndex, const PduInfoType* pduInfoPtr);

/* Multiplexer RX indiation pdu find select value */
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexFindSelectVal(
    IpduM_ByteOrderType byteOrder,
    const PduInfoType*  pduInfoPtr,
    uint16              SelectorFieldPosition,
    uint8               SelectorFieldLength,
    uint16*             selcetValPtr);

#endif

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)

/* this function handle in api IpduM_Transmit,shall handle contained for
upper layer data */
IPDUM_LOCAL Std_ReturnType IpduM_TransmitContained(PduIdType containedIndex, const PduInfoType* PduInfoPtr);

/* handle txRequest of a LAST IS BEST containedTxPdu */
IPDUM_LOCAL Std_ReturnType IpduM_ContainedTxLastIsBestInsertToList(
    PduIdType                          containedIndex,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    const PduInfoType*                 PduInfoPtr);

/* convert relative postion in a instance to a global position of a buffer where the instance is located in */
IPDUM_LOCAL_INLINE PduLengthType IpduM_ConverIndexInQueueToLocalPosition(
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    PduLengthType                      indexInQueue);

/* insert index of LAST IS BEST containedTxPdu to instance of its containerTxPdu */
IPDUM_LOCAL Std_ReturnType IpduM_InsertContainedToInstanceInOrder(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    PduIdType                           containedIndex,
    const IpduM_PartitionIDType         ipduMPartition,
    const PduLengthType                 pduLength);

#if (IPDUM_CONTAINEDTXPDU_PRIORITY_ENABLE == STD_ON)
/* try to get priority of a containedTxPdu */
IPDUM_LOCAL_INLINE uint8 IpduM_GetContainedPduPriority(PduIdType containedIndex);
#endif

/* trigger a contaienrTxPdu */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxTriggeredOrQueued(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInputInstancePtr,
    uint8                               transmissionMethod,
    boolean                             parentLockThisInstance);

/* judge whether a send call need to be called rigthnow when container is triggered */
IPDUM_LOCAL boolean IpduM_ContainerTxTriggeredRequestSendNow(
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    uint8                              transmissionMethod);

/* request send a containerTxPdu in context of container is triggered */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxTriggerSendContainer(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr);

/*Assemble static or dynamic containerTxPdu */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxAssemble(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr);

/* assemble containedTxPdu which collectSemantic is last is best */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxLastIsBestContainedAssemble(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    const IpduM_ContainedTxPduCfgType*  containedTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    uint8                               headerSize,
    PduIdType                           indexInInstance,
    PduIdType*                          containdIndexBufPtr);

/* put a header in front of a containedTxPdu when it is put into a containerTxPdu */
IPDUM_LOCAL void IpduM_FillContainedHeaderInContainer(uint8 headerSizeCfg, uint8* dataBuf, uint32 idVal, uint32 dlcVal);

/* update instance information of a static containerTxPdu */
IPDUM_LOCAL void IpduM_UpdateStaticContainerTxInstance(
    const IpduM_ContainedTxPduCfgType*  containedTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    const PduLengthType                 pduLength);

/* this function is try to send a container instance */
IPDUM_LOCAL Std_ReturnType IpduM_TransmitContainerInstance(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInputInstancePtr,
    boolean                             parentLockThisInstance);

/* handle ContainerTx transmission request */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxHandleSendPdu(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr);

/* this function is try to send  data of a instance to PduR or fill a requested buffer */
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxSendPdu(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    boolean*                            ReqDropInstancePtr);

/* handle txRequest of a queue collected containedTxPdu */
IPDUM_LOCAL Std_ReturnType IpduM_ContainedTxCollectQueuedAdd(
    PduIdType                          containedIndex,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    const PduInfoType*                 PduInfoPtr);

/* try to get a new write instance for a containerTxPdu and lock it */
IPDUM_LOCAL boolean IpduM_ContainerTxGetNewWriteInstance(
    const IpduM_PartitionIDType        ipduMPartition,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr);

/* whether a containerTxPdu is triggered by send request of a containedTxPdu */
IPDUM_LOCAL uint8 IpduM_ContainerTxIsTriggerByContained(
    const IpduM_ContainedTxPduCfgType* containedTxCfgPtr,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    const IpduM_InnerContainerTxType*  innerContainerPtr);

/* container tx confirmation handle */
IPDUM_LOCAL void IpduM_TxConfirmOfContainer(PduIdType containerIndex, Std_ReturnType result);

/* container tx i-pdu handle */
IPDUM_LOCAL void IpduM_ContainerTxHandle(PduIdType containerIndex);

/* release instance of containerTxPdu */
IPDUM_LOCAL void IpduM_ContainerTxReleaseTxInstance(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr);

/* whether instance is triggered before  */
IPDUM_LOCAL void IpduM_ContainerCheckTxInstanceIsOverwrite(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerTxPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr);

/* Init containerTxPud */
IPDUM_LOCAL void IpduM_ContainerTxPduInit(IpduM_PartitionIDType ipduMPartition);
#endif

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)

/* in rx indiation handle container */
IPDUM_LOCAL void IpduM_ContainerRxIndication(PduIdType containerIndex, const PduInfoType* PduInfoPtr);

/* get ID field size in header */
IPDUM_LOCAL uint8 IpduM_GetContaiedIDSize(uint8 headerSize);

/* get DLC field size in header */
IPDUM_LOCAL uint8 IpduM_GetContaiedDLCSize(uint8 headerSize);

/* check Queue of a ContainerRxPdu is full or not*/
IPDUM_LOCAL_INLINE void IpduM_ContainerCheckRxInstanceIsOverwrite(
    const IpduM_ContainerRxPduCfgType*  containerRxCfgPtr,
    IpduM_InnerContainerRxType*         innerContainerRxPtr,
    IpduM_InnerContainerRxInstanceType* rxInstancePtr);

/* container rx i-pdu handle */
IPDUM_LOCAL void IpduM_ContainerRxExtractInstance(
    const IpduM_ContainerRxPduCfgType*  containerRxCfgPtr,
    IpduM_InnerContainerRxInstanceType* rxContainerInstancePtr);

/* extract containedRxPdu in a static containerRxPdu */
IPDUM_LOCAL void IpduM_ContainerRxExtractStatic(
    const IpduM_ContainerRxPduCfgType*        containerRxCfgPtr,
    const IpduM_InnerContainerRxInstanceType* rxInstancePtr);

/* extract containedRxPdu in a dynamic containerRxPdu */
IPDUM_LOCAL void IpduM_ContainerRxExtractDynamic(
    const IpduM_ContainerRxPduCfgType*        containerRxCfgPtr,
    const IpduM_InnerContainerRxInstanceType* rxInstancePtr);

/* get header information of containedRxPdu */
IPDUM_LOCAL void IpduM_ContainerRxGetHeadInfomation(
    const uint8* dataBuffer,
    uint32*      headerIdPtr,
    uint32*      headerDlcPtr,
    uint8        headerSize);

/* Init containerRxPdu */
IPDUM_LOCAL void IpduM_ContainerRxPduInit(IpduM_PartitionIDType ipduMPartition);
#endif

/* convert partition index in EcuC to index of IpduM */
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
IPDUM_LOCAL_INLINE IpduM_PartitionIDType IpduM_GetIpduMPartionByECUCParttion(IpduM_PartitionIDType ecuCPartition);
#endif

/* set section bits mask */
IPDUM_LOCAL_INLINE void IpduM_SetBitInSection(uint8* data, uint8 startBit, uint8 endBit);

/* extract section bit */
IPDUM_LOCAL_INLINE void IpduM_ExtractBitInSection(uint8* data, uint8 startBit, uint8 endBit);

#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
/* report develop error to Det */
IPDUM_LOCAL_INLINE void IpduM_DetReportError(uint8 instanceId, uint8 apiId, uint8 errorId);

/* report runtime error to Det */
IPDUM_LOCAL_INLINE void IpduM_DetReportRuntimeError(uint8 instanceId, uint8 apiId, uint8 errorId);
#endif

/* get start index of containerTxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerTxPduInPartition(uint8 ipduMPartitionId);

/* get end index of containerTxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfContainerTxPduInPartition(uint8 ipduMPartitionId);

/* get start index of containerRxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerRxPduInPartition(uint8 ipduMPartitionId);

/* get end index of containerRxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfContainerRxPduInPartition(uint8 ipduMPartitionId);

/* get start index of txRequest on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfTxRequestPduInPartition(uint8 ipduMPartitionId);

/* get end index of txRequest on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfTxRequestPduInPartition(uint8 ipduMPartitionId);

/* ============================================ internal data definitions =========================================== */
/*PB configuration pointer*/
#define IPDUM_START_SEC_VAR_CLEARED_PTR
#include "IpduM_MemMap.h"
IPDUM_LOCAL const IpduM_ConfigType* IpduM_CfgPtr;
#define IPDUM_STOP_SEC_VAR_CLEARED_PTR
#include "IpduM_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

/**
 * @brief         Initializes the I-PDU Multiplexer.
 * @param[in]     config : Pointer to the AUTOSAR IpduM module's configuration data.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-60204
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
void IpduM_Init(const IpduM_ConfigType* config)
{
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ecuCPartition  = (IpduM_PartitionIDType)GetApplicationID();
    IpduM_PartitionIDType ipduMPartition = IpduM_GetIpduMPartionByECUCParttion(ecuCPartition);
#else
    IpduM_PartitionIDType ipduMPartition = 0u;
#endif
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    boolean errFlg = FALSE;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    if (IPDUM_UNUSED_PARTITION_ID == ipduMPartition)
    {
        IpduM_DetReportError(ecuCPartition, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
    }
    else
#endif
    {
#if (STD_ON == IPDUM_REINIT_CHECK)
        if (1u == IpduM_InitStatus[ipduMPartition])
        {
            errFlg = TRUE;
            IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_INIT, IPDUM_E_INIT_FAILED);
        }
#endif
        if (NULL_PTR == config)
        {
            errFlg = TRUE;
            IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_INIT, IPDUM_E_PARAM_POINTER);
        }
        if (!errFlg)
#endif
        {
            /* @SWS_IpduM_00033: initalize all Global Variable */
            IpduM_CfgPtr = config;
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
            IpduM_MultiplexedTxInit(ipduMPartition);
#endif

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
            IpduM_ContainerTxPduInit(ipduMPartition);
#endif

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
            IpduM_ContainerRxPduInit(ipduMPartition);
#endif
            IpduM_InitStatus[ipduMPartition] = 1u;
        }
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    }
#endif
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#if (STD_ON == IPDUM_VERSION_INFO_API)
/**
 * @brief         Service returns the version information of this module.
 * @param[in]     versioninfo : Pointer to where to store the version.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-60210
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
void IpduM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_GETVERSIONINFO, IPDUM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->moduleID         = IPDUM_MODULE_ID;
        versioninfo->sw_major_version = IPDUM_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = IPDUM_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = IPDUM_C_SW_PATCH_VERSION;
        versioninfo->vendorID         = IPDUM_VENDOR_ID;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif

/**
 * @brief         Service is called by the PDU-Router to request a transmission.
 * @param[in]     TxPduId : ID of I-PDU to be transmitted.
 * @param[in]     PduInfoPtr : A pointer to a structure with I-PDU related
 *                data that shall be transmitted: data length and pointer to I-SDU buffer
 * @reentrant     Non Reentrant for the same PDU-ID. Reentrant for different PDU-ID.
 * @synchronous   TRUE
 * @trace         CPD-60203
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
Std_ReturnType IpduM_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType ret        = E_NOT_OK;
    PduIdType      cfgTxPduId = TxPduId;
#if (IPDUM_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (TxPduId < IPDUM_MAX_FOR_ALL_VARIANTS_FROM_UP_TX_PDU_NUM)
    {
        cfgTxPduId = IpduM_CfgPtr->ReceivedUpTxPduMacroMap[TxPduId];
    }
#endif
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ecuCPartition  = (IpduM_PartitionIDType)GetApplicationID();
    IpduM_PartitionIDType ipduMPartition = IpduM_GetIpduMPartionByECUCParttion(ecuCPartition);
    if (IPDUM_UNUSED_PARTITION_ID == ipduMPartition)
    {
        IpduM_DetReportError(ecuCPartition, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
    }
    else if (0u == IpduM_InitStatus[ipduMPartition])
#else
    if (0u == IpduM_InitStatus[0u])
#endif
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_UNINIT);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM_POINTER);
    }
    else if ((0u == PduInfoPtr->SduLength) || (IpduM_CfgPtr->TxUpPduNumInCurrentVariant <= cfgTxPduId))
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_PARAM);
    }
    else
#endif
    {
        if (cfgTxPduId < IpduM_CfgPtr->TxContainedNumInCurrentVariant)
        {
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
            /* IPDUM_TRANSMIT_TYPE_CONTAINED */
#if ((STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED))
            const PduIdType containerIndex =
                IpduM_CfgPtr->IpduMContainedTxPduPtr[cfgTxPduId].ContainerIndexRefByContained;
            const IpduM_ContainerTxPduCfgType* containerTxCfgPtr =
                &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
            if (containerTxCfgPtr->IpduMContainerTxPartition != ipduMPartition)
            {
                IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
            }
            else
#endif
            {
                ret = IpduM_TransmitContained(cfgTxPduId, PduInfoPtr);
            }
#endif
        }
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
        else
        {
            PduIdType partIndex = (cfgTxPduId - IpduM_CfgPtr->TxContainedNumInCurrentVariant);
#if ((STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED))
            const IpduM_TxPartType*       txPartPtr    = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[partIndex];
            const IpduM_TxRequestCfgType* txRequestPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txPartPtr->TxRequestRefIndex];
            if (txRequestPtr->IpduMTxRequestPartition != ipduMPartition)
            {
                IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
            }
            else
#endif
            {
                ret = IpduM_TransmitPartOfMultiplex(partIndex, PduInfoPtr);
            }
        }
#endif
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/*Call-back notifications*/

/**
 * @brief         Indication of a received I-PDU from a lower layer communication interface module.
 * @param[in]     RxPduId : ID of the received I-PDU.
 * @param[in]     PduInfoPtr : Contains the length of the received I-PDU and a pointer to a buffer (SduDataPtr)
 * containing the I-PDU.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60207
 */
#define IPDUM_START_SEC_CODE_FAST
#include "IpduM_MemMap.h"
void IpduM_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    PduIdType cfgRxPduId = RxPduId;
#if (IPDUM_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (RxPduId < IPDUM_MAX_FOR_ALL_VARIANTS_RX_LOWER_LAYER_PDU_NUM)
    {
        cfgRxPduId = IpduM_CfgPtr->RxLowerLayerPduMacroMap[RxPduId];
    }
#endif
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ecuCPartition  = (IpduM_PartitionIDType)GetApplicationID();
    IpduM_PartitionIDType ipduMPartition = IpduM_GetIpduMPartionByECUCParttion(ecuCPartition);
    if (IPDUM_UNUSED_PARTITION_ID == ipduMPartition)
    {
        IpduM_DetReportError(ecuCPartition, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
    }
    else if (0u == IpduM_InitStatus[ipduMPartition])
#else
    if (0u == IpduM_InitStatus[0u])
#endif
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_UNINIT);
    }
    else if (
        (IpduM_CfgPtr->RxIndicationNumInCurrentVariant + IpduM_CfgPtr->RxContainerNumInCurrentVariant) <= cfgRxPduId)
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (cfgRxPduId < IpduM_CfgPtr->RxContainerNumInCurrentVariant)
        {
#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
            /* IPDUM_RXIND_TYPE_CONTAINER */
            IpduM_ContainerRxIndication(cfgRxPduId, PduInfoPtr);
#endif
        }
#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
        else
        {
            IpduM_MultiplexRxIndication((cfgRxPduId - IpduM_CfgPtr->RxContainerNumInCurrentVariant), PduInfoPtr);
        }
#endif
    }
    return;
}
#define IPDUM_STOP_SEC_CODE_FAST
#include "IpduM_MemMap.h"

/**
 * @brief         The lower layer communication interface module confirms the transmission of an I-PDU
 * @param[in]     TxPduId : ID of the I-PDU that has been transmitted
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60205
 */
#define IPDUM_START_SEC_CODE_FAST
#include "IpduM_MemMap.h"
void IpduM_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    PduIdType cfgTxPduId = TxPduId;
#if (IPDUM_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (TxPduId < IPDUM_MAX_FOR_ALL_VARIANTS_TX_PDU_TO_LOWER_LAYER_NUM)
    {
        cfgTxPduId = IpduM_CfgPtr->TxPduToLowerLayerMacroMap[TxPduId];
    }
#endif
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ecuCPartition  = (IpduM_PartitionIDType)GetApplicationID();
    IpduM_PartitionIDType ipduMPartition = IpduM_GetIpduMPartionByECUCParttion(ecuCPartition);
    if (IPDUM_UNUSED_PARTITION_ID == ipduMPartition)
    {
        IpduM_DetReportError(ecuCPartition, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
    }
    else if (0u == IpduM_InitStatus[ipduMPartition])
#else
    if (0u == IpduM_InitStatus[0u])
#endif
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_UNINIT);
    }
    else if ((IpduM_CfgPtr->TxRequestNumInCurrentVariant + IpduM_CfgPtr->TxContainerNumInCurrentVariant) <= cfgTxPduId)
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TXCONFIRMATION, IPDUM_E_PARAM);
    }
    else
#endif
    {
        if (IpduM_CfgPtr->TxContainerNumInCurrentVariant > cfgTxPduId)
        {
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
            /* container PDU */
            IpduM_TxConfirmOfContainer(cfgTxPduId, result);
#endif
        }
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
        else
        {
            /* multiplexed PDU */
            IpduM_TxConfirmOfMultiplex((cfgTxPduId - IpduM_CfgPtr->TxContainerNumInCurrentVariant), result);
        }
#endif
    }
    return;
}
#define IPDUM_STOP_SEC_CODE_FAST
#include "IpduM_MemMap.h"

/**
 * @brief         The lower layer communication interface module confirms the transmission of an I-PDU
 * @param[in]     TxPduId : ID of the I-PDU that has been transmitted
 * @param[in/out] PduInfoPtr : Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                             data shall be copied, and the available buffer size in SduLengh.
 *                             On return, the service will indicate the length of the copied SDU
 *                             data in SduLength.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-60206
 */
#define IPDUM_START_SEC_CODE_FAST
#include "IpduM_MemMap.h"
Std_ReturnType IpduM_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    Std_ReturnType        ret            = E_NOT_OK;
    IpduM_PartitionIDType ipduMPartition = 0u;
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ecuCPartition = (IpduM_PartitionIDType)GetApplicationID();
    ipduMPartition                      = IpduM_GetIpduMPartionByECUCParttion(ecuCPartition);
    if (IPDUM_UNUSED_PARTITION_ID == ipduMPartition)
    {
        IpduM_DetReportError(ecuCPartition, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_INVALID_PARTITION);
    }
    else if (0u == IpduM_InitStatus[ipduMPartition])
#else
    if (0u == IpduM_InitStatus[0u])
#endif
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_UNINIT);
    }
    else if ((IpduM_CfgPtr->TxContainerNumInCurrentVariant + IpduM_CfgPtr->TxRequestNumInCurrentVariant) <= TxPduId)
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        IpduM_DetReportError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRIGGERTRANSMIT, IPDUM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (IpduM_CfgPtr->TxContainerNumInCurrentVariant > TxPduId)
        {
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
            /* IPDUM_TXCONF_TYPE_CONTAINER  */
            /*@SWS_IpduM_00256: for IPDUM_TRIGGERTRANSMIT mode assemble the container
                    in context of the API call IpduM_TriggerTransmit*/
            const IpduM_ContainerTxPduCfgType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[TxPduId];
            boolean                            getLock           = FALSE;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
            ipduMPartition                                  = containerTxCfgPtr->IpduMContainerTxPartition;
            PduIdType                   startOnPartionIndex = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartition);
            PduIdType                   onPartitionIndex    = TxPduId - startOnPartionIndex;
            IpduM_InnerContainerTxType* innerContainerTxPtr =
                &IpduM_ContainerTxDescription[ipduMPartition][onPartitionIndex];
#else
            IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[0u][TxPduId];
#endif
            SchM_Enter_IpduM_Context();
            IpduM_InnerContainerTxInstanceType* txInstancePtr = &IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
                (containerTxCfgPtr->ContainerQueueStartPos) + innerContainerTxPtr->TxReadPoint)];
            if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
            {
                getLock = TRUE;
                txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
            }
            SchM_Exit_IpduM_Context();

            if (getLock)
            {
                ret = IpduM_ContainerTxAssemble(containerTxCfgPtr, txInstancePtr);
                if (E_OK == ret)
                {
                    ret = IpduM_TransmitContainerInstance(
                        containerTxCfgPtr,
                        innerContainerTxPtr,
                        PduInfoPtr,
                        txInstancePtr,
                        getLock);
                }
                txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
            }
#endif
        }
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)
        else
        {
            /* multiplexed pdu */
            const PduIdType               txRequestIndex = TxPduId - IpduM_CfgPtr->TxContainerNumInCurrentVariant;
            const IpduM_TxRequestCfgType* txRequestPtr   = &IpduM_CfgPtr->IpduMTxRequestPtr[txRequestIndex];
            ret = IpduM_MultiplexedPduTriggerTransmit(txRequestPtr, txRequestIndex, PduInfoPtr);
        }
#endif
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE_FAST
#include "IpduM_MemMap.h"

/*
Scheduled functions
*/
/**
 * @brief         Performs the processes of the activities that are not directly
 *                initiated by the calls from PDU-R.
 * @param[in]     ipduMPartitionId : ID of partition where this function will running.
 * @reentrant     Reentrant for different instances. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-60208
 */
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
void IpduM_MainFunctionTx(IpduM_PartitionIDType ipduMPartitionId)
{
#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
    if (1u == IpduM_InitStatus[ipduMPartitionId])
    {
#if ((STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED))
        IpduM_PartitionIDType applicationID = (IpduM_PartitionIDType)GetApplicationID();
        if (ipduMPartitionId != IpduM_GetIpduMPartionByECUCParttion(applicationID))
        {
            IpduM_DetReportError(applicationID, IPDUM_SERVICE_ID_MAINFUNCTIONTX, IPDUM_E_PARAM);
        }
        else
#endif
        {
            /* Check variant configuration */
            if (IpduM_CfgPtr->TxContainerNumInCurrentVariant > 0u)
            {
                /* get range configuration of this partition */
                PduIdType startID = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartitionId);
                PduIdType endID   = IpduM_GetEndOfContainerTxPduInPartition(ipduMPartitionId);
                for (; startID < endID; startID++)
                {
                    IpduM_ContainerTxHandle(startID);
                }
            }
        }
    }
#endif
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif
/*
Scheduled functions
*/
/**
 * @brief         Performs the processes of the activities that are not directly
                  initiated by the calls from PDU-R.
 *                initiated by the calls from PDU-R.
 * @param[in]     ipduMPartitionId : ID of partition where this function will running.
 * @reentrant     Reentrant for different instances. Non reentrant for the same instance.
 * @synchronous   TRUE
 * @trace         CPD-60209
 */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
void IpduM_MainFunctionRx(IpduM_PartitionIDType ipduMPartitionId)
{
#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
    if (1u == IpduM_InitStatus[ipduMPartitionId])
    {
#if ((STD_ON == IPDUM_DEV_ERROR_DETECT) && (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED))
        IpduM_PartitionIDType applicationID = (IpduM_PartitionIDType)GetApplicationID();
        if (ipduMPartitionId != IpduM_GetIpduMPartionByECUCParttion(applicationID))
        {
            IpduM_DetReportError(applicationID, IPDUM_SERVICE_ID_MAINFUNCTIONTX, IPDUM_E_PARAM);
        }
        else
#endif
        {
            /* Check variant configuration */
            if (IpduM_CfgPtr->RxContainerNumInCurrentVariant > 0u)
            {
                /* get range configuration of this partition */
                PduIdType startID = IpduM_GetStartOfContainerRxPduInPartition(ipduMPartitionId);
                PduIdType endID   = IpduM_GetEndOfContainerRxPduInPartition(ipduMPartitionId);
                boolean   getLock;
                for (; startID < endID; startID++)
                {
                    const IpduM_ContainerRxPduCfgType* containerRxCfgPtr =
                        &IpduM_CfgPtr->IpduMContainerRxPduPtr[startID];
                    getLock                              = FALSE;
                    IpduM_PartitionIDType ipduMPartition = 0u;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
                    ipduMPartition                  = containerRxCfgPtr->IpduMContainerRxPartition;
                    PduIdType startOnPartitionIndex = IpduM_GetStartOfContainerRxPduInPartition(ipduMPartition);
                    PduIdType onPartitionIndex      = startID - startOnPartitionIndex;
                    IpduM_InnerContainerRxType* innerContainerRxPtr =
                        &IpduM_ContainerRxDescription[ipduMPartition][onPartitionIndex];
#else
                    IpduM_InnerContainerRxType* innerContainerRxPtr =
                        &IpduM_ContainerRxDescription[ipduMPartition][startID];
#endif
                    SchM_Enter_IpduM_Context();
                    IpduM_InnerContainerRxInstanceType* rxInstancePtr =
                        &IpduM_InnerContainerRxInstanceRecord[ipduMPartition][(
                            (containerRxCfgPtr->ContainerRxQueueStartPos) + innerContainerRxPtr->RxReadPoint)];
                    if (0u == (rxInstancePtr->InstanceState & IPDUM_CONTAINERRX_INSTANCE_LOCKED))
                    {
                        getLock = TRUE;
                        rxInstancePtr->InstanceState |= IPDUM_CONTAINERRX_INSTANCE_LOCKED;
                    }
                    SchM_Exit_IpduM_Context();
                    if (getLock)
                    {
                        if (IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED
                            == (rxInstancePtr->InstanceState & IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED))
                        {
                            IpduM_ContainerRxExtractInstance(containerRxCfgPtr, rxInstancePtr);
                            if (1u < containerRxCfgPtr->IpduMContainerQueueSize)
                            {
                                uint8 raisedReadPoint = (innerContainerRxPtr->RxReadPoint + 1u);
                                /* PRQA S 2834 ++ */ /* VL_IpduM_Division_by_zero */
                                innerContainerRxPtr->RxReadPoint =
                                    (raisedReadPoint % containerRxCfgPtr->IpduMContainerQueueSize);
                                /* PRQA S 2834 -- */
                            }
                        }
                        rxInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERRX_INSTANCE_LOCKED);
                    }
                }
            }
        }
    }
#endif
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (IPDUM_TX_REQUEST_NUMBER_MAX > 0u)

/* ipdum receiver upper layer Multiplexer i-PDU need package
relevant tx request */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_TransmitPartOfMultiplex(PduIdType PduIndex, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType                ret                 = E_NOT_OK;
    const IpduM_TxPartType*       txPartCfgPtr        = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[PduIndex];
    PduIdType                     txRequestIndex      = txPartCfgPtr->TxRequestRefIndex;
    const IpduM_TxRequestCfgType* txRequestPtr        = &IpduM_CfgPtr->IpduMTxRequestPtr[txRequestIndex];
    IpduM_PartitionIDType         ipduMPartitionIndex = 0u;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    ipduMPartitionIndex                             = txRequestPtr->IpduMTxRequestPartition;
    PduIdType                 startOnPartitionIndex = IpduM_GetStartOfTxRequestPduInPartition(ipduMPartitionIndex);
    PduIdType                 onPartitionIndex      = txRequestIndex - startOnPartitionIndex;
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[ipduMPartitionIndex][onPartitionIndex];
#else
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[ipduMPartitionIndex][txRequestIndex];
#endif
    /* @SWS_IpduM_00152
    as long as no transmission confirmation for the I-PDU is received the function IpduM_Transmit
    shall return E_NOT_OK */
    if (innerTxReqPtr->IsNotWaitTxConfirm)
    {
        IpduM_PduTranslateType         pduTrans;
        PduInfoType                    outPdu;
        uint8                          sendPartType;
        IpduM_MultiplexTriggerModeType multiplexTriggerMode = txRequestPtr->IpduMTxTriggerMode;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
        if (IpduM_CfgPtr->TxStaticPartPduNumInCurrentVariant > PduIndex)
        {
            /* static part */
            pduTrans.SegmentCnt = txRequestPtr->TxStaticSegmentCnt;
            pduTrans.SegmentPtr = &IpduM_CfgPtr->IpduMTxRxSegmentPtr[txRequestPtr->IpduMTxStaticSegmentStartIndex];
            sendPartType        = IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_STATIC;
        }
        else
#endif
        {
            /* @SWS_IpduM_00223: store latest dynamic part sent by upper layer */
            innerTxReqPtr->LastTxDynamicPartIndex = PduIndex;
            pduTrans.SegmentCnt                   = txRequestPtr->TxDynamicSegmentCnt;
            pduTrans.SegmentPtr = &IpduM_CfgPtr->IpduMTxRxSegmentPtr[txRequestPtr->IpduMTxDynamicSegmenStartIndex];
            sendPartType        = IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_DYNAMIC;
        }

        if (txPartCfgPtr->PduLen == PduInfoPtr->SduLength)
        {
            /* @SWS_IpduM_00015: merge corresponding I-PDU(part) to tx request */
            outPdu.SduDataPtr = &IpduM_DataBuf[ipduMPartitionIndex][txRequestPtr->BufIndex];

            outPdu.SduLength      = txRequestPtr->PduLen;
            pduTrans.ByteOrder    = txRequestPtr->IpduMByteOrder;
            pduTrans.InputPduPtr  = PduInfoPtr;
            pduTrans.OutputPduPtr = &outPdu;
            if (IPDUM_NONE != multiplexTriggerMode)
            {
                ret = IpduM_SegmentCopy(&pduTrans);
            }
            else
            {
                /* maybe overlaped with IpduM_TriggerTransmit context */
                SchM_Enter_IpduM_Context();
                ret = IpduM_SegmentCopy(&pduTrans);
                SchM_Exit_IpduM_Context();
            }

            /* Check trigger conditions*/
            if (E_OK == ret)
            {
#if (IPDUM_METADATA_SUPPORT == STD_ON)
                /* @SWS_IpduM_00226 IpduMTxTriggerMode is set to None,
                                shall use the MetaData of the last updated part for sending of the multiplexed IPDU */
                if ((IPDUM_NONE == multiplexTriggerMode) && (0u < txRequestPtr->MetaDataSize))
                {
                    SchM_Enter_IpduM_Context();
                    (void)
                        IStdLib_MemCpy(innerTxReqPtr->MetaDataPtr, PduInfoPtr->MetaDataPtr, txRequestPtr->MetaDataSize);
                    SchM_Exit_IpduM_Context();
                }
#endif
                /* @SWS_IpduM_00021:send a transmission request for the new multiplexed I-PDU to
                    the PduR because of trigger conditions. */
                if ((IPDUM_STATIC_OR_DYNAMIC_PART_TRIGGER == multiplexTriggerMode) ||
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
                    ((IPDUM_STATIC_PART_TRIGGER == multiplexTriggerMode)
                     && (IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_STATIC == sendPartType))
                    ||
#endif
                    ((IPDUM_DYNAMIC_PART_TRIGGER == multiplexTriggerMode)
                     && (IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_DYNAMIC == sendPartType)))
                {
#if (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0)
                    /*@SWS_IpduM_171: the multiplexed I-PDU shall not be send if PduR_IpduMTriggerTransmit return
                     * E_NOT_OK.*/
                    ret = IpduM_MultiplexTxCheckJitUpdate(txRequestPtr, innerTxReqPtr, sendPartType);
                    if (E_OK == ret)
#endif
                    {
#if (IPDUM_METADATA_SUPPORT == STD_ON)
                        /* @SWS_IpduM_00225 IpduMTxTriggerMode is not set to None,
                                shall use the MetaData of the triggering part for sending of the multiplexed IPDU */
                        if (0u < txRequestPtr->MetaDataSize)
                        {
                            /* synchronized transmission do not need store metaData */
                            innerTxReqPtr->MetaDataPtr = PduInfoPtr->MetaDataPtr;
                        }
#endif
                        ret = IpduM_MultiplexTxSendPdu(txRequestPtr, innerTxReqPtr);
                    }
                }
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* Multiplexer pdu data info handle by segment */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_SegmentCopy(const IpduM_PduTranslateType* pduTransPtr)
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

    for (uint16 i = 0u; (E_OK == ret) && (i < (pduTransPtr->SegmentCnt)); i++)
    {
        uint16 bitCnt = pduTransPtr->SegmentPtr[i].IpduMSegmentLength;
        uint16 bitPos = pduTransPtr->SegmentPtr[i].IpduMSegmentPosition;

        while ((bitCnt != 0u) && (E_OK == ret))
        {
            uint8 byteBitEndPos, onCoBitCnt;
            uint8 intTmp          = pduTransPtr->InputPduPtr->SduDataPtr[bitPos >> IPDUM_BITSHIFT_THREE];
            uint8 byteBitStartPos = (uint8)(bitPos & IPDUM_MAGIC_NUM_SEVEN);
            /* calculated end copy bit position in byte */
            if (IPDUM_MAGIC_NUM_EIGHT > (byteBitStartPos + bitCnt))
            {
                byteBitEndPos = (uint8)(byteBitStartPos + bitCnt - 1u);
            }
            else
            {
                byteBitEndPos = IPDUM_MAGIC_NUM_SEVEN;
            }
            if ((0u != byteBitStartPos) || (byteBitEndPos != IPDUM_MAGIC_NUM_SEVEN))
            {
                /* bit data copy
                copy start position is greath then now position
                */
                uint8 outTmp = pduTransPtr->OutputPduPtr->SduDataPtr[bitPos >> IPDUM_BITSHIFT_THREE];
                uint8 bitsMask;
                onCoBitCnt = byteBitEndPos - byteBitStartPos + 1u;

                IpduM_SetBitInSection(&bitsMask, byteBitStartPos, byteBitEndPos);
                intTmp &= bitsMask;
                outTmp &= (uint8)(~bitsMask);
                outTmp |= intTmp;
                pduTransPtr->OutputPduPtr->SduDataPtr[bitPos >> IPDUM_BITSHIFT_THREE] = outTmp;
                /* restart calculated bit position */
                if ((byteBitEndPos == IPDUM_MAGIC_NUM_SEVEN) && (IPDUM_BYTE_ORDER_BIG_ENDIAN == pduTransPtr->ByteOrder))
                {
                    if (bitPos >= IPDUM_MAGIC_NUM_EIGHT)
                    {
                        bitPos = (bitPos - IPDUM_MAGIC_NUM_EIGHT) & IPDUM_UINT16_DIVIDED_BY_EIGHT;
                    }
                }
                else
                {
                    bitPos += onCoBitCnt;
                }
            }
            else
            {
                onCoBitCnt = IPDUM_MAGIC_NUM_EIGHT;
                /* byte data copy */
                pduTransPtr->OutputPduPtr->SduDataPtr[bitPos >> IPDUM_BITSHIFT_THREE] = intTmp;
                /* different endian bit position calculated */
                if (IPDUM_BYTE_ORDER_BIG_ENDIAN == pduTransPtr->ByteOrder)
                {
                    if (bitPos >= IPDUM_MAGIC_NUM_EIGHT)
                    {
                        bitPos = bitPos - IPDUM_MAGIC_NUM_EIGHT;
                    }
                }
                else
                {
                    bitPos = bitPos + IPDUM_MAGIC_NUM_EIGHT;
                }
            }
            bitCnt -= onCoBitCnt;
            if ((bitPos > (uint16)(pduTransPtr->OutputPduPtr->SduLength << IPDUM_BITSHIFT_THREE))
                || (bitPos > (uint16)(pduTransPtr->InputPduPtr->SduLength << IPDUM_BITSHIFT_THREE)))
            {
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#if (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0)
/* Multiplexer assemble part pdu to tx request */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexTxCheckJitUpdate(
    const IpduM_TxRequestCfgType*   txRequestPtr,
    const IpduM_InnerTxRequtstType* innerTxReqPtr,
    uint8                           triggerPartType)
{
    Std_ReturnType          ret = E_OK;
    const IpduM_TxPartType* txPartCfgPtr;
    PduInfoType             outPdu;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    const IpduM_PartitionIDType ipduMPartition = txRequestPtr->IpduMTxRequestPartition;
#else
    const IpduM_PartitionIDType ipduMPartition = 0u;
#endif
    /*  @SWS_IpduM_000168: only update second part which IpduMJitUpdate is configured.
        @SWS_IpduM_000169: in IpduM_TriggerTransmit update all parts if IpduMJitUpdate
        configured to true */

#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /* check static part's JIT bit */
    if ((IPDUM_UNUSED_MULTIPLEXE_PART_INDEX != txRequestPtr->IpduMTxStaticPartIndex)
        && (IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_STATIC != triggerPartType))
    {
        txPartCfgPtr = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[txRequestPtr->IpduMTxStaticPartIndex];
        if (txPartCfgPtr->IpduMJitUpdate)
        {
            PduInfoType            localPdu;
            IpduM_PduTranslateType staticPduTrans;
            uint8                  localStaticPartBuff[IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX];
            localPdu.SduDataPtr = localStaticPartBuff;
            localPdu.SduLength  = txPartCfgPtr->PduLen;
            ret                 = PduR_IpduMTriggerTransmit(txPartCfgPtr->PduId, &localPdu);
            if (E_OK == ret)
            {
                outPdu.SduDataPtr           = &IpduM_DataBuf[ipduMPartition][txRequestPtr->BufIndex];
                outPdu.SduLength            = txRequestPtr->PduLen;
                staticPduTrans.ByteOrder    = txRequestPtr->IpduMByteOrder;
                staticPduTrans.InputPduPtr  = &localPdu;
                staticPduTrans.OutputPduPtr = &outPdu;
                staticPduTrans.SegmentCnt   = (txRequestPtr->TxStaticSegmentCnt);
                staticPduTrans.SegmentPtr =
                    &IpduM_CfgPtr->IpduMTxRxSegmentPtr[txRequestPtr->IpduMTxStaticSegmentStartIndex];
                ret = IpduM_SegmentCopy(&staticPduTrans);
            }
        }
    }
#endif

    /* @SWS_IpduM_000223
    dynamic part select latest send by upper layer */
    txPartCfgPtr = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[innerTxReqPtr->LastTxDynamicPartIndex];
    if ((txPartCfgPtr->IpduMJitUpdate)
        && ((IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_STATIC == triggerPartType)
            || ((IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_TRIGGER == triggerPartType) && (E_OK == ret))))
    {
        PduInfoType            localDynamicPdu;
        IpduM_PduTranslateType dynamicPduTrans;
        uint8                  localDynamicPartBuff[IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX];
        localDynamicPdu.SduDataPtr = localDynamicPartBuff;
        localDynamicPdu.SduLength  = txPartCfgPtr->PduLen;
        ret                        = PduR_IpduMTriggerTransmit(txPartCfgPtr->PduId, &localDynamicPdu);
        if (E_OK == ret)
        {
            outPdu.SduDataPtr            = &IpduM_DataBuf[ipduMPartition][txRequestPtr->BufIndex];
            outPdu.SduLength             = txRequestPtr->PduLen;
            dynamicPduTrans.ByteOrder    = txRequestPtr->IpduMByteOrder;
            dynamicPduTrans.InputPduPtr  = &localDynamicPdu;
            dynamicPduTrans.OutputPduPtr = &outPdu;
            dynamicPduTrans.SegmentCnt   = (txRequestPtr->TxDynamicSegmentCnt);
            dynamicPduTrans.SegmentPtr =
                &IpduM_CfgPtr->IpduMTxRxSegmentPtr[txRequestPtr->IpduMTxDynamicSegmenStartIndex];
            ret = IpduM_SegmentCopy(&dynamicPduTrans);
        }
    }
    /* @SWS_IpduM_00172: return E_NOT_OK if any of the JIT-update request via
    PduR_IpduMTriggerTransmit return E_NOT_OK.*/
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif

/* Multiplexer tx request transmit */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType
    IpduM_MultiplexTxSendPdu(const IpduM_TxRequestCfgType* txRequestPtr, IpduM_InnerTxRequtstType* innerTxReqPtr)
{
    Std_ReturnType ret;
    PduInfoType    temporaryPdu;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    temporaryPdu.SduDataPtr = &IpduM_DataBuf[txRequestPtr->IpduMTxRequestPartition][txRequestPtr->BufIndex];
#else
    temporaryPdu.SduDataPtr = &IpduM_DataBuf[0u][txRequestPtr->BufIndex];
#endif
    temporaryPdu.SduLength = txRequestPtr->PduLen;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
    /* @SWS_IpduM_00226 shall use the MetaData of the last updated part for sending of the multiplexed IPDU */
    if (0u < txRequestPtr->MetaDataSize)
    {
        temporaryPdu.MetaDataPtr = innerTxReqPtr->MetaDataPtr;
    }
    else
#endif
    {
        temporaryPdu.MetaDataPtr = NULL_PTR;
    }

    ret = PduR_IpduMTransmit(txRequestPtr->PduId, &temporaryPdu);
    if (E_OK == ret)
    {
        innerTxReqPtr->IsNotWaitTxConfirm = FALSE;
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexedPduTriggerTransmit(
    const IpduM_TxRequestCfgType* txRequestPtr,
    const PduIdType               txRequestIndex,
    PduInfoType*                  PduInfoPtr)
{
    Std_ReturnType ret         = E_NOT_OK;
    PduLengthType  localPduLen = txRequestPtr->PduLen;

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType partitionIndex = txRequestPtr->IpduMTxRequestPartition;
#if (                                                                                              \
    (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0) || (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0) \
    || (IPDUM_METADATA_SUPPORT == STD_ON))
    PduIdType                       startOnPartitionIndex = IpduM_GetStartOfTxRequestPduInPartition(partitionIndex);
    PduIdType                       onPartitionIndex      = txRequestIndex - startOnPartitionIndex;
    const IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[partitionIndex][onPartitionIndex];
#else
    (void)txRequestIndex;
#endif
#else
    const IpduM_PartitionIDType partitionIndex = 0u;
#if ((IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0) || (IPDUM_METADATA_SUPPORT == STD_ON))
    const IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[partitionIndex][txRequestIndex];
#endif
#endif

    /*SWS_IpduM_00060: called module shall check whether the available data fits into the buffer size
        reported by PduInfoPtr->SduLength*/
    if (PduInfoPtr->SduLength >= localPduLen)
    {
#if (IPDUM_MULTIPLEX_JIT_PART_PDU_LENGTH_MAX > 0)
        uint8 triggerPartType = IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_TRIGGER;
        ret                   = IpduM_MultiplexTxCheckJitUpdate(txRequestPtr, innerTxReqPtr, triggerPartType);
        /*@SWS_IpduM_171: the multiplexed I-PDU shall not be send if PduR_IpduMTriggerTransmit return E_NOT_OK.*/
        if (E_OK == ret)
#else
        ret = E_OK;
#endif
        {
            SchM_Enter_IpduM_Context();
            (void)IStdLib_MemCpy(
                PduInfoPtr->SduDataPtr,
                &IpduM_DataBuf[partitionIndex][txRequestPtr->BufIndex],
                localPduLen);
            SchM_Exit_IpduM_Context();
            PduInfoPtr->SduLength = localPduLen;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            /* @SWS_IpduM_00226 IpduMTxTriggerMode is set to None,
                shall use the MetaData of the last updated part for sending of the multiplexed IPDU */
            if (0u < txRequestPtr->MetaDataSize)
            {
                PduInfoPtr->MetaDataPtr = innerTxReqPtr->MetaDataPtr;
            }
            else
#endif
            {
                PduInfoPtr->MetaDataPtr = NULL_PTR;
            }
        }
    }
    IPDUM_NOUSED(txRequestIndex);
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* Multiplexer handle tx confirmation */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_TxConfirmOfMultiplex(PduIdType txRequestIndex, Std_ReturnType result)
{
    /* @SWS_IpduM_00022: shall translate confirmation into the corresponding confirmation for I-pdu
    which were contained in the last sent out multiplexed */
    const IpduM_TxPartType*       txPartCfgPtr;
    const IpduM_TxRequestCfgType* txRequestPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[txRequestIndex];
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType     partitionIndex        = txRequestPtr->IpduMTxRequestPartition;
    PduIdType                 startOnPartitionIndex = IpduM_GetStartOfTxRequestPduInPartition(partitionIndex);
    PduIdType                 onPartitionIndex      = txRequestIndex - startOnPartitionIndex;
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[partitionIndex][onPartitionIndex];
#else
    IpduM_InnerTxRequtstType* innerTxReqPtr = &IpduM_InnerTxRequestDescription[0u][txRequestIndex];
#endif

#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    if (IPDUM_UNUSED_MULTIPLEXE_PART_INDEX != txRequestPtr->IpduMTxStaticPartIndex)
    {
        txPartCfgPtr = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[txRequestPtr->IpduMTxStaticPartIndex];
        if (txPartCfgPtr->IpduMTxConfirmation)
        {
            IPDUM_NotifyTxConfirmation(txPartCfgPtr->PduId, result);
        }
    }
#endif
    txPartCfgPtr = &IpduM_CfgPtr->IpduMMultiplexedPduTxPartPtr[innerTxReqPtr->LastTxDynamicPartIndex];
    if (txPartCfgPtr->IpduMTxConfirmation)
    {
        IPDUM_NotifyTxConfirmation(txPartCfgPtr->PduId, result);
    }
    innerTxReqPtr->IsNotWaitTxConfirm = TRUE;
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* MultiplexerTxPdu Init */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_MultiplexedTxInit(IpduM_PartitionIDType ipduMPartition)
{
    PduIdType startTxRequestID          = IpduM_GetStartOfTxRequestPduInPartition(ipduMPartition);
    PduIdType endTxRequestID            = IpduM_GetEndOfTxRequestPduInPartition(ipduMPartition);
    PduIdType inPartitionTxRequestIndex = 0u;
    for (; startTxRequestID < endTxRequestID; startTxRequestID++)
    {
        const IpduM_TxRequestCfgType* txRequestPtr = &IpduM_CfgPtr->IpduMTxRequestPtr[startTxRequestID];

        /*@SWS_IpduM_00067: initialize its internal transmit buffers*/
        /*@SWS_IpduM_00068: initial dynamic part shall be set to IpduMInitialDynamicPart*/
        /*@SWS_IpduM_00143: initial static part shall be set to IpduMTxStaticPart*/
        PduLengthType loopPos = txRequestPtr->BufIndex;
        (void)IStdLib_MemCpy(
            &IpduM_DataBuf[ipduMPartition][loopPos],
            &IpduM_CfgPtr->TxRequestPdusInitialVal[ipduMPartition][loopPos],
            txRequestPtr->PduLen);
        IpduM_InnerTxRequestDescription[ipduMPartition][inPartitionTxRequestIndex].LastTxDynamicPartIndex =
            txRequestPtr->IndexOfIpduMInitialDynamicPart;
        IpduM_InnerTxRequestDescription[ipduMPartition][inPartitionTxRequestIndex].IsNotWaitTxConfirm = TRUE;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
        if ((IPDUM_NONE == txRequestPtr->IpduMTxTriggerMode) && (0u < txRequestPtr->MetaDataSize))
        {
            IpduM_InnerTxRequestDescription[ipduMPartition][inPartitionTxRequestIndex].MetaDataPtr =
                &IpduM_MetaDataBuf[ipduMPartition][txRequestPtr->MetaDataBufIndex];
        }
        else
        {
            IpduM_InnerTxRequestDescription[ipduMPartition][inPartitionTxRequestIndex].MetaDataPtr = NULL_PTR;
        }
#endif
        inPartitionTxRequestIndex++;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#endif

#if (IPDUM_RX_INDICA_NUMBER_MAX > 0u)
/* Multiplexer rx indiation handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_MultiplexRxIndication(PduIdType rxIndIndex, const PduInfoType* pduInfoPtr)
{
    /* Variable declarations */
    const IpduM_RxIndicationCfgType* rxIndicationCfgPtr = &IpduM_CfgPtr->IpduMRxIndicationPtr[rxIndIndex];
    PduInfoType                      outputPdu;
    outputPdu.SduDataPtr = pduInfoPtr->SduDataPtr;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /*@SWS_IpduM_00041: forwards static part */
    if (IPDUM_UNUSED_MULTIPLEXE_PART_INDEX != rxIndicationCfgPtr->RxStaticPartIndex)
    {
        const IpduM_RxStaticPartType* rxStaticPrtPtr =
            &IpduM_CfgPtr->IpduMRxStaticPartPduPtr[rxIndicationCfgPtr->RxStaticPartIndex];
        outputPdu.SduLength = rxStaticPrtPtr->PduLen;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
        /* @SWS_IpduM_00227: forward the received MeataData */
        outputPdu.MetaDataPtr = pduInfoPtr->MetaDataPtr;
#endif
        IPDUM_NotifyRxIndication(rxStaticPrtPtr->PduId, &outputPdu);
    }
#endif

    /* @SWS_IpduM_00042: forwards Dynamic part */
    const IpduM_RxDynamicPartType* rxDynamicPartPtr;
    uint16                         selcetVal  = 0x0u;
    uint16                         startIndex = rxIndicationCfgPtr->RxDynamicPartStartIndex;
    boolean                        findFlag   = FALSE;
    /* find selcetor value */
    Std_ReturnType ret = IpduM_MultiplexFindSelectVal(
        rxIndicationCfgPtr->IpduMByteOrder,
        pduInfoPtr,
        rxIndicationCfgPtr->IpduMSelectorFieldPosition,
        rxIndicationCfgPtr->IpduMSelectorFieldLength,
        &selcetVal);
    for (PduIdType index = 0u; ((E_OK == ret) && (index < rxIndicationCfgPtr->IpduMRxDynamicPartCnt) && (!findFlag));
         index++)
    {
        rxDynamicPartPtr = &IpduM_CfgPtr->IpduMRxDynamicPartPduPtr[startIndex + index];
        if (selcetVal == rxDynamicPartPtr->IpduMRxSelectorValue)
        {
            findFlag = TRUE;
        }
    }
    if (findFlag)
    {
        outputPdu.SduLength = rxDynamicPartPtr->PduLen;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
        outputPdu.MetaDataPtr = pduInfoPtr->MetaDataPtr;
#endif
        IPDUM_NotifyRxIndication(rxDynamicPartPtr->PduId, &outputPdu);
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* Multiplexer RX indiation pdu find select value */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_MultiplexFindSelectVal(
    IpduM_ByteOrderType byteOrder,
    const PduInfoType*  pduInfoPtr,
    uint16              SelectorFieldPosition,
    uint8               SelectorFieldLength,
    uint16*             selcetValPtr)
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
    Std_ReturnType ret         = E_OK;
    uint8          bitCnt      = SelectorFieldLength;
    uint16         bitPos      = SelectorFieldPosition;
    uint16         outSeletVal = 0x0u;
    while (0x0u != bitCnt)
    {
        uint8 byteBitEndPos, onByteCopyBits;
        uint8 tmp             = pduInfoPtr->SduDataPtr[bitPos >> IPDUM_BITSHIFT_THREE];
        uint8 byteBitStartPos = (uint8)(bitPos & IPDUM_MAGIC_NUM_SEVEN);
        /* calculated end copy bit position in byte */
        if (IPDUM_MAGIC_NUM_EIGHT > (byteBitStartPos + bitCnt))
        {
            byteBitEndPos = byteBitStartPos + bitCnt - 1u;
        }
        else
        {
            byteBitEndPos = IPDUM_MAGIC_NUM_SEVEN;
        }
        if ((0u != byteBitStartPos) || (byteBitEndPos != IPDUM_MAGIC_NUM_SEVEN))
        {
            /* bit data copy: copy start position is greath then now position*/
            IpduM_ExtractBitInSection(&tmp, byteBitStartPos, byteBitEndPos);
            /* restart calculated bit position */
            if ((byteBitEndPos == IPDUM_MAGIC_NUM_SEVEN) && (IPDUM_BYTE_ORDER_BIG_ENDIAN == byteOrder))
            {
                if (bitPos >= IPDUM_MAGIC_NUM_EIGHT)
                {
                    bitPos = (bitPos - IPDUM_MAGIC_NUM_EIGHT) & IPDUM_UINT16_DIVIDED_BY_EIGHT;
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
            onByteCopyBits = IPDUM_MAGIC_NUM_EIGHT;
            if (IPDUM_BYTE_ORDER_BIG_ENDIAN == byteOrder)
            {
                if (bitPos >= IPDUM_MAGIC_NUM_EIGHT)
                {
                    bitPos = bitPos - IPDUM_MAGIC_NUM_EIGHT;
                }
            }
            else
            {
                bitPos = bitPos + IPDUM_MAGIC_NUM_EIGHT;
            }
        }
        bitCnt -= onByteCopyBits;

        if (bitPos > (uint16)((pduInfoPtr->SduLength) << IPDUM_BITSHIFT_THREE))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* all endian ,first find value is lsb */
            outSeletVal >>= (byteBitEndPos - byteBitStartPos + 1u);
            outSeletVal |= (uint16)((uint16)tmp << (IPDUM_MAGIC_NUM_FIFTEEN - (byteBitEndPos - byteBitStartPos)));
        }
    }
    /* remove lsb fill not used data */
    outSeletVal >>= (IPDUM_MAGIC_NUM_SIXTEEN - SelectorFieldLength);
    *selcetValPtr = outSeletVal;
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif

#if (IPDUM_CONTAINER_TX_NUMBER_MAX > 0u)
/* this function handle in api IpduM_Transmit,shall handle contained for
upper layer data */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_TransmitContained(PduIdType containedIndex, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType                     ret               = E_NOT_OK;
    const IpduM_ContainedTxPduCfgType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    PduIdType                          containerIndex    = containedTxCfgPtr->ContainerIndexRefByContained;
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType       ipduMPartition      = containerTxCfgPtr->IpduMContainerTxPartition;
    PduIdType                   startOnPartionIndex = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartition);
    PduIdType                   onPartitionIndex    = containerIndex - startOnPartionIndex;
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[ipduMPartition][onPartitionIndex];
#else
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[0u][containerIndex];
#endif
    if (containerTxCfgPtr->PduLen >= PduInfoPtr->SduLength)
    {
        if (IPDUM_COLLECT_LAST_IS_BEST == containedTxCfgPtr->IpduMContainedTxPduCollectionSemantics)
        {
            /*@SWS_IpduM_00220: shall use PduR_IpduMTriggerTransmit to fetch the PDU data from its upper layyer
            so only insert Index of containedTxPdu and do not copy any data */
            ret = IpduM_ContainedTxLastIsBestInsertToList(
                containedIndex,
                containerTxCfgPtr,
                innerContainerTxPtr,
                PduInfoPtr);
        }
        else
        {
            /* IPDUM_COLLECT_QUEUED */
            /* @SWS_IpduM_00179:
            IPDUM_COLLECT_QUEUED,this contained shall be add to container */
            ret = IpduM_ContainedTxCollectQueuedAdd(containedIndex, containerTxCfgPtr, innerContainerTxPtr, PduInfoPtr);
        }
        if (E_OK == ret)
        {
            if ((0u == innerContainerTxPtr->TransmissionTimer)
                && ((0u != containedTxCfgPtr->IpduMContainedTxPduSendTimeout)
                    || (0u != containerTxCfgPtr->IpduMContainerTxSendTimeout)))
            {
                /*@SWS_IpduM_00184:start the transmissing timer of the container PDU. */
                if (((containedTxCfgPtr->IpduMContainedTxPduSendTimeout
                      < containerTxCfgPtr->IpduMContainerTxSendTimeout)
                     || (0u == containerTxCfgPtr->IpduMContainerTxSendTimeout))
                    && (0u != containedTxCfgPtr->IpduMContainedTxPduSendTimeout))
                {
                    innerContainerTxPtr->TransmissionTimer = containedTxCfgPtr->IpduMContainedTxPduSendTimeout;
                }
                else
                {
                    innerContainerTxPtr->TransmissionTimer = containerTxCfgPtr->IpduMContainerTxSendTimeout;
                }
            }
            else
            {
                /*@SWS_IpduM_00185:the transmission timer of the container PDU shall be updated with the contained
                    IPDU's timeout if it is less than the remaining time of the container PDU.*/
                if ((0u != containedTxCfgPtr->IpduMContainedTxPduSendTimeout)
                    && (containedTxCfgPtr->IpduMContainedTxPduSendTimeout < innerContainerTxPtr->TransmissionTimer))
                {
                    innerContainerTxPtr->TransmissionTimer = containedTxCfgPtr->IpduMContainedTxPduSendTimeout;
                }
            }

            if (containerTxCfgPtr->IpduMContainerTxFirstContainedPduTrigger)
            {
                /* SWS_IpduM_00201: Queued for transmission ,easy to move writePoint in mainTx */
                innerContainerTxPtr->TransmissionTimer = 1u;
            }

            uint8 transmissionMethod =
                IpduM_ContainerTxIsTriggerByContained(containedTxCfgPtr, containerTxCfgPtr, innerContainerTxPtr);
            if (0u != transmissionMethod)
            {
                (void)IpduM_ContainerTxTriggeredOrQueued(
                    containerTxCfgPtr,
                    innerContainerTxPtr,
                    NULL_PTR,
                    transmissionMethod,
                    FALSE);
            }
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* To insert index of LAST_IS_BEST containedTxPdu in list of current containerTxPdu and update its length
whatever static or dynamic container */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainedTxLastIsBestInsertToList(
    PduIdType                          containedIndex,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    const PduInfoType*                 PduInfoPtr)
{
    Std_ReturnType ret     = E_NOT_OK;
    boolean        getLock = FALSE;

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    const IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
    const IpduM_PartitionIDType ipduMPartition = 0u;
#endif
    SchM_Enter_IpduM_Context();
    IpduM_InnerContainerTxInstanceType* txInstancePtr = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
        containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
    if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
    {
        getLock = TRUE;
        txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
    }
    SchM_Exit_IpduM_Context();

    if (getLock)
    {
        /*@SWS_IpduM_00221: if a transmission request of the same contained I-PDU is passed multiple
            times, the IpduM shall store it only once at the postion matching its first occurrence.
            so to check whether current instance of containerTxPdu ever reveived this contained */
        boolean       hasNotReceived = TRUE;
        PduLengthType startPos, localPos;
        PduLengthType cnt;
        cnt      = txInstancePtr->ContainedCnt;
        startPos = txInstancePtr->ContainedIndexStartPos;
        for (PduLengthType loop = 0u; loop < cnt; loop++)
        {
            localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, startPos);
            if (containedIndex == IpduM_InnerContainedIndexRecord[ipduMPartition][localPos])
            {
                hasNotReceived = FALSE;
                ret            = E_OK;
                break;
            }
            startPos++;
        }

        if (hasNotReceived)
        {
            startPos = txInstancePtr->ContainedIndexStartPos;
            localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (cnt + startPos));
            if (IPDUM_UNUSED_CONTAINEDTX_INDEX == IpduM_InnerContainedIndexRecord[ipduMPartition][localPos])
            {
                ret = IpduM_InsertContainedToInstanceInOrder(
                    containerTxCfgPtr,
                    innerContainerPtr,
                    txInstancePtr,
                    containedIndex,
                    ipduMPartition,
                    PduInfoPtr->SduLength);
            }
        }

        txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

IPDUM_LOCAL_INLINE PduLengthType IpduM_ConverIndexInQueueToLocalPosition(
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    PduLengthType                      indexInQueue)
{
    PduLengthType indexQueueSize = containerTxCfgPtr->ContainedIndexQueueSize;
    PduLengthType localPosition  = containerTxCfgPtr->ContainedIndexQueueStartPos;

    localPosition += (indexInQueue % indexQueueSize); /* PRQA S 2834 */ /* VL_IpduM_Division_by_zero */
    return localPosition;
}

IPDUM_LOCAL Std_ReturnType IpduM_InsertContainedToInstanceInOrder(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    PduIdType                           containedIndex,
    const IpduM_PartitionIDType         ipduMPartition,
    const PduLengthType                 pduLength)
{
    Std_ReturnType ret      = E_NOT_OK;
    PduLengthType  startPos = txInstancePtr->ContainedIndexStartPos;
    PduLengthType  cnt      = txInstancePtr->ContainedCnt;
    PduLengthType  localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (cnt + startPos));
    const IpduM_ContainedTxPduCfgType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    /*static container */
    if (IPDUM_CONTAINER_CFG_HEADER_SIZE_NONE == containerTxCfgPtr->IpduMContainerHeaderSize)
    {
        if (pduLength <= containedTxCfgPtr->PduLen)
        {
            IpduM_InnerContainedIndexRecord[ipduMPartition][localPos] = containedIndex;
            txInstancePtr->ContainedCnt                               = (uint8)(cnt + 1u);
            /*SWS_IpduM_00254 SduLength shall be calculated with respec to IpduMPduUpdateBitPosition,
                IpduMContainedPduOffset*/
            if (containedTxCfgPtr->IpduMContainedPduOffset >= txInstancePtr->PayloadSize)
            {
                txInstancePtr->PayloadSize = containedTxCfgPtr->IpduMContainedPduOffset + pduLength;
            }
            /* if configured UpdateBit */
            if ((0u < containedTxCfgPtr->UpdateBitByteMask)
                && (containedTxCfgPtr->UpdatedBitBytePos >= txInstancePtr->PayloadSize))
            {
                txInstancePtr->PayloadSize = (containedTxCfgPtr->UpdatedBitBytePos + 1u);
            }
            ret = E_OK;
        }
    }
    else
    {
        /*dynamic container*/
        /* calculate surplus size of the current instance*/
        PduLengthType containedSize = containerTxCfgPtr->IpduMContainerHeaderSize + pduLength;
        PduLengthType surplusSize   = containerTxCfgPtr->PduLen - txInstancePtr->PayloadSize;
        if (containedSize <= surplusSize)
        {
#if (IPDUM_CONTAINEDTXPDU_PRIORITY_ENABLE == STD_OFF)
            IpduM_InnerContainedIndexRecord[ipduMPartition][localPos] = containedIndex;
#else
            localPos                     = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (startPos + cnt));
            PduLengthType localPosInsert = localPos;
            for (PduLengthType loop = cnt; loop > 0u; loop--)
            {
                PduLengthType localPosMin =
                    IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (startPos + loop - 1u));
                localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (startPos + loop));
                PduIdType tempContainedIndex = IpduM_InnerContainedIndexRecord[ipduMPartition][localPosMin];
                if (containedTxCfgPtr->IpduMContainedTxPduPirority < IpduM_GetContainedPduPriority(tempContainedIndex))
                {
                    IpduM_InnerContainedIndexRecord[ipduMPartition][localPos] =
                        IpduM_InnerContainedIndexRecord[ipduMPartition][localPosMin];
                }
                else
                {
                    break;
                }
                localPosInsert = localPosMin;
            }
            IpduM_InnerContainedIndexRecord[ipduMPartition][localPosInsert] = containedIndex;
#endif
            txInstancePtr->ContainedCnt = (uint8)(cnt + 1u);
            txInstancePtr->PayloadSize  = txInstancePtr->PayloadSize + containedSize;
            ret                         = E_OK;
        }
        else
        {
            /* @SWS_IpduM_00231:exceed the maximum size of the container.
               @SWS_IpduM_00260: for IPDUM_COLLECT_LAST_IS_BEST-Contained, Container has no queue.
            so trigger container and abort requested contained
            just trigger and do not check response
            SWS_IpduM_00259 */
            (void)IpduM_ContainerTxTriggeredOrQueued(
                containerTxCfgPtr,
                innerContainerPtr,
                txInstancePtr,
                IPDUM_CONTAINERTX_INSTANCE_TRIGGERED,
                TRUE);
        }
    }
    return ret;
}

#if (IPDUM_CONTAINEDTXPDU_PRIORITY_ENABLE == STD_ON)
IPDUM_LOCAL_INLINE uint8 IpduM_GetContainedPduPriority(
    PduIdType containedIndex) /* PRQA S 3219 */ /* VL_IpduM_NotUsedWithinThisTranslationUnit */
{
    const IpduM_ContainedTxPduCfgType* containedTxCfgPtr = &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
    return (uint8)(containedTxCfgPtr->IpduMContainedTxPduPirority);
}
#endif

/* trigger a container */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxTriggeredOrQueued(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInputInstancePtr,
    uint8                               transmissionMethod,
    boolean                             parentLockThisInstance)
{
    Std_ReturnType                      ret     = E_NOT_OK;
    boolean                             getLock = FALSE;
    IpduM_InnerContainerTxInstanceType* txInstancePtr;

    if (!parentLockThisInstance)
    {
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
        const IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
        const IpduM_PartitionIDType ipduMPartition = 0u;
#endif
        SchM_Enter_IpduM_Context();
        txInstancePtr = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
            containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
        if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
        {
            getLock = TRUE;
            txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
        }
        SchM_Exit_IpduM_Context();
    }
    else
    {
        txInstancePtr = txInputInstancePtr;
        getLock       = TRUE;
    }
    if (getLock)
    {
        if (0u < txInstancePtr->ContainedCnt)
        {
            txInstancePtr->InstanceState |= transmissionMethod;
            boolean sendRequest =
                IpduM_ContainerTxTriggeredRequestSendNow(containerTxCfgPtr, innerContainerPtr, transmissionMethod);
            if (sendRequest)
            {
                ret = IpduM_ContainerTxTriggerSendContainer(containerTxCfgPtr, innerContainerPtr, txInstancePtr);
            }
            if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            {
                /*IPDUM_DIRECT*/
                if (containerTxCfgPtr->IpduMContainerQueueSize > 1u)
                {
                    /* queued or move current instance*/
                    /* @SWS_IpduM_00262: queueSize > 1 IpduM shall store the container PDU in a queue.*/
                    /* PRQA S 2834 ++ */ /* VL_IpduM_Division_by_zero */
                    PduIdType LastInstanceIndex =
                        (PduIdType)(txInstancePtr->ContainedIndexStartPos + txInstancePtr->ContainedCnt)
                        % (containerTxCfgPtr->ContainedIndexQueueSize);
                    /* PRQA S 2834 -- */
                    uint8 raisedWritePoint          = (innerContainerPtr->TxWritePoint + 1u);
                    innerContainerPtr->TxWritePoint = (raisedWritePoint % containerTxCfgPtr->IpduMContainerQueueSize);
                    innerContainerPtr->NewInstanceContainedIndexStartPos = LastInstanceIndex;
                }
            }
        }

        if (!parentLockThisInstance)
        {
            txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* judge whether a send call need to be called rigthnow when container is triggered */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL boolean IpduM_ContainerTxTriggeredRequestSendNow(
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    uint8                              transmissionMethod)
{
    boolean sendRequest = FALSE;
    if (IPDUM_CONTAINERTX_INSTANCE_TRIGGERED == (transmissionMethod & IPDUM_CONTAINERTX_INSTANCE_TRIGGERED))
    {
        innerContainerPtr->TransmissionTimer = 0u;
        /* @SWS_IpduM_00188 : shall assemble the container and handle transmission with respect to the
        configuration for the transmission queue.*/
        if (innerContainerPtr->TxWritePoint == innerContainerPtr->TxReadPoint)
        {
            sendRequest = TRUE;
        }
    }
    else
    {
        /* queued for transmission */
        /* @SWS_IpduM_00261: IpduMContainerQueueSize is not set or set to 1 ,IpduM shall call PduR_IpduMTransmit */
        if (containerTxCfgPtr->IpduMContainerQueueSize <= 1u)
        {
            sendRequest = TRUE;
        }
    }
    return sendRequest;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxTriggerSendContainer(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr)
{
    Std_ReturnType ret;
    /* @SWS_IpduM_00261 : queueSize not set or set to 1 IpduM shall call PduR_IpduMTransmit*/
    if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
    {
        ret = IpduM_ContainerTxAssemble(containerTxCfgPtr, txInstancePtr);
    }
    else
    {
        ret = E_OK;
    }

    if (E_OK == ret)
    {
        ret = IpduM_TransmitContainerInstance(containerTxCfgPtr, innerContainerPtr, NULL_PTR, txInstancePtr, TRUE);
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxAssemble(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
    IpduM_PartitionIDType ipduMPartition = 0u;
#endif

    /* parent already locked this containerTxPdu instance */
    /* static containerTxPdu */
    if (IPDUM_CONTAINER_CFG_HEADER_SIZE_NONE == containerTxCfgPtr->IpduMContainerHeaderSize)
    {
        /* SWS_IpduM_00233: static container has configured IpduMUnusedAreasDefault
            shall ensure that all not updated areas of the container are set to
            IpduMUnusedAreasDefault value */
        (void)IStdLib_MemSet(
            txInstancePtr->BufPtr,
            containerTxCfgPtr->IpduMUnusedAreasDefault,
            containerTxCfgPtr->PduLen);
    }
    uint8 headerSize = containerTxCfgPtr->IpduMContainerHeaderSize;
    for (PduIdType index = 0u; index < txInstancePtr->ContainedCnt; index++)
    {
        PduLengthType localPos =
            IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (txInstancePtr->ContainedIndexStartPos + index));
        PduIdType containedIndex = IpduM_InnerContainedIndexRecord[ipduMPartition][localPos];
        if (containedIndex >= IpduM_CfgPtr->TxContainedNumInCurrentVariant)
        {
            /* under some special condition this function may be called twice. */
            continue; /* PRQA S 0770 */ /* VL_IpduM_Continue_hasbeen_used */
        }
        const IpduM_ContainedTxPduCfgType* containedTxCfgPtr = (&IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex]);
        if (IPDUM_COLLECT_QUEUED == containedTxCfgPtr->IpduMContainedTxPduCollectionSemantics)
        {
            ret = E_OK;
            break;
        }
        else
        {
            ret = IpduM_ContainerTxLastIsBestContainedAssemble(
                containerTxCfgPtr,
                containedTxCfgPtr,
                txInstancePtr,
                headerSize,
                index,
                &IpduM_InnerContainedIndexRecord[ipduMPartition][localPos]);
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* assemble containedTxPdu which collectSemantic is last is best */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxLastIsBestContainedAssemble(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    const IpduM_ContainedTxPduCfgType*  containedTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    uint8                               headerSize,
    PduIdType                           indexInInstance,
    PduIdType*                          containdIndexBufPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    /* IPDUM_COLLECT_LAST_IS_BEST */
    PduInfoType         pduInfo;
    const PduLengthType containerCfgLen = containerTxCfgPtr->PduLen;
    if (headerSize > 0u)
    {
        /*dynamic container*/
        if (0u == indexInInstance)
        {
            txInstancePtr->PayloadSize = 0u;
        }
        pduInfo.SduDataPtr = (&txInstancePtr->BufPtr[(txInstancePtr->PayloadSize + headerSize)]);
        pduInfo.SduLength  = containerCfgLen - (txInstancePtr->PayloadSize + headerSize);
    }
    else
    {
        /*static container*/
        pduInfo.SduDataPtr = (&txInstancePtr->BufPtr[containedTxCfgPtr->IpduMContainedPduOffset]);
        pduInfo.SduLength  = containedTxCfgPtr->PduLen;
    }

    ret                            = PduR_IpduMTriggerTransmit(containedTxCfgPtr->PduId, &pduInfo);
    PduLengthType tempContainerLen = pduInfo.SduLength + headerSize + txInstancePtr->PayloadSize;
    /*@SWS_IpduM_00222
    if PduR_IpduMTriggerTransmit returns E_NOT_OK for a contained I-PDU,
    IpduM shall omit this contained I-PDU silently. */
    if ((E_OK == ret) && (pduInfo.SduLength <= containerTxCfgPtr->PduLen) && (containerCfgLen >= tempContainerLen))
    {
        /*dynamic container*/
        if (headerSize > 0u)
        {
            /*fill headerID and DLC to Header*/
            IpduM_FillContainedHeaderInContainer(
                headerSize,
                (&txInstancePtr->BufPtr[txInstancePtr->PayloadSize]),
                containedTxCfgPtr->IpduMContainedPduHeaderId,
                pduInfo.SduLength);
            /*@SWS_IpduM_00231: the overall size of the updated I-PDUs could fluctuate.*/
            txInstancePtr->PayloadSize += (pduInfo.SduLength + headerSize);
        }
        else
        {
            /*SWS_IpduM_00246: Only the last contained Ipdu of a static containerIpdu may be a dynamic length PDU*/
            /*SWS_IpduM_00235: ensure that the update bit of this contained I-PDU is set and only if
                the contained I-PDU was successfully updated.*/
            if (((indexInInstance + 1u) == txInstancePtr->ContainedCnt)
                || (containedTxCfgPtr->PduLen == pduInfo.SduLength))
            {
                IpduM_UpdateStaticContainerTxInstance(containedTxCfgPtr, txInstancePtr, pduInfo.SduLength);
            }
            else
            {
                containdIndexBufPtr[0u] = IPDUM_UNUSED_CONTAINEDTX_INDEX;
            }
        }
#if (IPDUM_METADATA_SUPPORT == STD_ON)
        /* @SWS_IpduM_00228 shall use the MetaData last collected from the contained IPDU*/
        if (0u < containerTxCfgPtr->MetaDataSize)
        {
            (void)IStdLib_MemCpy(txInstancePtr->MetaDataPtr, pduInfo.MetaDataPtr, containerTxCfgPtr->MetaDataSize);
        }
#endif
    }
    else
    {
        containdIndexBufPtr[0u] = IPDUM_UNUSED_CONTAINEDTX_INDEX;
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_FillContainedHeaderInContainer(uint8 headerSizeCfg, uint8* dataBuf, uint32 idVal, uint32 dlcVal)
{
    uint8  idSizeInByte, dlcSizeInByte;
    uint8* dataPtr;
    if (IPDUM_CONTAINER_CFG_HEADER_SIZE_SHORT == headerSizeCfg)
    {
        idSizeInByte  = IPDUM_SHOT_HEADER_ID_SIZE;
        dlcSizeInByte = IPDUM_SHOT_HEADER_DLC_SIZE;
    }
    else
    {
        idSizeInByte  = IPDUM_LONG_HEADER_ID_SIZE;
        dlcSizeInByte = IPDUM_LONG_HEADER_DLC_SIZE;
    }
    dataPtr = dataBuf;
    for (uint8 byteLoop = 0u; byteLoop < idSizeInByte; byteLoop++)
    {
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
        dataPtr[byteLoop] = (uint8)(idVal >> (((idSizeInByte - byteLoop) - 1u) * 8u));
#else
        dataPtr[byteLoop] = (uint8)(idVal >> (byteLoop * IPDUM_MAGIC_NUM_EIGHT));
#endif
    }
    dataPtr = (&dataBuf[idSizeInByte]);
    for (uint8 byteLoop = 0u; byteLoop < dlcSizeInByte; byteLoop++)
    {
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
        dataPtr[byteLoop] = (uint8)(dlcVal >> (((dlcSizeInByte - byteLoop) - 1u) * 8u));
#else
        dataPtr[byteLoop] = (uint8)(dlcVal >> (byteLoop * IPDUM_MAGIC_NUM_EIGHT));
#endif
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_UpdateStaticContainerTxInstance(
    const IpduM_ContainedTxPduCfgType*  containedTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    const PduLengthType                 pduLength)
{
    /* @SWS_IpduM_00235: shall ensure that the update bit of this contained
         I-PDU is set if and only if the contained I-PDU was successfully updated */
    if (containedTxCfgPtr->UpdateBitByteMask > 0u)
    {
        txInstancePtr->BufPtr[containedTxCfgPtr->UpdatedBitBytePos] |= containedTxCfgPtr->UpdateBitByteMask;
    }
    /* @SWS_IpduM_00254 SduLength shall be calculated with respec to IpduMPduUpdateBitPosition,
        IpduMContainedPduOffset*/
    if ((0u < containedTxCfgPtr->UpdateBitByteMask)
        && (containedTxCfgPtr->UpdatedBitBytePos >= txInstancePtr->PayloadSize))
    {
        txInstancePtr->PayloadSize = (containedTxCfgPtr->UpdatedBitBytePos + 1u);
    }

    if (containedTxCfgPtr->IpduMContainedPduOffset >= txInstancePtr->PayloadSize)
    {
        txInstancePtr->PayloadSize = (containedTxCfgPtr->IpduMContainedPduOffset + pduLength);
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* transmit container instance */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_TransmitContainerInstance(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInputInstancePtr,
    boolean                             parentLockThisInstance)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
    IpduM_PartitionIDType ipduMPartition = 0u;
#endif
    IpduM_InnerContainerTxInstanceType* txInstancePtr;
    boolean                             getLock = FALSE;
    if (!parentLockThisInstance)
    {
        SchM_Enter_IpduM_Context();
        txInstancePtr = &IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
            (containerTxCfgPtr->ContainerQueueStartPos) + innerContainerPtr->TxReadPoint)];
        if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
        {
            getLock = TRUE;
            txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
        }
        SchM_Exit_IpduM_Context();
    }
    else
    {
        txInstancePtr = txInputInstancePtr;
        getLock       = TRUE;
    }
    if (getLock)
    {
        if (0u < txInstancePtr->PayloadSize)
        {
            boolean requestSend = FALSE;
            if (NULL_PTR != triggPduInfoPtr)
            {
                /* @SWS_IpduM_00194: IpduM_TriggTransmit if the queue is empty/non-existent, the current instance of the
                   container PDU is copied. if the current instance of is empty as well ,E_NOT_OK is returned. */
                requestSend = TRUE;
            }
            else
            {
                if (((innerContainerPtr->TxNotOkRetyNumber <= IPDUM_MAX_TRANSMIT_RETRIES)
                     && (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOWERLAYER_ACCEPTED)))
                    && ((IPDUM_CONTAINERTX_INSTANCE_TRIGGERED
                         == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_TRIGGERED))
                        || (IPDUM_CONTAINERTX_INSTANCE_QUEUED
                            == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_QUEUED))))
                {
                    requestSend = TRUE;
                }
            }

            if (requestSend)
            {
                ret = IpduM_ContainerTxHandleSendPdu(
                    ipduMPartition,
                    containerTxCfgPtr,
                    innerContainerPtr,
                    triggPduInfoPtr,
                    txInstancePtr);
            }
        }

        if (!parentLockThisInstance)
        {
            txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* handle ContainerTx transmission request */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxHandleSendPdu(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr)

{
    Std_ReturnType ret             = E_NOT_OK;
    boolean        ReqDropInstance = FALSE;

    ret = IpduM_ContainerTxSendPdu(
        containerTxCfgPtr,
        innerContainerPtr,
        triggPduInfoPtr,
        txInstancePtr,
        &ReqDropInstance);
    if (E_OK == ret)
    {
        PduLengthType localPos, startPos, cnt;
        PduLengthType startPosInConf = containerTxCfgPtr->ContainedIndexQueueStartPos;
        startPos                     = txInstancePtr->ContainedIndexStartPos;
        for (cnt = 0u; cnt < txInstancePtr->ContainedCnt; cnt++)
        {
            localPos                 = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, startPos);
            PduIdType containedIndex = IpduM_InnerContainedIndexRecord[ipduMPartition][localPos];
            if (IPDUM_UNUSED_CONTAINEDTX_INDEX != containedIndex)
            {
                const IpduM_ContainedTxPduCfgType* containedTxCfgPtr =
                    &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
                if (containedTxCfgPtr->IpduMContainedTxPduConfirmation)
                {
                    IpduM_ContainedPendingTxConfirmation[ipduMPartition][startPosInConf] = containedIndex;
                    startPosInConf++;
                }
            }
            startPos++;
        }
        innerContainerPtr->NumberOfWaitConfirmContained =
            (PduIdType)(startPosInConf - containerTxCfgPtr->ContainedIndexQueueStartPos);
    }
    else
    {
        /* SWS_IpduM_00195:  up to IpduMMaxTransmitRetries times */
        if (IPDUM_MAX_TRANSMIT_RETRIES < innerContainerPtr->TxNotOkRetyNumber)
        {
            innerContainerPtr->TxNotOkRetyNumber = 0u;
            ReqDropInstance                      = TRUE;
        }
    }

    if (ReqDropInstance)
    {
        if (E_OK == ret)
        {
            innerContainerPtr->IsNotWaitContainerTxConf = FALSE;
        }
        /* @SWS_IpduM_00191: PduR_IpduMTransmit returns E_OK for that Container PDU , shall remove that instance */
        /* @SWS_IpduM_00200: in IPDUM_TRIGGERTRANSMIT a container PDU instance shall be dropped from the queue if
            it is fetched by IpduM_TriggerTransmit */
        IpduM_ContainerTxReleaseTxInstance(ipduMPartition, containerTxCfgPtr, txInstancePtr);
        if (containerTxCfgPtr->IpduMContainerQueueSize > 1u)
        {
            uint8 raisedReadPoint          = (innerContainerPtr->TxReadPoint + 1u);
            innerContainerPtr->TxReadPoint = (raisedReadPoint % containerTxCfgPtr->IpduMContainerQueueSize);
        }
    }
    return ret;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* send or get  instance of containerTxPdu */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainerTxSendPdu(
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerPtr,
    PduInfoType*                        triggPduInfoPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr,
    boolean*                            ReqDropInstancePtr)
{
    Std_ReturnType ret = E_NOT_OK;
    /* Directed transmit or transmit a instance in queue called in IpduMMainFunctionTx*/
    if (NULL_PTR == triggPduInfoPtr)
    {
        /* @SWS_IpduM_00189: shall wait for transmission confirmation of a container PDU before invoking
                PduR_IpduMTransmit. */
        if (innerContainerPtr->IsNotWaitContainerTxConf)
        {
            PduInfoType pduInfo;
            if (IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            {
                /*@SWS_IpduM_00254: shall invoke Pdur_IpduMTransmit with SduDataPtr set NULL.*/
                pduInfo.SduDataPtr = NULL_PTR;
            }
            else
            {
                /* @SWS_IpduM_00192: IpduMContainerTxTriggerMode is IPDUM_DIRECT, assemble and pass this contaienr
                to PduR */
                pduInfo.SduDataPtr = txInstancePtr->BufPtr;
            }
            pduInfo.SduLength = (PduLengthType)txInstancePtr->PayloadSize;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            if (0u < containerTxCfgPtr->MetaDataSize)
            {
                pduInfo.MetaDataPtr = txInstancePtr->MetaDataPtr;
            }
            else
#endif
            {
                pduInfo.MetaDataPtr = NULL_PTR;
            }

            ret = PduR_IpduMTransmit(containerTxCfgPtr->PduId, &pduInfo);
            if (E_OK == ret)
            {
                txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOWERLAYER_ACCEPTED;
                innerContainerPtr->TxNotOkRetyNumber = 0u;
                if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
                {
                    *ReqDropInstancePtr = TRUE;
                }
            }
            else
            {
                innerContainerPtr->TxNotOkRetyNumber++;
            }
        }
    }
    else
    {
        /*current context is in IpduM_TriggerTransmit()
        @SWS_IpduM_00060*/
        if (triggPduInfoPtr->SduLength >= txInstancePtr->PayloadSize)
        {
            (void)IStdLib_MemCpy(triggPduInfoPtr->SduDataPtr, txInstancePtr->BufPtr, txInstancePtr->PayloadSize);
            triggPduInfoPtr->SduLength = txInstancePtr->PayloadSize;
            *ReqDropInstancePtr        = TRUE;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            if (0u < containerTxCfgPtr->MetaDataSize)
            {
                triggPduInfoPtr->MetaDataPtr = txInstancePtr->MetaDataPtr;
            }
            else
#endif
            {
                triggPduInfoPtr->MetaDataPtr = NULL_PTR;
            }
            ret = E_OK;
        }
    }
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* contained in container queue need be calculate position in buffer */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL Std_ReturnType IpduM_ContainedTxCollectQueuedAdd(
    PduIdType                          containedIndex,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr,
    const PduInfoType*                 PduInfoPtr)
{
    Std_ReturnType ret     = E_NOT_OK;
    boolean        getLock = FALSE;

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    const IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
    const IpduM_PartitionIDType ipduMPartition = 0u;
#endif
    SchM_Enter_IpduM_Context();
    IpduM_InnerContainerTxInstanceType* txInstancePtr = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
        containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
    if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
    {
        getLock = TRUE;
        txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
    }
    SchM_Exit_IpduM_Context();
    if (getLock)
    {
        uint8 headerSize = containerTxCfgPtr->IpduMContainerHeaderSize;
        IpduM_ContainerCheckTxInstanceIsOverwrite(ipduMPartition, containerTxCfgPtr, innerContainerPtr, txInstancePtr);
        PduLengthType containedLen    = PduInfoPtr->SduLength + headerSize;
        PduLengthType surplusSize     = (containerTxCfgPtr->PduLen - txInstancePtr->PayloadSize);
        boolean       txInstanceValid = TRUE;

        if (containedLen > surplusSize)
        {
            txInstanceValid = FALSE;
            uint8 transmissionMethod;
            if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
            {
                /* @SWS_IpduM_00182: Container shall be triggered */
                transmissionMethod = IPDUM_CONTAINERTX_INSTANCE_TRIGGERED;
            }
            else
            {
                /* @SWS_IpduM_00183: Container shall be queued */
                transmissionMethod = IPDUM_CONTAINERTX_INSTANCE_QUEUED;
            }
            (void)IpduM_ContainerTxTriggeredOrQueued(
                containerTxCfgPtr,
                innerContainerPtr,
                txInstancePtr,
                transmissionMethod,
                getLock);

            if (IpduM_ContainerTxGetNewWriteInstance(ipduMPartition, containerTxCfgPtr, innerContainerPtr))
            {
                IpduM_InnerContainerTxInstanceType* txNewInstancePtr =
                    (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
                        containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
                txNewInstancePtr->ContainedIndexStartPos = innerContainerPtr->NewInstanceContainedIndexStartPos;
                txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_FULL_OCCUPIED;
                txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
                txInstancePtr   = txNewInstancePtr;
                txInstanceValid = TRUE;
            }
        }

        if (txInstanceValid)
        {
            uint8*        dataPtr  = &txInstancePtr->BufPtr[txInstancePtr->PayloadSize];
            PduLengthType startPos = txInstancePtr->ContainedIndexStartPos;
            PduLengthType cnt      = txInstancePtr->ContainedCnt;
            PduLengthType localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (cnt + startPos));
            if (IPDUM_UNUSED_CONTAINEDTX_INDEX == IpduM_InnerContainedIndexRecord[ipduMPartition][localPos])
            {
                IpduM_InnerContainedIndexRecord[ipduMPartition][localPos] = containedIndex;
                txInstancePtr->ContainedCnt                               = (uint8)(cnt + 1u);
                /*fill headerID and DLC to Header*/
                const IpduM_ContainedTxPduCfgType* containedTxCfgPtr =
                    &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
                IpduM_FillContainedHeaderInContainer(
                    headerSize,
                    dataPtr,
                    containedTxCfgPtr->IpduMContainedPduHeaderId,
                    PduInfoPtr->SduLength);
                (void)IStdLib_MemCpy(&dataPtr[headerSize], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
                txInstancePtr->PayloadSize += (uint16)(PduInfoPtr->SduLength + headerSize);
#if (IPDUM_METADATA_SUPPORT == STD_ON)
                /* @SWS_IpduM_00228 shall use the MetaData last collected from the contained IPDU*/
                if (0u < containerTxCfgPtr->MetaDataSize)
                {
                    (void)IStdLib_MemCpy(
                        txInstancePtr->MetaDataPtr,
                        PduInfoPtr->MetaDataPtr,
                        containerTxCfgPtr->MetaDataSize);
                }
#endif
                ret = E_OK;
            }
        }
        else
        {
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_QUEUEOVFL);
#endif
        }

        txInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERTX_INSTANCE_LOCKED);
    }

    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL boolean IpduM_ContainerTxGetNewWriteInstance(
    const IpduM_PartitionIDType        ipduMPartition,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    IpduM_InnerContainerTxType*        innerContainerPtr)
{
    IpduM_InnerContainerTxInstanceType* txInstancePtr;
    boolean                             retValue = FALSE;

    SchM_Enter_IpduM_Context();
    txInstancePtr = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
        containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
    /* queued or move current instance*/
    if ((IPDUM_TRIGGERTRANSMIT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
        && (containerTxCfgPtr->IpduMContainerQueueSize > 1u))
    {
        /* @SWS_IpduM_00256: on IPDUM_TRIGGERTRANSMIT shall assemble the container PDU in context of
        IpduM_TriggerTransmit and transmission requests are continuously collected, even after a call of
        PduR_IpduMTransmit.*/
        /* PRQA S 2834 ++ */ /* VL_IpduM_Division_by_zero */
        PduIdType LastInstanceIndex = (PduIdType)((txInstancePtr->ContainedIndexStartPos + txInstancePtr->ContainedCnt)
                                                  % (containerTxCfgPtr->ContainedIndexQueueSize));
        /* PRQA S 2834 -- */
        uint8 raisedWritePoint          = (innerContainerPtr->TxWritePoint + 1u);
        innerContainerPtr->TxWritePoint = (raisedWritePoint % containerTxCfgPtr->IpduMContainerQueueSize);
        innerContainerPtr->NewInstanceContainedIndexStartPos = LastInstanceIndex;
        txInstancePtr                                        = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
            containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);
    }

    if (0u == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_LOCKED))
    {
        retValue = TRUE;
        txInstancePtr->InstanceState |= IPDUM_CONTAINERTX_INSTANCE_LOCKED;
    }
    SchM_Exit_IpduM_Context();
    if (retValue)
    {
        IpduM_ContainerCheckTxInstanceIsOverwrite(ipduMPartition, containerTxCfgPtr, innerContainerPtr, txInstancePtr);
    }
    return retValue;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* trigger condition check */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL uint8 IpduM_ContainerTxIsTriggerByContained(
    const IpduM_ContainedTxPduCfgType* containedTxCfgPtr,
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr,
    const IpduM_InnerContainerTxType*  innerContainerPtr)
{
    uint8 ret;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    const IpduM_PartitionIDType ipduMPartition = containerTxCfgPtr->IpduMContainerTxPartition;
#else
    const IpduM_PartitionIDType ipduMPartition = 0u;
#endif
    const IpduM_InnerContainerTxInstanceType* txInstancePtr = (&IpduM_InnerContainerTxInstanceRecord[ipduMPartition][(
        containerTxCfgPtr->ContainerQueueStartPos + innerContainerPtr->TxWritePoint)]);

    /*SWS_IpduM_00181 trigger always
      SWS_IpduM_00201 first contained I-PDU trigger*/
    if (IPDUM_TRIGGER_ALWAYS == containedTxCfgPtr->IpduMContainedTxPduTrigger)
    {
        ret = IPDUM_CONTAINERTX_INSTANCE_TRIGGERED;
    }
    else if (
        (IPDUM_CONTAINER_CFG_HEADER_SIZE_NONE == containerTxCfgPtr->IpduMContainerHeaderSize)
        && (containerTxCfgPtr->StaticContainerIncContainedNum == txInstancePtr->ContainedCnt))
    {
        /*SWS_IpduM_00234
        all contained I-PDUs were updated.*/
        ret = IPDUM_CONTAINERTX_INSTANCE_TRIGGERED;
    }
    else if (
        (IPDUM_UNUSED_CONTAINER_SIZE_THRESHOLD != containerTxCfgPtr->IpduMcontainerTxSizeThreshold)
        && (txInstancePtr->PayloadSize >= containerTxCfgPtr->IpduMcontainerTxSizeThreshold))
    {
        /*SWS_IpduM_00180
        Instance size reach threshold.*/
        ret = IPDUM_CONTAINERTX_INSTANCE_TRIGGERED;
    }
    else
    {
        ret = IPDUM_CONTAINERTX_INSTANCE_NO_TRIGGERED_NO_QUEUED;
    }
    /*do not check ContainerTxSendTimeout in this function and
    do it in IpduMMainTxFunction would be better.*/
    return ret;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* container tx confirmation handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_TxConfirmOfContainer(PduIdType containerIndex, Std_ReturnType result)
{
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType       ipduMPartition      = containerTxCfgPtr->IpduMContainerTxPartition;
    PduIdType                   startOnPartionIndex = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartition);
    PduIdType                   onPartitionIndex    = containerIndex - startOnPartionIndex;
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[ipduMPartition][onPartitionIndex];
#else
    IpduM_PartitionIDType       ipduMPartition      = 0u;
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[ipduMPartition][containerIndex];
#endif

    PduLengthType startIndex = containerTxCfgPtr->ContainedIndexQueueStartPos;
    for (uint16 index = 0u; index < innerContainerTxPtr->NumberOfWaitConfirmContained; index++)
    {
        PduIdType containedIndex = IpduM_ContainedPendingTxConfirmation[ipduMPartition][startIndex + index];
        if (IPDUM_UNUSED_CONTAINEDTX_INDEX != containedIndex)
        {
            /* @SWS_IpduM_00196,
            translate this confirmation  */
            const IpduM_ContainedTxPduCfgType* containedTxCfgPtr =
                &IpduM_CfgPtr->IpduMContainedTxPduPtr[containedIndex];
            PduR_IpduMTxConfirmation(containedTxCfgPtr->PduId, result);
        }
    }
    /* clear contained index */
    (void)IStdLib_MemSet(
        &IpduM_ContainedPendingTxConfirmation[ipduMPartition][startIndex],
        IPDUM_UNUSED_UINT8,
        (uint32)(innerContainerTxPtr->NumberOfWaitConfirmContained * sizeof(PduIdType)));
    innerContainerTxPtr->NumberOfWaitConfirmContained = 0u;
    innerContainerTxPtr->IsNotWaitContainerTxConf     = TRUE;

    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* container tx i-pdu handle */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerTxHandle(PduIdType containerIndex)
{
    const IpduM_ContainerTxPduCfgType* containerTxCfgPtr;
    containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[containerIndex];
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType       ipduMPartition      = containerTxCfgPtr->IpduMContainerTxPartition;
    PduIdType                   startOnPartionIndex = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartition);
    PduIdType                   onPartitionIndex    = containerIndex - startOnPartionIndex;
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[ipduMPartition][onPartitionIndex];
#else
    IpduM_InnerContainerTxType* innerContainerTxPtr = &IpduM_ContainerTxDescription[0u][containerIndex];
#endif
    uint16 currentRetryNum = innerContainerTxPtr->TxNotOkRetyNumber;
    if (0x0u < innerContainerTxPtr->TransmissionTimer)
    {
        boolean timeTrigger = FALSE;
        SchM_Enter_IpduM_Context();
        innerContainerTxPtr->TransmissionTimer--;
        if (0x0u == innerContainerTxPtr->TransmissionTimer)
        {
            timeTrigger = TRUE;
        }
        SchM_Exit_IpduM_Context();
        if (timeTrigger)
        {
            /* @SWS_IpduM_00186: when transmission timer of container has elapse,this
                container shall be trigger */
            (void)IpduM_ContainerTxTriggeredOrQueued(
                containerTxCfgPtr,
                innerContainerTxPtr,
                NULL_PTR,
                IPDUM_CONTAINERTX_INSTANCE_TRIGGERED,
                FALSE);
        }
    }

    if (innerContainerTxPtr->IsNotWaitContainerTxConf
        && ((currentRetryNum + 1u) != innerContainerTxPtr->TxNotOkRetyNumber))
    {
        /* do not check response */
        (void)IpduM_TransmitContainerInstance(containerTxCfgPtr, innerContainerTxPtr, NULL_PTR, NULL_PTR, FALSE);
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerTxReleaseTxInstance(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr)
{
    PduLengthType startPos = txInstancePtr->ContainedIndexStartPos;
    for (uint8 cnt = 0u; cnt < txInstancePtr->ContainedCnt; cnt++)
    {
        /*release containedIndexes instance */
        PduLengthType localPos = IpduM_ConverIndexInQueueToLocalPosition(containerTxCfgPtr, (startPos + cnt));
        IpduM_InnerContainedIndexRecord[ipduMPartition][localPos] = IPDUM_UNUSED_CONTAINEDTX_INDEX;
    }
    txInstancePtr->ContainedCnt = 0u;
    txInstancePtr->PayloadSize  = 0u;
    txInstancePtr->InstanceState &= IPDUM_CONTAINERTX_INSTANCE_CLEAR_EXCEP_LOCKED;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerCheckTxInstanceIsOverwrite(
    IpduM_PartitionIDType               ipduMPartition,
    const IpduM_ContainerTxPduCfgType*  containerTxCfgPtr,
    IpduM_InnerContainerTxType*         innerContainerTxPtr,
    IpduM_InnerContainerTxInstanceType* txInstancePtr)
{
    boolean queueIsFull = FALSE;
    if (IPDUM_CONTAINERTX_INSTANCE_TRIGGERED == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_TRIGGERED))
    {
        if (IPDUM_DIRECT == containerTxCfgPtr->IpduMContainerTxTriggerMode)
        {
            queueIsFull = TRUE;
        }
        else
        {
            /* @SWS_IpduM_00256: transmission request are continuously collected  */
            if ((innerContainerTxPtr->TxReadPoint == innerContainerTxPtr->TxWritePoint)
                && (IPDUM_CONTAINERTX_INSTANCE_FULL_OCCUPIED
                    == (txInstancePtr->InstanceState & IPDUM_CONTAINERTX_INSTANCE_FULL_OCCUPIED)))
            {
                queueIsFull = TRUE;
            }
        }

        if (queueIsFull)
        {
            IpduM_ContainerTxReleaseTxInstance(ipduMPartition, containerTxCfgPtr, txInstancePtr);
            innerContainerTxPtr->TxNotOkRetyNumber = 0u;
            uint8 raisedReadPoint                  = (innerContainerTxPtr->TxReadPoint + 1u);
            /* PRQA S 2834 ++ */ /* VL_IpduM_Division_by_zero */
            innerContainerTxPtr->TxReadPoint = (raisedReadPoint % containerTxCfgPtr->IpduMContainerQueueSize);
            /* PRQA S 2834 -- */
            txInstancePtr->ContainedIndexStartPos = innerContainerTxPtr->NewInstanceContainedIndexStartPos;
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
            IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_TRANSMIT, IPDUM_E_QUEUEOVFL);
#endif
        }
    }
    else
    {
        if (0u == txInstancePtr->ContainedCnt)
        {
            txInstancePtr->ContainedIndexStartPos = innerContainerTxPtr->NewInstanceContainedIndexStartPos;
        }
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerTxPduInit(IpduM_PartitionIDType ipduMPartition)
{
    PduIdType startID                     = IpduM_GetStartOfContainerTxPduInPartition(ipduMPartition);
    PduIdType endID                       = IpduM_GetEndOfContainerTxPduInPartition(ipduMPartition);
    PduIdType inPartitionContainerTxIndex = 0u;
    for (; startID < endID; startID++)
    {
        /* containerTxPdu state initialization */
        const IpduM_ContainerTxPduCfgType* containerTxCfgPtr = &IpduM_CfgPtr->IpduMContainerTxPduPtr[startID];
        IpduM_InnerContainerTxType*        innerContainerPtr =
            &IpduM_ContainerTxDescription[ipduMPartition][inPartitionContainerTxIndex];
        innerContainerPtr->IsNotWaitContainerTxConf          = TRUE;
        innerContainerPtr->NumberOfWaitConfirmContained      = 0u;
        innerContainerPtr->TxReadPoint                       = 0u;
        innerContainerPtr->TxWritePoint                      = 0u;
        innerContainerPtr->TransmissionTimer                 = 0u;
        innerContainerPtr->NewInstanceContainedIndexStartPos = 0u;
        innerContainerPtr->TxNotOkRetyNumber                 = 0u;
        /* containerTxPdu instance initialization */
        PduLengthType loopPos = containerTxCfgPtr->ContainerQueueStartPos;
        for (uint16 loop = 0u; loop < containerTxCfgPtr->IpduMContainerQueueSize; loop++)
        {
            IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].ContainedCnt           = 0u;
            IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].ContainedIndexStartPos = 0u;
            IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].PayloadSize            = 0u;
            IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].InstanceState          = 0u;
            IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].BufPtr =
                &IpduM_DataBuf[ipduMPartition][containerTxCfgPtr->BufIndex + (loop * containerTxCfgPtr->PduLen)];
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            if (0u < containerTxCfgPtr->MetaDataSize)
            {
                IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].MetaDataPtr =
                    &IpduM_MetaDataBuf[ipduMPartition]
                                      [containerTxCfgPtr->MetaDataBufIndex + (loop * containerTxCfgPtr->MetaDataSize)];
            }
            else
            {
                IpduM_InnerContainerTxInstanceRecord[ipduMPartition][loopPos].MetaDataPtr = NULL_PTR;
            }
#endif
            loopPos++;
        }

        /* containedTxPdu index queueu initialization */
        loopPos = containerTxCfgPtr->ContainedIndexQueueStartPos;
        (void)IStdLib_MemSet(
            &IpduM_InnerContainedIndexRecord[ipduMPartition][loopPos],
            IPDUM_UNUSED_UINT8,
            (uint32)(containerTxCfgPtr->ContainedIndexQueueSize * sizeof(PduIdType)));
        (void)IStdLib_MemSet(
            &IpduM_ContainedPendingTxConfirmation[ipduMPartition][loopPos],
            IPDUM_UNUSED_UINT8,
            (uint32)(containerTxCfgPtr->ContainedIndexQueueSize * sizeof(PduIdType)));
        inPartitionContainerTxIndex++;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"
#endif

#if (IPDUM_CONTAINER_RX_NUMBER_MAX > 0u)
/* in rx indiation handle container */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxIndication(PduIdType containerIndex, const PduInfoType* PduInfoPtr)
{
    const IpduM_ContainerRxPduCfgType* containerRxCfgPtr = &IpduM_CfgPtr->IpduMContainerRxPduPtr[containerIndex];
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    const IpduM_PartitionIDType ipduMPartition        = containerRxCfgPtr->IpduMContainerRxPartition;
    PduIdType                   startOnPartitionIndex = IpduM_GetStartOfContainerRxPduInPartition(ipduMPartition);
    PduIdType                   onPartitionIndex      = containerIndex - startOnPartitionIndex;
    IpduM_InnerContainerRxType* innerContainerRxPtr   = &IpduM_ContainerRxDescription[ipduMPartition][onPartitionIndex];
#else
    const IpduM_PartitionIDType ipduMPartition      = 0u;
    IpduM_InnerContainerRxType* innerContainerRxPtr = &IpduM_ContainerRxDescription[ipduMPartition][containerIndex];
#endif
    PduLengthType acceptPduLength = PduInfoPtr->SduLength;
    if (containerRxCfgPtr->PduLen < acceptPduLength)
    {
        acceptPduLength = containerRxCfgPtr->PduLen;
    }

    if (0u < acceptPduLength)
    {
        if (IPDUM_PROCESSING_IMMEDIATE == containerRxCfgPtr->IpduMContainerPduProcessing)
        {
            /* no queue do not need lock instance */
            IpduM_InnerContainerRxInstanceType* rxInstancePtr =
                &IpduM_InnerContainerRxInstanceRecord[ipduMPartition]
                                                     [(containerRxCfgPtr->ContainerRxQueueStartPos)
                                                      + innerContainerRxPtr->RxWritePoint];
            rxInstancePtr->PayloadSize = acceptPduLength;
            rxInstancePtr->DataPtr     = PduInfoPtr->SduDataPtr;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            /* @SWS_IpduM_00229: forward the MetaData */
            if (0u < containerRxCfgPtr->MetaDataSize)
            {
                rxInstancePtr->MetaDataPtr = PduInfoPtr->MetaDataPtr;
            }
#endif
            IpduM_ContainerRxExtractInstance(containerRxCfgPtr, rxInstancePtr);
        }
        else
        {
            /* @SWS_IpduM_00211
            IPDUM_PROCESSING_DEFERRED shall be queue next mainfunction handle */
            boolean getLock = FALSE;
            SchM_Enter_IpduM_Context();
            IpduM_InnerContainerRxInstanceType* rxInstancePtr =
                &IpduM_InnerContainerRxInstanceRecord[ipduMPartition]
                                                     [(containerRxCfgPtr->ContainerRxQueueStartPos)
                                                      + innerContainerRxPtr->RxWritePoint];
            if (0u == (rxInstancePtr->InstanceState & IPDUM_CONTAINERRX_INSTANCE_LOCKED))
            {
                getLock = TRUE;
                rxInstancePtr->InstanceState |= IPDUM_CONTAINERRX_INSTANCE_LOCKED;
            }
            SchM_Exit_IpduM_Context();

            if (getLock)
            {
                IpduM_ContainerCheckRxInstanceIsOverwrite(containerRxCfgPtr, innerContainerRxPtr, rxInstancePtr);
                /* calculate byf index */
                PduLengthType bufIndex =
                    containerRxCfgPtr->BufIndex + (innerContainerRxPtr->RxWritePoint * containerRxCfgPtr->PduLen);
                /*copy data do buffer*/
                (void)IStdLib_MemCpy(
                    &IpduM_DataBuf[ipduMPartition][bufIndex],
                    PduInfoPtr->SduDataPtr,
                    PduInfoPtr->SduLength);
                rxInstancePtr->PayloadSize = acceptPduLength;
                rxInstancePtr->DataPtr     = &IpduM_DataBuf[ipduMPartition][bufIndex];
#if (IPDUM_METADATA_SUPPORT == STD_ON)
                /* @SWS_IpduM_00229: store and forward the MetaData */
                if (0u < containerRxCfgPtr->MetaDataSize)
                {
                    (void)IStdLib_MemCpy(
                        rxInstancePtr->MetaDataPtr,
                        PduInfoPtr->MetaDataPtr,
                        containerRxCfgPtr->MetaDataSize);
                }
#endif
                /* @SWS_IpduM_00212
                exceed IpduMContainerQueueSize discard oldest instance and report to det */
                uint8 raisedWritePoint = (innerContainerRxPtr->RxWritePoint + 1u);
                /* PRQA S 2834 ++ */ /* VL_IpduM_Division_by_zero */
                innerContainerRxPtr->RxWritePoint = (raisedWritePoint % containerRxCfgPtr->IpduMContainerQueueSize);
                /* PRQA S 2834 --*/
                rxInstancePtr->InstanceState |= IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED;
                rxInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERRX_INSTANCE_LOCKED);
            }
            else
            {
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
                IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_QUEUEOVFL);
#endif
            }
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxExtractInstance(
    const IpduM_ContainerRxPduCfgType*  containerRxCfgPtr,
    IpduM_InnerContainerRxInstanceType* rxContainerInstancePtr)
{
    if (IPDUM_CONTAINER_CFG_HEADER_SIZE_NONE == containerRxCfgPtr->IpduMContainerCfgHeaderSize)
    {
        IpduM_ContainerRxExtractStatic(containerRxCfgPtr, rxContainerInstancePtr);
    }
    else
    {
        IpduM_ContainerRxExtractDynamic(containerRxCfgPtr, rxContainerInstancePtr);
    }
    rxContainerInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED);
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* extracted contained in container rx pdu*/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxExtractStatic(
    const IpduM_ContainerRxPduCfgType*        containerRxCfgPtr,
    const IpduM_InnerContainerRxInstanceType* rxInstancePtr)
{
    const IpduM_ContainedRxPduCfgType* containedRxPtr;
    PduLengthType                      containedIndex     = containerRxCfgPtr->RxContainedStartIndexInContainedPdu;
    PduIdType                          containedCnt       = containerRxCfgPtr->ContainerRxContainContainedCnt;
    PduIdType                          acceptContainedCnt = 0u;
    /* containerRxPdu length check */
    for (PduIdType loop = containedCnt; loop > 0u; loop--)
    {
        containedRxPtr = &IpduM_CfgPtr->IpduMContainedRxPduPtr[containedIndex + loop - 1u];
        if (rxInstancePtr->PayloadSize > containedRxPtr->IpduMContainedRxPduOffset)
        {
            /* @SWS_IpduM_00237:
                   ignore all contained that are not or not completely contained in the received container */
            /* SWS_IpduM_00246: Only the last contained Ipdu of a static containerIpdu may be a dynamic length PDU */
            if (rxInstancePtr->PayloadSize >= (containedRxPtr->IpduMContainedRxPduOffset + containedRxPtr->PduLen))
            {
                acceptContainedCnt = loop;
            }
            else
            {
                /* drop all containteds */
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
                IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_CONTAINER);
#endif
            }
            break;
        }
    }

    for (PduIdType loop = 0u; loop < acceptContainedCnt; loop++)
    {
        PduInfoType outPdu;
        containedRxPtr    = &IpduM_CfgPtr->IpduMContainedRxPduPtr[containedIndex + loop];
        outPdu.SduDataPtr = &rxInstancePtr->DataPtr[containedRxPtr->IpduMContainedRxPduOffset];
#if (IPDUM_METADATA_SUPPORT == STD_ON)
        if (0u < containerRxCfgPtr->MetaDataSize)
        {
            outPdu.MetaDataPtr = rxInstancePtr->MetaDataPtr;
        }
        else
#endif
        {
            outPdu.MetaDataPtr = NULL_PTR;
        }
        uint8 tempUpdateMask = containedRxPtr->UpdateBitByteMask;
        /* SWS_IpduM_00246: Only the last contained Ipdu of a static containerIpdu may be a dynamic length PDU */
        if (loop != (containedCnt - 1u))
        {
            outPdu.SduLength = containedRxPtr->PduLen;
        }
        else
        {
            outPdu.SduLength = (rxInstancePtr->PayloadSize - containedRxPtr->IpduMContainedRxPduOffset);
        }

        if (0u < tempUpdateMask)
        {
            /* @SWS_IpduM_00236: if configued update bit, only process and indicate it if its update-bit is set */
            if ((outPdu.SduDataPtr[containedRxPtr->UpdatedBitBytePos] & tempUpdateMask) == tempUpdateMask)
            {
                PduR_IpduMRxIndication(containedRxPtr->PduId, &outPdu);
            }
        }
        else
        {
            PduR_IpduMRxIndication(containedRxPtr->PduId, &outPdu);
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* extracted contained in container rx pdu*/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxExtractDynamic(
    const IpduM_ContainerRxPduCfgType*        containerRxCfgPtr,
    const IpduM_InnerContainerRxInstanceType* rxInstancePtr)
{
    boolean       stopFlg    = FALSE;
    PduLengthType index      = 0u;
    const uint8   headerSize = containerRxCfgPtr->IpduMContainerCfgHeaderSize;
    /*@SWS_IpduM_00214: if the remaining bytes in a container are less than the IpduMContainerHeadersize the
        remaining bytes shall be ignore.*/
    uint8* localDataPtr = rxInstancePtr->DataPtr;
    uint32 headerId, headerDlc;
    while (((index + headerSize) < rxInstancePtr->PayloadSize) && (!stopFlg))
    {
        IpduM_ContainerRxGetHeadInfomation(localDataPtr, &headerId, &headerDlc, headerSize);
        localDataPtr = &localDataPtr[headerSize];
        index += headerSize;
        if (headerId > 0UL)
        {
            PduLengthType containedCnt;
            PduLengthType startIndex;
            /*@SWS_IpduM_00205: match only contained I-PDUs that referenced the containerRxPdu*/
            if (IPDUM_ACCEPT_CONFIGURED == containerRxCfgPtr->IpduMContainerRxAcceptContainedPdu)
            {
                startIndex   = containerRxCfgPtr->RxContainedStartIndexInContainedPdu;
                containedCnt = containerRxCfgPtr->ContainerRxContainContainedCnt;
            }
            else
            {
                /*@SWS_IpduM_00204: compare with contained I-PDUs that do not define referenced containerRxPdu*/
                startIndex   = IpduM_CfgPtr->StartIndexOfContainedRxForAccepALLCfg;
                containedCnt = IpduM_CfgPtr->ContainerRxAcceptAllContainedSize;
            }

            for (PduIdType loop = 0x0u; loop < containedCnt; loop++)
            {
                const IpduM_ContainedRxPduCfgType* containedRxPtr =
                    &IpduM_CfgPtr->IpduMContainedRxPduPtr[startIndex + loop];
                uint32 containedId;
                if (IPDUM_CONTAINER_CFG_HEADER_SIZE_SHORT == containerRxCfgPtr->IpduMContainerCfgHeaderSize)
                {
                    containedId = containedRxPtr->IpduMContainedPduShortHeaderId;
                }
                else
                {
                    containedId = containedRxPtr->IpduMContainedPduLongHeaderId;
                }

                if (headerId == containedId)
                {
                    if ((headerDlc + index) > rxInstancePtr->PayloadSize)
                    {
                        /* @SWS_IpduM_00213: the payload length exceeds the remaining bytes,
                        processing shall be stop,and report det*/
                        stopFlg = TRUE;
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
                        IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_HEADER);
#endif
                    }
                    else
                    {
                        PduInfoType outPdu;
                        outPdu.SduDataPtr = localDataPtr;
                        outPdu.SduLength  = (PduLengthType)headerDlc;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
                        /* @SWS_IpduM_00229: forward the MetaData */
                        if (0u < containerRxCfgPtr->MetaDataSize)
                        {
                            outPdu.MetaDataPtr = rxInstancePtr->MetaDataPtr;
                        }
                        else
#endif
                        {
                            outPdu.MetaDataPtr = NULL_PTR;
                        }
                        PduR_IpduMRxIndication(containedRxPtr->PduId, &outPdu);
                    }
                    break;
                }
            }
            /* @SWS_IpduM_00207: not match,shall be discarded silently */
            index += (PduLengthType)headerDlc;
            localDataPtr = &localDataPtr[headerDlc];
        }
        else
        {
            /* @SWS_IpduM_00210 :detecting a header containing the ID 0 the processing for this container PDU
                shall be stopped */
            stopFlg = TRUE;
        }
    }
    return;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* get header information of containedRxPdu */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxGetHeadInfomation(
    const uint8* dataBuffer,
    uint32*      headerIdPtr,
    uint32*      headerDlcPtr,
    uint8        headerSize)
{
    PduLengthType byteSize     = IpduM_GetContaiedIDSize(headerSize);
    const uint8*  localDataPtr = dataBuffer;
    uint32        headerId     = 0u;

    for (PduLengthType loop = 0u; loop < byteSize; loop++)
    {
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
        headerId |= (uint32)(((uint32)(*localDataPtr)) << (((byteSize - 1u) - loop) * 8u));
#else
        headerId |= (uint32)(((uint32)(*localDataPtr)) << (loop * IPDUM_MAGIC_NUM_EIGHT));
#endif
        localDataPtr++;
    }

    uint32 headerDlc = 0u;
    byteSize         = IpduM_GetContaiedDLCSize(headerSize);
    for (PduLengthType loop = 0u; loop < byteSize; loop++)
    {
#if (IPDUM_BIG_ENDIAN == IPDUM_HEADER_BYTE_ORDER)
        headerDlc |= (uint32)(((uint32)(*localDataPtr)) << (((byteSize - 1u) - loop) * 8u));
#else
        headerDlc |= (uint32)(((uint32)(*localDataPtr)) << (loop * IPDUM_MAGIC_NUM_EIGHT));
#endif
        localDataPtr++;
    }
    *headerIdPtr  = headerId;
    *headerDlcPtr = headerDlc;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL void IpduM_ContainerRxPduInit(IpduM_PartitionIDType ipduMPartition)
{
    PduIdType ContainerRxStartID          = IpduM_GetStartOfContainerRxPduInPartition(ipduMPartition);
    PduIdType ContainerRxEndID            = IpduM_GetEndOfContainerRxPduInPartition(ipduMPartition);
    PduIdType inPartitionContainerRxIndex = 0u;
    for (; ContainerRxStartID < ContainerRxEndID; ContainerRxStartID++)
    {
        IpduM_ContainerRxDescription[ipduMPartition][inPartitionContainerRxIndex].RxReadPoint  = 0u;
        IpduM_ContainerRxDescription[ipduMPartition][inPartitionContainerRxIndex].RxWritePoint = 0u;
        const IpduM_ContainerRxPduCfgType* containerRxCfgPtr =
            &IpduM_CfgPtr->IpduMContainerRxPduPtr[ContainerRxStartID];
        /* containerTxPdu instance initialization */
        PduLengthType loopPos = containerRxCfgPtr->ContainerRxQueueStartPos;
        for (uint16 loop = 0u; loop < containerRxCfgPtr->IpduMContainerQueueSize; loop++)
        {
            IpduM_InnerContainerRxInstanceRecord[ipduMPartition][loopPos].InstanceState = 0u;
            IpduM_InnerContainerRxInstanceRecord[ipduMPartition][loopPos].PayloadSize   = 0u;
            IpduM_InnerContainerRxInstanceRecord[ipduMPartition][loopPos].DataPtr =
                &IpduM_DataBuf[ipduMPartition][containerRxCfgPtr->BufIndex + (loop * containerRxCfgPtr->PduLen)];
#if (IPDUM_METADATA_SUPPORT == STD_ON)
            if (0u < containerRxCfgPtr->MetaDataSize)
            {
                IpduM_InnerContainerRxInstanceRecord[ipduMPartition][loopPos].MetaDataPtr =
                    &IpduM_MetaDataBuf[ipduMPartition]
                                      [containerRxCfgPtr->MetaDataBufIndex + (loop * containerRxCfgPtr->MetaDataSize)];
            }
            else
            {
                IpduM_InnerContainerRxInstanceRecord[ipduMPartition][loopPos].MetaDataPtr = NULL_PTR;
            }
#endif
            loopPos++;
        }
        inPartitionContainerRxIndex++;
    }
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* get Id field size in header */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL uint8 IpduM_GetContaiedIDSize(uint8 headerSize)
{
    uint8 sizeInByte;
    /*@SWS_IpduM_00175*/
    switch (headerSize)
    {
    case IPDUM_CONTAINER_CFG_HEADER_SIZE_SHORT:
        sizeInByte = IPDUM_SHOT_HEADER_ID_SIZE;
        break;
    case IPDUM_CONTAINER_CFG_HEADER_SIZE_LONG:
        sizeInByte = IPDUM_LONG_HEADER_ID_SIZE;
        break;
    default:
        sizeInByte = 0u;
        break;
    }
    return sizeInByte;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

/* get DLC field size in header */
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"
IPDUM_LOCAL uint8 IpduM_GetContaiedDLCSize(uint8 headerSize)
{
    uint8 sizeInByte;
    /*@SWS_IpduM_00175*/
    switch (headerSize)
    {
    case IPDUM_CONTAINER_CFG_HEADER_SIZE_SHORT:
        sizeInByte = IPDUM_SHOT_HEADER_DLC_SIZE;
        break;
    case IPDUM_CONTAINER_CFG_HEADER_SIZE_LONG:
        sizeInByte = IPDUM_LONG_HEADER_DLC_SIZE;
        break;
    default:
        sizeInByte = 0u;
        break;
    }
    return sizeInByte;
}
#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"

IPDUM_LOCAL_INLINE void IpduM_ContainerCheckRxInstanceIsOverwrite(
    const IpduM_ContainerRxPduCfgType*  containerRxCfgPtr,
    IpduM_InnerContainerRxType*         innerContainerRxPtr,
    IpduM_InnerContainerRxInstanceType* rxInstancePtr)
{
    if (IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED
        == (rxInstancePtr->InstanceState & IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED))
    {
        uint8 raisedReadPoint = (innerContainerRxPtr->RxReadPoint + 1u);
        /* PRQA S 2834 ++*/ /* VL_IpduM_Division_by_zero */
        innerContainerRxPtr->RxReadPoint = (raisedReadPoint % containerRxCfgPtr->IpduMContainerQueueSize);
        /* PRQA S 2834 --*/
        rxInstancePtr->InstanceState &= (uint8)(~IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED);
#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
        IpduM_DetReportRuntimeError(IPDUM_INSTANCE_ID, IPDUM_SERVICE_ID_RXINDICATION, IPDUM_E_QUEUEOVFL);
#endif
    }
}

#endif

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
IPDUM_LOCAL_INLINE IpduM_PartitionIDType IpduM_GetIpduMPartionByECUCParttion(IpduM_PartitionIDType ecuCPartition)
{
    IpduM_PartitionIDType ipduMPartition = IPDUM_UNUSED_PARTITION_ID;
    if (ecuCPartition < IPDUM_ECUC_USED_PARTITION_NUM)
    {
        ipduMPartition = IpduM_EcuCPartitionIdInIpduMPartition[ecuCPartition];
    }
    return ipduMPartition;
}
#endif

/* set section bits mask */
IPDUM_LOCAL_INLINE void IpduM_SetBitInSection(uint8* data, uint8 startBit, uint8 endBit)
{
    /* PRQA S 2907 ++*/ /* VL_IpduM_Positive_integer_truncate */
    *data = (uint8)((uint8)(IPDUM_UINT8_MAX << (startBit)) & (IPDUM_UINT8_MAX >> (IPDUM_MAGIC_NUM_SEVEN - endBit)));
    /* PRQA S 2907 --*/
}

/* extract section bit */
IPDUM_LOCAL_INLINE void IpduM_ExtractBitInSection(uint8* data, uint8 startBit, uint8 endBit)
{
    *data =
        ((uint8)((*data) << (IPDUM_MAGIC_NUM_SEVEN - (endBit))) >> (IPDUM_MAGIC_NUM_SEVEN - (endBit))) >> (startBit);
}

#if (STD_ON == IPDUM_DEV_ERROR_DETECT)
/* report develop error to Det */
IPDUM_LOCAL_INLINE void IpduM_DetReportError(uint8 instanceId, uint8 apiId, uint8 errorId)
{
    (void)Det_ReportError(IPDUM_MODULE_ID, instanceId, apiId, errorId);
}

/* report runtime error to Det */
IPDUM_LOCAL_INLINE void IpduM_DetReportRuntimeError(uint8 instanceId, uint8 apiId, uint8 errorId)
{
    (void)Det_ReportRuntimeError(IPDUM_MODULE_ID, instanceId, apiId, errorId);
}
#endif

/* get start index of containerTxPdu on given partition */
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerTxPduInPartition(uint8 ipduMPartitionId)
{
    return (((ipduMPartitionId) == 0u) ? 0u : IpduM_CfgPtr->ContainerTxPduRange[(ipduMPartitionId)-1u]);
}
#else
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerTxPduInPartition(uint8 ipduMPartitionId)
{
    IPDUM_NOUSED(ipduMPartitionId);
    return 0u;
}
#endif
/* get end index of containerTxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfContainerTxPduInPartition(uint8 ipduMPartitionId)
{
    return (IpduM_CfgPtr->ContainerTxPduRange[(ipduMPartitionId)]);
}

/* get start index of containerRxPdu on given partition */
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerRxPduInPartition(uint8 ipduMPartitionId)
{
    return (((ipduMPartitionId) == 0u) ? 0u : IpduM_CfgPtr->ContainerRxPduRange[(ipduMPartitionId)-1u]);
}
#else
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfContainerRxPduInPartition(uint8 ipduMPartitionId)
{
    IPDUM_NOUSED(ipduMPartitionId);
    return 0u;
}
#endif
/* get end index of containerRxPdu on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfContainerRxPduInPartition(uint8 ipduMPartitionId)
{
    return (IpduM_CfgPtr->ContainerRxPduRange[(ipduMPartitionId)]);
}

/* get start index of txRequest on given partition */
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfTxRequestPduInPartition(uint8 ipduMPartitionId)
{
    return (((ipduMPartitionId) == 0u) ? 0u : IpduM_CfgPtr->TxRequestPduRange[(ipduMPartitionId)-1u]);
}
#else
IPDUM_LOCAL_INLINE PduIdType IpduM_GetStartOfTxRequestPduInPartition(uint8 ipduMPartitionId)
{
    IPDUM_NOUSED(ipduMPartitionId);
    return 0u;
}
#endif
/* get end index of txRequest on given partition */
IPDUM_LOCAL_INLINE PduIdType IpduM_GetEndOfTxRequestPduInPartition(uint8 ipduMPartitionId)
{
    return (IpduM_CfgPtr->TxRequestPduRange[(ipduMPartitionId)]);
}
/* PRQA S 6040,6030,6070 -- */
/* PRQA S 1503,1532 -- */
