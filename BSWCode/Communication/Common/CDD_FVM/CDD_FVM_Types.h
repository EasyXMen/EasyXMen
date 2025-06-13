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
********************************************************************************
**                                                                            **
**  FILENAME    : CDD_FVM_Types.h                                             **
**                                                                            **
**  Created on  : 2023-10-20                                                  **
**  Author      : Jian.Jiang                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef CDD_FVM_TYPES_H
#define CDD_FVM_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "CDD_FVM_Cfg.h"
#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON)
#include "PduR_CDD_FVM.h"
#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON */
#if (CDD_FVM_NVM_BLOCK_ENABLED == STD_ON) || (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U)
#include "NvM.h"
#endif /* CDD_FVM_NVM_BLOCK_ENABLED == STD_ON) || (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U */
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

typedef enum
{
    CDD_FVM_UNINIT,
    CDD_FVM_INIT
} CDD_FVM_StateType;

typedef enum
{
    CDD_FVM_MODE_SINGLE_COUNTER,
    CDD_FVM_MODE_SINGLE_TIMESTAMP,
    CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER,
    CDD_FVM_MODE_COMPLETE_MULTIPLE_COUNTER,
    CDD_FVM_MODE_UNDEFINED,
} CDD_FVM_ModeType;

typedef struct
{
    uint16 FvmFreshnessId; /* Defines the freshness management ID from the SecOCFreshnessValueId configuration item */
    CDD_FVM_ModeType mode; /* Used to record being in that freshness */
    uint16 freshnessIndex; /* Used to record the position in the relevant configuration */
} CDD_FVM_TxFreshnessConfigType;

typedef enum
{
    CDD_FVM_IFPDU,
    CDD_FVM_TPPDU
} CDD_FVM_PduType;

typedef struct
{
    uint16 FvmFreshnessId; /* Defines the freshness management ID from the SecOCFreshnessValueId configuration item */
    CDD_FVM_ModeType mode; /* Used to record being in that freshness */
    uint16 freshnessIndex; /* Used to record the position in the relevant configuration */
} CDD_FVM_RxFreshnessConfigType;

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/* Define a single counter send configuration */
typedef struct
{
    uint8 FvmFreshnessLength; /* Define the length of freshness, in bits, that needs to be sent */
} CDD_FVM_TxSingleFreshnessCounterConfigType;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/* Define a single counter receive configuration */
typedef struct
{
    uint8 FvmFreshnessLength; /* Define the freshness length of the actual freshness value, in bits */
    uint16 MaxDeltaCounter;   /* Delta counter */
} CDD_FVM_RxSingleFreshnessCounterConfigType;
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON) || (CDD_FVM_MASTER_ECU_ENABLED == STD_ON)
typedef struct
{
    PduIdType FvMIfTxPduHandleId;
    PduLengthType FvMIfTxPduLength;
    PduIdType FvMIfRxPduHandleId;
    PduLengthType FvMIfRxPduLength;
} CDD_FVM_SyncMsgCtrlPudType;
#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON || \
CDD_FVM_MASTER_ECU_ENABLED == STD_ON */

#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON)
typedef struct
{
    uint8 CounterPos; /* Position of the Counter */
    uint8 CounterLen; /* Length of the Counter */
} CDD_FVM_FreshnessCounterConfigType;

typedef struct
{
    CDD_FVM_FreshnessCounterConfigType tirpCnt;
    CDD_FVM_FreshnessCounterConfigType resetCnt;
    CDD_FVM_FreshnessCounterConfigType msgCntUpper;
    CDD_FVM_FreshnessCounterConfigType msgCntLower;
    CDD_FVM_FreshnessCounterConfigType resetFlag;
} CDD_FVM_MultipleFreshnessValueStructureType;

#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
typedef struct
{
    CDD_FVM_PduType pduType;
    PduIdType RxPduHandleId;
    PduLengthType RxPduBufferLength;
} CDD_FVM_SlaveSyncMsgPduType;

typedef struct
{
    uint16 FvMSyncMsgId; /* Message ID associated with this message */
    uint8 FvMTripCntLength;
    uint8 FvMResetCntLength;
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
    P2CONST(CDD_FVM_SyncMsgCtrlPudType, AUTOMATIC, CDD_FVM_APPL_CONST) FvMCtrlPduRef;
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
    P2CONST(CDD_FVM_SlaveSyncMsgPduType, AUTOMATIC, CDD_FVM_APPL_CONST) FvMSyncMsgRef;
#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
    /* Master index associated with this message, Generate 0XFFFF when it doesn't exist, and the index of the
     * corresponding master when it does exist. */
    uint16 FvMCommonMasterIndex;
#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */
    uint16 FvMClearAcceptanceWindow;
#if (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U)
    P2CONST(NvM_BlockIdType, AUTOMATIC, CDD_FVM_APPL_CONST) blkIdRef;
#endif /* CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0U */
} CDD_FVM_SlaveECUSyncConfigType;
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON)
#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
typedef struct
{
    uint16 syncMsgIndex;
    uint8 FvmFreshnessLength; /* Define the freshness length of the actual freshness value, in bits */
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) freshnessConfigRef;
} CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
typedef struct
{
    uint16 syncMsgIndex;
    uint8 FvmFreshnessLength; /* Define the freshness length of the actual freshness value, in bits */
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) freshnessConfigRef;
} CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
#endif /* CDD_FVM_TRUNCATED_MULTIPLE_FRESHNESS_COUNTERS_ENABLED == STD_ON */

#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON)

typedef struct
{
    CDD_FVM_PduType pduType;
    PduIdType TxPduHandleId;
    PduLengthType TxPduBufferLength;
} CDD_FVM_MasterSyncMsgPduType;
typedef struct
{
    uint16 FvMSyncMsgId; /* Message ID associated with this message */
    uint8 FvMTripCntLength;
    uint8 FvMResetCntLength;
    uint16 FvMResetCycle;
    P2CONST(CDD_FVM_MasterSyncMsgPduType, AUTOMATIC, CDD_FVM_APPL_CONST) fvmSyncMsgPduRef;
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
    P2CONST(CDD_FVM_SyncMsgCtrlPudType, AUTOMATIC, CDD_FVM_APPL_CONST) fvmCtrlPduRef;
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
#if (CDD_FVM_NVM_BLOCK_ENABLED == STD_ON)
    NvM_BlockIdType blkId;
#endif /* CDD_FVM_NVM_BLOCK_ENABLED == STD_ON */
} CDD_FVM_MasterConfigType;
#endif /* CDD_FVM_MASTER_ECU_ENABLE == STD_ON */

/* Configuration data structure of FVM module */
typedef struct
{
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
    P2CONST(CDD_FVM_MasterConfigType, AUTOMATIC, CDD_FVM_CONST) FvmMasterEcuConfig;
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST) FvmSlaveEcuConfig;
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

    P2CONST(CDD_FVM_TxFreshnessConfigType, TYPEDEF, CDD_FVM_CONST) FvmTxFreshnessConfig;

    P2CONST(CDD_FVM_RxFreshnessConfigType, TYPEDEF, CDD_FVM_CONST) FvmRxFreshnessConfig;
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
    P2CONST(CDD_FVM_TxSingleFreshnessCounterConfigType, TYPEDEF, CDD_FVM_CONST) FvmTxSingleFreshnessConfig;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */
#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
    P2CONST(CDD_FVM_RxSingleFreshnessCounterConfigType, TYPEDEF, CDD_FVM_CONST) FvmRxSingleFreshnessConfig;
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
    P2CONST(CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_CONST)
    FvmTxMultipleFreshnessTruncateConfig;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_CONST)
    FvmRxMultipleFreshnessTruncateConfig;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

} CDD_FVM_ConfigType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* CDD_FVM_TYPES_H */
