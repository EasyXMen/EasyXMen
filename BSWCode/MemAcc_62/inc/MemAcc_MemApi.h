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
 **  @file               : MemAcc_MemApi.h
 **  @author             : peng.wu
 **  @date               : 2025/04/04
 **  @vendor             : isoft
 **  @description        : Mem api definitions of MemAcc
 **
 ***********************************************************************************************************************/

#ifndef MEMACC_MEMAPI_H
#define MEMACC_MEMAPI_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "MemAcc_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
#define E_MEM_SERVICE_NOT_AVAIL 0x03u /**< The underlying Mem driver service function is not available. */

/* ================================================ type definitions ================================================ */
/**
 * @brief     Physical memory device address type @range 0..0xFFFFFFFF
 */
typedef MemAcc_AddressType MemAcc_MemAddressType;

/**
 * @brief     Memory driver configuration structure type @range NA
 */
typedef void MemAcc_MemConfigType;

/**
 * @brief     General data type @range 0..0xFF
 */
typedef uint8 MemAcc_MemDataType;

/**
 * @brief     Memory driver instance ID type @range 0..0xFFFFFFFF
 */
typedef uint32 MemAcc_MemInstanceIdType;

/**
 * @brief     Physical memory device length type @range NA
 */
typedef MemAcc_LengthType MemAcc_MemLengthType;

/**
 * @brief     Index type for Mem driver hardware specific service table @range 0..0xFFFFFFFF
 */
typedef uint32 MemAcc_MemHwServiceIdType;

/**
 * @brief          Function pointer for the Mem_Init service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      configPtr: Pointer to the Mem driver configuration data structure
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemInitFuncType)(MemAcc_MemConfigType* configPtr);

/**
 * @brief          Function pointer for the Mem_DeInit service for the invocation of the Mem driver API via function
 * pointer interface
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemDeInitFuncType)(void);

/**
 * @brief          Function pointer for the Mem_JobResultType service for the invocation of the Mem driver API via
 * function pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @return         MemAcc_MemJobResultType
 * @retval         MEM_JOB_OK: The last job has been finished successfully
 * @retval         MEM_JOB_PENDING: A job is currently being processed
 * @retval         MEM_JOB_FAILED: Job failed for some unspecific reason
 * @retval         MEM_INCONSISTENT: The checked page is not blank
 * @retval         MEM_ECC_UNCORRECTED: Uncorrectable ECC errors occurred during memory access
 * @retval         MEM_ECC_CORRECTED: Correctable ECC errors occurred during memory access
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef MemAcc_MemJobResultType (*MemAcc_MemGetJobResultFuncType)(MemAcc_MemInstanceIdType instanceId);

/**
 * @brief          Function pointer for the Mem_Suspend service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemSuspendFuncType)(MemAcc_MemInstanceIdType instanceId);

/**
 * @brief          Function pointer for the Mem_Resume service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemResumeFuncType)(MemAcc_MemInstanceIdType instanceId);

/**
 * @brief          Function pointer for the Mem_PropagateError service for the invocation of the Mem driver API via
 * function pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemPropagateErrorFuncType)(MemAcc_MemInstanceIdType instanceId);

/**
 * @brief          Function pointer for the Mem_Read service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @param[in]      sourceAddress: Physical address to read data from
 * @param[in]      length: Read length in bytes
 * @param[out]     destinationDataPtr: Destination memory pointer to store the read data
 * @return         Std_ReturnType
 * @retval         E_OK: The requested job has been accepted by the module
 * @retval         E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant      Non Reentrant
 * @synchronous    FALSE
 * @trace          CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*MemAcc_MemReadFuncType)(
    MemAcc_MemInstanceIdType instanceId,
    MemAcc_MemAddressType    sourceAddress,
    MemAcc_MemDataType*      destinationDataPtr,
    MemAcc_MemLengthType     length);

/**
 * @brief          Function pointer for the Mem_Write service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @param[in]      targetAddress: Physical write address (aligned to page size)
 * @param[in]      sourceDataPtr: Source data pointer (aligned to page size)
 * @param[in]      length: Write length in bytes (aligned to page size)
 * @return         Std_ReturnType
 * @retval         E_OK: The requested job has been accepted by the module
 * @retval         E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant      Non Reentrant
 * @synchronous    FALSE
 * @trace          CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*MemAcc_MemWriteFuncType)(
    MemAcc_MemInstanceIdType  instanceId,
    MemAcc_MemAddressType     targetAddress,
    const MemAcc_MemDataType* sourceDataPtr,
    MemAcc_MemLengthType      length);

/**
 * @brief          Function pointer for the Mem_Erase service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @param[in]      targetAddress: Physical erase address (aligned to sector size)
 * @param[in]      length: Erase length in bytes (aligned to sector size)
 * @return         Std_ReturnType
 * @retval         E_OK: The requested job has been accepted by the module
 * @retval         E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant      Non Reentrant
 * @synchronous    FALSE
 * @trace          CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*MemAcc_MemEraseFuncType)(
    MemAcc_MemInstanceIdType instanceId,
    MemAcc_MemAddressType    targetAddress,
    MemAcc_MemLengthType     length);

/**
 * @brief          Function pointer for the Mem_BlankCheck service for the invocation of the Mem driver API via function
 * pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @param[in]      targetAddress: Physical blank check address
 * @param[in]      length: Blank check length
 * @return         Std_ReturnType
 * @retval         E_OK: The requested job has been accepted by the module
 * @retval         E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant      Non Reentrant
 * @synchronous    FALSE
 * @trace          CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*MemAcc_MemBlankCheckFuncType)(
    MemAcc_MemInstanceIdType instanceId,
    MemAcc_MemAddressType    targetAddress,
    MemAcc_MemLengthType     length);

/**
 * @brief          Function pointer for the Mem_HwSpecificService function for the invocation of the Mem driver API via
 * function pointer interface
 * @param[in]      instanceId: ID of the related memory driver instance
 * @param[in]      hwServiceId: Hardware specific service request identifier for dispatching the request
 * @param[in]      lengthPtr: Size pointer of the passed data
 * @param[out]     dataPtr: Request specific data pointer
 * @return         Std_ReturnType
 * @retval         E_OK: The requested job has been accepted by the module
 * @retval         E_NOT_OK: The requested job has not been accepted by the module
 * @reentrant      Non Reentrant
 * @synchronous    FALSE
 * @trace          CPD-PLACEHOLDER
 */
typedef Std_ReturnType (*MemAcc_MemHwSpecificServiceFuncType)(
    MemAcc_MemInstanceIdType  instanceId,
    MemAcc_MemHwServiceIdType hwServiceId,
    MemAcc_MemDataType*       dataPtr,
    MemAcc_MemLengthType*     lengthPtr);

/**
 * @brief          Function pointer for the Mem_MainFunction service for the invocation of the Mem driver API via
 * function pointer interface
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace          CPD-PLACEHOLDER
 */
typedef void (*MemAcc_MemMainFuncType)(void);

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
