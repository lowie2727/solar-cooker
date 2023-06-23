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

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

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
  clockSetup();
  LCDSetup();
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

    if (currentMillis - previousMillis >= 2000) {
      previousMillis = currentMillis;

      String year = getYear();
      String month = getMonth();
      String day = getDay();
      String hour12 = getHour24();
      String hour24 = getHour24();
      String minute = getMinute();
      String second = getSecond();

      String AM2315Temp = getAM2315Temp();
      String windSpeed = getWindSpeed();
      String BME680Temp = getBME680Temperature();
      String BME680Pres = getBME680Pressure();
      String BME680Hum = getBME680Humidity();
      String PT100Temp1 = getPT100Temp1();
      //String PT100Temp2 = getPT100Temp2();
      //String PT100Temp3 = getPT100Temp3();
      String pyranoIrr = getSolarIrradiance();

      String dataSd = year + ";" + month + ";" + day + ";" + hour12 + ";" +
                      minute + ";" + second + ";" + AM2315Temp + ";" +
                      windSpeed + ";" + BME680Pres + ";" + BME680Hum + ";" +
                      PT100Temp1 + ";" + pyranoIrr;
      stringToSd(dataSd);

      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_GREEN);
      tft.setTextSize(2);
      tft.println("date: " + day + "-" + month + "-" + year);
      tft.println("time: " + hour24 + ":" + minute + ":" + second);
      tft.println();
      tft.println("Temp. pot: " + PT100Temp1 + " C");
      tft.println("Temp. out: " + AM2315Temp + " C");
      tft.println("Sun irr. : " + pyranoIrr + " W/m2");
      tft.println("Wind     : " + windSpeed + " m/s");
      tft.println("Pressure : " + BME680Pres + " hpa");
      tft.println("Humidity : " + BME680Hum + " %");
    }
  } else {
    redLedOn();
    fileNameUpdated = 0;
  }
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}
