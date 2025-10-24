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
 **  @file               : Xcp.h
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of XCP
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version         Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V1.0.0       2020-01-18  qinchun.yang    Modify DAQ consistency bug.
 *  V1.0.1       2020-01-19  qinchun.yang    Increase STIM function.
 *  V1.0.2       2020-01-20  qinchun.yang    Modify DAQ resume function.Modify SET_REQUEST command.
 *               2020-02-12  qinchun.yang
 *   1> Move the Xcp_CanIfRxIndication and Xcp_CanIfTxConfirmation to XcpOnCan.c
 *   2> Modify Xcp_Transmit API (Add Ethernet sending function).
 *   3> Initial version.Distinguish between different transport layers
 *  V1.0.3       2020-05-06  qinchun.yang    Modify function Xcp_EventIndication().
 *  V1.0.4       2020-05-08  qinchun.yang    Solve byte alignment bug(Dynamic allocation daq).
 *  V1.0.5       2021-02-05  qinchun.yang    Add interleaved communication mode.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 *  V2.0.1       2023-02-20  qinchun.yang    Added modification to DAQ measurement length.
 *  V2.0.1       2023-04-12  qinchun.yang    PRD0042022-659 & PRD0042022-782.
 *  V2.0.2       2023-05-09  tong.zhao
 *    1> Delete invalid code in Xcp_RxIndication() when PID OFF is ON (JIRA:CPT-279)
 *  V2.0.3       2023-05-15  qinchun.yang
 *    1> Modify UPLOAD/SHORT_UPLOAD/BULID_CHECKSUM ERR_ACCESS_LOCKED error check.
 *               2023-05-26  tong.zhao       fix Bug CPT-90
 *    2> Modify the bug of API Xcp_GetSeedHal when GET_SEED command to request
 *    the remaining part of the seed, don't care resource.
 *  V2.0.4       2023-08-15  tong.zhao       fix bug CPT-61
 *    1> Modify the bug of API Xcp_CheckAddress ,error generated when addr overflows after calculation.
 *  V2.0.5       2023-11-07  tong.zhao       fix bug CPT-6464
 *    1> add through short_upload command to get measurement data in CANape's polling mode.
 *  V2.0.6       2023-11-21  qinchun.yang       CPD-33590
 *    1> Code execution efficiency optimization.
 *  V2.0.7       2023-12-07  qinchun.yang    fix bug CPT-7804 & CPT-7805
 *    1> CONNECT & DISCONNECT command.
 *  V2.0.8       2024-04-09  qinchun.yang    Remove daq list priority process logic;
 *  V2.0.9       2024-08-02  shaoqiang.Liang    Replace the Frt interface with the internal XCP ReadOutMS interface;
 *  V2.0.10      2024-08-13  shaoqiang.Liang    Modify DAQ consistency bug.
 *  V3.0.0       2024-11-17  qinchun.yang       R23-11 Initial Version(Inc QAC Modify).
 *  V3.0.1       2025-01-17  shaoqiang.Liang    Application of PBS.
 *  V3.0.2       2025-01-22  shaoqiang.Liang    Replacement of the IStdLib interface. Nvm interface contains.
 *  V3.0.3       2025-02-25  shaoqiang.Liang    Remove duplicate judgment conditions and modify the inclusion of the
 CanIf configuration file.
 *  V3.0.4       2025-03-03  shaoqiang.Liang    Fix the bugs related to static DAQ configuration.
 *  V3.0.5       2025-03-06  shaoqiang.Liang    Fix the bug when closing PGM, where the related types of PGM are not
 compiled when PGM is closed.
 *  V3.0.6       2025-03-26  shaoqiang.Liang    Add little-endian and big-endian self-check functionality.
 ==================================================================================================================== */

#ifndef XCP_H
#define XCP_H

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Xcp<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Xcp_3218
      Reason: file scoped variables only used in one function are necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3233
      Reason: file scoped variables only used in one function are necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2995
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2985
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2986
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_1039
      Reason: Configure the length of the generated array, it depends on the configuration, may be configured as 1
              length.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_1258
      Reason: pduLength type will variy depnding on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Xcp_5004
      Reason: pointer type definition is essential
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_4440
      Reason: function-like macros are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3678
      Reason: non-const type declaration is necessary depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0832
      Reason: Defining header files using generated references for macros
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_IndirectUse
      Reason: This variable needs to be used in the user's callback interface.
      Risk: It depends on the user configuration, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_Xcp_0553
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2841
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0750
      Reason: It may lead to memory alignment and filling issues, byte order issues, and type safety issues.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0759
      Reason: It may lead to memory alignment and filling issues, byte order issues, and type safety issues.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_TYPE_CAST_001
      Reason: Global variables are used in the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0306
      Reason: The code needs to convert integer Pointers into Pointers pointing to objects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0309
      Reason: The code is designed so.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0326
      Reason: Forced conversions of void Pointers and Pointers of other types will be used in the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Xcp_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Xcp_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
    stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Xcp_STPTH
      Reason: The functions were not separated into sub-functions because there were higher requirements for
    minimizing the stack and applying them to the runtime usage of the code
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_4391
      Reason: In the code, it is necessary to convert a small range of unsigned integers into a larger range of
    unsigned integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_4397
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_4399
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2001
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_2015
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Xcp_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
    but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3305
      Reason: The code is designed so
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0310
      Reason: Compatible with different types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_0722
      Reason: Enum constant not explicitly initialized.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_1338
      Reason: The parameter is an in/out parameter, and is intended to be modified.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3469
      Reason: A function should be used in preference to a function-like macro where they are interchangeable.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_4558
      Reason: Boolean types are used on the right side of logical expressions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3223
      Reason: Static variables need to be defined and used within the function, and there is no need for
    global declaration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_1502
      Reason: Configuration variables design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Xcp_3408
      Reason: There is no declaration before the definition.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/

/* =============================================== version information ============================================== */
#define XCP_H_VENDOR_ID (uint16)62
#define XCP_H_MODULE_ID (uint16)212

#define XCP_H_AR_MAJOR_VERSION 4u
#define XCP_H_AR_MINOR_VERSION 9u
#define XCP_H_AR_PATCH_VERSION 0u
#define XCP_H_SW_MAJOR_VERSION 3u
#define XCP_H_SW_MINOR_VERSION 0u
#define XCP_H_SW_PATCH_VERSION 3u

#define XCP_INSTANCE_ID ((uint8)(0u))

/* ===================================================== macros ===================================================== */
#include "Xcp_PBcfg.h"
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
#include XCP_FLS_HEADER /* PRQA S 0832 */ /* VL_Xcp_0832 */
#endif
/*
 * Development errors used by XCP
 */
#define XCP_E_UNINIT        0x02u
#define XCP_E_INVALID_PDUID 0x03u
#define XCP_E_INIT_FAILED   0x04u
#define XCP_E_PARAM_POINTER 0x12u
/*Service IDs*/
#define XCP_SERVICE_ID_INIT                0x00u
#define XCP_SERVICE_ID_GET_VERSION_INFO    0X01u
#define XCP_SERVICE_ID_TXCONFIRMATION      0X02u
#define XCP_SERVICE_ID_RXINDICATION        0x03u
#define XCP_SERVICE_ID_MAINFUNCTION        0x04U
#define XCP_SERVICE_ID_SETTRANSMISSOINMODE 0x03u
#define XCP_SERVICE_ID_TRIGGER_TRANSMIT    0x41u

#define XCP_PROTOCOL_VERSIOM_MAJOR           1u
#define XCP_PROTOCOL_VERSIOM_MINOR           1u
#define XCP_CAN_TRANSPORT_LAYER_VERION_MAJOR 1u
#define XCP_CAN_TRANSPORT_LAYER_VERION_MINOR 1u

#define XCP_AVOID_WARNNING(var) (void)(var)

#define XCP_LOCAL_INLINE static inline

#if !defined(XCP_LOCAL)
#define XCP_LOCAL static /* PRQA S 3414 */ /* VL_Xcp_3414 */
#endif

/* ================================================ type definitions ================================================ */
typedef Xcp_PbConfigType Xcp_ConfigType;
typedef struct Xcp_MeaAddrCheckTag
{
    uint32 MeaAddr;
    uint32 MeaLen;
} Xcp_MeaAddrCheckType;

typedef enum
{
    XCP_TX_OFF, /*disable the transmission capability*/
    XCP_TX_ON   /*enble the transmission capability*/
} Xcp_TransmissionModeType;

#if (XCP_GET_ID == STD_ON)
typedef struct Xcp_IdInfoTag
{
    uint8        type;
    uint8        mode;
    uint32       length;
    const uint8* ptr;
} Xcp_IdInfoType;
#endif

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
typedef struct XcpDtoTag
{
    uint32 LowLayerPduId;
} XcpDtoType;

typedef struct Xcp_EntryTag
{
    uint8       XcpOdtEntryBitOffset; /*now dont support*/
    uint8       XcpOdtEntryNumber;
    uint8       XcpOdtEntryLength;
    Xcp_AGType* XcpOdtEntryAddr;
} Xcp_EntryType;

typedef struct Xcp_OdtTag
{
    uint8          XcpOdtEntryCount;
    Xcp_EntryType* XcpOdtEntry;
} Xcp_OdtType;

#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
typedef struct Xcp_DaqListConfigTag
{
    uint16               XcpFirstPid;
    Xcp_DaqDirectionType XcpDaqListType;
    uint8                XcpMaxOdt;
    uint8                XcpMaxOdtEntries;
    XcpDtoType*          XcpDto;
    Xcp_OdtType*         XcpOdt;
} Xcp_DaqListConfigType;
#endif

typedef struct Xcp_EvChConfigTag
{
    const uint8*         XcpEvChNamePtr;
    uint8                XcpEvChNameLength;
    Xcp_EvChConsistType  XcpEvChConsistency;
    uint8                XcpEvChMaxDaqList;
    uint16               XcpEvChNumber;
    uint8                XcpEvChPriority;
    uint8                XcpEvChTimeCycle;
    XcpTimestampUnitType XcpEvChTimeUnit;
    Xcp_DaqDirectionType XcpEvChType;
    uint16               XcpEvChBuffDpth; /* for consistency */
    Xcp_AGType*          XcpEvChBuffPtr;
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
    const Xcp_DaqNumType* XcpEvChTrigDaqListRef;
#endif
} Xcp_EvChConfigType;
#endif

typedef struct Xcp_PCConfigTag
{
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
    const Xcp_DaqListConfigType* XcpDaqList;
#endif /*XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE*/
    const Xcp_EvChConfigType* XcpEvCh;
#endif
#if (STD_ON == XCP_GET_ID)
    uint8                 Xcp_IdInfoNum;
    const Xcp_IdInfoType* Xcp_IdInfoPtr;
#endif

} Xcp_PCConfigType;

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
typedef struct Xcp_SegmentInfoTag
{
    uint32 size;
    uint32 wpStartAddr;
    uint32 wpEndAddr;
    uint32 rpStartAddr;
    uint32 rpEndAddr;
#if ((STD_ON == XCP_PAG_SUPPORT) && (STD_OFF == XCP_SWITCH_PAG_BY_USER))
    uint32 bufferPtr;
#endif
} Xcp_SegmentInfoType;
#endif /* XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE */

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
typedef struct Xcp_SegmentCfgTag
{
    uint8                      maxSegNum;
    const Xcp_SegmentInfoType* segInfoPtr;
} Xcp_SegmentCfgType;
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/* xcp sector information,used in GET_SECTOR_INFO command */
typedef struct Xcp_SectorInfoTag
{
    const uint8* sectorNamePtr;       /**< SECTOR_NAME */
    uint8        sectorNameLength;    /**< SECTOR_NAME length */
    uint32       sectorclrStartAddr;  /**< Address */
    uint32       sectorclrLength;     /**< Length  */
    uint32       rangeType;           /**< flash range type */
    uint32       progStartAddr;       /**< Program Start Address */
    uint32       progDataSize;        /**< Program Data Size */
    uint8        sectorclrSequenceNo; /**< CLEAR_SEQUENCE_NUMBER */
    uint8        sectorPgmSequenceNo; /**< PROGRAM_SEQUENCE_NUMBER */
    uint8        sectorPgmMethod;     /**< PROGRAM_METHOD */
} Xcp_SectorInfoType;

typedef struct Xcp_FlsFuncPtrTag
{
    Std_ReturnType (
        *flsWrite)(Xcp_FlsAddressType TargetAddress, const uint8* SourceAddressPtr, Xcp_FlsLengthType Length);
    Std_ReturnType (*flsErase)(Xcp_FlsAddressType TargetAddress, Xcp_FlsLengthType Length);
    MemIf_JobResultType (*flsGetJobResult)(void);

} Xcp_FlsFuncPtrType;
#endif /* XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/**********Post-Build configuration parameter declarations*************/
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
extern const Xcp_PduType Xcp_Daq_Pdu[XCP_MAX_DAQ];
#endif

extern const Xcp_PCConfigType Xcp_PConfig;

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
extern const Xcp_SegmentCfgType Xcp_SegmentInfo;
#endif

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
extern const Xcp_SectorInfoType Xcp_SectorInfo[XCP_MAX_SECTOR];

extern const Xcp_FlsFuncPtrType Xcp_FlsFuncPtr;
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Xcp Module Initialization Function.
 * @param[in]   Xcp_ConfigPtr   : Pointer to the Xcp module configuration data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63912
 */
extern void Xcp_Init(const Xcp_ConfigType* Xcp_ConfigPtr);

#if (XCP_VERSION_INFO_API == STD_ON)
/**
 * @brief       The interface provided to the outside for returning version information.
 * @param[out]   versioninfo   : Variable for storing version information.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63917
 */
extern void Xcp_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (XCP_SUPPRESS_TX_SUPPORTED == STD_ON)
/**
 * @brief       The interface for enabling or disabling XCP transmission.
 * @param[in]   Channel   : An XCP protocol stack channel.
 * @param[in]   Mode : The mode to be set for enabling or disabling.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63919
 */
extern void Xcp_SetTransmissionMode(NetworkHandleType Channel, Xcp_TransmissionModeType Mode);
#endif

/**
 * @brief       The event measurement notification interface, which sends the observation data of a DAQ list with each
 * invocation.
 * @param[in]   eventNum   : The event number to start observation.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63988
 */
extern void Xcp_EventIndication(uint16 eventNum);

#endif /* endof XCP_H */
