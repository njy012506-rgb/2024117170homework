#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500       // 触摸触发阈值
#define DEBOUNCE_MS 50      // 消抖时间，50ms足够过滤抖动

bool ledState = false;
bool lastTouchState = false;  // 上一次的触摸状态
unsigned long debounceTimer = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 读取当前触摸状态：true=触摸中，false=未触摸
  bool currentTouch = (touchRead(TOUCH_PIN) < THRESHOLD);
  unsigned long now = millis();

  // 状态变化时重置消抖计时器
  if (currentTouch != lastTouchState) {
    debounceTimer = now;
    lastTouchState = currentTouch;
  }

  // 状态稳定超过消抖时间，且检测到「按下边沿」（从未触摸→触摸）
  if ((now - debounceTimer) > DEBOUNCE_MS) {
    if (currentTouch == true) {
      // 有效按下，翻转LED状态
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      // 锁定：必须松开后才能再次触发，避免长按反复翻转
      while (touchRead(TOUCH_PIN) < THRESHOLD) {
        delay(10);
      }
    }
  }
  
  delay(10);
}
