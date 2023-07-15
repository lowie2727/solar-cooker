#include <Adafruit_MAX31865.h>

const uint8_t CS_Pt100_1 = 11;
const uint8_t CS_Pt100_2 = 12;
const uint8_t CS_Pt100_3 = 13;

Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(CS_Pt100_1);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(CS_Pt100_2);
Adafruit_MAX31865 thermo3 = Adafruit_MAX31865(CS_Pt100_3);

const float RREF = 430.0;
const float RNOMINAL = 100.0;

void Pt100Setup() {
  Serial.println(F("Pt100 setup start"));
  thermo1.begin(MAX31865_4WIRE);
  thermo2.begin(MAX31865_4WIRE);
  thermo2.begin(MAX31865_4WIRE);
  Serial.println(F("Pt100 setup finished\n"));
}

float getPt100Temp1() {
  thermo1.readRTD();
  float temperature = thermo1.temperature(RNOMINAL, RREF);
  return temperature;
}

float getPt100Temp2() {
  thermo2.readRTD();
  float temperature = thermo2.temperature(RNOMINAL, RREF);
  return temperature;
}

float getPt100Temp3() {
  thermo3.readRTD();
  float temperature = thermo3.temperature(RNOMINAL, RREF);
  return temperature;
}

uint8_t getPt100Fault_1() { return thermo1.readFault(); }
uint8_t getPt100Fault_2() { return thermo2.readFault(); }
uint8_t getPt100Fault_3() { return thermo3.readFault(); }
