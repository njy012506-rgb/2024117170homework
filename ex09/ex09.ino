#include <WiFi.h>
#include <WebServer.h>

// ==================== 硬件与网络配置 ====================
const char* ap_ssid = "ESP32-LAB170";
const char* ap_pass = "12345678";
const int TOUCH_PIN = 4;  // 触摸传感器引脚

WebServer server(80);

// ==================== 仪表盘网页生成 ====================
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>触摸传感器实时监控面板</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      background: linear-gradient(135deg, #0a0e27 0%, #1a1f3a 100%);
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
      color: #e2e8f0;
    }
    .dashboard {
      width: 100%;
      max-width: 480px;
      background: rgba(255, 255, 255, 0.05);
      backdrop-filter: blur(16px);
      border: 1px solid rgba(0, 212, 255, 0.2);
      border-radius: 20px;
      padding: 48px 36px;
      box-shadow: 0 20px 60px rgba(0, 0, 0, 0.4);
      text-align: center;
    }
    .title {
      font-size: 20px;
      font-weight: 600;
      color: #00d4ff;
      letter-spacing: 2px;
      margin-bottom: 40px;
      text-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
    }
    /* 核心数值显示 */
    .value-display {
      font-size: 96px;
      font-weight: 700;
      font-family: 'Courier New', Consolas, monospace;
      color: #00d4ff;
      text-shadow: 0 0 20px rgba(0, 212, 255, 0.6);
      margin-bottom: 24px;
      transition: color 0.2s ease;
    }
    .value-display.warning {
      color: #faad14;
      text-shadow: 0 0 20px rgba(250, 173, 20, 0.6);
    }
    .value-display.danger {
      color: #ff4d4f;
      text-shadow: 0 0 20px rgba(255, 77, 79, 0.6);
    }
    .unit {
      font-size: 18px;
      color: #94a3b8;
      margin-bottom: 32px;
      letter-spacing: 1px;
    }
    /* 强度进度条 */
    .progress-wrap {
      width: 100%;
      height: 12px;
      background: rgba(255, 255, 255, 0.1);
      border-radius: 6px;
      overflow: hidden;
      margin-bottom: 20px;
    }
    .progress-bar {
      height: 100%;
      width: 0%;
      background: linear-gradient(90deg, #00d4ff, #7c3aed);
      border-radius: 6px;
      transition: width 0.2s ease;
      box-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
    }
    /* 状态标签 */
    .status-tag {
      display: inline-block;
      padding: 6px 16px;
      border-radius: 20px;
      font-size: 14px;
      font-weight: 500;
      background: rgba(0, 212, 255, 0.15);
      color: #00d4ff;
      margin-bottom: 32px;
      border: 1px solid rgba(0, 212, 255, 0.3);
    }
    .status-tag.warning {
      background: rgba(250, 173, 20, 0.15);
      color: #faad14;
      border-color: rgba(250, 173, 20, 0.3);
    }
    .status-tag.danger {
      background: rgba(255, 77, 79, 0.15);
      color: #ff4d4f;
      border-color: rgba(255, 77, 79, 0.3);
    }
    .info {
      font-size: 12px;
      color: #64748b;
      line-height: 1.6;
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <div class="title">TOUCH SENSOR MONITOR</div>
    
    <div class="value-display" id="sensorValue">--</div>
    <div class="unit">原始采样值 (Raw Value)</div>
    
    <div class="progress-wrap">
      <div class="progress-bar" id="progressBar"></div>
    </div>
    
    <div class="status-tag" id="statusTag">检测中</div>
    
    <div class="info">
      数值越小 → 手指距离越近<br>
      刷新频率：5次/秒 · 实时同步
    </div>
  </div>

  <script>
    const valueEl = document.getElementById('sensorValue');
    const barEl = document.getElementById('progressBar');
    const tagEl = document.getElementById('statusTag');

    // 数值映射：将原始值 0-1000 映射为进度百分比 100%-0%
    function map(x, inMin, inMax, outMin, outMax) {
      return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    // 拉取传感器数据并更新界面
    function updateSensor() {
      fetch('/api/sensor')
        .then(res => res.text())
        .then(rawVal => {
          const val = parseInt(rawVal);
          
          // 1. 更新数值
          valueEl.textContent = val;
          
          // 2. 更新进度条（数值越小，触摸强度越高，进度越满）
          const percent = map(val, 0, 1000, 100, 0);
          const safePercent = Math.max(0, Math.min(100, percent));
          barEl.style.width = safePercent + '%';
          
          // 3. 更新状态与颜色
          valueEl.classList.remove('warning', 'danger');
          tagEl.classList.remove('warning', 'danger');
          
          if (val < 400) {
            // 已触摸
            valueEl.classList.add('danger');
            tagEl.classList.add('danger');
            tagEl.textContent = '已触摸';
          } else if (val < 600) {
            // 接近中
            valueEl.classList.add('warning');
            tagEl.classList.add('warning');
            tagEl.textContent = '接近中';
          } else {
            // 空闲
            tagEl.textContent = '空闲状态';
          }
        })
        .catch(err => console.error('数据读取失败:', err));
    }

    // 每 200ms 刷新一次，5Hz 刷新率
    setInterval(updateSensor, 200);
    // 页面加载立即执行一次
    updateSensor();
  </script>
</body>
</html>
  )rawliteral";
  return html;
}

// ==================== 路由处理函数 ====================
// 主页面：返回仪表盘HTML
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 传感器数据接口：返回触摸原始数值（纯文本）
void handleSensorApi() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);
  
  // 开启AP热点
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("传感器监控面板已启动");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.softAPIP()); // 默认 192.168.4.1

  // 注册路由
  server.on("/", handleRoot);
  server.on("/api/sensor", handleSensorApi);
  server.begin();
}

void loop() {
  server.handleClient();
}
