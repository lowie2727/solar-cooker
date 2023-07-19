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
void writeLine7();
void writeLine8();
void writeLine9();
void writeLine10();

unsigned long previousMillis = 0;
unsigned long previousMillisTemp = 0;

uint8_t previousSecond = -1;
float previousTemp = -1;
float previousWind = -1;
float previousPressure = -1;
float previousHumidity = -1;
float previousPt100Temp1 = -1;
float previousPt100Temp2 = -1;
float previousSolarIrradiance = -1;

char previousLine1[100];
char previousLine2[100];
char previousLine3[100];
char previousLine4[100];
char previousLine5[100];
char previousLine6[100];
char previousLine7[100];
char previousLine8[100];
char previousLine9[100];
char previousLine10[100];

int fileNameUpdated = 0;

static float tempBuffer[300] = {0};
static int bufferIndex = 0; 
float pretempsum = 0.0;
int counter3 = 0;

static float tempBuffer2[60] = {0};
static int bufferIndex2 = 0;
static int bufferIndex3 = 0; 
int counter4 = 0;

unsigned long counter = 0;
unsigned long counter2 = 0;
unsigned long previousValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();

  AM2315CSetup();
  BME680Setup();
  clockSetup();
  LCDSetup();
  Pt100Setup();
  switchSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 100) {
      writeDataToScreen();
      previousMillis = currentMillis;
    }

  } else {
    redLedOn();
    fileNameUpdated = 0;
  }
}

void writeDataToScreen() {
  writeLine1();
  writeLine2();
  writeLine3();
  writeLine4();
  writeLine5();
  writeLine6();
  writeLine7();
  writeLine8();
  writeLine9();
  writeLine10();
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

  temp = getAM2315CTemp();
  dtostrf(temp, 4, 2, AM2315CTemp);
  previousMillisTemp = currentMillisTemp;

  char line2[100];
  snprintf(line2, 100, "Temp out: %sC", AM2315CTemp);

  if (previousTemp != temp) {
    previousTemp = temp;
    tft.setCursor(0, 120);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine2);
    strcpy(previousLine2, line2);
    tft.setCursor(0, 120);
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
  snprintf(line3, 100, "Wind : %sm/s", windSpeed);

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

void writeLine4() {
  float BME680PressureFloat = getBME680Pressure();

  char BME680Pressure[20];
  dtostrf(BME680PressureFloat, 6, 2, BME680Pressure);

  char line4[100];
  snprintf(line4, 100, "Pressure: %shPa", BME680Pressure);

  if (previousPressure != BME680PressureFloat) {
    previousPressure = BME680PressureFloat;
    tft.setCursor(0, 60);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine4);
    strcpy(previousLine4, line4);
    tft.setCursor(0, 60);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line4);
  }
}

void writeLine5() {
  float BME680HumidityFloat = getBME680Humidity();

  char BME680Humidity[20];
  dtostrf(BME680HumidityFloat, 4, 2, BME680Humidity);

  char line4[100];
  snprintf(line4, 100, "Humidity: %s%%", BME680Humidity); //switchen naar de am2315C

  if (previousHumidity != BME680HumidityFloat) {
    previousHumidity = BME680HumidityFloat;
    tft.setCursor(0, 80);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine4);
    strcpy(previousLine4, line4);
    tft.setCursor(0, 80);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line4);
  }
}

void writeLine6() {
  char line6[100];
  if (!getPt100Fault_1()) {
    float Pt100Temp1Float = getPt100Temp1();

    char Pt100Temp1[20];
    dtostrf(Pt100Temp1Float, 5, 2, Pt100Temp1);

    char line6[100];
    snprintf(line6, 100, "Pot 1: %sC", Pt100Temp1);

    if (previousPt100Temp1 != Pt100Temp1Float) {
      previousPt100Temp1 = Pt100Temp1Float;
      tft.setCursor(0, 140);
      tft.setTextColor(ILI9341_BLACK);
      tft.setTextSize(2);
      tft.println(previousLine6);
      strcpy(previousLine6, line6);
      tft.setCursor(0, 140);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println(line6);
    }
  } else {
    tft.setCursor(0, 140);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    snprintf(line6, 100, "Pot 1: NULL");
    tft.println(previousLine6);
    strcpy(previousLine6, line6);
    tft.setCursor(0, 140);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line6);
  }
}

void writeLine7() {
  float solarIrradianceFloat = getSolarIrradiance();

  char solarIrradiance[20];
  dtostrf(solarIrradianceFloat, 6, 2, solarIrradiance);

  char line7[100];
  snprintf(line7, 100, "irr: %sW/m2", solarIrradiance);

  if (previousSolarIrradiance != solarIrradianceFloat) {
    previousSolarIrradiance = solarIrradianceFloat;
    tft.setCursor(0, 100);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine7);
    strcpy(previousLine7, line7);
    tft.setCursor(0, 100);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line7);
  }
}

void writeLine8() {
  char line8[100];
  if (!getPt100Fault_2()) {
    float Pt100Temp2Float = getPt100Temp2();

    char Pt100Temp2[20];
    dtostrf(Pt100Temp2Float, 5, 2, Pt100Temp2);

    char line8[100];
    snprintf(line8, 100, "Pot 2: %sC", Pt100Temp2);

    if (previousPt100Temp2 != Pt100Temp2Float) {
      previousPt100Temp2 = Pt100Temp2Float;
      tft.setCursor(0, 160);
      tft.setTextColor(ILI9341_BLACK);
      tft.setTextSize(2);
      tft.println(previousLine8);
      strcpy(previousLine8, line8);
      tft.setCursor(0, 160);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println(line8);
    }
  } else {
    tft.setCursor(0, 160);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    snprintf(line8, 100, "Pot 2: NULL");
    tft.println(previousLine8);
    strcpy(previousLine8, line8);
    tft.setCursor(0, 160);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line8);
  }
}


void writeLine10() {
  float pretemp = 0.0;
  float sum = 0.0;

  float currentTemp = getPt100Temp1();
  tempBuffer[bufferIndex] = currentTemp;
  
  if(counter3 < 300){
    pretempsum = pretempsum + currentTemp;
    counter3 += 1;
    pretemp = pretempsum/counter3;
  }

  for (int i = 0; i < 300; i++) {
    sum += tempBuffer[i];
  }

  float avgTemp = sum / 300;
  bufferIndex = (bufferIndex + 1) % 300;

  char line10[100];
  if (!getPt100Fault_1()) {
    char Pt100Temp1[20];
    if(counter3 < 300){
        dtostrf(pretemp, 5, 2, Pt100Temp1);
    } else{
      dtostrf(avgTemp, 5, 2, Pt100Temp1);
    }
    snprintf(line10, 100, "Avg 5min: %sC", Pt100Temp1);
  } else {
    snprintf(line10, 100, "Avg 5min: NULL");
  }

  if (strcmp(line10, previousLine10) != 0) {
    previousPt100Temp1 = currentTemp;
    tft.setCursor(0, 180);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine10);
    strcpy(previousLine10, line10);
    tft.setCursor(0, 180);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line10);
  }
}


void writeLine9() {
  float slope = 0.0;

  float currentTemp = getPt100Temp1();
  tempBuffer2[bufferIndex2] = currentTemp;
  
  if(counter4 < 60){
    counter4 += 1;
  }

  bufferIndex2 = (bufferIndex2 + 1) % 60;
  bufferIndex3 = (bufferIndex2 + 59) % 60;

  char line9[100];
  if (!getPt100Fault_1()) {
    char Pt100Temp1[20];
    if(counter4 < 60){
        snprintf(line9, 100, "Slope 1min: ...");
    } else{
      dtostrf(slope, 5, 2, Pt100Temp1);
      snprintf(line9, 100, "Slope 1min: %sC/s", Pt100Temp1);
    }
  } else {
    snprintf(line9, 100, "Slope 1min: NULL");
  }

  if (strcmp(line9, previousLine9) != 0) {
    previousPt100Temp1 = currentTemp;
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine9);
    strcpy(previousLine9, line9);
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line9);
  }
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}
