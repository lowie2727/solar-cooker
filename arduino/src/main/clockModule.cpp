#include <Arduino.h>
#include <GravityRtc.h>

GravityRtc rtc;  // RTC Initialization

void clockModuleSetup() {
  rtc.setup();
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
}

String getDateTime() {
  rtc.read();

  int array[5] = { rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second };

  String arrayf[5];

  for (uint8_t i = 0; i < 5; i++) {
    char buffer[3];
    sprintf(buffer, "%02d", array[i]);
    arrayf[i] = String(buffer);
  }

  return String(rtc.year) + "-" + arrayf[0] + "-" + arrayf[1] + "T" + arrayf[2] + ":" + arrayf[3] + ":" + arrayf[4];
}

String getDateTimeFile() {
  rtc.read();
  int array[4] = { rtc.day, rtc.hour, rtc.minute, rtc.second };

  String arrayf[4];

  for (uint8_t i = 0; i < 4; i++) {
    char buffer[3];
    sprintf(buffer, "%02d", array[i]);
    arrayf[i] = String(buffer);
  }

  return arrayf[0] + arrayf[1] + arrayf[2] + arrayf[3] + ".csv";
}

String getYear() {
  rtc.read();
  return String(rtc.year);
}

String getMonth() {
  rtc.read();
  char buffer[3];
  sprintf(buffer, "%02d", rtc.month);
  return String(buffer);
}
