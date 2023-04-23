#include "clockModule.h"
#include <SD.h>


const uint8_t CS_SD = 4;
const String CSVHeaders = "temperaturePT100 (°C)"; // TODO update this
String filePath;

uint8_t fileNameUpdated = 0;

void microSDSetup() {
  Serial.print(F("Initializing SD card..."));

  // see if the card is present and can be initialized:
  if (!SD.begin(CS_SD)) {
    Serial.println(F("Card failed, or not present"));
    while (1) {
    };
  }
  Serial.println(F("card initialized."));
}

void updateFileName() { // make a function in clockModule that gets the time for
                        // the filename
  String directory = getYear() + "/" + getMonth() + "/";
  String fileName = getDateTimeFile();

  SD.mkdir(directory);

  filePath = directory + fileName;
  File dataFile = SD.open(filePath, FILE_WRITE);

  if (dataFile) {
    dataFile.close();
  } else {
    Serial.print(F("error opening "));
    Serial.println(filePath);
  }
}

void stringToSd(String data) {
  File dataFile = SD.open(filePath, FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  } else {
    Serial.print(F("error opening "));
    Serial.println(filePath);
  }
}

void writeCSVHeaders() { stringToSd(CSVHeaders); }
