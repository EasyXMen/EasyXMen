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

/***********************************************************************************************************************
 *
 * iSoft Arti.h
 *
 **********************************************************************************************************************/
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Arti<br>
    RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Arti_0342
    Reason: Using the glue operator ##, To match the diversity of macro functions
    Risk: It may lead to poor readability of the code
    Prevention: Functional reliability guaranteed by design.

    \li VL_Arti_3408
    Reason: The function may have external linkage, but it has not been declared.
    Risk: Function definition is not used, no risk.
    Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Arti_CONF
    Reason: For certain special files that contain little or no code, it is acceptable to reduce or omit extraneous
  comments. Risk: Reduced code readability Prevention: Functional reliability guaranteed by design.

*/

/**
 * \brief Macro based on the number of parameters
 *
 * It is necessary to define macros separately for different numbers of parameters.
 *
 * numEventParameters = number of data parameters + 1
 * - numEventParameters = 1: final macro has 1 parameter (instanceParameter)
 * - numEventParameters = 2: final macro has 2 parameters (instanceParameter, param1)
 * - numEventParameters = 3: final macro has 3 parameters (instanceParameter, param1, param2)
 * - etc.
 */
/* PRQA S 0342 EOF */ /* VL_Arti_0342 */
#ifndef ARTI_H
#define ARTI_H

/***********************************************************************************************************************
 *
 * Arti Trace Classes
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * Trace Variables
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * General Trace Hook Definition
 *
 **********************************************************************************************************************/

/* ARTI TRACE with 1 parameters */
#define ARTI_TRACE_1(_contextName, _className, _instanceName, instanceParameter, _eventName, idOfService) \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName(instanceParameter)

/* ARTI TRACE with 2 parameters */
#define ARTI_TRACE_2(_contextName, _className, _instanceName, instanceParameter, _eventName, idOfService, param1) \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName(                  \
        instanceParameter,                                                                                        \
        param1)

/* ARTI TRACE with 3 parameters */
#define ARTI_TRACE_3(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2)

/* ARTI TRACE with 4 parameters */
#define ARTI_TRACE_4(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3)

/* ARTI TRACE with 5 parameters */
#define ARTI_TRACE_5(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4)

/* ARTI TRACE with 6 parameters */
#define ARTI_TRACE_6(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5)

/* ARTI TRACE with 7 parameters */
#define ARTI_TRACE_7(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6)

/* ARTI TRACE with 8 parameters */
#define ARTI_TRACE_8(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7)

/* ARTI TRACE with 9 parameters */
#define ARTI_TRACE_9(                                                                            \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8)

/* ARTI TRACE with 10 parameters */
#define ARTI_TRACE_10(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9)                                                                                      \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9)

/* ARTI TRACE with 11 parameters */
#define ARTI_TRACE_11(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9,                                                                                      \
    param10)                                                                                     \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9,                                                                                  \
        param10)

/* ARTI TRACE with 12 parameters */
#define ARTI_TRACE_12(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9,                                                                                      \
    param10,                                                                                     \
    param11)                                                                                     \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9,                                                                                  \
        param10,                                                                                 \
        param11)

/* ARTI TRACE with 13 parameters */
#define ARTI_TRACE_13(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9,                                                                                      \
    param10,                                                                                     \
    param11,                                                                                     \
    param12)                                                                                     \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9,                                                                                  \
        param10,                                                                                 \
        param11,                                                                                 \
        param12)

/* ARTI TRACE with 14 parameters */
#define ARTI_TRACE_14(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9,                                                                                      \
    param10,                                                                                     \
    param11,                                                                                     \
    param12,                                                                                     \
    param13)                                                                                     \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9,                                                                                  \
        param10,                                                                                 \
        param11,                                                                                 \
        param12,                                                                                 \
        param13)

/* ARTI TRACE with 15 parameters */
#define ARTI_TRACE_15(                                                                           \
    _contextName,                                                                                \
    _className,                                                                                  \
    _instanceName,                                                                               \
    instanceParameter,                                                                           \
    _eventName,                                                                                  \
    idOfService,                                                                                 \
    param1,                                                                                      \
    param2,                                                                                      \
    param3,                                                                                      \
    param4,                                                                                      \
    param5,                                                                                      \
    param6,                                                                                      \
    param7,                                                                                      \
    param8,                                                                                      \
    param9,                                                                                      \
    param10,                                                                                     \
    param11,                                                                                     \
    param12,                                                                                     \
    param13,                                                                                     \
    param14)                                                                                     \
    ARTI_TRACE##_##_className##_##_instanceName##_##idOfService##_##_eventName##_##_contextName( \
        instanceParameter,                                                                       \
        param1,                                                                                  \
        param2,                                                                                  \
        param3,                                                                                  \
        param4,                                                                                  \
        param5,                                                                                  \
        param6,                                                                                  \
        param7,                                                                                  \
        param8,                                                                                  \
        param9,                                                                                  \
        param10,                                                                                 \
        param11,                                                                                 \
        param12,                                                                                 \
        param13,                                                                                 \
        param14)

/* ARTI TRACE with x parameters */
/* TODO */

#ifndef ARTI_TRACE_N
/**
 * \brief Common ARTI trace macro
 *
 * This macro acts as a top level macro that
 *
 * Macro parameters:
 * - \param[in] _contextName Name of the context, value: NOSUP/SPRVSR/USER
 * - \param[in] _className Used as token, Fixed entry: AR_CP_BSW_API for BSW
 * - \param[in] _instanceName The MIP of BSW
 * - \param[in] instanceParameter Fixed entry: 0 for BSW
 * - \param[in] _eventName Bsw_Start, or Bsw_Return for BSW
 * - \param[in] numEventParameters The number of parameters + 1
 */
#define ARTI_TRACE_N(_contextName, _className, _instanceName, instanceParameter, _eventName, numEventParameters, ...) \
    ARTI_TRACE_##numEventParameters(_contextName, _className, _instanceName, instanceParameter, _eventName, __VA_ARGS__)
#endif /* ARTI_TRACE_N */
/***********************************************************************************************************************
 *
 * End of header file
 *
 **********************************************************************************************************************/

#endif
