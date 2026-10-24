# 项目名称 Project Name

开源安全车控操作系统小满EasyXMen（简称“开源小满”）是面向嵌入式电子控制单元场景的实时安全操作系统，具有安全性、可靠性、实时性和可移植性等特点。  
Open Source Safety Vehicle Control Operating System EasyXMen ("EasyXMen") is a real-time safety operating system designed for embedded electronic control unit scenarios, featuring security, reliability, real-time performance, and portability.

# 项目特点 Project Features

- 具有广泛的可扩展性，完善的系统服务和调度机制，可快速适应在多种硬件平台上部署运行  
Highly scalable with comprehensive system services and scheduling mechanisms, enabling rapid adaptation for deployment and operation across multiple hardware platforms

- 支持CAN/CANFD、LIN、Ethernet多种总线通信，适用不同通信需求和场景  
Support CAN/CANFD, LIN, and Ethernet bus communication protocols, suitable for various communication requirements and scenarios

- 提供功能安全机制检测和阻止软件故障，实现系统的功能安全保护  
Provide functional safety mechanisms to detect and prevent software failures, ensuring functional safety protection for the system

- 采用架构模块化设计思路，便于软件的升级和维护  
Adopt a modular architectural design approach, facilitating software upgrades and maintenance

# 代码仓地址 Source Code Repository

https://atomgit.com/easyxmen/XMen

# 文档仓地址 Documentation Repository
https://atomgit.com/easyxmen/docs

- 快速预览（Quick Preview）：https://easyxmen.atomgit.com/


# 目录结构 Directory Structure

```
├── BSWCode                 # 模块静态代码(Module static code)
│   ├── Arti
│   ├── BndM
│   ├── BswM
│   ├── CanIf
│   ├── CanNm
│   ├── CanSM
│   ├── CanTSyn
│   ├── CanTp
│   ├── Com
│   ├── ComM
│   ├── CommonInclude       # 共用的头文件(Common header files)
│   ├── CryIf
│   ├── Crypto_62
│   ├── Csm
│   ├── Dcm
│   ├── Dem
│   ├── Det
│   ├── Dlt
│   ├── DoIP
│   ├── Ea
│   ├── EcuM
│   ├── Eep_62
│   ├── EthIf
│   ├── EthSM
│   ├── EthTSyn
│   ├── Fee_62
│   ├── FiM
│   ├── IpduM
│   ├── KeyM
│   ├── LdCom
│   ├── Libraries
│   │   ├── Bfx
│   │   ├── Crc
│   │   ├── E2E
│   │   └── IStdLib
│   ├── LinIf
│   ├── LinSM
│   ├── MemAcc_62
│   ├── MemIf
│   ├── Nm
│   ├── NvM
│   ├── PduR
│   ├── Sd
│   ├── SecOC
│   ├── SoAd
│   ├── SomeIpTp
│   ├── StbM
│   ├── TcpIp
│   ├── Tm
│   ├── UdpNm
│   ├── WdgIf
│   ├── WdgM
│   └── Xcp
├── Drivers                 # 板级外设芯片代码(Board-level peripheral chip code)
├── Examples                # 示例工程(Example projects)
├── RTOS                    # OS 代码(OS code)
│   ├── Extend              # OS 扩展代码(OS extension code)
│   ├── Kernel              # OS 内核代码(OS kernel code)
│   └── Portable            # OS移植代码(OS porting code)
│       ├── Mcu             # MCU相关的移植代码(MCU related porting code)
│       └── Processor       # 处理器架构相关移植代码(Processor architecture related porting code)
└── Test                    # bsw模块测试代码(bsw module test code)
    └── UT                  # 单元测试代码(Unit test code)
```

**补充说明 Supplementary Note**

- Portable/Mcu 下一个芯片一个目录，比如TC397,S32K148  
  One directory for each chip under Portable/Mcu, e.g. TC397, S32K148.

- Portable/Processor 一种芯片架构一个目录，比如arm,RISC-V  
  One directory for each chip architecture under Portable/Processor, e.g. arm, RISC-V

- Test/UT 下一个Bsw模块一个目录  
  One directory for each Bsw module under Test/UT

- Drivers 下放置板级外设芯片实现代码，例如 TJA1101  
  Save board-level peripheral chip implementation code under Drivers , e.g. TJA1101.

- Examples 下以一个硬件板型+具体功能组合成目录名，例如S32K148EVB_Q147_Demo  
  One directory with combined name of a hardware board model+ specific function, e.g. S32K148EVB_Q147_Demo


  - 工程对仓库中已有的实现代码进行引用  
    Project references to existing implementation code in the repository

  - 工程应实现自动化编译  
    Automatic compilation shall be implemented for engineering

  - 工程的内部目录供参考  
    Internal directory of the project for reference

  ```log
   ├── MCAL                    # MCAL 静态代码(MCAL static code)
   ├── Asw                     # 应用代码(Application code)
   |   ├── src                 # 应用源码(Application source code)
   |   └── gen                 # 应用配置生成(Application configuration generation)
   ├── PaltformFils            # 编译器等平台相关(Compiler and other) platform related
   └── ConfigProj              # 配置工程(Configuration project)
       ├── BswCfgProj          # bsw 配置工程(bsw configuration project)
       ├── SwcCfgProj          # swc 配置工程(swc configuration project)
       └── McalCfgProj         # MCAL 配置工程(MCAL configuration project)
  ```

# 运行条件 Operational Conditions

- 参考[快速上手指南](https://easyxmen.atomgit.com/)  
  Refer to [Quick Start Guide](https://easyxmen.atomgit.com/)

- 工具及工程申请地址  
  Tool and Project Application Links
  
  - [开源小满开发平台](https://easyxmen.com/welcome.html?channel=3)：面向全球 OEM、Tier1 及行业伙伴，提供专业工程化支撑，助力缩短开发周期、降低开发成本，加速向量产平滑过渡。（内含BSW及SWC工具、代码及配置工程等）  
   [EasyXMen Development Platform](https://easyxmen.com/welcome.html?channel=3): Designed for OEMs, Tier 1 suppliers, and industry partners worldwide, the platform provides professional engineering support to help shorten development cycles, reduce development costs, and accelerate a smooth transition to mass production. (Includes BSW and SWC tools, source code projects, configuration projects, and other resources.)

  - [开源小满学习版](https://easyxmen.com/welcome_rm.html?channel=3)：推荐场景为个人学习、院校/机构教学、初步功能评估。如需用于实际项目，建议申请[开源小满开发平台](https://easyxmen.com/welcome.html?channel=3)。（内含BSW工具、代码及配置工程等）  
   [EasyXMen Learning Edition](https://easyxmen.com/welcome_rm.html?channel=3): Recommended for individual learning, teaching at educational institutions and training organizations, and preliminary functional evaluation. For use in actual development projects, users are advised to apply for access to the [EasyXMen Development Platform](https://easyxmen.com/welcome.html?channel=3). (Includes BSW tools, source code projects, configuration projects, and other resources.)

# 开发贡献与指导 Development Contributions and Guidance

## commit

[commit规范（commit Specification）](https://atomgit.com/easyxmen/XMen/wiki/Commit规范)

## issue

- 在提交 Issue 之前，请先使用开源EasyXmen项目的搜索功能，查看是否已经有其他人提交了类似的问题。如果已经存在相关的 Issue，可以在该 Issue 下留言  
  Before submitting an issue, please use the search function of the open source EasyXmen project to check whether someone else has already submitted a similar issue. If there is already a related issue, you can leave a comment under that issue.

- 提交 issue 时选择合适的模板  
  Select the appropriate template when submitting an issue

- 提交 Issue 后，可以关注 Issue 的回复状态，了解问题的处理进度  
  After submitting an issue, you can follow the response status of the issue to understand the progress of the issue handling

- 在 Issue 交流过程中，若议题已经得到解决，或 Issue 并非项目相关问题，请及时关闭  
  During the issue communication process, if the issue has been resolved or the issue is not related to the project, please close it in a timely manner

## Pull Request

[PR提交指南（PR Submission Guidelines）](https://atomgit.com/easyxmen/XMen/wiki/PR提交指南)

# 开源许可 Open Source License

- EasyXMen项目整体基于LGPL V2.1开源，详见[LICENSE.txt](./LICENSE.txt)，并附有例外，例外详见[EXCEPTION.md](./EXCEPTION.md)   
  The EasyXMen project is open-source based on LGPL V2.1, see [LICENSE.txt](./LICENSE.txt) for details, with exceptions outlined in [EXCEPTION.md](./EXCEPTION.md).
