const int LED_PIN = 2;

const unsigned long BLINK_INTERVAL = 500;
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
