#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

void AM2315CSetup() {
  Serial.println("Adafruit AHT10/AHT20 demo!");

  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1)
      delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
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
