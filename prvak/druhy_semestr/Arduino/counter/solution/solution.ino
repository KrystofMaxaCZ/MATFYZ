#include "funshield.h"

const uint8_t GLYPHS_PINS_COUNT = 3;
const uint8_t BUTTONS_COUNT = 3;
const uint8_t DIGITS_COUNT = 4;

enum ButtonActionType {
  NONE,
  INCREMENT,
  DECREMENT,
  MOVE_POS
};


class Timer {
  private:
    unsigned long lastPressedTime = 0;
    const unsigned long bouncingDelay = 50;

  public:
    // returns true if the difference between lastPressedTime and currentTime is bigger than bouncingDelay to track bounces
    bool isReady() {
      unsigned long currentTime = millis();
      return (currentTime - lastPressedTime >= bouncingDelay);
    }

    void reset() {
      lastPressedTime = millis();
    }
};

class Button {
  private:
    uint8_t pin;              // used hw pin for this button
    ButtonActionType action;  // what action should be done after hitting the button
    bool lastState;           // we will use that to track the button oscillation problem
    Timer timer;

    bool isPressed() {
      return (digitalRead(pin) == OFF);
    }

  public:
    Button(uint8_t pinValue, ButtonActionType actionValue) : pin(pinValue), action(actionValue), lastState(OFF) {}

    void setupButton() {
      pinMode(pin, INPUT);
    }

    // returns true if the button was pressed, handles oscillation problem
    bool wasPressed() {
      bool currentState = isPressed();
      bool wasPressed = false;

      // if we pressed buttons - check debouncing - button was pressed
      if (( currentState == ON && lastState == OFF)) {
        if (timer.isReady()) {
          wasPressed = true;
          timer.reset(); // sets lastPressedTime to now
        }
      }

      lastState = currentState;

      return wasPressed;
    }

    ButtonActionType getAction() {
      return action;
    }
};

// handles pressing buttons and what action should be done
class ButtonHandler {
  private:
    Button buttons[BUTTONS_COUNT] = {{BUTTON1_PIN, INCREMENT}, {BUTTON2_PIN, DECREMENT}, {BUTTON3_PIN, MOVE_POS}};

  public:

    void setupButtons() {
      for (int buttonIndex = 0; buttonIndex < BUTTONS_COUNT; buttonIndex++) {
        buttons[buttonIndex].setupButton();
      }
    }

    // checking if button was pressed - passing what action should be done to Counter
    ButtonActionType handleButtons() {
      for (int buttonIndex = 0; buttonIndex < BUTTONS_COUNT; buttonIndex++) {
        if (buttons[buttonIndex].wasPressed()) {
          return buttons[buttonIndex].getAction();
        }
      }

      return NONE;
    }
};


// handles counting logic
class Counter {
  private:
    ButtonHandler buttonHandler; // handles low-level logic of pressing buttons
    int numberToDisplay; // the shared number we are showing
    uint8_t decimalPosition; // what digit should be displayed
    int valueToAdd = 1;

    int maxValue;
    const uint8_t BASE = 10;

    void setMaxValue() {
      long maxVal = 1;

      for (int i = 0; i < DIGITS_COUNT; i++) {
        maxVal *= BASE;
      }

      maxValue = maxVal;
    }

    void incrementNumber() {
      numberToDisplay = (numberToDisplay + valueToAdd) % maxValue;
    }

    void decrementNumber() {
      numberToDisplay = (numberToDisplay - valueToAdd + maxValue) % maxValue;
    }

    void incrementDecPosition() {
      decimalPosition = (decimalPosition + 1) % DIGITS_COUNT;
      valueToAdd *= BASE;

      // reset after overflow
      if (valueToAdd >= maxValue) {
        valueToAdd = 1;
      }
    }

    void handleAction(ButtonActionType action) {
      switch (action) {
        case INCREMENT:
          incrementNumber();
          break;
        case DECREMENT:
          decrementNumber();
          break;
        case MOVE_POS:
          incrementDecPosition();
          break;
        case NONE:
          break;
      }
    }

  public:
    Counter() : numberToDisplay(0), decimalPosition(0) {
      setMaxValue();
    }

    void setupButtons() {
      buttonHandler.setupButtons();
    }

    void handleCounting() {
      ButtonActionType btnAction = buttonHandler.handleButtons();
      handleAction(btnAction);
    }

    // returns one digit on concrete digitPosition
    int getDigitAtDecPos() {
      return (numberToDisplay / valueToAdd) % BASE;
    }

    uint8_t getDecimalPosition() {
      return decimalPosition;
    }

};

class Display {
  private:
    const int glyphsPins[GLYPHS_PINS_COUNT] = {SEG7_LATCH_PIN, SEG7_CLOCK_PIN, SEG7_DATA_PIN};

    // format position for writeGlyphBitmnask func, 0x01 / 0x02 / 0x04 / 0x08
    byte formatPosBitmask(uint8_t digitPosition) {
      return 1 << ((DIGITS_COUNT - 1) - digitPosition);
    }

    // function given us by master, it writes LOW HIGH on dispaly
    void writeGlyphBitmask(byte glyph, byte pos_bitmask) {
      digitalWrite(SEG7_LATCH_PIN, LOW);
      shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, glyph);
      shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, pos_bitmask);
      digitalWrite( SEG7_LATCH_PIN, HIGH);
    }

  public:
    // will setup pins
    void setupGlyphs() {
      for (int glyphsPinsIndex = 0; glyphsPinsIndex < GLYPHS_PINS_COUNT; glyphsPinsIndex++) {
        pinMode(glyphsPins[glyphsPinsIndex], OUTPUT);
      }
    }

    // displays digit on the digital screen
    void displayDigit(uint8_t digitValue, uint8_t digitPosition) {
      byte digit = SEG7_DIGIT_GLYPHS[digitValue];
      byte positionBitmask = formatPosBitmask(digitPosition);

      writeGlyphBitmask(digit, positionBitmask);
    }
};

class CounterApp {
  private:
    Display digitDisplay;
    Counter counter;

  public:
    void begin() {
      counter.setupButtons();
      digitDisplay.setupGlyphs();
    }
    void runApp() {
      counter.handleCounting();

      uint8_t digitToDisplay = counter.getDigitAtDecPos();
      uint8_t decimalPosition = counter.getDecimalPosition();

      digitDisplay.displayDigit(digitToDisplay, decimalPosition);
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
