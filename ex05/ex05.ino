#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500       // 触摸触发阈值，按实测值调整
#define DEBOUNCE_MS 50      // 触摸消抖时间
#define PWM_FREQ 5000       // PWM呼吸灯频率
#define PWM_RES 8           // PWM分辨率 0-255
#define SPEED_LEVELS 3      // 总速度档位数

// 触摸状态相关
bool lastTouchState = false;
unsigned long debounceTimer = 0;
bool waitForRelease = false;  // 等待松手标记，实现自锁

// 呼吸灯相关
int dutyCycle = 0;
int breathDir = 1;            // 呼吸方向：1=渐亮，-1=渐暗
unsigned long lastBreathTime = 0;

// 速度档位控制
int speedLevel = 1;           // 当前档位，默认1档
// 每个档位对应的步进间隔（ms），数值越小呼吸速度越快
const int breathIntervals[SPEED_LEVELS] = {10, 5, 2}; 

void setup() {
  Serial.begin(115200);
  // 初始化LED PWM通道
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(LED_PIN, dutyCycle);
  
  Serial.println("系统启动，当前呼吸档位：1");
}

void loop() {
  unsigned long now = millis();
  bool currentTouch = (touchRead(TOUCH_PIN) < THRESHOLD);

  // ========== 1. 触摸检测：消抖 + 边沿识别 + 松手自锁 ==========
  // 状态变化时重置消抖计时器
  if (currentTouch != lastTouchState) {
    debounceTimer = now;
    lastTouchState = currentTouch;
  }

  // 状态稳定超过消抖时间，判定为有效状态
  if ((now - debounceTimer) > DEBOUNCE_MS) {
    // 按下边沿：手指刚触摸，标记等待松手
    if (currentTouch == true && !waitForRelease) {
      waitForRelease = true;
    }
    
    // 松开边沿：手指完全离开，执行一次档位切换
    if (currentTouch == false && waitForRelease) {
      waitForRelease = false;
      
      // 循环切换档位：1→2→3→1
      speedLevel++;
      if (speedLevel > SPEED_LEVELS) {
        speedLevel = 1;
      }
      
      Serial.print("切换到档位：");
      Serial.println(speedLevel);
    }
  }

  // ========== 2. 非阻塞呼吸灯：根据当前档位自动调速 ==========
  int currentInterval = breathIntervals[speedLevel - 1];
  if (now - lastBreathTime >= currentInterval) {
    lastBreathTime = now;
    
    // 更新亮度
    dutyCycle += breathDir;
    // 到达亮度极值时反转呼吸方向
    if (dutyCycle >= 255) {
      dutyCycle = 255;
      breathDir = -1;
    } else if (dutyCycle <= 0) {
      dutyCycle = 0;
      breathDir = 1;
    }
    
    ledcWrite(LED_PIN, dutyCycle);
  }
  
  delay(1); // 轻微降载，不影响响应速度
}
