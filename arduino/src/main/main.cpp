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

String dateTime;
String AM2315Temp, AM2315Hum;
String windSpeed;
String BME680Temp, BME680Pres, BME680Hum;
String PT100Temp;

void setup() {
  Serial.begin(9600);
  Serial.println();

  AM2315Setup();
  BME680Setup();
  clockModuleSetup();
  e_PaperSetup();
  GPSSetup();
  // microSDSetup();
  PT100Setup();
  switchSetup();
}

void loop() {
  int switchState = digitalRead(SWITCH_PIN);

  if (switchState) {
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);

    /*if (!fileNameUpdated) {
    updateFileName();
    writeCSVHeaders();
    fileNameUpdated = 1;
  }*/

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      Serial.println(F("AM2315T (°C);Wind speed (m/s);BME680T "
                       "(°C);BME680P (°C);BME680H(°C);PT100T (°C)"));

      dateTime = getDateTime();
      getAM2315TempAndHum(&AM2315Temp, &AM2315Hum);
      windSpeed = getWindSpeed();
      BME680Temp = getBME680Temperature();
      BME680Pres = getBME680Pressure();
      BME680Hum = getBME680Humidity();
      // String GPSLat = getGPSLatitude();
      // String GPSLong = getGPSLongitude();
      PT100Temp = getPT100Temperature();
      // String PyranoIrr = getSolarIrradiance();
      String data = dateTime + ";" + AM2315Temp + ";" + AM2315Hum + ";" +
                    windSpeed + ";" + BME680Temp + ";" + BME680Pres + ";" +
                    BME680Hum + ";" +
                    /*GPSLat + ";" + GPSLong + ";"*/
                    PT100Temp + ";" /* + PyranoIrr */;
      Serial.println(data);
      e_PaperPrint(dateTime, AM2315Temp, AM2315Hum, windSpeed, BME680Temp,
                   BME680Pres, BME680Hum, PT100Temp);
    }
  } else {
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, HIGH);
    fileNameUpdated = 0;
  }
}
