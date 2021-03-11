#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#define EXTERNAL_BUTTON 12
#define CAPTOUCH T6 // D14
#define FADE_3 22
#define FADE_1 21

#define LED_BUILTIN 2
#define LED_BUILTIN_ON HIGH
int BUTTON_BUILTIN =  0;

#include <AceButton.h>
using namespace ace_button;

typedef struct command_struct {
  byte cmd;
  byte option;
  byte isIncreasing;
};

//Servo
#include <ESP32Servo.h>
Servo servo;
int servoPin = 13;
const int numOfSections = 7;
bool isIncreasing = true;
int currAngleArrayPosition = 0;
byte angles[numOfSections];
byte prevPos = 0;
bool prevIncreasing = false;

/// Led Settings ///
bool isBlinking = false;
bool readyToBlink = false;
unsigned long blinkTime;
int blinkDuration = 200;

// Touch settings and config
class CapacitiveConfig: public ButtonConfig {
  public:
    uint8_t _pin;
    uint16_t _threshold;
    CapacitiveConfig(uint8_t pin, uint16_t threshold) {
      _pin = pin;
      _threshold = threshold;
    }
    void setThreshold(uint16_t CapThreshold) {
      _threshold = CapThreshold;
    }
  protected:
    int readButton(uint8_t /*pin*/) override {
      uint16_t val = touchRead(_pin);
      return (val < _threshold) ? LOW : HIGH;
    }

};

int TOUCH_THRESHOLD = 60;
int TOUCH_HYSTERESIS = 20;
#define LONG_TOUCH 1500
CapacitiveConfig touchConfig(CAPTOUCH, TOUCH_THRESHOLD);
AceButton buttonTouch(&touchConfig);
bool isSelectingColour = false;

//Button Settings
AceButton buttonBuiltIn(BUTTON_BUILTIN);
AceButton buttonExternal(EXTERNAL_BUTTON);

void handleButtonEvent(AceButton*, uint8_t, uint8_t);
int longButtonPressDelay = 5000;

void setup() {
  Serial.begin(115200);
  initEspNow();
  generateAngles();
  setupServo();
  setupPins();
  setupCapacitiveTouch();
  
}

void loop() {
  ledHandler();
  buttonBuiltIn.check();
  buttonExternal.check();
  buttonTouch.check();
}
