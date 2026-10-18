# LAMOST 外接板 MCU 固件说明

本文对应 `lamost_broad_MCU` 当前源码，说明外接板的职责、编译配置、开机板号显示、CAN 接收与定位指示框架，以及后续接入正式协议的方法。

更新时间：2026-10-08。

## 1. 工程用途与当前状态

本工程运行在带有 16 个单元接口的小型外接 PCB 上。每个接口对应一个单元，并配有本地 LED，用于现场插拔、寻找接口和位置确认。

外接板提供 CAN 接线通路；MCU 在总线上**只静默监听**，识别指定单元的定位指令后控制本板 LED。

外接板 MCU 不发送 CAN 数据帧、不发送应答或心跳、不做软件转发，也不承担主节点的通信调度工作。当前 CAN 控制器使用 `CAN_MODE_SILENT`，不是“正常模式下不调用发送函数”。静默配置与实际电气行为仍需通过实板验证。

### 1.1 已实现

| 功能 | 当前实现 |
|---|---|
| CAN 静默接收 | 250 kbit/s，SILENT 模式，配置同步到 `.ioc` |
| 接收缓存 | FIFO0 接收回调入队，主循环处理环形缓存 |
| 指令框架 | 接收、解析、目标匹配、LED 状态更新已接通 |
| 开机自检 | 十进制显示 PCB 板号：十位、间隔、个位，总计 5 秒 |
| 板号配置 | CMake 支持 `-DPCB_ID=N`，范围为 1～52 |
| 端口配置检查 | 检查端口表长度、非零 ID 范围及重复 ID |
| 跨板定位切换 | 合法新目标不属于本板时，清除本板旧指示 |
| 本地诊断 | 接收、解析、命中等计数可通过调试器查看 |
| 主机测试 | 使用 HAL 替身验证显示、解析、匹配和队列行为 |

### 1.2 尚未接入

- 现场实际使用的 CAN 亮灯指令格式。
- 每块 PCB 的 16 个端口与组内单元 ID 的真实对应关系。
- 实板烧录、实际亮灯效果、总线静默行为和连续运行验证。

**默认构建只做板号自检和 CAN 静默接收，不会依据未知报文点亮定位灯。** 台架示例协议默认关闭，端口单元表默认全 0。两项均配置完成后，示例定位流程才有可点亮的目标。

## 2. 两级 CAN 结构与编号

```text
上级 CAN
  ├─ 主节点 M01 ─ 本组下行 CAN ─ 两块外接 PCB ─ 单元 ID 1～32
  ├─ 主节点 M02 ─ 本组下行 CAN ─ 两块外接 PCB ─ 单元 ID 1～32
  ├─ ……
  └─ 主节点 M26 ─ 本组下行 CAN ─ 两块外接 PCB ─ 单元 ID 1～32
```

每个主节点下挂两块 PCB，每块 PCB 有 16 个接口。同一组最多有 32 个单元接口，不同主节点下的单元 ID 重复使用 1～32。

| 编号 | 范围 | 作用 |
|---|---|---|
| 主节点编号 | M01～M26 | 区分不同下行 CAN 分组 |
| PCB 全局编号 | PCB1～PCB52 | 标识物理板，选择本板固件配置和焦面映射 |
| 本板端口号 | 1～16 | 对应物理接口和 LED1～LED16 |
| LED 数组下标 | 0～15 | 软件中的 `just_led_control()` 参数 |
| 组内单元 ID | 1～32 | 实际定位命令指向的单元，不同组重复使用 |
| 焦面 A 编号 | A1～A726 | 全局焦面位置，供接线和维护核对 |

注意区分 CAN 帧的仲裁 ID 和报文中的目标单元 ID，两者不必相等。例如台架示例的仲裁 ID 固定为 `0x6F0`，目标单元 ID 放在数据区。

物理分组记录见 [main_node_pcb_groups.json](../Canvas/main_node_pcb_groups.json)。其中历史字段 `local_can_id=0/1` 仅作为组内两块 PCB 的排序索引保留，不代表外接板自身拥有 CAN 地址 0/1，也不能据此自动推断两块板各自连接哪些单元 ID。

## 3. 工程目录

```text
lamost_broad_MCU/
├── Core/
│   ├── Inc/                         CubeMX 生成的外设头文件
│   ├── Src/
│   │   ├── main.c                   初始化、启动、主循环
│   │   ├── can.c                    CAN 模式、位时序、滤波器、引脚
│   │   ├── gpio.c                   LED GPIO 初始化
│   │   ├── tim.c                    TIM1 周期与输出比较配置
│   │   ├── usart.c                  USART1 调试串口
│   │   └── stm32f3xx_it.c           中断入口
│   └── pcb_broad/
│       ├── just_led.c/.h           16 路 LED 驱动及全局焦面映射
│       ├── can_rx_buffer.c/.h      CAN 接收环形缓存
│       └── can_command.c/.h        板号显示、解析入口、定位控制
├── Drivers/                        STM32 HAL 与 CMSIS
├── cmake/                          ARM GCC 工具链及 CubeMX 构建文件
├── tests/
│   ├── stubs/                      主机测试使用的 HAL 替身
│   ├── test_can_command.c          功能回归测试
│   └── run_can_command_tests.py    本机 GCC 编译和运行入口
├── CMakeLists.txt                  源文件、板号、示例协议选项
├── CMakePresets.json               Debug / Release 预设
├── lamost_broad_MCU.ioc            CubeMX 配置
├── STM32F302XX_FLASH.ld            Flash / RAM 分配
└── readme.md                       本说明
```

`can_command.c/.h` 是本次新增的业务模块。原有接收缓存、GPIO 和 TIM1 软件 PWM 结构继续沿用，没有引入 RTOS。

## 4. 硬件与外设参数

以下参数来自当前工程配置，属于源码参数，不能替代实板测量。

| 项目 | 配置 |
|---|---|
| MCU | STM32F302K8Ux，UFQFPN32 工程配置 |
| Flash / RAM | 64 KiB / 16 KiB |
| 外部晶振 | 24 MHz |
| 主频 | HSE ×3，72 MHz |
| APB1 时钟 | 36 MHz |
| CAN | 经典 CAN，250 kbit/s，静默监听 |
| CAN 位时序 | Prescaler=8，BS1=12 TQ，BS2=5 TQ，SJW=1 TQ |
| CAN 滤波器 | 当前掩码全 0，接收全部帧，软件再识别目标指令 |
| CAN 接收路径 | FIFO0 → 接收中断 → 软件队列 |
| 自动 Bus-Off / 自动重传 | 当前均关闭；未新增应用层自动恢复流程 |
| USART1 | 115200 bit/s，已有 `printf` 重定向 |
| LED PWM | TIM1 中断驱动 GPIO，100 Hz，约 25% 占空比 |
| LED 闪烁能力 | 原驱动保留约 1 Hz 闪烁状态；当前定位指示使用常亮状态 |

CAN 波特率：

```text
36 MHz ÷ 8 ÷ (1 + 12 + 5) = 250 kbit/s
```

TIM1 配置为分频 `72-1`、周期 `10000-1`、比较值 `2500-1`。计数时钟约 1 MHz，每 10 ms 开始一个 LED PWM 周期，约 2.5 ms 后关闭输出。

### 4.1 引脚对应

| 功能 | 引脚 | 功能 | 引脚 |
|---|---|---|---|
| CAN RX | PA11 | CAN TX | PA12 |
| LED1 | PA0 | LED9 | PA8 |
| LED2 | PA1 | LED10 | PB0 |
| LED3 | PA2 | LED11 | PA15 |
| LED4 | PA3 | LED12 | PB3 |
| LED5 | PA4 | LED13 | PB4 |
| LED6 | PA5 | LED14 | PB5 |
| LED7 | PA6 | LED15 | PB6 |
| LED8 | PA7 | LED16 | PB7 |

当前 LED 驱动以 GPIO 高电平点亮、低电平熄灭。这里的“常亮”由现有软件 PWM 保持固定亮度，不代表 GPIO 始终输出高电平。

## 5. 编译与板号配置

### 5.1 工具要求

- CMake 3.22 或以上。
- Ninja。
- ARM GNU 工具链，`arm-none-eabi-gcc`、`arm-none-eabi-g++` 等位于 PATH。
- 主机回归测试另需 Python 3 和本机 GCC，例如 Windows 的 MinGW GCC。ARM GCC 不能直接生成可在 Windows 上运行的主机测试程序。

可先检查命令是否可用：

```powershell
cmake --version
ninja --version
arm-none-eabi-gcc --version
```

### 5.2 编译指定板号

下面命令在本 MCU 工程目录执行。例如构建 PCB23 的 Debug 固件：

```powershell
cmake --preset Debug -DPCB_ID=23 -DCAN_COMMAND_EXAMPLE_ENABLE=OFF
cmake --build --preset Debug
```

Release 示例：

```powershell
cmake --preset Release -DPCB_ID=23 -DCAN_COMMAND_EXAMPLE_ENABLE=OFF
cmake --build --preset Release
```

主要输出：

```text
build/Debug/lamost_broad_MCU.elf
build/Debug/lamost_broad_MCU.map
build/Release/lamost_broad_MCU.elf
build/Release/lamost_broad_MCU.map
```

当前构建主要生成 ELF 和 MAP，不会自动生成 BIN 或 HEX。如烧录工具需要这些格式，可自行转换：

```powershell
arm-none-eabi-objcopy -O ihex build/Debug/lamost_broad_MCU.elf build/Debug/lamost_broad_MCU.hex
arm-none-eabi-objcopy -O binary build/Debug/lamost_broad_MCU.elf build/Debug/lamost_broad_MCU.bin
```

### 5.3 避免板号与缓存混淆

`PCB_ID` 的范围是 1～52。新构建目录未指定时默认 PCB1；CMake 缓存已经设置过板号时，后续省略参数会继续使用缓存值，不会自动恢复为 1。配置输出会显示实际的 `PCB_ID` 和示例协议开关。

`CAN_COMMAND_EXAMPLE_ENABLE` 也会保存在 CMake 缓存中。示例开关的默认 OFF 只代表首次配置默认值；之前开启过时，需要显式传入 `-DCAN_COMMAND_EXAMPLE_ENABLE=OFF` 才会关闭。

批量制作不同板号固件时，建议分开构建目录，例如：

```powershell
cmake -S . -B build/PCB23-Release -G Ninja "-DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE=Release -DPCB_ID=23 -DCAN_COMMAND_EXAMPLE_ENABLE=OFF
cmake --build build/PCB23-Release
```

切换 PCB 编号不会自动改变组内单元端口表，后者需要按实际接线单独配置。

## 6. 开机十进制板号显示

每次上电或复位后按十进制分两次显示板号：

| 相对初始化时间 | LED 行为 |
|---|---|
| 0～不足 2 秒 | 点亮十位对应的 LED |
| 2～不足 3 秒 | 全部熄灭，分隔两个数字 |
| 3～不足 5 秒 | 点亮个位对应的 LED |
| 5 秒起 | 结束板号显示；无定位指令时全灭 |

数字 1～9 对应 LED1～LED9，**数字 0 对应 LED10**。显示板号时，每次只亮一盏灯，LED11～LED16 不参与数字显示。

| PCB 编号 | 十位显示 | 间隔 | 个位显示 |
|---|---|---|---|
| PCB1，显示 01 | LED10 | 全灭 1 秒 | LED1 |
| PCB7，显示 07 | LED10 | 全灭 1 秒 | LED7 |
| PCB10 | LED1 | 全灭 1 秒 | LED10 |
| PCB11 | LED1 | 全灭 1 秒 | LED1 |
| PCB23 | LED2 | 全灭 1 秒 | LED3 |
| PCB40 | LED4 | 全灭 1 秒 | LED10 |
| PCB52 | LED5 | 全灭 1 秒 | LED2 |

PCB11 等相同数字仍会被中间的熄灯阶段隔开，不会表现为持续亮同一盏灯。

相关参数位于 [can_command.h](Core/pcb_broad/can_command.h)：

```c
#define CAN_COMMAND_DIGIT_TIME_MS 2000U
#define CAN_COMMAND_DIGIT_GAP_MS 1000U
#define CAN_COMMAND_BOOT_TIME_MS (2U * CAN_COMMAND_DIGIT_TIME_MS + CAN_COMMAND_DIGIT_GAP_MS)
```

显示过程不使用 `HAL_Delay()`。主循环持续调用 `CAN_Command_Poll(HAL_GetTick())`，采用无符号时间差计算，能处理 tick 回绕。实际 GPIO 更新仍受主循环执行与 TIM1 PWM 周期影响。

### 6.1 开机显示期间收到定位指令

板号显示期间继续接收和解析 CAN，但不立即覆盖板号显示。程序保存最后一条合法定位或取消指令，5 秒结束后应用：

- 没有合法指令：全灭。
- 最后一条是本板单元定位：点亮对应端口。
- 最后一条目标属于其他板，或为取消定位：全灭。
- 无关报文和格式错误报文不覆盖之前保存的合法指令。

## 7. 端口与组内单元 ID 配置

### 7.1 两张表的区别

| 配置 | 数据意义 | 当前用途 |
|---|---|---|
| `pcb_map[16]` | 本板端口对应的全局 A 编号，0 表示保留 | 与 Canvas 和物理接线核对，按 `PCB_ID` 选表 |
| `CAN_COMMAND_PORT_UNIT_IDS` | 本板端口对应的组内单元 ID，1～32 | 收到定位指令后查找本板 LED |

`pcb_map` 不能直接替代组内单元表。当前定位框架不查询全局 A 映射，因此链接器可能移除未引用的 `pcb_map` 常量，这不表示源文件中的映射数据丢失。

### 7.2 配置位置与规则

在 [can_command.h](Core/pcb_broad/can_command.h) 中填写：

```c
#define CAN_COMMAND_PORT_UNIT_IDS {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
```

16 个条目依次对应物理端口 1～16，也就是 LED1～LED16。

- 条目为 1～32：该端口连接对应的组内单元。
- 条目为 0：该端口未配置或保留，不匹配任何定位目标。
- 初始化时检查所有非零 ID 必须有效且不重复。
- 全部为 0、存在重复非零 ID 或无效 ID 时，`map_ready=0`，不执行目标端口点亮。
- 至少配置一个有效 ID，且所有非零项均通过检查时，`map_ready=1`；允许其余端口保留为 0。
- 检查覆盖本板配置；两块板之间是否重复分配单元 ID，需要配置时共同核对。

仅用于台架演示的例子：若测试接线确实是端口 1～16 对应单元 1～16，可填写：

```c
#define CAN_COMMAND_PORT_UNIT_IDS {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}
```

这个例子不代表现场接线，也不表示某块 PCB 固定负责 1～16。正式固件必须按该物理板的实际接线配置。

当前端口表是编译期配置，不支持通过 CAN 写入，也不保存到 Flash。头文件中的 `#ifndef` 允许测试或外部构建用编译器宏覆盖，但尚未提供独立的 CMake 端口表缓存选项。

## 8. CAN 接收与指令处理流程

```text
CAN FIFO0 接收中断
  → HAL_CAN_RxFifo0MsgPendingCallback()
  → CAN_RX_Write()：写入原有环形队列

主循环
  → CAN_RX_Process()
  → CAN_RX_Read()：取出报文
  → CAN_Command_Process()
      → CAN_Command_Decode()：识别指令、校验字段
      → 查询本板端口单元表
      → 更新定位灯掩码
  → CAN_Command_Poll()：推进开机十进制显示

TIM1 中断
  → 根据 pcb_leds[].state 点亮或熄灭 GPIO，维持软件 PWM
```

### 8.1 接收缓存

缓存定义在 [can_rx_buffer.h](Core/pcb_broad/can_rx_buffer.h)，配置为 32 个槽位，保留一个槽位区分空和满，因此实际容量为 31 帧。

中断只负责收帧和入队，不在中断里做定位业务。缓存满时保留已有消息、丢弃新帧，增加 `overflow_count`。

`CAN_RX_Process()` 每轮最多尝试处理 32 帧，避免持续到达的报文使主循环一直停留在队列处理里，影响板号计时任务。

### 8.2 定位行为

| 收到的事件 | 处理方式 |
|---|---|
| 合法定位，本板找到对应单元 | 清除旧定位，仅点亮新目标端口 |
| 合法定位，本板没有该单元 | 清除本板旧定位，由同组另一块板自行匹配 |
| 合法取消定位 | 清除定位 |
| 重复定位同一单元 | 保持对应端口指示 |
| 无关 CAN 消息 | 忽略，不改变当前定位 |
| 格式错误的候选指令 | 计数后忽略，不改变当前定位 |
| 开机显示尚未结束 | 暂存最后一条合法指令，显示结束后应用 |

定位状态没有自动熄灭超时，持续到下一条合法定位、取消指令或复位。两块板应旁听同一组的定位指令，才能在切换到另一块板时同时清除旧灯。

## 9. 台架示例协议

**本节仅描述框架自带的测试协议，不是已确认的现场协议。** 默认构建时 `CAN_COMMAND_EXAMPLE_ENABLE=OFF`，解析入口返回“无关报文”。

### 9.1 启用条件

1. 根据测试接线填写本板 `CAN_COMMAND_PORT_UNIT_IDS`。
2. 选择正确的 `PCB_ID`。
3. 显式启用示例协议并重新编译。

```powershell
cmake --preset Debug -DPCB_ID=23 -DCAN_COMMAND_EXAMPLE_ENABLE=ON
cmake --build --preset Debug
```

只有打开示例开关、没有填写端口表时，合法报文会被解析和计数，但不能点亮本板目标端口。

### 9.2 帧格式

| 项目 | 要求 |
|---|---|
| 波特率 | 250 kbit/s |
| 帧类型 | 标准数据帧，非扩展帧、非远程帧 |
| 仲裁 ID | `0x6F0` |
| DLC | 必须为 4 |

| 数据字节 | 意义 | 取值 |
|---|---|---|
| Byte0 | 标记1 | `0x4C` |
| Byte1 | 标记2 | `0x44` |
| Byte2 | 操作 | `0x01` 定位，`0x00` 取消 |
| Byte3 | 目标单元 ID | 定位时 `0x01～0x20`；取消时必须 `0x00` |

数据示例均为十六进制：

| 操作 | ID | DLC | 数据 |
|---|---|---:|---|
| 定位单元3 | `0x6F0` | 4 | `4C 44 01 03` |
| 定位单元16 | `0x6F0` | 4 | `4C 44 01 10` |
| 定位单元23 | `0x6F0` | 4 | `4C 44 01 17` |
| 定位单元32 | `0x6F0` | 4 | `4C 44 01 20` |
| 取消定位 | `0x6F0` | 4 | `4C 44 00 00` |

例如使用端口 1～16 对应单元 1～16 的测试表时，单元3应点亮 LED3，单元23会清除本板旧灯，不会点亮 LED7 或其他接口。

### 9.3 接入正式协议

修改 [can_command.c](Core/pcb_broad/can_command.c) 中的 `CAN_Command_Decode()`。保持接口返回约定：

| 返回值 | 含义 |
|---:|---|
| 1 | 合法业务指令，`*unit_id=1～32` 表示定位，`*unit_id=0` 表示取消 |
| 0 | 无关报文 |
| -1 | 已识别为候选指令，但长度、标记、操作或 ID 不合法 |

正式协议适配需要明确仲裁 ID、标准/扩展帧、数据长度、命令字段、单元 ID 字段及取消指令规则。不要仅因报文 ID 的数值位于 1～32 就直接点灯。

还需同步调整示例开关和 `protocol_enabled` 的含义：目前 `protocol_enabled` 仅表示台架示例是否编译启用，不是正式协议已经接入的标志。

## 10. 本地调试状态

用 SWD 调试器观察全局变量 `can_command_status`：

| 字段 | 意义 |
|---|---|
| `received_count` | 进入指令处理函数的非空消息数，不是硬件 FIFO 收帧总数 |
| `command_count` | 已识别的合法定位和取消指令数 |
| `invalid_count` | 候选指令格式错误数 |
| `ignored_count` | 无关报文数；示例协议关闭时，消息均进入此项 |
| `local_count` | 合法定位成功匹配本板端口的次数 |
| `other_count` | 合法非零目标未点亮本板端口的次数，包含目标在其他板或本板映射未就绪的情况 |
| `led_mask` | 当前提交给 LED 驱动的掩码，bit0 对应 LED1；开机期间反映当前数字或熄灯间隔 |
| `pcb_id` | 当前编译板号 |
| `target_unit_id` | 最近合法指令的目标，0 表示取消或尚无定位 |
| `boot_display` | 1 表示开机显示阶段，0 表示显示结束 |
| `map_ready` | 本板端口表是否通过检查且至少包含一个有效单元 |
| `protocol_enabled` | 当前是否启用台架示例协议 |

接收队列状态位于主程序全局变量 `can_rx`：

| 字段 | 意义 |
|---|---|
| `write_index` / `read_index` | 队列写入/读取位置 |
| `overflow_count` | 软件环形缓存满而丢弃新帧的次数 |
| `count` | 调试用队列数量；中断和主循环均会更新，不用于业务判定 |

当前没有新增硬件 FIFO 溢出统计或 CAN 总线错误上报。USART1 虽然已有 `printf` 重定向，但新模块没有自动打印上述状态，也不会通过 CAN 回传。

## 11. 测试与实板检查

### 11.1 主机回归测试

在 MCU 工程目录执行：

```powershell
python tests/run_can_command_tests.py
```

或在仓库根目录执行：

```powershell
python lamost_broad_MCU/tests/run_can_command_tests.py
```

脚本用本机 GCC 编译实际的 `can_command.c` 和 `can_rx_buffer.c`，用测试替身代替 HAL 和 GPIO 输出函数。二进制放在临时目录，不会连接、烧录或控制硬件。

当前 8 组用例包括：

1. 示例协议关闭时不响应示例报文。
2. 本板单元命中与另一块板目标切换。
3. 单元 ID 32 的边界处理。
4. 端口表全部未配置。
5. 端口表存在重复单元 ID。
6. 端口表包含无效 ID。
7. PCB10 的个位为 0，使用 LED10。
8. PCB11 的两位相同，中间仍有熄灯间隔。

同时验证显示阶段的时间边界、tick 回绕、开机期间缓存定位、非法报文不改变指示、接收队列容量与回绕次序。

### 11.2 当前验证范围

- 当前十进制显示版本已完成 PCB1 Debug 和 PCB52 Release 编译。
- 8 组主机测试通过。
- 主机测试中的 1～16、17～32 端口表是测试输入，不是现场板卡配置。
- 尚未通过实板验证 GPIO 极性、板号显示、实际 CAN 报文接收、总线静默行为或长期运行。

### 11.3 实板首次验证建议

1. 确认编译输出中的 PCB 编号与待烧录物理板一致。
2. 通过 SWD 烧录对应 ELF；如使用 BIN，链接起始地址为 `0x08000000`。
3. 复位后观察十位、熄灯间隔、个位，确认显示与板号一致。
4. 使用已确认的测试接线表和示例开关，发送已知定位帧。
5. 检查本板命中、跨板目标切换、取消指令和非法帧处理。
6. 结合分析仪或示波器验证外接板没有发送、ACK 或错误标志输出。

外接板不会提供 ACK。台架上需有其他正常通信节点或正确配置的测试设备完成正常应答；只有发送器和本静默板时，发送器可能报告 ACK 错误，这不能单独证明外接板没有收到报文。

## 12. 常见问题

| 现象 | 优先检查 |
|---|---|
| 每块板开机都显示 01 | 是否仍使用默认 PCB1，是否复用了错误的构建目录或 ELF |
| 单位数板号先亮 LED10 | 正常，这是十位的 0，不是 LED10 故障 |
| PCB11 看起来像只亮一次 | 两次 LED1 之间应有 1 秒全灭，观察完整 5 秒流程 |
| 只显示板号，随后一直不响应 CAN | 示例开关是否关闭，正式协议是否尚未接入 |
| `received_count` 增长但不亮灯 | 查看 `protocol_enabled`、`map_ready`、`ignored_count` 和 `invalid_count` |
| `command_count` 增长但本板不亮 | 目标是否属于本板，端口表是否正确；`other_count` 也包括映射未就绪 |
| 定位后一直亮着 | 当前没有自动定位超时，需新定位、取消指令或复位 |
| 切换到另一块板后本板仍留灯 | 检查两板是否都收到合法新指令，另一板是否仍运行旧固件 |
| `overflow_count` 增长 | 主循环是否被调试打印或其他阻塞操作拖慢，报文量是否过大 |
| 收发工具报告无 ACK | 外接板为静默模式，需检查其他正常应答节点及测试工具配置 |
| 修改头文件示例开关后没有变化 | CMake 会定义该宏，以构建缓存中的 ON/OFF 为准 |
| 修改 Canvas 后 MCU 没变化 | 页面、映射导出、固件编译、烧录是独立步骤，需逐项确认 |

## 13. 与 Canvas 的关联

当前可视化入口为 [canvas_assembly.html](../Canvas/canvas_assembly.html)。

- 页面提供 A → L → PCB/端口的全局位置映射和主节点分组查看。
- [映射同步脚本](../Canvas/opt_front_to_middle.py)读取页面内置默认映射，生成 JSON 和固件中的全局 `pcb_map` 表。
- 同步脚本不会修改 `CAN_COMMAND_PORT_UNIT_IDS`，也不会把全局 A 编号转换成组内单元 ID。
- 浏览器中保存的映射 JSON 不会自动写回 HTML、固件或 MCU。
- 任何全局接线调整后，仍需核对本板实际端口单元表。

## 14. 维护范围

- 正式协议主要修改 `CAN_Command_Decode()`，端口配置主要修改 `CAN_COMMAND_PORT_UNIT_IDS`。
- 开机显示时长通过 `CAN_COMMAND_DIGIT_TIME_MS` 和 `CAN_COMMAND_DIGIT_GAP_MS` 调整。
- 保留 CAN 接收中断入队、主循环解析、定时器驱动 LED 的现有结构。
- `main.c` 的用户逻辑放在 CubeMX `USER CODE` 区域中。
- 当前 `.ioc` 已同步 SILENT 模式，重新生成代码后仍应核对 `can.c` 中的实际模式和 CAN 位时序。
- 不在外接板上增加 CAN 发送、应答、心跳或软件转发。

正式协议和现场端口对应确定后，应补充真实报文用例，并在 README 中替换“待接入”说明。编译通过、主机测试通过与实板验证通过应分别记录。
