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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Barrier.c
 **  @author             : i-soft-os
 **  @date               : 2025/04/14
 **  @vendor             : isoft
 **  @description        : Os source file for Barrier API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Barrier.h"
#include "Os_Kernel.h"
#include "Os_Sprot.h"
#include "Os_Arch_Processor.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

#if (CFG_BARRIER_MAX > 0U)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */
extern const Os_BarrierCfgType * const * const Os_BarrierCfg_Inf[OS_AUTOSAR_CORES]; /* PRQA S 1512 */ /* VL_Os_1512 */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (TRUE == BARRIER_DETECT_DEADLOCK_ENABLE)
/**
 * @brief           Detects potential deadlocks when attempting to synchronize at a barrier
 * @param[in]       barrierCfg: Pointer to the barrier configuration
 * @return          StatusType
 * @retval          E_OK: No deadlock detected
 * @retval          E_OS_DEADLOCK: Deadlock detected
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_BarrierDetectDeadLock(const Os_BarrierCfgType * barrierCfg);
#endif

/**
 * @brief           Implements the core synchronization logic at a barrier
 * @param[in]       barrierCfg: Pointer to the barrier configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_BarrierSynchronizeInternal(const Os_BarrierCfgType * barrierCfg);

/* ========================================== external function definitions ========================================= */
/**
 * Init the barrier control block
 */
void Os_BarrierInit(void)/* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint8 i,j;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    if (OS_CORE_ID_MASTER == pScb->SysCore) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        for (i = 0; i < OS_AUTOSAR_CORES; i ++) /* PRQA S 1290 */ /* VL_Os_1290 */
        {
            for (j = 0; j < CFG_BARRIER_MAX; j ++) /* PRQA S 1290 */ /* VL_Os_1290 */
            {
                if ((NULL_PTR != Os_BarrierCfg_Inf[i]) && (NULL_PTR != Os_BarrierCfg_Inf[i][j]))
                {
                    const Os_BarrierCfgType * barrierCfg = Os_BarrierCfg_Inf[i][j];
                    barrierCfg->BarrierCB->ticket = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
                    barrierCfg->BarrierCB->status = BARRIER_RELEASED;
                }
            }
        }
    }
}


/**
 * A circle compares the size of two values
 */
boolean Os_BarrierTicketCompare(Os_BarrierTicketType a, Os_BarrierTicketType b) /* PRQA S 3408, 1505 */ /* VL_Os_3408, VL_Os_1505 */
{
    /* PRQA S 1840, 4404, 4543 ++ */ /* VL_Os_1840, VL_Os_4404, VL_Os_4543 */
    return (((Os_BarrierTicketType)(a - b)) < (BARRRIR_TICKET_THRESHOLD_VALUE));
    /* PRQA S 1840, 4404, 4543 -- */
}

/*
--------------------------------------------
            Core0   Core1     
OsBarrier0    V       V
OsBarrier1    V       V
--------------------------------------------
  Core0       Core1                  
OsBarrier0                
            OsBarrier1
--------------------------------------------

The deadlock detection process for Os_BarrierSynchronize(OsBarrier1) is as follows:

1.  Retrieve the OwnBarriers of Os_OsBarrier_1_Core1_BarrierCfg, 
    which is OsBarrier_Core1_OwnBarriers.
2.  Iterate through OsBarrier_Core1_OwnBarriers 
    (excluding Os_OsBarrier_1_Core1_BarrierCfg itself).
3.  OsBarrier_Core1_OwnBarriers[0] is Os_OsBarrier_0_Core1_BarrierCfg.
4.  Retrieve the Participants of Os_OsBarrier_0_Core1_BarrierCfg, 
    which is OsBarrier_0_Participant_Barriers.
5.  Iterate through OsBarrier_0_Participant_Barriers 
    (excluding Os_OsBarrier_0_Core1_BarrierCfg itself).
6.  OsBarrier_0_Participant_Barriers[0] is Os_OsBarrier_0_Core0_BarrierCfg.
7.  Check the status of Os_OsBarrier_0_Core0_BarrierCfg; if it is WAITING:
8.  Retrieve the OwnBarriers of Os_OsBarrier_0_Core0_BarrierCfg, 
    which is OsBarrier_Core0_OwnBarriers.
9.  Iterate through OsBarrier_Core0_OwnBarriers 
    (excluding Os_OsBarrier_0_Core0_BarrierCfg itself).
10. OsBarrier_Core0_OwnBarriers[1] is Os_OsBarrier_1_Core0_BarrierCfg.
11. Retrieve the Participants of Os_OsBarrier_1_Core0_BarrierCfg, 
    which is OsBarrier_1_Participant_Barriers.
12. Iterate through OsBarrier_1_Participant_Barriers 
    (excluding Os_OsBarrier_1_Core0_BarrierCfg itself).
13. If OsBarrier_1_Participant_Barriers[1] equals Os_OsBarrier_1_Core1_BarrierCfg, 
    a cycle is formed and a deadlock is detected.
 */

 #if (TRUE == BARRIER_DETECT_DEADLOCK_ENABLE)
 /**
 * Detect whether running this barrier causes a deadlock
 */
/* PRQA S 6060, 6030, 6010, 2889 ++ */ /* VL_MTR_Os_STM19, VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_QAC_MultiReturn */
 OS_LOCAL StatusType Os_BarrierDetectDeadLock(const Os_BarrierCfgType * barrierCfg)
/* PRQA S 6060, 6030, 6010, 2889 -- */
{
    uint8 i,j,k,g;
    StatusType status = E_OK;
    const Os_BarrierCfgType * const * Core1_OwnBarriers = barrierCfg->OwnBarriers;
    uint8 Core1_OwnBarriersCount = barrierCfg->OwnBarriersCount;
    for (i = 0; i < Core1_OwnBarriersCount; i ++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        if (Core1_OwnBarriers[i] == barrierCfg)
            continue; /* PRQA S 0770, 2212 */ /* VL_Os_0770, VL_Os_2212 */
        const Os_BarrierCfgType * OsBarrier0_Core1_BarrierCfg = Core1_OwnBarriers[i];
        const Os_BarrierCfgType * const * Barrier_0_Participant = OsBarrier0_Core1_BarrierCfg->Participant;
        uint8 Barrier_0_ParticipantCount = OsBarrier0_Core1_BarrierCfg->ParticipantCount;
        for (j = 0; j < Barrier_0_ParticipantCount; j ++) /* PRQA S 1290 */ /* VL_Os_1290 */
        {
            if (OsBarrier0_Core1_BarrierCfg == Barrier_0_Participant[j])
                continue; /* PRQA S 0770, 2212 */ /* VL_Os_0770, VL_Os_2212 */
            const Os_BarrierCfgType * OsBarrier0_Core0_BarrierCfg = Barrier_0_Participant[j];
            if (OsBarrier0_Core0_BarrierCfg->BarrierCB->status == BARRIER_WATING) /* PRQA S 3442 */ /* VL_Os_3442 */
            {
            	const Os_BarrierCfgType * const * Core0_OwnBarriers = OsBarrier0_Core0_BarrierCfg->OwnBarriers;
                uint8 Core0_OwnBarriersCount = OsBarrier0_Core0_BarrierCfg->OwnBarriersCount;
                for (k = 0; k < Core0_OwnBarriersCount; k ++) /* PRQA S 1290 */ /* VL_Os_1290 */
                {
                    if (Core0_OwnBarriers[k] == OsBarrier0_Core0_BarrierCfg)
                        continue; /* PRQA S 0770, 2212 */ /* VL_Os_0770, VL_Os_2212 */
                    const Os_BarrierCfgType * OsBarrier1_Core0_BarrierCfg = Core0_OwnBarriers[k];
                    const Os_BarrierCfgType * const * Barrier_1_Participant = OsBarrier1_Core0_BarrierCfg->Participant;
                    uint8 Barrier_1_ParticipantCount = OsBarrier1_Core0_BarrierCfg->ParticipantCount;
                    for (g = 0; g < Barrier_1_ParticipantCount; g ++) /* PRQA S 1290 */ /* VL_Os_1290 */
                    {
                        if (OsBarrier1_Core0_BarrierCfg == Barrier_1_Participant[g])
                           continue; /* PRQA S 0770, 2212 */ /* VL_Os_0770, VL_Os_2212 */
                        const Os_BarrierCfgType * OsBarrier1_Core1_BarrierCfg = Barrier_1_Participant[g];
                        if (OsBarrier1_Core1_BarrierCfg == barrierCfg)
                        {
                            status = E_OS_DEADLOCK;
                            return status;
                        }
                    }
                }
            }
        }
    }
    return status;
}
#endif

/**
 * Wait for all Synchronize barriers to arrive
 */
OS_LOCAL void Os_BarrierSynchronizeInternal(const Os_BarrierCfgType * barrierCfg)
{
    boolean waitingStatus;
    uint8 currentTicket;
    const Os_BarrierCfgType * const * participant = barrierCfg->Participant;
    uint8 ParticipantCount = barrierCfg->ParticipantCount;
    barrierCfg->BarrierCB->status = BARRIER_WATING;
    barrierCfg->BarrierCB->ticket ++; /* PRQA S 3442, 3387 */ /* VL_Os_3442, VL_Os_3387 */
    currentTicket = barrierCfg->BarrierCB->ticket;
    
    do
    {
        uint8 i;
        waitingStatus = TRUE;
        for (i = 0; i < ParticipantCount; i ++) /* PRQA S 1290 */ /* VL_Os_1290 */
        {
            uint8 participantTicket = participant[i]->BarrierCB->ticket;
            /* PRQA S 4404, 4115, 4558 ++ */ /* VL_Os_4404, VL_Os_4115, VL_Os_AutosarBool */
            waitingStatus = (Os_BarrierTicketCompare(participantTicket, currentTicket)) && waitingStatus;
            /* PRQA S 4404, 4115, 4558 -- */
            //Inactive cores need to be excluded
        }
    }
    while (waitingStatus == FALSE);
    barrierCfg->BarrierCB->status = BARRIER_RELEASED;
}

/**
 * Synchronize the calling task at the barrier given in "BarrierID".
 */
/* PRQA S 3408, 1512, 3006, 6030 ++ */ /* VL_Os_3408, VL_Os_1512, VL_Os_3006, VL_MTR_Os_STMIF */
StatusType Os_BarrierSynchronize(Os_BarrierIdType BarrierID)
/* PRQA S 3408, 1512, 3006, 6030 -- */
{
    /* PRQA S 2742, 2880, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 3138, 3141 ++ */ /* VL_Os_1006, VL_Os_3138, VL_Os_3141 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006, 3138, 3141 -- */
    /* PRQA S 2742, 2880, 2741 -- */

    StatusType status = E_OK;
#if ((OS_STATUS_EXTENDED == CFG_STATUS) || (TRUE == CFG_SERVICE_PROTECTION_ENABLE) || (TRUE == CFG_RTI_ENABLE))
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521 */
    OSRtiEnterApi(pScb, OSApiId_BarrierSynchronize); /* PRQA S 4442 */ /* VL_Os_4442 */
    /* PRQA S 1317, 3432, 4442, 4521 -- */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_BarrierSynchronize_Start, BarrierID);
    /* PRQA S 3138, 3141 -- */
#endif
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    TaskType runningTaskId  = pScb->SysRunningTaskId;
#endif
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (BarrierID >= CFG_BARRIER_MAX) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        status = E_OS_ID;
    }
    else if ((NULL_PTR == Os_BarrierCfg_Inf[pScb->SysCore]) || 
             (NULL_PTR == Os_BarrierCfg_Inf[pScb->SysCore][BarrierID]))
    {
        status = E_OS_SYS_NO_BARRIER_PARTICIPANT;
    }
    else
#endif
    {
    	const Os_BarrierCfgType * barrierCfg = Os_BarrierCfg_Inf[pScb->SysCore][BarrierID];
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (barrierCfg->OwnerTask != runningTaskId)
        {
            status = E_OS_SYS_NO_BARRIER_PARTICIPANT;
        }
        else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(pScb, OS_CONTEXT_BARRIERSYNCHRONIZE) != TRUE)
        {
            status = E_OS_CALLEVEL;
        }
        else
#endif
#if (TRUE == BARRIER_DETECT_DEADLOCK_ENABLE)
        if (Os_BarrierDetectDeadLock(barrierCfg) != E_OK)
        {
            status = E_OS_DEADLOCK;
        }
        else
#endif
        {
            Os_BarrierSynchronizeInternal(barrierCfg);
        }
    }

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    OSRtiExitApi(pScb, OSApiId_BarrierSynchronize); /* PRQA S 3432 */ /* VL_Os_3432 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_BarrierSynchronize_Return, status);
    OS_HAL_EXIT_KERNEL();
    /* PRQA S 3138, 3141 -- */

    return status;
}

#endif
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
