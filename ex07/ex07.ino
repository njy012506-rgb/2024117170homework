#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-LAB170";
const char* ap_pass = "12345678";
const int LED_PIN = 2;

// PWM 配置
const int freq = 5000;          // PWM 频率 5000Hz
const int resolution = 8;       // 8位分辨率，亮度范围 0-255
int brightness = 0;             // 全局当前亮度值

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>LED亮度控制</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: Arial, sans-serif;
      background: #f0f3f8;
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
    .bright-num {
      font-size: 56px;
      font-weight: bold;
      color: #1677ff;
      margin: 20px 0;
      transition: color 0.2s;
    }
    .slider {
      width: 100%;
      height: 8px;
      border-radius: 4px;
      background: #e5e7eb;
      outline: none;
      -webkit-appearance: none;
      cursor: pointer;
      margin: 20px 0;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 22px;
      height: 22px;
      border-radius: 50%;
      background: #1677ff;
      cursor: pointer;
      box-shadow: 0 2px 6px rgba(22, 119, 255, 0.4);
    }
    .btn-group {
      margin-top: 30px;
      display: flex;
      gap: 16px;
      justify-content: center;
    }
    button {
      flex: 1;
      padding: 12px 0;
      font-size: 16px;
      border: none;
      border-radius: 8px;
      color: #fff;
      cursor: pointer;
      transition: opacity 0.2s;
    }
    button:hover { opacity: 0.85; }
    .btn-on { background: #52c41a; }
    .btn-off { background: #ff4d4f; }
  </style>
</head>
<body>
  <div class="card">
    <h1>LED 亮度无级调节</h1>
    <div class="bright-num" id="brightVal">)rawliteral" + String(brightness) + R"rawliteral(</div>
    <input type="range" min="0" max="255" value=")rawliteral" + String(brightness) + R"rawliteral("
           class="slider" id="brightSlider">
    <div class="btn-group">
      <button class="btn-on" onclick="setBrightness(255)">全亮</button>
      <button class="btn-off" onclick="setBrightness(0)">全灭</button>
    </div>
  </div>

  <script>
    const slider = document.getElementById('brightSlider');
    const valDisplay = document.getElementById('brightVal');
    let sendTimer = null;

    // 监听滑动条拖动
    slider.addEventListener('input', function() {
      const val = this.value;
      valDisplay.textContent = val;
      
      // 防抖：拖动停止 40ms 后再发送请求，避免频繁发包卡顿
      clearTimeout(sendTimer);
      sendTimer = setTimeout(() => {
        sendBrightness(val);
      }, 40);
    });

    // 发送亮度请求到 ESP32
    function sendBrightness(val) {
      fetch('/brightness?value=' + val);
    }

    // 快捷按钮设置亮度
    function setBrightness(val) {
      slider.value = val;
      valDisplay.textContent = val;
      sendBrightness(val);
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 处理亮度调节接口
void handleBrightness() {
  if (server.hasArg("value")) {
    int val = server.arg("value").toInt();
    // 数值范围校验，防止越界
    val = constrain(val, 0, 255);
    brightness = val;
    ledcWrite(LED_PIN, brightness);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  
  // 初始化 PWM 通道
  ledcAttach(LED_PIN, freq, resolution);
  ledcWrite(LED_PIN, brightness);
  
  // 开启 AP 热点
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP已开启");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.softAPIP()); // 默认 192.168.4.1

  // 注册网页路由
  server.on("/", handleRoot);
  server.on("/brightness", handleBrightness);
  server.begin();
}

void loop() {
  server.handleClient();
}
