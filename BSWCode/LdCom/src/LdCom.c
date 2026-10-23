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
 **  @file               : LdCom.c
 **  @author             : zhengfei.li
 **  @date               : 2020/07/03
 **  @vendor             : isoft
 **  @description        : Large Data Communication
 **
 ***********************************************************************************************************************/

/* PRQA S 3415 EOF */ /* VL_LdCom_3415 */

/* =================================================== inclusions =================================================== */
#include "LdCom_Internal.h"
#include "LdCom_Cbk.h"
#include "PduR_LdCom.h"
#include "LdCom_PBcfg.h"
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#include "Os.h"
#endif
#include "LdCom_Arti.h"

/* ===================================================== macros ===================================================== */

/* Max LdComIpdu number */
#define LDCOM_IPDU_NUMBER_MAX LdCom_CfgPtr->IpduNum

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */
/*Module initialize status, TRUE initialized, FALSE not initialized*/
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#define LdCom_InitStatus (*LdCom_MultiPartitionInitStatusPtr[partIndex])
#else
#define LDCOM_START_SEC_VAR_CLEARED_BOOLEAN
#include "LdCom_MemMap.h"
LDCOM_LOCAL boolean LdCom_InitStatus;
#define LDCOM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "LdCom_MemMap.h"
#endif

/*configuration pointer*/
#define LDCOM_START_SEC_VAR_CLEARED_PTR
#include "LdCom_MemMap.h"
LDCOM_LOCAL const LdCom_ConfigType* LdCom_CfgPtr;
#define LDCOM_STOP_SEC_VAR_CLEARED_PTR
#include "LdCom_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
LDCOM_LOCAL uint8 LdCom_GetPartitionIndex(void);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
#endif

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
LDCOM_LOCAL boolean LdCom_DetCheckInitStatus(uint8 apiId);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
LDCOM_LOCAL boolean LdCom_DetCheckParaPointer(uint8 apiId, const void* parameterPtr);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
LDCOM_LOCAL boolean LdCom_DetCheckPduId(uint8 apiId, PduIdType pduId, PduIdType ldComPduId, boolean checkPartition);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
LDCOM_LOCAL boolean LdCom_DetCheckPartitionId(uint8 apiId, uint8 partIndex);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
#endif
#endif

/* ========================================== external function definition ========================================== */

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Initializes internal variables of the LdCom module.
 */
void LdCom_Init(const LdCom_ConfigType* config) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 2, 0x01, config);

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
    uint8 partIndex = LdCom_GetPartitionIndex();
#endif

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_INIT, config)
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        && (LdCom_DetCheckPartitionId(LDCOM_SERVICE_ID_INIT, partIndex))
#endif
    )
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        LdCom_InitStatus = TRUE; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        if (LdCom_CfgPtr == NULL_PTR)
#else
        LdCom_InitStatus = TRUE;
#endif
        {
            LdCom_CfgPtr = config;
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x01);
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Deinitialize internal variables of the LdCom module. LdCom module has been switch to uninitialized state.
 */
void LdCom_DeInit(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 1, 0x02);

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
    uint8 partIndex = LdCom_GetPartitionIndex();
#endif

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_DEINIT)
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        && (LdCom_DetCheckPartitionId(LDCOM_SERVICE_ID_DEINIT, partIndex))
#endif
    )
#endif
    {
        LdCom_CfgPtr = NULL_PTR;
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        LdCom_InitStatus = FALSE; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
#else
        LdCom_InitStatus = FALSE;
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x02);
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#if (STD_ON == LDCOM_VERSION_INFO_API)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Set the members of versioninfo
 */
void LdCom_GetVersionInfo(Std_VersionInfoType* versioninfo) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 2, 0x03, versioninfo);

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->moduleID         = LDCOM_MODULE_ID;
        versioninfo->vendorID         = LDCOM_VENDOR_ID;
        versioninfo->sw_major_version = LDCOM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = LDCOM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = LDCOM_SW_PATCH_VERSION;
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x03);
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
#endif

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Users transmit signal.
 * 1.Use the Id ("id" parameter in the call) to look-up the correct LdComIPdu in the LdCom configuration.
 * 2.Use the LdComIPdu Derive the PduRId of the corresponding PduR in the PduR configuration.
 * 3.The PduRId be used to forward the call towards the PduR.
 */
/* PRQA S 1503,1532 ++*/ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
Std_ReturnType LdCom_Transmit(PduIdType Id, const PduInfoType* PduInfoPtr)
/* PRQA S 1503,1532 --*/
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x49, Id, PduInfoPtr);

    Std_ReturnType ret     = E_NOT_OK;
    PduIdType      ldComId = Id;

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_TRANSMIT)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TRANSMIT, Id, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_TRANSMIT, PduInfoPtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        ldComId = LdCom_GetIpduIdFromUserId(Id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
        /* PRQA S 2814 -- */
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TRANSMIT, Id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2814, 2824 ++ */ /* VL_QAC_DerefNullPtr */
            ret = PduR_LdComTransmit(LdCom_CfgPtr->IPduCfgPtr[ldComId].DestModulePduId, PduInfoPtr);
            /* PRQA S 2814, 2824 -- */
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 2, 0x49, ret);

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Use the id to look-up the correct <LdComUser_LdComCbkCopyTxData> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkCopyTxData> with the info,retry and availableDataPtr.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
BufReq_ReturnType
    LdCom_CopyTxData(PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr)
/* PRQA S 1503 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 5, 0x43, id, info, retry, availableDataPtr);

    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_COPYTXDATA)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_COPYTXDATA, id, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_COPYTXDATA, info)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_COPYTXDATA, availableDataPtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
        PduIdType ldComId = id;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_COPYTXDATA, id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComTxCopyTxDataFuncPtrType copyTxDataFunc = LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkTxCopyTxDataFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != copyTxDataFunc)
            {
                ret = copyTxDataFunc(info, retry, availableDataPtr);
            }
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(retry);
        LDCOM_NOUSED(availableDataPtr);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 2, 0x43, ret);

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE_FAST
#include "LdCom_MemMap.h"
/**
 * Use the id to look-up the correct <LdComUser_LdComCbkTpTxConfirmation> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkTpTxConfirmation> with the result.
 */
void LdCom_TpTxConfirmation(PduIdType id, Std_ReturnType result) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x48, id, result);

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_TPTXCONFIRMATION)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TPTXCONFIRMATION, id, 0u, FALSE))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
        PduIdType ldComId = id;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TPTXCONFIRMATION, id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComTpTxConfirmationFuncPtrType tpTxConfirFunc =
                LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkTpTxConfirmationFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != tpTxConfirFunc)
            {
                tpTxConfirFunc(result);
            }
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(result);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x48);
}
#define LDCOM_STOP_SEC_CODE_FAST
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Use the id to look-up the correct <LdComUser_LdComCbkStartOfReception> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkStartOfReception> with the info, TpSduLength and bufferSizePtr.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
BufReq_ReturnType LdCom_StartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 1503 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 5, 0x46, id, info, TpSduLength, bufferSizePtr);

    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_STARTOFRECEPTION)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_STARTOFRECEPTION, id, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_STARTOFRECEPTION, bufferSizePtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        PduIdType ldComId = id;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_STARTOFRECEPTION, id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComRxStartOfReceptionFuncPtrType rxStartOfReceptionFunc =
                LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkRxStartOfReceptionFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != rxStartOfReceptionFunc)
            {
                ret = rxStartOfReceptionFunc(info, TpSduLength, bufferSizePtr);
            }
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(TpSduLength);
        LDCOM_NOUSED(bufferSizePtr);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 2, 0x46, ret);

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Use the id to look-up the correct <LdComUser_LdComCbkCopyRxData> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkCopyRxData> with the info and bufferSizePtr.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
BufReq_ReturnType LdCom_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
/* PRQA S 1503 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 4, 0x44, id, info, bufferSizePtr);

    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_COPYRXDATA)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_COPYRXDATA, id, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_COPYRXDATA, info)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_COPYRXDATA, bufferSizePtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        PduIdType ldComId = id;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_COPYRXDATA, id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComRxCopyRxDataFuncPtrType rxCopyRxDataFunc = LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkRxCopyRxDataFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != rxCopyRxDataFunc)
            {
                ret = rxCopyRxDataFunc(info, bufferSizePtr);
            }
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(info);
        LDCOM_NOUSED(bufferSizePtr);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 2, 0x44, ret);

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE_FAST
#include "LdCom_MemMap.h"
/**
 * Use the id to look-up the correct <LdComUser_LdComCbkTpRxIndication> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkTpRxIndication> with the result.
 */
void LdCom_TpRxIndication(PduIdType id, Std_ReturnType result) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x45, id, result);

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_TPRXINDICATION)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TPRXINDICATION, id, 0u, FALSE))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
        PduIdType ldComId = id;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(id, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TPRXINDICATION, id, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComTpRxIndicationFuncPtrType tpRxIndicationFunc =
                LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkTpRxIndicationFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != tpRxIndicationFunc)
            {
                tpRxIndicationFunc(result);
            }
        }
#else
        LDCOM_NOUSED(id);
        LDCOM_NOUSED(result);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x45);
}
#define LDCOM_STOP_SEC_CODE_FAST
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE_FAST
#include "LdCom_MemMap.h"
/**
 * Use the RxPduId to look-up the correct <LdComUser_LdComCbkRxIndication> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkRxIndication> with the PduInfoPtr.
 */
void LdCom_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x42, RxPduId, PduInfoPtr);

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_RXINDICATION)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_RXINDICATION, RxPduId, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_RXINDICATION, PduInfoPtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_RECEIVER_IF)
        PduIdType ldComId = RxPduId;
#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(RxPduId, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_RXINDICATION, RxPduId, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComRxIndicationFuncPtrType rxIndicationFunc = LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkRxIndicationFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != rxIndicationFunc)
            {
                rxIndicationFunc(PduInfoPtr);
            }
        }
#else
        LDCOM_NOUSED(RxPduId);
        LDCOM_NOUSED(PduInfoPtr);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x42);
}
#define LDCOM_STOP_SEC_CODE_FAST
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE_FAST
#include "LdCom_MemMap.h"
/**
 * Use the TxPduId to look-up the correct <LdComUser_LdComCbkTxConfirmation> in the LdCom configuration.
 * Forward the call towards the <LdComUser_LdComCbkTxConfirmation> with the result.
 */
void LdCom_TxConfirmation(PduIdType TxPduId, Std_ReturnType result) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x40, TxPduId, result);

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_TXCONFIRMATION)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TXCONFIRMATION, TxPduId, 0u, FALSE))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_SEND_IF)
        PduIdType ldComId = TxPduId;
#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(TxPduId, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TXCONFIRMATION, TxPduId, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComTxConfirmationFuncPtrType txConfirFunc = LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkTxConfirmationFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != txConfirFunc)
            {
                txConfirFunc(result);
            }
        }
#else
        LDCOM_NOUSED(TxPduId);
        LDCOM_NOUSED(result);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 1, 0x40);
}
#define LDCOM_STOP_SEC_CODE_FAST
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * Use the TxPduId to look-up the correct <LdComUser_LdComCbkTriggerTransmit> in the LdCom configuration.
 * Forward the call towards the configured <LdComUser_LdComCbkTriggerTransmit> where copy the upper layer
 * module (called module) data into buffer provided by PduInfoPtr->SduDataPtr and update the length of the
 * actual copied data in PduInfoPtr->SduLength.
 */
/* PRQA S 1503,3673 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_3673 */
Std_ReturnType LdCom_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1503,3673 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Start, 3, 0x41, TxPduId, PduInfoPtr);

    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
    if (LdCom_DetCheckInitStatus(LDCOM_SERVICE_ID_TRIGGERTRANSMIT)
        && LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, TxPduId, 0u, FALSE)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, PduInfoPtr)
        && LdCom_DetCheckParaPointer(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, PduInfoPtr->SduDataPtr))
#endif
    {
#if (STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT)
        PduIdType ldComId = TxPduId;

#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        ldComId = LdCom_GetIpduIdFromUserId(TxPduId, LdCom_CfgPtr->UserIdMapLdComIPduIdPtr);
#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
        if (LdCom_DetCheckPduId(LDCOM_SERVICE_ID_TRIGGERTRANSMIT, TxPduId, ldComId, TRUE))
#endif
#endif
        {
            /* PRQA S 2824 ++ */ /* VL_QAC_DerefNullPtr */
            const LdComTriggerTransmitFuncPtrType triggerTransmitFunc =
                LdCom_CfgPtr->IPduCfgPtr[ldComId].CbkTriggerTransmitFunc;
            /* PRQA S 2824 -- */
            if (NULL_PTR != triggerTransmitFunc)
            {
                ret = triggerTransmitFunc(PduInfoPtr);
            }
        }
#else
        LDCOM_NOUSED(TxPduId);
        LDCOM_NOUSED(PduInfoPtr);
#endif
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, LdCom, 0, Bsw_Return, 2, 0x41, ret);

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

/* ========================================== internal function definition ========================================== */
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * get LdCom running application id.
 */
LDCOM_LOCAL uint8 LdCom_GetPartitionIndex(void)
{
    uint8           index;
    ApplicationType curAppId = GetApplicationID();

    for (index = 0u; index < LDCOM_PARTITION_NUM; index++)
    {
        if (curAppId == LdCom_IPduApplicationId[index])
        {
            break;
        }
    }

    return index;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
#endif

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"

/**
 * @brief     Check initialization status of the LdCom module.
 */
LDCOM_LOCAL boolean LdCom_DetCheckInitStatus(uint8 apiId)
{
    boolean ret = FALSE;

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
    uint8 partIndex = LdCom_GetPartitionIndex();
    if ((partIndex < LDCOM_PARTITION_NUM) && (LdCom_InitStatus)) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
#else
    if (LdCom_InitStatus)
#endif
    {
        ret = TRUE;
    }
    else
    {
        LDCOM_DET_REPORT(apiId, LDCOM_E_UNINIT);
        ret = FALSE;
    }

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * @brief     Check if input pointer parameters is NULL.
 */
LDCOM_LOCAL boolean LdCom_DetCheckParaPointer(uint8 apiId, const void* parameterPtr)
{
    boolean ret = TRUE;

    if (NULL_PTR == parameterPtr)
    {
        LDCOM_DET_REPORT(apiId, LDCOM_E_PARAM_POINTER);
        ret = FALSE;
    }

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * @brief     Check if the validity of the Pdu ID.
 */
LDCOM_LOCAL boolean LdCom_DetCheckPduId(uint8 apiId, PduIdType pduId, PduIdType ldComPduId, boolean checkPartition)
{
    boolean ret = TRUE;

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
    ApplicationType curAppId = GetApplicationID();
#endif

    if ((LDCOM_CFG_IPDU_NUMBER_MAX <= pduId)
#if (STD_ON == LDCOM_SUPPORT_POST_BUILD)
        || (LDCOM_IPDU_NUMBER_MAX <= ldComPduId) || (LDCOM_PDU_ID_INVALID == ldComPduId)
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        || (checkPartition && (curAppId != LdCom_CfgPtr->IPduCfgPtr[ldComPduId].ApplicationId))
#endif
#else
#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
        || (curAppId != LdCom_CfgPtr->IPduCfgPtr[pduId].ApplicationId)
#endif
#endif

    )
    {
        uint8 errorId =
            (LDCOM_SERVICE_ID_TRANSMIT == apiId) ? (LDCOM_E_INVALID_SIGNAL_ID) : (LDCOM_E_INVALID_PDU_SDU_ID);
        LDCOM_DET_REPORT(apiId, errorId);
        ret = FALSE;
    }
    LDCOM_NOUSED(ldComPduId);
    LDCOM_NOUSED(checkPartition);
    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"

#if (STD_ON == LDCOM_SUPPORT_MULTIPLE_PARTITION)
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"
/**
 * @brief     Check if the validity of the partition ID.
 */
LDCOM_LOCAL boolean LdCom_DetCheckPartitionId(uint8 apiId, uint8 partIndex)
{
    boolean ret = TRUE;

    if (partIndex >= LDCOM_PARTITION_NUM)
    {
        LDCOM_DET_REPORT(apiId, LDCOM_E_INVALID_PARTITION);
        ret = FALSE;
    }

    return ret;
}
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
#endif
#endif
