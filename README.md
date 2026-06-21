# 2024117170homework

嵌入式系统设计课程作业仓库

## 实验 ex01 - C 语言 Hello World

### 文件结构

```
ex01/
  hello.c    # Hello World + 键盘输入回显程序
```

### 功能说明

- 输出 "Hello, World!"
- 使用 `fgets` 函数接收键盘输入并回显

### 编译运行

```bash
# 编译
gcc ex01/hello.c -o ex01/hello -fexec-charset=UTF-8

# 运行
./ex01/hello
```

> **Windows 用户注意**：需要 `-fexec-charset=UTF-8` 参数确保中文正常显示。

### .gitignore

忽略编译产物：`*.exe`、`hello`、`*.o`

## 实验 lab01 - Arduino IDE 安装与 ESP32 环境配置

### 实验目的

1. 掌握 Arduino IDE 的下载与安装方法
2. 学会在 Arduino IDE 中添加 ESP32 开发板支持
3. 安装 USB 驱动（CH340）并识别串口
4. 编写并上传第一个 ESP32 测试程序，验证开发环境

### 实验准备

| 类别 | 内容 |
|------|------|
| 硬件 | 计算机、ESP32 开发板、USB-C 数据线 |
| 软件 | Arduino IDE、ESP32 开发板支持包、CH340 驱动 |

### 实验步骤

#### 1. Arduino IDE 安装

- 访问 [Arduino 官网](https://www.arduino.cc/en/software) 下载 Windows 安装包
- 运行安装程序，建议全选安装组件（关联 .ino 文件）
- 启动后可通过 File → Preferences 设置语言和字体

#### 2. 添加 ESP32 开发板支持

- File → Preferences → Additional Boards Manager URLs，添加：

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
https://dl.espressif.com/dl/package_esp32_index.json
```

- 工具 → 开发板 → 开发板管理器，搜索 "esp32"，安装 "esp32 by Espressif Systems"

#### 3. 安装 USB 驱动（CH340）

- 下载 CH340 驱动并安装
- 设备管理器中确认 COM 端口识别成功

#### 4. 配置开发环境

| 配置项 | 推荐值 |
|--------|--------|
| 开发板 | ESP32 Dev Module |
| Upload Speed | 921600 / 115200 |
| Flash Frequency | 80MHz |
| Flash Mode | QIO |
| Partition Scheme | Default 4MB with spiffs |

#### 5. 上传测试程序

```c
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.println("Hello ESP32!");
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
```

#### 6. 实验结果

- **串口监视器**：波特率 115200，输出 "Hello ESP32!"
- **硬件现象**：ESP32 板载 LED（GPIO2）每秒闪烁一次
- 实验结果截图见 `lab01/` 目录

### 常见问题

| 问题 | 解决方案 |
|------|----------|
| Failed to connect to ESP32 | 按住 BOOT 按钮再上传，出现 "Connecting..." 时松开 |
| Timed out waiting for packet header | 降低上传波特率 921600 → 115200 |
| 端口不显示 | 检查 USB 线是否支持数据传输，重新安装 CH340 驱动 |
| 编译错误 | 清理临时文件，重启 IDE，重新安装 ESP32 支持包 |

### 思考题

1. **串口监视器的波特率与 `Serial.begin(115200)` 不一致时会发生什么？为什么？**

   当串口监视器的波特率与代码中设置的波特率不一致时，串口监视器将显示乱码或完全不显示有效内容。

   **原因：** 波特率决定了发送端和接收端每秒钟传输的码元数量（即数据传输速率）。ESP32 以 115200 bps 的速率发送数据，而串口监视器以不同的速率（如 9600 bps）对信号进行采样解读。由于收发双方对每个比特的时间宽度理解不一致，采样点会错位，导致数据比特被错误解析，从而显示为乱码。只有收发双方的波特率完全一致，才能正确同步和解析串口数据。

2. **修改 `delay(1000)` 数值，观察 LED 闪烁频率如何变化？**

   `delay(1000)` 表示程序暂停执行 1000 毫秒（即 1 秒），因此 LED 亮 1 秒、灭 1 秒，闪烁周期为 2 秒，频率为 0.5 Hz。

   - **减小数值**（如 `delay(100)`）：LED 亮灭速度变快，闪烁频率增大。delay(100) 时工作周期 0.2s，频率 5 Hz。
   - **增大数值**（如 `delay(2000)`）：LED 亮灭速度变慢，闪烁频率减小。delay(2000) 时工作周期 4s，频率 0.25 Hz。

   闪烁频率公式：f = 1 / (2 × delay)，单位为 Hz（delay 单位为秒）。

### 文件结构

```
lab01/
  ex02.ino           # ESP32 LED 闪烁 + 串口输出测试程序
  *.png, *.jpg       # 实验结果截图
```

## 实验 lab02 - 基础 IO 实验：LED 闪烁控制

### 实验目标

掌握 ESP32 GPIO 输出控制基本方法，使用板载 LED（GPIO2）实现 LED 的点亮和熄灭控制。

### 实验器材

| 类别 | 内容 |
|------|------|
| 硬件 | ESP32 开发板 ×1、USB-C 数据线 ×1 |
| 软件 | 电脑（已安装 Arduino IDE） ×1 |

### 知识点

- GPIO 输出控制：学习如何设置引脚为输出模式
- `digitalWrite` 函数：掌握 HIGH 和 LOW 电平输出
- `delay` 函数：理解延时函数的作用和单位（毫秒）
- 串口通信：学习使用 `Serial` 进行调试输出

### 文件结构

```
lab02/
  lab02.ino    # 基础 LED 闪烁控制程序
```

## 实验 lab03 - LED 呼吸灯

### 实验目标

使用 ESP32 开发板自带的 LED（GPIO2），通过 PWM（脉冲宽度调制）技术实现呼吸灯效果。

### 实验器材

| 类别 | 内容 |
|------|------|
| 硬件 | ESP32 开发板 ×1、USB-C 数据线 ×1 |
| 软件 | 电脑（已安装 Arduino IDE） ×1 |

### PWM 原理

PWM (Pulse Width Modulation) 即脉冲宽度调制，通过快速开关数字信号来模拟模拟信号。

- **100% 占空比**：信号一直是高电平 (3.3V)，LED 最亮
- **50% 占空比**：一半时间高电平，一半时间低电平，LED 亮度约为一半
- **0% 占空比**：信号一直是低电平 (0V)，LED 熄灭

由于 LED 开关速度远超人眼识别频率，人眼看到的是平均亮度的变化。

### 文件结构

```
lab03/
  lab03.ino    # PWM 呼吸灯程序
```

## 作业 ex02 - millis() 函数实现 1Hz LED 闪烁

### 目标

使用 `millis()` 函数代替 `delay()`，控制 LED 以 1Hz 的频率稳定闪烁。

### 原理

`millis()` 返回系统运行毫秒数。通过比较两次状态切换的时间差，实现非阻塞延时控制。

- `BLINK_INTERVAL = 500`：每 500ms 切换一次状态，周期 1s，频率 1Hz

### 文件结构

```
ex02/
  ex02.ino              # millis() 1Hz LED 闪烁程序
  *.png, *.jpg          # 实验结果截图
```

## 作业 ex03 - millis() 函数实现 SOS 闪烁信号

### 目标

使用 `millis()` 函数控制 LED 产生 SOS（国际求救信号）闪烁信息。

### SOS 信号编码

- **S**：连续三次短闪（亮 200ms / 灭 200ms）
- **O**：连续三次长闪（亮 600ms / 灭 200ms）
- **S**：连续三次短闪（亮 200ms / 灭 200ms）
- 每次 SOS 播放完毕后有较长的熄灭停顿（2000ms）

### 实现方式

通过有限状态机（FSM）枚举 SOS 状态，在每个 `loop()` 迭代中检查 `millis()` 时间差，达到预定时间后切换到下一状态。

### 文件结构

```
ex03/
  ex03.ino              # SOS 闪烁信号程序
  *.png, *.jpg          # 实验结果截图
  *.mp4                 # 实验微视频
```

## 实验路线

- [x] ex01 - C 语言 Hello World
- [x] lab01 - Arduino IDE 安装与 ESP32 环境配置
- [x] lab02 - 基础 IO 实验：LED 闪烁控制
- [x] lab03 - LED 呼吸灯
- [x] ex02 - millis() 实现 1Hz LED 闪烁
- [x] ex03 - millis() 实现 SOS 闪烁信号
