void setupPins() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_BUILTIN, INPUT);
  pinMode(EXTERNAL_BUTTON, INPUT_PULLUP);

  pinMode(27, OUTPUT);
  digitalWrite(27, LOW);

  pinMode(FADE_3, INPUT_PULLUP);
  pinMode(FADE_1, INPUT_PULLUP);

  ButtonConfig* buttonConfigBuiltIn = buttonBuiltIn.getButtonConfig();
  buttonConfigBuiltIn->setEventHandler(handleButtonEvent);
  buttonConfigBuiltIn->setFeature(ButtonConfig::kFeatureClick);
  buttonConfigBuiltIn->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfigBuiltIn->setLongPressDelay(LONG_TOUCH);

  touchConfig.setFeature(ButtonConfig::kFeatureClick);
  touchConfig.setFeature(ButtonConfig::kFeatureLongPress);
  touchConfig.setEventHandler(handleTouchEvent);
  touchConfig.setLongPressDelay(LONG_TOUCH);
}

//internal led functions

void blinkDevice() {
  if (readyToBlink == false) {
    readyToBlink = true;
  }
}

void ledHandler() {
  if (readyToBlink == true && isBlinking == false) {
    isBlinking = true;
    blinkTime = millis();
    digitalWrite(LED_BUILTIN, 1);
  }
  if (millis() - blinkTime > blinkDuration && isBlinking == true) {
    digitalWrite(LED_BUILTIN, 0);
    isBlinking = false;
    readyToBlink = false;
  }
}

void blinkOnConnect() {
  byte NUM_BLINKS = 3;
  for (byte i = 0; i < NUM_BLINKS; i++) {
    digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);
    delay(400);
  }
  delay(600);
}

// button functions
void handleButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.print("ID IS: ");
  Serial.println(button->getPin());
  switch (button->getPin()) {
    case 0:
      switch (eventType) {
        case AceButton::kEventPressed:
          break;
        case AceButton::kEventReleased:
          sendButtonPress();
          break;
        case AceButton::kEventLongPressed:
          break;
        case AceButton::kEventRepeatPressed:
          break;
      }
      break;
    case EXTERNAL_BUTTON:
      switch (eventType) {
        case AceButton::kEventPressed:
          Serial.println("TOUCH: pressed");
          prevPos = getPosition();
          prevIncreasing = isIncreasing;
          advanceAngle();
          setAngle(getAngle());
          sendServo();
          break;
        case AceButton::kEventLongPressed:
          Serial.println("TOUCH: Long pressed");

          break;
        case AceButton::kEventReleased:
          Serial.println("TOUCH: released");
          break;
        case AceButton::kEventClicked:
          Serial.println("TOUCH: clicked");
          break;
      }
      break;
  }
}

// button functions
void handleTouchEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println(button->getId());

  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("TOUCH: pressed");
      prevPos = getPosition();
      prevIncreasing = isIncreasing;
      advanceAngle();
      setAngle(getAngle());
      sendServo();
      break;
    case AceButton::kEventLongPressed:
      Serial.println("TOUCH: Long pressed");
      break;
    case AceButton::kEventReleased:
      Serial.println("TOUCH: released");
      break;
    case AceButton::kEventClicked:
      Serial.println("TOUCH: clicked");
      break;
  }
}

void setupCapacitiveTouch() {
  int touchAverage = 0;
  for (byte i = 0; i < 10; i++) {
    touchAverage = touchAverage + touchRead(CAPTOUCH);
    delay(100);
  }
  touchAverage = touchAverage / 10;
  TOUCH_THRESHOLD = touchAverage - TOUCH_HYSTERESIS;
  Serial.print("Touch threshold is:");
  Serial.println(TOUCH_THRESHOLD);
  touchConfig.setThreshold(TOUCH_THRESHOLD);
}
