#include "funshield.h"
#include <ctype.h>

const int DIGITS_COUNT = 4;
const int GLYPHS_PINS_COUNT = 3;
const int glyphsPins[GLYPHS_PINS_COUNT] = {SEG7_LATCH_PIN, SEG7_CLOCK_PIN, SEG7_DATA_PIN};

void setupGlyphs() {
      for (int glyphsPinsIndex = 0; glyphsPinsIndex < GLYPHS_PINS_COUNT; glyphsPinsIndex++) {
        pinMode(glyphsPins[glyphsPinsIndex], OUTPUT);
      }
    }
void setup() {
  Serial.begin(9600);
  Serial.println("Posli mi nejaky text a ja ho zanalyzuji!");
  setupGlyphs();
}
byte formatPosBitmask(uint8_t digitPosition) {
  return 1 << ((DIGITS_COUNT - 1) - digitPosition);
}
void displayChar(byte characterValue, uint8_t digitPosition) {
  byte positionBitmask = formatPosBitmask(digitPosition);
  writeGlyphBitmask(characterValue, positionBitmask);
}

void writeGlyphBitmask(byte glyph, byte pos_bitmask) {
  digitalWrite(SEG7_LATCH_PIN, LOW);
  shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, glyph);
  shiftOut(SEG7_DATA_PIN, SEG7_CLOCK_PIN, MSBFIRST, pos_bitmask);
  digitalWrite(SEG7_LATCH_PIN, HIGH);
}

void loop() {
  // Pokud nam pres Seriovou linku prisla nejaka data
  if (Serial.available() > 0) {
    // Precteme jeden znak
    char c = Serial.read();

    // Vyfiltrujeme neviditelne znaky jako enter (\n, \r), aby nas nerusily ve vypisu
    if (c == '\n' || c == '\r') return;

    Serial.print("Analyzuji znak: '");
    Serial.print(c);
    Serial.println("'");

    // 1. Je to pismeno?
    if (isalpha(c)) {
      Serial.println(" -> Je to pismeno.");

      // Pokud ano, je to male, nebo velke pismeno?
      if (islower(c)) {
        Serial.println(" -> Je to male pismeno (a-z).");
        byte character = SEG7_LETTER_GLYPHS[c - 'a'];
        displayChar(character, 1);
        // TADY JE TEN TRIK Z PREZENTACE:
        // 'a' ma v ASCII tabulce hodnotu 97. Kdyz od znaku odectes 'a',
        // ziskas jeho poradi v abecede (0 pro 'a', 1 pro 'b', 2 pro 'c' atd.).
        int index = c - 'a';
        Serial.print(" -> Index v abecede: ");
        Serial.println(index);

      } else if (isupper(c)) {
        Serial.println(" -> Je to velke pismeno (A-Z).");
      }
    }
    // 2. Je to cislo?
    else if (isdigit(c)) {
      Serial.println(" -> Je to cislo (0-9).");
    }
    // 3. Je to neco jineho?
    else {
      Serial.println(" -> Je to jiny znak (mezera, carka, zavinac...).");
    }

    Serial.println("-------------------");
  }
}