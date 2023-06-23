#include "clock.h"
#include <SD.h>

const uint8_t CS_SD = 4;
const String CSVHeaders =
    "Year;Month;Day;Hour (12-hour clock);Minute;Second;Outside temperature "
    "[°C];Wind speed [m/s];Air pressure (inside box) [hPa];Relative humidity "
    "(inside box) [%];Temperature inside pot [°C];Solar irradiance [W/m²]";
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
  String directoryName = getDirectoryName();
  String fileName = getDateTimeFile();

  SD.mkdir(getYear());
  SD.mkdir(directoryName);

  filePath = directoryName + fileName;
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
