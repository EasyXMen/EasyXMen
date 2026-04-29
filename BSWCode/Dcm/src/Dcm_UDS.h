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
 **  @file               : Dcm_UDS.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS Functions Declaration and Type Definitions
 **
 ***********************************************************************************************************************/
#ifndef DCM_UDS_H_
#define DCM_UDS_H_

/* =================================================== inclusions =================================================== */
#include "Dcm_Dsp.h"
#include "Dcm_Type.h"
#if (STD_ON == DCM_UDS_0X29)
#include "KeyM.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

#define DCM_UDS3E_MINREQLEN 2u /**<The minimum request length of UDS 0x3D */
#if (STD_ON == DCM_UDS_0X86)
#define DCM_UDS86_SERVICERESP_MAXSIZE 7u /**<The length of 0x86 response record */
#endif

#define DCM_READ_MEMORY 0u /**<used to identify memory access method */

#define DCM_WRITE_MEMORY 1u /**<used to identify memory access method */

#define DCM_PIDDID_RANGE_LOWER 0xF400u /**<lower limit of PID DID range */

#define DCM_PIDDID_RANGE_UPPER 0xF4FFu /**<upper limit of PID DID range */

#define DCM_MIDDID_RANGE_LOWER 0xF600u /**<lower limit of MID DID range */

#define DCM_MIDDID_RANGE_UPPER 0xF6FFu /**<upper limit of MID DID range */

#define DCM_VEHINFODID_RANGE_LOWER 0xF800u /**<lower limit of VehInfo DID range */

#define DCM_VEHINFODID_RANGE_UPPER 0xF8FFu /**<upper limit of VehInfo DID range */

#define DCM_UDS_ECURESET_SID 0x11u /**<service id of UDS service EcuReset */

#define DCM_UDS_SESSIONCONTROL_SID 0x10u /**<service id of UDS service Diagnostic Session Control */

#define DCM_UDS_DYNDID_RANGE_LOWER 0xF200u /**<the lower limit of dynamic did */

#define DCM_UDS_DYNDID_RANGE_UPPER 0xF3FFu /**<the upper limit of dynamic did */

#define DCM_DID_LENGTH 2uL /**<The length of did */

#define DCM_UDS19_REPOMTFDTC 0xDu /**<subfunction reportMostRecentTestFailedDTC */

#define DCM_UDS19_REPOEXTDATA 6u /**<subfunction reportDTCExtDataRecordByDTCNumber */
/* ================================================ type definitions ================================================ */
#if (STD_ON == DCM_UDS_0X2C)
/**
 * @brief DDDID status enum
 */
typedef enum
{
    DCM_NOT_DEFINED,        /**<this did is not defined  @range 0x00*/
    DCM_DEFINED_SOURCE_DID, /**<this is did is defined to target a source did @range 0x01*/
    DCM_DEFINED_MEMORY_ADD  /**<this did is defined to target a memory address @range 0x02*/
} Dcm_DDDIDStatusType;

/**
 * @brief dynamically defined did definition structure
 */
typedef struct
{
    uint8  SourceDidPosition; /**<the source did positition @range 0..255*/
    uint8  SourceDidSize;     /**<the source did size  @range 0..255*/
    uint8  MemoryIdentifier;  /**<the target memory identifier @range 0..255*/
    uint16 SourceDidIndex;    /**<the source did index in configuration @range 0..65535*/
    uint32 MemoryAddress;     /**<the target memory address @range 0..0xFFFFFFFF*/
    uint32 MemorySize;        /**<the target memory size @range 0..0xFFFFFFFF*/
} Dcm_DDDIDDefineType;

/**
 * @brief dynamically defined did structure
 */
typedef struct
{
    Dcm_DDDIDStatusType DDDIDStatus[DCM_DSP_DDDID_MAX_ELEMENTS]; /**<the current definition status @range NA*/
    Dcm_MsgLenType      SourceElementsNum;                       /**<the number of sourceElements @range NA*/
    Dcm_DDDIDDefineType DDDIDDefine[DCM_DSP_DDDID_MAX_ELEMENTS]; /**< the definitions @range NA*/
} Dcm_DDDIDType;
#endif

#if (STD_ON == DCM_UDS_0X36)
/**
 * @brief UDS 0x36 transfer status enum
 */
typedef enum
{
    DCM_TRANSFER_IDLE,         /**<no transfer @range 0x00*/
    DCM_REQUEST_UPLOAD,        /**<previously request UDS 0x35 @range 0x01 */
    DCM_REQUEST_DOWNLOAD,      /**<previously request UDS 0x34 @range 0x02 */
    DCM_REQUEST_WRITEFILE,     /**<previously request UDS 0x38 Writefile @range 0x03 */
    DCM_REQUEST_READFILEORDIR, /**<previously request UDS 0x38 readFileorDirectory @range 0x04*/
} Dcm_TransferStatusType;

/**
 * @brief UDS 0x36 transfer data management unit
 */
typedef struct
{
    Dcm_TransferStatusType TransferStatus;       /**<current transfering status @range NA*/
    uint8                  BlockSequenceCounter; /**<the previous sequenceCounter @range 0..255 */
    boolean                FirstTransfer;        /**<whether it is the first transfer @range 0..1 */
    uint8                  MemoryIdentifier;     /**< the related memoryId @range 0..255*/
    uint32                 BlockLength;          /**<the previously provided blocklength @range 0..0xFFFFFFFF*/
    uint32                 LastSize;             /**<the last transfer size @range 0..0xFFFFFFFF */
    uint32                 MemorySizeRemain;     /**<the memory size remain to transfer @range 0..0xFFFFFFFF */
    uint32                 MemoryAddress;        /**<the current memory address @range 0..0xFFFFFFFF */
} Dcm_TransferDataType;
#endif

#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief UDS roe event status enum
 */
typedef enum
{
    DCM_ROE_UNDEFINED     = 0u, /**<ROE is not defined @range 0x00*/
    DCM_ROE_ONDTC         = 1u, /**<ROE is defined to be triggerd on DTC status change @range 0x01*/
    DCM_ROE_ONDID         = 3u, /**<ROE is defined to be triggerd on DID data change  @range 0x03*/
    DCM_ROE_MOSTRECENTDTC = 8u, /**<ROE is defined to be triggerd on mode recent DTC @range 0x08*/
    DCM_ROE_DTCRECORD     = 9u  /**<ROE is defined to be triggerd on DTC record @range 0x09*/
} Dcm_RoeEventType;

/**
 * @brief UDS roe current status
 */
typedef enum
{
    DCM_ROE_OFF, /**< Indicates that the UDS ROE (Routine Operation Execution) is off @range 0x00*/
    DCM_ROE_ON   /**< Indicates that the UDS ROE (Routine Operation Execution) is on @range 0x01*/
} Dcm_RoeStatusType;

/**
 * @brief UDS roe windowtime enum
 */
typedef enum
{
    DCM_ROE_WINDOWTIME_UNDEFINED = 0u, /**< Indicates that the UDS ROE window time is undefined @range 0x00*/
    DCM_ROE_WINDOWTIME_INFINITE  = 2u, /**< Indicates that the UDS ROE window time is infinite @range 0x02*/
    DCM_ROE_WINDOWTIME_POWER     = 6u  /**< Indicates that the UDS ROE window time is related to power @range 0x06*/
} Dcm_RoeEventWindowTimeType;

/**
 * @brief Dcm roe control unit
 */
typedef struct RoeCtrlType
{
    Dcm_RoeEventType           RoeEvent;                           /**<requested RoeEvent @range NA*/
    Dcm_RoeStatusType          RoeStatus;                          /**<current status of roe @range NA*/
    Dcm_RoeEventWindowTimeType RoeEventWindowTime;                 /**<requested RoeEventWindowTime @range NA*/
    uint8 RoeServiceResponseRecord[DCM_UDS86_SERVICERESP_MAXSIZE]; /**<previously requested RoeServiceResponseRecord
                                                                      @range NA*/
    Dcm_MsgLenType
              RoeServiceResponseRecordSize; /**<the size of previously requested RoeServiceResponseRecord @range NA*/
    uint8     DTCMask;                      /**<the previously requested DTCMask @range 0..255*/
    uint16    Did;                          /**<the previously requested did  @range 0..65535*/
    boolean   PendingServiceReq;            /**<there is a pending service request @range 0..1*/
    PduIdType RoePduId;                     /**<roe protocolrow rxpdu id @range NA*/
#ifdef DCM_UDS_0X86_0X3
    uint8 DidSignalData[DCM_ROE_MAX_DID_LENGTH]; /**<did signal Data @range NA*/
#endif
} Dcm_RoeCtrlType;
#endif

/**
 * @brief Dcm did type enum
 */
typedef enum
{
    DCM_OBD_DID,     /**< Represents an On - Board Diagnostics (OBD) Diagnostic Identifier @range 0x00 */
    DCM_REGULAR_DID, /**< Represents a regular Diagnostic Identifier @range 0x01*/
    DCM_RANGE_DID,   /**< Represents a range - based Diagnostic Identifier @range 0x02*/
} Dcm_DidType;

#if (STD_ON == DCM_UDS_0X29)
/**
 * @brief authentication process status enum
 */
typedef enum
{
    DCM_ASYNC_IDLE,                         /**<Authentication async process is idle. @range 0x00*/
    DCM_KEYM_VERIFY_ASYNC_PENDING,          /**<Key management verify async process is pending @range 0x00*/
    DCM_KEYM_VERIFY_ASYNC_FINISH,           /**<Key management verify async process is finished @range 0x01*/
    DCM_CSM_GENERATE_ASYNC_PENDING,         /**<CSM generation async process is pending @range 0x02*/
    DCM_CSM_GENERATE_ASYNC_FINISH,          /**<CSM generation async process is finished @range 0x03*/
    DCM_CSM_SIGNATURE_ASYNC_PENDING,        /**<CSM signature async process is pending @range 0x04*/
    DCM_CSM_SIGNATURE_ASYNC_FINISH,         /**<CSM signature async process is finished @range 0x05*/
    DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING, /**<CSM signature verify async process is pending @range 0x06*/
    DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC,  /**<CSM has finished signature verify async process @range 0x07*/
} Dcm_AuthenticateProcessStatusTypes;
#endif
/* =========================================== external data declarations =========================================== */
#if (STD_ON == DCM_UDS_0X2C)
#if (DCM_DDDID_NUM > 0u)
/**
 * @brief The dynamic defined did manager structure @range NA
 */
/* PRQA S 3208,1037 ++ */ /* VL_Dcm_3208,VL_Dcm_1037 */
extern Dcm_DDDIDType Dcm_DDDID[DCM_DDDID_NUM];
#endif
#endif
#if (STD_ON == DCM_UDS_0X2A)
#if (DCM_PERIODIC_CONNECTION_NUM > 0u)
/**
 * @brief used to check txConfirmation, set to true when sent and false when confirmed @range NA
 */
extern boolean Dcm_PeriodicConnectionSent[DCM_PERIODIC_CONNECTION_NUM];
/* PRQA S 3208,1037 -- */
#endif
#endif
#if (STD_ON == DCM_UDS_0X36)
/**
 * @brief The data transfer control unit @range NA
 */
extern Dcm_TransferDataType Dcm_TransferData;
#endif
#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief total requested number roe @range 0..255
 */
extern uint8 Dcm_RoeCtrlNum;
/**
 * @brief roe control unit @range NA
 */
extern Dcm_RoeCtrlType Dcm_RoeCtrl[DCM_MAX_ROE_DID + DCM_MAX_ROE_DTC];
#endif
#if (STD_ON == DCM_UDS_0X2E)
#if (STD_ON == DCM_USE_BNDM)
/**
 * @brief store the BndM result for asynchronous BndM write @range NA
 */
extern BndM_ResultType Dcm_BndMResult[DCM_BNDMDID_NUM];
#endif
#ifdef DCM_SECURE_CODING_DID_NUM
/**
 * @brief store the secure coding did offset of the secure coding buffer @range NA
 */
extern uint32 Dcm_SecureCodingDidOffset[DCM_SECURE_CODING_DID_NUM];
/**
 * @brief store the current secure coding total data length @range NA
 */
extern Dcm_MsgLenType Dcm_SecureCodingDataLength;
/**
 * @brief secure coding buffer @range NA
 */
extern uint8 Dcm_SecureCodingBuffer[DCM_SECURE_CODING_DATA_BUFFER_SIZE];
#endif
#endif
#if (STD_ON == DCM_UDS_0X11)
/**
 * @brief indicate processing ecuReset @range 0..1
 */
extern boolean Dcm_ResetProcessing;
#endif
#if (STD_ON == DCM_UDS_0X14)
/**
 * @brief indicate processing clearingDiagInfo @range 0..1
 */
extern boolean Dcm_ProcessingClearingDiagInfo;
#endif
#if (STD_ON == DCM_UDS_0X29)
/**
 * @brief Csm result for asynchronous Csm request @range NA
 */
extern Crypto_ResultType Dcm_CsmResult;
/**
 * @brief CsmJobId for asynchronous Csm request @range 0..0xFFFFFFFF
 */
extern uint32 Dcm_CsmJobId;
/**
 * @brief async authenticate process status @range NA
 */
extern Dcm_AuthenticateProcessStatusTypes Dcm_AuthenticateAsync;
#endif
#ifdef DCM_UDS_0X29_0X4
/**
 * @brief whether finished transmitting certificate @range 0..1
 */
extern boolean Dcm_TransmitCertificate;
#endif
#if (STD_ON == DCM_UDS_0X27)
/**
 * @brief store the previous requested seed security level @range 0..255
 */
extern uint8 Dcm_reqSeedSecurityLevel;
#endif
/* ========================================= external function declarations ========================================= */
/**
 * @brief         The service interpreter for UDS 0x10
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Session Type and current session level
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x10(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         Called by Dcm_MainFunction/DcmInternal_CheckTimer to deal with session timer
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x10_CheckTimer(void);
/**
 * @brief         Called by Dcm_TpTxConfirmation for 0x10 service postivie response confirmation and
 *                ResetToDefaultSession to change the session
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x10_SessionChange(void);
/**
 * @brief         Called by DsdInternal_RxIndication to check general service/subService session and most of th UDS
 *                services to check did, rid, memory, etc. session.
 * @param[in]     session    : the required session
 * @param[in]     sessionNum : the required session num
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType
    Dcm_UDS0x10_CheckSession(const Dcm_SesCtrlType* session, uint8 sessionNum, Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_UDS_0X11)
/**
 * @brief         The service interpreter for UDS 0x11
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Configuration DcmSendRespPendOnRestart
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x11(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X14)
/**
 * @brief         The service interpreter for UDS 0x14
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x14(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X19)
/**
 * @brief         The service interpreter for UDS 0x19
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                             pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x19(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X22)
/**
 * @brief         The service interpreter for UDS 0x22
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x22(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X23)
/**
 * @brief         The service interpreter for UDS 0x23
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x23(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X24)
/**
 * @brief         The service interpreter for UDS 0x24
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x24(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X27)
/**
 * @brief         The service interpreter for UDS 0x27
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x27(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         This function checks whether the current security State is supported
 * @param[in]     security    : the requested security
 * @param[in]     securityNum : the requested security number
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x27_CheckSecurity(
    const Dcm_SecLevelType*       security,
    uint8                         securityNum,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         This function checks the security timer and decrease counter
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x27_CheckTimer(void);

/**
 * @brief         This function calls the SetAttemptCounterFnc and actually sets the attemptCounter
 * @param[in]     OpStatus       : Indicates the current operation status
 * @param[in]     securityIndex  : the cofigured securityRow index
 * @param[in]     attemptCounter : the input attemptCounter to set
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x27_SetAttemptCounter(Dcm_OpStatusType OpStatus, uint8 securityIndex, uint8 attemptCounter);
#if (STD_ON == DCM_SECURITY_READOUT)
/**
 * @brief         This function is called by Dcm_MainFunction to process the getAttemptCounter
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x27_GetAttemptCounter(void);
/**
 * @brief         This function is called by Dcm_MainFunction to process the setAttemptCounter
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x27_SetAttemptCounterHandle(void);
#endif
#endif

#if (STD_ON == DCM_UDS_0X31)
/**
 * @brief         The service interpreter for UDS 0x31
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x31(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X34)
/**
 * @brief         The service interpreter for UDS 0x34
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x34(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X35)
/**
 * @brief         The service interpreter for UDS 0x35
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was not successful
 * @retval        DCM_E_PENDING: Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x35(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X37)
/**
 * @brief         The service interpreter for UDS 0x37
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x37(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X38)
/**
 * @brief         The service interpreter for UDS 0x38
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x38(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X85)
/**
 * @brief        The service interpreter for UDS 0x85
 * @param[in]    OpStatus    : Indicates the current operation status
 * @param[inout] pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                             pMsgContext shall point behind the SID.
 * @param[out]   ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Dem
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x85(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         enables the DTC setting (session change or 85 req)
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x85_EnableDTCSetting(void);
#if (STD_ON == DCM_UDS_0X85_MODERULE)
/**
 * @brief         enables the DTC setting when mode rule fails
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x85_MainFunction(void);
#endif
#endif

#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief         The service interpreter for UDS 0x86
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x86(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X2A)
/**
 * @brief         The service interpreter for UDS 0x2A
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x2A(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (DCM_PERIODIC_CONNECTION_NUM > 0u)
/**
 * @brief         Called by Dcm_MainFunction to deal with 2A scheduler counter and message sending
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x2A_MainFunction(void);
#endif

/**
 * @brief         Called by UDS 0x10, 0x29 and 0x27 to notify the session/security/authenticationState change so as to
 *                check the access right again. Stop the periodic transmission if access right is lost.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x2A_StatusChangeHandle(void);

/**
 * @brief         Remove the corresponding DID reading service from the 0x2A service dispatch table.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x2A_Remove_Scheduler(uint16 DidIndex);

#endif

#if (STD_ON == DCM_UDS_0X2C)
/**
 * @brief         The service interpreter for UDS 0x2C
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x2C(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X2E)
/**
 * @brief         The service interpreter for UDS 0x2E
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x2E(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Called to write did data
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     DcmDspDidCfg : the DcmDspDid configuration
 * @param[in]     reqData      : the requested data to write
 * @param[in]     reqDataLen   : the requested data length to write
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x2E_WriteDspData(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDidType*         DcmDspDidCfg,
    const uint8*                  reqData,
    Dcm_MsgLenType                reqDataLen,
    Dcm_NegativeResponseCodeType* ErrorCode);

#ifdef DCM_SECURE_CODING_DID_NUM
/**
 * @brief         Called by Dcm_Init to initialize secure coding did offset
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x2E_Init(void);
#endif
#endif

#if (STD_ON == DCM_UDS_0X2F)
/**
 * @brief         The service interpreter for UDS 0x2F
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x2F(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (DCM_DID_NUM > 0)
/**
 * @brief         Called by UDS 0x2F and 0x10 to notify session/security/authenticationState change and call
 *                returnControlToEcu accordingly if it is no long supported in current status
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x2F_StatusChangeHandle(Dcm_SesCtrlType newSession);
#endif
#endif

#if ((STD_ON == DCM_UDS_0X23) || (STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X2C))
/**
 * @brief         Called by UDS 0x2C, 0x3D, 0x23, 0x34, 0x35 to check whether the input memoryAdd and memorySize is
 *                valid
 * @param[in]     MemoryCfg        : the target memory configuration ptr
 * @param[in]     AddAndLenFid     : the requested address and length format id
 * @param[in]     byteNumber       : the requested address and length format id
 * @param[in]     reqData          : the input request data
 * @param[out]    MemoryIdentifier : the configured memoryIdentifier (if available)
 * @param[out]    MemoryAddress    : the extracted memoryAddress from input data
 * @param[out]    MemorySize       : the extracted MemorySize from input data
 * @param[in]     memoryAccess     : read/write memory access
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS_CheckMemory(
    const Dcm_DspMemoryType*      MemoryCfg,
    uint8                         AddAndLenFid,
    Dcm_MsgLenType                byteNumber,
    uint8*                        reqData,
    uint8*                        MemoryIdentifier,
    uint32*                       MemoryAddress,
    uint32*                       MemorySize,
    uint8                         memoryAccess,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_UDS_0X34) || (STD_ON == DCM_UDS_0X35))
/**
 * @brief         Called by UDS 0x34, 0x35 to set the lengthFormatIdentifier and blockLength to resData accordingly
 * @param[in]     BlockLength : the input blockLength
 * @param[out]    ResData     : the output data
 * @param[out]    ResDataLen  : the response data length
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS_SetBlockLength(uint32 BlockLength, uint8* ResData, Dcm_MsgLenType* ResDataLen);
#endif

#if (STD_ON == DCM_UDS_0X36)
/**
 * @brief         The service interpreter for UDS 0x36
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x36(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         Called by UDS 0x34, 0x35 and 0x37 to initialize transferData status
 * @param[in]     transferStatus   : current transfer status
 * @param[in]     memoryIdentifier : configured memory id
 * @param[in]     memoryAddress    : input memory address
 * @param[in]     blockLength      : max blockLength returned by callout
 * @param[in]     memorySize       : input memory size
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS_InitTransferData(
    Dcm_TransferStatusType transferStatus,
    uint8                  memoryIdentifier,
    uint32                 memoryAddress,
    uint32                 blockLength,
    uint32                 memorySize);
#endif

#if (STD_ON == DCM_UDS_0X3D)
/**
 * @brief         The service interpreter for UDS 0x3D
 * @param[in]     OpStatus : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     FALSE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x3D(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X3E)
/**
 * @brief         The service interpreter for UDS 0x3E
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x3E(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X24) || (STD_ON == DCM_UDS_0X2C))
/**
 * @brief         Called by UDS 0x22, 0x24, 0x2A, 0x2C and 0x2E to check whether this did is supported to read/write in
 *                the current session/security/mode/authenticationState
 * @param[in]     ConnectionId       : the current connectionId
 * @param[in]     Did                : the input did
 * @param[out]    DcmDspDidReadWrite : did's read/write configuration
 * @param[out]    ErrorCode          : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                     module shall send a negative response with NRC code equal to the parameter
 *                                     ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS_CheckReadWriteDid(
    uint16                         ConnectionId,
    uint16                         Did,
    const Dcm_DspDidReadWriteType* DcmDspDidReadWrite,
    Dcm_NegativeResponseCodeType*  ErrorCode);
#endif

#if (                                                                                                            \
    (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F) || (STD_ON == DCM_UDS_0X86) || (STD_ON == DCM_UDS_0X24) \
    || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X2A))
/**
 * @brief         This function gets the configured did index, also applying to rangeDid and OBDDid.
 * @param[in]     Did       : the input did
 * @param[out]    didType   : the type of did
 * @param[out]    DidIndex  : the did configuration index
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module shall
 *                            send a negative response with NRC code equal to the parameter ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was unsuccessful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType
    Dcm_UDS_FindDid(uint16 Did, Dcm_DidType* didType, uint16* DidIndex, Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2F))
/**
 * @brief         Called by UDS 0x22/0x2A/0x2F to read data in did's signals
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     DidIndex   : the input did index
 * @param[out]    DestBuffer : address to output the signal data
 * @param[out]    DataLength : the data length of did (only for dynamic ones)
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS_SignalHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DidIndex,
    uint8*                        DestBuffer,
    uint16*                       DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Called by UDS 0x22 to read data in did's signals
 * @param[in]     OpStatus   : Indicates the current operation status
 * @param[in]     DidIndex   : the input did index
 * @param[out]    DestBuffer : address to output the signal data
 * @param[inout]  BufSize    : When the function is called this parameter contains the maximum number of data bytes that
 *                             can be written to the buffer. The function returns the actual number of written data
 *                             bytes in this parameter.
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS_DidReadHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        DidIndex,
    uint8*                        DestBuffer,
    Dcm_MsgLenType*               BufSize,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_DYN_DID)
/**
 * @brief         Called by UDS 0x22 and 0x2A to read data of a dynamic did
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     ConnectionId : the current connection
 * @param[in]     DDDIdIndex   : the index of the dynamic defined did
 * @param[out]    DestBuffer   : the did's output data
 * @param[inout]  BufSize      : When the function is called this parameter contains the maximum number of data bytes
 *                               that can be written to the buffer. The function returns the actual number of written
 *                               data bytes in this parameter.
 * @param[out]    checkFail    : whether the source did check fails
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS_DynDidHandle(
    Dcm_ExtendedOpStatusType OpStatus,
    uint16                   ConnectionId,
    uint16                   DDDIdIndex,
    uint8*                   DestBuffer,
    Dcm_MsgLenType*          BufSize,
#if (STD_ON == DCM_DDDID_CHECK_SOURCE)
    boolean* checkFail,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#endif

#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief         called by Dcm_Init to initialize roe status, only apply for nvm stored roe data
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x86_Init(void);
/**
 * @brief         called when DTC Status change or did written to trigger established service request
 * @param[in]     roeCtrlPtr : the related roeCtrl unit
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x86_TriggerServiceRequest(Dcm_RoeCtrlType* roeCtrlPtr);
/**
 * @brief         called by Dcm_MainFunction to check scheduler rate
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x86_CheckTimer(void);
#endif

#if (STD_ON == DCM_UDS_0X28)
/**
 * @brief         The service interpreter for UDS 0x28
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x28(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         This function switch the comMode and calls the releated function when txConfirmation
 * @param[in]     ConnectionId : the current connectionId
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x28_TxConfirmation(uint16 ConnectionId);

#if ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0))
/**
 * @brief         This function is called when session is changed and it enables the communication accordingly
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x28_SessionChange(void);
/**
 * @brief         This function is called by Dcm_MainFunction when UDS 0x28 is configured with modeRule, it checks
 *                whether the mode rule is no longer effective and enables the communication again
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x28_MainFunction(void);
#endif
#endif

#if (STD_ON == DCM_UDS_0X29)
/**
 * @brief         The service interpreter for UDS 0x29
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x29(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         This function initialize the variables used by UDS 0x29
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x29_Init(void);
/**
 * @brief         called by Dcm_MainFunction to handle timer
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
void Dcm_UDS0x29_MainFunction(void);
/**
 * @brief         called by DsdInternal_RxIndication to verify service and subfunction authentication
 * @param[in]     ConnectionId  : the current connection
 * @param[in]     subRoleRef    : the input role ref
 * @param[in]     subRoleRefNum : the number of input role ref
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
#endif
Std_ReturnType Dcm_UDS0x29_AuthenticationCheck(
    uint16                        ConnectionId,
    const uint8*                  subRoleRef,
    uint8                         subRoleRefNum,
    Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         This function switches the authenticationStatus to DCM_DEAUTHENTICATED and restore status accordingly
 * @param[in]     ConnectionId : the current connectionId
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x29_SetDeAuthenticate(uint16 ConnectionId);
#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
/**
 * @brief         This function is called by Dcm_TpTxConfirmation to actually set the authenticationState on
 * @param[in]     ConnectionId : the current connection Id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x29_SetAuthenticationCtrlOn(uint16 ConnectionId);
#endif
/**
 * @brief         This function is called by Dcm_StartOfReception to actually set the authenticationState off
 * @param[in]     ConnectionId : the current connection Id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void Dcm_UDS0x29_SetAuthenticationCtrlOff(uint16 ConnectionId);

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F))
/**
 * @brief         called by UDS 0x22, 0x24, 0x2A, 0x2C, 0x2E, 0x2F to check did authentication
 * @param[in]     ConnectionId : the current connection
 * @param[in]     Did          : the input did
 * @param[in]     didReadWrite : the input did read/write info
 * @param[in]     didControl   : the input did control info
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x29_DidAuthenticationCheck(
    uint16                         ConnectionId,
    uint16                         Did,
    const Dcm_DspDidReadWriteType* didReadWrite,
    const Dcm_DspDidControlType*   didControl,
    Dcm_NegativeResponseCodeType*  ErrorCode);
#endif

#if (STD_ON == DCM_UDS_0X31)
/**
 * @brief         called by UDS 0x31 to check rid authentication
 * @param[in]     ConnectionId  : the current connection
 * @param[in]     RoutineCfgIdx : the target rid configuration index
 * @param[out]    ErrorCode     : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                parameter value.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType Dcm_UDS0x29_RidAuthenticationCheck(
    uint16                        ConnectionId,
    uint16                        RoutineCfgIdx,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
