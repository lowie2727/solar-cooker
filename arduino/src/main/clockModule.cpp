#include <Arduino.h>
#include <GravityRtc.h>

GravityRtc rtc; // RTC Initialization

void clockModuleSetup() {
  rtc.setup();
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
}

String getDateTimeISO() {
  rtc.read();

  char dateTimeISO[100];
  sprintf(dateTimeISO, "%04u-%02u-%02uT%02u:%02u:%02u", rtc.year, rtc.month,
          rtc.day, rtc.hour, rtc.minute, rtc.second);

  return String(dateTimeISO);
}

String getDateTimeFile() {
  rtc.read();

  char fileName[100];
  snprintf(fileName, 100, "%02u%02u%02u%02u", rtc.day, rtc.hour, rtc.minute,
           rtc.second);

  return String(fileName);
}

String getDirectoryName() {
  rtc.read();

  char diretoryName[100];
  snprintf(diretoryName, 100, "%04u/%02u/", rtc.year, rtc.month);

  return String(diretoryName);
}

uint16_t getYear() {
  rtc.read();
  return rtc.year;
}

uint8_t getMonth() {
  rtc.read();
  return rtc.month;
}

uint8_t getDay() {
  rtc.read();
  return rtc.day;
}

String getHour() {
  rtc.read();
  char buffer[3];
  if (rtc.hour < 12) {
    sprintf(buffer, "%02d AM", rtc.hour);
  } else {
    sprintf(buffer, "%02d PM", rtc.hour);
  }
  return String(buffer);
}

uint8_t getMinute() {
  rtc.read();
  return rtc.minute;
}

uint8_t getSecond() {
  rtc.read();
  return rtc.second;
}
