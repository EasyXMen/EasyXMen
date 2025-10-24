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
 ************************************************************************************************************************
 **
 **  @file               : Dcm_Internal.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm Private Functions/Types Declaration
 **
 ***********************************************************************************************************************/
#ifndef DCM_INTERNAL_H_
#define DCM_INTERNAL_H_
/* PRQA S 3472 EOF */ /* VL_Dcm_3472 */
/* =================================================== inclusions =================================================== */
#include "Det.h"
#include "Dcm_CfgData.h"
#include "Dcm_UDS.h"
#include "Dcm_OBD.h"
#include "Dcm_Dsl.h"
#include "Dcm_Dsd.h"
#include "SchM_Dcm.h"
#include "Dcm_Externals.h"
#include "Rte_Dcm.h"
#ifdef DCM_MEMCOPY_USED_ILIB
#include "IStdLib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * @brief The followings are configuration shortcuts
 */
#define DCM_PROTOCOL_TX(connectionId) Dcm_DslMainConnection[connectionId].ProtocolTx
#define DCM_SERVICETABLE(protocolId)  Dcm_DslProtocolRow[protocolId].SidTable
#if ((STD_ON == DCM_UDS_0X22) && (DCM_DID_RANGE_ENABLED == STD_ON))
#define DCM_RANGEDID_INFO(rangeDidIndex) Dcm_DspDidInfo[Dcm_DspDidRange[rangeDidIndex].DidRangeInfoRef]
#endif
#if (DCM_DID_NUM > 0)
#define DCM_DID_INFO(didIndex) Dcm_DspDidInfo[Dcm_DspDid[didIndex].DidInfoRef]
#endif
#define DCM_GET_AVAILINDEX(availIndex) ((availIndex) * DCM_OBDID_AVAILDATALEN)

#define DCM_INVALID_UINT8  0xFFu           /**<invalid values */
#define DCM_INVALID_UINT16 (uint16)0xFFFFu /**<invalid values */
#define DCM_INVALID_UINT24 0xFFFFFFuL      /**<invalid values */
#define DCM_INVALID_UINT32 0xFFFFFFFFuL    /**<invalid values */

#if !defined(DCM_LOCAL)
#define DCM_LOCAL static /* PRQA S 3414 */ /* VL_Dcm_3414 */
#endif

#define DCM_POSITIVE_RSPMASK   0x40u /**< positive response mask */
#define DCM_NEGATIVE_RSPCODE   0x7Fu /**< negative response code */
#define DCM_NEGATIVE_RSPMSGLEN 3uL   /**< negative response code */
#define DCM_SUPPRESS_BIT       0x80u /**< positive response suppression bit */
#define DCM_SUBFUNCTION_MASK   0x7Fu /**< subFunction mask */
#ifdef DCM_VIN_REF
#define DCM_VIN_SIZE 17u /**< VIN data size */
#endif
#define DCM_UNUSED(a) (void)(a) /**< used for unsed params */
/**
 * @brief transform uint8_N data to specific dataType
 */
#define DCM_U8N_TO_U32(data)                                                                                        \
    ((uint32)((uint32)(data)[0u] << 24u) | (uint32)((uint32)(data)[1u] << 16u) | (uint32)((uint32)(data)[2u] << 8u) \
     | (uint32)(data)[3u])
#define DCM_U8N_TO_U24(data) \
    ((uint32)((uint32)(data)[0u] << 16u) | (uint32)((uint32)(data)[1u] << 8u) | (uint32)(data)[2u])
#define DCM_U8N_TO_U16(data) ((uint16)((uint16)(data)[0u] << 8u) | (uint16)(data)[1u])
#define DCM_U8N_TO_S32(data)                                                                                        \
    ((sint32)((sint32)(data)[0u] << 24u) | (sint32)((sint32)(data)[1u] << 16u) | (sint32)((sint32)(data)[2u] << 8u) \
     | (sint32)(data)[3u])
#define DCM_U8N_TO_S16(data) ((sint16)((sint16)(data)[0u] << 8u) | (sint16)(data)[1u])

#define DCM_U8N_TO_U32_LE(data)                                                                                     \
    ((uint32)((uint32)(data)[3u] << 24u) | (uint32)((uint32)(data)[2u] << 16u) | (uint32)((uint32)(data)[1u] << 8u) \
     | (uint32)(data)[0u])
#define DCM_U8N_TO_U24_LE(data) \
    ((uint32)((uint32)(data)[2u] << 16u) | (uint32)((uint32)(data)[1u] << 8u) | (uint32)(data)[0u])
#define DCM_U8N_TO_U16_LE(data) ((uint16)((uint16)(data)[1u] << 8u) | (uint16)(data)[0u])
#define DCM_U8N_TO_S32_LE(data)                                                                                     \
    ((sint32)((sint32)(data)[3u] << 24u) | (sint32)((sint32)(data)[2u] << 16u) | (sint32)((sint32)(data)[1u] << 8u) \
     | (sint32)(data))
#define DCM_U8N_TO_S16_LE(data) ((sint16)((sint16)(data)[1u] << 8u) | (sint16)(data)[0u])
/**
 * @brief DET development and runtime error report
 */
#if (STD_ON == DCM_DEV_ERROR_DETECT)
#define DCM_DET_REPORT(ApiId, ErrorId) \
    ((void)Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId), (ErrorId))) /**< DET report */
#endif
#define DCM_DET_REPORTRUNTIME(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId), (ErrorId)))
/* =========================================== external data declarations =========================================== */
/**
 * @brief the programing condition (communicating with bootloader) @range NA
 */
extern Dcm_ProgConditionsType Dcm_ProgConditions;
/**
 * @brief the config pointer @range NA
 */
extern const Dcm_ConfigType* Dcm_CfgPtr;
/* ========================================= external function declarations ========================================= */
#ifndef DCM_MEMCOPY_USED_ILIB
/**
 * @brief         Dcm internal memory copy function
 * @param[out]    dest : the output data
 * @param[in]     src  : the input data
 * @param[in]     size : the input data size
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_Memcpy(uint8* dest, const uint8* src, uint32 size);

/**
 * @brief         Dcm internal memory set function
 * @param[out]    dest : the output data
 * @param[in]     data : the input data
 * @param[in]     size : date size to set
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
int DcmInternal_MemCmp(const void* ptr1, const void* ptr2, int num);

/**
 * @brief         Dcm internal memory set function
 * @param[out]    dest : the output data
 * @param[in]     data : the input data
 * @param[in]     size : date size to set
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_Memset(uint8* dest, uint8 data, uint32 size);
#else
/* API mapping for memcpy/memset */
#define DcmInternal_Memcpy(dest, src, size)  IStdLib_MemCpy(dest, src, size)
#define DcmInternal_Memset(dest, data, size) IStdLib_MemSet(dest, data, size)
#define DcmInternal_MemCmp(dest, data, size) IStdLib_MemCmp(dest, data, size)
#endif

/**
 * @brief         called by Dcm_MainFunction to process pending requests
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_ProcessRequest(void);

/**
 * @brief         called to initialize programing condition
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_ProgCtrlInit(void);

/**
 * @brief         get the SATA accorgding to protocolId
 * @param[in]     protocolId    : the input protocolId
 * @param[out]    sourceAddress : the current sourceAddress
 * @param[out]    targetAddress : the current targetAddress
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_GetMetaDataSATA(uint8 protocolId, uint16* sourceAddress, uint16* targetAddress);

/**
 * @brief         deals with timer to decrease counter
 * @param[in]     timer : the input timer
 * @return        Std_ReturnType
 * @retval        TRUE  : timeout occurs
 * @retval        FALSE : timeout does not occur
 * @reentrant     TRUE for different timer
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
/* PRQA S 3208 ++ */ /* VL_Dcm_3208 */
boolean DcmInternal_DecreaseTimer(uint32* timer);
/* PRQA S 3208 -- */

/**
 * @brief         called by Dcm_MainFunction to check timers
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_CheckTimer(void);

/**
 * @brief         called to cancel jobs for the input protocol
 * @param[in]     ProtocolId : the input protocolId
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE for different protocolId
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DcmInternal_CancelJobs(uint8 ProtocolId);

/**
 * @brief         called to find configured session Index
 * @param[in]     session      : the input sessionId
 * @param[out]    sessionIndex : the configuration index
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DcmInternal_FindSessionIndex(Dcm_SesCtrlType session, uint8* sessionIndex);

#if (STD_ON == DCM_UDS_0X27)
/**
 * @brief         called to find configured security Index
 * @param[in]     security      : input security level
 * @param[out]    securityIndex : configuration index
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_FindSecurityIndex(Dcm_SecLevelType security, uint8* securityIndex);
#endif

/**
 * @brief         called to set the programing condition
 * @param[in]     OpStatus : Indicates the current operation status
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DcmInternal_SetProgConditions(Dcm_OpStatusType OpStatus);

/**
 * @brief         called to transform uint16 data to uint8_N
 * @param[out]    dest       : the dest buffer
 * @param[in]     src        : the input data
 * @param[in]     endianness : the configured endianness
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_TransformArray_u16(uint8* dest, uint16 src, Dcm_EndiannessType endianness);

/**
 * @brief         called to transform uint24 data to uint8_N
 * @param[out]    dest       : the dest buffer
 * @param[in]     src        : the input data
 * @param[in]     endianness : the configured endianness
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_TransformArray_u24(uint8* dest, uint32 src, Dcm_EndiannessType endianness);

/**
 * @brief         called to transform uint32 data to uint8_N
 * @param[out]    dest       : the dest buffer
 * @param[in]     src        : the input data
 * @param[in]     endianness : the configured endianness
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_TransformArray_u32(uint8* dest, uint32 src, Dcm_EndiannessType endianness);

/**
 * @brief         called to transform uint64 data to uint8_N
 * @param[out]    dest       : the dest buffer
 * @param[in]     src        : the input data
 * @param[in]     endianness : the configured endianness
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_TransformArray_u64(uint8* dest, uint64 src, Dcm_EndiannessType endianness);

/**
 * @brief         called to asynchronously deal with rxIndication
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_ProcessRxIndication(void);

/**
 * @brief         called to asynchronously deal with txConfirmation
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_ProcessTxConfirmation(void);

/**
 * @brief         Allows to notify ComM the current DiagnosticState for all connections
 * @param[in]     active : notify ComM the active or inactive diagnosticState
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DcmInternal_SetActiveDiagnostic(boolean active);

#ifdef __cplusplus
}
#endif
#endif
