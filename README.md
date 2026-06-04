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

### 实验路线

- [x] ex01 - C 语言 Hello World
- [ ] ex02 - LED 闪烁控制（即将更新）
- [ ] ex03 - LED 呼吸灯
- [ ] ex04 - 触摸传感器
- [ ] ex05 - Web Server
