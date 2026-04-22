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
 **  @file               : FiM_Dem.h
 **  @author             : tao.yu
 **  @date               : 2024/12/04
 **  @vendor             : isoft
 **  @description        : API declaration of FiM
 **
 ***********************************************************************************************************************/
#ifndef FIM_DEM_H_
#define FIM_DEM_H_

/* =================================================== inclusions =================================================== */
#include "Dem.h"
#include "FiM_Cfg.h"
#if (FIM_DEM_EVENT_PARTITION_NUMBER > 1u)
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if (FIM_EVENT_UPDATE_TRIGGERED_BY_DEM == STD_ON)
/**
 * @brief         This service is provided to be called by the Dem in order to inform the Fim about monitor status
 *                changes.
 * @param[in]     EventId: Identification of an Event by assigned event number. The Event Number is configured in the
 *                         DEM. Min.: 1 (0: Indication of no Event or Failure) Max.: Result of configuration of Event
 *                         Numbers in DEM (Max is either 255 or 65535)
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69576
 */
extern void FiM_DemTriggerOnMonitorStatus(Dem_EventIdType EventId);

/**
 * @brief         Triggers on changes of the component failed status.
 * @param[in]     ComponentId: Identification of a DemComponent.
 * @param[in]     ComponentFailedStatus: New FAILED status of the component.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69577
 */
extern void FiM_DemTriggerOnComponentStatus(Dem_ComponentIdType ComponentId, boolean ComponentFailedStatus);

/**
 * @brief         This service re-initializes the FIM.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69578
 */
extern void FiM_DemInit(void);

#if (FIM_DEM_EVENT_PARTITION_NUMBER > 1u)
/**
 * @brief         This service re-initializes the FIM Satellite.
 * @param[in]     ApplicationId: Partition (OsApplication identifier) of the satellite.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69581
 */
extern void FiM_DemInitSatellite(ApplicationType ApplicationId);
#endif

#endif

#ifdef __cplusplus
}
#endif
#endif
