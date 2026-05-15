#include "funshield.h"

const uint8_t GLYPHS_PINS_COUNT = 3;
const uint8_t BUTTONS_COUNT = 3;
const uint8_t DIGITS_COUNT = 4;
const uint8_t DECIMAL = 10;

enum ButtonActionType {
  NONE,
  START_STOP,
  LAP_RESUME,
  RESET
};

enum StopsActionType {
  STOPPED,
  RUNNING,
  LAPPED,
  RESETED
};


class Timer {
private:
  unsigned long startMillis;

public:
  Timer() {
    startMillis = millis();
  }

  unsigned long getElapsedTime() {
    return millis() - startMillis;
  }

  bool hasPassed(unsigned long interval) {
    return getElapsedTime() >= interval;
  }

  void reset() {
    startMillis = millis();
  }

  unsigned long getTime() {
    return startMillis;
  }
};

class Button {
private:
  uint8_t pin;  // used hw pin for this button
  const unsigned long bouncingDelay = 50;
  bool lastState = false;
  ButtonActionType action;  // what action should be done after hitting the button
  Timer debounceTimer;

  bool isPressed() {
    return (digitalRead(pin) == ON);
  }

public:
  Button(uint8_t pinValue, ButtonActionType actionValue)
    : pin(pinValue), action(actionValue) {}

  void setupButton() {
    pinMode(pin, INPUT);
  }

  // returns true if the button was pressed, handles oscillation problem
  bool wasPressed() {
    bool currentState = isPressed();
    bool triggered = false;

    if (debounceTimer.hasPassed(bouncingDelay) && (currentState != lastState)) {
      debounceTimer.reset();
      lastState = currentState;

      if (currentState == true) {
        triggered = true;
      }
    }
    return triggered;
  }

  ButtonActionType getAction() {
    return action;
  }
};

// handles pressing buttons and what action should be done
class ButtonHandler {
private:
  Button buttons[BUTTONS_COUNT] = { { BUTTON1_PIN, START_STOP }, { BUTTON2_PIN, LAP_RESUME }, { BUTTON3_PIN, RESET } };

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



// returns what action should stops do based on what button was clicked and what current action is
// current action is passed from class Stops
class StopsActionHandler {
private:
  ButtonHandler buttonHandler;
public:
  void setup() {
    buttonHandler.setupButtons();
  }
  StopsActionType handleStopsAction(StopsActionType currentAction) {
    ButtonActionType btnAction = buttonHandler.handleButtons();

    switch (btnAction) {
      case START_STOP:
        if (currentAction == STOPPED) return RUNNING;
        if (currentAction == RUNNING) return STOPPED;
        return currentAction;
      case LAP_RESUME:
        if (currentAction == RUNNING) return LAPPED;
        if (currentAction == LAPPED) return RUNNING;
        return currentAction;
      case RESET:
        if (currentAction == STOPPED){ 
          return RESETED; 
        }else{
          return currentAction;
        }
      case NONE:
        return currentAction;
      default:
        return currentAction;
    }
  }
};


class Stops {
private:
  unsigned long time = 0;
  unsigned long stopedTime = 0;
  unsigned long lapTime = 0;

  StopsActionType currentAction = STOPPED;  // first state will be presented as STOPPED
  StopsActionHandler stopsActionHandler;
  Timer timer;


  // FUNCTIONS FOR ACTIONS
  void startTimer() {
    // timer.start();
    time = timer.getElapsedTime() + stopedTime;
  }
  void stopTimer() {
    stopedTime = time;
  }
  void makeLap() {
    lapTime = time;
  }
  void resetTimer() {
    timer.reset();
    stopedTime = 0;
    time = 0;
  }

public:
  void setup() {
    stopsActionHandler.setup();
  }
  void handleAction() {

    StopsActionType previousAction = currentAction;
    currentAction = stopsActionHandler.handleStopsAction(currentAction);

    if (previousAction == STOPPED && currentAction == RUNNING) {
      timer.reset();
    }
    if (previousAction == RUNNING && currentAction == STOPPED) {
      stopTimer();
    }
    // now we should do smthing based on currentAction
    // we will use switch and call functions
    switch (currentAction) {
      case RUNNING:
        startTimer();
        break;
      case LAPPED:
        if (previousAction == RUNNING){ makeLap(); }
        break;
      case RESETED:
        resetTimer();
        currentAction = STOPPED;
        break;
      case STOPPED:
        break;
    }
  }

  void oneStep() {
    handleAction();
  }

  unsigned long getTime() {
    return (currentAction == LAPPED) ? lapTime : time;
  }
};

class Display {
private:
  const int glyphsPins[GLYPHS_PINS_COUNT] = { SEG7_LATCH_PIN, SEG7_CLOCK_PIN, SEG7_DATA_PIN };
  uint8_t currentPos = 0;

  const byte BLANK_GLYPH = 0xFF;
  const uint8_t DECIMAL_POINT_BIT = 0x80;
  const uint8_t DECIMAL_POINT_POS = 1;
  const unsigned long MILLIS_TO_TENTHS = 100;


  // format position for writeGlyphBitmnask func, 0x01 / 0x02 / 0x04 / 0x08
  byte formatPosBitmask(uint8_t digitPosition) {
    return 1 << ((DIGITS_COUNT - 1) - digitPosition);
  }

  // function given us by master, it writes LOW HIGH on dispaly
  void writeGlyphBitmask(byte glyph, byte pos_bitmask) {
    digitalWrite(SEG7_LATCH_PIN, LOW);
    shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, glyph);
    shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, pos_bitmask);
    digitalWrite(SEG7_LATCH_PIN, HIGH);
  }

  void displayDigit(uint8_t digitValue, uint8_t pos, bool decimalPoint) {
    byte glyph = SEG7_DIGIT_GLYPHS[digitValue];
    if (decimalPoint) {
      byte mask = ~DECIMAL_POINT_BIT;
      glyph &= mask;
    }

    byte posMask = formatPosBitmask(pos);
    writeGlyphBitmask(glyph, posMask);
  }

  uint8_t getDigitAt(unsigned long number, uint8_t pos) {
    for (int i = 0; i < pos; i++) {
      number = number / DECIMAL;
    }
    return number % DECIMAL;
  }

  bool isLeadingZero(uint8_t pos, unsigned long number) {
    for (int i = 0; i < pos; i++) {
      number = number / DECIMAL;
    }
    return (pos > 1 && number == 0);
  }

public:
  // will setup pins
  void setupGlyphs() {
    for (int glyphsPinsIndex = 0; glyphsPinsIndex < GLYPHS_PINS_COUNT; glyphsPinsIndex++) {
      pinMode(glyphsPins[glyphsPinsIndex], OUTPUT);
    }
  }

  void displayTime(unsigned long time) {
    unsigned long tenths = time / MILLIS_TO_TENTHS;

    uint8_t pos = currentPos;
    currentPos = (currentPos + 1) % DIGITS_COUNT;

    if (isLeadingZero(pos, tenths)) {
      writeGlyphBitmask(BLANK_GLYPH, formatPosBitmask(pos));
    } else {
      uint8_t digit = getDigitAt(tenths, pos);
      bool hasDecimalPoint = (pos == DECIMAL_POINT_POS);
      displayDigit(digit, pos, hasDecimalPoint);
    }
  }
};



class StopsApp {
private:
  Display display;
  Stops stops;

public:
  void begin() {
    display.setupGlyphs();
    stops.setup();
  }

  void runApp() {
    stops.oneStep();
    display.displayTime(stops.getTime());
  }
};

StopsApp stopsApp;

void setup() {
  stopsApp.begin();
}

void loop() {
  stopsApp.runApp();
}
