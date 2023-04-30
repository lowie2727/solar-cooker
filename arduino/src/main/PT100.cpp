#include <Adafruit_MAX31865.h>

const uint8_t CS_PT100 = 11;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(CS_PT100);

const float RREF = 430.0;
const float RNOMINAL = 100.0;

void PT100Fault();

void PT100Setup() { thermo.begin(MAX31865_4WIRE); }

float getPT100Temperature() {
  thermo.readRTD();
  float temperature = thermo.temperature(RNOMINAL, RREF);
  PT100Fault();
  return temperature;
}

void PT100Fault() {
  uint8_t fault = thermo.readFault();

  if (fault) {
    Serial.print(F("Fault 0x"));
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println(F("RTD High Threshold"));
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println(F("RTD Low Threshold"));
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println(F("REFIN- > 0.85 x Bias"));
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println(F("REFIN- < 0.85 x Bias - FORCE- open"));
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println(F("RTDIN- < 0.85 x Bias - FORCE- open"));
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println(F("Under/Over voltage"));
    }
    thermo.clearFault();
  }
  Serial.println();
}
