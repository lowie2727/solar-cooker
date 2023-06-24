#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void AM2315Setup() {
  if (!am2315.begin()) {
    Serial.println(F("Sensor not found, check wiring & pullups!"));
  }
}

float getAM2315CTemp() {
  float temperature, humidity;
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println(F("Failed to read data from AM2315"));
    return -1.0;
  }
  return temperature;
}
