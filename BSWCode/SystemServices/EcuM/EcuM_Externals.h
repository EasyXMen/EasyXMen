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
**  FILENAME    : EcuM_Externals.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Callout function declaration                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11.                     **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */
#ifndef ECUM_EXTERNAL_H
#define ECUM_EXTERNAL_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "EcuM.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * The ECU State Manager will call the error hook if the error codes
 * "ECUM_E_RAM_CHECK_FAILED" or "ECUM_E_CONFIGURATION_DATA_INCONSISTENT" occur.
 * In this situation it is not possible to continue processing and the ECU must
 * be stopped. The integrator may choose the modality how the ECU is stopped,
 * i.e. reset, halt, restart, safe state etc.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): reason, Reason for calling the error hook
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_ERRORHOOK_CODE) EcuM_ErrorHook(uint16 reason);

#if (ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON)
/**
 * If the configuration parameter EcuMSetProgrammableInterrupts is set to true,
 * this callout EcuM_AL_SetProgrammableInterrupts is executed and shall set the
 * interrupts on ECUs with programmable interrupts.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_SETPROGRAMMABLEINTERRUPTS_CODE)
EcuM_AL_SetProgrammableInterrupts(void);
#endif /*ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON*/

#if (ECUM_DRIVER_INIT_LIST_ZERO == STD_ON)
/**
 * This callout shall provide driver initialization and other hardware-related
 * startup activities for loading the post-build configuration data. Beware:
 * Here only pre-compile and link-time configurable modules may be used.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_DRIVERINITZERO_CODE)
EcuM_AL_DriverInitZero(void);
#endif /*ECUM_DRIVER_INIT_LIST_ZERO == STD_ON*/

/**
 * This callout should evaluate some condition, like port pin or NVRAM value,
 * to determine which post-build configuration shall be used in the remainder
 * of the startup process. It shall load this configuration data into a piece
 * of memory that is accessible by all BSW modules and shall return a pointer
 * to the EcuM post-build configuration as a base for all BSW module post-build
 * configurations.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: Pointer to the EcuM post-build configuration which contains
 *               pointers to all other BSW module post-build configurations.
 */
FUNC_P2CONST(EcuM_ConfigType, ECUM_CONFIG_DATA, ECUM_DETERMINEPBCONFIGURATION_CODE)
EcuM_DeterminePbConfiguration(void);

/**
 * This callout shall provide driver initialization and other hardware-related
 * startup activities in case of a power on reset.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr, pointer to the EcuM post-build configuration which
 *                             contains pointers to all other BSW module post-build
 *                             configurations.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_DRIVERINITONE_CODE)
EcuM_AL_DriverInitOne(void);

#if (ECUM_RESET_LOOP_DETECTION == STD_ON)
/**
 * If the configuration parameter EcuMResetLoopDetection is set to true,
 * this callout EcuM_LoopDetection is called on every startup.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: FALSE: no reset loop is detected
 *               TRUE: reset loop is detected
 */
FUNC(boolean, ECUM_LOOPDETECTION_CODE)
EcuM_LoopDetection(void);
#endif /*ECUM_RESET_LOOP_DETECTION == STD_ON*/

/**
 * This call allows the system designer to notify that the GO OFF I state is
 * about to be entered.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_ONGOOFFONE_CODE)
EcuM_OnGoOffOne(void);

/**
 * This call allows the system designer to notify that the GO OFF II state is
 * about to be entered.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_ONGOOFFTWO_CODE)
EcuM_OnGoOffTwo(void);

/**
 * This callout shall take the code for shutting off the power supply of the ECU.
 * If the ECU cannot unpower itself, a reset may be an adequate reaction.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_SWITCHOFF_CODE)
EcuM_AL_SwitchOff(void);

/**
 * This callout shall take the code for resetting the ECU.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): reset, type of reset to be performed.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_RESET_CODE)
EcuM_AL_Reset(EcuM_ResetType reset);

/**
 * The ECU Manager Module calls EcuM_EnableWakeupSource to allow the system
 * designer to notify wakeup sources defined in the wakeupSource bitfield that
 * SLEEP will be entered and to adjust their source accordingly.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_ENABLEWAKEUPSOURCES_CODE)
EcuM_EnableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * Generate code for RAM integrity test.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_GENERATERAMHASH_CODE)
EcuM_GenerateRamHash(void);

/**
 * This callout is invoked periodically in all reduced clock sleep modes.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_SLEEPACTIVITY_CODE)
EcuM_SleepActivity(void);

/**
 * This API is called by the ECU Firmware to start the CheckWakeupTimer for the
 * corresponding wakeupSource. If EcuMCheckWakeupTimeout > 0 the CheckWakeupTimer
 * for the wakeupSource is started. If EcuMCheckWakeupTimeout ' 0 the API call
 * is ignored by the EcuM.
 * Service ID[hex]: 0x28
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource, For this wakeup source the corresponding
 *                                CheckWakeupTimer shall be started.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_STARTCHECKWAKEUP_CODE)
EcuM_StartCheckWakeup(EcuM_WakeupSourceType wakeupSource);

/**
 * This API is called by any SW Module whose wakeup source is checked
 * asynchronously (e.g. asynchronous Can Trcv Driver) and the Check of the Wakeup
 * returns a negative Result (no Wakeup by this Source). The API cancels the
 * CheckWakeupTimer for the wakeupSource. If the correponding CheckWakeupTimer is
 * canceled the check of this wakeup source is finished.
 * Service ID[hex]: 0x29
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource, For this wakeup source the corresponding
 *                                CheckWakeupTimer shall be canceled.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_ENDCHECKWAKEUP_CODE)
EcuM_EndCheckWakeup(EcuM_WakeupSourceType WakeupSource);

/**
 * This callout is intended to provide a RAM integrity test. The goal of this
 * test is to ensure that after a long SLEEP duration, RAM contents is still
 * consistent. The check does not need to be exhaustive since this would consume
 * quite some processing time during wakeups. A well designed check will execute
 * quickly and detect RAM integrity defects with a sufficient probability.
 * The areas of RAM which will be checked have to be chosen carefully. It depends
 * on the check algorithm itself and the task structure. Stack contents of the
 * task executing the RAM check e.g. very likely cannot be checked. It is good
 * practice to have the hash generation and checking in the same task and that this
 * task is not preemptible and that there is only little activity between hash
 * generation and hash check. The RAM check itself is provided by the system
 * designer. In case of applied multi core and existence of Satellite-EcuM(s):
 * this API will be called by the Master-EcuM only.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: 0: RAM integrity test failed else: RAM integrity test passed
 */
FUNC(uint8, ECUM_CHECKRAMHASH_CODE)
EcuM_CheckRamHash(void);

/**
 * The ECU Manager Module calls EcuM_DisableWakeupSources to set the wakeup
 * source(s) defined in the wakeupSource bitfield so that they are not able to
 * wake the ECU up.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_DISABLEWAKEUPSOURCES_CODE)
EcuM_DisableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This callout shall provide driver initialization and other hardware-related
 * startup activities in the wakeup case.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr, pointer to the EcuM post-build configuration which
 *                             contains pointers to all other BSW module post-build
 *                             configurations.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_AL_DRIVERRESTART_CODE)
EcuM_AL_DriverRestart(void);

/**
 * The callout shall start the given wakeup source(s) so that they are ready to
 * perform wakeup validation.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_STARTWAKEUPSOURCES_CODE)
EcuM_StartWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This callout is called by the EcuM to validate a wakeup source. If a valid
 * wakeup has been detected, it shall be reported to EcuM via EcuM_ValidateWakeupEvent().
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_CHECKVALIDATION_CODE)
EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource);

/**
 * The callout shall stop the given wakeup source(s) after unsuccessful wakeup
 * validation.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_STOPWAKEUPSOURCES_CODE)
EcuM_StopWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This function calls Mcu_SetMode as a callback
 * validation.
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): wakeupSource
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_MCUSETMODE_CODE)
EcuM_McuSetMode(Mcu_ModeType mode);

/******************************************************************************/
/*
 * Description         This function retrieves the current timestamp in milliseconds.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint32: Current timestamp in milliseconds.
 */
/******************************************************************************/
FUNC(uint32, ECUM_CURRENTTIMESTAMPMS_CODE) EcuM_CurrentTimestampMS(void);

/******************************************************************************/
/*
 * Description         This function calculates the elapsed time in milliseconds based on the provided old timestamp.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      OldCurMs: Old timestamp value for calculating elapsed time.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint32: Elapsed time in milliseconds.
 */
/******************************************************************************/
FUNC(uint32, ECUM_CALCULATEELAPSEDMS_CODE) EcuM_CalculateElapsedMS(uint32 OldCurMs);

#endif /*ECUM_EXTERNAL_H*/
