#include "funshield.h" 

const int GLYPHS_PINS_COUNT = 3;
const int DIGITS_COUNT = 4;

class Display {
  private:
    const int glyphsPins[GLYPHS_PINS_COUNT] = {SEG7_LATCH_PIN, SEG7_CLOCK_PIN, SEG7_DATA_PIN};
    int position = 0;
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

    int glyph( int c){
      if (isupper( c)){
        return SEG7_LETTER_GLYPHS[ c - 'a'];
      }
    }
    void displayChar(byte characterValue, uint8_t digitPosition){
      byte character = glyph((int)characterValue);
      byte positionBitmask = formatPosBitmask(digitPosition);
      writeGlyphBitmask(character, positionBitmask);
    }

    void displayString(int *pString){
      while( *pString++) {
        byte character = glyph((int)pString);
        byte positionBitmask = formatPosBitmask(position);
        writeGlyphBitmask(character, positionBitmask);
        position++;
      }
      position=0;
    }
};

Display display;
void setup() {
  // put your setup code here, to run once:
  display.setupGlyphs();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char hello[] = "ahoj";
  display.displayString(*hello);
  // display.displayChar('o', 1);
  // display.displayChar('j', 0);

}
