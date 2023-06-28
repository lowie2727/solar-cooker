#include <Arduino.h>

// clock module
#include "clock.h"

// LCD
void LCDSetup();

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"

#define TFT_DC 7
#define TFT_CS 3
#define TFT_RST 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void writeLine1();
void writeLine2();
void writeLine3();

char previousLine1[100];
char previousLine2[100];
char previousLine3[100];

unsigned long previousMillis = 0;

uint8_t previousSecond = 0;
void writeDataToScreen();

// SD
const int chipSelect = 4;
#include <SdFat.h>
SdFat sd;
SdFile myFile;

void SDSetup();
void testWriteToSd();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println();

  clockSetup();
  LCDSetup();
}

void loop() {
  writeDataToScreen();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 3000) {
    testWriteToSd();
    previousMillis = currentMillis;
  }
}

void writeDataToScreen() {
  writeLine1();
  //  writeLine2();
  //  writeLine3();
}

void writeLine1() {
  uint16_t year = getYear();
  uint8_t month = getMonth();
  uint8_t day = getDay();
  uint8_t hour = getHour24();
  uint8_t minute = getMinute();
  uint8_t second = getSecond();

  char line1[100];
  snprintf(line1, 100, "%04d-%02d-%02d T%02d:%02d:%02d", year, month, day, hour,
           minute, second);

  if (previousSecond != second) {
    previousSecond = second;
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine1);
    strcpy(previousLine1, line1);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line1);
  }
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}

void SDSetup() {
  if (!sd.begin(chipSelect, SPI_EIGHTH_SPEED)) {
    sd.initErrorHalt();
  }
}

void testWriteToSd() {
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening test.txt for write failed"));
  }
  Serial.println(F("Writing to test.txt..."));
  myFile.println(F("testing 1, 2, 3."));
  myFile.close();
  Serial.println(F("done writing."));
}