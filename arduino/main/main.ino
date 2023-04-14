// AM2315
#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

// anemometer
const int ANEMO_PIN = A0;

// BME680
#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

const int CS_BME680 = 6;
const float SEALEVELPRESSURE_HPA = 1013.25;

Adafruit_BME680 bme(CS_BME680);

// clock module
#include "Wire.h"
#include "libraries/GravityRtc/GravityRtc.cpp"

GravityRtc rtc;  // RTC Initialization

// e-Paper
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#define GxEPD2_DISPLAY_CLASS GxEPD2_3C
#define GxEPD2_DRIVER_CLASS GxEPD2_583c_Z83  // 648x480

const int CS_paper = 10;
const int DC_paper = 9;
const int RST_paper = 8;
const int BUSY_paper = 7;

const int MAX_DISPLAY_BUFFER_SIZE = 5000;
#define MAX_HEIGHT(EPD) \
  (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) \
     ? EPD::HEIGHT \
     : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)>
  display(GxEPD2_DRIVER_CLASS(CS_paper, DC_paper, RST_paper, BUSY_paper));

// GPS
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// microSD
#include <SD.h>
#include <SPI.h>

const int CS_SD = 4;
const String CSVHeaders = "temperaturePT100 (°C)";  // TODO update this
String filePath;

int fileNameUpdated = 0;

// PT100
#include <Adafruit_MAX31865.h>

const int CS_PT100 = 11;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(CS_PT100);

const float RREF = 430.0;
const float RNOMINAL = 100.0;

// pyranometer
const int PYRANO_PIN = A1;

// switch
const int SWITCH_PIN = 34;
const int LED_GREEN_PIN = 32;
const int LED_RED_PIN = 30;

unsigned long previousMillis = 0;

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
  // switchSetup();
}

void loop() {
  // int switchState = digitalRead(SWITCH_PIN);
  //  if (switchState) {
  //  digitalWrite(LED_GREEN_PIN, HIGH);
  //  digitalWrite(LED_RED_PIN, LOW);

  /*if (!fileNameUpdated) {
    updateFileName();
    writeCSVHeaders();
    fileNameUpdated = 1;
  }*/

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    Serial.println("AM2315T (°C);Wind speed (m/s);BME680T "
                   "(°C);BME680P (°C);BME680H(°C);PT100T (°C)");

    String AM2315Temp, AM2315Hum;
    getAM2315TempAndHum(&AM2315Temp, &AM2315Hum);
    String windSpeed = getWindSpeed();
    String BME680Temp = getBME680Temperature();
    String BME680Pres = getBME680Pressure();
    String BME680Hum = getBME680Humidity();
    //String dateTime = getDateTime();
    //Serial.println(dateTime);
    // String GPSLat = getGPSLatitude();
    // String GPSLong = getGPSLongitude();
    String PT100Temp = getPT100Temperature();
    // String PyranoIrr = getSolarIrradiance();
    String data = AM2315Temp + ";" + AM2315Hum + ";" + windSpeed + ";" + BME680Temp + ";" + BME680Pres + ";" + BME680Hum + ";" +
                  /*GPSLat + ";" + GPSLong + ";"*/
                  PT100Temp + ";" /* + PyranoIrr */;
    Serial.println(data);
    e_PaperPrint(AM2315Temp, AM2315Hum, windSpeed, BME680Temp, BME680Pres,
                 BME680Hum, PT100Temp);
  }
  /*} else {
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, HIGH);
    fileNameUpdated = 0;
  }*/
}

void AM2315Setup() {
  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
  }
}

void getAM2315TempAndHum(String* temp, String* hum) {
  float temperature, humidity;
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
  }
  *temp = temperature;
  *hum = humidity;
}

String getAM2315Humidity() {
  float temperature, humidity;
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return "error";
  }
  return String(humidity);
}

String getWindSpeed() {
  float sensorValue = analogRead(ANEMO_PIN);
  float voltage = (sensorValue / 1023) * 5;
  float windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  return String(windSpeed);
}

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void BME680Setup() {
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
}

String getBME680Temperature() {
  if (!bme.performReading()) {
    Serial.println("BME680: Failed to perform reading :(");
    return "";
  }
  return String(bme.temperature);  // °C
}

String getBME680Pressure() {
  if (!bme.performReading()) {
    Serial.println("BME680: Failed to perform reading :(");
    return "";
  }
  return String(bme.pressure);  // Pa
}

String getBME680Humidity() {
  if (!bme.performReading()) {
    Serial.println("BME680: Failed to perform reading :(");
    return "";
  }
  return String(bme.humidity);  // %
}

void clockModuleSetup() {
  rtc.setup();
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
}

String getDateTime() {
  rtc.read();

  int array[5] = { rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second };

  String arrayf[5];

  for (int i = 0; i < 5; i++) {
    char buffer[3];
    sprintf(buffer, "%02d", array[i]);
    arrayf[i] = String(buffer);
  }

  return String(rtc.year) + "-" + arrayf[0] + "-" + arrayf[1] + "T" + arrayf[2] + ":" + arrayf[3] + ":" + arrayf[4];
}

void e_PaperSetup() {
  display.init(9600, true, 2, false);
  display.setFullWindow();
  display.clearScreen();
  display.setRotation(2);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
}

void e_PaperPrint(String AM2315Temp, String AM2315Hum, String windSpeed,
                  String BME680Temp, String BME680Pres, String BME680Hum,
                  String PT100Temp) {
  int16_t tbx_line1, tby_line1, tbx_line2, tby_line2, tbx_line3, tby_line3,
    tbx_line4, tby_line4, tbx_line5, tby_line5, tbx_line6, tby_line6,
    tbx_line11, tby_line11, tbx_line21, tby_line21, tbx_line31, tby_line31,
    tbx_line41, tby_line41, tbx_line51, tby_line51;
  uint16_t tbw_line1, tbh_line1, tbw_line2, tbh_line2, tbw_line3, tbh_line3,
    tbw_line4, tbh_line4, tbw_line5, tbh_line5, tbw_line6, tbh_line6,
    tbw_line11, tbh_line11, tbw_line21, tbh_line21, tbw_line31, tbh_line31,
    tbw_line41, tbh_line41, tbw_line51, tbh_line51;

  String line1 = "Water temperature: ";
  String line2 = "Wind speed: ";
  String line3 = "Outside temperature: ";
  String line4 = "Air pressure: ";
  String line5 = "Humidity: ";
  String line6 = "Last updated: ";

  String line11 = PT100Temp + " *C";
  String line21 = windSpeed + " m/s";
  String line31 = AM2315Temp + " *C";
  String line41 = BME680Pres + " Pa";
  String line51 = AM2315Hum + " %";

  display.getTextBounds(line1, 0, 0, &tbx_line1, &tby_line1, &tbw_line1,
                        &tbh_line1);
  display.getTextBounds(line2, 0, 0, &tbx_line2, &tby_line2, &tbw_line2,
                        &tbh_line2);
  display.getTextBounds(line3, 0, 0, &tbx_line3, &tby_line3, &tbw_line3,
                        &tbh_line3);
  display.getTextBounds(line4, 0, 0, &tbx_line4, &tby_line4, &tbw_line4,
                        &tbh_line4);
  display.getTextBounds(line5, 0, 0, &tbx_line5, &tby_line5, &tbw_line5,
                        &tbh_line5);
  display.getTextBounds(line6, 0, 0, &tbx_line6, &tby_line6, &tbw_line6,
                        &tbh_line6);
  display.getTextBounds(line11, 0, 0, &tbx_line11, &tby_line11, &tbw_line11,
                        &tbh_line11);
  display.getTextBounds(line21, 0, 0, &tbx_line21, &tby_line21, &tbw_line21,
                        &tbh_line21);
  display.getTextBounds(line31, 0, 0, &tbx_line31, &tby_line31, &tbw_line31,
                        &tbh_line31);
  display.getTextBounds(line41, 0, 0, &tbx_line41, &tby_line41, &tbw_line41,
                        &tbh_line41);
  display.getTextBounds(line51, 0, 0, &tbx_line51, &tby_line51, &tbw_line51,
                        &tbh_line51);

  uint16_t x = 40;

  uint16_t x11 = (display.width() - tbw_line11 - 40);
  uint16_t x21 = (display.width() - tbw_line21 - 40);
  uint16_t x31 = (display.width() - tbw_line31 - 40);
  uint16_t x41 = (display.width() - tbw_line41 - 40);
  uint16_t x51 = (display.width() - tbw_line51 - 40);

  uint16_t y = ((display.height() - tbh_line1) / 2) - tby_line1;

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y - 40);
    display.print(line1);
    display.setCursor(x, y);
    display.print(line2);
    display.setCursor(x, y + 40);
    display.print(line3);
    display.setCursor(x, y + 80);
    display.print(line4);
    display.setCursor(x, y + 120);
    display.print(line5);
    display.setCursor(x, y + 200);
    display.print(line6);
    display.setCursor(x11, y - 40);
    display.print(line11);
    display.setCursor(x21, y);
    display.print(line21);
    display.setCursor(x31, y + 40);
    display.print(line31);
    display.setCursor(x41, y + 80);
    display.print(line41);
    display.setCursor(x51, y + 120);
    display.print(line51);
  } while (display.nextPage());
}

void GPSSetup() {
  ss.begin(GPSBaud);
}

String getGPSLatitude() {
  gps.encode(ss.read());
  return String(gps.location.lat());
}

String getGPSLongitude() {
  gps.encode(ss.read());
  return String(gps.location.lng());
}

void microSDSetup() {
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(CS_SD)) {
    Serial.println("Card failed, or not present");
    while (1) {
    };
  }
  Serial.println("card initialized.");
}

void updateFileName() {
  rtc.read();
  int year = rtc.year;
  int month = rtc.month;
  int day = rtc.day;
  int hour = rtc.hour;
  int minute = rtc.minute;
  int second = rtc.second;

  int array[5] = { month, day, hour, minute, second };

  String arrayf[5] = { "", "", "", "", "" };

  for (int i = 0; i < 5; i++) {
    char buffer[3];
    sprintf(buffer, "%02d", array[i]);
    arrayf[i] = String(buffer);
  }

  String directory = String(year) + "/" + arrayf[0] + "/";
  String fileName = arrayf[1] + arrayf[2] + arrayf[3] + arrayf[4] + ".csv";

  SD.mkdir(directory);

  filePath = directory + fileName;
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

void writeCSVHeaders() {
  stringToSd(CSVHeaders);
}

void PT100Setup() {
  thermo.begin(MAX31865_4WIRE);
}

String getPT100Temperature() {
  uint16_t rtd = thermo.readRTD();
  float temperature = thermo.temperature(RNOMINAL, RREF);
  PT100Fault();
  return String(temperature);
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

String getSolarIrradiance() {
  int pyranoValue = analogRead(PYRANO_PIN);
  float pyranoVoltage = pyranoValue * (5.0 / 1023.0);
  float pyranoIrradiance = pyranoVoltage * 20.0;
  return String(pyranoIrradiance);
}

void switchSetup() {
  pinMode(SWITCH_PIN, INPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
}
