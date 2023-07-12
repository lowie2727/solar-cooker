#include <SdFat.h>

#include "clock.h"

SdFat sd;
SdFile myFile;

const uint8_t CS_SD = 4;
char CSVHeaders[] =
    "Year;Month;Day;Hour (12-hour clock);Minute;Second;Outside temperature "
    "[°C];Wind speed [m/s];Air pressure (inside box) [hPa];Relative humidity "
    "(inside box) [%];Temperature inside pot 1 [°C];Temperature inside pot 2 "
    "[°C];Solar irradiance [W/m²]";
char filePath[200];

void microSDSetup() {
  Serial.print(F("Initializing SD card..."));

  if (!sd.begin(CS_SD, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  Serial.println(F("Initialized"));
}

void updateFileName() {
  snprintf(filePath, 200, "%04d/%02d/%02d%02d%02d%02d.csv", getYear(),
           getMonth(), getDay(), getHour24(), getMinute(), getSecond());

  char dirName[200];
  snprintf(dirName, 200, "%04d/%02d/", getYear(), getMonth());

  sd.mkdir(String(getYear()));
  sd.mkdir(dirName);

  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening file for write failed");
  }
  myFile.close();
}

void stringToSd(char *data) {
  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening file for write failed");
  }
  myFile.println(data);
  myFile.close();
}

void writeCSVHeaders() { stringToSd(CSVHeaders); }
