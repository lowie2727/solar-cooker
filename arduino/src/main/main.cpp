#include <Arduino.h>

#include "anemometer.h"

#include "AM2315C.h"

#include "BME680.h"

#include "clock.h"

#include "Pt100.h"

#include "pyranometer.h"

#include "switch.h"

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
void writeLine4();
void writeLine5();
void writeLine6();
void writeLine7();
void writeLine8();
void writeLine9();

char previousLine1[100];
char previousLine2[100];
char previousLine3[100];
char previousLine4[100];
char previousLine5[100];
char previousLine6[100];
char previousLine7[100];
char previousLine8[100];
char previousLine9[100];

float previousTemp = -1;
float previousHumidity = -1;
float previousPt100Temp1 = -1;
float previousPt100Temp2 = -1;
float previousPt100Temp3 = -1;
uint8_t previousSecond = -1;
float previousWind = -1;
float previousSolarIrradiance = -1;
float previousSlope = -1;

void writeDataToScreen();

unsigned long previousMillisScreen = 0;

// SD
#include <SdFat.h>

const int chipSelect = 4;

SdFat sd;
SdFile myFile;

char filePath[200];
int fileNameUpdated = 0;
unsigned long previousMillisSD = 0;

void updateFileName();
void makeFile();

void microSDSetup();
void writeCSVHeaders();
void writeDataToSD();

float tempBuffer[60];
int bufferIndex = 0;
int isArrayEmpty = 1;

void shiftAndAppend(float array[], float value);
void initArray(float array[], float value);

unsigned long testDurationMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println();

  AM2315CSetup();
  anemoSetup();
  BME680Setup();
  clockSetup();
  microSDSetup();
  Pt100Setup();
  pyranoSetup();
  switchSetup();
  LCDSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    if (!fileNameUpdated) {
      testDurationMillis = millis();
      updateFileName();
      writeCSVHeaders();
      fileNameUpdated = 1;
    }

    unsigned long currentMillisScreen = millis();

    if (currentMillisScreen - previousMillisScreen >= 1000) {
      writeDataToScreen();
      previousMillisScreen = currentMillisScreen;
    }

    unsigned long currentMillisSD = millis();

    if (currentMillisSD - previousMillisSD >= 10000) {
      writeDataToSD();
      previousMillisSD = currentMillisSD;
    }
  } else {
    redLedOn();
    fileNameUpdated = 0;
    isArrayEmpty = 1;
  }
}

void microSDSetup() {
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    sd.initErrorHalt();
  }
}

void writeCSVHeaders() {
  char CSVHeaders[] =
      "Year;Month;Day;Hour;Minute;Second;Outside temperature "
      "[°C];Wind speed [m/s];Air pressure (inside box) [hPa];Relative humidity "
      "(inside box) [%];Temperature inside pot 1 [°C];Temperature inside pot 2 "
      "[°C];Temperature inside pot 3 [°C];Solar irradiance [W/m²]";
  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening test.txt for write failed"));
  }
  Serial.println("Writing CSV headers");
  myFile.println(CSVHeaders);
  myFile.close();
  Serial.println(F("done writing."));
}

void writeDataToSD() {
  char AM2315CTemp[30];
  dtostrf(getAM2315CTemp(), 4, 2, AM2315CTemp);

  char AM2315CHum[20];
  dtostrf(getAM2315CHum(), 5, 2, AM2315CHum);

  char BME680Humidity[20];
  dtostrf(getBME680Humidity(), 6, 2, BME680Humidity);

  char BME680Pressure[20];
  dtostrf(getBME680Pressure(), 6, 2, BME680Pressure);

  char BME680Temperature[20];
  dtostrf(getBME680Temperature(), 6, 2, BME680Temperature);

  char windSpeed[20];
  dtostrf(getWindSpeed(), 4, 2, windSpeed);

  char Pt100Temp1[20];
  if (!getPt100Fault_1()) {
    dtostrf(getPt100Temp1(), 5, 2, Pt100Temp1);
  } else {
    snprintf(Pt100Temp1, 20, "x");
  }

  char Pt100Temp2[20];
  if (!getPt100Fault_2()) {
    dtostrf(getPt100Temp2(), 5, 2, Pt100Temp2);
  } else {
    snprintf(Pt100Temp2, 20, "x");
  }

  char Pt100Temp3[20];
  if (!getPt100Fault_3()) {
    dtostrf(getPt100Temp3(), 5, 2, Pt100Temp3);
  } else {
    snprintf(Pt100Temp3, 20, "x");
  }

  char solarIrradiance[20];
  dtostrf(getSolarIrradiance(), 6, 2, solarIrradiance);

  char dataSd[500];
  snprintf(dataSd, 500, "%04d;%02d;%02d;%02d;%02d;%02d;%s;%s;%s;%s;%s;%s;%s;%s",
           getYear(), getMonth(), getDay(), getHour24(), getMinute(),
           getSecond(), AM2315CTemp, windSpeed, BME680Pressure, AM2315CHum,
           Pt100Temp1, Pt100Temp2, Pt100Temp3, solarIrradiance);

  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening test.txt for write failed"));
  }
  Serial.println(dataSd);
  myFile.println(dataSd);
  myFile.close();
  Serial.println(F("done writing."));
}

void updateFileName() {
  snprintf(filePath, 200, "%04d/%02d/%02d%02d%02d%02d.csv", getYear(),
           getMonth(), getDay(), getHour24(), getMinute(), getSecond());

  Serial.println(filePath);

  char dirName[200];
  snprintf(dirName, 200, "%04d/%02d/", getYear(), getMonth());

  sd.mkdir(String(getYear()));
  sd.mkdir(dirName);

  makeFile();
}

void makeFile() {
  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening file for write failed");
  }
  myFile.close();
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
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
}

void writeLine1() {
  unsigned long currentTestDurationSeconds =
      (unsigned long)(millis() - testDurationMillis) / 1000;

  uint8_t hours = currentTestDurationSeconds / 3600;

  int remainder1 = currentTestDurationSeconds % 3600;
  uint8_t minutes = remainder1 / 60;

  uint8_t seconds = remainder1 % 60;

  char line1[100];
  snprintf(line1, 100, "Time  : %02dh %02dm %02ds", hours, minutes, seconds);

  if (previousSecond != seconds) {
    previousSecond = seconds;
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
  char line2[100];
  if (!getPt100Fault_1()) {
    float Pt100Temp1Float = getPt100Temp1();

    char Pt100Temp1[20];
    dtostrf(Pt100Temp1Float, 5, 2, Pt100Temp1);

    char line2[100];
    snprintf(line2, 100, "Pot 1 : %s C", Pt100Temp1);

    if (previousPt100Temp1 != Pt100Temp1Float) {
      previousPt100Temp1 = Pt100Temp1Float;
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
  } else {
    tft.setCursor(0, 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    snprintf(line2, 100, "Pot 1 : NULL");
    tft.println(previousLine2);
    strcpy(previousLine2, line2);
    tft.setCursor(0, 20);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line2);
  }
}

void writeLine3() {
  char line3[100];
  if (!getPt100Fault_2()) {
    float Pt100Temp2Float = getPt100Temp2();

    char Pt100Temp2[20];
    dtostrf(Pt100Temp2Float, 5, 2, Pt100Temp2);

    char line3[100];
    snprintf(line3, 100, "Pot 2 : %s C", Pt100Temp2);

    if (previousPt100Temp2 != Pt100Temp2Float) {
      previousPt100Temp2 = Pt100Temp2Float;
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
  } else {
    tft.setCursor(0, 40);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    snprintf(line3, 100, "Pot 2 : NULL");
    tft.println(previousLine3);
    strcpy(previousLine3, line3);
    tft.setCursor(0, 40);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line3);
  }
}

void writeLine4() {
  char line4[100];
  if (!getPt100Fault_3()) {
    float Pt100Temp3Float = getPt100Temp3();

    char Pt100Temp3[20];
    dtostrf(Pt100Temp3Float, 5, 2, Pt100Temp3);

    char line4[100];
    snprintf(line4, 100, "Pot 3 : %s C", Pt100Temp3);

    if (previousPt100Temp3 != Pt100Temp3Float) {
      previousPt100Temp3 = Pt100Temp3Float;
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
  } else {
    tft.setCursor(0, 60);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    snprintf(line4, 100, "Pot 3 : NULL");
    tft.println(previousLine4);
    strcpy(previousLine4, line4);
    tft.setCursor(0, 60);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line4);
  }
}

void writeLine5() {
  float wind = getWindSpeed();

  char windSpeed[20];
  dtostrf(getWindSpeed(), 4, 2, windSpeed);

  char line5[100];
  snprintf(line5, 100, "Wind  : %s m/s", windSpeed);

  if (previousWind != wind) {
    previousWind = wind;
    tft.setCursor(0, 80);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine5);
    strcpy(previousLine5, line5);
    tft.setCursor(0, 80);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line5);
  }
}

void writeLine6() {
  float solarIrradianceFloat = getSolarIrradiance();

  char solarIrradiance[20];
  dtostrf(solarIrradianceFloat, 6, 2, solarIrradiance);

  char line6[100];
  snprintf(line6, 100, "Irr : %s W/m2", solarIrradiance);

  if (previousSolarIrradiance != solarIrradianceFloat) {
    previousSolarIrradiance = solarIrradianceFloat;
    tft.setCursor(0, 100);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine6);
    strcpy(previousLine6, line6);
    tft.setCursor(0, 100);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line6);
  }
}

void writeLine7() {
  if (!getPt100Fault_1()) {
    float temp = getPt100Temp1();
    if (isArrayEmpty) {
      initArray(tempBuffer, temp);
      isArrayEmpty = 0;
    } else {
      shiftAndAppend(tempBuffer, temp);
    }
  }

  char slope[30];
  float slopeFloat = tempBuffer[59] - tempBuffer[0];
  dtostrf(slopeFloat, 4, 2, slope);

  char line7[100];
  snprintf(line7, 100, "Slope 1min: %sC", slope);

  if (previousSlope != slopeFloat) {
    previousSlope = slopeFloat;
    tft.setCursor(0, 120);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine7);
    strcpy(previousLine7, line7);
    tft.setCursor(0, 120);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line7);
  }
}

void shiftAndAppend(float array[], float value) {
  for (size_t i = 0; i < 60 - 1; i++) {
    array[i] = array[i + 1];
  }
  array[60 - 1] = value;
}

void initArray(float array[], float value) {
  for (size_t i = 0; i < 60; i++) {
    array[i] = value;
  }
}

void writeLine8() {
  float temp = getAM2315CTemp();

  char AM2315CTemp[30];
  dtostrf(temp, 4, 2, AM2315CTemp);

  char line8[100];
  snprintf(line8, 100, "Temp out: %sC", AM2315CTemp);

  if (previousTemp != temp) {
    previousTemp = temp;
    tft.setCursor(0, 140);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine8);
    strcpy(previousLine8, line8);
    tft.setCursor(0, 140);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line8);
  }
}

void writeLine9() {
  float AM2315CHumidityFloat = getAM2315CHum();

  char AM2315CHumidity[20];
  dtostrf(AM2315CHumidityFloat, 4, 2, AM2315CHumidity);

  char line9[100];
  snprintf(line9, 100, "Humidity: %s%%", AM2315CHumidity);

  if (previousHumidity != AM2315CHumidityFloat) {
    previousHumidity = AM2315CHumidityFloat;
    tft.setCursor(0, 160);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine9);
    strcpy(previousLine9, line9);
    tft.setCursor(0, 160);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(line9);
  }
}
