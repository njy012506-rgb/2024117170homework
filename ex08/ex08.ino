#include <WiFi.h>
#include <WebServer.h>

// ==================== 硬件配置 ====================
const char* ap_ssid = "ESP32-LAB170";
const char* ap_pass = "12345678";
const int LED_PIN = 2;
const int TOUCH_PIN = 4;

#define THRESHOLD 500       // 触摸触发阈值
#define DEBOUNCE_MS 50      // 触摸消抖时间
#define BLINK_INTERVAL 100  // 报警闪烁间隔(ms)，数值越小闪得越快

// ==================== 系统状态变量 ====================
bool systemArmed = false;    // 系统布防状态：true=布防，false=撤防
bool alarmTriggered = false; // 报警触发状态：true=已触发报警
bool ledState = false;       // LED当前电平
unsigned long lastBlinkTime = 0;

// 触摸状态变量
bool lastTouchState = false;
unsigned long debounceTimer = 0;

WebServer server(80);

// ==================== 网页页面 ====================
String makePage() {
  String statusText;
  String statusColor;
  if (alarmTriggered) {
    statusText = "⚠️ 报警触发中";
    statusColor = "#ff4d4f";
  } else if (systemArmed) {
    statusText = "🛡️ 已布防";
    statusColor = "#faad14";
  } else {
    statusText = "✅ 已撤防";
    statusColor = "#52c41a";
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>安防主机控制终端</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: Arial, sans-serif;
      background: #f0f2f5;
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
    }
    .card {
      width: 100%;
      max-width: 420px;
      background: #fff;
      border-radius: 16px;
      padding: 40px 32px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.08);
      text-align: center;
    }
    h1 {
      font-size: 22px;
      color: #222;
      margin-bottom: 30px;
    }
    .status-box {
      padding: 24px;
      border-radius: 12px;
      margin-bottom: 30px;
      background: #f6f7f9;
    }
    .status-label {
      font-size: 14px;
      color: #666;
      margin-bottom: 8px;
    }
    .status-value {
      font-size: 24px;
      font-weight: bold;
      color: )rawliteral" + statusColor + R"rawliteral(;
    }
    .btn-group {
      display: flex;
      gap: 16px;
    }
    button {
      flex: 1;
      padding: 16px 0;
      font-size: 16px;
      font-weight: bold;
      border: none;
      border-radius: 10px;
      color: #fff;
      cursor: pointer;
      transition: all 0.2s;
    }
    button:hover { transform: translateY(-2px); opacity: 0.9; }
    .btn-arm { background: #faad14; }
    .btn-disarm { background: #52c41a; }
    .tip {
      margin-top: 24px;
      font-size: 12px;
      color: #999;
      line-height: 1.5;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>安防主机控制终端</h1>
    <div class="status-box">
      <div class="status-label">系统当前状态</div>
      <div class="status-value" id="statusText">)rawliteral" + statusText + R"rawliteral(</div>
    </div>
    <div class="btn-group">
      <button class="btn-arm" onclick="handleArm()">布防 Arm</button>
      <button class="btn-disarm" onclick="handleDisarm()">撤防 Disarm</button>
    </div>
    <div class="tip">
      布防后触碰触摸引脚将触发报警<br>
      仅网页端撤防可解除报警状态
    </div>
  </div>

  <script>
    // 布防
    function handleArm() {
      fetch('/arm').then(() => location.reload());
    }
    // 撤防
    function handleDisarm() {
      fetch('/disarm').then(() => location.reload());
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// ==================== 路由处理函数 ====================
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 布防接口
void handleArm() {
  systemArmed = true;
  alarmTriggered = false; // 布防时重置报警状态
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

// 撤防接口
void handleDisarm() {
  systemArmed = false;
  alarmTriggered = false; // 撤防同时解除报警
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 开启AP热点
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("安防主机启动");
  Serial.print("管理地址: http://");
  Serial.println(WiFi.softAPIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  // ==================== 1. 触摸检测（仅布防且未报警时生效） ====================
  if (systemArmed && !alarmTriggered) {
    bool currentTouch = (touchRead(TOUCH_PIN) < THRESHOLD);

    // 状态变化时重置消抖计时器
    if (currentTouch != lastTouchState) {
      debounceTimer = now;
      lastTouchState = currentTouch;
    }

    // 消抖稳定后，检测到按下边沿则触发报警
    if ((now - debounceTimer) > DEBOUNCE_MS) {
      if (currentTouch == true) {
        alarmTriggered = true;
        Serial.println("⚠️ 触发报警！");
      }
    }
  }

  // ==================== 2. 报警LED闪烁（非阻塞，不影响Web响应） ====================
  if (alarmTriggered) {
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }

  delay(1);
}
