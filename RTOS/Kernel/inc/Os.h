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
 **  @file               : Os.h
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for external API declarations
 **
 ***********************************************************************************************************************/
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dcm<br>
    RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Os_PlatformNoDef
      Reason: Determined by the chip platform, macro functions may be empty.
      Risk: Logic short circuit may cause functions to not be executed
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_PlatformDef
      Reason: Defined by the chip platform, macro functions have different usage scenarios for ported code.
      Risk: No risk.
      Prevention: Ensuring the reliability of functionality through design.

    \li VL_Os_3006
      Reason: To optimize code execution efficiency.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_ConstToIntegral
      Reason: Used when assigning special values, such as null pointers.
      Risk: No risk.
      Prevention: Ensure the normal operation of the project through unit testing

    \li VL_Os_3432
      Reason: Simple macro parameter expressions are only used for conditional judgments and do not require parentheses.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3450
      Reason: The order of function calls for internal links is determined, and there is no need to declare it again.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_1840
      Reason: Configure to ensure the security of implicit conversion of constants to unsigned types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0582
      Reason: The configuration ensures the security of the padding bit size
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_1290
      Reason: Configuration, parameter checking, and design ensure the security of data type conversion
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_VolatileAccess
      Reason: Access to volatile objects involving chips, guaranteed by hardware.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2481
      Reason: Chip platform porting code ensures the correctness of macro function loop conditions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0310
      Reason: Design and configure to ensure the security of pointer type conversion for different objects
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2743
      Reason: To ensure that the macro expansion is an independent statement.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0306
      Reason: Design and configure to ensure the security of pointer type conversion for different objects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_4544
      Reason: Code design uses fixed offset to obtain information, and computational security is ensured by design.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3333
      Reason: There are multiple ending branches in the specified case conditions that cannot be further divided.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2834
      Reason: The dividend is derived from the configuration and cannot be set to 0.
      Risk: No risk.
      Prevention: Ensure the normal operation of the project through unit testing

    \li VL_Os_0311
      Reason: Obtain pointers of Const type in the configuration for use.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3678
      Reason: Pointer application control block, designed to ensure safe use.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3471
      Reason: In order to improve code readability and call efficiency.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3472
      Reason: In order to improve code readability and call efficiency.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2870
      Reason: Special scenario usage, such as CPU running Idletask.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_4397
      Reason: Ensure the security of data conversion through parameter inspection and design.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2487
      Reason: Used in special scenarios to control CPU execution status and wait for other asynchronous awakenings.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_4340
      Reason: Conditional judgment of unsigned types used as boolean values.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_Os_2911
      Reason: Confirmed: enclosed in unsigned arithmetic operations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_HookDef
      Reason: Effective after Hook configuration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_1505
      Reason: Configure tools to be used under certain configuration conditions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2880
      Reason: Defensive code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2742
      Reason: The macro definition determines the condition, which is used in special single character scenarios.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2953
      Reason: Design, tool configuration, and parameter checking to ensure non negative operands.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2853
      Reason: Tool configuration and parameter checking ensure that the size of the operand does not exceed the limit.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_Os_2842
      Reason: Pointer dereference, passed in by the user.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3138
      Reason: Non empty statements in multi-core or other configuration scenarios.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3672
      Reason: Ioc callback function usage, callback function is determined by the user.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0489
      Reason: Pointer operation is used for efficient copying of data.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3412
      Reason: Ioc multi-core synchronization scenario usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_1338
      Reason: Obtain some bits and use them internally in the function to avoid the influence of CoreID.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0488
      Reason: Quick access to memory.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2740
      Reason: Special scenario use, such as defensive code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0490
      Reason: Os module design ensures correctness.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0318
      Reason: Arch call, a redundant operation performed to ensure security
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3223
      Reason: Bitmap initialization usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3455
      Reason: Cross core scenarios, passing callback functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2990
      Reason: Spinlock algorithm to obtain the last lock user.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_1259
      Reason: Stack check is used to mark memory.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_4461
      Reason: Configure and design to ensure that data conversion does not result in data loss.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0303
      Reason: Cross core scenarios, using volatile to ensure data consistency.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3442
      Reason: Ensure the correctness of strong conversion through parameter checking and configuration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0770
      Reason: 'Continue' statement is used to implement special features of the OS.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0771
      Reason: Spinlock is used in special scenarios where the design ensures the correctness of the code execution flow.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3679
      Reason: Ensure the correctness of access through design.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3406
      Reason: Related to hardware, cloning variables are used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3410
      Reason: Reference to arch, related to hardware platform implementation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_2755
      Reason: Remove the termination task from the bitmap in the app, and use it in special scenarios.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_3209
      Reason: The return value is ignored because it is not needed or cannot be processed.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0789
      Reason: User defined characters, with configuration tools ensuring the accuracy of character matching.
      Risk: No risk.
      Prevention: Use different test data to cover.

    \li VL_Os_0779
      Reason: Configure tools to ensure the correctness of character matching.
      Risk: No risk.
      Prevention: Use different test data to cover.

    \li VL_Os_0631
      Reason: Arch specific data type, only used for memory protection function address tagging.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Os_0750
      Reason: Ioc module specific data type, using union to identify block relationships.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4342
      Reason: The Os module enforces the conversion of unsigned expressions to enumeration types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3464
      Reason: The macro is only used in well-defined OS internal contexts where 
              the argument evaluation is guaranteed to occur at most once.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3469
      Reason: To enhance code readability and generality, use simple macro functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1258
      Reason: OS uses special meaning integer constants to convert to different integer types1.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3449
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3451
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3458
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0784
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3409
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3684
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3218
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3345
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2872
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1255
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3141
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3219
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2053
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2741
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0791
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1514
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0286
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0884
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0341
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3203
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2983
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3408
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0404
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0883
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1502
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0305
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0428
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3206
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3008
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2961
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4434
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0597
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1272
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1340
      Reason: Store the address of the parameter in a constant pointer to avoid unexpected modifications.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1252
      Reason: Suffixed integer constant implicitly converted to different integer type, Special code design of the Ioc.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4464
      Reason: Unsigned data type strong conversion, Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
      
      \li VL_Os_4600
      Reason: A macro of this name is defined in the specified library header file. Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0793
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4152
      Reason: The standard interface specified by AUTOSAR.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1891
      Reason: The correctness of implicit conversion of unsigned type parameters to a wider unsigned type is ensured by the OS module.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2812
      Reason: The OS logic code ensures that pointer dereferencing will not result in a null pointer situation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0499
      Reason: The OS service protection module ensures the correctness of shift operator operations.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0326
      Reason: The cast of a null pointer to an integer type is a requirement of the code design.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0674
      Reason: For the universality of parameter transmission, initializer for pointer is of incompatible type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0691
      Reason: Unused para, default initialization is applied to the remainder of the array elements.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0704
      Reason: Unused para, default initialization is applied to the remainder of the members.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2991
      Reason: Due to configuration or other reasons, the value of this if controlling expression might be always false.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2992
      Reason: Due to configuration or other reasons, the value of this if controlling expression might be always true.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2995
      Reason: Due to configuration or other reasons, the result of this logical operation might be always true.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.   

      \li VL_Os_2996
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
      
      \li VL_Os_2997
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2998
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2982
      Reason: Control variables are used to reflect the current status, the value of this object might be never used before being modified.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.  

      \li VL_Os_4521
      Reason: The OS module ensures that using the enum type as the left-hand operand of the arithmetic operator is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4522
      Reason: The OS module ensures that using the enum type as the left-hand operand of the arithmetic operator is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4523
      Reason: The OS module ensures that using the enum type as the left-hand operand of the arithmetic operator is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1317
      Reason: The OS module ensures that using the constant type as the enum type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1441
      Reason: The OS module ensures that using the unsigned type as the enum type in function calls is valid.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4442
      Reason: The OS module ensures that using the unsigned type as enum type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4322
      Reason: The OS module ensures that using the enum type as different enum type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3326
      Reason: The OS module ensures that the result of an assignment is correctly used in a logical operation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3200
      Reason: The return value is not used because it is not needed in the current scenario.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3334
      Reason: This declaration of para hides a more global declaration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3305
      Reason: For the sake of code generality, pointer type conversion is used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4424
      Reason: An expression of essentially enum is being converted to unsigned type, on assignment.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0432
      Reason: For the sake of code generality, pointer type conversion is used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0342
      Reason: Using the glue operator ##.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1271
      Reason: For the sake of code intuitiveness, using a non-int expression to define an enum constant.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0602
      Reason: The identifier is reserved for use by the library.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3625
      Reason: Type char has been used in the declaration of an object or a function.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1821
      Reason: The OS module ensures that using the signed int type as the unsigned long type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4532
      Reason: The OS module ensures that using the signed int type as the left-hand operand of this bitwise operator (|) is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4542
      Reason: The OS module ensures that using the signed int type as the right-hand operand of this bitwise operator (|) is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4543
      Reason: The OS module ensures that using the signed int type as the left-hand operand of this bitwise operator (<<) is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1861
      Reason: The OS module ensures that the result generated by the operands of this bitwise operator is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3762
      Reason: The OS module ensures that using the int type as the unsigned long type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4527
      Reason: The OS module ensures that applying the increment/decrement operator to an enum type is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1277
      Reason: The OS module ensures that Hex constant is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2986
      Reason: The OS module ensures that the right-hand operand is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1880
      Reason: The OS module ensures that comparison between different types is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1841
      Reason: The OS module ensures that implicitly converted operation is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2813
      Reason: The OS logic code ensures that pointer dereferencing will not result in a null pointer situation.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2985
      Reason: The OS module ensures that the left-hand operand is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3332
      Reason: The OS module ensures that undefined identifiers can be used correctly.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1297
      Reason: The OS module ensures that security of data type conversion.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1881
      Reason: The OS module ensures that comparison between different types is correct.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3122
      Reason: The OS module guarantees the correctness of directly used string literals.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2016
      Reason: The OS module guarantees the correctness of macro parameters that are not enclosed in parentheses within the macro body.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3433
      Reason: The OS module guarantees the correctness of implicit conversions between pointers and integers.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3344
      Reason: The OS module guarantees the correctness of applying left- or right-shift operators to signed negative values.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3440
      Reason: The OS module guarantees the correctness of pointer casts.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3387
      Reason: The OS module guarantees the correctness of function declared with external linkage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0314
      Reason: The OS module guarantees the correctness of header file inclusion.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4432
      Reason: The OS module guarantees the correctness of macro parameter operand.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0722
      Reason: The OS module guarantees the correctness of include file nested to a depth.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0723
      Reason: The OS module guarantees the correctness of include file name matches another file on the include search path.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0724
      Reason: The OS module guarantees the correctness of enumeration constant value.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1434
      Reason: The OS module guarantees the correctness of identifier declared but never referenced.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1461
      Reason: The OS module guarantees the correctness that the function has no return statement.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3443
      Reason: The OS module guarantees the correctness of implicit conversion between pointer and integer types.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2987
      Reason: The OS module guarantees the correctness of function declared with external linkage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1039
      Reason: The OS module guarantees the correctness of treating a one-element array as a potentially flexible array member.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1431
      Reason: The OS module guarantees the correctness of passing an enum object as an argument to a function whose parameter is of non-enum type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1006
      Reason: Code may expand under certain architectures.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2004
      Reason: The OS module guarantees the correctness of if-else-if.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3415
      Reason: The OS module guarantees the correctness of the right-hand operand in '&&' or '||'.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1753
      Reason:The function with external linkage is declared but not defined within this project.
      Risk:No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1501
      Reason:The OS module guarantees the correctness that the function is declared but is not used within this project.
      Risk:No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1512
      Reason:The OS module guarantees the correctness that the value with external linkage has separate declarations in multiple translation units.
      Risk:No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1513
      Reason:The OS module guarantees the correctness that identifier with external linkage has separate non-defining declarations in more than one location.
      Risk:No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1712
      Reason:The OS module guarantees the correctness that external identifiers have some the same on characters.
      Risk:No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1506
      Reason: The OS module guarantees the correctness that ObjectType is declared as a typedef and is used elsewhere for a different kind of declaration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_5209
      Reason: The OS module ensures the correctness that unsigned long is used.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1520
      Reason: The OS module ensures the correctness that the functions are indirectly recursive.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1752
      Reason: Objects with external links have been declared, but not defined in this project, for
      declaring identifiers in linked files.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1500
      Reason: The OS module guarantees the correctness that the value is used in this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3132
      Reason: The OS module guarantees the correctness that magic number is used to define the size of an array.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4404
      Reason: The OS module guarantees the correctness of defining the boolean type as unsigned char.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3207
      Reason: The switch statement allows only the default actions to run, will not affect the running results.
      Risk: No risk
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0632
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_4115
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2212
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0613
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3335
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1021
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0605
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_2200
      Reason: Indentation of this line is to the left of the current block indentation level.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_3446
      Reason: The 2nd or 3rd operand of this conditional operator is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1894
      Reason: one is a composite expression of a narrower type than the other.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_1533
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_0388
      Reason: Special code design of the Os.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

      \li VL_Os_AutosarBool
      Reason: The Autosar specification defines the boolean type as an unsigned char.
      Risk: The behavior of Boolean logic does not align with expectations.
      Prevention: Ensure normal functionality through unit testing.

      \li VL_MTR_Os_STPTH
      Reason: There is no separation of functionality into sub functions, as there are higher requirements for
      minimizing the stack and applying it to the runtime of the code.
      Risk: Understandability and testability may become overly complex.
      Prevention: Design and code review+clear structure and annotated code.

      \li VL_MTR_Os_STCYC
      Reason: No separation of functionality into sub-functions because there is a higher requirement to minimize the
      stack and runtime usage applied to the code.
      Risk: Understandability and testability can become too complex.
      Prevention: Design and code review + clearly structured and commented code.

      \li VL_MTR_Os_STCAL
      Reason: The software structure is defined by the AUTOSAR standard. The standard compliance vote is above the
      measurement threshold. In addition, a typical way to reduce STCAL is to deepen function nesting, which increases
      the use of the call stack and runtime.
      Risk: Because of fanning out to many functions, understandability and testability may become overly complex.
      Prevention: Design and code review + clearly structured and commented code.

      \li VL_MTR_Os_STMIF
      Reason: Functions handle specific tasks in the overall component behavior; Tasks have different scenarios to
      cover, depending on local conditions - this leads to deep nesting of control structures. Since there is more
      common code than different code, a higher nesting level is acceptable to keep the code.
      Risk: The code is hard to maintain.
      Prevention: Design and code review + clearly structured and commented code.

      \li VL_MTR_Os_STLIN
      Reason: Some functions have overly complex logic, and implementing them separately will result in reduced
      performance.
      Risk: Code is difficult to maintain.
      Prevention: Design and code review+clear structure and annotated code.

      \li VL_MTR_Os_STST3
      Reason: Some functions have overly complex logic, and implementing them separately will result in reduced
      performance.
      Risk: Code is difficult to maintain.
      Prevention: Design and code review+clear structure and annotated code.

      \li VL_MTR_Os_STECT
      Reason: Some functions have overly complex logic, and implementing them separately will result in reduced
      performance.
      Risk: The code is difficult to maintain and the data flow is complex.
      Prevention: Design and code review+clear structure and annotated code.

      \li VL_MTR_Os_STVAR
      Reason: The total number of variables is related to user configuration.
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

      \li VL_MTR_Os_STTPP
      Reason: The total number of variables is related to user configuration.
      Risk: Reduced code readability
      Prevention: Functional reliability guaranteed by design.

      \li VL_MTR_Os_CONF
      Reason: For certain special files that contain little or no code, it is acceptable to reduce or omit extraneous comments.
      Risk: Reduced code readability
      Prevention: Functional reliability guaranteed by design.

      \li VL_MTR_Os_STCDN
      Reason: Comment to Code Ratio measures the proportion of comments in the source code. 
      Risk: Reduced code readability
      Prevention: Functional reliability guaranteed by design.

      \li VL_MTR_Os_STSCT
      Reason: The OS extensively uses static variables to ensure the reliability of data.
      Risk: Reduced code readability
      Prevention: Functional reliability guaranteed by design.

      \li VL_MTR_Os_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
      but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/

#ifndef OS_H
#define OS_H

/* =================================================== inclusions =================================================== */
#include "Os_Err.h"
#include "Os_Types.h"

/* ===================================================== macros ===================================================== */

/* ========================================= external function declarations ========================================= */

#if (CFG_TASK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The task<TaskID> is transferred from the  suspended state into the  ready  state.
 *                  The operating system ensures that the task code is being executed from the first statement.
 * @param[in]       TaskID: Task reference.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Task <TaskID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The state of running OS-Application is not equal to APPLICATION_ACCESSIBLE;
 *                               Or the OS-Application of <TaskID> cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ActivateTask(TaskType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           Asynchronous version of the ActivateTask() function. Intended to be used for cross core task activation.
 *                  Possible errors are not returned to the caller, but may be reported via error hooks.
 * @param[in]       TaskID: Task reference.
 * @synchronous     FALSE
 * @reentrant       TRUE
 * @trace           CPD-57615
 */
extern void ActivateTaskAsyn(TaskType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service causes the termination of the calling task. The calling task is transferred
 *                  from the running state into the suspended state.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType TerminateTask(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service causes the termination of the calling task. After termination of the
 *                  calling task a succeeding task <TaskID> is activated. Using this service,
 *                  it ensures that the succeeding task starts to run at the earliest after the calling task has been terminated.
 * @param[in]       TaskID: Reference to the sequential succeeding task to be activated.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Task <TaskID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The state of running OS-Application is not equal to APPLICATION_ACCESSIBLE;
 *                               Or the OS-Application of <TaskID> cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_RESOURCE: Task still occupies resources.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType ChainTask(TaskType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           If a higher-priority task is ready, the internal resource of the task is released,
 *                  the current task is put into the ready state, its context is saved and the higher-priority task is executed.
 *                  Otherwise the calling task is continued.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_RESOURCE: Task still occupies resources.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType Schedule(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           GetTaskID  returns the information about the TaskID of
 *                  the task which is currently  running.
 * @param[inout]    TaskID: Reference to the task which is currently running.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is NULL_PTR.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is invalid (address is not writable by this OS-Application).
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetTaskID(TaskRefType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           Returns the state of a task (running, ready, waiting, suspended) at the time of calling GetTaskState.
 * @param[in]       TaskID: Task reference.
 * @param[inout]    State: Reference to the state of the task <TaskID>.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of State is invalid or NULL_PTR.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of <TaskID> cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid TaskID.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is invalid (address is not writable by this OS-Application).
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetTaskState(TaskType TaskID, TaskStateRefType State); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief           This call serves to enter critical sections in the code that are assigned to the resource referenced by <ResID>.
 *                  A critical section shall always be left using ReleaseResource.
 * @param[in]       ResID: Reference to resource.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Resource <ResID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: Attempt to get a resource which is already occupied by any task or ISR,
 *                  or the statically assigned priority of the calling task or interrupt routine is higher than the calculated ceiling priority.
 * @retval          E_OS_CORE : The core id of ResID is not equal to running system core.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetResource(ResourceType ResID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           ReleaseResource is the counterpart of GetResource and serves to leave critical sections
 *                  in the code that are assigned to the resource referenced by <ResID>.
 * @param[in]       ResID: Reference to resource.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_NOFUNC: Attempt to release a resource which is not occupied by any task or ISR, or another resource shall be released before.
 * @retval          E_OS_ID: Resource <ResID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: Attempt to release a resource which has a lower ceiling priority than
 *                  the statically assigned priority of the calling task or interrupt routine.
 * @retval          E_OS_CORE : The core id of ResID is not equal to running system core.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType ReleaseResource(ResourceType ResID); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (CFG_EXTENDED_TASK_MAX > 0U)
/**
 * @brief           The service may be called from an interrupt service routine and from the task level, but not from hook routines.
 * @param[in]       TaskID: Reference to the task for which one or several events are to beset.
 * @param[in]       Mask: Mask of the events to be set.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Task <TaskID> is invalid.
 * @retval          E_OS_ACCESS: Referenced task is no extended task.
 * @retval          E_OS_STATE: Events cannot be set as the referenced task is in the suspended state.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_CORE : The remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType SetEvent(TaskType TaskID, EventMaskType Mask); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The service may be called from an interrupt service routine and from the task level, but not from hook routines.
 * @param[in]       TaskID: Reference to the task for which one or several events are to beset.
 * @param[in]       Mask: Mask of the events to be set.
 * @synchronous     FALSE
 * @reentrant       TRUE
 * @trace           CPD-57289
 */
extern void SetEventAsyn(TaskType TaskID, EventMaskType Mask); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The events of the extended task calling ClearEvent are cleared according to the event mask <Mask>.
 * @param[in]       Mask: Mask of the events to be cleared.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ACCESS: Call not from extended task.
 * @retval          E_OS_CALLEVEL: Call at interrupt level.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ClearEvent(EventMaskType Mask); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service returns the current state of all event bits of the task<TaskID>, not the events that the task is waiting for.
 * @param[in]       TaskID: Task whose event mask is to be returned.
 * @param[out]      Event: Reference to the memory of the return data.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Task <TaskID> is invalid.
 * @retval          E_OS_ACCESS:  Referenced task <TaskID> is not an extended task.
 * @retval          E_OS_STATE:  Referenced task <TaskID> is in the suspended state.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS : The Event address is not writable.
 * @retval          E_OS_CORE: The remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_ILLEGAL_ADDRESS: The Event is NULL.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType GetEvent(TaskType TaskID, EventMaskRefType Event); /* PRQA S 1512, 3209 */ /* VL_Os_1512, VL_Os_3209 */

/**
 * @brief           The state of the calling task is set to waiting, unless at least one of the events specified in <Mask> has already been set.
 * @param[in]       Mask: Mask of the events waited for.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ACCESS: Calling task is not an extended task.
 * @retval          E_OS_RESOURCE: Calling task occupies resources.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_SPINLOCK: There are any outstanding spinlocks.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType WaitEvent(EventMaskType Mask); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The state of the calling task is set to waiting, must all the events specified in <Mask> has already been set.
 * @param[in]       Mask: Mask of the events waited for.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ACCESS: Calling task is not an extended task.
 * @retval          E_OS_RESOURCE: Calling task occupies resources.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_SPINLOCK: There are any outstanding spinlocks.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType WaitAllEvents(EventMaskType Mask); /* PRQA S 1512, 3209 */ /* VL_Os_1512, VL_Os_3209 */
#endif

#if (CFG_ALARM_MAX > 0U)
/**
 * @brief           The system service GetAlarm returns the relative value in ticks before the alarm <AlarmID> expires.
 * @param[in]       AlarmID: Reference to alarm.
 * @param[out]      Tick: Tick: Relative value in ticks before the alarm <AlarmID> expires.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Tick is invalid.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of AlarmID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid AlarmID.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Tick is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The system service GetAlarmBase reads the alarm base characteristics.
 *                  The return value <Info> is a structure in which the information of data type AlarmBaseType is stored.
 * @param[in]       AlarmID: Reference to alarm.
 * @param[out]      Info: Reference to structure with constants of the alarm base.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Info is invalid.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of AlarmID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid AlarmID.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Info is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info); /* PRQA S 1512 */ /* VL_Os_1512 */

/******************************************************************************/
/*
 * Brief               <The system service cancels the alarm <AlarmID>.>
 * ServiceId           <0xf4>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/******************************************************************************/
/**
 * @brief           The system service cancels the alarm <AlarmID>.
 * @param[in]       AlarmID: Reference to alarm.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of AlarmID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid AlarmID.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType CancelAlarm(AlarmType AlarmID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The system service occupies the alarm <AlarmID> element. After <increment> ticks have elapsed,
 *                  the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended tasks)
 *                  is set or the alarm-callback routine is called.
 * @param[in]       AlarmID: AlarmID: Reference to the alarm element.
 * @param[in]       increment: Relative value in ticks
 * @param[in]       cycle: Cycle value in case of cyclic alarm. In case of single alarms, cycle shall be zero.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of AlarmID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid AlarmID.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The system service occupies the alarm <AlarmID> element. When <start> ticks are reached,
 *                  the task assigned to the alarm<AlarmID> is activated or the assigned event (only for extended tasks)
 *                  is set or the alarm-callback routine is called.
 * @param[in]       AlarmID: AlarmID: Reference to the alarm element.
 * @param[in]       increment: Absolute value in ticks
 * @param[in]       cycle: Cycle value in case of cyclic alarm. In case of single alarms, cycle shall be zero.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of AlarmID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid AlarmID.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (CFG_COUNTER_MAX > 0U)
/**
 * @brief           This service increases count for a software counter.
 * @param[in]       CounterID: The Counter to be incremented.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : The CounterID was not valid or counter is implemented in hardware and cannot be incremented by software.
 * @retval          E_OS_CORE: The core id of CounterID is not equal to running system core.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of CounterID cannot be accessed.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57533
 */
extern StatusType IncrementCounter(CounterType CounterID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service reads the current count value of a counter (returning either the hardware timer ticks
 *                  if counter is driven by hardware or the software ticks when user drives counter).
 * @param[in]       CounterID: The Counter which tick value should be read.
 * @param[out]      Value: Contains the current tick value of the counter.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value is invalid (address is not writable by this OS-Application).
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of CounterID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: Invalid CounterID.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57539
 */
extern StatusType GetCounterValue(CounterType CounterID, TickRefType Value); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service gets the number of ticks between the current tick value and a previously read tick value.
 * @param[in]       CounterID: The Counter which tick value should be read.
 * @param[inout]    Value: the previously read tick value of the counter out: the current tick value of the counter.
 * @param[out]      ElapsedValue The difference to the previous read value.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The given Value or ElapsedValue was NULL.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value or ElapsedValue is invalid (address is not writable by this OS-Application).
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of CounterID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: The CounterID was not valid.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57544
 */
extern StatusType GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

/**
 * @brief           This service returns the current application mode. It may be used to write mode dependent code.
 * @return          AppModeType
 * @retval          AppModeType: the active application mode.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern AppModeType GetActiveApplicationMode(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service disables all interrupts for which the hardware supports disabling.
 *                  The state before is saved for the EnableAllInterrupts call.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void DisableAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service restores the state saved by DisableAllInterrupts.
 *                  The state before is saved for the EnableAllInterrupts call.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void EnableAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service restores the recognition status of all interrupts saved by the SuspendAllInterrupts service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ResumeAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service saves the re cognition status of all
 *                  interrupts and disables all interrupts for which the hardware supports.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void SuspendAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service restores the recognition status of interrupts saved by the SuspendOSInterrupts service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ResumeOSInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service saves the recognition status of interrupts of category 2
 *                  and disables the recognition of these interrupts.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void SuspendOSInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           Enables the interrupt source by modifying the interrupt controller registers.
 *                  Additionally it may clear the interrupt pending flag.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @param[in]       ClearPending: Defines whether the pending flag shall be cleared (TRUE) or not(FALSE).
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_NOFUNC: Breaks nested calls to source control API errors.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function(EXTENDED status).
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57612
 */
extern StatusType EnableInterruptSource(ISRType ISRID, boolean ClearPending); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           Disables the interrupt source by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_NOFUNC: Breaks nested calls to source control API errors.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function(EXTENDED status).
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57613
 */
extern StatusType DisableInterruptSource(ISRType ISRID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           Clears the interrupt pending flag by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function(EXTENDED status).
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57614
 */
extern StatusType ClearPendingInterrupt(ISRType ISRID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The user can call this system service to start the operating system in a specific mode,
 *                  see chapter 5, Application modes.
 * @param[in]       Mode: application mode.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void StartOS(AppModeType Mode); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The user can call this system service to abort the overall system (e.g. emergency off).
 *                  The operating system also calls this function internally,
 *                  if it has reached an undefined internal state and is no longer ready to run.
 * @param[in]       Error: error occurred.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ShutdownOS(StatusType Error); /* PRQA S 1512 */ /* VL_Os_1512 */

#if (CFG_SCHEDTBL_MAX > 0U)
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           This service starts an explicitly synchronized schedule table synchronously.
 * @param[in]       ScheduleTableID: Schedule table to be started.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_ID : Invalid ScheduleTableID; Or the synchronization strategy of ScheduleTableID is not EXPLICIT.
 * @retval          E_OS_STATE: Schedule table was already started.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57507
 */
extern StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service provides the schedule table with a synchronization count and start synchronization.
 * @param[in]       ScheduleTableID: Schedule table to be synchronized.
 * @param[in]       value: The current value of the synchronization counter.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_ID : The ScheduleTableID was not valid or schedule table cannot be synchronized
 *                  (OsScheduleTblSyncStrategy not set or OsScheduleTblSyncStrategy = IMPLICIT).
 * @retval          E_OS_VALUE (only in EXETENDED status): The <Value> is out of range.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57512
 */
extern StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service stops synchronization of a schedule table.
 * @param[in]       ScheduleTableID: Schedule table for which status is requested.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : Invalid ScheduleTableID.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 *                  (OsScheduleTblSyncStrategy not set or OsScheduleTblSyncStrategy = IMPLICIT).
 * @retval          E_OS_STATE: The state of ScheduleTableID is STOPPED/NEXT/WAITTING.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57518
 */
extern StatusType SetScheduleTableAsync(ScheduleTableType ScheduleTableID); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

/**
 * @brief           This service cancels the processing of a schedule table immediately at any point while the schedule table is running.
 * @param[in]       ScheduleTableID: Schedule table to be stopped.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : Invalid ScheduleTableID.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 *                  (OsScheduleTblSyncStrategy not set or OsScheduleTblSyncStrategy = IMPLICIT).
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57491
 */
extern StatusType StopScheduleTable(ScheduleTableType ScheduleTableID); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service starts the processing of a schedule table at an absolute value "Start" on the underlying counter.
 * @param[in]       ScheduleTableID: Schedule table to be stopped.
 * @param[in]       Start: Absolute counter tick value at which the schedule table is started.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : Invalid ScheduleTableID.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_CORE: the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57485
 */
extern StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service starts the processing of a schedule table at "Offset" relative to the "Now" value on the underlying counter.
 * @param[in]       ScheduleTableID: Schedule table to be started.
 * @param[in]       Offset: Number of ticks on the counter before the schedule table processing is started.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : Invalid ScheduleTableID.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_CORE: the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57477
 */
extern StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service switches the processing from one schedule table to another scheduletable.
 * @param[in]       ScheduleTableID_From: Currently processed schedule table.
 * @param[in]       ScheduleTableID_To: Schedule table that provides its series of expiry points.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : ScheduleTableID_From or ScheduleTableID_To not valid;
 *                  Or <ScheduleTableID_To> is driven by different counter than schedule table <ScheduleTableID_From>;
 *                  Or the synchronization strategy of <ScheduleTableID_To> is not equal to <ScheduleTableID_From>.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID_From or ScheduleTableID_To cannot be accessed.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 * @retval          E_OS_NOFUNC: ScheduleTableID_From not started.
 * @retval          E_OS_STATE: ScheduleTableID_To is started or next; Or ScheduleTableID_From is repeating.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57496
 */
extern StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service queries the state of a schedule table (also with respect to synchronization).
 * @param[in]       ScheduleTableID: Schedule table for which status is requested.
 * @param[out]      ScheduleStatus: Reference to ScheduleTableStatusType.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of ScheduleStatus is invalid.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID : Invalid ScheduleTableID.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of ScheduleStatus is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57525
 */
extern StatusType GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

/**
 * @brief           This service returns the identifier of the currently executing ISR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57443
 */
extern ISRType GetISRID(void); /* PRQA S 1512 */ /* VL_Os_1512 */

#if (CFG_OSAPPLICATION_MAX > 0)
/**
 * @brief           This service determines the OS-Application (a unique identifier has to be allocated
                    to each application) where the caller originally belongs to (was configured to).
 * @return          ApplicationType
 * @retval          ApplicationType: <identifier of running OS-Application> or INVALID_OSAPPLICATION.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57435
 */
extern ApplicationType GetApplicationID(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service determines the OS-Application where the caller of the service is currently executing.
                    Note that if the caller is not within a CallTrustedFunction() call the value is equal to the result of GetApplicationID().
 * @return          ApplicationType
 * @retval          ApplicationType: <identifier of running OS-Application> or INVALID_OSAPPLICATION.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57439
 */
extern ApplicationType GetCurrentApplicationID(void); /* PRQA S 1753, 1501, 1512 */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */

/**
 * @brief           This service sets the own state of an OS-Application from APPLICATION_RESTARTING to APPLICATION_ACCESSIBLE.
 * @return          StatusType
 * @retval          E_OK: No errors.
 * @retval          E_OS_STATE: The OS-Application of the caller is in the wrong state.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57561
 */
extern StatusType AllowAccess(void); /* PRQA S 1753, 1501, 1512 */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */

/**
 * @brief           This service returns the current state of an OS-Application.
 * @param[in]       Application: The OS-Application from which the state is requested.
 * @param[out]      Value: The current state of the application.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: <Application> is not valid.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value is invalid (address is not writable by this OS-Application).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57565
 */
/* PRQA S 1753, 1501, 1512 ++ */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */
extern StatusType GetApplicationState(ApplicationType Application, ApplicationStateRefType Value);
/* PRQA S 1753, 1501, 1512 -- */

/**
 * @brief           This service determines if the OS-Applications, given by ApplID,
 *                  is allowed to use the IDs of a Task, Resource, Counter, Alarm or Schedule Table in API calls.
 * @param[in]       ApplID: OS-Application identifier.
 * @param[in]       ObjectType: Type of the following parameter.
 * @param[in]       ObjectID: The object to be examined.
 * @return          ObjectAccessType
 * @retval          ACCESS: the ApplID has access to the object.
 * @retval          NO_ACCESS: the ApplID is invalid; ObjectType is invalid; the ApplID does not have access to the object;
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57468
 */
/* PRQA S 1753, 1501, 1506, 1512 ++ */ /* VL_Os_1753, VL_Os_1501, VL_Os_1506, VL_Os_1512 */
extern ObjectAccessType CheckObjectAccess(
    ApplicationType ApplID,
    ObjectTypeType ObjectType,
    AppObjectId ObjectID);
/* PRQA S 1753, 1501, 1506, 1512 -- */

/**
 * @brief           This service determines to which OS-Application a given Task, ISR, Counter, Alarm or Schedule Table belongs.
 * @param[in]       ObjectType: Type of the following parameter.
 * @param[in]       ObjectID: The object to be examined.
 * @return          ApplicationType
 * @retval          INVALID_OSAPPLICATION: the ObjectID does not exist; ObjectType is invalid;
 *                  the object type is OBJECT_RESOURCE and the object is RES_SCHEDULER.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57473
 */
extern ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID); /* PRQA S 1512, 1506 */ /* VL_Os_1512, VL_Os_1506 */

/**
 * @brief           This service terminates the OS-Application to which the calling Task/Category 2ISR/application specific error hook belongs.
 * @param[in]       Application: The identifier of the OS-Application to be terminated.
 *                  If the caller belongs to <Application> the call results in a self termination.
 * @param[in]       RestartOption: Either RESTART for doing a restart of the OS-Application or NO_RESTART if OS-Application shall not be restarted.
 * @return          StatusType
 * @retval          E_OK: No errors.
 * @retval          E_OS_ID: <Application> was not valid (only in EXTENDED status).
 * @retval          E_OS_VALUE: <RestartOption> was neither RESTART nor NO_RESTART (only in EXTENDED status).
 * @retval          E_OS_ACCESS: The caller does not have the right to terminate<Application> (only in EXTENDED status).
 * @retval          E_OS_STATE: The state of <Application> does not allow terminating <Application>.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57551
 */
extern StatusType TerminateApplication(ApplicationType Application, RestartType RestartOption); /* PRQA S 1753, 1501, 1512 */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/**
 * @brief           This service checks if a memory region is write/read/execute accessible and
 *                  also returns information if the memory region is part of the stack space.
 * @param[in]       ISRID: ISR reference.
 * @param[in]       Address: Start of memory area.
 * @param[in]       Size: Size of memory area.
 * @return          Access: Value which contains the access rights to the memory area.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57458
 */
extern AccessType CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service checks if a memory region is write/read/execute accessible and
 *                  also returns information if the memory region is part of the stack space.
 * @param[in]       TaskID: Task reference.
 * @param[in]       Address: Start of memory area.
 * @param[in]       Size: Size of memory area.
 * @return          Access: Value which contains the access rights to the memory area.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57463
 */
extern AccessType CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/**
 * @brief           A (trusted or non-trusted) OS-Application uses this service to call a trusted function.
 * @param[in]       FunctionIndex: Index of the function to be called.
 *                  If the caller belongs to <Application> the call results in a self termination.
 * @param[in]       FunctionParams: Pointer to the parameters for the function - specified by the FunctionIndex - to be called.
 *                  If no parameters are provided, a NULL pointer has to be passed.
 * @return          StatusType
 * @retval          E_OK: No errors.
 * @retval          E_OS_SERVICEID: No function defined for this index.
 * @retval          E_OS_ACCESS: The invoked trusted function does not belong to the same core as the caller.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS : The FunctionParams address is not writable.
 * @retval          E_OS_ILLEGAL_ADDRESS: FunctionParams is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57447
 */
extern StatusType CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief           GetSpinlock tries to occupy a spin-lock variable. If the function returns,
 *                  either the lock is successfully taken or an error has occurred.
 *                  The spinlock mechanism is an active polling mechanism. The function does not cause a de-scheduling.
 * @param[in]       SpinlockId: The value refers to the spinlock instance that shall be locked.
 *                  If the caller belongs to <Application> the call results in a self termination.
 * @return          StatusType
 * @retval          E_OK: In standard and extended status : No Error.
 * @retval          E_OS_ID: In extended status: The SpinlockId is invalid.
 * @retval          E_OS_INTERFERENCE_DEADLOCK: In extended status: A task tries to occupy the spinlock
 *                  while the lock is already occupied by a TASK on the same core. This would cause a deadlock.
 * @retval          E_OS_NESTING_DEADLOCK - In extended status: A TASK tries to occupy the spinlock
 *                  while a TASK on the same core is holding a different spinlock in a way that may cause a deadlock.
 * @retval          E_OS_ACCESS: In extended status: The spinlock cannot be accessed.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57578
 */
extern StatusType GetSpinlock(SpinlockIdType SpinlockId); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           ReleaseSpinlock releases a spinlock variable that was occupied before.
 *                  Before terminating a TASK all spinlock variables that have been occupied with GetSpinlock() shall be released.
 *                  Before calling WaitEVENT all Spinlocks shall be released.
 * @param[in]       SpinlockId: The value refers to the spinlock instance that shall be unlocked.
 * @return          StatusType
 * @retval          E_OK: In standard and extended status : No Error.
 * @retval          E_OS_ID: In extended status: The SpinlockId is invalid.
 * @retval          E_OS_STATE - In extended status: The Spinlock is not occupied by the TASK.
 * @retval          E_OS_ACCESS: In extended status: The spinlock cannot be accessed.
 * @retval          E_OS_NOFUNC: In extended status: Attempt to release a spinlock while another spinlock has to be released before.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57587
 */
extern StatusType ReleaseSpinlock(SpinlockIdType SpinlockId); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           TryToGetSpinlock has the same functionality as GetSpinlock with the difference
 *                  that if the spinlock is already occupied by a TASK on a different core the function sets
 *                  the OUT parameter "Success" and returns with E_OK.
 * @param[in]       SpinlockId: The value refers to the spinlock instance that shall be locked.
 * @param[out]      Success: Returns if the lock has been occupied or not.
 * @return          StatusType
 * @retval          E_OK: In standard and extended status : No Error.
 * @retval          E_OS_ID: In extended status: The SpinlockId is invalid.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Success is NULL_PTR.
 * @retval          E_OS_INTERFERENCE_DEADLOCK: In extended status: A TASK tries to occupy the spinlock
 *                  while the lock is already occupied by a TASK on the same core. This would cause a deadlock.
 * @retval          E_OS_NESTING_DEADLOCK: In extended status: A TASK tries to occupy a spinlock
 *                  while holding a different spinlock in a way that may cause a deadlock.
 * @retval          E_OS_ACCESS: In extended status: The spinlock cannot be accessed.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Success is not writable.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57594
 */
extern StatusType TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType *Success); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

/**
 * @brief           It is not supported to call this function after StartOS(). The function starts the core specified by the parameter CoreID.
 *                  The OUT parameter allows the caller to check whether the operation was successful or not.
 *                  If a core is started by means of this function StartOS shall be called on the core.
 * @param[in]       CoreID: Core identifier.
 * @param[out]      Status: Output status.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57573
 */
extern void StartCore(CoreIdType CoreID, StatusType *Status); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The function starts the core specified by the parameter CoreID. It is allowed to call this function after StartOS().
                    The OUT parameter allows the caller to check whether the operation was successful or not.
                    It is not allowed to call StartOS on cores activated by StartNonAutosarCore. Otherwise the behaviour is unspecified.
 * @param[in]       CoreID: Core identifier.
 * @param[out]      Status: Output status.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern void StartNonAutosarCore(CoreIdType CoreID, StatusType *Status); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This API allows the caller to select the idle mode action which is performed during idle time of the OS (e.g. if no Task/ISR is active).
 *                  It can be used to implement energy savings. The real idle modes are hardware dependent and not standardized.
 *                  The default idle mode on each core is IDLE_NO_HALT.
 * @param[in]       CoreID: selects the core which idle mode is set.
 * @param[in]       IdleMode: the mode which shall be performed during idle.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Invalid core and/or invalid idleMode.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57608
 */
extern StatusType ControlIdle(CoreIdType CoreID, IdleModeType IdleMode); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The function returns the number of cores activated by the StartCore function. This function might be a macro.
 * @return          uint32
 * @retval          Number of cores activated by the StartCore function (see below)
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57569
 */
extern uint32 GetNumberOfActivatedCores(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           The function returns a unique core identifier.
 * @return          CoreIdType: The return value is the unique ID of the core.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57571
 */
extern CoreIdType GetCoreID(void); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           After this service the OS on all cores is shut down. Allowed at TASK level and ISR level and also internally by the OS.
 *                  The function will never return. The function will force other cores into a shutdown.
 * @param[in]       Error: needs to be a valid error code supported by the OS.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57604
 */
extern void ShutdownAllCores(StatusType Error); /* PRQA S 1512 */ /* VL_Os_1512 */

#if (CFG_PERIPHERAL_MAX > 0U)

#define Os_ReadPeripheral8        ReadPeripheral8
#define Os_ReadPeripheral16       ReadPeripheral16
#define Os_ReadPeripheral32       ReadPeripheral32
#define Os_WritePeripheral8       WritePeripheral8
#define Os_WritePeripheral16      WritePeripheral16
#define Os_WritePeripheral32      WritePeripheral32
#define Os_ModifyPeripheral8      ModifyPeripheral8
#define Os_ModifyPeripheral16     ModifyPeripheral16
#define Os_ModifyPeripheral32     ModifyPeripheral32

/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      ReadValue: content of the given memory location (<Address>).
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL(EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ReadPeripheral8(AreaIdType Area, const uint8 *Address, uint8 *ReadValue);/* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      ReadValue: content of the given memory location (<Address>).
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL(EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
/* PRQA S 4152, 1512 ++ */ /* VL_Os_4152, VL_Os_1512 */
extern StatusType ReadPeripheral16(AreaIdType Area, const uint16 *Address, uint16 *ReadValue);
/* PRQA S 4152, 1512 -- */

/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      ReadValue: content of the given memory location (<Address>).
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL(EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ReadPeripheral32(AreaIdType Area, const uint32 *Address, uint32 *ReadValue); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      WriteValue: value to be written at the memory address.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType WritePeripheral8(AreaIdType Area, uint8 *Address, uint8 WriteValue); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      WriteValue: value to be written at the memory address.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
/* PRQA S 4152, 1512 ++ */ /* VL_Os_4152, VL_Os_1512 */
extern StatusType WritePeripheral16(AreaIdType Area, uint16 *Address, uint16 WriteValue);
/* PRQA S 4152, 1512 -- */

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[out]      WriteValue: value to be written at the memory address.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType WritePeripheral32(AreaIdType Area, uint32 *Address, uint32 WriteValue); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service modifies a given memory location (<memory address>)
 *                  with the formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[in]       Clearmask: memory address will be modified by a bit-AND.
 * @param[in]       Setmask: memory address will be modified by a bit-OR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ModifyPeripheral8(AreaIdType Area, uint8 *Address, uint8 Clearmask, uint8 Setmask); /* PRQA S 1512 */ /* VL_Os_1512 */

/**
 * @brief           This service modifies a given memory location (<memory address>)
 *                  with the formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[in]       Clearmask: memory address will be modified by a bit-AND.
 * @param[in]       Setmask: memory address will be modified by a bit-OR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
/* PRQA S 4152, 1512 ++ */ /* VL_Os_4152, VL_Os_1512 */
extern StatusType ModifyPeripheral16(AreaIdType Area, uint16 *Address, uint16 Clearmask, uint16 Setmask);
/* PRQA S 4152, 1512 -- */

/**
 * @brief           This service modifies a given memory location (<memory address>)
 *                  with the formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference.
 * @param[in]       Address: memory address.
 * @param[in]       Clearmask: memory address will be modified by a bit-AND.
 * @param[in]       Setmask: memory address will be modified by a bit-OR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status).
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status).
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern StatusType ModifyPeripheral32(AreaIdType Area, uint32 *Address, uint32 Clearmask, uint32 Setmask); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif


#if (CFG_BARRIER_MAX > 0U)
/**
 * @brief           Synchronize the calling task at the barrier given in "BarrierID".
 *                  The calling task blocks until all other participating tasks have called 
 *                  this API method with the same "BarrierID".
 * @param[in]       BarrierID: Barrier reference.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Barrier <BarrierID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_SYS_NO_BARRIER_PARTICIPANT: Task is not configured to participate in the barrier.
 * @retval          E_OS_DEADLOCK : Deadlock is deteted.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
 extern StatusType Os_BarrierSynchronize(Os_BarrierIdType BarrierID); /* PRQA S 1512 */ /* VL_Os_1512 */
 #endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
/*
 * Brief                <Get the scheduling frequency of tasks.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
extern void GetTaskScheduleCount(TaskType taskId, uint32* taskCnt); /* PRQA S 1512 */ /* VL_Os_1512 */

/*
 * Brief                <Get the scheduling frequency of ISR2.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
extern void GetIsr2ScheduleCount(ISRType isrId, uint32* isrCnt); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/*
 * Brief                <Get the specified Task or ISR load ratio value>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ObjectID: Task ID or ISR ID>
 *                      <calcType: Calculation method of load ratio>
 *                      <For CPU load ratio, Do Not care the value of ObjectID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <10000 times larger percentage load rate>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
extern uint32 GetLoadRatioValue(ObjectType objectId, LoadRatioCalcType calcType); /* PRQA S 1512 */ /* VL_Os_1512 */
#endif

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
