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
 **  @file               : CDD_FVM.h
 **  @author             : Jian.Jiang
 **  @date               : 2023/10/20
 **  @vendor             : isoft
 **  @description        : Freshness Value Manager
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00    2023-10-20  Jian.Jiang   R19-11 FVM initial version.
 *  V01.00.01    2023-12-15  Jian.Jiang   Modify the receiving mechanism to require its upward growth and add frame
 *                                     loss check.
 *  V01.00.02    2024-04-22  Jian.Jiang   Solve the problem that FVM multicounters can be changed only after secoc
 *                                     changes, as well as the problem of NVM usage.
 *  V01.00.03    2024-06-11  Jian.Jiang   Solve the problem that FVM sends and receives when TripConter and
 *                                     ResetCounter are not full bytes, resulting in a shift error.
 *  V01.00.04    2024-06-14  Jian.Jiang   Solving the problem of reading and writing NVM exception handling for
 *                                     master and slave machines.
 *  V01.00.05    2024-07-11  Jian.Jiang   Add judgement PDUR to send return value
 *  V01.00.06    2024-08-06  Jian.Jiang   Solve the problem that the first frame master is delayed due to failure to
 *                                     send
 *  V01.01.00    2025-01-10  Jian.Jiang   Make code specification changes based on R23 version
 *  V01.01.01    2025-02-14  Jian.Jiang   Solving Nested Loop Inclusion Problems in Header Files
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FVM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_CDD_FVM_1532
      Reason: In order to reduce the number of lines of code, some interfaces are designed as external interfaces.
      Risk: Still only called within this module, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_CDD_FVM_1503
      Reason: Allow some external functions to be uninvoked. Since we provide an API set, whether they are invoked
            depends on how the project integrates them..
      Risk: Causing waste of flash space.
      Prevention: Code review.

    \li VL_CDD_FVM_4397
      Reason: Macro Conveniently determining authenticity.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CDD_FVM_3673
      Reason: The parameter will be modified, it could not be qulified as "const".
      Risk: This parameter is declared as an array may be misused.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_CDD_FVM_2985
      Reason: Allow some conditional statements to always equal the result on the left or right. The product code has
  high reusability, and in certain configuration scenarios, conditional statements are redundant.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_MTR_CDD_FVM_STTPP
      Reason: The number of lines of code is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CDD_FVM_STVAR
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CDD_FVM_STCAL
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CDD_FVM_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CDD_FVM_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CDD_FVM_3408
      Reason: There is no declaration before the definition.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef CDD_FVM_H
#define CDD_FVM_H

/* =================================================== inclusions =================================================== */
#include "CDD_FVM_Types.h"
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "SecOC.h"

#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
/** Module ID for CDD_FVM */
#define CDD_FVM_MODULE_ID 2099u
/** Instance ID for CDD_FVM */
#define CDD_FVM_INSTANCE_ID 0u
/** Vendor ID for CDD_FVM */
#define CDD_FVM_VENDOR_ID 62u
/** Software major version for CDD_FVM */
#define CDD_FVM_SW_MAJOR_VERSION 1u
/** Software minor version for CDD_FVM */
#define CDD_FVM_SW_MINOR_VERSION 1u
/** Software patch version for CDD_FVM */
#define CDD_FVM_SW_PATCH_VERSION 1u
/** AUTOSAR major version for CDD_FVM */
#define CDD_FVM_AR_MAJOR_VERSION 4u
/** AUTOSAR minor version for CDD_FVM */
#define CDD_FVM_AR_MINOR_VERSION 9u
/** AUTOSAR patch version for CDD_FVM */
#define CDD_FVM_AR_PATCH_VERSION 0u

/* ===================================================== macros ===================================================== */

/** Development Error: Invalid pointer parameter */
#define CDD_FVM_E_PARAM_POINTER 0x01u
/** Development Error: Module not initialized */
#define CDD_FVM_E_UNINIT 0x02u
/** Development Error: Invalid PDU SDU ID */
#define CDD_FVM_E_INVALID_PDU_SDU_ID 0x03u
/**Development Error: Initialization failed */
#define CDD_FVM_E_INIT_FAILED 0x07u

/*Service ID*/
/** Service ID for CDD_FVM_Init */
#define CDD_FVM_INIT_ID ((uint8)0x01u)
/** Service ID for CDD_FVM_DeInit */
#define CDD_FVM_DEINIT_ID ((uint8)0x02u)
/** Service ID for CDD_FVM_GetVersionInfo */
#define CDD_FVM_GETVERSIONINFO ((uint8)0x03u)
/** Service ID for CDD_FVM_GetRxFreshness */
#define CDD_FVM_GETRXFRESHNESS ((uint8)0x04u)
/** Service ID for CDD_FVM_GetRxFreshnessAuthData */
#define CDD_FVM_GETRXFRESHNESSAUTHDATA ((uint8)0x05u)
/** Service ID for CDD_FVM_GetTxFreshness */
#define CDD_FVM_GETTXFRESHNESS ((uint8)0x06u)
/** Service ID for CDD_FVM_GetTxFreshnessTruncData */
#define CDD_FVM_GETTXFRESHNESSTRUNCDATA ((uint8)0x07u)
/** Service ID for CDD_FVM_SPduTxConfirmation */
#define CDD_FVM_SPDUTXCONFIRMATION ((uint8)0x08u)
/** Service ID for CDD_FVM_RxIndication */
#define CDD_FVM_RXINDICATION_ID ((uint8)0X09u)
/** Service ID for CDD_FVM_SlaveRequestSyncMsg */
#define CDD_FVM_SLAVEREQUESTSYNCMSG ((uint8)0X0Au)
/** Service ID for CDD_FVM_RxSecOCVerificationNotify */
#define CDD_FVM_RXSECOCVERIFICATIONNOTIFY ((uint8)0X0Bu)

#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
/**
 * @brief          Report error to DET
 * @param[in]      ApiId: API ID
 * @param[in]      ErrorId: Error ID
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70853
 */
CDD_FVM_LOCAL_INLINE void CDD_FVM_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CDD_FVM_MODULE_ID, CDD_FVM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**
 * @brief    Configuration structure variable of CDD_FVM. @range NA
 */
extern const CDD_FVM_ConfigType CDD_FVM_Config;

/* ========================================= external function declarations ========================================= */
/**
 * @brief          Initializes the the FVM module.
 * @param[in]      config: Pointer to a selected configuration structure
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70854
 */
void CDD_FVM_Init(const CDD_FVM_ConfigType* config);

/**
 * @brief          This service stops the fvm.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70855
 */
void CDD_FVM_DeInit(void);

/**
 * @brief          Returns the version information of this module
 * @param[out]     versioninfo: Pointer to where to store the version
 *                              information of this module.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70856
 */
void CDD_FVM_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief          This interface is used by the FVM to obtain the current
 *                 freshness value.
 * @param[in]      FVMFreshnessValueID: Holds the identifier of the freshness
 *                 value.
 * @param[in]      FVMTruncatedFreshnessValue: Holds the truncated freshness
 *                 value that was contained in the Secured I-PDU.
 * @param[in]      FVMTruncatedFreshnessValueLength: Holds the length in bits
 *                 of the truncated freshness value.
 * @param[in]      FVMAuthVerifyAttempts: Hold the number of authentication
 *                 verify attempts of this PDU since the last reception.
 * @param[inout]   FVMFreshnessValueLength: Holds the length in bits of the
 *                 freshness value.
 * @param[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                 for the calculation of the authenticator.
 * @return         Std_ReturnType
 * @retval         E_OK: request successful.
 * @retval         E_NOT_OK: request failed, a freshness value cannot be
 *                           provided due to general issues for freshness
 *                           orx this FreshnessValueId.
 * @retval         E_BUSY: The freshness information can temporarily not be
 *                         provided.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70857
 */
Std_ReturnType CDD_FVM_GetRxFreshness(
    uint16       FVMFreshnessValueID,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength);

/**
 * @brief          This interface is used by the FVM to obtain the current
 *                 freshness value.
 * @param[in]      FVMFreshnessValueID: Holds the identifier of the freshness
 *                 value.
 * @param[in]      FVMTruncatedFreshnessValue: Holds the truncated freshness
 *                 value that was contained in the Secured I-PDU.
 * @param[in]      FVMTruncatedFreshnessValueLength: Holds the length in bits
 *                 of the truncated freshness value.
 * @param[in]      FVMAuthDataFreshnessValue: The parameter holds a part of
 *                 the received, not yet authenticated PDU.
 * @param[in]      FVMAuthDataFreshnessValueLength: This is the length value
 *                 in bits that holds the freshness from
 * @param[in]      FVMAuthVerifyAttempts: Holds the number of authentication
 *                 verify attempts of this PDU since the last reception.
 * @param[inout]   FVMFreshnessValueLength: Holds the length in bits of the
 *                 freshness value.
 * @param[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                 for the calculation of the authenticator.
 * @return         Std_ReturnType
 * @retval         E_OK: request successful.
 * @retval         E_NOT_OK: request failed, a freshness value cannot be
 *                           provided due to general issues for freshness orx
 *                           this FreshnessValueId.
 * @retval         E_BUSY: The freshness information can temporarily not be
 *                         provided.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace       CPD-70858
 */
Std_ReturnType CDD_FVM_GetRxFreshnessAuthData(
    uint16       FVMFreshnessValueID,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    const uint8* FVMAuthDataFreshnessValue,
    uint16       FVMAuthDataFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength);

/**
 * @brief          This API returns the freshness value from the Most Significant
 *                 Bits in the first byte in the array (FVMFreshnessValue), in
 *                 big endian format.
 * @param[in]      FVMFreshnessValueID: Holds the identifier of the freshness value.
 * @param[inout]   FVMFreshnessValueLength: Holds the length in bits of the freshness
 *                 value.
 * @param[out]     FVMFreshnessValue: Holds the freshness value to be used for the
 *                 calculation of the authenticator.
 * @return         Std_ReturnType
 * @retval         E_OK: request successful.
 * @retval         E_NOT_OK: request failed, a freshness value cannot be provided
 *                 due to general issues for freshness orx this FreshnessValueId.
 * @retval         E_BUSY: The freshness information can temporarily not be provided.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70859
 */
Std_ReturnType
    CDD_FVM_GetTxFreshness(uint16 FVMFreshnessValueID, uint8* FVMFreshnessValue, uint32* FVMFreshnessValueLength);

/**
 * @brief          This interface is used by the FVM to obtain the current
 *                 freshness value. The interface function provides also the
 *                 truncated freshness transmitted in the secured I-PDU.
 * @param[in]      FVMFreshnessValueID: Holds the identifier of the freshness value.
 * @param[inout]   FVMFreshnessValueLength: Holds the length in bits of the
 *                 freshness value.
 * @param[inout]   FVMTruncatedFreshnessValueLength: Provides the truncated
 *                 freshness length configured for this freshness.
 * @param[out]     FVMFreshnessValue: Holds the current freshness value.
 * @param[out]     FVMTruncatedFreshnessValue: Holds the truncated freshness to
 *                 be included into the Secured I-PDU.
 * @return         Std_ReturnType
 * @retval         E_OK: request successful.
 * @retval         E_NOT_OK: request failed, a freshness value cannot be provided
 *                 due to general issues for freshness orx
 * @retval         E_BUSY: The freshness information can temporarily not be provided.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70860
 */
Std_ReturnType CDD_FVM_GetTxFreshnessTruncData(
    uint16  FVMFreshnessValueID,
    uint8*  FVMFreshnessValue,
    uint32* FVMFreshnessValueLength,
    uint8*  FVMTruncatedFreshnessValue,
    uint32* FVMTruncatedFreshnessValueLength);

/**
 * @brief          This interface is used by the FVM to indicate that the Secured
 *                 I-PDU has been initiated for transmission.
 * @param[in]      FVMFreshnessValueID: Holds the identifier of the freshness
 *                 value.
 * @return         Std_ReturnType
 * @retval         E_OK: Notification successful.
 * @retval         E_NOT_OK: Notification failed.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70861
 */
Std_ReturnType CDD_FVM_SPduTxConfirmation(uint16 FVMFreshnessValueID);

/**
 * @brief          This function is used to notify the reception of SecOC verification
 *                 status.
 * @param[in]      verificationStatus: SecOC verification status to notify.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70862
 */
void CDD_FVM_RxSecOCVerificationNotify(SecOC_VerificationStatusType verificationStatus);

/**
 * @brief          This function is the Rx indication callback function for the CDD
 *                 FVM module.
 * @param[in]      RxPduId: ID of the received PDU.
 * @param[in]      PduInfoPtr: Pointer to the received PDU data.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70863
 */
void CDD_FVM_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief          This function is the main function for the master ECU of the
 *                 CDD FVM module.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70864
 */
#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON) && (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
void CDD_FVM_MainFunctionMaster(void);
#endif /* CDD_FVM_MASTER_ECU_ENABLED == STD_ON */

/**
 * @brief          This function is used to request a synchronization message by
 *                 the slave.
 * @param[in]      syncMsgId: ID of the synchronization message.
 * @return         Std_ReturnType
 * @retval         Standard return type indicating the success or failure of the function.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace       CPD-70865
 */
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
Std_ReturnType CDD_FVM_SlaveRequestSyncMsg(uint16 syncMsgId);
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON && CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u*/

#ifdef __cplusplus
}
#endif
#endif /* CDD_FVM_H */
