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
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2022/7/27 14:29:43                                         **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                     **
**                                                                           **
**************************************************************************** */
#include "Dcm_Internal.h"
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
#include "KeyM.h"
#include "Csm.h"

/****************************************************************
             UDS:Authentication (29 hex) service
 ***************************************************************/

/* SWS_Dcm_01559
   AUTOSAR Dcm only implements the authentication via PKI certificated exchange.
   Authentication with challenge-response (ACR) is out of scope of AUTOSAR.
   If it is required it needs a full custom implementation using existing Dcm
   callouts for custom service processing.*/
/*deAuthenticate*/
#define DCM_DEAUTHENTICATE 0x0u
/*verifyCertificateUnidirectional*/
#define DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL 0x1u
/*verifyCertificateBidirectional*/
#define DCM_VERIFYCERTIFICATE_BIDIRECTIONAL 0x2u
/*proofOfOwnership*/
#define DCM_PROOFOFOWNERSHIP 0x3u
/*authenticationConfiguration*/
#define DCM_AUTHENTICATION_CONFIGURATION 0x8u

/*DeAuthentication successful.*/
#define DCM_DEAUTHENTICATION_SUCCESSFUL 0x10u

/*AuthenticationConfiguration APCE*/
#define DCM_AUTHENTICATION_APCE 0x2u

/*Certificate verified, Ownership verification necessary*/
#define DCM_AUTHENTICATION_CVOVN 0x11u

/*Ownership verified, Authentication complete*/
#define DCM_AUTHENTICATION_CVAC 0x12u

/* Bit 0: Read access */
#define DCM_DID_READ_ACCESS 0x01u
/* Bit1: Write access */
#define DCM_DID_WRITE_ACCESS 0x02u
/* Bit2: Control access (by InputOutputControlByIdentifier) */
#define DCM_DID_CONTROL_ACCESS 0x04u

/* Bit 0: Access to sub-function startRoutine鈥� */
#define DCM_RID_START_ACCESS 0x01u
/* Bit1: Access to sub-function stopRoutine鈥� */
#define DCM_RID_STOP_ACCESS 0x02u
/* Bit2: Access to sub-function requestRoutineResult*/
#define DCM_RID_REQUEST_ACCESS 0x04u

typedef enum
{
    DCM_AUTHENTICATIONTIMER_ON = 0,
    DCM_AUTHENTICATIONTIMER_OFF = 1
} Dcm_AuthenticationStateType;

typedef struct
{
    uint32 Dcm_AuthenticationCurTimer;
    Dcm_AuthenticationStateType Dcm_AuthenticationState; /*Authenticationtimer status */
} Dcm_AuthenticationCtrlType;

typedef enum
{
    DCM_DEAUTHENTICATED = 0x1u,
    DCM_PRE_AUTHENTICATE = 0x2u,
    DCM_AUTHENTICATE = 0x3u,
} Dcm_AuthenticateStatusTypes;

typedef enum
{
    DCM_ASYNC_IDLE = 0x0U,
    DCM_KEYM_VERIFY_ASYNC_PENDING,
    DCM_KEYM_VERIFY_ASYNC_FINISH,
    DCM_CSM_GENERATE_ASYNC_PENDING,
    DCM_CSM_GENERATE_ASYNC_FINISH,
    DCM_CSM_SIGNATURE_ASYNC_PENDING,
    DCM_CSM_SIGNATURE_ASYNC_FINISH,
    DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING,
    DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC,
} Dcm_AuthenticateProcessStatusTypes;

typedef struct
{
    uint8 Dcm_WhiteListDidLength;
    uint8 Dcm_WhiteListDid[DCM_DSP_AUTHENTICATION_DID_WHITE_LIST];
    uint8 Dcm_WhiteListRidLength;
    uint8 Dcm_WhiteListRid[DCM_DSP_AUTHENTICATION_RID_WHITE_LIST];
    uint8 Dcm_WhiteListServicesNum;
    uint8 Dcm_WhiteListServiceslength[DCM_DSP_AUTHENTICATION_SERVICE_WHITE_LIST];
    uint8 Dcm_WhiteListServices[DCM_DSP_AUTHENTICATION_SERVICE_WHITE_LIST];
} Dcm_WhiteListTypes;

typedef struct
{
    Dcm_AuthenticateStatusTypes Dcm_AuthenticateStatus;
    uint32 Dcm_Certificate;
    Dcm_WhiteListTypes Dcm_WhiteList;
} Dcm_AuthenticateInfoTypes;

typedef struct
{
    KeyM_CertificateIdType CertId;
    KeyM_CertElementIdType CertElementId;
    uint8 WhiteListMaxSize;
    uint8* Dcm_WhiteListNum;
    uint8* Dcm_WhiteListlength;
} Dcm_KeyMCertInfoTypes;

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
static VAR(Dcm_AuthenticationCtrlType, DCM_VAR_NOINIT) Dcm_AuthenticationCtrl[DCM_MAINCONNECTION_NUM];

static VAR(Dcm_AuthenticateInfoTypes, DCM_VAR_NOINIT) Dcm_AuthenticateInfo[DCM_MAINCONNECTION_NUM];
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_16
#include "Dcm_MemMap.h"
static VAR(Crypto_ResultType, DCM_VAR_NOINIT) Dcm_CsmResult;
#define DCM_STOP_SEC_VAR_NO_INIT_16
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"
static VAR(uint32, DCM_VAR_NOINIT) Dcm_CsmJobId;
#define DCM_STOP_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_POWER_ON_INIT_32
#include "Dcm_MemMap.h"
static VAR(uint32, DCM_VAR_POWER_ON_INIT) Dcm_0x29ResLen = 0;
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
static VAR(KeyM_CertificateStatusType, DCM_VAR_NOINIT) Dcm_KeyMResult;
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_16
#include "Dcm_MemMap.h"
static VAR(KeyM_CertificateIdType, DCM_VAR_NOINIT) Dcm_KeyMCertID;
#define DCM_STOP_SEC_VAR_NO_INIT_16
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
static VAR(Dcm_AuthenticateProcessStatusTypes, DCM_VAR_NOINIT) Dcm_AuthenticateAsync;

static VAR(Crypto_VerifyResultType, DCM_VAR_NOINIT) Dcm_CsmVerifyResult;
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"

#if (                                                                    \
    (STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED)                    \
    || (STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED) \
    || (STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED))
#define DCM_START_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"
static VAR(uint32, DCM_VAR_NOINIT) Dcm_challengeLength;
#define DCM_STOP_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
static VAR(uint8, DCM_VAR_NOINIT) Dcm_challengeData[DCM_DSP_AUTHENTICATION_MAX_ECU_CHALLENGE_LENGTH];
#define DCM_STOP_SEC_VAR_NO_INIT_8
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED          \
|| STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED \
|| STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED */

#if (                                                                 \
    (STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED) \
    || (STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED))
#define DCM_START_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"
static VAR(uint32, DCM_VAR_NOINIT) Dcm_ProofOfOwnershipServerLength;

static VAR(uint32, DCM_VAR_NOINIT) Dcm_certDataLength;
#define DCM_STOP_SEC_VAR_NO_INIT_32
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED \
                || STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED*/

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

#if (NVM_ENABLE == STD_ON)
/*************************************************************************/
/*
 * Brief               <Dcm_AuthenticateStore>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_AuthenticateStore(void)
{
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    if (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationPersistRef)
    {
        uint16 blockId = *pDcm_DspAuthentication->DcmDspAuthenticationPersistRef;
        NvM_RequestResultType NvmRet;
        Dcm_MemoryCopy(
            (uint8*)&Dcm_AuthenticateInfo,
            NvM_BlockDescriptor[blockId - 1u].NvmRamBlockDataAddress,
            (sizeof(Dcm_AuthenticateInfoTypes) * DCM_MAINCONNECTION_NUM));
        (void)NvM_GetErrorStatus(blockId, &NvmRet);
        if (NVM_REQ_PENDING != NvmRet)
        {
            (void)NvM_WriteBlock(blockId, (void*)&Dcm_AuthenticateInfo);
        }
    }
}
#endif

#if (                                                                    \
    (STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED)                    \
    || (STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED) \
    || (STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED))
/*************************************************************************/
/*
 * Brief               <UDS  0x29 service SetNrc>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <ErrorCode>
 * Return              <None>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_UDS0x29_SetNrc(P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    /*the Dcm shall send the NRC code given in DcmDspAuthenticationGeneralNRC
    instead of the specific NRC codes in all situation where a Certificate verification failed
    - NRC is returned.*/
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    if (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationGeneralNRC)
    {
        *ErrorCode = *(pDcm_DspAuthentication->DcmDspAuthenticationGeneralNRC);
    }
}
#endif /* STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED          \
|| STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED \
|| STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED */

/*************************************************************************/
/*
 * Brief               <Dcm_SetAuthenticateRole>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_SetDeAuthenticateRole(uint8 connectionId)
{
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    /*set all role to inactive*/
    Dcm_AuthenticateInfo[connectionId].Dcm_Certificate = 0;
    /*set deAuthenticate role*/
    for (uint8 i = 0; i < pDcm_DspAuthentication->DcmDspAuthenticationDeauthenticatedRoleRefNum; i++)
    {
        uint8 pos = pDcm_DspAuthentication->DcmDspAuthenticationDeauthenticatedRoleRef[i];
        Dcm_AuthenticateInfo[connectionId].Dcm_Certificate |= (uint32)((uint32)1u << pos);
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_ClearWhiteList>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
static FUNC(void, DCM_CODE) Dcm_ClearWhiteList(uint8 connectionId)
{
    Dcm_WhiteListTypes* pDcm_WhiteList = &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList;
    /*Clear Did White List*/
    pDcm_WhiteList->Dcm_WhiteListDidLength = DCM_DSP_AUTHENTICATION_DID_WHITE_LIST;
    for (uint8 i = 0; i < DCM_DSP_AUTHENTICATION_DID_WHITE_LIST; i++)
    {
        pDcm_WhiteList->Dcm_WhiteListDid[i] = 0;
    }
    /*Clear Rid White List*/
    pDcm_WhiteList->Dcm_WhiteListRidLength = DCM_DSP_AUTHENTICATION_RID_WHITE_LIST;
    for (uint8 i = 0; i < DCM_DSP_AUTHENTICATION_RID_WHITE_LIST; i++)
    {
        pDcm_WhiteList->Dcm_WhiteListRid[i] = 0;
    }
    /*Clear Service White List*/
    pDcm_WhiteList->Dcm_WhiteListServicesNum = DCM_DSP_AUTHENTICATION_SERVICE_WHITE_LIST;
    for (uint8 i = 0; i < DCM_DSP_AUTHENTICATION_SERVICE_WHITE_LIST; i++)
    {
        pDcm_WhiteList->Dcm_WhiteListServices[i] = 0;
        pDcm_WhiteList->Dcm_WhiteListServiceslength[i] = 0;
    }
}

#if (STD_ON == DCM_UDS_0X29_DEAUTHENTICATE_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x00 sub>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_0x00(
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResLen)
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret = E_OK;
    uint8 connectionId;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId]
                       .DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    /*check the massage length*/
    if (DCM_UDS0X29_REQ_DATA_MINLENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    else
    {
        Dcm_SetDeAuthenticate(connectionId);
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].offset;
        /*On reception of an Authentication (0x29) service with subfunction equal to
          de-Authenticate, the Dcm shall reply with a positive response having
          the authenticationReturnParameter set to DeAuthentication successful.*/
        Dcm_Channel[Offset + 2u] = DCM_DEAUTHENTICATION_SUCCESSFUL;
        *ResLen = 1;
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_0X29_DEAUTHENTICATE_ENABLED */

#if (                                                                 \
    (STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED) \
    || (STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED))
/*************************************************************************/
/*
 * Brief               <UDS  0x29 service KeyM Async Check>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    Dcm_UDS0x29_KeyMAsyncCheck(P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;
    switch (Dcm_KeyMResult)
    {
    case KEYM_CERTIFICATE_VALID:
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
        ret = E_OK;
        break;
    case KEYM_E_CERTIFICATE_SIGNATURE_FAIL:
        /* SWS_Dcm_01469 Behavior on failed certificate verification
        After the Dcm has verified a certificate and KeyM_CertificateStatusType
        is set to KEYM_E_CERTIFICATE_SIGNATURE_FAIL, the Dcm shall send a
        negative response with NRC set to ertificate verification failed
        - Invalid Signature. */
        *ErrorCode = DCM_E_INVALIDSIGNATURE;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST:
        /* SWS_Dcm_01470 Check certificate chain of trust
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST, the Dcm shall refuse the client
        certificate and return a negative response with NRC ertificate verification
        failed - Invalid Chain of Trust. */
        *ErrorCode = DCM_E_INVALIDCHAINOFTRUST;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_INVALID_TYPE:
        /* SWS_Dcm_01471] Check certificate type
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_INVALID_TYPE, the Dcm shall refuse the client certificate and
        return a negative response with NRC ertificate verification failed - Invalid Type. */
        *ErrorCode = DCM_E_INVALIDTYPE;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_INVALID_FORMAT:
        /* SWS_Dcm_01472 Check certificate format
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_INVALID_FORMAT, the Dcm shall refuse the client certificate
        and return a negative response with NRC ertificate verification failed
        - Invalid Format. */
        *ErrorCode = DCM_E_INVALIDFORMAT;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_INVALID_CONTENT:
        /* SWS_Dcm_01473 Check certificate format
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_INVALID_CONTENT, the Dcm shall refuse the client certificate
        and return a negative response with NRC ertificate verification failed
        - Invalid Scope. */
        *ErrorCode = DCM_E_INVALIDSCOPE;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_REVOKED:
        /* SWS_Dcm_01475 Check certificate format
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_REVOKED, the Dcm shall refuse the client certificate
        and return a negative response with NRC ertificate verification failed
        - Invalid Certificate (revoked). */
        *ErrorCode = DCM_E_INVALIDCERTIFICATE;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    case KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL:
        /* SWS_Dcm_01476 Check certificate valid until
        If the operation started in [SWS_Dcm_01468] returns a result of
        KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL, the Dcm shall refuse the client
        certificate and return a negative response with NRC ertificate
        verification failed - Invalid Time Period. */
        *ErrorCode = DCM_E_INVALIDTIMEPERIOD;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    default:
        *ErrorCode = DCM_E_GENERALREJECT;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
        break;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x01 and 0x02 sub Challenge>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_Challenge(
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResLen)
{
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[pMsgContext->DcmRxPduId].DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    const Dcm_DspAuthenticationConnectionTypes* pAuthenticationConnection =
        &DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication->DcmDspAuthenticationConnection[connectionId];
    Std_ReturnType ret = E_OK;
    uint8 TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    uint32 Offset = pDcmChannelCfg->offset;
    uint32 length;

    if (DCM_ASYNC_IDLE == Dcm_AuthenticateAsync)
    {
        Dcm_challengeLength = (uint32)pAuthenticationConnection->DcmDspAuthenticationEcuChallengeLength;
        ret = Csm_RandomGenerate(
            pAuthenticationConnection->DcmDspAuthenticationRandomJobRef,
            Dcm_challengeData,
            &Dcm_challengeLength);
        if (E_OK == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret = DCM_E_PENDING;
            Dcm_AuthenticateAsync = DCM_CSM_GENERATE_ASYNC_PENDING;
            Dcm_CsmJobId = pAuthenticationConnection->DcmDspAuthenticationRandomJobRef;
        }
        else if (CRYPTO_E_BUSY == ret)
        {
            /* SWS_Dcm_01465 Behavior on busy crypto operation
            If any of the Csm or KeyM APIs called by the Dcm is returning
            CRYPTO_E_BUSY or KEYM_E_BUSY, the Dcm shall return the NRC 0x21 (busyRepeat).*/
            *ErrorCode = DCM_E_BUSYREPEATREQUEST;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
    }
    if (DCM_CSM_GENERATE_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
        if (E_OK == Dcm_CsmResult)
        {
            /*get the sub-function */
            uint8 subFunc = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
            Dcm_Channel[Offset + 2u] = DCM_AUTHENTICATION_CVOVN;
            Dcm_Channel[Offset + 3u] = (uint8)(Dcm_challengeLength >> 8u);
            Dcm_Channel[Offset + 4u] = (uint8)Dcm_challengeLength;
            Dcm_MemoryCopy(Dcm_challengeData, &Dcm_Channel[Offset + 5u], Dcm_challengeLength);
            (*ResLen) = 3u + Dcm_challengeLength;
            if (DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL == subFunc)
            {
                /*SWS_Dcm_01503 Sending positive response on verifyCertificateUniDirectional
                If the Dcm has successfully calculated server challenge the Dcm shall send a
                positive response for the verifyCertificateUniDirectional request with the following
                parameter values: authenticationReturnParameter : ertificate verified, Ownership
                verification necessary lengtfOfChallengeServer: length of the challenge generated in
                [SWS_Dcm_01493] challengeServer: challenge generated in [SWS_Dcm_01493]
                lengthOfEphemeralPublicKeyServer: 0x0000*/
                Dcm_Channel[Offset + 5u + Dcm_challengeLength] = 0;
                Dcm_Channel[Offset + 6u + Dcm_challengeLength] = 0;
                (*ResLen) += 2u;
                Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus = DCM_PRE_AUTHENTICATE;
                ret = E_OK;
            }
            else

            {
                KeyM_CertDataType CertData;
                CertData.certData = &Dcm_Channel[Offset + 7u + Dcm_challengeLength];
                CertData.certDataLength = pDcmChannelCfg->Dcm_DslBufferSize - (*ResLen);
                ret =
                    KeyM_GetCertificate((*pAuthenticationConnection->DcmDspAuthenticationECUCertificateRef), &CertData);
                if (E_OK == ret)
                {
                    uint16 lengthOfCertificateClient =
                        (uint16)(((uint16)pMsgContext->pReqData[3u] << 8) | (uint16)pMsgContext->pReqData[4u]);
                    uint16 lengthOfChallengeClient =
                        (uint16)(((uint16)pMsgContext->pReqData[5u + lengthOfCertificateClient] << 8)
                                 | (uint16)pMsgContext->pReqData[6u + lengthOfCertificateClient]);
                    /*SWS_Dcm_01504 Signing client challenge
                    On reception of an Authentication (0x29) service with sub-function equal to
                    verifyCertificateBidirectional, the Dcm shall sign the received client
                    challenge by calling Csm_SignatureGenerate with the following parameter values:
                    jobId: DcmDspAuthenticationClientChallengeSignJobRef -> CsmJobId
                    mode: CRYPTO_OPERATIONMODE_SINGLECALL
                    dataPtr: Pointer to challengeClient in request
                    dataLength: lengthOfChallengeClient from request
                    resultPtr: Dcm sided buffer to store the proofOfOwnershipServer
                    resultLengthPtr: Response data for lengthOfProofOfOwnershipServer*/
                    Dcm_certDataLength = CertData.certDataLength;
                    (*ResLen) += (2u + Dcm_certDataLength);
                    Dcm_CsmJobId = pAuthenticationConnection->DcmDspAuthenticationClientChallengeSignJobRef;
                    Dcm_Channel[Offset + 5u + Dcm_challengeLength] = (uint8)(Dcm_certDataLength >> 8u);
                    Dcm_Channel[Offset + 6u + Dcm_challengeLength] = (uint8)Dcm_certDataLength;
                    ret = Csm_SignatureGenerate(
                        Dcm_CsmJobId,
                        CRYPTO_OPERATIONMODE_SINGLECALL,
                        &pMsgContext->pReqData[7u + lengthOfCertificateClient],
                        lengthOfChallengeClient,
                        &Dcm_Channel[Offset + 9u + Dcm_challengeLength + Dcm_certDataLength],
                        &Dcm_ProofOfOwnershipServerLength);
                    if (E_OK == ret)
                    {
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                        ret = DCM_E_PENDING;
                        Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_ASYNC_PENDING;
                        Dcm_CsmJobId = pAuthenticationConnection->DcmDspAuthenticationClientChallengeSignJobRef;
                    }
                    else if (CRYPTO_E_BUSY == ret)
                    {
                        /* SWS_Dcm_01465 Behavior on busy crypto operation
                        If any of the Csm or KeyM APIs called by the Dcm is returning
                        CRYPTO_E_BUSY or KEYM_E_BUSY, the Dcm shall return the NRC 0x21 (busyRepeat).*/
                        *ErrorCode = DCM_E_BUSYREPEATREQUEST;
                        Dcm_UDS0x29_SetNrc(ErrorCode);
                        ret = E_NOT_OK;
                    }
                    else
                    {
                        *ErrorCode = DCM_E_GENERALREJECT;
                        Dcm_UDS0x29_SetNrc(ErrorCode);
                        ret = E_NOT_OK;
                    }
                }
                else
                {
                    /*SWS_Dcm_01507 Server certificate size check
                    If the API KeyM_GetCertificate returns KEYM_E_KEY_CERT_SIZE_MISMATCH (0x04),
                    the Dcm shall return NRC 0x10 (generalReject).*/
                    *ErrorCode = DCM_E_GENERALREJECT;
                    Dcm_UDS0x29_SetNrc(ErrorCode);
                    ret = E_NOT_OK;
                }
            }
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
    }
    if (DCM_CSM_SIGNATURE_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
        if (E_OK == Dcm_CsmResult)
        {
            length = Dcm_challengeLength + Dcm_certDataLength;
            /*SWS_Dcm_01508 Sending positive response on verifyCertificateBidirectional
            If the Dcm has successfully calculated server challenge, the server challenge and the
            server certificate, the Dcm shall send a positive response for the
            verifyCertificateBidirectional request with the following parameter values:
            authenticationReturnParameter: ertificate verified, Ownership verification necessary
            lengtfOfChallengeServer: length of the challenge generated in [SWS_Dcm_01493]
            challengeServer: challenge generated in [SWS_Dcm_01493]
            lengthOfCertificateServer: length of the certificated provided in [SWS_Dcm_01506]
            certificateServer: certificated provided in [SWS_Dcm_01506]
            lengthOfProofOfOwnershipServer: length of proof-of-ownership server created
            in[SWS_Dcm_01504] proofOfOwnershipServer: proof-of-ownership server created in
            [SWS_Dcm_01504] lengthOfEphemeralPublicKeyServer: 0x0000*/
            Dcm_Channel[Offset + 7u + length] = (uint8)(Dcm_ProofOfOwnershipServerLength >> 8u);
            Dcm_Channel[Offset + 8u + length] = (uint8)Dcm_ProofOfOwnershipServerLength;
            Dcm_Channel[Offset + 9u + length + Dcm_ProofOfOwnershipServerLength] = 0;
            Dcm_Channel[Offset + 10u + length + Dcm_ProofOfOwnershipServerLength] = 0;
            (*ResLen) += (4u + Dcm_ProofOfOwnershipServerLength);
            Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus = DCM_PRE_AUTHENTICATE;
            ret = E_OK;
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x01 and 0x02 sub>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_0x01_and_0x02(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResLen)
{
    uint8 MsgCtrlId;
    Std_ReturnType ret = E_OK;
    uint8 communicationConfiguration;
    uint16 lengthOfCertificateClient;
    uint16 lengthOfChallengeClient;
    uint8 connectionId;
    Std_ReturnType KeyMret;
    Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[pMsgContext->DcmRxPduId].DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);

    /*get all request info*/
    communicationConfiguration = pMsgContext->pReqData[2u];
    lengthOfCertificateClient = (uint16)(((uint16)pMsgContext->pReqData[3u] << 8) | (uint16)pMsgContext->pReqData[4u]);
    lengthOfChallengeClient = (uint16)(((uint16)pMsgContext->pReqData[5u + lengthOfCertificateClient] << 8)
                                       | (uint16)pMsgContext->pReqData[6u + lengthOfCertificateClient]);
    /* check the massage length */
    /* SWS_Dcm_01579 Mandatory certificate data dOn reception of an Authentication(0x29)
    service with sub-function equal to verifyCertificateUnidirectional or
    verifyCertificateBidirectionaland a lengthOfCertificateClient equal to zero,
    the Dcm shall respond with NRC 0x13 (incorrectMessageLengthOrInvalidFormat).*/
    if ((DCM_INITIAL == OpStatus)
        && ((((uint32)(7u + (uint32)lengthOfCertificateClient + (uint32)lengthOfChallengeClient))
             != pMsgContext->ReqDataLen)
            || (0x0u == lengthOfCertificateClient)))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }

    /* SWS_Dcm_01459 Supported communicationConfiguration
    On reception of an Authentication (0x29) service with sub-function equal to
    verifyCertificateUnidirectional or verifyCertificateBidirectional, the Dcm shall reply
    with an NRC 0x31 (requestOutOfRange) if the communicationConfiguration (COCO)
    parameter has a value different than 0x00.*/
    if ((DCM_INITIAL == OpStatus) && ((E_OK == ret) && (0x0u != communicationConfiguration)))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }

    if ((DCM_INITIAL == OpStatus) && (E_OK == ret))
    {
        uint16 CertificateId =
            DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication->DcmDspAuthenticationConnection[connectionId]
                .DcmDspAuthenticationConnectionCertificateRef;
        KeyM_CertDataType CertificateData;
        CertificateData.certData = &pMsgContext->pReqData[5];
        CertificateData.certDataLength = lengthOfCertificateClient;
        /* SWS_Dcm_01463 Certificate validation dOn reception of an Authentication (0x29)
        service with sub-function equal to verifyCertificateUnidirectional or
        verifyCertificateBidirectional, the Dcm shall use the KeyM API KeyM_SetCertificate
        to store the client certificate from the service request within the KeyM module.
        The following parameter values shall be used:
        CertificateId =
        DcmDspAuthenticationConnectionCertificateRef->KeyMCertificate.KeyMCertificateId
        certificateDataPtr.certData = Pointer to certificateClient from Request
        certificateDataPtr.certDataLength = lengthOfCertificateClient from Request*/
        Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
        KeyMret = KeyM_SetCertificate(CertificateId, &CertificateData);
        switch (KeyMret)
        {
        case E_OK:
            /* SWS_Dcm_01468 Verifying client certificate
            After the Dcm has set the certificate according to [SWS_Dcm_01463],
            the Dcm shall verify the certificate by calling KeyM_VerifyCertificate
            with the following parameters:
            CertificateId = DcmDspAuthenticationConnectionCertificateRef->
            KeyMMCertificate.KeyMCertificateId */
            KeyMret = KeyM_VerifyCertificate(CertificateId);
            if (E_OK == KeyMret)
            {
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                ret = DCM_E_PENDING;
                Dcm_AuthenticateAsync = DCM_KEYM_VERIFY_ASYNC_PENDING;
                Dcm_KeyMCertID = CertificateId;
            }
            else if (KEYM_E_BUSY == KeyMret)
            {
                /* SWS_Dcm_01465 Behavior on busy crypto operation
                If any of the Csm or KeyM APIs called by the Dcm is returning
                CRYPTO_E_BUSY or KEYM_E_BUSY, the Dcm shall return the NRC 0x21 (busyRepeat).*/
                *ErrorCode = DCM_E_BUSYREPEATREQUEST;
                Dcm_UDS0x29_SetNrc(ErrorCode);
                ret = E_NOT_OK;
            }
            else
            {
                *ErrorCode = DCM_E_GENERALREJECT;
                Dcm_UDS0x29_SetNrc(ErrorCode);
                ret = E_NOT_OK;
            }
            break;
        case KEYM_E_KEY_CERT_SIZE_MISMATCH:
            /*SWS_Dcm_01464 Invalid certificate size failure
            If the API KeyM_SetCertificate returns KEYM_E_SIZE_MISMATCH,
            the Dcm shall return the NRC 0x13 (incorrectMessageLengthOrInvalidFormat).*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
            break;
        default:
            /*SWS_Dcm_01466 Csm APIs returning failure code behavior
            Throughout this chapter the Csm or KeyM are used to process the
            authentication requests. These APIs have return values for failures.
            Unless there is no dedicated requirement for a given return value
            and if the return value is different to E_OK, the Dcm shall return
            a NRC 0x10 eneralReject.*/
            *ErrorCode = DCM_E_GENERALREJECT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
            break;
        }
    }
    if (DCM_KEYM_VERIFY_ASYNC_FINISH == Dcm_AuthenticateAsync)
    {
        ret = Dcm_UDS0x29_KeyMAsyncCheck(ErrorCode);
    }
    if ((E_OK == ret) && (DCM_KEYM_VERIFY_ASYNC_PENDING != Dcm_AuthenticateAsync))
    {
        ret = Dcm_UDS0x29_Challenge(ProtocolCtrlId, ErrorCode, ResLen);
    }
    if ((E_OK == ret) && (DCM_ASYNC_IDLE != Dcm_AuthenticateAsync))
    {
        ret = DCM_E_PENDING;
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED \
        || STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED */

#if (STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x03 sub get list>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_0x03_getlist(
    Dcm_KeyMCertInfoTypes Dcm_KeyMCertInfo,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DataPtr,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;
    KeyM_CertElementIteratorType CertElementIterator;
    uint32 getDataLength = *DataLength;
    uint32 TotalLength = *DataLength;

    *DataLength = 0;
    /*get white list*/
    ret = KeyM_CertElementGetFirst(
        Dcm_KeyMCertInfo.CertId,
        Dcm_KeyMCertInfo.CertElementId,
        &CertElementIterator,
        DataPtr,
        &getDataLength);
    if (E_OK == ret)
    {
        Dcm_KeyMCertInfo.Dcm_WhiteListlength[*Dcm_KeyMCertInfo.Dcm_WhiteListNum] = (uint8)getDataLength;
        (*Dcm_KeyMCertInfo.Dcm_WhiteListNum) += 1u;
        /* SWS_Dcm_01518 White list size check */
        (*DataLength) = (uint8)getDataLength;
        if (getDataLength > Dcm_KeyMCertInfo.WhiteListMaxSize)
        {
            *ErrorCode = DCM_E_INVALIDCONTENT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
        while (E_OK == ret)
        {
            getDataLength = TotalLength - (*DataLength);
            ret = KeyM_CertElementGetNext(&CertElementIterator, &DataPtr[*DataLength], &getDataLength);
            /* SWS_Dcm_01518 White list size check */
            if (E_OK == ret)
            {
                Dcm_KeyMCertInfo.Dcm_WhiteListlength[*Dcm_KeyMCertInfo.Dcm_WhiteListNum] = (uint8)getDataLength;
                (*Dcm_KeyMCertInfo.Dcm_WhiteListNum) += 1u;
                (*DataLength) += (uint8)getDataLength;
            }
        }
    }
    if ((*DataLength) > Dcm_KeyMCertInfo.WhiteListMaxSize)
    {
        *ErrorCode = DCM_E_INVALIDCONTENT;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
    }
    else if (E_NOT_OK == ret)
    {
        ret = E_OK;
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x03 sub update white list>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    Dcm_UDS0x29_0x03_whitelist(uint8 connectionId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;
    const Dcm_DspAuthenticationConnectionTypes* pAuthenticationConnection =
        &DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication->DcmDspAuthenticationConnection[connectionId];
    Dcm_KeyMCertInfoTypes Dcm_KeyMCertInfo;
    uint8 listNum = 0;
    uint8 Data[DCM_DSP_AUTHENTICATION_SERVICE_WHITE_LIST] = {0};
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    Dcm_WhiteListTypes* pDcm_WhiteList = &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList;

    Dcm_ClearWhiteList(connectionId);
    /*get services white list*/
    if (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationWhiteListServicesMaxSize)
    {
        Dcm_KeyMCertInfo.WhiteListMaxSize = *pDcm_DspAuthentication->DcmDspAuthenticationWhiteListServicesMaxSize;
    }
    Dcm_KeyMCertInfo.CertElementId = pAuthenticationConnection->DcmDspAuthenticationWhiteListServicesElementRef;
    Dcm_KeyMCertInfo.CertId = pAuthenticationConnection->DcmDspAuthenticationConnectionCertificateRef;
    Dcm_KeyMCertInfo.Dcm_WhiteListNum = &listNum;
    Dcm_KeyMCertInfo.Dcm_WhiteListlength = Data;
    ret = Dcm_UDS0x29_0x03_getlist(
        Dcm_KeyMCertInfo,
        pDcm_WhiteList->Dcm_WhiteListServices,
        &pDcm_WhiteList->Dcm_WhiteListServicesNum,
        ErrorCode);
    pDcm_WhiteList->Dcm_WhiteListServicesNum = listNum;
    Dcm_MemoryCopy(Data, pDcm_WhiteList->Dcm_WhiteListServiceslength, listNum);

    if ((E_OK == ret) && (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationWhiteListDIDMaxSize)
        && (NULL_PTR != pAuthenticationConnection->DcmDspAuthenticationWhiteListDIDElementRef))
    {
        /*get Did white list*/
        listNum = 0;
        Dcm_KeyMCertInfo.WhiteListMaxSize = *pDcm_DspAuthentication->DcmDspAuthenticationWhiteListDIDMaxSize;
        Dcm_KeyMCertInfo.CertElementId = *pAuthenticationConnection->DcmDspAuthenticationWhiteListDIDElementRef;
        Dcm_KeyMCertInfo.CertId = pAuthenticationConnection->DcmDspAuthenticationConnectionCertificateRef;
        ret = Dcm_UDS0x29_0x03_getlist(
            Dcm_KeyMCertInfo,
            Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListDid,
            &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListDidLength,
            ErrorCode);
    }

    if ((E_OK == ret) && (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationWhiteListRIDMaxSize)
        && (NULL_PTR != pAuthenticationConnection->DcmDspAuthenticationWhiteListRIDElementRef))
    {
        /*get Rid white list*/
        listNum = 0;
        Dcm_KeyMCertInfo.WhiteListMaxSize = *pDcm_DspAuthentication->DcmDspAuthenticationWhiteListRIDMaxSize;
        Dcm_KeyMCertInfo.CertElementId = *pAuthenticationConnection->DcmDspAuthenticationWhiteListRIDElementRef;
        Dcm_KeyMCertInfo.CertId = pAuthenticationConnection->DcmDspAuthenticationConnectionCertificateRef;
        ret = Dcm_UDS0x29_0x03_getlist(
            Dcm_KeyMCertInfo,
            Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListRid,
            &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListRidLength,
            ErrorCode);
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x03 sub update role list>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    Dcm_UDS0x29_0x03_Update(uint8 connectionId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    const Dcm_DspAuthenticationConnectionTypes* pAuthenticationConnection =
        &pDcm_DspAuthentication->DcmDspAuthenticationConnection[connectionId];
    uint8 CertElementData[4];
    uint32 CertElementDataLength = 4;

    /*get role*/
    ret = KeyM_CertElementGet(
        pAuthenticationConnection->DcmDspAuthenticationConnectionCertificateRef,
        pAuthenticationConnection->DcmDspAuthenticationRoleElementRef,
        CertElementData,
        &CertElementDataLength);
    if (E_OK == ret)
    {
        if (CertElementDataLength != pDcm_DspAuthentication->DcmDspAuthenticationRoleSize)
        {
            /*SWS_Dcm_01515 Role size check
            If the size of the read role information in [SWS_Dcm_01514] is different than the
            size in DcmDspAuthenticationRoleSize, the Dcm shall send a negative response with
            NRC ertificate verification failed - Invalid Content.*/
            *ErrorCode = DCM_E_INVALIDCONTENT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
        else
        {
            /*set all role to inactive*/
            Dcm_AuthenticateInfo[connectionId].Dcm_Certificate = 0;
            /*set Authenticate role*/
            for (uint8 i = 0; i < CertElementDataLength; i++)
            {
                /*SWS_Dcm_01521 Integer interpretation of roles in certificates
                The Dcm shall interpret all configured role integer values in the little endian
                format.*/
                Dcm_AuthenticateInfo[connectionId].Dcm_Certificate |=
                    (uint32)((uint32)CertElementData[i] << (CertElementDataLength - i - 1u));
            }
            ret = Dcm_UDS0x29_0x03_whitelist(connectionId, ErrorCode);
            /*set Authenticate status*/
            if (E_OK == ret)
            {
                Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus = DCM_AUTHENTICATE;
#if (NVM_ENABLE == STD_ON)
                Dcm_AuthenticateStore();
#endif
            }
        }
    }
    else
    {
        *ErrorCode = DCM_E_GENERALREJECT;
        Dcm_UDS0x29_SetNrc(ErrorCode);
        ret = E_NOT_OK;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x03 sub>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_0x03(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResLen)
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret = (DCM_INITIAL == OpStatus) ? E_OK : DCM_E_PENDING;
    uint32 lengthOfProofOfOwnershipClient;
    uint32 lengthOfEphemeralPublicKeyClient;
    uint8 connectionId;
    const Dcm_DspAuthenticationConnectionTypes* pAuthenticationConnection;
    Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[pMsgContext->DcmRxPduId].DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);

    /*get all request info*/
    lengthOfProofOfOwnershipClient =
        (uint32)(((uint32)pMsgContext->pReqData[2u] << 8) | (uint32)pMsgContext->pReqData[3u]);
    lengthOfEphemeralPublicKeyClient =
        (uint32)(((uint32)pMsgContext->pReqData[4u + lengthOfProofOfOwnershipClient] << 8)
                 | (uint32)pMsgContext->pReqData[5u + lengthOfProofOfOwnershipClient]);

    /*check the massage length*/
    if ((DCM_INITIAL == OpStatus)
        && (((6u + lengthOfProofOfOwnershipClient + lengthOfEphemeralPublicKeyClient)) != pMsgContext->ReqDataLen))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }

    /*check request Sequence*/
    if ((DCM_INITIAL == OpStatus) && (E_OK == ret)
        && (DCM_DEAUTHENTICATED == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus))
    {
        /*SWS_Dcm_01509 Sequence check
        On reception of an Authentication (0x29) service with sub-function equal to
        proofOfOwnership and if on this connection the most recent processed
        verifyCertificateUnidirectional or verifyCertificateBidirectional service
        failed or no such sub-function was processed yet, the Dcm shall send the negative
        response equestSequenceError. The connection shall remain in de-authenticated state.*/
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        ret = E_NOT_OK;
    }

    if ((DCM_INITIAL == OpStatus) && (E_OK == ret))
    {
        pAuthenticationConnection =
            &DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication->DcmDspAuthenticationConnection[connectionId];
        ret = Csm_SignatureVerify(
            pAuthenticationConnection->DcmDspAuthenticationVerifyProofOfOwnerShipClientJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            Dcm_challengeData,
            Dcm_challengeLength,
            &pMsgContext->pReqData[4],
            lengthOfProofOfOwnershipClient,
            &Dcm_CsmVerifyResult);
        if (E_OK == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret = DCM_E_PENDING;
            Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING;
            Dcm_CsmJobId = pAuthenticationConnection->DcmDspAuthenticationVerifyProofOfOwnerShipClientJobRef;
        }
        else if (CRYPTO_E_BUSY == ret)
        {
            /* SWS_Dcm_01465 Behavior on busy crypto operation
            If any of the Csm or KeyM APIs called by the Dcm is returning
            CRYPTO_E_BUSY or KEYM_E_BUSY, the Dcm shall return the NRC 0x21 (busyRepeat).*/
            *ErrorCode = DCM_E_BUSYREPEATREQUEST;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
        else
        {
            *ErrorCode = DCM_E_GENERALREJECT;
            Dcm_UDS0x29_SetNrc(ErrorCode);
            ret = E_NOT_OK;
        }
    }

    if (DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC == Dcm_AuthenticateAsync)
    {
        if ((E_OK == Dcm_CsmResult) && (CRYPTO_E_VER_OK == Dcm_CsmVerifyResult))
        {
            TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
            TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
            Offset = DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].offset;
            /*SWS_Dcm_01513 SessionKey support
            Upon sending a positive response for an authentication request with sub-function
            equal to proofOfOwnership, the Dcm shall:
            set all bytes of lengthOfSessionKeyInfo to 0x00
            omit the sessionKeyInfo*/
            Dcm_Channel[Offset + 2u] = DCM_AUTHENTICATION_CVAC;
            Dcm_Channel[Offset + 3u] = 0;
            Dcm_Channel[Offset + 4u] = 0;
            (*ResLen) += 3u;
            ret = Dcm_UDS0x29_0x03_Update(connectionId, ErrorCode);
        }
        else
        {
            if (CRYPTO_E_VER_OK != Dcm_CsmVerifyResult)
            {
                /*SWS_Dcm_01512 Failed ownership verification
                If the result of Csm_SignatureVerify from [SWS_Dcm_01511] is Crypto_VerifyResultType
                equal to CRYPTO_E_VER_NOT_OK, the Dcm shall send a negative response with
                NRC wnership verification failed.*/
                *ErrorCode = DCM_E_OWNERSHIPVERIFICATIONFAILED;
                Dcm_UDS0x29_SetNrc(ErrorCode);
                ret = E_NOT_OK;
            }
            else
            {
                *ErrorCode = DCM_E_GENERALREJECT;
                Dcm_UDS0x29_SetNrc(ErrorCode);
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#endif /*STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED */

#if (STD_ON == DCM_UDS_0X29_AUTHENTICATION_CONFIGURATION_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x29 service 0x08 sub>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x29_0x08(
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode,
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResLen)
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret = E_OK;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*check the massage length*/
    if (DCM_UDS0X29_REQ_DATA_MINLENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    else
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].offset;
        /*SWS_Dcm_01533 Providing the authentication configuration
        If DcmDspAuthentication is configured and an Authentication (0x29)
        service with subfunction equal to authenticationConfiguration is received,
        the Dcm shall send a positive response with authenticationReturnParameter
        set to uthenticationConfiguration APCE.*/
        Dcm_Channel[Offset + 2u] = DCM_AUTHENTICATION_APCE;
        *ResLen = 1;
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_0X29_AUTHENTICATION_CONFIGURATION_ENABLED */

/*************************************************************************/
/*
 * Brief               <UDS  0x29 service>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x29(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 subFunc;
    uint32 Offset;
    Std_ReturnType ret;
    const Dcm_DslBufferType* pDcmChannelCfg;

    DCM_UNUSED(OpStatus);
    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

    /*get the sub-function */
    subFunc = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
    switch (subFunc)
    {
#if (STD_ON == DCM_UDS_0X29_DEAUTHENTICATE_ENABLED)
    case DCM_DEAUTHENTICATE:
        /*deAuthenticate*/
        ret = Dcm_UDS0x29_0x00(ProtocolCtrlId, ErrorCode, &Dcm_0x29ResLen);
        break;
#endif
#if (STD_ON == DCM_UDS_0X29_VERIFYCERTIFICATE_UNIDIRECTIONAL_ENABLED)
    case DCM_VERIFYCERTIFICATE_UNIDIRECTIONAL:
        /*verifyCertificateUnidirectional*/
        ret = Dcm_UDS0x29_0x01_and_0x02(OpStatus, ProtocolCtrlId, ErrorCode, &Dcm_0x29ResLen);
        break;
#endif
#if (STD_ON == DCM_UDS_0X29_BIDIRECTIONAL_ENABLED)
    case DCM_VERIFYCERTIFICATE_BIDIRECTIONAL:
        /*verifyCertificateBidirectional*/
        ret = Dcm_UDS0x29_0x01_and_0x02(OpStatus, ProtocolCtrlId, ErrorCode, &Dcm_0x29ResLen);
        break;
#endif
#if (STD_ON == DCM_UDS_0X29_PROOFOFOWNERSHIP_ENABLED)
    case DCM_PROOFOFOWNERSHIP:
        /*proofOfOwnership*/
        ret = Dcm_UDS0x29_0x03(OpStatus, ProtocolCtrlId, ErrorCode, &Dcm_0x29ResLen);
        break;
#endif
#if (STD_ON == DCM_UDS_0X29_AUTHENTICATION_CONFIGURATION_ENABLED)
    case DCM_AUTHENTICATION_CONFIGURATION:
        /*authenticationConfiguration*/
        ret = Dcm_UDS0x29_0x08(ProtocolCtrlId, ErrorCode, &Dcm_0x29ResLen);
        break;
#endif
    default:
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        ret = E_NOT_OK;
        break;
    }

    if (E_OK == ret)
    {
        /* assemble positive response*/
        /*the processing is successful,assemble positive response*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        /* check tx data length */
        if ((2u + Dcm_0x29ResLen) > pDcmChannelCfg->Dcm_DslBufferSize)
        {
            /*Pdu length is bigger than buffer size,ignore the request message */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
        else
        {
            Dcm_Channel[Offset] = 0x69;         /*response SID*/
            Dcm_Channel[Offset + 1u] = subFunc; /*echo of 0 - 6 bits of sub-function*/
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x02u + Dcm_0x29ResLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen = 0x02u + Dcm_0x29ResLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    /* Dcm_0x29ResLen shall be store because async, there is end of 0x29 so clear */
    if ((E_OK == ret) || (E_NOT_OK == ret))
    {
        Dcm_0x29ResLen = 0;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Dcm_SetDeAuthenticate>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_SetDeAuthenticate(uint8 connectionId)
{
    /*SWS_Dcm_01484 Clearing persisted authentication state
    If the authentication state of a connection performs a transition to deauthenticated state,
    the Dcm shall clear all persisted authentication information on that connection.*/
    /*SWS_Dcm_01485 Reaction of fallback into deauthenticated state
    Upon a transition from authenticated into deauthenticated state, the Dcm shall discard
    the current role, white list and use the configured deauthentication role from
    DcmDspAuthenticationDeauthenticatedRole.*/
    Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus = DCM_DEAUTHENTICATED;
    Dcm_AuthenticationCtrl[connectionId].Dcm_AuthenticationState = DCM_AUTHENTICATIONTIMER_OFF;
    Dcm_SetDeAuthenticateRole(connectionId);
    Dcm_ClearWhiteList(connectionId);
#if (NVM_ENABLE == STD_ON)
    Dcm_AuthenticateStore();
#endif
    Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
}

/*************************************************************************/
/*
 * Brief               <Dcm_AuthenticateInit>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthenticateInit(void)
{
    Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
    Dcm_CsmVerifyResult = CRYPTO_E_VER_OK;
#if (NVM_ENABLE == STD_ON)
    NvM_RequestResultType NvmRet;
    const uint16* PersistRef = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication->DcmDspAuthenticationPersistRef;
#endif

    for (uint8 i = 0; i < DCM_MAINCONNECTION_NUM; i++)
    {
        Dcm_SetDeAuthenticate(i);
    }
#if (NVM_ENABLE == STD_ON)
    if (NULL_PTR != PersistRef)
    {
        uint16 blockId = *PersistRef;
        (void)NvM_GetErrorStatus(blockId, &NvmRet);
        if (NVM_REQ_OK == NvmRet)
        {
            Dcm_MemoryCopy(
                NvM_BlockDescriptor[blockId - 1u].NvmRamBlockDataAddress,
                (uint8*)&Dcm_AuthenticateInfo,
                (sizeof(Dcm_AuthenticateInfoTypes) * DCM_MAINCONNECTION_NUM));
        }
    }
#endif
    for (uint8 i = 0; i < DCM_MAINCONNECTION_NUM; i++)
    {
        DspInternal_SetAuthenticationCtrlOff(i);
        DspInternal_SetAuthenticationCtrlOn(i);
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_KeyMAsyncCertificateVerifyFinished>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <CertID/Result>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
Dcm_KeyMAsyncCertificateVerifyFinished(KeyM_CertificateIdType CertID, KeyM_CertificateStatusType Result)
{
    if (Dcm_KeyMCertID == CertID)
    {
        Dcm_KeyMResult = Result;
        Dcm_AuthenticateAsync = DCM_KEYM_VERIFY_ASYNC_FINISH;
    }
    return E_OK;
}

/*************************************************************************/
/*
 * Brief               <Dcm_CsmAsyncJobFinished>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <job/Result>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_CsmAsyncJobFinished(const Crypto_JobType* job, Crypto_ResultType result)
{
    if (Dcm_CsmJobId == job->jobId)
    {
        Dcm_CsmResult = result;
        switch (Dcm_AuthenticateAsync)
        {
        case DCM_CSM_GENERATE_ASYNC_PENDING:
            Dcm_AuthenticateAsync = DCM_CSM_GENERATE_ASYNC_FINISH;
            break;
        case DCM_CSM_SIGNATURE_ASYNC_PENDING:
            Dcm_AuthenticateAsync = DCM_CSM_SIGNATURE_ASYNC_FINISH;
            break;
        case DCM_CSM_SIGNATURE_VERIFY_ASYNC_PENDING:
            Dcm_AuthenticateAsync = DCM_CSM_FINISH_SIGNATURE_VERIFY_ASYNC;
            break;
        default:
            Dcm_AuthenticateAsync = DCM_ASYNC_IDLE;
            break;
        }
    }
    return E_OK;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_RoleCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DspInternal_RoleCheck(uint8 ProtocolCtrlId, P2CONST(uint8, AUTOMATIC, DCM_CONST) RoleRef, uint8 RoleNum)
{
    Std_ReturnType ret = E_NOT_OK;
    /*check Currently valid Role state*/
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    PduIdType DcmRxPduId = Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[DcmRxPduId].DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    const uint8* pRoleRef = RoleRef;
    uint8 pRoleNum = RoleNum;
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;

    if (0u == pRoleNum)
    {
        ret = E_OK;
    }
    /*SWS_Dcm_01538 Access rights for services in deauthenticated state
    If the current connection is in deauthenticated state, the Dcm shall use
    the role configured by DcmDspAuthenticationDeauthenticatedRoleRef as
    current role for all role based access verification checks.*/
    if (DCM_DEAUTHENTICATED == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
    {
        const uint8* dRoleRef = pDcm_DspAuthentication->DcmDspAuthenticationDeauthenticatedRoleRef;
        uint8 dRoleNum = pDcm_DspAuthentication->DcmDspAuthenticationDeauthenticatedRoleRefNum;
        for (uint8 i = 0; (i < dRoleNum) && (E_NOT_OK == ret); i++)
        {
            for (uint8 j = 0; (j < pRoleNum) && (E_NOT_OK == ret); j++)
            {
                if (pRoleRef[j] == dRoleRef[i])
                {
                    ret = E_OK;
                }
            }
        }
    }
    else
    {
        for (uint8 i = 0; (i < pRoleNum) && (E_NOT_OK == ret); i++)
        {
            if ((Dcm_AuthenticateInfo[connectionId].Dcm_Certificate & (uint32)((uint32)1u << pRoleRef[i])) != 0u)
            {
                ret = E_OK;
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_WhiteListServiceCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DspInternal_WhiteListServiceCheck(uint8 ProtocolCtrlId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId]
                             .DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    uint8 Sid = Dcm_MsgCtrl[MsgCtrlId].SID;
    uint8 ServicesNum = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServicesNum;
    uint8* WhiteListServices = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServices;
    uint8* length = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServiceslength;
    uint8 offset = 0;
    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
    {
        /* check service white list*/
        for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i++)
        {
            if (Sid == WhiteListServices[offset])
            {
                ret = E_OK;
            }
            offset += length[i];
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_SetAuthenticationCtrlOn>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DspInternal_SetAuthenticationCtrlOn(uint8 connectionId)
{
    if ((DCM_DEFAULT_SESSION == Dcm_MkCtrl.Dcm_ActiveSes)
        && (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus))
    {
        Dcm_AuthenticationCtrl[connectionId].Dcm_AuthenticationState = DCM_AUTHENTICATIONTIMER_ON;
        Dcm_ResetTime(&Dcm_AuthenticationCtrl[connectionId].Dcm_AuthenticationCurTimer);
    }
}

/*************************************************************************/
/*
 * Brief               <DspInternal_SetAuthenticationCtrlOff>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DspInternal_SetAuthenticationCtrlOff(uint8 connectionId)
{
    if ((DCM_DEFAULT_SESSION != Dcm_MkCtrl.Dcm_ActiveSes)
        || (DCM_AUTHENTICATE != Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus))
    {
        Dcm_AuthenticationCtrl[connectionId].Dcm_AuthenticationState = DCM_AUTHENTICATIONTIMER_OFF;
    }
}

/*************************************************************************/
/*
 * Brief               <Authentication Timer background processing function>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) Dcm_MainFunction_AuthenticationTimer(void)
{
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    if (NULL_PTR != pDcm_DspAuthentication->DcmDspAuthenticationDefaultSessionTimeOut)
    {
        for (uint8 i = 0; i < DCM_MAINCONNECTION_NUM; i++)
        {
            /* DcmDspAuthenticationDefaultSessionTimeOut check*/
            if (DCM_AUTHENTICATIONTIMER_ON == Dcm_AuthenticationCtrl[i].Dcm_AuthenticationState)
            {
                uint32 Timer;
                uint32 OldTimer = Dcm_AuthenticationCtrl[i].Dcm_AuthenticationCurTimer;
                uint32 ExpireTimer = *pDcm_DspAuthentication->DcmDspAuthenticationDefaultSessionTimeOut;
                Dcm_GetTimeSpan(OldTimer, &Timer);
                if (Timer >= ExpireTimer)
                {
                    /*SWS_Dcm_01482 Fallback to deauthenticated session on idle connection
                    The Dcm shall make a transition from authenticated into deauthenticated
                    state for a configured connection if the following conditions apply:
                    The Dcm was in default session when the last diagnostic response was
                    send on that connection and
                    DcmDspAuthenticationDefaultSessionTimeOut is configured and no valid
                    diagnostic request was received on that connection for
                    DcmDspAuthenticationDefaultSessionTimeOut seconds after the last
                    Dcm_TpTxConfirmation on that connection.*/
                    Dcm_SetDeAuthenticate(i);
                }
            }
        }
    }
}

/*************************************************************************/
/*
 * Brief               <Dcm_SetDeauthenticatedRole>
 * ServiceId           <0x79>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId,deauthenticatedRole>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(void, DCM_CODE)
Dcm_SetDeauthenticatedRole(uint16 connectionId, P2CONST(uint8, AUTOMATIC, DCM_VAR) deauthenticatedRole)
{
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    /*SWS_Dcm_01486 Default authentication role set from SWC
    If a connection is in deauthenticated state and the API Dcm_SetDeauthenticatedRole
    is called, the Dcm shall use the provided deauthenticatedRole as new role per
    deauthenticated state for this connection.
    SWS_Dcm_01487 Setting deauthenticated role by SWC only in deauthenticated state
    The Dcm shall process a call of Dcm_SetDeauthenticatedRole only if the connection
    is in deauthenticated state.
    SWS_Dcm_01488 Lifetime of deauthenticated role by SWC
    A deauthenticated role set by Dcm_SetDeauthenticatedRole is discarded when that
    connection performs a transition authenticated state.
    SWS_Dcm_01489 No persistency for deauthenticated roles by SWC
    At startup the ECU shall always use the deauthentication state configured in
    DcmDspAuthenticationDeauthenticatedRole.*/
    if ((DCM_DEAUTHENTICATED == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
        && (NULL_PTR != pDcm_DspAuthentication))
    {
        /*set all role to inactive*/
        Dcm_AuthenticateInfo[connectionId].Dcm_Certificate = 0;
        /*set Deauthenticate role*/
        for (uint8 i = 0; i < pDcm_DspAuthentication->DcmDspAuthenticationRoleSize; i++)
        {
            /*SWS_Dcm_01521 Integer interpretation of roles in certificates
            The Dcm shall interpret all configured role integer values in the little endian
            format.*/
            Dcm_AuthenticateInfo[connectionId].Dcm_Certificate |= (uint32)((uint32)deauthenticatedRole[i] << (3u - i));
        }
    }
}

/*************************************************************************/
/*
 * Brief               <DspInternal_WhiteListCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternal_WhiteListCheck(uint8 ProtocolCtrlId, uint8 checkLength)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId]
                             .DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    uint8* req = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;
    uint8 ServicesNum = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServicesNum;
    uint8* WhiteListServices = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServices;
    uint8* length = Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList.Dcm_WhiteListServiceslength;
    uint8 offset = 0;
    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
    {
        /* check service white list*/
        for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i++)
        {
            /* 1. Checks on service ID level */
            if (((req[0] == WhiteListServices[offset]) && (1u == length[i]))
                /* 2. Checks on service ID and sub-function level */
                || ((checkLength >= 2u) && (2u == length[i]) && (req[0] == WhiteListServices[offset])
                    && (req[1] == WhiteListServices[offset + 1u]))
                /* 3. Checks for services with one or multiple DIDs */
                /* 4. Check on dynamically defined DIDs */
                || ((checkLength >= 3u) && (3u == length[i]) && (req[0] == WhiteListServices[offset])
                    && (req[1] == WhiteListServices[offset + 1u]) && (req[2] == WhiteListServices[offset + 2u]))
                /* 5. Checks on service 0x31 per sub-function */
                || ((checkLength >= 4u) && (4u == length[i]) && (req[0] == WhiteListServices[offset])
                    && (req[1] == WhiteListServices[offset + 1u]) && (req[2] == WhiteListServices[offset + 2u])
                    && (req[3] == WhiteListServices[offset + 3u])))
            {
                ret = E_OK;
            }
            offset += length[i];
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <Dcm_GetconnectionIndex>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <connectionId>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 */
/*************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_GetconnectionIndex(uint8 connectionId)
{
    uint8 ret = 0;
    const Dcm_DspAuthenticationTypes* pDcm_DspAuthentication = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspAuthentication;
    uint8 ConnectionNum = pDcm_DspAuthentication->DcmDspAuthenticationConnectionNum;
    for (uint8 iloop = 0; iloop < ConnectionNum; iloop++)
    {
        if (pDcm_DspAuthentication->DcmDspAuthenticationConnection[iloop]
                .DcmDspAuthenticationConnectionMainConnectionRef
            == connectionId)
        {
            ret = iloop;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_SubServiceAuthenticationCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId,ErrorCode,checkLength>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DspInternal_SubServiceAuthenticationCheck(
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    /*if the required protocol is configured,get the index of runtime datum*/
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 Sid = Dcm_MsgCtrl[MsgCtrlId].SID;
    uint16 SidTabCfgIndex;
    uint16 SidTabServiceCfgIndex;
    uint16 SubServieCfgIndex;
    Std_ReturnType ret;

    ret = DsdInternal_SearchSidTabServiceIndex(Sid, ProtocolCtrlId, &SidTabCfgIndex, &SidTabServiceCfgIndex);
    if (E_OK == ret)
    {
        const Dcm_DsdServiceCfgType* pDcmDsdService =
            &DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable[SidTabCfgIndex].pDcmDsdService[SidTabServiceCfgIndex];
        if ((TRUE == pDcmDsdService->DcmDsdSubfuncAvial))
        {
            ret = DsdInternal_SearchSidTabSubServiceIndex(
                ProtocolCtrlId,
                SidTabCfgIndex,
                SidTabServiceCfgIndex,
                &SubServieCfgIndex);
            if (E_OK == ret)
            {
                const Dcm_DsdSubServiceCfgType* pSubService = &pDcmDsdService->DcmDsdSubService[SubServieCfgIndex];
                uint8 subRoleRefNum = pSubService->DcmDsdSubServiceRoleRef_Num;
                const uint8* subRoleRef = pSubService->DcmDsdSubServiceRoleRef;
                ret = DspInternal_RoleCheck(ProtocolCtrlId, subRoleRef, subRoleRefNum);
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    if (E_NOT_OK == ret)
    {
        ret = DspInternal_WhiteListCheck(ProtocolCtrlId, DCM_UDS_REQ_DATA_MINLENGTH);
    }
    if (E_NOT_OK == ret)
    {
        /*SWS_Dcm_01544 Response behavior of services without access rights
        If the service execution verification fails due to a failed check in
        scope of [SWS_Dcm_01540], the Dcm shall send a NRC 0x34 authenticationRequired
        and stop the service processing.*/
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return ret;
}

#if (                                                                                  \
    (STD_ON == DCM_UDS_SERVICE0X22_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X2E_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED))
/*************************************************************************/
/*
 * Brief               <DspInternal_DidWhiteListCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternal_DidWhiteListCheck(uint8 ProtocolCtrlId, uint8 Sid, uint16 RecDid)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId]
                             .DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    Dcm_WhiteListTypes* Dcm_WhiteList = &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList;
    uint8 ServicesNum = Dcm_WhiteList->Dcm_WhiteListServicesNum;
    uint8* WhiteListServices = Dcm_WhiteList->Dcm_WhiteListServices;
    uint8* length = Dcm_WhiteList->Dcm_WhiteListServiceslength;
    uint8 offset = 0;
    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
    {
        /* check service white list*/
        for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i++)
        {
            /* 1. Checks on service ID level */
            if (((Sid == WhiteListServices[offset]) && (1u == length[i]))
                /* 3. Checks for services with one or multiple DIDs */
                /* 4. Check on dynamically defined DIDs */
                || ((3u == length[i]) && (Sid == WhiteListServices[offset])
                    && ((uint8)(RecDid >> 8u) == WhiteListServices[offset + 1u])
                    && ((uint8)RecDid == WhiteListServices[offset + 2u])))
            {
                ret = E_OK;
            }
            offset += length[i];
        }
        if (E_NOT_OK == ret)
        {
            /* check did white list*/
            ServicesNum = Dcm_WhiteList->Dcm_WhiteListDidLength;
            WhiteListServices = Dcm_WhiteList->Dcm_WhiteListDid;
            for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i += 3u)
            {
                /*chehck did is same*/
                if ((((uint8)(RecDid >> 8u) == WhiteListServices[i]) && ((uint8)RecDid == WhiteListServices[i + 1u]))
                    /*check read*/
                    || (((Sid == SID_READ_DATA_BY_IDENTIFIER) || (Sid == SID_READ_DATA_BY_PERIODIC_IDENTIFER))
                        && ((DCM_DID_READ_ACCESS & WhiteListServices[i + 2u]) != 0u))
                    /*check write*/
                    || ((Sid == SID_WRITE_DATA_BY_IDENTIFIER)
                        && ((DCM_DID_WRITE_ACCESS & WhiteListServices[i + 2u]) != 0u))
                    /*check control*/
                    || ((Sid == SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER)
                        && ((DCM_DID_CONTROL_ACCESS & WhiteListServices[i + 2u]) != 0u)))
                {
                    ret = E_OK;
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_DidAuthenticationCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId,ErrorCode,checkLength>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DspInternal_DidAuthenticationCheck(
    uint8 ProtocolCtrlId,
    uint16 RecDid,
    uint16 DidInfoCfgIdx,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    /*if the required protocol is configured,get the index of runtime datum*/
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 Sid = Dcm_MsgCtrl[MsgCtrlId].SID;
    Std_ReturnType ret;
    uint8 RoleRefNum;
    const uint8* RoleRef;

    const Dcm_DspDidInfoType* pDcmDspDidInfo = &DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo[DidInfoCfgIdx];
    if ((Sid == SID_READ_DATA_BY_IDENTIFIER) || (Sid == SID_READ_DATA_BY_PERIODIC_IDENTIFER))
    {
        const Dcm_DspDidReadType* DcmDspDidRead = pDcmDspDidInfo->pDcmDspDidRead;
        RoleRefNum = DcmDspDidRead->DcmDspDidReadRoleRef_Num;
        RoleRef = DcmDspDidRead->DcmDspDidReadRoleRef;
    }
    else if (Sid == SID_WRITE_DATA_BY_IDENTIFIER)
    {
        const Dcm_DspDidWriteType* DcmDspDidWrite = pDcmDspDidInfo->pDcmDspDidWrite;
        RoleRefNum = DcmDspDidWrite->DcmDspDidWriteRoleRef_Num;
        RoleRef = DcmDspDidWrite->DcmDspDidWriteRoleRef;
    }
    else
    {
        const Dcm_DspDidControlType* DcmDspDidControl = pDcmDspDidInfo->pDcmDspDidControl;
        RoleRefNum = DcmDspDidControl->DcmDspDidControlRoleRef_Num;
        RoleRef = DcmDspDidControl->DcmDspDidControlRoleRef;
    }
    ret = DspInternal_RoleCheck(ProtocolCtrlId, RoleRef, RoleRefNum);
    if (E_NOT_OK == ret)
    {
        ret = DspInternal_DidWhiteListCheck(ProtocolCtrlId, Sid, RecDid);
    }
    if (E_NOT_OK == ret)
    {
        /*SWS_Dcm_01544 Response behavior of services without access rights
        If the service execution verification fails due to a failed check in
        scope of [SWS_Dcm_01540], the Dcm shall send a NRC 0x34 authenticationRequired
        and stop the service processing.*/
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X22_ENABLED  \
        || STD_ON == DCM_UDS_SERVICE0X2A_ENABLED \
        || STD_ON == DCM_UDS_SERVICE0X2E_ENABLED \
        || STD_ON == DCM_UDS_SERVICE0X2F_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X31_ENABLED)
/*************************************************************************/
/*
 * Brief               <DspInternal_RidWhiteListCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternal_RidWhiteListCheck(uint8 ProtocolCtrlId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8* req = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;
    uint8 connectionId = DcmPbCfgPtr->DslProtocolConnectionRxCfg[Dcm_MsgCtrl[MsgCtrlId].MsgContext.DcmRxPduId]
                             .DcmDslParentConnectionCtrlId;
    connectionId = Dcm_GetconnectionIndex(connectionId);
    Dcm_WhiteListTypes* Dcm_WhiteList = &Dcm_AuthenticateInfo[connectionId].Dcm_WhiteList;
    uint8 ServicesNum = Dcm_WhiteList->Dcm_WhiteListServicesNum;
    uint8* WhiteListServices = Dcm_WhiteList->Dcm_WhiteListServices;
    uint8* length = Dcm_WhiteList->Dcm_WhiteListServiceslength;
    uint8 offset = 0;
    if (DCM_AUTHENTICATE == Dcm_AuthenticateInfo[connectionId].Dcm_AuthenticateStatus)
    {
        /* check service white list*/
        for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i++)
        {
            /* 1. Checks on service ID level */
            if (((req[0u] == WhiteListServices[offset]) && (1u == length[i]))
                /* 5. Checks on service 0x31 per sub-function */
                || ((4u == length[i]) && (req[0u] == WhiteListServices[offset])
                    && (req[1u] == WhiteListServices[offset + 1u]) && (req[2u] == WhiteListServices[offset + 2u])
                    && (req[3u] == WhiteListServices[offset + 3u])))
            {
                ret = E_OK;
            }
            offset += length[i];
        }
        if (E_NOT_OK == ret)
        {
            /* check Rid white list*/
            ServicesNum = Dcm_WhiteList->Dcm_WhiteListRidLength;
            WhiteListServices = Dcm_WhiteList->Dcm_WhiteListRid;
            for (uint8 i = 0; (i < ServicesNum) && (E_NOT_OK == ret); i += 3u)
            {
                /*chehck Rid is same*/
                if (((req[2u] == WhiteListServices[i]) && (req[3u] == WhiteListServices[i + 1u]))
                    /*check startRoutine */
                    || (((req[1u] == DCM_UDS0X31_STARTROUTINE)
                         && ((DCM_RID_START_ACCESS & WhiteListServices[i + 2u]) != 0u))
                        /*check stopRoutine*/
                        || ((req[1u] == DCM_UDS0X31_STOPROUTINE)
                            && ((DCM_RID_STOP_ACCESS & WhiteListServices[i + 2u]) != 0u))
                        /*check requestRoutineResult*/
                        || ((req[1u] == DCM_UDS0X31_REQUESTROUTINERESULTS)
                            && ((DCM_RID_REQUEST_ACCESS & WhiteListServices[i + 2u]) != 0u))))
                {
                    ret = E_OK;
                }
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <DspInternal_RidAuthenticationCheck>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId,ErrorCode,checkLength>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 */
/*************************************************************************/
FUNC(Std_ReturnType, DCM_CODE)
DspInternal_RidAuthenticationCheck(
    uint8 ProtocolCtrlId,
    uint8 RoutineCfgIdx,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    /*if the required protocol is configured,get the index of runtime datum*/
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 SubSid = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
    Std_ReturnType ret;
    uint8 RoleRefNum;
    const uint8* RoleRef;

    ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    if (E_OK == ret)
    {
        const Dcm_DspRoutineType* pDcmDspRoutine = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRoutine;
        boolean call = TRUE;
        switch (SubSid)
        {
        case DCM_UDS0X31_STARTROUTINE:
            RoleRefNum = pDcmDspRoutine[RoutineCfgIdx].DcmDspStartRoutine->DcmDspStartRoutineRoleRef_Num;
            RoleRef = pDcmDspRoutine[RoutineCfgIdx].DcmDspStartRoutine->DcmDspStartRoutineRoleRef;
            break;
        case DCM_UDS0X31_STOPROUTINE:
            RoleRefNum = pDcmDspRoutine[RoutineCfgIdx].DcmDspStopRoutine->DcmDspStopRoutineRoleRef_Num;
            RoleRef = pDcmDspRoutine[RoutineCfgIdx].DcmDspStopRoutine->DcmDspStopRoutineRoleRef;
            break;
        case DCM_UDS0X31_REQUESTROUTINERESULTS:
            RoleRefNum =
                pDcmDspRoutine[RoutineCfgIdx].DcmDspRequestRoutineResults->DcmDspRequestRoutineResultsRoleRef_Num;
            RoleRef = pDcmDspRoutine[RoutineCfgIdx].DcmDspRequestRoutineResults->DcmDspRequestRoutineResultsRoleRef;
            break;
        default:
            call = FALSE;
            ret = E_NOT_OK;
            break;
        }
        if (TRUE == call)
        {
            ret = DspInternal_RoleCheck(ProtocolCtrlId, RoleRef, RoleRefNum);
        }
        if (E_NOT_OK == ret)
        {
            ret = DspInternal_RidWhiteListCheck(ProtocolCtrlId);
        }
    }
    if (E_NOT_OK == ret)
    {
        /*SWS_Dcm_01544 Response behavior of services without access rights
        If the service execution verification fails due to a failed check in
        scope of [SWS_Dcm_01540], the Dcm shall send a NRC 0x34 authenticationRequired
        and stop the service processing.*/
        *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X31_ENABLED */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
