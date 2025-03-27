# 项目名称

EasyXMen是面向嵌入式电子控制单元场景的实时安全操作系统，具有安全性、可靠性、实时性和可移植性等特点。

# 项目特点

具有广泛的可扩展性，完善的系统服务和调度机制，可快速适应在多种硬件平台上部署运行
支持CAN/CANFD、LIN、Ethernet多种总线通信，适用不同通信需求和场景
提供功能安全机制检测和阻止软件故障，实现系统的功能安全保护
采用架构模块化设计思路，便于软件的升级和维护

# 代码地址

https://atomgit.com/easyxmen/XMen

# 文档地址

仓库地址：https://atomgit.com/easyxmen/docs
网页浏览地址：https://easyxmen.atomgit.net/docs/

# 目录结构

```
├── BSWCode                 # 模块静态代码
│   ├── CommonInclude       # 共用的头文件
│   ├── Communication       # 通信模块代码
│   ├── Crypto              # 加密模块代码
│   ├── Libraries           # Lib库代码
│   ├── Memory              # 存储模块代码
│   └── SystemServices      # 系统服务模块代码
├── Drivers                 # 板级外设芯片代码
├── Examples                # 示例工程
├── RTE                     # RTE代码
│   └── StaticCode          # RTE 静态代码
├── RTOS                    # OS 代码
│   ├── Extend              # OS 扩展代码
│   ├── Kernel              # OS 内核代码
│   └── Portable            # OS移植代码
│       ├── Mcu             # MCU相关的移植代码
│       └── Processor       # 处理器架构相关移植代码
└── Test                    # bsw模块测试代码
    └── UT                  # 单元测试代码
```

补充说明

- Portable/Mcu 下一个芯片一个目录，比如TC397,S32K148
- Portable/Processor 一种芯片架构一个目录，比如arm,RISC-V
- Test/UT 下一个Bsw模块一个目录
- Drivers 下放置板级外设芯片实现代码，例如 TJA1101
- Examples 下以一个硬件板型+具体功能组合成目录名，例如S32K148EVB_Q147_Demo

  - 工程对仓库中已有的实现代码进行引用
  - 工程应实现自动化编译
  - 工程的内部目录供参考

  ```log
   ├── MCAL                    # MCAL 静态代码
   ├── Asw                     # 应用代码
   |   ├── src                 # 应用源码
   |   └── gen                 # 应用配置生成
   ├── PaltformFils            # 编译器等平台相关
   └── ConfigProj              # 配置工程
       ├── BswCfgProj          # bsw 配置工程
       ├── SwcCfgProj          # swc 配置工程
       └── McalCfgProj         # MCAL 配置工程
  ```

# 运行条件

协议栈静态代码需要结合开发工具进行使用，可通过[官方入口](https://register.easyxmen.com/welcome.html?channel=3)进行开发工具链免费申请。 各模块配置、工具使用方法，可参考模块用户手册； 基于特定芯片平台Demo工程，可通过配置工具获取使用。

## 项目状态

EasyXMen目前支持NXP S32K148平台



# 开发贡献与指导

## commit

[commit规范](https://atomgit.com/easyxmen/XMen/wiki/Commit规范)

## issue

- 在提交 Issue 之前，请先使用开源EasyXmen项目的搜索功能，查看是否已经有其他人提交了类似的问题。如果已经存在相关的 Issue，可以在该 Issue 下留言

- 提交issue时选择合适的模板
- 提交 Issue 后，可以关注 Issue 的回复状态，了解问题的处理进度
- 在 Issue 交流过程中，若议题已经得到解决，或 Issue 并非项目相关问题，请及时关闭

## Pull Request

[PR提交指南](https://atomgit.com/easyxmen/XMen/wiki/PR提交指南)

# 开源许可

EasyXMen项目整体基于LGPL V2.1开源，详见[LICENSE.txt](./LICENSE.txt)，并附有例外，例外详见[EXCEPTION.txt](./EXCEPTION.txt)

同时，EasyXMen也提供商业许可协议（Commercial License)，详见[https://EasyXMen.com/xy/reference/permissions.html](https://EasyXMen.com/xy/reference/permissions.html)

_EasyXMen提供商业许可是为了给用户提供更加丰富的许可选项，并不代表LGPL V2.1开源许可证禁止商业利用，您完全可以在遵循LGPL V2.1和我们的例外声明的前提下自由地使用本开源项目，具体内容请详见相关许可协议文本_

# 贡献者

| 用户名               | 邮箱                         | 贡献的描述               |
| -------------------- | ---------------------------- | ------------------------ |
| *示例：hongkunzhang* | *hongkunzhang@i-soft.com.cn* | *同步代码，仓库规则制定* |
|                      |                              |                          |
|                      |                              |                          |

