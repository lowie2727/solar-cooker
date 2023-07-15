#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>

const uint8_t CS_BME680 = 6;
const float SEALEVELPRESSURE_HPA = 1013.25;

Adafruit_BME680 bme(CS_BME680);

void BME680Setup() {
  Serial.println(F("BME680 setup start"));
  if (!bme.begin()) {
    Serial.println(F("Could not find BME680, check wiring"));
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  Serial.println("BME680 setup finished\n");
}

float getBME680Humidity() {
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0;
  }
  return bme.humidity; // %
}

float getBME680Pressure() {
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0;
  }
  return (bme.pressure / 100); // hPa
}

float getBME680Temperature() {
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0;
  }
  return bme.temperature; // °C
}
