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
 **  @file               : Dcm_UDS0x29.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x29 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X29)
#if ((STD_ON == DCM_UDS_0X29_0X3) || (STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
#include "Csm.h"
#endif
#if ((defined SEV_UDS_CERTIFICATE_FAILURE) || (defined SEV_UDS_AUTHENTICATION_SUCCESSFUL))
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x29
 */
#define DCM_UDS0X29_MINREQLEN 1u
/**
 * @brief The subfunction deAuthenticate
 */
#define DCM_DEAUTHENTICATE 0u
#if ((STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * @brief The subfunction verifyCertificateUnidirectional
 */
#define DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL 1u
/**
 * @brief The subfunction verifyCertificateBidirectional
 */
#define DCM_VERIFYCERTIFICATE_BIDIRECTIONAL 2u
#endif
#ifdef DCM_UDS_0X29_0X3
/**
 * @brief The subfunction proofOfOwnership
 */
#define DCM_PROOFOFOWNERSHIP 3u
#endif
#ifdef DCM_UDS_0X29_0X4
/**
 * @brief The subfunction transmitCertificate
 */
#define DCM_TRANSMITCERTIFICATE 4u
#endif
#ifdef DCM_UDS_0X29_0X8
/**
 * @brief The subfunction authenticationConfiguration
 */
#define DCM_AUTHENTICATION_CONFIGURATION 8u
#endif
/**
 * @brief DeAuthentication successful.
 */
#define DCM_DEAUTHENTICATION_SUCCESSFUL 0x10u
#ifdef DCM_UDS_0X29_0X8
/**
 * @brief AuthenticationConfiguration APCE
 */
#define DCM_AUTHENTICATION_APCE 2u
#endif
#if ((STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * @brief Certificate verified, Ownership verification necessary
 */
#define DCM_AUTHENTICATION_CVOVN 0x11u
#endif
#ifdef DCM_UDS_0X29_0X3
/**
 * @brief Ownership verified, Authentication complete
 */
#define DCM_AUTHENTICATION_CVAC 0x12u
#endif
#ifdef DCM_UDS_0X29_0X4
/**
 * @brief Certificate could be verified.
 */
#define DCM_AUTHENTICATION_CV 0x13u
#endif
#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F))
/**
 * @brief Bit 0: Read access
 */
#define DCM_DID_READ_ACCESS 0x01u
/**
 * @brief Bit 1: Write access
 */
#define DCM_DID_WRITE_ACCESS 0x02u
/**
 * @brief Bit2: Control access (by InputOutputControlByIdentifier)
 */
#define DCM_DID_CONTROL_ACCESS 0x04u
#endif
#if (STD_ON == DCM_UDS_0X31)
/**
 * @brief Bit 0: Access to sub-function startRoutine
 */
#define DCM_RID_START_ACCESS 0x01u
/**
 * @brief Bit1: Access to sub-function stopRoutine
 */
#define DCM_RID_STOP_ACCESS 0x02u
/**
 * @brief Bit2: Access to sub-function requestRoutineResult
 */
#define DCM_RID_REQUEST_ACCESS 0x04u
#endif
/* ================================================ type definitions ================================================ */
/**
 * @brief authentication State enum
 */
typedef enum
{
    DCM_AUTHENTICATIONTIMER_ON  = 0,
    DCM_AUTHENTICATIONTIMER_OFF = 1
} Dcm_AuthenticationStateType;

/**
 * @brief authentication ctrl structure type
 */
typedef struct AuthenticationCtrlType
{
    /* Authenticationtimer timer */
    uint32 AuthenticationCurTimer;
    /* Authenticationtimer status */
    Dcm_AuthenticationStateType AuthenticationState;
} Dcm_AuthenticationCtrlType;

/**
 * @brief authentication status enum
 */
typedef enum
{
    DCM_DEAUTHENTICATED  = 1,
    DCM_PRE_AUTHENTICATE = 2,
    DCM_AUTHENTICATE     = 3,
} Dcm_AuthenticateStatusType;

#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
/**
 * @brief whitelist structure type
 */
typedef struct WhiteListType
{
#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
    uint8 WhiteListDidLength;
    uint8 WhiteListDid[DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE];
#endif
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
    uint8 WhiteListRidLength;
    uint8 WhiteListRid[DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE];
#endif
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
    uint8 WhiteListServicesNum;
    uint8 WhiteListServiceslength[DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE];
    uint8 WhiteListServices[DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE];
#endif
} Dcm_WhiteListType;
#endif

/**
 * @brief authentication info structure type
 */
typedef struct AuthenticateInfoType
{
    Dcm_AuthenticateStatusType AuthenticateStatus;
    uint32                     Certificate;
#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
    Dcm_WhiteListType WhiteList;
#endif
    uint32 ProofOfOwnershipServerLength;
    uint32 CertDataLength;
} Dcm_AuthenticateInfoType;

/**
 * @brief KeyM Certificate info structure type
 */
typedef struct KeyMCertInfoType
{
    KeyM_CertificateIdType CertId;
    KeyM_CertElementIdType CertElementId;
    uint8                  WhiteListMaxSize;
    uint8*                 WhiteListNum;
} Dcm_KeyMCertInfoType;
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief authentication ctrl structure @range NA
 */
DCM_LOCAL Dcm_AuthenticationCtrlType Dcm_AuthenticationCtrl[DCM_MAINCONNECTION_NUM];
/**
 * @brief authentication info structure @range NA
 */
DCM_LOCAL Dcm_AuthenticateInfoType Dcm_AuthenticateInfo[DCM_MAINCONNECTION_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
/**
 * @brief KeyM cerfication id for asynchronous KeyM request @range NA
 */
DCM_LOCAL KeyM_CertificateIdType Dcm_KeyMCertID;
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#if ((STD_ON == DCM_UDS_0X29_0X3) || (STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * @brief configured challengeLength @range 0..0xFFFFFFFF
 */
DCM_LOCAL uint32 Dcm_ChallengeLength;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief KeyM result for asynchronous KeyM request @range NA
 */
DCM_LOCAL KeyM_CertificateStatusType Dcm_KeyMResult;
/**
 * @brief Csm verify result for asynchronous Csm request @range NA
 */
DCM_LOCAL Crypto_VerifyResultType Dcm_CsmVerifyResult;
#if ((STD_ON == DCM_UDS_0X29_0X3) || (STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * @brief to store challengeData @range NA
 */
DCM_LOCAL uint8 Dcm_challengeData[DCM_DSP_AUTHENTICATION_MAX_ECU_CHALLENGE_LENGTH];
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#ifdef DCM_UDS_0X29_0X4
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_TransmitCertificate;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief Csm result for asynchronous Csm request @range NA
 */
Crypto_ResultType Dcm_CsmResult;
/**
 * @brief async authenticate process status
 */
Dcm_AuthenticateProcessStatusTypes Dcm_AuthenticateAsync;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
/**
 * @brief CsmJobId for asynchronous Csm request @range NA
 */
uint32 Dcm_CsmJobId;
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
#ifdef DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID
/**
 * @brief         called to store the current authentication State
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x29_AuthenticateStore(void);
#endif

#if ((STD_ON == DCM_UDS_0X29_0X3) || (STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
/**
 * @brief         called to set to configured nrc (if have)
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module shall
 *                            send a negative response with NRC code equal to the parameter ErrorCode parameter value.
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x29_SetNrc(Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#endif
/**
 * @brief         called to set authentication role
 * @param[in]     connectionId : the current connectionId
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x29_SetDeAuthenticateRole(uint16 connectionId);
#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
/**
 * @brief         This function clears the white list
 * @param[in]     connectionId : the current connectionId
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x29_ClearWhiteList(uint16 connectionId);
#endif
/**
 * @brief         The service interpreter for UDS 0x29 subfunction 0x00
 * @param[in]     ConnectionId : the current connection id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x29_0x00(uint16 ConnectionId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);

#if ((STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * @brief         Checkes KeyM asynchronous result and set errorCode accordingly
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module shall
 *                            send a negative response with NRC code equal to the parameter ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_KeyMAsyncCheck(Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         This function deals with challenge and output accordingly
 * @param[in]     ConnectionId  : the current connection Id
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_Challenge(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         This function deals with Csm Generate asynchronous result
 * @param[in]     ConnectionId : the current connection Id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_HandleCsmGenerateResult(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         This function deals with Csm Signature asynchronous result
 * @param[in]     ConnectionId : the current connection Id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_HandleCsmSignatureResult(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         The service interpreter for UDS 0x29 subfunction 0x01 or 0x02
 * @param[in]     OpStatus     : Indicates the current operation status
 * @param[in]     ConnectionId : the current connection Id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_01_02(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Handle UDS 0x29 subfunction 0x01 or 0x02 when opStatus is DCM_INITIAL
 * @param[in]     ConnectionId : the current connection Id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers in
 *                               pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                               shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                               value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_01_02_InitialHandle(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X29_0X3
#if (                                                                                                                \
    (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE))
/**
 * @brief         gets the specific lists
 * @param[in]     Dcm_KeyMCertInfo : the input cerfication info
 * @param[out]    DataPtr          : the dest buffer
 * @param[out]    DataLength       : the total data length
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_getlist(
    Dcm_KeyMCertInfoType          Dcm_KeyMCertInfo,
    uint8*                        DataPtr,
    uint8*                        DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
/**
 * @brief         gets the white lists
 * @param[in]     ConnectionId : the current connection Id
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_whitelist(uint16 ConnectionId, Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/**
 * @brief         updates the authentication State
 * @param[in]     ConnectionId : the current connection Id
 * @param[out]    ErrorCode  : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                             shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                             value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_Update(uint16 ConnectionId, Dcm_NegativeResponseCodeType* ErrorCode);
/**
 * @brief         The service interpreter for UDS 0x29 subfunction 0x03
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     ConnectionId  : the current connection Id
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X29_0X4
/**
 * @brief         The service interpreter for UDS 0x29 subfunction 0x04
 * @param[in]     OpStatus       : Indicates the current operation status
 * @param[inout]  pMsgContext    : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                 pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode      : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                 shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                 parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x04(
    Dcm_OpStatusType              OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         Handles interaction with KeyM for UDS 0x29 subfunction 0x04
 * @param[in]     certData  : the input certData
 * @param[in]     CertId    : the input certificate evaluation id
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the
 *                            Dcm module shall send a negative response with NRC code equal to the
 *                            parameter ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request was not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x29_0x04_KeyMProcess(KeyM_CertDataType* certData, uint16 CertId, Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check conditions for subfunction 0x04
 * @param[inout]  pMsgContext            : Message-related information for one diagnostic protocol identifier. The
 *                                         pointers in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode              : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                         module shall send a negative response with NRC code equal to the parameter
 *                                         ErrorCode parameter value.
 * @param[out]    TransmitCertificateRef : The configured cerficateRef
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x04_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       TransmitCertificateRef);
#endif

#ifdef DCM_UDS_0X29_0X8
/**
 * @brief         The service interpreter for UDS 0x29 subfunction 0x08
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x08(Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/**
 * @brief         checks whether the role is validated
 * @param[in]     ConnectionId : the current connection Id
 * @param[in]     RoleRef      : the input role ref
 * @param[in]     RoleNum      : the number of role ref
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_RoleCheck(uint16 ConnectionId, const uint8* RoleRef, uint8 RoleNum);
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
/**
 * @brief         checks whether the service is in white list
 * @param[in]     ConnectionId : the current connection Id
 * @param[in]     CheckLength  : the input check length
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_WhiteListCheck(uint16 ConnectionId, Dcm_MsgLenType CheckLength);
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F))
#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
/**
 * @brief         checks whether the did is in white list
 * @param[in]     ConnectionId : the current connection Id
 * @param[in]     RecDid       : the input did
 * @return        Std_ReturnType
 * @retval        E_OK: Request was successful
 * @retval        E_NOT_OK: Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_DidWhiteListCheck(uint16 ConnectionId, uint16 RecDid);
#endif
#endif

#if (STD_ON == DCM_UDS_0X31)
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
/**
 * @brief         checks whether the rid is in white list
 * @param[in]     ConnectionId : the current connection Id
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_RidWhiteListCheck(uint16 ConnectionId);
#endif
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x29
 */
Std_ReturnType Dcm_UDS0x29(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8          subFunc = pMsgContext->reqData[0u];
    Std_ReturnType result;

    if ((DCM_INITIAL != OpStatus) || (DCM_ASYNC_IDLE == Dcm_AuthenticateAsync))
    {
        uint16 connectionId;
        (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
        switch (subFunc)
        {
        case DCM_DEAUTHENTICATE:
        {
            /*deAuthenticate*/
            result = Dcm_UDS0x29_0x00(connectionId, pMsgContext, ErrorCode);
            break;
        }
#if ((STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
        case DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL:
        case DCM_VERIFYCERTIFICATE_BIDIRECTIONAL:
        {
            result = Dcm_UDS0x29_01_02(OpStatus, connectionId, pMsgContext, ErrorCode);
            break;
        }
#endif
#ifdef DCM_UDS_0X29_0X3
        case DCM_PROOFOFOWNERSHIP:
        {
            /*proofOfOwnership*/
            result = Dcm_UDS0x29_0x03(OpStatus, connectionId, pMsgContext, ErrorCode);
            break;
        }
#endif
#ifdef DCM_UDS_0X29_0X4
        case DCM_TRANSMITCERTIFICATE:
        {
            /*transmitCertificate*/
            result = Dcm_UDS0x29_0x04(OpStatus, pMsgContext, ErrorCode);
            break;
        }
#endif
#ifdef DCM_UDS_0X29_0X8
        case DCM_AUTHENTICATION_CONFIGURATION:
        {
            /*authenticationConfiguration*/
            result = Dcm_UDS0x29_0x08(pMsgContext, ErrorCode);
            break;
        }
#endif
        default:
        {
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            result     = E_NOT_OK;
            break;
        }
        }
        if ((E_OK == result) && ((1u + pMsgContext->resDataLen) > pMsgContext->resMaxDataLen))
        {
            /* check tx data length */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
    }
    else
    {
        *ErrorCode = DCM_E_BUSYREPEATREQUEST;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        pMsgContext->resData[0u] = subFunc;
        pMsgContext->resDataLen++;
    }

    return result;
}

/**
 * set deauthentication for targeted connection
 */
void Dcm_UDS0x29_SetDeAuthenticate(uint16 ConnectionId)
{
    Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus    = DCM_DEAUTHENTICATED;
    Dcm_AuthenticationCtrl[ConnectionId].AuthenticationState = DCM_AUTHENTICATIONTIMER_OFF;
    Dcm_UDS0x29_SetDeAuthenticateRole(ConnectionId);
#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
    Dcm_UDS0x29_ClearWhiteList(ConnectionId);
#endif
#ifdef DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID
    Dcm_UDS0x29_AuthenticateStore();
#endif
    Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
}

/**
 * initialize variables for UDS 0x29
 */
void Dcm_UDS0x29_Init(void)
{
    Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
    Dcm_CsmVerifyResult   = CRYPTO_E_VER_OK;

#ifdef DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID
    Dcm_NegativeResponseCodeType ErrorCode;
    if (((NULL_PTR != Dcm_DspAuthentication.PersistStateModeRuleRef)
         && (E_OK != Dcm_DspAuthentication.PersistStateModeRuleRef(&ErrorCode))))
    {
        for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
        {
            Dcm_UDS0x29_SetDeAuthenticate(index);
        }
    }
#else
    for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
    {
        Dcm_UDS0x29_SetDeAuthenticate(index);
    }
#endif
    for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
    {
        Dcm_UDS0x29_SetAuthenticationCtrlOff(index);
#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
        Dcm_UDS0x29_SetAuthenticationCtrlOn(index);
#endif
    }
}

/**
 * callback function for asynchronous KeyM jobs finish notification
 */
Std_ReturnType Dcm_KeyMAsyncCertificateVerifyFinished(KeyM_CertificateIdType CertID, KeyM_CertificateStatusType Result)
{
    if (Dcm_KeyMCertID == CertID)
    {
        Dcm_KeyMResult        = Result;
        Dcm_AuthenticateAsync = DCM_KEYM_VERIFY_ASYNC_FINISH;
    }
    return E_OK;
}

/**
 * callback function for asynchronous Csm jobs finish notification
 */
Std_ReturnType Dcm_CsmAsyncJobFinished(uint32 jobId, Crypto_ResultType result)
{
    if (Dcm_CsmJobId == jobId)
    {
        Dcm_CsmResult = result;
        switch (Dcm_AuthenticateAsync)
        {
        case DCM_CSM_GENERATE_ASYNC_PENDING:
        {
            Dcm_AuthenticateAsync = DCM_CSM_GENERATE_ASYNC_FINISH;
            break;
        }
        case DCM_CSM_SIGNATURE_ASYNC_PENDING:
        {
            Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_ASYNC_FINISH;
            break;
        }
        case DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING:
        {
            Dcm_AuthenticateAsync = DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC;
            break;
        }
        default:
        {
            Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
            break;
        }
        }
    }
    return E_OK;
}

/**
 * set deauthenticationRole for targeted connection
 */
Std_ReturnType Dcm_SetDeauthenticatedRole(uint16 connectionId, const Dcm_AuthenticationRoleType deauthenticatedRole)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_SETDEAUTHENTICATEDROLE_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        if (DCM_DEAUTHENTICATED == Dcm_AuthenticateInfo[connectionId].AuthenticateStatus)
        {
            /*set all role to inactive*/
            Dcm_AuthenticateInfo[connectionId].Certificate = 0u;
            /*set Deauthenticate role*/
            for (uint8 index = 0u; index < DCM_AUTHENTICATION_ROLE_SIZE; index++)
            {
                Dcm_AuthenticateInfo[connectionId].Certificate |=
                    (uint32)((uint32)deauthenticatedRole[index] << (3u - index));
            }
        }
    }
    return E_OK;
}

#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
/**
 * turn on authentication default session timeout
 */
void Dcm_UDS0x29_SetAuthenticationCtrlOn(uint16 ConnectionId)
{
    if ((DCM_DEFAULT_SESSION == Dcm_SessionCtrl.CurrentSession)
        && (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus))
    {
        Dcm_AuthenticationCtrl[ConnectionId].AuthenticationState    = DCM_AUTHENTICATIONTIMER_ON;
        Dcm_AuthenticationCtrl[ConnectionId].AuthenticationCurTimer = DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT;
    }
}
#endif

/**
 * turn off authentication default session timeout
 */
void Dcm_UDS0x29_SetAuthenticationCtrlOff(uint16 ConnectionId)
{
    if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        || (DCM_AUTHENTICATE != Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus))
    {
        Dcm_AuthenticationCtrl[ConnectionId].AuthenticationState = DCM_AUTHENTICATIONTIMER_OFF;
    }
}

#ifdef DCM_AUTHENTICATION_DEFAULT_SESSION_TIMEOUT
/**
 * periodically check authentication timer, set to deauthentication if timeout
 */
void Dcm_UDS0x29_MainFunction(void)
{
    for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
    {
        /* DcmDspAuthenticationDefaultSessionTimeOut check*/
        if (DCM_AUTHENTICATIONTIMER_ON == Dcm_AuthenticationCtrl[index].AuthenticationState)
        {
            if (TRUE == DcmInternal_DecreaseTimer(&Dcm_AuthenticationCtrl[index].AuthenticationCurTimer))
            {
                Dcm_UDS0x29_SetDeAuthenticate(index);
#if ((STD_ON == DCM_UDS_0X2A) && (STD_ON == DCM_DYN_DID) && (STD_ON == DCM_DDDID_CHECK_SOURCE))
                Dcm_UDS0x2A_StatusChangeHandle();
#endif
            }
        }
    }
}
#endif

#if (STD_ON == DCM_UDS_0X31)
/**
 * check authentication status for rid
 */
Std_ReturnType Dcm_UDS0x29_RidAuthenticationCheck(
    uint16                        ConnectionId,
    uint16                        RoutineCfgIdx,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType            result = E_OK;
    uint8                     RoleRefNum;
    const uint8*              RoleRef;
    const Dcm_DspRoutineType* pDcmDspRoutine = &Dcm_DspRoutine[RoutineCfgIdx];
    boolean                   call           = TRUE;
    uint8                     protocolId     = Dcm_DslMainConnection[ConnectionId].ParentProtocolId;
    uint8                     SubSid         = Dcm_MsgContext[protocolId].reqData[0u];

    switch (SubSid)
    {
    case 0x01u:
    {
        RoleRefNum = pDcmDspRoutine->StartRoutine->RoutineRoleRefNum;
        RoleRef    = pDcmDspRoutine->StartRoutine->RoutineRoleRef;
        break;
    }
    case 0x02u:
    {
        RoleRefNum = pDcmDspRoutine->StopRoutine->RoutineRoleRefNum;
        RoleRef    = pDcmDspRoutine->StopRoutine->RoutineRoleRef;
        break;
    }
    case 0x03u:
    {
        RoleRefNum = pDcmDspRoutine->RequestRoutineResults->RoutineRoleRefNum;
        RoleRef    = pDcmDspRoutine->RequestRoutineResults->RoutineRoleRef;
        break;
    }
    default:
    {
        call = FALSE;
        break;
    }
    }
    if (TRUE == call)
    {
        result = Dcm_UDS0x29_RoleCheck(ConnectionId, RoleRef, RoleRefNum);
    }
    if (E_NOT_OK == result)
    {
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
        result = Dcm_UDS0x29_RidWhiteListCheck(ConnectionId);
#endif
    }
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return result;
}
#endif

#if (                                                                                                            \
    (STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X24) || (STD_ON == DCM_UDS_0X2A) \
    || (STD_ON == DCM_UDS_0X2C) || (STD_ON == DCM_UDS_0X2F))
/**
 * check authentication status for did
 */
Std_ReturnType Dcm_UDS0x29_DidAuthenticationCheck(
    uint16                         ConnectionId,
    uint16                         Did,
    const Dcm_DspDidReadWriteType* didReadWrite,
    const Dcm_DspDidControlType*   didControl,
    Dcm_NegativeResponseCodeType*  ErrorCode)
{
    uint8        RoleRefNum;
    const uint8* RoleRef;

    if (NULL_PTR != didReadWrite)
    {
        RoleRefNum = didReadWrite->DidReadWriteRoleRefNum;
        RoleRef    = didReadWrite->DidReadWriteRoleRef;
    }
    else
    {
        RoleRefNum = didControl->DidControlRoleRefNum;
        RoleRef    = didControl->DidControlRoleRef;
    }

    Std_ReturnType result =
        Dcm_UDS0x29_RoleCheck(Dcm_DslMainConnection[ConnectionId].ParentProtocolId, RoleRef, RoleRefNum);
    if (E_NOT_OK == result)
    {
#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
        result = Dcm_UDS0x29_DidWhiteListCheck(ConnectionId, Did);
#else
        result = E_NOT_OK;
#endif
    }
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return result;
}
#endif

/**
 * check authentication status for configured RoleRef
 */
Std_ReturnType Dcm_UDS0x29_AuthenticationCheck(
    uint16                        ConnectionId,
    const uint8*                  subRoleRef,
    uint8                         subRoleRefNum,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result =
        Dcm_UDS0x29_RoleCheck(Dcm_DslMainConnection[ConnectionId].ParentProtocolId, subRoleRef, subRoleRefNum);

#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
    if (E_NOT_OK == result)
    {
        result = Dcm_UDS0x29_WhiteListCheck(
            ConnectionId,
            Dcm_MsgContext[Dcm_DslMainConnection[ConnectionId].ParentProtocolId].reqDataLen + 1u);
    }
#endif
    if ((E_NOT_OK == result) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#ifdef DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID
/**
 * store the current authentication status
 */
DCM_LOCAL void Dcm_UDS0x29_AuthenticateStore(void)
{
    NvM_RequestResultType NvmRet;
    uint16                blockId = DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID;

    (void)NvM_GetErrorStatus(blockId, &NvmRet);
    if (NVM_REQ_PENDING != NvmRet)
    {
        (void)NvM_WritePRAMBlock(blockId);
    }
}
#endif

#if ((STD_ON == DCM_UDS_0X29_0X3) || (STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
/**
 * set to DcmDspAuthenticationGeneralNRC if configured modeRule passes
 */
DCM_LOCAL void Dcm_UDS0x29_SetNrc(Dcm_NegativeResponseCodeType* ErrorCode)
{
    /*the Dcm shall send the NRC code given in DcmDspAuthenticationGeneralNRC
    instead of the specific NRC codes in all situation where a Certificate verification failed
    - NRC is returned.*/
    if ((NULL_PTR != Dcm_DspAuthentication.GeneralNRCModeRuleRef)
        && (E_OK == Dcm_DspAuthentication.GeneralNRCModeRuleRef(ErrorCode)))
    {
        *ErrorCode = DCM_AUTHENTICATION_GENERAL_NRC;
    }
}
#endif
#endif

/**
 * called to set authentication role
 */
DCM_LOCAL void Dcm_UDS0x29_SetDeAuthenticateRole(uint16 connectionId)
{
    /*set all role to inactive*/
    Dcm_AuthenticateInfo[connectionId].Certificate = 0u;
    /*set deAuthenticate role*/
    for (uint8 index = 0u; index < Dcm_DspAuthentication.DeauthenticatedRoleRefNum; index++)
    {
        uint8 RoleRef = Dcm_DspAuthentication.DeauthenticatedRoleRef[index];
        uint8 pos     = Dcm_DspAuthenticationRow[RoleRef].AuthenticationRoleBitPosition;
        Dcm_AuthenticateInfo[connectionId].Certificate |= (uint32)((uint32)1u << pos);
    }
}

#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
/**
 * clear the whilelist
 */
DCM_LOCAL void Dcm_UDS0x29_ClearWhiteList(uint16 connectionId)
{
    Dcm_AuthenticateInfoType* authenticateInfo = &Dcm_AuthenticateInfo[connectionId];
#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
    /*Clear Did White List*/
    authenticateInfo->WhiteList.WhiteListDidLength = DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE;
    for (uint8 index = 0u; index < DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE; index++)
    {
        authenticateInfo->WhiteList.WhiteListDid[index] = 0u;
    }
#endif
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
    /*Clear Rid White List*/
    authenticateInfo->WhiteList.WhiteListRidLength = DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE;
    for (uint8 index = 0u; index < DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE; index++)
    {
        authenticateInfo->WhiteList.WhiteListRid[index] = 0u;
    }
#endif
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
    /*Clear Service White List*/
    authenticateInfo->WhiteList.WhiteListServicesNum = DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE;
    for (uint8 index = 0u; index < DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE; index++)
    {
        authenticateInfo->WhiteList.WhiteListServices[index]       = 0u;
        authenticateInfo->WhiteList.WhiteListServiceslength[index] = 0u;
    }
#endif
}
#endif

/**
 * The service interpreter for UDS 0x19, subfunctiun 0x00
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x29_0x00(uint16 ConnectionId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    /*check the massage length*/
    if (DCM_UDS0X29_MINREQLEN != pMsgContext->reqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        Dcm_UDS0x29_SetDeAuthenticate(ConnectionId);
        /*On reception of an Authentication (0x29) service with subfunction equal to
          de-Authenticate, the Dcm shall reply with a positive response having
          the authenticationReturnParameter set to DeAuthentication successful.*/
        pMsgContext->resData[1u] = DCM_DEAUTHENTICATION_SUCCESSFUL;
        pMsgContext->resDataLen  = 1u;
    }
    return result;
}

#if ((STD_ON == DCM_UDS_0X29_0X1) || (STD_ON == DCM_UDS_0X29_0X2))
/**
 * deal with KeyM Asynchronous result
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_KeyMAsyncCheck(Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;
    switch (Dcm_KeyMResult)
    {
    case KEYM_CERTIFICATE_VALID:
    {
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
        result                = E_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_SIGNATURE_FAIL:
    {
        *ErrorCode = DCM_E_INVALIDSIGNATURE;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST:
    {
        *ErrorCode = DCM_E_INVALIDCHAINOFTRUST;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_INVALID_TYPE:
    {
        *ErrorCode = DCM_E_INVALIDTYPE;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_INVALID_FORMAT:
    {
        *ErrorCode = DCM_E_INVALIDFORMAT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_INVALID_CONTENT:
    {
        *ErrorCode = DCM_E_INVALIDSCOPE;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_REVOKED:
    {
        *ErrorCode = DCM_E_INVALIDCERTIFICATE;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    case KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL:
    {
        *ErrorCode = DCM_E_INVALIDTIMEPERIOD;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    default:
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
        break;
    }
    }
#ifdef SEV_UDS_CERTIFICATE_FAILURE
    if (E_NOT_OK == result)
    {
        IdsM_SetSecurityEvent(SEV_UDS_CERTIFICATE_FAILURE);
    }
#endif
    return result;
}

/**
 * deals with challenge and output accordingly
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x29_Challenge(uint16 ConnectionId, Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    const Dcm_DspAuthenticationConnectionType* pAuthenticationConnection =
        &Dcm_DspAuthenticationConnection[ConnectionId];
    Std_ReturnType result = E_OK;

    if (DCM_ASYNC_IDLE == Dcm_AuthenticateAsync)
    {
        Dcm_ChallengeLength = (uint32)pAuthenticationConnection->EcuChallengeLength;
        result              = Csm_RandomGenerate(
            pAuthenticationConnection->RandomJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            Dcm_challengeData,
            &Dcm_ChallengeLength);
        switch (result)
        {
        case E_OK:
        {
            result                = DCM_E_PENDING;
            Dcm_AuthenticateAsync = DCM_CSM_GENERATE_ASYNC_PENDING;
            Dcm_CsmJobId          = pAuthenticationConnection->RandomJobRef;
            break;
        }
        case CRYPTO_E_BUSY:
        {
            *ErrorCode = DCM_E_BUSYREPEATREQUEST;
            result     = E_NOT_OK;
            break;
        }
        case CRYPTO_E_ENTROPY_EXHAUSTED:
        {
            *ErrorCode = DCM_E_CHALLENGECALCULATIONFAILED;
            result     = E_NOT_OK;
            break;
        }
        default:
        {
            *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
            break;
        }
        }
    }
    if (DCM_CSM_GENERATE_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        result = Dcm_UDS0x29_HandleCsmGenerateResult(ConnectionId, pMsgContext, ErrorCode);
    }
    if (DCM_CSM_SIGNATURE_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        result = Dcm_UDS0x29_HandleCsmSignatureResult(ConnectionId, pMsgContext, ErrorCode);
    }
    return result;
}

/* This function deals with Csm Generate asynchronous result */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_HandleCsmGenerateResult(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result;
    Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
    uint8* resData        = pMsgContext->resData;

    if (E_OK == Dcm_CsmResult)
    {
        /*get the sub-function */
        resData[1u] = DCM_AUTHENTICATION_CVOVN;
        resData[2u] = (uint8)(Dcm_ChallengeLength >> 8u);
        resData[3u] = (uint8)Dcm_ChallengeLength;
        DcmInternal_Memcpy(&resData[4u], Dcm_challengeData, Dcm_ChallengeLength);
        pMsgContext->resDataLen                                     = 3u + Dcm_ChallengeLength;
        const uint8*                               reqData          = pMsgContext->reqData;
        Dcm_AuthenticateInfoType*                  authenticateInfo = &Dcm_AuthenticateInfo[ConnectionId];
        const Dcm_DspAuthenticationConnectionType* pAuthenticationConnection =
            &Dcm_DspAuthenticationConnection[ConnectionId];

        if (DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL == pMsgContext->reqData[0u])
        {
            resData[4u + Dcm_ChallengeLength] = 0u;
            resData[5u + Dcm_ChallengeLength] = 0u;
            pMsgContext->resDataLen += 2u;
            authenticateInfo->AuthenticateStatus = DCM_PRE_AUTHENTICATE;
            result                               = E_OK;
        }
        else
        {
            KeyM_CertDataType CertData;
            CertData.certData       = &resData[6u + Dcm_ChallengeLength];
            CertData.certDataLength = pMsgContext->resMaxDataLen - pMsgContext->resDataLen - 2u;
            result                  = KeyM_GetCertificate(pAuthenticationConnection->ECUCertificateRef, &CertData);
            if (E_OK == result)
            {
                uint16 lengthOfCertificateClient = DCM_U8N_TO_U16(&reqData[2u]);
                uint16 lengthOfChallengeClient   = DCM_U8N_TO_U16(&reqData[4u + lengthOfCertificateClient]);
                authenticateInfo->CertDataLength = CertData.certDataLength;
                pMsgContext->resDataLen += (2u + CertData.certDataLength);
                Dcm_CsmJobId                      = pAuthenticationConnection->ClientChallengeSignJobRef;
                resData[4u + Dcm_ChallengeLength] = (uint8)(CertData.certDataLength >> 8u);
                resData[5u + Dcm_ChallengeLength] = (uint8)CertData.certDataLength;
                authenticateInfo->ProofOfOwnershipServerLength =
                    pMsgContext->resMaxDataLen - pMsgContext->resDataLen - 2u;
                result = Csm_SignatureGenerate(
                    Dcm_CsmJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &reqData[6u + lengthOfCertificateClient],
                    lengthOfChallengeClient,
                    &resData[8u + Dcm_ChallengeLength + CertData.certDataLength],
                    &(authenticateInfo->ProofOfOwnershipServerLength));
                switch (result)
                {
                case E_OK:
                {
                    result                = DCM_E_PENDING;
                    Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_ASYNC_PENDING;
                    Dcm_CsmJobId          = pAuthenticationConnection->ClientChallengeSignJobRef;
                    break;
                }
                case CRYPTO_E_BUSY:
                {
                    *ErrorCode = DCM_E_BUSYREPEATREQUEST;
                    result     = E_NOT_OK;
                    break;
                }
                default:
                {
                    *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
                    Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
                    result = E_NOT_OK;
                    break;
                }
                }
            }
            else if (KEYM_E_KEY_CERT_SIZE_MISMATCH == result)
            {
                *ErrorCode = DCM_E_GENERALREJECT;
                result     = E_NOT_OK;
            }
            else
            {
                *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
                Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
                result = E_NOT_OK;
            }
        }
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
    }

    return result;
}

/* This function deals with Csm Signature asynchronous result */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_HandleCsmSignatureResult(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result                            = E_OK;
    Dcm_AuthenticateAsync                            = DCM_ASYNC_IDLE;
    uint8*                          resData          = pMsgContext->resData;
    const Dcm_AuthenticateInfoType* authenticateInfo = &Dcm_AuthenticateInfo[ConnectionId];

    if (E_OK == Dcm_CsmResult)
    {
        resData[6u + Dcm_ChallengeLength + authenticateInfo->CertDataLength] =
            (uint8)(authenticateInfo->ProofOfOwnershipServerLength >> 8u);
        resData[7u + Dcm_ChallengeLength + authenticateInfo->CertDataLength] =
            (uint8)authenticateInfo->ProofOfOwnershipServerLength;
        resData
            [8u + Dcm_ChallengeLength + authenticateInfo->CertDataLength
             + authenticateInfo->ProofOfOwnershipServerLength] = 0u;
        resData
            [9u + Dcm_ChallengeLength + authenticateInfo->CertDataLength
             + authenticateInfo->ProofOfOwnershipServerLength] = 0u;
        pMsgContext->resDataLen += (4u + authenticateInfo->ProofOfOwnershipServerLength);
        Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus = DCM_PRE_AUTHENTICATE;
        result                                                = E_OK;
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
    }

    return result;
}

/**
 * The service interpreter for UDS 0x19, subfunctiun 0x01/0x02
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_01_02(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (DCM_INITIAL == OpStatus)
    {
        result = Dcm_UDS0x29_01_02_InitialHandle(ConnectionId, pMsgContext, ErrorCode);
    }

    if (DCM_KEYM_VERIFY_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        result = Dcm_UDS0x29_KeyMAsyncCheck(ErrorCode);
    }
    if ((E_OK == result) && (DCM_KEYM_VERIFY_ASYNC_PENDING != Dcm_AuthenticateAsync))
    {
        result = Dcm_UDS0x29_Challenge(ConnectionId, pMsgContext, ErrorCode);
    }
    if ((E_OK == result) && (DCM_ASYNC_IDLE != Dcm_AuthenticateAsync))
    {
        result = DCM_E_PENDING;
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x29_01_02_InitialHandle(
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8*         reqData = pMsgContext->reqData;
    Std_ReturnType result  = E_OK;

    /*get all request info*/
    uint16 lengthOfCertificateClient = DCM_U8N_TO_U16(&reqData[2u]);
    uint16 lengthOfChallengeClient   = DCM_U8N_TO_U16(&reqData[4u + lengthOfCertificateClient]);

    /* check the massage length */
    if (((6u + (Dcm_MsgLenType)lengthOfCertificateClient + (Dcm_MsgLenType)lengthOfChallengeClient)
         != pMsgContext->reqDataLen)
        || (0x00u == lengthOfCertificateClient))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else if (0x0u != reqData[1u])
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }
    else
    {
        uint16            CertificateId = Dcm_DspAuthenticationConnection[ConnectionId].ConnectionCertificateRef;
        KeyM_CertDataType CertificateData;
        CertificateData.certData       = &reqData[4];
        CertificateData.certDataLength = lengthOfCertificateClient;
        Dcm_AuthenticateAsync          = DCM_ASYNC_IDLE;
        Std_ReturnType KeyMret         = KeyM_SetCertificate(CertificateId, &CertificateData);
        switch (KeyMret)
        {
        case E_OK:
        {
            KeyMret = KeyM_VerifyCertificate(CertificateId);
            switch (KeyMret)
            {
            case E_OK:
            {
                result                = DCM_E_PENDING;
                Dcm_AuthenticateAsync = DCM_KEYM_VERIFY_ASYNC_PENDING;
                Dcm_KeyMCertID        = CertificateId;
                break;
            }
            case KEYM_E_BUSY:
            {
                *ErrorCode = DCM_E_BUSYREPEATREQUEST;
                result     = E_NOT_OK;
                break;
            }
            default:
            {
                *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
                Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
                result = E_NOT_OK;
                break;
            }
            }
            break;
        }
        case KEYM_E_KEY_CERT_SIZE_MISMATCH:
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
            break;
        }
        default:
        {
            *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
            break;
        }
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X29_0X3
#if (                                                                                                                \
    (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE))
/**
 * deal with subfunction 0x03, gets the specific lists
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_getlist(
    Dcm_KeyMCertInfoType          Dcm_KeyMCertInfo,
    uint8*                        DataPtr,
    uint8*                        DataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType               result;
    KeyM_CertElementIteratorType CertElementIterator;
    uint32                       getDataLength = *DataLength;
    uint32                       TotalLength   = *DataLength;
    *DataLength                                = 0u;

    /*get white list*/
    result = KeyM_CertElementGetFirst(
        Dcm_KeyMCertInfo.CertId,
        Dcm_KeyMCertInfo.CertElementId,
        &CertElementIterator,
        DataPtr,
        &getDataLength);
    if (E_OK == result)
    {
        (*Dcm_KeyMCertInfo.WhiteListNum) += 1u;
        (*DataLength) = (uint8)getDataLength;
        if (getDataLength > Dcm_KeyMCertInfo.WhiteListMaxSize)
        {
            *ErrorCode = DCM_E_INVALIDCONTENT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
        }
        while (E_OK == result)
        {
            getDataLength = TotalLength - (*DataLength);
            result        = KeyM_CertElementGetNext(&CertElementIterator, &DataPtr[*DataLength], &getDataLength);
            if (E_OK == result)
            {
                (*Dcm_KeyMCertInfo.WhiteListNum) += 1u;
                (*DataLength) += (uint8)getDataLength;
            }
        }
    }
    if ((*DataLength) > Dcm_KeyMCertInfo.WhiteListMaxSize)
    {
        *ErrorCode = DCM_E_INVALIDCONTENT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
    }
    else if (E_NOT_OK == result)
    {
        result = E_OK;
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
    }
    return result;
}
#endif

#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
/**
 * deal with subfunction 0x03, gets the specific whitelist
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_whitelist(uint16 ConnectionId, Dcm_NegativeResponseCodeType* ErrorCode)
{
    const Dcm_DspAuthenticationConnectionType* pAuthenticationConnection =
        &Dcm_DspAuthenticationConnection[ConnectionId];
    Dcm_KeyMCertInfoType Dcm_KeyMCertInfo;
    uint8                listNum = 0u;
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
    uint8 Data[DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE];
    DcmInternal_Memset(Data, 0u, (uint32)DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE);
#endif
    Dcm_UDS0x29_ClearWhiteList(ConnectionId);
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
    /*get services white list*/
    Dcm_KeyMCertInfo.WhiteListMaxSize = DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE;
    Dcm_KeyMCertInfo.CertElementId    = pAuthenticationConnection->WhiteListServicesElementRef;
    Dcm_KeyMCertInfo.CertId           = pAuthenticationConnection->ConnectionCertificateRef;
    Dcm_KeyMCertInfo.WhiteListNum     = &listNum;
    Std_ReturnType result             = Dcm_UDS0x29_0x03_getlist(
        Dcm_KeyMCertInfo,
        Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListServices,
        &Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListServicesNum,
        ErrorCode);
    Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListServicesNum = listNum;
    DcmInternal_Memcpy(Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListServiceslength, Data, listNum);
#endif

#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
    if ((E_OK == result) && (DCM_INVALID_UINT16 != pAuthenticationConnection->WhiteListDIDElementRef))
    {
        /*get Did white list*/
        listNum                           = 0u;
        Dcm_KeyMCertInfo.WhiteListMaxSize = DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE;
        Dcm_KeyMCertInfo.CertElementId    = pAuthenticationConnection->WhiteListDIDElementRef;
        Dcm_KeyMCertInfo.CertId           = pAuthenticationConnection->ConnectionCertificateRef;
        result                            = Dcm_UDS0x29_0x03_getlist(
            Dcm_KeyMCertInfo,
            Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListDid,
            &Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListDidLength,
            ErrorCode);
    }
#endif

#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
    if ((E_OK == result) && (DCM_INVALID_UINT16 != pAuthenticationConnection->WhiteListRIDElementRef))
    {
        /*get Rid white list*/
        listNum                           = 0u;
        Dcm_KeyMCertInfo.WhiteListMaxSize = DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE;
        Dcm_KeyMCertInfo.CertElementId    = pAuthenticationConnection->WhiteListRIDElementRef;
        Dcm_KeyMCertInfo.CertId           = pAuthenticationConnection->ConnectionCertificateRef;
        result                            = Dcm_UDS0x29_0x03_getlist(
            Dcm_KeyMCertInfo,
            Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListRid,
            &Dcm_AuthenticateInfo[ConnectionId].WhiteList.WhiteListRidLength,
            ErrorCode);
    }
#endif
    return result;
}
#endif

/**
 * deal with subfunction 0x03, updates the authentication State
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03_Update(uint16 ConnectionId, Dcm_NegativeResponseCodeType* ErrorCode)
{
    const Dcm_DspAuthenticationConnectionType* pAuthenticationConnection =
        &Dcm_DspAuthenticationConnection[ConnectionId];
    uint8  CertElementData[4];
    uint32 CertElementDataLength = 4u;

    /*get role*/
    Std_ReturnType result = KeyM_CertElementGet(
        pAuthenticationConnection->ConnectionCertificateRef,
        pAuthenticationConnection->RoleElementRef,
        CertElementData,
        &CertElementDataLength);
    if (E_OK == result)
    {
        if (CertElementDataLength != DCM_AUTHENTICATION_ROLE_SIZE)
        {
            *ErrorCode = DCM_E_INVALIDCONTENT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
        }
        else
        {
            /*set all role to inactive*/
            Dcm_AuthenticateInfo[ConnectionId].Certificate = 0u;
            /*set Authenticate role*/
            for (uint8 i = 0u; i < CertElementDataLength; i++)
            {
                Dcm_AuthenticateInfo[ConnectionId].Certificate |=
                    (uint32)((uint32)CertElementData[i] << ((CertElementDataLength - i - 1u) * 8u));
            }
#if (                                                                                                            \
    (defined DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE) || (defined DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE) \
    || (defined DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE))
            result = Dcm_UDS0x29_0x03_whitelist(ConnectionId, ErrorCode);
#endif
            /*set Authenticate status*/
            if (E_OK == result)
            {
                Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus = DCM_AUTHENTICATE;
#ifdef SEV_UDS_AUTHENTICATION_SUCCESSFUL
                IdsM_SetSecurityEvent(SEV_UDS_AUTHENTICATION_SUCCESSFUL);
#endif
#ifdef DCM_AUTHENTICATION_PERSIST_STATE_BLOCK_ID
                Dcm_UDS0x29_AuthenticateStore();
#endif
            }
        }
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
        result = E_NOT_OK;
    }
    return result;
}

/**
 * The service interpreter for UDS 0x19, subfunctiun 0x03
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x03(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint16                        ConnectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = (DCM_INITIAL == OpStatus) ? E_OK : DCM_E_PENDING;
    const uint8*   reqData = pMsgContext->reqData;

    /*get all request info*/
    uint16 lengthOfProofOfOwnershipClient   = DCM_U8N_TO_U16(&reqData[1u]);
    uint16 lengthOfEphemeralPublicKeyClient = DCM_U8N_TO_U16(&reqData[3u + lengthOfProofOfOwnershipClient]);

    /*check the massage length*/
    if ((DCM_INITIAL == OpStatus)
        && (((5u + (Dcm_MsgLenType)lengthOfProofOfOwnershipClient + (Dcm_MsgLenType)lengthOfEphemeralPublicKeyClient))
            != pMsgContext->reqDataLen))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    /*check request Sequence*/
    if ((DCM_INITIAL == OpStatus) && (E_OK == result)
        && (DCM_DEAUTHENTICATED == Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus))
    {
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        result     = E_NOT_OK;
    }

    if ((DCM_INITIAL == OpStatus) && (E_OK == result))
    {
        const Dcm_DspAuthenticationConnectionType* pAuthenticationConnection =
            &Dcm_DspAuthenticationConnection[ConnectionId];
        result = Csm_SignatureVerify(
            pAuthenticationConnection->VerifyProofOfOwnerShipClientJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            Dcm_challengeData,
            Dcm_ChallengeLength,
            &reqData[3u],
            lengthOfProofOfOwnershipClient,
            &Dcm_CsmVerifyResult);
        if (E_OK == result)
        {
            result                = DCM_E_PENDING;
            Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING;
            Dcm_CsmJobId          = pAuthenticationConnection->VerifyProofOfOwnerShipClientJobRef;
        }
        else if (CRYPTO_E_BUSY == result)
        {
            /* SWS_Dcm_01465 Behavior on busy crypto operation
            If any of the Csm or KeyM APIs called by the Dcm is returning
            CRYPTO_E_BUSY or KEYM_E_BUSY, the Dcm shall return the NRC 0x21 (busyRepeat).*/
            *ErrorCode = DCM_E_BUSYREPEATREQUEST;
            result     = E_NOT_OK;
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
        }
    }

    if (DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC == Dcm_AuthenticateAsync)
    {
        if ((E_OK == Dcm_CsmResult) && (CRYPTO_E_VER_OK == Dcm_CsmVerifyResult)
            && ((NULL_PTR == Dcm_DspAuthentication.PersistStateModeRuleRef)
                || (E_OK == Dcm_DspAuthentication.PersistStateModeRuleRef(ErrorCode))))
        {
            uint8* resData = pMsgContext->resData;
            resData[1u]    = DCM_AUTHENTICATION_CVAC;
            resData[2u]    = 0u;
            resData[3u]    = 0u;
            pMsgContext->resDataLen += 3u;
            result = Dcm_UDS0x29_0x03_Update(ConnectionId, ErrorCode);
        }
        else
        {
            if (CRYPTO_E_VER_OK != Dcm_CsmVerifyResult)
            {
                *ErrorCode = DCM_E_OWNERSHIPVERIFICATIONFAILED;
                result     = E_NOT_OK;
            }
            else
            {
                *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
                Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
                result = E_NOT_OK;
            }
        }
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
    }
    return result;
}
#endif

#ifdef DCM_UDS_0X29_0X4
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x04
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x04(
    Dcm_OpStatusType              OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (DCM_INITIAL == OpStatus)
    {
        uint16 TransmitCertificateRef;
        result = Dcm_UDS0x29_0x04_ConditionCheck(pMsgContext, ErrorCode, &TransmitCertificateRef);

        if (E_OK == result)
        {
            KeyM_CertDataType certData;
            uint16            certDataLength = DCM_U8N_TO_U16(&pMsgContext->reqData[3]);
            certData.certDataLength          = (uint32)certDataLength;
            certData.certData                = &pMsgContext->reqData[5];
            result = Dcm_UDS0x29_0x04_KeyMProcess(&certData, TransmitCertificateRef, ErrorCode);
        }
    }
    else if (DCM_PENDING == OpStatus)
    {
        if (DCM_KEYM_VERIFY_ASYNC_FINISH == Dcm_AuthenticateAsync)
        {
            result = Dcm_UDS0x29_KeyMAsyncCheck(ErrorCode);
            if (E_OK == result)
            {
                Dcm_TransmitCertificate = TRUE;
            }
        }
        if ((E_OK == result) && (DCM_KEYM_VERIFY_ASYNC_PENDING == Dcm_AuthenticateAsync))
        {
            result = DCM_E_PENDING;
        }
    }
    else
    {
        /* idle */
    }
    if (E_OK != result)
    {
        Dcm_TransmitCertificate = FALSE;
    }
    else
    {
        pMsgContext->resData[1u] = DCM_AUTHENTICATION_CV;
        pMsgContext->resDataLen += 1u;
    }
    return result;
}

DCM_LOCAL Std_ReturnType
    Dcm_UDS0x29_0x04_KeyMProcess(KeyM_CertDataType* certData, uint16 CertId, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = KeyM_SetCertificate(CertId, certData);
    if ((KEYM_E_PARAMETER_MISMATCH == result) || (KEYM_E_KEY_CERT_SIZE_MISMATCH == result))
    {
        result = E_NOT_OK;
    }
    if (E_NOT_OK == result)
    {
        *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
        Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
    }

    if (E_OK == result)
    {
        result = KeyM_VerifyCertificate(CertId);
        if (E_OK == result)
        {
            result                = DCM_E_PENDING;
            Dcm_AuthenticateAsync = DCM_KEYM_VERIFY_ASYNC_PENDING;
            Dcm_KeyMCertID        = CertId;
        }
        else if (KEYM_E_BUSY == result)
        {
            *ErrorCode = DCM_E_BUSYREPEATREQUEST;
            result     = E_NOT_OK;
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
#ifdef DCM_AUTHENTICATION_GENERAL_NRC
            Dcm_UDS0x29_SetNrc(ErrorCode);
#endif
            result = E_NOT_OK;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x04_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       TransmitCertificateRef)
{
    Std_ReturnType result = E_OK;

    if (pMsgContext->reqDataLen < 6u)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        uint16 certDataLength = DCM_U8N_TO_U16(&pMsgContext->reqData[3]);
        if (pMsgContext->reqDataLen != (5u + (Dcm_MsgLenType)certDataLength))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }

    uint16 certificateEvaluationId;
    if (E_OK == result)
    {
        result                  = E_NOT_OK;
        certificateEvaluationId = DCM_U8N_TO_U16(&pMsgContext->reqData[1]);
        for (uint16 index = 0u; index < DCM_AUTHENTICATION_TRANSMIT_CERTIFICATE_NUM; index++)
        {
            if (Dcm_DspAuthenticationTransmitCertificate[index].TransmitCertificateEvaluationId
                == certificateEvaluationId)
            {
                result                  = E_OK;
                *TransmitCertificateRef = Dcm_DspAuthenticationTransmitCertificate[index].TransmitCertificateRef;
                break;
            }
        }
        if (E_NOT_OK == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }

    return result;
}
#endif

#ifdef DCM_UDS_0X29_0X8
/**
 * The service interpreter for UDS 0x19, subfunctiun 0x08
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_0x08(Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    /*check the massage length*/
    if (DCM_UDS0X29_MINREQLEN != pMsgContext->reqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        pMsgContext->resData[1u] = DCM_AUTHENTICATION_APCE;
        pMsgContext->resDataLen  = 1u;
    }
    return result;
}
#endif

/**
 * check whether current authentication status satisfies roleRef
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_RoleCheck(uint16 ConnectionId, const uint8* RoleRef, uint8 RoleNum)
{
    Std_ReturnType result = E_NOT_OK;

    if (0u == RoleNum)
    {
        result = E_OK;
    }
    for (uint8 i = 0u; (i < RoleNum) && (E_NOT_OK == result); i++)
    {
        if ((RoleRef != NULL_PTR)
            && ((Dcm_AuthenticateInfo[ConnectionId].Certificate
                 & ((uint32)1u << Dcm_DspAuthenticationRow[RoleRef[i]].AuthenticationRoleBitPosition))
                != 0u))
        {
            result = E_OK;
        }
    }
    return result;
}

#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
/**
 * check for whitelist(did, rid, and service)
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_WhiteListCheck(uint16 ConnectionId, Dcm_MsgLenType CheckLength)
{
    Std_ReturnType                  result           = E_NOT_OK;
    const Dcm_AuthenticateInfoType* authenticateInfo = &Dcm_AuthenticateInfo[ConnectionId];

    if (DCM_AUTHENTICATE == authenticateInfo->AuthenticateStatus)
    {
        uint8 protocolId = Dcm_DslMainConnection[ConnectionId].ParentProtocolId;
        /* PRQA S 2983 ++ */ /* VL_Dcm_2983 */
        const uint8* reqData           = Dcm_MsgContext[protocolId].reqData;
        uint8        sid               = Dcm_ProtocolCtrl[protocolId].Sid;
        uint8        ServicesNum       = authenticateInfo->WhiteList.WhiteListServicesNum;
        const uint8* WhiteListServices = authenticateInfo->WhiteList.WhiteListServices;
        const uint8* length            = authenticateInfo->WhiteList.WhiteListServiceslength;
        /* PRQA S 2983 -- */ /* VL_Dcm_2983 */

        uint8 offset = 0u;
        /* check service white list*/
        /* PRQA S 2994, 2996, 2880 ++ */ /* VL_Dcm_2994, VL_Dcm_2996, VL_Dcm_2880 */
        for (uint8 index = 0u;
             (index < ServicesNum) && ((offset + 3u) < DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE);
             index++)
        {
            /* 1. Checks on service ID level */
            if (((sid == WhiteListServices[offset]) && (1u == length[index]))
                /* 2. Checks on service ID and sub-function level */
                || ((CheckLength >= 2u) && (2u == length[index]) && (sid == WhiteListServices[offset])
                    && (reqData[0u] == WhiteListServices[offset + 1u]))
                /* 3. Checks for services with one or multiple DIDs */
                /* 4. Check on dynamically defined DIDs */
                || ((CheckLength >= 3u) && (3u == length[index]) && (sid == WhiteListServices[offset])
                    && (reqData[0u] == WhiteListServices[offset + 1u])
                    && (reqData[1u] == WhiteListServices[offset + 2u]))
                /* 5. Checks on service 0x31 per sub-function */
                || ((CheckLength >= 4u) && (4u == length[index]) && (sid == WhiteListServices[offset])
                    && (reqData[0u] == WhiteListServices[offset + 1u])
                    && (reqData[1u] == WhiteListServices[offset + 2u])
                    && (reqData[2u] == WhiteListServices[offset + 3u])))
            {
                result = E_OK;
                break;
            }
            offset += length[index];
        }
        /* PRQA S 2994, 2996, 2880 -- */ /* VL_Dcm_2994, VL_Dcm_2996, VL_Dcm_2880 */
    }
    return result;
}
#endif

#if ((STD_ON == DCM_UDS_0X22) || (STD_ON == DCM_UDS_0X2A) || (STD_ON == DCM_UDS_0X2E) || (STD_ON == DCM_UDS_0X2F))
#ifdef DCM_AUTHENTICATION_WHITE_LIST_DID_MAX_SIZE
/**
 * check whitelist fo did
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_DidWhiteListCheck(uint16 ConnectionId, uint16 RecDid)
{
    Std_ReturnType result = E_NOT_OK;

    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus)
    {
        uint8                    protocolId  = Dcm_DslMainConnection[ConnectionId].ParentProtocolId;
        uint8                    sid         = Dcm_ProtocolCtrl[protocolId].Sid;
        const Dcm_WhiteListType* whiteList   = &Dcm_AuthenticateInfo[ConnectionId].WhiteList;
        uint8                    ServicesNum = whiteList->WhiteListServicesNum;
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
        const uint8* WhiteListServices = whiteList->WhiteListServices;
        const uint8* length            = whiteList->WhiteListServiceslength;
#endif
        uint8 offset = 0u;
        /* check service white list*/
        for (uint8 index = 0u; (index < ServicesNum) && (E_NOT_OK == result); index++)
        {
            /* 1. Checks on service ID level */
            if (((sid == WhiteListServices[offset]) && (1u == length[index]))
                /* 3. Checks for services with one or multiple DIDs */
                /* 4. Check on dynamically defined DIDs */
                || ((3u == length[index]) && (sid == WhiteListServices[offset])
                    && ((uint8)(RecDid >> 8u) == WhiteListServices[offset + 1u])
                    && ((uint8)RecDid == WhiteListServices[offset + 2u])))
            {
                result = E_OK;
            }
            offset += length[index];
        }
        if (E_NOT_OK == result)
        {
            /* check did white list*/
            ServicesNum       = whiteList->WhiteListDidLength;
            WhiteListServices = whiteList->WhiteListDid;

            for (uint8 index = 0u; (index < ServicesNum) && (E_NOT_OK == result); index += 3u)
            {
                /*chehck did is same*/
                if ((((uint8)(RecDid >> 8u) == WhiteListServices[index])
                     && ((uint8)RecDid == WhiteListServices[index + 1u]))
                    /*check read*/
                    && ((((sid == 0x22u) || (sid == 0x2Au) || (sid == 0x2Cu))
                         && ((DCM_DID_READ_ACCESS & WhiteListServices[index + 2u]) != 0u))
                        /*check write*/
                        || ((sid == 0x2Eu) && ((DCM_DID_WRITE_ACCESS & WhiteListServices[index + 2u]) != 0u))
                        /*check control*/
                        || ((sid == 0x2Fu) && ((DCM_DID_CONTROL_ACCESS & WhiteListServices[index + 2u]) != 0u))))
                {
                    result = E_OK;
                }
            }
        }
    }
    return result;
}
#endif
#endif

#if (STD_ON == DCM_UDS_0X31)
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
/**
 * check whitelist fo rid
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x29_RidWhiteListCheck(uint16 ConnectionId)
{
    Std_ReturnType result = E_NOT_OK;

    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[ConnectionId].AuthenticateStatus)
    {
        uint8        protocolId = Dcm_DslMainConnection[ConnectionId].ParentProtocolId;
        const uint8* reqData    = Dcm_MsgContext[protocolId].reqData;
        /* PRQA S 2983 ++ */ /* VL_Dcm_2983 */
        uint8                    sid         = Dcm_ProtocolCtrl[protocolId].Sid;
        uint8                    subFunction = reqData[0u];
        const Dcm_WhiteListType* whiteList   = &Dcm_AuthenticateInfo[ConnectionId].WhiteList;
        uint8                    ServicesNum = whiteList->WhiteListServicesNum;
#ifdef DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE
        const uint8* WhiteListServices = whiteList->WhiteListServices;
        const uint8* length            = whiteList->WhiteListServiceslength;
#endif
        /* PRQA S 2983 -- */ /* VL_Dcm_2983 */
        uint8 offset = 0u;
        /* check service white list*/
        /* PRQA S 2994, 2996, 2880 ++ */ /* VL_Dcm_2994, VL_Dcm_2996, VL_Dcm_2880 */
        for (uint8 i = 0u; (i < ServicesNum) && ((offset + 3u) < DCM_AUTHENTICATION_WHITE_LIST_SERVICE_MAX_SIZE); i++)
        {
            /* 1. Checks on service ID level */
            if (((sid == WhiteListServices[offset]) && (1u == length[i]))
                /* 5. Checks on service 0x31 per sub-function */
                || ((4u == length[i]) && (sid == WhiteListServices[offset])
                    && (reqData[0u] == WhiteListServices[offset + 1u])
                    && (reqData[1u] == WhiteListServices[offset + 2u])
                    && (reqData[2u] == WhiteListServices[offset + 3u])))
            {
                result = E_OK;
                break;
            }
            offset += length[i];
        }
        /* PRQA S 2994, 2996, 2880 -- */                /* VL_Dcm_2994, VL_Dcm_2996, VL_Dcm_2880 */
        if (E_NOT_OK == result) /* PRQA S 2991, 2995 */ /* VL_Dcm_2991, VL_Dcm_2995 */
        {
#ifdef DCM_AUTHENTICATION_WHITE_LIST_RID_MAX_SIZE
            /* check Rid white list*/
            ServicesNum       = whiteList->WhiteListRidLength;
            WhiteListServices = whiteList->WhiteListRid;
            for (uint8 i = 0u; (i < ServicesNum) && (E_NOT_OK == result); i += 3u)
            {
                /*chehck Rid is same*/
                if (((reqData[1u] == WhiteListServices[i]) && (reqData[2u] == WhiteListServices[i + 1u]))
                    /*check startRoutine */
                    && ((
                        ((subFunction == 0x01u) && ((DCM_RID_START_ACCESS & WhiteListServices[i + 2u]) != 0u))
                        /*check stopRoutine*/
                        || ((subFunction == 0x02u) && ((DCM_RID_STOP_ACCESS & WhiteListServices[i + 2u]) != 0u))
                        /*check requestRoutineResult*/
                        || ((subFunction == 0x03u) && ((DCM_RID_REQUEST_ACCESS & WhiteListServices[i + 2u]) != 0u)))))
                {
                    result = E_OK;
                }
            }
#endif
        }
    }
    return result;
}
#endif
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
