const int LED_PIN = 2;

const unsigned long SHORT_FLASH = 200;
const unsigned long LONG_FLASH = 600;
const unsigned long FLASH_GAP = 200;
const unsigned long SOS_PAUSE = 2000;

enum SOSState {
  SHORT1_ON, SHORT1_OFF,
  SHORT2_ON, SHORT2_OFF,
  SHORT3_ON, SHORT3_OFF,
  LONG1_ON, LONG1_OFF,
  LONG2_ON, LONG2_OFF,
  LONG3_ON, LONG3_OFF,
  SHORT4_ON, SHORT4_OFF,
  SHORT5_ON, SHORT5_OFF,
  SHORT6_ON, SHORT6_OFF,
  SOS_PAUSE_STATE
};

SOSState currentState = SHORT1_ON;
unsigned long previousMillis = 0;
unsigned long currentDuration = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= currentDuration) {
    previousMillis = currentMillis;
    advanceState();
  }
}

void advanceState() {
  switch (currentState) {
    case SHORT1_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT1_OFF;
      break;
    case SHORT1_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SHORT2_ON;
      break;
    case SHORT2_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT2_OFF;
      break;
    case SHORT2_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SHORT3_ON;
      break;
    case SHORT3_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT3_OFF;
      break;
    case SHORT3_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = LONG1_ON;
      break;
    case LONG1_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = LONG_FLASH;
      currentState = LONG1_OFF;
      break;
    case LONG1_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = LONG2_ON;
      break;
    case LONG2_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = LONG_FLASH;
      currentState = LONG2_OFF;
      break;
    case LONG2_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = LONG3_ON;
      break;
    case LONG3_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = LONG_FLASH;
      currentState = LONG3_OFF;
      break;
    case LONG3_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SHORT4_ON;
      break;
    case SHORT4_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT4_OFF;
      break;
    case SHORT4_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SHORT5_ON;
      break;
    case SHORT5_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT5_OFF;
      break;
    case SHORT5_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SHORT6_ON;
      break;
    case SHORT6_ON:
      digitalWrite(LED_PIN, HIGH);
      currentDuration = SHORT_FLASH;
      currentState = SHORT6_OFF;
      break;
    case SHORT6_OFF:
      digitalWrite(LED_PIN, LOW);
      currentDuration = FLASH_GAP;
      currentState = SOS_PAUSE_STATE;
      break;
    case SOS_PAUSE_STATE:
      digitalWrite(LED_PIN, LOW);
      currentDuration = SOS_PAUSE;
      currentState = SHORT1_ON;
      break;
  }
}
