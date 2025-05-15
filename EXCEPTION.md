Although this project is generally licensed under the LGPL-2.1, this license does not apply to applications that utilize the project's services solely through the unmodified interfaces defined in the header files provided specifically for that purpose. Such applications are considered standalone applications and do not constitute Derivative Works under the terms of the LGPL-2.1.

The header files listed in the ./HEADER.MD file define the interface of this project. Including these unmodified header files in another file is also considered normal use and does not constitute a "derived work" under LGPL-2.1.

Additionally, when using Basic Software (BSW) configuration tools to generate configuration code, some code from this project may be included. As a special exception, the copyright holders grant you permission to convey to a third party a Combined Work that links statically or dynamically to this project using "Basic Software (BSW) configuration tools" without adhering to the requirements set out in section 6a) of LGPL-2.1, provided that you comply with all other provisions of LGPL-2.1 and that the terms and conditions of the license(s) governing the Application are met.

Except for this special exception, all provisions of LGPL-2.1 continue to apply fully to this Library. If you modify this Library, you may choose to apply this exception to your version, but you are not obligated to do so. If you choose not to apply this exception, you should remove this exception statement from your version. Please note that this exception does not modify any license terms applicable to the Application, with which you must still comply.




**例外声明参考译文如下（因LGPL原文为英文，因此如存在中英不一致之处，仍需以英文为准）：**

虽然本项目通常是依据 LGPL-2.1 许可发布，但该许可不适用于仅通过项目中为此目的专门提供的头文件中定义的未修改接口来使用本项目服务的应用程序。此类应用程序被视为独立应用程序，不构成 LGPL-2.1 条款下的“衍生作品”。

./HEADER.MD 文件中列出的头文件定义了本项目的接口。在其他文件中包含这些未修改的头文件也被视为正常使用，不构成 LGPL-2.1 下的“衍生作品”。

此外，在使用基础软件（BSW）配置工具生成配置代码时，部分代码可能包含本项目的内容。作为特别例外，版权所有者授予您在符合 LGPL-2.1 所有其他条款的前提下，使用基础软件（BSW）配置工具将与本项目静态或动态链接的组合作品传达给第三方的权限，而无需遵守 LGPL-2.1 第 6a)的要求，前提是您还需满足LGPL-2.1的其他条款和条件。

除此特别例外外，LGPL-2.1 的所有条款仍完全适用于本库。如果您修改本库，可以选择将此例外条款应用于您的版本，但这并非强制要求。如果您选择不适用此例外条款，则应从您的版本中删除该例外声明。请注意，此例外条款不会修改适用于应用程序的任何许可条款，您仍需遵守这些条款。




**以下为对本例外声明的简要解读**

以上例外声明旨在通过解决项目调用和配置工具使用的问题以降低用户使用难度：

1.为减轻用户在以静态链接或其他难以修改的方式使用本项目时的许可证合规压力，若用户通过 ./HEADER.MD 文件中提供的头文件接口调用本项目代码，则无论采用何种调用方式，均不视为触发 LGPL-2.1 所规定的开源义务。

2.若用户使用基础软件（BSW）配置工具生成配置文件，所生成的配置文件可能包含本项目的部分代码。为确保此类代码嵌入不会影响用户的专有代码或其他受不同许可证约束的开源代码，从而降低用户的合规顾虑，本例外声明特别明确：凡由 BSW 配置工具生成的内容，即使其中包含本项目部分代码，亦不受 LGPL-2.1 6a)规定的开源义务影响。