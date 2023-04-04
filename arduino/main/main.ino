// anemometer
const int ANEMO_PIN = A0;
float anemoSensorValue;
float anemoWindSpeed;

// clock module
#include "Wire.h"
#include "libraries/GravityRtc/GravityRtc.cpp"

GravityRtc rtc; // RTC Initialization

// GPS
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// microSD
#include <SD.h>
#include <SPI.h>

const int CS_SD = 4;
const String headers =
    "windspeed (m/s);latitude;longitude;temperaturePT100 (°C)";
String filePath;

// PT100
#include <Adafruit_MAX31865.h>

Adafruit_MAX31865 thermo = Adafruit_MAX31865(11); // CS

#define RREF 430.0
#define RNOMINAL 100.0

void setup() {
  Serial.begin(9600);
  Serial.println();

  microSDInit();
  clockModuleSetup();
  updateFileName();
  writeHeaders();

  GPSSetup();

  PT100Setup();
}

void loop() {
  String data = getWindSpeed() + ";" + getGPSLatitude() + ";" +
                getGPSLongitude() + ";" + getPT100Temperature();
  stringToSd(data);
  delay(1000);
}

void microSDInit() {
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(CS_SD)) {
    Serial.println("Card failed, or not present");
    while (1) {
    };
  }
  Serial.println("card initialized.");
}

void clockModuleSetup() {
  rtc.setup();
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
}

void updateFileName() {
  rtc.read();
  String directory = String(rtc.year) + "/" + String(rtc.month) + "/";
  String fileName = String(rtc.day) + "T" + String(rtc.hour) + "-" +
                    String(rtc.minute) + ".csv";

  SD.mkdir(directory);

  filePath = directory + fileName;
  Serial.println(filePath);

  File dataFile = SD.open(filePath, FILE_WRITE);

  if (dataFile) {
    dataFile.close();
  } else {
    Serial.println("error opening " + filePath);
  }
}

void stringToSd(String data) {
  File dataFile = SD.open(filePath, FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  } else {
    Serial.println("error opening " + filePath);
  }
}

void writeHeaders() { stringToSd(headers); }

String getWindSpeed() {
  float sensorValue = analogRead(A0);
  float voltage = (sensorValue / 1023) * 5;
  float windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  Serial.print("Wind Speed =");
  Serial.print(windSpeed);
  Serial.println("m/s");
  Serial.println(" ");
  return String(windSpeed);
}

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void GPSSetup() { ss.begin(GPSBaud); }

String getGPSLatitude() {
  gps.encode(ss.read());
  return String(gps.location.lat());
}

String getGPSLongitude() {
  gps.encode(ss.read());
  return String(gps.location.lng());
}

void PT100Setup() { thermo.begin(MAX31865_4WIRE); }

String getPT100Temperature() {
  uint16_t rtd = thermo.readRTD();
  Serial.print("Temperature = ");
  Serial.print(thermo.temperature(RNOMINAL, RREF));
  Serial.println(" °C");
  PT100Fault();
  return String(thermo.temperature(RNOMINAL, RREF));
}

void PT100Fault() {
  uint8_t fault = thermo.readFault();

  if (fault) {
    Serial.print("Fault 0x");
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    thermo.clearFault();
  }
  Serial.println();
}
