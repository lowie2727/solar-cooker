#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// AM2315
#include "AM2315.h"

// anemometer
#include "anemometer.h"

// BME680
#include "BME680.h"

// clock module
#include "clockModule.h"

// microSD
#include "microSD.h"

// PT100
#include "PT100.h"

// pyranometer
#include "pyranometer.h"

// switch
#include "switch.h"

unsigned long previousMillis = 0;

int fileNameUpdated = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();

  AM2315Setup();
  BME680Setup();
  clockModuleSetup();
  microSDSetup();
  PT100Setup();
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

    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;

      uint16_t year = getYear();
      uint8_t month = getMonth();
      uint8_t day = getDay();
      String hour = getHour();
      uint8_t minute = getMinute();
      uint8_t second = getSecond();

      float AM2315Temp = getAM2315Temp();
      float windSpeed = getWindSpeed();
      float BME680Temp = getBME680Temperature();
      float BME680Pres = getBME680Pressure();
      float BME680Hum = getBME680Humidity();
      float PT100Temp = getPT100Temperature();
      float pyranoIrr = getSolarIrradiance();

      // csv data
      char csv[100];
      snprintf(csv, 100,
               "%04u;%02u;%02u;%s;%02u;%02u;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f",
               year, month, day, hour.c_str(), minute, second,
               (double)AM2315Temp, (double)windSpeed, (double)BME680Pres,
               (double)BME680Hum, (double)PT100Temp, (double)pyranoIrr);
      stringToSd(csv);

      // serial monitor data
      char date[100];
      snprintf(date, 100, "Date: %04u-%02u-%02u", year, month, day);
      Serial.println(date);

      char time[100];
      snprintf(time, 100, "Time: %s:%02u:%02u", hour.c_str(), minute, second);
      Serial.println(time);

      char AM2315[100];
      snprintf(AM2315, 100, "AM2315 Temperature: %.2f °C", (double)AM2315Temp);
      Serial.println(AM2315);

      char anemo[100];
      snprintf(anemo, 100, "Wind speed: %.2f m/s", (double)windSpeed);
      Serial.println(anemo);

      char BME680[100];
      snprintf(
          BME680, 100,
          "BME680 Temperature: %.2f °C, Pressure: %.2f hPa, Humidity: %.2f \%",
          (double)BME680Temp, (double)BME680Pres, (double)BME680Hum);
      Serial.println(BME680);

      char PT100[100];
      snprintf(PT100, 100, "PT100: %.2f °C", (double)PT100Temp);
      Serial.println(PT100);

      char pyranometer[100];
      snprintf(pyranometer, 100, "Pyranometer Irradiance: %.2f °C",
               (double)pyranoIrr);
      Serial.println(pyranometer);
    }
  } else {
    redLedOn();
    fileNameUpdated = 0;
  }
}
