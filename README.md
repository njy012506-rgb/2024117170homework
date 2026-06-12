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

## 实验 ex02 - Arduino IDE 安装与 ESP32 环境配置

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
- 实验结果截图见 `ex02/` 目录

### 常见问题

| 问题 | 解决方案 |
|------|----------|
| Failed to connect to ESP32 | 按住 BOOT 按钮再上传，出现 "Connecting..." 时松开 |
| Timed out waiting for packet header | 降低上传波特率 921600 → 115200 |
| 端口不显示 | 检查 USB 线是否支持数据传输，重新安装 CH340 驱动 |
| 编译错误 | 清理临时文件，重启 IDE，重新安装 ESP32 支持包 |

### 思考题

1. 串口监视器的波特率与 `Serial.begin(115200)` 不一致时会发生什么？为什么？
2. 修改 `delay(1000)` 数值，观察 LED 闪烁频率如何变化？

### 文件结构

```
ex02/
  ex02.ino           # ESP32 LED 闪烁 + 串口输出测试程序
  *.png, *.jpg       # 实验结果截图
```

### 实验路线

- [x] ex01 - C 语言 Hello World
- [x] ex02 - Arduino IDE 安装与 ESP32 环境配置
- [ ] ex03 - LED 闪烁控制（即将更新）
- [ ] ex04 - LED 呼吸灯
- [ ] ex05 - 触摸传感器
- [ ] ex06 - Web Server
