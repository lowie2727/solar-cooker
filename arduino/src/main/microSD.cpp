#include "clockModule.h"
#include <SD.h>

const uint8_t CS_SD = 4;
const String CSVHeaders = "Time [YYYY-MM-DDTHH:MM:ss];Water temperature [°C];Wind speed [m/s];Outside temperature [°C];Air pressure [hPa];Relative humidity [%];Solar irradiance [W/m²]";
String filePath;

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

void updateFileName() {
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

void writeCSVHeaders() {
  stringToSd(CSVHeaders);
}
