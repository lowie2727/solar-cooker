#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_MAX31865.h>
#include <Adafruit_AM2315.h>

#define BME_SCK 52
#define BME_MISO 50
#define BME_MOSI 51
#define BME_CS 6

#define SEALEVELPRESSURE_HPA (1013.25)
#define RREF 430.0
#define RNOMINAL 100.0

Adafruit_AM2315 am2315;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(11); // CS
//Adafruit_BME680 bme; // I2C
Adafruit_BME680 bme(BME_CS);  // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1)
      ;
  }

  // Set up oversampling and filter initialization BME680
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms

  //Set up pt100
  thermo.begin(MAX31865_4WIRE);

  // begin() does a test read, so need to wait 2secs before first read
  delay(2000);
}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  float sensorValue = analogRead(A0);
  float voltage = (sensorValue / 1023) * 5;
  float windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  uint16_t rtd = thermo.readRTD();
  float temperature, humidity;
  
  //Serial.print("Temperature = ");
  //Serial.print(bme.temperature);
  //Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Wind Speed = ");
  Serial.print(windSpeed);
  Serial.print(" ");
  Serial.println("m/s");

  //Check and any defaults of the AM2315
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return;
  }

  Serial.print("Temperature outside = ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Temperature in cooker = ");
  Serial.print(thermo.temperature(RNOMINAL, RREF));
  Serial.println(" °C");
  
  //Check and print any defaults of the pt100
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x");
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    thermo.clearFault();
  }

  Serial.println();
  delay(2000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
