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

// microSD
#include "microSD.h"

// PT100
#include "Pt100.h"

// pyranometer
#include "pyranometer.h"

// switch
#include "switch.h"

void writeDataToSd();

unsigned long previousMillis = 0;

int fileNameUpdated = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();

  AM2315CSetup();
  anemoSetup();
  BME680Setup();
  clockSetup();
  microSDSetup();
  Pt100Setup();
  pyranoSetup();
  switchSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    if (!fileNameUpdated) {
      updateFileName();
      writeCSVHeaders();
      fileNameUpdated = 1;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 2000) {
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
  dtostrf(getPt100Temp1(), 5, 2, Pt100Temp1);

  char Pt100Temp2[20];
  dtostrf(getPt100Temp2(), 5, 2, Pt100Temp2);

  char solarIrradiance[20];
  dtostrf(getSolarIrradiance(), 6, 2, solarIrradiance);

  Serial.println("writing to sd");

  uint16_t year = getYear();
  uint8_t month = getMonth();
  uint8_t day = getDay();
  uint8_t hour = getHour24();
  uint8_t minute = getMinute();
  uint8_t second = getSecond();

  char dataSd[500];
  snprintf(dataSd, 500, "%04d;%02d;%02d;%02d;%02d;%02d;%s;%s;%s;%s;%s;%s;%s",
           year, month, day, hour, minute, second, AM2315CTemp, windSpeed,
           BME680Pressure, AM2315CHum, Pt100Temp1, Pt100Temp2, solarIrradiance);
  stringToSd(dataSd);
  Serial.println(dataSd);
  Serial.println();

  char line1[100];
  snprintf(line1, 100, "%04d-%02d-%02d T%02d:%02d:%02d", year, month, day, hour,
           minute, second);
  Serial.println(line1);

  char line2[100];
  snprintf(line2, 100, "Outside temperature: %s °C\nOutside humidity: %s%%",
           AM2315CTemp, AM2315CHum);
  Serial.println(line2);

  char line3[100];
  snprintf(line3, 100, "Temperature pot 1: %s °C\nTemperature pot 2: %s °C",
           Pt100Temp1, Pt100Temp2);
  Serial.println(line3);

  char line4[100];
  snprintf(line4, 100, "Wind speed: %s m/s", windSpeed);
  Serial.println(line4);

  char line5[100];
  snprintf(line5, 100, "Solar irradiance: %s W/m²", solarIrradiance);
  Serial.println(line5);

  char line6[100];
  snprintf(line6, 100, "Inside humidity: %s%%", BME680Humidity);
  Serial.println(line6);

  char line7[100];
  snprintf(line7, 100, "Inside pressure: %s hPa", BME680Pressure);
  Serial.println(line7);

  char line8[100];
  snprintf(line8, 100, "Inside temperature: %s °C", BME680Temperature);
  Serial.println(line8);
  Serial.println();
}
