#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// AM2315
#include "AM2315C.h"

// anemometer
#include "anemometer.h"

// BME680
#include "BME680.h"

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

// microSD
#include "microSD.h"

// PT100
#include "Pt100.h"

// pyranometer
#include "pyranometer.h"

// switch
#include "switch.h"

void writeDataToSd();
void writeDataToScreen();

void writeLine1();
void writeLine2();
void writeLine3();
void writeLine4();
void writeLine5();
void writeLine6();

unsigned long previousMillis = 0;
unsigned long previousMillisTemp = 0;

uint8_t previousSecond = 0;
float previousTemp = 0.0;
float previousWind = 0.0;

char previousLine1[100];
char previousLine2[100];
char previousLine3[100];
char previousLine4[100];
char previousLine5[100];
char previousLine6[100];

int fileNameUpdated = 0;
unsigned long counter = 0;
unsigned long counter2 = 0;
unsigned long previousValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();

  AM2315Setup();
  BME680Setup();
  clockSetup();
  LCDSetup();
  microSDSetup();
  Pt100Setup();
  switchSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    if (!fileNameUpdated) {
      // updateFileName();
      writeCSVHeaders();
      fileNameUpdated = 1;
    }

    writeDataToScreen();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 3000) {
      writeDataToSd();
      previousMillis = currentMillis;
    }

  } else {
    redLedOn();
    fileNameUpdated = 0;
  }
}

void writeDataToSd() {
  char AM2315CTemp[30];
  dtostrf(getAM2315CTemp(), 4, 2, AM2315CTemp);

  char windSpeed[20];
  dtostrf(getWindSpeed(), 4, 2, windSpeed);

  char BME680Pressure[20];
  dtostrf(getBME680Pressure(), 6, 2, BME680Pressure);

  char BME680Humidity[20];
  dtostrf(getBME680Humidity(), 4, 2, BME680Humidity);

  char Pt100Temp1[20];
  dtostrf(getPt100Temp1(), 5, 2, Pt100Temp1);

  char solarIrradiance[20];
  dtostrf(getSolarIrradiance(), 6, 2, solarIrradiance);

  Serial.println("writing to sd");

  char dataSd[500];
  snprintf(dataSd, 500, "%04d;%02d;%02d;%02d;%02d;%02d;%s;%s;%s;%s;%s;%s",
           getYear(), getMonth(), getDay(), getHour24(), getMinute(),
           getSecond(), AM2315CTemp, windSpeed, BME680Pressure, BME680Humidity,
           Pt100Temp1, solarIrradiance);
  stringToSd(dataSd);
}

void writeDataToScreen() {
  writeLine1();
  writeLine2();
  writeLine3();

  char BME680Pressure[20];
  dtostrf(getBME680Pressure(), 6, 2, BME680Pressure);

  char BME680Humidity[20];
  dtostrf(getBME680Humidity(), 4, 2, BME680Humidity);

  char Pt100Temp1[20];
  dtostrf(getPt100Temp1(), 5, 2, Pt100Temp1);

  char solarIrradiance[20];
  dtostrf(getSolarIrradiance(), 6, 2, solarIrradiance);
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

void writeLine2() {

  float temp = previousTemp;
  unsigned long currentMillisTemp = millis();
  char AM2315CTemp[30];
  dtostrf(temp, 4, 2, AM2315CTemp);

  if (currentMillisTemp - previousMillisTemp >= 3000) {
    temp = getAM2315CTemp();
    dtostrf(temp, 4, 2, AM2315CTemp);
    previousMillisTemp = currentMillisTemp;
  }

  char line2[100];
  snprintf(line2, 100, "temp out: %s", AM2315CTemp);

  if (previousTemp != temp) {
    previousTemp = temp;
    tft.setCursor(0, 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine2);
    strcpy(previousLine2, line2);
    tft.setCursor(0, 20);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line2);
  }
}

void writeLine3() {
  float wind = getWindSpeed();

  char windSpeed[20];
  dtostrf(getWindSpeed(), 4, 2, windSpeed);

  char line3[100];
  snprintf(line3, 100, "wind speed: %s", windSpeed);

  if (previousWind != wind) {
    previousWind = wind;
    tft.setCursor(0, 40);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine3);
    strcpy(previousLine3, line3);
    tft.setCursor(0, 40);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line3);
  }
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}
