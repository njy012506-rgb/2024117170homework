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

## 作业 ex04 - 基于触摸传感器的自锁开关

### 目标

基于触摸传感器实现"自锁"开关——摸一下 LED 亮并保持，再摸一下 LED 灭。

### 关联基础

- 实验2：基础 IO（GPIO 输出控制）
- 实验4：触摸引脚（touchRead）

### 原理

引入布尔型状态变量 `ledState` 和边缘检测逻辑：
1. 通过 `touchRead()` 读取触摸引脚数值，低于阈值判定为触摸中
2. 消抖处理（50ms），过滤手抖误触发
3. 仅在上一次未触摸、当前被触摸的**按下瞬间**翻转 LED 状态
4. 翻转后进入锁定等待，直到手指松开才允许下次触发

### 文件结构

```
ex04/
  ex04.ino              # 触摸自锁开关程序
```

## 作业 ex05 - 多档位触摸调速呼吸灯

### 目标

结合 PWM 呼吸灯与触摸自锁逻辑，实现多档位速度可切换的呼吸灯。

### 关联基础

- 实验3：PWM 呼吸灯
- 实验4：触摸引脚

### 原理

1. 定义速度档位变量（1/2/3 档），每次触摸触发后循环切换
2. 消抖 + 边沿检测 + 松手自锁，确保每次触摸只切换一次
3. 非阻塞呼吸灯循环中根据当前档位改变 `delay()` 间隔，实现三种呼吸节奏

### 预期效果

LED 持续呈呼吸灯效果。每触摸一次引脚，呼吸节奏改变（缓慢 → 适中 → 急促 → 循环）。

### 文件结构

```
ex05/
  ex05.ino              # 多档位触摸调速呼吸灯程序
```

## 作业 ex06 - 警车双闪灯效（双通道 PWM）

### 目标

使用两个 LED 连接至不同引脚，通过双通道 PWM 实现反相渐变闪烁（警车双闪效果）。

### 关联基础

- 实验3：PWM 呼吸灯

### 原理

两个 LED 亮度呈反相关系：
- 灯 A 占空比从 0 → 255 时，灯 B 占空比从 255 → 0
- 通过 `ledcWrite(pin1, duty)` 和 `ledcWrite(pin2, 255 - duty)` 实现互补

### 预期效果

两个 LED 平滑交替渐变，一个变亮的同时另一个变暗，过渡柔和。

### 文件结构

```
ex06/
  ex06.ino              # 警车双闪灯效程序
```

## 作业 ex07 - Web 网页端无极调光器

### 目标

通过 Web 服务器 + HTML 滑动条，实现手机/电脑浏览器远程无级调节 LED 亮度。

### 关联基础

- 实验3：PWM 呼吸灯
- Web 服务器

### 原理

1. ESP32 开启 AP 热点，运行 WebServer
2. HTML 页面包含 `<input type="range" min="0" max="255">` 滑动条
3. JavaScript 监听滑动条变动，通过 `fetch()` 发送 GET 请求至 `/brightness?value=xxx`
4. ESP32 解析 URL 参数并赋值给 PWM 通道

### 预期效果

浏览器打开 ESP32 网页，拖动滑动条时 LED 亮度实时平滑变化。

### 文件结构

```
ex07/
  ex07.ino              # Web 无极调光器程序
```

## 作业 ex08 - 物联网安防报警器模拟

### 目标

模拟安防主机系统，网页端布防/撤防，布防状态下触摸引脚触发 LED 报警闪烁。

### 关联基础

- 实验2：基础 IO
- 实验4：触摸引脚
- Web 服务器

### 原理

1. 全局变量 `systemArmed` 记录布防/撤防状态
2. 网页提供"布防(Arm)"和"撤防(Disarm)"按钮
3. 布防状态下触摸引脚 → 触发 `alarmTriggered` → LED 高频闪烁
4. 报警锁定：手松开后闪烁不会停止
5. 仅网页点击"撤防"可解除报警并重置状态

### 预期效果

- 未布防时触摸引脚无反应
- 布防后触碰引脚 LED 立即高频闪烁并锁定
- 仅在网页端撤防后 LED 熄灭

### 文件结构

```
ex08/
  ex08.ino              # 物联网安防报警器程序
```

## 作业 ex09 - 实时传感器 Web 仪表盘

### 目标

实现数据采集（上报）+ 数据展示，通过 AJAX 技术实时显示触摸传感器模拟量数值。

### 关联基础

- 实验4：触摸引脚
- Web 服务器
- AJAX 异步数据请求

### 原理

1. ESP32 WebServer 提供 `/api/sensor` 接口，返回 `touchRead()` 原始数值
2. HTML 仪表盘页面使用 `setInterval` + `fetch()` 每 200ms 拉取一次数据
3. 页面显示实时跳动的数值、进度条、状态标签（空闲/接近中/已触摸）
4. 数值映射到进度条百分比，手指越近数值越小、进度条越满

### 预期效果

网页中央显示实时跳动的触摸数值。手指靠近引脚时数值变小，远离时数值恢复，实现仪器仪表数据实时监控。

### 文件结构

```
ex09/
  ex09.ino              # 实时传感器 Web 仪表盘程序
```

## 实验路线

- [x] ex01 - C 语言 Hello World
- [x] lab01 - Arduino IDE 安装与 ESP32 环境配置
- [x] lab02 - 基础 IO 实验：LED 闪烁控制
- [x] lab03 - LED 呼吸灯
- [x] lab04 - 触摸引脚实验
- [x] ex02 - millis() 实现 1Hz LED 闪烁
- [x] ex03 - millis() 实现 SOS 闪烁信号
- [x] ex04 - 触摸传感器自锁开关
- [x] ex05 - 多档位触摸调速呼吸灯
- [x] ex06 - 警车双闪灯效（双通道 PWM）
- [x] ex07 - Web 网页端无极调光器
- [x] ex08 - 物联网安防报警器模拟
- [x] ex09 - 实时传感器 Web 仪表盘
