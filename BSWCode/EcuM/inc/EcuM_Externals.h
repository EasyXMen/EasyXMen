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
 **  @file               : EcuM_Externals.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

#ifndef ECUM_EXTERNAL_H
#define ECUM_EXTERNAL_H

/* PRQA S 1753 EOF */ /* VL_EcuM_1753 */

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "EcuM.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief       The ECU State Manager will call the error hook if the error codes
 *              "ECUM_E_RAM_CHECK_FAILED" or "ECUM_E_CONFIGURATION_DATA_INCONSISTENT" occur.
 *              In this situation it is not possible to continue processing and the ECU must
 *              be stopped. The integrator may choose the modality how the ECU is stopped,
 *              i.e. reset, halt, restart, safe state etc.
 * @param[in]   reason : Reason for calling the error hook (e.g., "ECUM_E_RAM_CHECK_FAILED" or
 * "ECUM_E_CONFIGURATION_DATA_INCONSISTENT").
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68944
 */
void EcuM_ErrorHook(uint16 reason);

#if (ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON)
/**
 * @brief       If the configuration parameter EcuMSetProgrammableInterrupts is set to true,
 *              this callout EcuM_AL_SetProgrammableInterrupts is executed and shall set the
 *              interrupts on ECUs with programmable interrupts.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68945
 */
void EcuM_AL_SetProgrammableInterrupts(void);
#endif /*ECUM_SET_PROGRAMMABLE_INTERRUPTS == STD_ON*/

#if (ECUM_DRIVER_INIT_LIST_ZERO == STD_ON)
/**
 * @brief       This callout shall provide driver initialization and other hardware-related
 *              startup activities for loading the post-build configuration data.
 *              Beware: Here only pre-compile and link-time configurable modules may be used.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68946
 */
void EcuM_AL_DriverInitZero(void);
#endif /*ECUM_DRIVER_INIT_LIST_ZERO == STD_ON*/

/**
 * @brief       This callout should evaluate some condition, like port pin or NVRAM value,
 *              to determine which post-build configuration shall be used in the remainder
 *              of the startup process. It shall load this configuration data into a piece
 *              of memory that is accessible by all BSW modules and shall return a pointer
 *              to the EcuM post-build configuration as a base for all BSW module post-build
 *              configurations.
 * @return      Pointer to the EcuM post-build configuration which contains pointers to all
 *              other BSW module post-build configurations.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68948
 */
/* PRQA S 5016 ++ */ /* VL_EcuM_5016 */
const EcuM_ConfigType* EcuM_DeterminePbConfiguration(void);
/* PRQA S 5016 -- */

/**
 * @brief       This callout shall provide driver initialization and other hardware-related
 *              startup activities in case of a power on reset.
 * @param[in]   ConfigPtr : Pointer to the EcuM post-build configuration which contains
 *                          pointers to all other BSW module post-build configurations.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68950
 */
void EcuM_AL_DriverInitOne(void);

#if (ECUM_RESET_LOOP_DETECTION == STD_ON)
/**
 * @brief       If the configuration parameter EcuMResetLoopDetection is set to true,
 *              this callout EcuM_LoopDetection is called on every startup.
 * @return      boolean
 * @retval      FALSE: No reset loop is detected
 * @retval      TRUE: Reset loop is detected
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68952
 */
boolean EcuM_LoopDetection(void);
#endif /*ECUM_RESET_LOOP_DETECTION == STD_ON*/

/**
 * @brief       This call allows the system designer to notify that the GO OFF I state is
 *              about to be entered.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68953
 */
void EcuM_OnGoOffOne(void);

/**
 * @brief       This call allows the system designer to notify that the GO OFF II state is
 *              about to be entered.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68954
 */
/* PRQA S 1753 ++ */ /* VL_EcuM_1753 */
void EcuM_OnGoOffTwo(void);
/* PRQA S 1753 -- */

/**
 * @brief       This callout shall take the code for shutting off the power supply of the ECU.
 *              If the ECU cannot unpower itself, a reset may be an adequate reaction.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68955
 */
void EcuM_AL_SwitchOff(void);

/**
 * @brief       This callout shall take the code for resetting the ECU.
 * @param[in]   reset : Type of reset to be performed.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68956
 */
void EcuM_AL_Reset(EcuM_ResetType reset);

/**
 * @brief       The ECU Manager Module calls EcuM_EnableWakeupSource to allow the system
 *              designer to notify wakeup sources defined in the wakeupSource bitfield that
 *              SLEEP will be entered and to adjust their source accordingly.
 * @param[in]   wakeupSource : Bitfield defining the wakeup sources.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68957
 */
void EcuM_EnableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       Generate code for RAM integrity test.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68958
 */
void EcuM_GenerateRamHash(void);

/**
 * @brief       This callout is invoked periodically in all reduced clock sleep modes.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68960
 */
void EcuM_SleepActivity(void);

/**
 * @brief       This API is called by the ECU Firmware to start the CheckWakeupTimer for the
 *              corresponding wakeupSource. If EcuMCheckWakeupTimeout > 0, the CheckWakeupTimer
 *              for the wakeupSource is started. If EcuMCheckWakeupTimeout <= 0, the API call
 *              is ignored by the EcuM.
 * @param[in]   WakeupSource : For this wakeup source, the corresponding CheckWakeupTimer shall be started.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68961
 */
void EcuM_StartCheckWakeup(EcuM_WakeupSourceType WakeupSource);

/**
 * @brief       This callout is intended to provide a RAM integrity test. The goal of this
 *              test is to ensure that after a long SLEEP duration, RAM contents are still
 *              consistent. The check does not need to be exhaustive since this would consume
 *              quite some processing time during wakeups. A well-designed check will execute
 *              quickly and detect RAM integrity defects with a sufficient probability.
 *              The areas of RAM which will be checked have to be chosen carefully. It depends
 *              on the check algorithm itself and the task structure. Stack contents of the
 *              task executing the RAM check, for example, very likely cannot be checked. It is
 *              good practice to have the hash generation and checking in the same task and that
 *              this task is not preemptible and that there is only little activity between
 *              hash generation and hash check. The RAM check itself is provided by the system
 *              designer. In case of applied multi-core and the existence of Satellite-EcuM(s),
 *              this API will be called by the Master-EcuM only.
 * @return      uint8
 * @retval      0: RAM integrity test failed
 * @retval      non-zero: RAM integrity test passed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68963
 */
uint8 EcuM_CheckRamHash(void);

/**
 * @brief       The ECU Manager Module calls EcuM_DisableWakeupSources to set the wakeup
 *              source(s) defined in the wakeupSource bitfield so that they are not able to
 *              wake the ECU up.
 * @param[in]   wakeupSource : Bitfield defining the wakeup sources to be disabled.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68966
 */
void EcuM_DisableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       This callout shall provide driver initialization and other hardware-related
 *              startup activities in the wakeup case.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68968
 */
void EcuM_AL_DriverRestart(void);

/**
 * @brief       The callout shall start the given wakeup source(s) so that they are ready to
 *              perform wakeup validation.
 * @param[in]   wakeupSource : Bitfield defining the wakeup sources to be started.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68971
 */
void EcuM_StartWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       This callout is called by the EcuM to validate a wakeup source. If a valid
 *              wakeup has been detected, it shall be reported to EcuM via EcuM_ValidateWakeupEvent().
 * @param[in]   wakeupSource : Bitfield defining the wakeup source to be validated.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68976
 */
void EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       The callout shall stop the given wakeup source(s) after unsuccessful wakeup validation.
 * @param[in]   wakeupSource : Bitfield defining the wakeup sources to be stopped.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68981
 */
void EcuM_StopWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       This callout is called by the EcuM to poll a wakeup source. It shall also be
 *              called by the ISR of a wakeup source to set up the PLL and check other
 *              wakeup sources that may be connected to the same interrupt.
 * @param[in]   wakeupSource : Bitfield defining the wakeup source to be polled.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68987
 */
void EcuM_CheckWakeupHook(EcuM_WakeupSourceType wakeupSource);

/**
 * @brief       This function calls Mcu_SetMode as a callback validation.
 * @param[in]   mode : Mcu Mode number as configured in the configuration set
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68992
 */
void EcuM_McuSetMode(Mcu_ModeType mode);

/**
 * @brief       This function retrieves the current timestamp in milliseconds.
 * @return      uint32: Current timestamp in milliseconds.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68999
 */
uint32 EcuM_CurrentTimestampMS(void);

/**
 * @brief       This function calculates the elapsed time in milliseconds based on the provided old timestamp.
 * @param[in]   OldCurMs : Old timestamp value for calculating elapsed time.
 * @return      uint32_t: Elapsed time in milliseconds.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69002
 */
uint32 EcuM_CalculateElapsedMS(uint32 OldCurMs);

#ifdef __cplusplus
}
#endif
#endif /*ECUM_EXTERNAL_H*/
