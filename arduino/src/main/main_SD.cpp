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
  BME680Setup();
  clockSetup();
  Pt100Setup();
  switchSetup();
  microSDSetup();
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

    if (currentMillis - previousMillis >= 1000) {
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

  char AM2315CHum[20];
  dtostrf(getAM2315CHum(), 5, 2, AM2315CHum);

  char Pt100Temp1[20];
  dtostrf(getPt100Temp1(), 5, 2, Pt100Temp1);

  char Pt100Temp2[20];
  dtostrf(getPt100Temp2(), 5, 2, Pt100Temp2);

  char solarIrradiance[20];
  dtostrf(getSolarIrradiance(), 6, 2, solarIrradiance);

  Serial.println("writing to sd");

  char dataSd[500];
  snprintf(dataSd, 500, "%04d;%02d;%02d;%02d;%02d;%02d;%s;%s;%s;%s;%s;%s;%s",
           getYear(), getMonth(), getDay(), getHour24(), getMinute(),
           getSecond(), AM2315CTemp, windSpeed, BME680Pressure, AM2315CHum,
           Pt100Temp1, Pt100Temp2, solarIrradiance);
  stringToSd(dataSd);
  Serial.println(dataSd);
}
