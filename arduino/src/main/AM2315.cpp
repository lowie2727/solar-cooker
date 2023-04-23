#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void AM2315Setup() {
  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
  }
}

void getAM2315TempAndHum(String *temp, String *hum) {
  float temperature, humidity;
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println(F("Failed to read data from AM2315"));
  }
  *temp = temperature;
  *hum = humidity;
}
