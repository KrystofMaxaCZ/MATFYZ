#include "funshield.h"
const uint8_t FUN_SHIELD_BUTTONS_COUNT = 2;
const uint8_t FUN_SHIELD_LEDS_COUNT = 4;

class Timer {
  private:
    unsigned long startTime;

  public:
    Timer() : startTime(0) {}

    void start() {
      startTime = millis();
    }

    unsigned long getElapsedTime() {
      return millis() - startTime;
    }
};

class Led {
  private:
    uint8_t pin;

  public:
    Led(uint8_t pinNumber) : pin(pinNumber) {}

    void setupLed() {
      pinMode(pin, OUTPUT);
      turnOnOff(false);
    }

    void turnOnOff(bool state) {
      digitalWrite(pin, (state) ? ON : OFF);
    }
};


class Display {
  private:
    Led leds[FUN_SHIELD_LEDS_COUNT] = { Led(LED1_PIN), Led(LED2_PIN), Led(LED3_PIN), Led(LED4_PIN) };
  public:

    void setupLeds() {
      for (int ledIndex = 0; ledIndex < FUN_SHIELD_LEDS_COUNT; ledIndex++) {
        leds[ledIndex].setupLed();
      }
    }

    void displayNumber(int number) {
      for (int ledIndex = (FUN_SHIELD_LEDS_COUNT - 1); ledIndex >= 0; ledIndex--) {
        leds[ledIndex].turnOnOff(number & 1);
        number >>= 1;
      }
    }
};

class Button {
  private:
    uint8_t pin;
    int shownNumberIncrementBy;
    bool lastState;

    unsigned long lastActionTime;
    bool isAutoRepeating;

  public:
    Button(uint8_t pinNumber, int value) : pin(pinNumber), shownNumberIncrementBy(value), lastState(OFF), lastActionTime(0), isAutoRepeating(false) {}

    void setupButton() {
      pinMode(pin, INPUT_PULLUP);
    }

    bool getCurrentState() {
      bool state = (digitalRead(pin) == OFF);
      return state;
    }

    void setLastState(bool state) {
      lastState = state;
    }

    bool getLastState() {
      return lastState;
    }

    int getShownNumberIncrementBy() {
      return shownNumberIncrementBy;
    }

    void setAutoRepeat(bool state) {
      isAutoRepeating = state;
    }
    bool getAutoRepeat() {
      return isAutoRepeating;
    }

    void setLastActionTime(unsigned long time) {
      lastActionTime = time;
    }
    unsigned long getLastActionTime() {
      return lastActionTime;
    }
};


class CounterApp {
  private:
    Button buttons[FUN_SHIELD_BUTTONS_COUNT] = { Button(BUTTON1_PIN, 1), Button(BUTTON2_PIN, -1) };
    
    Timer timer;
    Display ledDisplay;
    int shownNumber;

    const unsigned long holdTimeToAuto = 1000;
    const unsigned long deltaTime = 300;

  public:
    CounterApp() : shownNumber(0) {}

    void begin() {
      for (int buttonIndex = 0; buttonIndex < FUN_SHIELD_BUTTONS_COUNT; buttonIndex++) {
        buttons[buttonIndex].setupButton();
      }
      ledDisplay.setupLeds();
      timer.start();
    }

    bool isTimeForNextIncrement(int buttonIndex) {
      unsigned long elapsedTime = timer.getElapsedTime() - buttons[buttonIndex].getLastActionTime();

      unsigned long waitTime = (buttons[buttonIndex].getAutoRepeat()) ? deltaTime : holdTimeToAuto;

      return (elapsedTime >= waitTime);
    }


    void updateNumber(int value) {
      int mask = (1 << FUN_SHIELD_LEDS_COUNT) - 1; 
      shownNumber = (shownNumber + value) & mask;
    }

    void makeAdditionWithAutoRepeat(int incrementValue, int buttonIndex, bool doAutoRepeat) {
      buttons[buttonIndex].setAutoRepeat(doAutoRepeat);
      buttons[buttonIndex].setLastActionTime(timer.getElapsedTime());
      updateNumber(incrementValue);
      ledDisplay.displayNumber(shownNumber);
    }

    void runApp() {
      for (int buttonIndex = 0; buttonIndex < FUN_SHIELD_BUTTONS_COUNT; buttonIndex++) {
        bool currentState = buttons[buttonIndex].getCurrentState();
        bool lastState = buttons[buttonIndex].getLastState();

        int incrementValue = buttons[buttonIndex].getShownNumberIncrementBy();
        
        // if first pressed - make an addition to shownNumber depending on shownNumberIncrementBy value of pressed button
        // setAutoRepeat to false bc we need to hold it longer time -> isTimeForNextPressed
        if (currentState == ON && lastState == OFF) {
          makeAdditionWithAutoRepeat(incrementValue, buttonIndex, false);
        // if holding button, check if it is time to make an addition
        } else if (currentState == ON && lastState == ON) {
          if (isTimeForNextIncrement(buttonIndex)) {
            // now we are holding button more than holdTimeToAuto so auto repeat is true
            makeAdditionWithAutoRepeat(incrementValue, buttonIndex, true); 
          }
        }
        buttons[buttonIndex].setLastState(currentState);
      }
    }
};


CounterApp counterApp;

void setup() {
  counterApp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  counterApp.runApp();
}
