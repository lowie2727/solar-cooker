#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void AM2315Setup() {
  if (!am2315.begin()) {
    Serial.println(F("Sensor not found, check wiring & pullups!"));
  }
}

String getAM2315Temp() {
  float temperature, humidity;
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println(F("Failed to read data from AM2315"));
    return String(-1.0);
  }
  return String(temperature);
}
