#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

void AM2315CSetup() {
  Serial.println("AM2315C setup start");

  if (!aht.begin()) {
    Serial.println(F("Could not find AM2315C, check wiring"));
  }
  Serial.println(F("AM2315C setup finished\n"));
}

float getAM2315CTemp() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  return temp.temperature;
}

float getAM2315CHum() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  return humidity.relative_humidity;
}
