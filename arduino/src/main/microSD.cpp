/*#include "clock.h"
#include <SD.h>

const uint8_t CS_SD = 4;
char CSVHeaders[] =
    "Year;Month;Day;Hour (12-hour clock);Minute;Second;Outside temperature "
    "[°C];Wind speed [m/s];Air pressure (inside box) [hPa];Relative humidity "
    "(inside box) [%];Temperature inside pot [°C];Solar irradiance [W/m²]";
char filePath[200] = "test.csv";

void microSDSetup() {

  pinMode(53, OUTPUT);
  Serial.print(F("Initializing SD card..."));

  // see if the card is present and can be initialized:
  if (!SD.begin(CS_SD)) {
    Serial.println(F("Card failed, or not present"));
    while (1) {
    };
  }
  Serial.println(F("card initialized."));
  File dataFile = SD.open(filePath, FILE_WRITE);

  if (dataFile) {
    dataFile.close();
  } else {
    Serial.print(F("error opening "));
    Serial.println(filePath);
  }
}

void updateFileName() {
  snprintf(filePath, 200, "%04d/%02d/%02d%02d%02d%02d.csv", getYear(),
           getMonth(), getDay(), getHour24(), getMinute(), getSecond());

  char dirName[200];
  snprintf(dirName, 200, "%04d/%02d/", getYear(), getMonth());

  SD.mkdir(String(getYear()));
  SD.mkdir(dirName);

  File dataFile = SD.open(filePath, FILE_WRITE);

  if (dataFile) {
    dataFile.close();
  } else {
    Serial.print(F("error opening "));
    Serial.println(filePath);
  }
}

void stringToSd(char *data) {
  File dataFile = SD.open(filePath, FILE_WRITE);
  Serial.println(data);
  if (dataFile) {
    dataFile.print(data);
    dataFile.close();
  } else {
    Serial.print(F("error opening "));
    Serial.println(filePath);
  }
}

void stringToSdLn(String data) {
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
*/