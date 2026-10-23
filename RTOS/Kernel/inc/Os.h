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
      Risk: Low risk. Logic short circuit may cause functions to not be executed
      Prevention: Code review to verify macro expansion behavior across all supported platform configurations,
  supplemented by unit testing on each target platform.

    \li VL_Os_PlatformDef
      Reason: Defined by the chip platform, macro functions have different usage scenarios for ported code.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4391
      Reason: The OS module ensures that converted operation is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1831
      Reason: The OS module ensures that implicitly converted operation is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4447
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2850
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1291
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3793
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4446
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3431
      Reason: This rule indicates that a macro defines an operator, punctuation mark, or control statement keyword. The
  masked macro definition clearly expresses its intent here.
      Risk: Low risk. Macro substitution may lead to semantic
  ambiguity or unexpected behavior.
      Prevention: Conduct code review and strictly check whether the result after this
  type of macro substitution meets expectations.

    \li VL_Os_0603
      Reason: This rule indicates that the macro identifier is reserved. The project defines macro identifiers beginning
  with an underscore, which are flagged as reserved identifiers by the static analysis tool. These typically involve
  platform‑ or compiler‑related macro definitions and do not conflict with identifiers in the system library within the
  project.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3411
      Reason: This rule indicates mismatched brackets, parentheses, or braces in the macro definition. The masked
  content is a highly specialized macro that clearly expresses its intent.
      Risk: Low risk. Bracket mismatches after
  macro substitution could cause compilation errors or unexpected parsing.
      Prevention: Conduct code review and strictly
  check whether bracket mismatches still exist after this type of macro substitution.

    \li VL_Os_0401
      Reason: This rule indicates that an object may be modified more than once between sequence points. In the project,
  the masked object is indeed modified multiple times, but the order of these modifications is well-defined and will not
  result in undefined behavior.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0307
      Reason: This rule indicates a conversion between a pointer to an object and a pointer to a function. In the
  project, the masked conversion between a function pointer and an object pointer ensures that no pointer content is
  lost due to insufficient width, therefore this conversion is safe within the project context.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3771
      Reason: QAC will compile the code and assume that a type conversion has occurred for the parameters, but in
  reality, this is not the case.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3006
      Reason: The function may contain some inline assembly code or assembly-related macro definitions. In embedded
  systems, the use of assembly language is necessary, especially in scenarios that require direct access to hardware
  registers or the implementation of specific performance optimizations. This assembly code has been encapsulated within
  macro definitions or functions, isolated from other C code, which complies with the rule requirements.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_ConstToIntegral
      Reason: Used when assigning special values, such as null pointers.
      Risk: Low risk. The conversion from const to non-const may lead to potential loss of type safety and unintended
  modification of read-only data.
      Prevention: Code review to verify that each const-to-integral conversion preserves
  type safety and that the converted values remain within the valid range of the target integral type.

    \li VL_Os_3432
      Reason: Simple macro parameter expressions are only used for conditional judgments and do not require parentheses.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3450
      Reason: The order of function calls for internal links is determined, and there is no need to declare it again.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1840
      Reason: Configure to ensure the security of implicit conversion of constants to unsigned types.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0582
      Reason: The configuration ensures the security of the padding bit size
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1290
      Reason: Configuration, parameter checking, and design ensure the security of data type conversion
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0404
      Reason: Access to volatile objects involving chips, guaranteed by hardware.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2481
      Reason: Chip platform porting code ensures the correctness of macro function loop conditions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0310
      Reason: The expression involves the conversion of a configuration array pointer type. The converted type matches
  the actual stored data type, ensuring type safety. The generic void** type configuration array is converted to a
  specific type pointer to support polymorphic configuration data.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2743
      Reason: To ensure that the macro expansion is an independent statement.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0306
      Reason: The expression involves conversion from a pointer type to an integer type, converting a pointer to a
  uint32 type. This is used for checking if a memory address is writable, verifying memory address ranges, performing
  stack alignment calculations, etc.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4544
      Reason: Code design uses fixed offset to obtain information, and computational security is ensured by design.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3333
      Reason: There are multiple ending branches in the specified case conditions that cannot be further divided.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2834
      Reason: The dividend is derived from the configuration and cannot be set to 0.
      Risk: Low risk. If the configuration is incorrectly set to 0, a division-by-zero error may occur, leading to
  undefined behavior or system crash.
      Prevention: Configuration validation to ensure the divisor is always a non-zero
  value before use, and code review to verify that all division operations involving configuration-derived divisors are
  guarded.

    \li VL_Os_0311
      Reason: This rule indicates that pointer type conversions should not remove the const or volatile qualifiers of
  the pointed-to type. In the expression, the original data is defined as const during the configuration phase, and it
  needs to be accessed via a pointer at runtime. The conversion maintains the const qualifier, ensuring the data is not
  accidentally modified.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3678
      Reason: This rule indicates that pointers should point to const-qualified types as much as possible. The pointer
  type in the expression has already been qualified with the const specifier, but QAC may generate false positives
  regarding the const qualification of multi-level pointers. Additionally, in actual development, there are scenarios
  where data needs to be modified via pointers. In such cases, using the const qualifier may lead to type mismatches or
  compilation errors.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3471
      Reason: In order to improve code readability and call efficiency.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3472
      Reason: This rule indicates that functions should be preferred over function-like macros when they are
  interchangeable. The use of function-like macros here is for performance considerations, avoiding the overhead of
  function calls, which is crucial for real-time operating system performance. Furthermore, encapsulating implementation
  differences across various hardware architectures within macros provides a unified interface, facilitating code
  maintenance and portability.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2870
      Reason: This rule indicates that the expression is a constant infinite loop structure. In actual projects,
  infinite loops are typically required to provide the core idle state or no-operation handling during core system
  exceptions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4397
      Reason: Ensure the security of data conversion through parameter inspection and design.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2487
      Reason: Used in special scenarios to control CPU execution status and wait for other asynchronous awakenings.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4340
      Reason: Conditional judgment of unsigned types used as boolean values.
      Risk: Low risk. It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_Os_2911
      Reason: Confirmed: enclosed in unsigned arithmetic operations.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_HookDef
      Reason: Effective after Hook configuration.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1505
      Reason: This rule indicates that if functions and objects are only referenced within a single translation unit,
  they should not be defined with external linkage. While these functions are defined in the current file, they need to
  be visible in other translation units in order to be called by other modules. Therefore, using external linkage is
  necessary.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2880
      Reason: This rule indicates that the project must not contain unreachable code. In functional safety development,
  defensive coding is strongly recommended. Examples include null pointer checks for reference type parameters and
  adding default branches in switch-case statements, among others. Additionally, the project includes code paths whose
  reachability depends on configurations. Under the current configuration, such code may be flagged as unreachable, but
  it may be reached under other configurations.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2742
      Reason: The macro definition determines the condition, which is used in special single character scenarios.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2953
      Reason: Design, tool configuration, and parameter checking to ensure non negative operands.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2842
      Reason: This rule indicates that the project contains dereferencing operations on an invalid pointer value. The
  suppressed dereferencing operations involve safe access to configuration data and will not result in out-of-bounds
  array access during dereferencing.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3138
      Reason: Non empty statements in multi-core or other configuration scenarios.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3672
      Reason: Ioc callback function usage, callback function is determined by the user.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0489
      Reason: Pointer operation is used for efficient copying of data.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3412
      Reason: Ioc multi-core synchronization scenario usage.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1338
      Reason: This rule indicates that function parameters should not be modified. The expressions involving violations
  of this rule do not rely on the parameter as an output argument. The primary uses are: Converting a global object ID
  to a local object ID, with the converted value used for subsequent secure access via array indexing. Using the
  parameter as a local variable within the function body. Since C uses pass-by-value, modifying the parameter inside the
  function does not affect the caller.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0488
      Reason: This rule indicates that an addition or subtraction operation on a pointer-type expression is being
  performed, typically used for memory allocation, building stack frames, or calculating array offsets, etc. The pointer
  arithmetic being executed in the masking items is strictly checked for its step size within the context to ensure that
  it does not result in out-of-bounds accesses or accesses to unallocated memory. Additionally, the original type of the
  address offset step is ensured to be an integer multiple of the pointer type to guarantee that no misaligned memory
  accesses occur.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2740
      Reason: Special scenario use, such as defensive code.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0490
      Reason: Os module design ensures correctness.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0318
      Reason: Arch call, a redundant operation performed to ensure security
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3223
      Reason: Bitmap initialization usage.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3455
      Reason: Cross core scenarios, passing callback functions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1259
      Reason: Stack check is used to mark memory.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4461
      Reason: Configure and design to ensure that data conversion does not result in data loss.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0303
      Reason: Cross core scenarios, using volatile to ensure data consistency.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3442
      Reason: Ensure the correctness of strong conversion through parameter checking and configuration.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0770
      Reason: 'Continue' statement is used to implement special features of the OS.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0771
      Reason: This rule indicates that the termination of any iterative statement should not use more than one break
  statement. Although multiple break statements are used, each break has a clear exit condition, and the logic is
  well-defined.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3679
      Reason: This rule indicates that pointers should, as much as possible, point to a const-qualified type. However,
  in embedded operating systems, there are often scenarios where data is modified through pointers. Therefore, they
  cannot be declared as const pointer types.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3406
      Reason: Related to hardware, cloning variables are used.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3410
      Reason: This rule indicates that macro parameters are not enclosed in parentheses. In the project, the macro
  parameters are ensured to always be atomic, thus they will not cause macro substitution errors.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2755
      Reason: Remove the termination task from the bitmap in the app, and use it in special scenarios.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3209
      Reason: The return value is ignored because it is not needed or cannot be processed.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0789
      Reason: User defined characters, with configuration tools ensuring the accuracy of character matching.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0779
      Reason: Configure tools to ensure the correctness of character matching.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0631
      Reason: Arch specific data type, only used for memory protection function address tagging.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4342
      Reason: This rule indicates that an expression of an unsigned type is cast to an enumeration type. In the project,
  the involved casts from unsigned types to enumeration types are all guaranteed to keep the basic type within the valid
  range of the enumeration type.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3464
      Reason: The macro is only used in well-defined OS internal contexts where the argument evaluation is guaranteed to
  occur at most once.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3469
      Reason: This rule indicates that when functions and function-like macros are interchangeable, functions should be
  preferred over function-like macros. However, in embedded systems, the use of function-like macros is aimed at
  improving performance, ensuring real-time capabilities and reliability, or encapsulating low-level hardware operations
  to provide greater flexibility and portability.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1258
      Reason: OS uses special meaning integer constants to convert to different integer types1.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3232
      Reason: The static pointer is reassigned at runtime to reference
              different string literals. QAC cannot detect the reassignment
              and reports a false positive suggesting the pointer could be
              const-qualified.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3449
      Reason: This rule indicates that an external object or function is declared multiple times. Due to the presence of
  header file guard declarations, this will not result in multiple declarations of the external object or function.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3451
      Reason: This rule indicates that a global identifier is declared in multiple files. Due to the presence of header
  file guard declarations, this will not result in the global identifier being declared in multiple files.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3458
      Reason: This rule indicates that the macro defines a code statement block enclosed in braces. In fact, no issues
  arise when these macros are expanded during preprocessing.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0784
      Reason: This rule indicates that an identifier is used both as a macro name and a non-macro name. The identifier
  is employed as an object-like macro name and remains in the program source code after preprocessing.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3409
      Reason: This rule indicates that the replacement list of a function-like macro is not enclosed in parentheses.
  When the replacement list contains arithmetic operators and is not enclosed in parentheses, the result may not meet
  expectations. In the project, the masked macro functions, when expanded, are ensured to produce results that meet
  expectations.
      Risk: Low risk. Operator precedence after macro expansion may cause unexpected evaluation order and
  incorrect computation results.
      Prevention: Code review ensures that the expansion results of macro functions comply
  with expectations.

    \li VL_Os_3684
      Reason: This rule indicates that when declaring an array with external linkage, its size shall be explicitly
  specified. However, in the suppressed extern declaration, the symbol referenced is defined in the linker script and is
  used to mark the boundaries of memory regions. Declaring it as uint8[] is for the convenience of pointer arithmetic
  and address calculations. Since these symbols are defined by the linker, their sizes are unknown at compile time,
  making it impossible to explicitly specify the array size.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3218
      Reason: This rule indicates that a static variable with file scope is accessed in only a single function, and it
  is recommended to define it within the function scope. However, defining static variables at file scope rather than
  function scope can maintain structural clarity and improve readability. Additionally, some compilers may be unable to
  compile code containing static declarations inside functions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3345
      Reason: This rule concerns multiple accesses to a volatile object. In the project, multiple accesses to volatile
  objects do not cause side effects, as it is ensured that their execution order will not have any impact.
      Risk: Low
  risk. Multiple accesses to volatile objects may trigger unintended side effects, such as clearing hardware status
  flags or reading stale/inconsistent data across accesses.
      Prevention: During the code review phase, strict scrutiny is
  conducted on accesses to volatile objects to ensure that no side effects are triggered.

    \li VL_Os_2872
      Reason: This rule indicates that if execution enters this loop, it will never terminate. In projects, there are
  two scenarios: 1. When a systematic exception occurs, the loop is entered as part of the intended design. 2. In normal
  loops, the loop control variable is correctly modified, ensuring the loop terminates as expected.
      Risk: NO risk.
      Prevention: None.

    \li VL_Os_3141
      Reason: This rule indicates the presence of null statement occupying a line by itself. The blocked content is
  often related to hardware platforms and specific configurations, and in some hardware platforms or configuration
  scenarios, it is not null statement.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3219
      Reason: This rule indicates that the static function is not utilized within this translation unit. The project may
  contain static functions that are invoked in assembly code, which may not be recognized correctly. Additionally, some
  functions may be defined as inline functions, but not all source files that include the corresponding header file
  actually use these functions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2053
      Reason: This rule indicates that the block comment appears to be commenting out source code. In the project, there
  is no practice of commenting out source code; the natural language comment is misinterpreted as valid C code due to
  formatting issues. Therefore, some false positives and false negatives may occur.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2741
      Reason: This rule indicates that the 'if' control expression is a constant expression that evaluates to 'true'. In
  the project, this value remains consistently 'true' based on configuration parameters or platform-specific
  implementations; it is actually a defensive programming design adopted within the OS and is not a redundant operation.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0791
      Reason: This rule indicates that macro identifiers should be distinguishable within the specified number of
  significant characters. The suppressed items involve macros with similar functional attributes. While the differences
  between their identifiers are small, they are sufficient to guarantee that they are distinct macros, thus ensuring the
  correctness of the functionality.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_5100
      Reason: Although the variable name contains "DEBUG", it is actually used as production application access 
    permission configuration. It is product functional data, not debug-only data, and must be present in all build variants. 
    Its memory footprint has been evaluated and it has no adverse impact on the system.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1514
      Reason: This rule indicates that the object is referenced by functions only within the translation unit where it
  is defined. The blocked object needs to be referenced by other translation units and cannot be designed as a static
  object.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0286
      Reason: This rule indicates that a string literal contains characters that are not part of the basic source
  character set. In the project, what is blocked is not a string literal but platform-related inline assembly code.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0884
      Reason: This rule indicates that using multiple # operators within the same macro definition may lead to
  non-portable code during macro expansion, as the standard does not explicitly specify the evaluation order of the #
  operator. However, in the project, the # operator connects only one macro parameter, so the aforementioned issue does
  not exist.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0341
      Reason: This rule indicates that the stringizing operator # is used. Early versions of the C language may not have
  supported this feature, but the C language version used in the project no longer has this issue.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3203
      Reason: This rule indicates that a variable is assigned but never used. In the project, some variables are
  declared and used within macro expansions. Therefore, it is not truly a case of a variable being assigned but never
  used.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2983
      Reason: This rule indicates that the assignment is redundant, as the value of this object is never used
  thereafter. The usage of such objects depends on specific configurations. In the current configuration, it may not be
  used, but it will be used in other configurations.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3408
      Reason: This rule indicates that an object or function with external linkage is being defined, but no prior
  declaration for it was encountered. The suppressed items involve functions that are only accessed within the current
  translation unit and will not cause linking errors.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1502
      Reason: This rule indicates that an object is defined but not used in this project. In the project, some objects
  are defined and used within macro expansions. Therefore, it is not a true case of an object being defined but not
  used.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0305
      Reason: This rule indicates that a forced conversion occurs between a function pointer and an integer type. In the
  project, it is common to encounter conversions of function pointers to uint32. The integer type's supported size is
  determined by the hardware platform. Currently, on the supported hardware platforms, such forced conversions do not
  cause data overflow.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0428
      Reason: This rule indicates that a function identifier is not followed by () but may be intended for a function
  call. However, in the project, most cases are not intended for function calls but rather for use as function pointers.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3206
      Reason: This rule indicates that a parameter is not used in this function. In the project, some function
  parameters are used within macro expansions or in assembly code. Therefore, it is not truly a case of a parameter
  being unused.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3008
      Reason: This rule indicates that the function mixes inline assembly with "non-executable" C statements, such as
  uninitialized variable declarations. In the project, inline assembly uses local variables as output parameters;
  therefore, the uninitialized state of these variables does not affect the safe execution of the function.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2961
      Reason: This rule indicates the use of uninitialized automatic objects. The automatic objects involved in the
  suppression are assigned values in the assembly code, thus preventing their use without initialization.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4434
      Reason: This rule indicates that a non-constant expression of an inherently signed type is converted to an
  unsigned type during assignment. In the project, such conversions do not result in data overflow or loss, thus
  ensuring the safety of these conversions.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1340
      Reason: Store the address of the parameter in a constant pointer to avoid unexpected modifications.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1252
      Reason: Suffixed integer constant implicitly converted to different integer type, Special code design of the Ioc.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4152
      Reason: The standard interface specified by AUTOSAR.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1891
      Reason: The correctness of implicit conversion of unsigned type parameters to a wider unsigned type is ensured by
  the OS module.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2812
      Reason: This rule indicates a potential null pointer dereference. In practice, references of pointer type are
  always verified to be not NULL before dereferencing.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0499
      Reason: The OS service protection module ensures the correctness of shift operator operations.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0326
      Reason: The cast of a null pointer to an integer type is a requirement of the code design.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0674
      Reason: For the universality of parameter transmission, initializer for pointer is of incompatible type.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0691
      Reason: Unused para, default initialization is applied to the remainder of the array elements.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0704
      Reason: Unused para, default initialization is applied to the remainder of the members.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2991
      Reason: This rule indicates that the value of this if control expression is always true. However, by using
  conditional compilation to support different configuration options, the value of the expression may vary depending on
  the configuration and will not always be true.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2992
      Reason: This rule indicates that the value of the if-expression is always false. In the project, it is impossible
  for the value of this expression to always be false, as the inspection mechanism within the function body returns
  different error states, thereby breaking the condition where the if-expression would always evaluate to false.
      Risk:
  No risk.
      Prevention: None.

    \li VL_Os_2995
      Reason: This rule indicates that the project contains dead code, and the result of this logical operation is
  always true. However, this is a result of conditional compilation; it may be enabled or disabled based on
  configuration and is not genuinely dead code.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2996
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2997
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2998
      Reason: Due to configuration or other reasons, the result of this logical operation might be always false.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2982
      Reason: Control variables are used to reflect the current status, the value of this object might be never used
  before being modified.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4521
      Reason: This rule indicates that an expression of an enumeration type is used as an operand of an arithmetic
  operator. In the masked expression, it must be ensured that the result of the arithmetic operation on the enumeration
  type will not cause overflow.
      Risk: Low risk. Arithmetic operations on enumeration types may cause overflow, producing
  values outside the valid enumeration range and leading to undefined behavior.
      Prevention: Code review to ensure that
  arithmetic operations on all enumeration types do not overflow.

    \li VL_Os_4522
      Reason: This rule indicates that an expression of an enumeration type is used as an operand of a bitwise operator.
  In the masked expression, it must be ensured that the result of the bitwise operation on the enumeration type will not
  cause overflow.
      Risk: Low risk. Bitwise operations on enumeration types may produce values outside the valid
  enumeration range, resulting in undefined behavior.
      Prevention: Code review to ensure that bitwise operations on all
  enumeration types do not overflow.

    \li VL_Os_4523
      Reason: This rule indicates that an expression of an enumeration type is used as the left-hand operand of a shift
  operator. In the masked expression, it must be ensured that the result of the shift operation on the enumeration type
  will not cause overflow.
      Risk: Low risk. Shift operations on enumeration types may produce values outside the valid
  enumeration range, resulting in undefined behavior.
      Prevention: Code review to ensure that shift operations on all
  enumeration types do not overflow.

    \li VL_Os_1317
      Reason: The OS module ensures that using the constant type as the enum type is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1441
      Reason: The OS module ensures that using the unsigned type as the enum type in function calls is valid.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4442
      Reason: The OS module ensures that using the unsigned type as enum type is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4322
      Reason: This rule indicates that the expression involves a forced conversion between different enumeration types.
  Although it involves casts between different enum types, it is ensured that the value after conversion remains
  compatible.
      Risk: Low risk. Casting between different enumeration types may produce incompatible values, leading to
  undefined behavior if the value does not correspond to a valid enumerator in the target type.
      Prevention: Code review
  ensures the compatibility of values after conversion between different enumeration types.

    \li VL_Os_3326
      Reason: The OS module ensures that the result of an assignment is correctly used in a logical operation.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3200
      Reason: The return value is not used because it is not needed in the current scenario.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3334
      Reason: This declaration of para hides a more global declaration.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3305
      Reason: This rule indicates that a pointer is being cast to a pointer type with stricter alignment requirements.
  The suppressed expressions are all verified to ensure that the pointer type conversions maintain proper alignment,
  with no associated risk.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4424
      Reason: An expression of essentially enum is being converted to unsigned type, on assignment.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0432
      Reason: For the sake of code generality, pointer type conversion is used.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0342
      Reason: This rule indicates that the concatenation operator ## is used in the project to join two identifiers into
  a new single identifier. The suppressed item involves parameter expansion within a macro definition, ensuring that
  parameters are correctly concatenated during macro expansion.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1271
      Reason: For the sake of code intuitiveness, using a non-int expression to define an enum constant.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3229
      Reason: The variable is accessed from inline assembly, which is outside
              the scope of QAC dataflow analysis. This causes an incorrect
              report that the variable is unused.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0602
      Reason: The identifier is reserved for use by the library.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3625
      Reason: Type char has been used in the declaration of an object or a function.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1821
      Reason: The OS module ensures that using the signed int type as the unsigned long type is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4532
      Reason: The OS module ensures that using the signed int type as the left-hand operand of this bitwise operator (|)
  is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4542
      Reason: The OS module ensures that using the signed int type as the right-hand operand of this bitwise operator
  (|) is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4543
      Reason: The OS module ensures that using the signed int type as the left-hand operand of this bitwise operator
  (<<) is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1861
      Reason: The OS module ensures that the result generated by the operands of this bitwise operator is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3762
      Reason: The OS module ensures that using the int type as the unsigned long type is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4527
      Reason: This rule indicates that an expression of an enumeration type is used as an operand of an increment or
  decrement operator. In the masked expression, it must be ensured that the result of the increment or decrement
  operation on the enumeration type will not cause overflow.
      Risk: Low risk. Increment or decrement operations on
  enumeration types may cause overflow, producing values outside the valid enumeration range and leading to undefined
  behavior.
      Prevention: Code review to ensure that increment or decrement operations on all enumeration types do not
  overflow.

    \li VL_Os_1277
      Reason: The OS module ensures that Hex constant is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1880
      Reason: The OS module ensures that comparison between different types is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1841
      Reason: The OS module ensures that implicitly converted operation is correct.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2813
      Reason: The OS logic code ensures that pointer dereferencing will not result in a null pointer situation.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3332
      Reason: The OS module ensures that undefined identifiers can be used correctly.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1297
      Reason: The OS module ensures that security of data type conversion.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3433
      Reason: The OS module guarantees the correctness of implicit conversions between pointers and integers.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3344
      Reason: The OS module guarantees the correctness of applying left- or right-shift operators to signed negative
  values.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3440
      Reason: The OS module guarantees the correctness of pointer casts.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3387
      Reason: The OS module guarantees the correctness of function declared with external linkage.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0314
      Reason: This rule indicates that a pointer to an object type is cast to a void pointer. Since there is no
  violation of alignment requirements, a pointer to an object can always be safely cast to a pointer to void.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_4432
      Reason: The OS module guarantees the correctness of macro parameter operand.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0722
      Reason: The OS module guarantees the correctness of include file nested to a depth.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0723
      Reason: The OS module guarantees the correctness of include file name matches another file on the include search
  path.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0724
      Reason: The OS module guarantees the correctness of enumeration constant value.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1434
      Reason: The OS module guarantees the correctness of identifier declared but never referenced.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1461
      Reason: The OS module guarantees the correctness that the function has no return statement.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3443
      Reason: The OS module guarantees the correctness of implicit conversion between pointer and integer types.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2987
      Reason: The OS module guarantees the correctness of function declared with external linkage.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1039
      Reason: The OS module guarantees the correctness of treating a one-element array as a potentially flexible array
  member.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1431
      Reason: The OS module guarantees the correctness of passing an enum object as an argument to a function whose
  parameter is of non-enum type.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1006
      Reason: Code may expand under certain architectures.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2004
      Reason: The OS module guarantees the correctness of if-else-if.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3415
      Reason: The OS module guarantees the correctness of the right-hand operand in '&&' or '||'.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1753
      Reason:The function with external linkage is declared but not defined within this project.
      Risk:No risk.
      Prevention: None.

    \li VL_Os_1501
      Reason:The OS module guarantees the correctness that the function is declared but is not used within this project.
      Risk:No risk.
      Prevention: None.

    \li VL_Os_1512
      Reason:The OS module guarantees the correctness that the value with external linkage has separate declarations in
  multiple translation units.
      Risk:No risk.
      Prevention: None.

    \li VL_Os_1513
      Reason:The OS module guarantees the correctness that identifier with external linkage has separate non-defining
  declarations in more than one location.
      Risk:No risk.
      Prevention: None.

    \li VL_Os_1712
      Reason: This rule indicates that the project contains multiple identifiers whose first few characters are
  identical. Although the naming of these identifiers may appear similar in certain contexts, their scopes and purposes
  are well-defined and will not cause confusion.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1506
      Reason: The OS module guarantees the correctness that ObjectType is declared as a typedef and is used elsewhere
  for a different kind of declaration.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_5209
      Reason: The OS module ensures the correctness that unsigned long is used.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1520
      Reason: The OS module ensures the correctness that the functions are indirectly recursive.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1752
      Reason: This rule indicates that an object with external linkage has been declared in the project but is not
  defined within it. The suppressed extern declarations refer to symbols defined in the linker script. These linker
  symbols are provided definitions by the linker during the linking stage, not defined in C source files.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1500
      Reason: The OS module guarantees the correctness that the value is used in this project.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3207
      Reason: The switch statement allows only the default actions to run, will not affect the running results.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0632
      Reason: This rule indicates that a tentative definition of an object with internal linkage cannot have an unknown
  size. The objects suppressed in the project are tentative definitions of interrupt vector table arrays, whose actual
  sizes depend on specific configurations and do not lead to out-of-bounds array access behavior.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0613
      Reason: This rule indicates that the size of the object exceeds the minimum standard requirement. The size of the
  masked item directly depends on the configuration data, and this message is not an error code identifier, so it will
  not affect the safe execution of the function.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1021
      Reason: This rule indicates that the expression includes a statement expression. The masked content is all
  hardware-platform-related, but in the project, this part is uniformly abstracted and adapted by the adaptation module
  according to different compilers, thus it will not reduce portability.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0605
      Reason: This rule indicates that this is not a valid declaration. The masked content involves platform-dependent
  macro expansion and inline assembly, which have been incorrectly flagged as a constraint violation by the static
  analysis tool.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2200
      Reason: Indentation of this line is to the left of the current block indentation level.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_3446
      Reason: The 2nd or 3rd operand of this conditional operator is an expression with persistent side effects.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_1894
      Reason: one is a composite expression of a narrower type than the other.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_2011
      Reason: Used to speed up the execution of the function.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0388
      Reason: This rule indicates that the nesting level of #include directives exceeds the limit and does not strictly
  comply with the ISO:C99 standard. These messages do not point to erroneous code, but rather identify deviations from
  strict compliance as described in the relevant standard.
      Risk: No risk.
      Prevention: None.
    
    \li VL_Os_1002
      Reason: The '$' character is used in linker-generated section boundary
      symbols. This is an IAR toolchain extension and is not part of the ISO C basic source character set.
      These symbols are required for memory protection configuration.
      Risk: No risk.
      Prevention: None.

    \li VL_Os_0289
      Reason: The identifiers containing '$' are linker-generated section boundary symbols. 
      The '$' character is accepted by the IAR compiler as an extension. These symbols are necessary to obtain
      the start and end addresses of memory sections for memory protection setup..
      Risk: No risk.
      Prevention: None.

    \li VL_Os_AutosarBool
      Reason: The Autosar specification defines the boolean type as an unsigned char.
      Risk: Low risk. The behavior of Boolean logic does not align with expectations.
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
      Reason: For certain special files that contain little or no code, it is acceptable to reduce or omit extraneous
  comments.
      Risk: Reduced code readability Prevention: Functional reliability guaranteed by design.

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
      Risk: Increase the difficulty of function debugging.
      Prevention: Ensure functional correctness through comprehensive testing.

    \li VL_Os_Redeclaration
      Reason: Other modules declare some internal operating system functions of the OS to achieve efficient access.
      Risk: There is an internal function intrusion into the OS module.
      Prevention: Control the scope of exposed functions and only provide library functions that allow re-entry.

*/

#ifndef OS_H
#define OS_H

/* =================================================== inclusions =================================================== */
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Asynchronous version of the ActivateTask() function. Intended to be used for cross core task
 * activation. Possible errors are not returned to the caller, but may be reported via error hooks.
 * @param[in]       TaskID: Task reference.
 * @synchronous     FALSE
 * @reentrant       TRUE
 * @trace           CPD-57615
 */
extern void ActivateTaskAsyn(TaskType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service causes the termination of the calling task. After termination of the
 *                  calling task a succeeding task <TaskID> is activated. Using this service,
 *                  it ensures that the succeeding task starts to run at the earliest after the calling task has been
 * terminated.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           If a higher-priority task is ready, the internal resource of the task is released,
 *                  the current task is put into the ready state, its context is saved and the higher-priority task is
 * executed. Otherwise the calling task is continued.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           GetTaskID  returns the information about the TaskID of
 *                  the task which is currently  running.
 * @param[inout]    TaskID: Reference to the task which is currently running.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is NULL_PTR.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is invalid (address is not writable by this
 * OS-Application).
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetTaskID(TaskRefType TaskID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Returns the state of a task (running, ready, waiting, suspended) at the time of calling
 * GetTaskState.
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
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of TaskID is invalid (address is not writable by this
 * OS-Application).
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetTaskState(TaskType TaskID, TaskStateRefType State); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This call serves to enter critical sections in the code that are assigned to the resource referenced
 * by <ResID>. A critical section shall always be left using ReleaseResource.
 * @param[in]       ResID: Reference to resource.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID: Resource <ResID> is invalid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: Attempt to get a resource which is already occupied by any task or ISR,
 *                  or the statically assigned priority of the calling task or interrupt routine is higher than the
 * calculated ceiling priority.
 * @retval          E_OS_CORE : The core id of ResID is not equal to running system core.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern StatusType GetResource(ResourceType ResID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           ReleaseResource is the counterpart of GetResource and serves to leave critical sections
 *                  in the code that are assigned to the resource referenced by <ResID>.
 * @param[in]       ResID: Reference to resource.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_NOFUNC: Attempt to release a resource which is not occupied by any task or ISR, or another
 * resource shall be released before.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_EXTENDED_TASK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The service may be called from an interrupt service routine and from the task level, but not from
 * hook routines.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The service may be called from an interrupt service routine and from the task level, but not from
 * hook routines.
 * @param[in]       TaskID: Reference to the task for which one or several events are to beset.
 * @param[in]       Mask: Mask of the events to be set.
 * @synchronous     FALSE
 * @reentrant       TRUE
 * @trace           CPD-57289
 */
extern void SetEventAsyn(TaskType TaskID, EventMaskType Mask); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service returns the current state of all event bits of the task<TaskID>, not the events that
 * the task is waiting for.
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
extern StatusType GetEvent(TaskType TaskID, EventMaskRefType Event);
/* PRQA S 1512, 3209 */ /* VL_Os_1512, VL_Os_3209 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The state of the calling task is set to waiting, unless at least one of the events specified in
 * <Mask> has already been set.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The state of the calling task is set to waiting, must all the events specified in <Mask> has already
 * been set.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_ALARM_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The system service GetAlarmBase reads the alarm base characteristics.
 *                  The return value <Info> is a structure in which the information of data type AlarmBaseType is
 * stored.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The system service occupies the alarm <AlarmID> element. After <increment> ticks have elapsed,
 *                  the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended
 * tasks) is set or the alarm-callback routine is called.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#if (CFG_COUNTER_MAX > 0U)
#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service increases count for a software counter.
 * @param[in]       CounterID: The Counter to be incremented.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : The CounterID was not valid or counter is implemented in hardware and cannot be
 * incremented by software.
 * @retval          E_OS_CORE: The core id of CounterID is not equal to running system core.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of CounterID cannot be accessed.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57533
 */
extern StatusType IncrementCounter(CounterType CounterID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service reads the current count value of a counter (returning either the hardware timer ticks
 *                  if counter is driven by hardware or the software ticks when user drives counter).
 * @param[in]       CounterID: The Counter which tick value should be read.
 * @param[out]      Value: Contains the current tick value of the counter.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The Value is NULL.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value is invalid (address is not writable by this
 * OS-Application).
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service gets the number of ticks between the current tick value and a previously read tick
 * value.
 * @param[in]       CounterID: The Counter which tick value should be read.
 * @param[inout]    Value: the previously read tick value of the counter out: the current tick value of the counter.
 * @param[out]      ElapsedValue The difference to the previous read value.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_ILLEGAL_ADDRESS: The given Value or ElapsedValue was NULL.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value or ElapsedValue is invalid (address is not writable by
 * this OS-Application).
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of CounterID cannot be accessed.
 * @retval          E_OS_CORE : the remote core ID is invalid or the remote core is not running.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @retval          E_OS_ID: The CounterID was not valid.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57544
 */
extern StatusType GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service returns the current application mode. It may be used to write mode dependent code.
 * @return          AppModeType
 * @retval          AppModeType: the active application mode.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern AppModeType GetActiveApplicationMode(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service disables all interrupts for which the hardware supports disabling.
 *                  The state before is saved for the EnableAllInterrupts call.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void DisableAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service restores the state saved by DisableAllInterrupts.
 *                  The state before is saved for the EnableAllInterrupts call.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void EnableAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service restores the recognition status of all interrupts saved by the SuspendAllInterrupts
 * service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ResumeAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service saves the re cognition status of all
 *                  interrupts and disables all interrupts for which the hardware supports.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void SuspendAllInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service restores the recognition status of interrupts saved by the SuspendOSInterrupts service.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ResumeOSInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service saves the recognition status of interrupts of category 2
 *                  and disables the recognition of these interrupts.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void SuspendOSInterrupts(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service
 * Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57612
 */
extern StatusType EnableInterruptSource(ISRType ISRID, boolean ClearPending); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Disables the interrupt source by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_NOFUNC: Breaks nested calls to source control API errors.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function(EXTENDED status).
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service
 * Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57613
 */
extern StatusType DisableInterruptSource(ISRType ISRID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           Clears the interrupt pending flag by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier (EXTENDED status).
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function(EXTENDED status).
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID (Service
 * Protection).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57614
 */
extern StatusType ClearPendingInterrupt(ISRType ISRID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The user can call this system service to start the operating system in a specific mode,
 *                  see chapter 5, Application modes.
 * @param[in]       Mode: application mode.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void StartOS(AppModeType Mode); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_SCHEDTBL_MAX > 0U)
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service starts an explicitly synchronized schedule table synchronously.
 * @param[in]       ScheduleTableID: Schedule table to be started.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_CORE: Cross core use shall be not supported.
 * @retval          E_OS_CALLEVEL: Call level error.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ACCESS: The OS-Application of ScheduleTableID cannot be accessed.
 * @retval          E_OS_ID : Invalid ScheduleTableID; Or the synchronization strategy of ScheduleTableID is not
 * EXPLICIT.
 * @retval          E_OS_STATE: Schedule table was already started.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57507
 */
extern StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service cancels the processing of a schedule table immediately at any point while the schedule
 * table is running.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service starts the processing of a schedule table at an absolute value "Start" on the
 * underlying counter.
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
extern StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service starts the processing of a schedule table at "Offset" relative to the "Now" value on
 * the underlying counter.
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
extern StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service returns the identifier of the currently executing ISR.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57443
 */
extern ISRType GetISRID(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_OSAPPLICATION_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service determines the OS-Application where the caller of the service is currently executing.
                    Note that if the caller is not within a CallTrustedFunction() call the value is equal to the result
 of GetApplicationID().
 * @return          ApplicationType
 * @retval          ApplicationType: <identifier of running OS-Application> or INVALID_OSAPPLICATION.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57439
 */
extern ApplicationType GetCurrentApplicationID(void);
/* PRQA S 1753, 1501, 1512 */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service sets the own state of an OS-Application from APPLICATION_RESTARTING to
 * APPLICATION_ACCESSIBLE.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value is invalid (address is not writable by this
 * OS-Application).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57565
 */
/* PRQA S 1753, 1501, 1512 ++ */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */
extern StatusType GetApplicationState(ApplicationType Application, ApplicationStateRefType Value);
/* PRQA S 1753, 1501, 1512 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service determines if the OS-Applications, given by ApplID,
 *                  is allowed to use the IDs of a Task, Resource, Counter, Alarm or Schedule Table in API calls.
 * @param[in]       ApplID: OS-Application identifier.
 * @param[in]       ObjectType: Type of the following parameter.
 * @param[in]       ObjectID: The object to be examined.
 * @return          ObjectAccessType
 * @retval          ACCESS: the ApplID has access to the object.
 * @retval          NO_ACCESS: the ApplID is invalid; ObjectType is invalid; the ApplID does not have access to the
 * object;
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57468
 */
/* PRQA S 1753, 1501, 1506, 1512 ++ */ /* VL_Os_1753, VL_Os_1501, VL_Os_1506, VL_Os_1512 */
extern ObjectAccessType CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID);
/* PRQA S 1753, 1501, 1506, 1512 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service determines to which OS-Application a given Task, ISR, Counter, Alarm or Schedule Table
 * belongs.
 * @param[in]       ObjectType: Type of the following parameter.
 * @param[in]       ObjectID: The object to be examined.
 * @return          ApplicationType
 * @retval          INVALID_OSAPPLICATION: the ObjectID does not exist; ObjectType is invalid;
 *                  the object type is OBJECT_RESOURCE and the object is RES_SCHEDULER.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57473
 */
extern ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID);
/* PRQA S 1512, 1506 */ /* VL_Os_1512, VL_Os_1506 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This service terminates the OS-Application to which the calling Task/Category 2ISR/application
 * specific error hook belongs.
 * @param[in]       Application: The identifier of the OS-Application to be terminated.
 *                  If the caller belongs to <Application> the call results in a self termination.
 * @param[in]       RestartOption: Either RESTART for doing a restart of the OS-Application or NO_RESTART if
 * OS-Application shall not be restarted.
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
extern StatusType TerminateApplication(ApplicationType Application, RestartType RestartOption);
/* PRQA S 1753, 1501, 1512 */ /* VL_Os_1753, VL_Os_1501, VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#if (FALSE == RTE_USER_MODE)
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern AccessType CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern AccessType CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           A (trusted or non-trusted) OS-Application uses this service to call a trusted function.
 * @param[in]       FunctionIndex: Index of the function to be called.
 *                  If the caller belongs to <Application> the call results in a self termination.
 * @param[in]       FunctionParams: Pointer to the parameters for the function - specified by the FunctionIndex - to be
 * called. If no parameters are provided, a NULL pointer has to be passed.
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
extern StatusType
    CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#if (CFG_SPINLOCK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           ReleaseSpinlock releases a spinlock variable that was occupied before.
 *                  Before terminating a TASK all spinlock variables that have been occupied with GetSpinlock() shall be
 * released. Before calling WaitEVENT all Spinlocks shall be released.
 * @param[in]       SpinlockId: The value refers to the spinlock instance that shall be unlocked.
 * @return          StatusType
 * @retval          E_OK: In standard and extended status : No Error.
 * @retval          E_OS_ID: In extended status: The SpinlockId is invalid.
 * @retval          E_OS_STATE - In extended status: The Spinlock is not occupied by the TASK.
 * @retval          E_OS_ACCESS: In extended status: The spinlock cannot be accessed.
 * @retval          E_OS_NOFUNC: In extended status: Attempt to release a spinlock while another spinlock has to be
 * released before.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57587
 */
extern StatusType ReleaseSpinlock(SpinlockIdType SpinlockId); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           It is not supported to call this function after StartOS(). The function starts the core specified by
 * the parameter CoreID. The OUT parameter allows the caller to check whether the operation was successful or not. If a
 * core is started by means of this function StartOS shall be called on the core.
 * @param[in]       CoreID: Core identifier.
 * @param[out]      Status: Output status.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57573
 */
extern void StartCore(CoreIdType CoreID, StatusType* Status); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (FALSE == RTE_USER_MODE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The function starts the core specified by the parameter CoreID. It is allowed to call this function
 after StartOS(). The OUT parameter allows the caller to check whether the operation was successful or not. It is not
 allowed to call StartOS on cores activated by StartNonAutosarCore. Otherwise the behaviour is unspecified.
 * @param[in]       CoreID: Core identifier.
 * @param[out]      Status: Output status.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           -
 */
extern void StartNonAutosarCore(CoreIdType CoreID, StatusType* Status); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           This API allows the caller to select the idle mode action which is performed during idle time of the
 * OS (e.g. if no Task/ISR is active). It can be used to implement energy savings. The real idle modes are hardware
 * dependent and not standardized. The default idle mode on each core is IDLE_NO_HALT.
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The function returns the number of cores activated by the StartCore function. This function might be
 * a macro.
 * @return          uint32
 * @retval          Number of cores activated by the StartCore function (see below)
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57569
 */
extern uint32 GetNumberOfActivatedCores(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           The function returns a unique core identifier.
 * @return          CoreIdType: The return value is the unique ID of the core.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57571
 */
extern CoreIdType GetCoreID(void); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief           After this service the OS on all cores is shut down. Allowed at TASK level and ISR level and also
 * internally by the OS. The function will never return. The function will force other cores into a shutdown.
 * @param[in]       Error: needs to be a valid error code supported by the OS.
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-57604
 */
extern void ShutdownAllCores(StatusType Error); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_PERIPHERAL_MAX > 0U)

#define Os_ReadPeripheral8    ReadPeripheral8
#define Os_ReadPeripheral16   ReadPeripheral16
#define Os_ReadPeripheral32   ReadPeripheral32
#define Os_WritePeripheral8   WritePeripheral8
#define Os_WritePeripheral16  WritePeripheral16
#define Os_WritePeripheral32  WritePeripheral32
#define Os_ModifyPeripheral8  ModifyPeripheral8
#define Os_ModifyPeripheral16 ModifyPeripheral16
#define Os_ModifyPeripheral32 ModifyPeripheral32

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ReadPeripheral8(AreaIdType Area, const uint8* Address, uint8* ReadValue);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ReadPeripheral16(AreaIdType Area, const uint16* Address, uint16* ReadValue);
/* PRQA S 4152, 1512 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ReadPeripheral32(AreaIdType Area, const uint32* Address, uint32* ReadValue);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType WritePeripheral8(AreaIdType Area, uint8* Address, uint8 WriteValue);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType WritePeripheral16(AreaIdType Area, uint16* Address, uint16 WriteValue);
/* PRQA S 4152, 1512 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType WritePeripheral32(AreaIdType Area, uint32* Address, uint32 WriteValue);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ModifyPeripheral8(AreaIdType Area, uint8* Address, uint8 Clearmask, uint8 Setmask);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ModifyPeripheral16(AreaIdType Area, uint16* Address, uint16 Clearmask, uint16 Setmask);
/* PRQA S 4152, 1512 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
extern StatusType ModifyPeripheral32(AreaIdType Area, uint32* Address, uint32 Clearmask, uint32 Setmask);
/* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_BARRIER_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
