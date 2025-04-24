/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Xcp.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of XCP                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef XCP_H
#define XCP_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define XCP_H_VENDOR_ID        (uint16)62
#define XCP_H_MODULE_ID        (uint16)212

#define XCP_H_AR_MAJOR_VERSION 4u
#define XCP_H_AR_MINOR_VERSION 5u
#define XCP_H_AR_PATCH_VERSION 0u
#define XCP_H_SW_MAJOR_VERSION 2u
#define XCP_H_SW_MINOR_VERSION 0u
#define XCP_H_SW_PATCH_VERSION 10u

#define XCP_INSTANCE_ID        ((uint8)(0u))
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Cfg.h"
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
#include XCP_FLS_HEADER
#endif /*XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE*/
/*
 * Development errors used by XCP
 */
#define XCP_E_INV_POINTER     0x01u
#define XCP_E_NOT_INITIALIZED 0x02u
#define XCP_E_INVALID_PDUID   0x03u
#define XCP_E_INIT_FAILED     0x04u
#define XCP_E_PARAM_POINTER   0x12u
/*Service IDs*/
#define XCP_SERVICE_ID_INIT                0x00u
#define XCP_SERVICE_ID_GET_VERSION_INFO    0X01u
#define XCP_SERVICE_ID_TXCONFIRMATION      0X02u
#define XCP_SERVICE_ID_RXINDICATION        0x03u
#define XCP_SERVICE_ID_MAINFUNCTION        0x04U
#define XCP_SERVICE_ID_SETTRANSMISSOINMODE 0x03u
#define XCP_SERVICE_ID_TRIGGER_TRANSMIT    0x41u
/*=======[M A C R O S]========================================================*/
#define XCP_PROTOCOL_VERSIOM_MAJOR           1
#define XCP_PROTOCOL_VERSIOM_MINOR           1
#define XCP_CAN_TRANSPORT_LAYER_VERION_MAJOR 1
#define XCP_CAN_TRANSPORT_LAYER_VERION_MINOR 1 /* PRQA S 0791 */ /* MISRA Rule 5.4 */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    uint16 XcpLocalRxPduId;
} Xcp_RxPduType;

typedef struct
{
    uint16 XcpLocalTxPduId;
    uint32 LowLayerTxPduId;
} Xcp_TxPduType;

typedef struct
{
    P2CONST(Xcp_RxPduType, TYPEDEF, XCP_CONST_PBCFG) XcpRxPdu;
    P2CONST(Xcp_TxPduType, TYPEDEF, XCP_CONST_PBCFG) XcpTxPdu;
} Xcp_PduType;

typedef struct
{
    uint8 numOfRxPdu;
    uint8 numberOfTxPdu;
    P2CONST(Xcp_PduType, TYPEDEF, XCP_CONST_PBCFG) XcpPduRef;
} Xcp_ConfigType;
typedef enum
{
    XCP_TX_OFF, /*disable the transmission capability*/
    XCP_TX_ON   /*enble the transmission capability*/
} Xcp_TransmissionModeType;

#if (XCP_GET_ID == STD_ON)
typedef struct
{
    uint8 type;
    uint8 mode;
    uint32 length;
    P2CONST(uint8, TYPEDEF, XCP_CONST_PBCFG) ptr;
} Xcp_IdInfoType;
#endif

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
typedef struct
{
    uint8 XcpDtoPid;
    P2CONST(Xcp_PduType, TYPEDEF, XCP_CONST_PBCFG) XcpDto2PduMapping;
} XcpDtoType;

typedef struct
{
    uint8 XcpOdtEntryBitOffset; /*now dont support*/
    uint8 XcpOdtEntryNumber;
    uint8 XcpOdtEntryLength;
    P2VAR(Xcp_AGType, TYPEDEF, XCP_VAR) XcpOdtEntryAddr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} Xcp_EntryType;

typedef struct
{
    uint8 XcpOdtEntryCount;
    uint8 XcpOdtNumber;
    P2VAR(Xcp_EntryType, TYPEDEF, XCP_VAR) XcpOdtEntry; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} Xcp_OdtType;

typedef struct
{
    uint16 XcpDaqListNumber;
    Xcp_DaqDirectionType XcpDaqListType;
#if (XCP_DAQ_STATIC == XCP_DAQ_CONFIG_TYPE)
    uint8 XcpMaxOdt;
    uint8 XcpMaxOdtEntries;
#else
    uint8 XcpDaqSize;
#endif
    P2CONST(XcpDtoType, TYPEDEF, XCP_CONST) XcpDto;
    P2VAR(Xcp_OdtType, TYPEDEF, XCP_CONST) XcpOdt; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} Xcp_DaqListConfigType;

typedef struct
{
    P2CONST(uint8, TYPEDEF, XCP_CONST)
    XcpEvChNamePtr;
    uint8 XcpEvChNameLength;
    Xcp_EvChConsistType XcpEvChConsistency;
    uint8 XcpEvChMaxDaqList;
    uint16 XcpEvChNumber;
    uint8 XcpEvChPriority;
    uint8 XcpEvChTimeCycle;
    XcpTimestampUnitType XcpEvChTimeUnit;
    Xcp_DaqDirectionType XcpEvChType;
    uint32 XcpEvChBuffDpth;                                 /* for consistency */
    P2VAR(Xcp_AGType, TYPEDEF, XCP_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    XcpEvChBuffPtr;
    P2CONST(Xcp_DaqNumType, TYPEDEF, XCP_VAR)
    XcpEvChTrigDaqListRef;
} Xcp_EvChConfigType;
#endif

typedef struct
{
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
    P2CONST(Xcp_DaqListConfigType, TYPEDEF, XCP_CONST) XcpDaqList;
    P2CONST(Xcp_EvChConfigType, TYPEDEF, XCP_CONST) XcpEvCh;
#endif
#if (STD_ON == XCP_GET_ID)
    uint8 Xcp_IdInfoNum;
    P2CONST(Xcp_IdInfoType, TYPEDEF, XCP_CONST)
    Xcp_IdInfoPtr;
#endif
} Xcp_PCConfigType;

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
typedef struct
{
    uint32 size;
    uint32 ramStart;
    uint32 ramEnd;
    uint32 romStart;
    uint32 romEnd;
    uint32 bufferPtr;
} Xcp_PageInfoType;
#endif /* XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE */

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
typedef struct
{
    uint8 maxSegNum;
    P2CONST(Xcp_PageInfoType, AUTOMATIC, XCP_CONST) pageInfoPtr;
} Xcp_SegmentInfoType;
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/* xcp sector information,used in GET_SECTOR_INFO command */
typedef struct
{
    P2CONST(uint8, TYPEDEF, XCP_CONST) sectorNamePtr; /**< SECTOR_NAME */
    uint8 sectorNameLength;                           /**< SECTOR_NAME length */
    uint8 sectorNo;                                   /**< SECTOR_NUMBER */
    uint32 sectorclrStartAddr;                        /**< Address */
    uint32 sectorclrLength;                           /**< Length  */
    uint32 rangeType;                                 /**< flash range type */
    uint32 progStartAddr;                             /**< Program Start Address */
    uint32 progDataSize;                              /**< Program Data Size */
    uint8 sectorclrSequenceNo;                        /**< CLEAR_SEQUENCE_NUMBER */
    uint8 sectorPgmSequenceNo;                        /**< PROGRAM_SEQUENCE_NUMBER */
    uint8 sectorPgmMethod;                            /**< PROGRAM_METHOD */
} Xcp_SectorInfoType;

typedef struct
{
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2FUNC(Std_ReturnType, BSWM_APPL_CODE, flsWrite)
    (Xcp_FlsAddressType TargetAddress, const uint8* SourceAddressPtr, Xcp_FlsLengthType Length);
    P2FUNC(Std_ReturnType, BSWM_APPL_CODE, flsErase)(Xcp_FlsAddressType TargetAddress, Xcp_FlsLengthType Length);
    P2FUNC(MemIf_JobResultType, BSWM_APPL_CODE, flsGetJobResult)(void);
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
} Xcp_FlsFuncPtrType;
#endif /* XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE */

/**********Post-Build configuration parameter declarations*************/
extern const Xcp_ConfigType Xcp_PBConfig;

extern const Xcp_PduType Xcp_Daq_Pdu[XCP_MAX_DAQ];

/*=======[E X T E R N A L   D A T A]==========================================*/
extern CONST(Xcp_PCConfigType, XCP_CONST) Xcp_PConfig;

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
extern CONST(uint32, XCP_CONST) Xcp_MeaStartAddr;
extern CONST(uint32, XCP_CONST) Xcp_MeaLen;
extern CONST(Xcp_SegmentInfoType, XCP_CONST) Xcp_SegmentInfo;
#endif

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
extern CONST(Xcp_SectorInfoType, XCP_CONST) Xcp_SectorInfo[XCP_MAX_SECTOR];

extern CONST(Xcp_FlsFuncPtrType, XCP_CONST_PBCFG) Xcp_FlsFuncPtr;
#endif
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/******************************************************************************/
/*
 * @brief               <Xcp Module Initialization Function>
 *
 * <This service initializes interfaces and variables of the AUTOSAR XCP layer> .
 * Service ID   :       <XCP_INIT_ID (0x00)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <Xcp_ConfigPtr (IN)>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
extern FUNC(void, XCP_CODE) Xcp_Init(CONSTP2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_DATA) Xcp_ConfigPtr);

#if (XCP_VERSION_INFO_API == STD_ON)
/******************************************************************************/
/*
 * @brief               <Get version information.>
 *
 * <Returns the version information of this module.> .
 * Service ID   :       <(0x01)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <None>
 * @param[out]          <versioninfo>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
extern FUNC(void, XCP_CODE) Xcp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) versioninfo);
#endif

#if (XCP_SUPPRESS_TX_SUPPORTED == STD_ON)
/******************************************************************************/
/*
 * @brief               <Control TX capabilities.>
 *
 * <This API is used to turn on and off of the TX capabilities of used communication
 * bus channel in XCP module..> .
 * Service ID   :       <(0x05)>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <Channel>/<Mode>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
extern FUNC(void, XCP_CODE) Xcp_SetTransmissionMode(NetworkHandleType Channel, Xcp_TransmissionModeType Mode);
#endif

extern FUNC(void, XCP_CODE) Xcp_EventIndication(uint16 eventNum);
#endif /* endof XCP_H */
