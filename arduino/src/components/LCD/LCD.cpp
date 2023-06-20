/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <Arduino.h>

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 7
#define TFT_CS 3
#define TFT_MOSI 51
#define TFT_CLK 52
#define TFT_RST 5
#define TFT_MISO 50

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft =
    Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

unsigned long testText();

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!");

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}

void loop(void) {
  testText();
  delay(1000);
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("Lowie en Kato zijn 2 mensen");
  tft.setTextSize(1);
  tft.println("voet ziektes zijn erg");
  tft.println("lorem ipsum latijn vbla bla");
  tft.println("with crinkly bindlewurdles,");
  tft.println("wie goed doet goed ontmoet");
  tft.println("in the gobberwarts");
  tft.println("ondersteboven als de vindt met dt");
  return micros() - start;
}
