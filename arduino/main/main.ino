// clock module
#include "libraries/GravityRtc/GravityRtc.cpp"
#include "Wire.h"

GravityRtc rtc;  //RTC Initialization

// sd card
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
const String headers = "column1;column2;column3";
String filePath;

void setup() {
  Serial.begin(9600);
  Serial.println();

  sdCardInit();
  clockModuleSetup();
  updateFileName();
  writeHeaders();
}

void loop() {
  for (int i = 0; i < 10; i++) {
    int random1 = random(20);
    int random2 = random(20);
    int random3 = random(20);
    String data = String(random1) + ";" + String(random2) + ";" + String(random3);
    stringToSd(data);
    delay(1000);
  }
}

void sdCardInit() {
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {};
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
  String fileName = String(rtc.day) + "T" + String(rtc.hour) + "-" + String(rtc.minute) + ".csv";

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

void writeHeaders() {
  stringToSd(headers);
}
