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

// e-Paper
#include "e-Paper.h"

// GPS
#include "GPS.h"

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
  e_PaperSetup();
  GPSSetup();
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

      String dateTime = getDateTime();
      float AM2315Temp = getAM2315Temp();
      float windSpeed = getWindSpeed();
      float BME680Temp = getBME680Temperature();
      float BME680Pres = getBME680Pressure();
      float BME680Hum = getBME680Humidity();
      float PT100Temp = getPT100Temperature();
      float pyranoIrr = getSolarIrradiance();

      char data[200] PROGMEM;
      char AM2315TempString[6] PROGMEM;
      char windSpeedString[6] PROGMEM;
      char BME680TempString[6] PROGMEM;
      char BME680PresString[8] PROGMEM;
      char BME680HumString[6] PROGMEM;
      char PT100TempString[7] PROGMEM;
      char pyranoIrrString[8] PROGMEM;

      dtostrf(AM2315Temp, 4, 2, AM2315TempString);
      dtostrf(windSpeed, 4, 2, windSpeedString);
      dtostrf(BME680Temp, 5, 2, BME680TempString);
      dtostrf(BME680Pres, 6, 2, BME680PresString);
      dtostrf(BME680Hum, 5, 2, BME680HumString);
      dtostrf(PT100Temp, 5, 2, PT100TempString);
      dtostrf(pyranoIrr, 6, 2, pyranoIrrString);

      snprintf(data, 200, "%s;%s;%s;%s;%s;%s;%s;%s", dateTime.c_str(),
               AM2315TempString, windSpeedString, BME680TempString,
               BME680PresString, BME680HumString, PT100TempString,
               pyranoIrrString);

      //"Time [YYYY-MM-DDTHH:MM:ss];Water temperature [°C];Wind speed
      //[m/s];Outside temperature [°C];Air pressure [hPa];Relative humidity
      //[%];Solar irradiance [W/m²]";

      // GPSLat + ";" + GPSLong + ";"*/
      /*PyranoIrr*/;
      /*e_PaperPrint(dateTime, AM2315Temp, BME680Hum, windSpeed, BME680Temp,
                   BME680Pres, BME680Hum, PT100Temp);*/
      stringToSd(data);
      Serial.println(data);
    }
  } else {
    redLedOn();
    fileNameUpdated = 0;
  }
}
