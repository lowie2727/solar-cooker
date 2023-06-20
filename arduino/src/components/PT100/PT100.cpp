#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MAX31865.h>

Adafruit_MAX31865 thermo = Adafruit_MAX31865(11); // CS

#define RREF 430.0
#define RNOMINAL 100.0

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  thermo.begin(MAX31865_4WIRE);
}

void loop() {
  thermo.readRTD();
  Serial.print("Temperature = ");
  Serial.print(thermo.temperature(RNOMINAL, RREF));
  Serial.println(" °C");

  // Check and print any faults
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
  delay(1000);
}
