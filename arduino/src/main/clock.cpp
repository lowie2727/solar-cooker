#include <Arduino.h>

#include "RTClib.h"
#include <SPI.h>

RTC_DS3231 rtc;

void clockSetup() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

String getDateTimeISO() {
  DateTime now = rtc.now();

  char dateTimeISO[100];
  sprintf(dateTimeISO, "%04u-%02u-%02uT%02u:%02u:%02u", now.year(), now.month(),
          now.day(), now.hour(), now.minute(), now.second());

  return String(dateTimeISO);
}

String getDateTimeFile() {
  DateTime now = rtc.now();

  char fileName[100];
  snprintf(fileName, 100, "%02u%02u%02u%02u.csv", now.day(), now.hour(),
           now.minute(), now.second());

  return String(fileName);
}

String getDirectoryName() {
  DateTime now = rtc.now();

  char diretoryName[100];
  snprintf(diretoryName, 100, "%04u/%02u/", now.year(), now.month());

  return String(diretoryName);
}

String getYear() {
  DateTime now = rtc.now();
  return String(now.year());
}

String getMonth() {
  DateTime now = rtc.now();
  char buffer[3];
  sprintf(buffer, "%02d", now.month());
  return String(buffer);
}

String getDay() {
  DateTime now = rtc.now();
  char buffer[3];
  sprintf(buffer, "%02d", now.day());
  return String(buffer);
}

String getHour12() {
  DateTime now = rtc.now();
  char buffer[3];
  if (now.hour() < 12) {
    sprintf(buffer, "%02d AM", now.hour());
  } else {
    sprintf(buffer, "%02d PM", now.hour() - 12);
  }
  return String(buffer);
}

String getHour24() {
  DateTime now = rtc.now();
  char buffer[3];
  sprintf(buffer, "%02d", now.hour());
  return String(buffer);
}

String getMinute() {
  DateTime now = rtc.now();
  char buffer[3];
  sprintf(buffer, "%02d", now.minute());
  return String(buffer);
}

String getSecond() {
  DateTime now = rtc.now();
  char buffer[3];
  sprintf(buffer, "%02d", now.second());
  return String(buffer);
}
