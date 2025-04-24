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
**************************************************************************** **
**                                                                           **
**  FILENAME    : NvM_Inter.h                                                **
**                                                                           **
**  Created on  : 2020/5/9 15:21:52                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : callback function                                          **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef NVM_INTER_H
#define NVM_INTER_H

#include "NvM.h"
#include "Det.h" /* req NVM077 */

static inline void NVM_DET_REPORTRUNTIMEERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(NVM_MODULE_ID, NVM_INSTANCE_ID, ApiId, ErrorId);
}
#if (STD_ON == NVM_DEV_ERROR_DETECT)
static inline void NVM_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(NVM_MODULE_ID, NVM_INSTANCE_ID, ApiId, ErrorId));
}
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
extern VAR(uint16, NVM_VAR) NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE];
#endif /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS && STD_ON == NVM_JOB_PRIORITIZATION */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
extern VAR(NvM_PriTable2QueueType, NVM_VAR_NOINIT) NvM_PriTable2Queue[2][NVM_TOTAL_NUM_DIFF_PRI];
extern VAR(NvM_ImmedQueueType, NVM_VAR_NOINIT) NvM_ImmedQueue[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif /* STD_ON == NVM_JOB_PRIORITIZATION */
extern VAR(NvM_StandQueueType, NVM_VAR_NOINIT) NvM_StandQueue[NVM_SIZE_STANDARD_JOB_QUEUE];
#endif /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS */
extern VAR(NvM_AdminBlockType, NVM_VAR) NvM_AdminBlock[NVM_BLOCK_NUM_ALL];
extern VAR(NvM_ModuleType, NVM_VAR_POWER_ON_INIT) NvM_Module;
extern VAR(NvM_MultiJobType, NVM_VAR_POWER_ON_INIT) NvM_MultiJob;
extern VAR(NvM_AtomJobType, NVM_VAR_POWER_ON_INIT) NvM_AtomJob;
extern VAR(NvM_CurRunningType, NVM_VAR_POWER_ON_INIT) NvM_CurRunning;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
extern VAR(NvM_RoundRobinQueueManageType, NVM_VAR_POWER_ON_INIT) NvM_ImmedQueueManage;
#else  /* STD_ON == NVM_JOB_PRIORITIZATION */
extern VAR(NvM_RoundRobinQueueManageType, NVM_VAR_POWER_ON_INIT) NvM_StandQueueManage;
#endif /* STD_ON == NVM_JOB_PRIORITIZATION */
#endif /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS */
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
extern VAR(uint16, NVM_VAR_POWER_ON_INIT) NvM_PriorityTable[2][NVM_TABLE_SIZE_PRIORITY];
#endif              /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS && STD_ON == NVM_JOB_PRIORITIZATION */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_JobEnqueue(
    NvM_BlockIdType BlockId,
    NvM_ServiceIdType ServiceId,
    P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DestSrcPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
extern FUNC(void, NVM_CODE) NvM_InterSetRamStatus(void);
#endif /* STD_ON == NVM_SET_RAM_BLOCK_STATUS_API */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
extern FUNC(void, NVM_CODE) NvM_InterRestoreBlockDefaults(void);
#endif /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_JobDequeue(void);
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
extern FUNC(void, NVM_CODE) NvM_InterEraseNvBlock(void);
#endif /* STD_ON == NVM_JOB_PRIORITIZATION */
extern FUNC(void, NVM_CODE) NvM_InterInvalidateNvBlock(void);
#endif /* NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS */
extern FUNC(void, NVM_CODE) NvM_InterWriteVerification(void);
extern FUNC(void, NVM_CODE) NvM_InterWriteAll(void);
extern FUNC(void, NVM_CODE) NvM_InterReadAll(void);
extern FUNC(void, NVM_CODE) NvM_InterWriteBlock(void);
extern FUNC(void, NVM_CODE) NvM_InterReadBlock(void);
extern FUNC(void, NVM_CODE) NvM_InterCalcCrc(void);
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
extern FUNC(void, NVM_CODE) NvM_InterValidateAll(void);
#endif              /* NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, NVM_CODE) NvM_SetWordBitState(P2VAR(uint16, AUTOMATIC, NVM_VAR) WordPtr, uint8 SetBit, uint8 OnOff);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
extern FUNC(uint8, NVM_CODE) NvM_GetWordBitState(uint16 WordNum, uint8 GetBit);
extern FUNC(void, NVM_CODE) NvM_InterReadRom(void);

#endif /* End of NVM_INTER_H*/
/*====================[E N D   O F   F I L E]====================*/
